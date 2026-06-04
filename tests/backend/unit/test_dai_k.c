#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Sint16 time_stop;
Uint8 chibi_flag;
ushort_union swdata;
ushort_union swdata1;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ride_on_chk_count;
static sprite_status *ride_on_platform;
static sprite_status *ride_on_player;
static Sint16 ride_on_chk_result;
static int sinset_count;
static Uint8 sinset_angles[8];
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void soundset(Sint16 ReqNo);

#define R11A
#include "src/dai_k.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_platform = pActwk;
    ride_on_player = pPlayerwk;
    return ride_on_chk_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < 8) {
        sinset_angles[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_dai_k_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_stop = 0;
    chibi_flag = 0;
    memset(&swdata, 0, sizeof(swdata));
    memset(&swdata1, 0, sizeof(swdata1));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ride_on_chk_count = 0;
    ride_on_platform = 0;
    ride_on_player = 0;
    ride_on_chk_result = 0;
    sinset_count = 0;
    memset(sinset_angles, 0, sizeof(sinset_angles));
    sinset_sin = 0;
    sinset_cos = 256;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_dai_k_initializes_and_runs_common_callbacks(test_context *ctx) {
    sprite_status *platform = &actwk[4];

    reset_dai_k_state();
    platform->xposi.w.h = 1000;

    dai_k(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == platform);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)&k_daichg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
}

static void test_dai_k_skips_animation_when_time_is_stopped(test_context *ctx) {
    sprite_status *platform = &actwk[4];

    reset_dai_k_state();
    platform->r_no0 = 2;
    time_stop = 1;

    dai_k(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_dai_k_move_waits_for_visible_and_ride_state(test_context *ctx) {
    sprite_status *platform = &actwk[4];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 0;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);

    platform->actflg = 128;
    ride_on_chk_result = 0;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_platform == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
}

static void test_dai_k_move_starts_player_orbit_on_first_ride(test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 1032;
    player->r_no0 = 0;
    ride_on_chk_result = 1;
    swdata1.b.h = 8;
    sinset_sin = 0;
    sinset_cos = 256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 8, swdata.b.h);
}

static void test_dai_k_move_starts_counterclockwise_orbit_from_left(
    test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 960;
    player->r_no0 = 0;
    ride_on_chk_result = 1;
    sinset_sin = 0;
    sinset_cos = -256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 136, sinset_angles[0]);
}

static void assert_k_move_visible_radius(test_context *ctx, Sint16 player_start,
                                         Uint8 input, Uint8 initial_radius,
                                         Sint16 expected_x) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = player_start;
    player->r_no0 = 0;
    player_work_get(player)->status_flags = 1;
    player_work_get(player)->orbit_radius = initial_radius;
    ride_on_chk_result = 1;
    sinset_cos = 256;
    swdata.b.h = input;

    k_move(platform, player);
    dai_k_move(platform);

}

static void test_k_move_changes_radius_by_side_and_input(test_context *ctx) {
    assert_k_move_visible_radius(ctx, 1030, 8, 10, 1011);
    assert_k_move_visible_radius(ctx, 1030, 4, 10, 1009);
    assert_k_move_visible_radius(ctx, 1030, 4, 0, 1000);
    assert_k_move_visible_radius(ctx, 970, 4, 10, 1011);
    assert_k_move_visible_radius(ctx, 970, 8, 10, 1009);
    assert_k_move_visible_radius(ctx, 970, 8, 0, 1000);
}

static void test_dai_k_move_increments_radius_on_64_step_angle(
    test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->r_no0 = 0;
    player_work_get(player)->special_angle = 56;
    player_work_get(player)->status_flags = 1;
    player_work_get(player)->orbit_radius = 10;
    ride_on_chk_result = 1;
    sinset_cos = 256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_angles[0]);
}

static void test_dai_k_move_keeps_existing_orbit_when_player_state_is_late(
    test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    player->r_no0 = 6;
    player_work_get(player)->status_flags = 1;
    ride_on_chk_result = 1;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
}

static void test_dai_k_jump_uses_normal_player_dimensions(test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 1008;
    player->r_no0 = 0;
    player->direc.b.h = 64;
    ride_on_chk_result = 1;
    swdata1.b.l = 112;
    sinset_sin = 0;
    sinset_cos = 256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angles[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
}

static void test_dai_k_jump_uses_normal_ducking_dimensions(test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 1008;
    player->r_no0 = 0;
    player->cddat = 4;
    player->direc.b.h = 64;
    ride_on_chk_result = 1;
    swdata1.b.l = 112;
    sinset_sin = 0;
    sinset_cos = 256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
}

static void test_dai_k_jump_uses_chibi_standing_dimensions(test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 1008;
    player->r_no0 = 0;
    player->direc.b.h = 64;
    chibi_flag = 1;
    ride_on_chk_result = 1;
    swdata1.b.l = 112;
    sinset_sin = 0;
    sinset_cos = 256;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
}

static void test_dai_k_jump_uses_chibi_ducking_dimensions(test_context *ctx) {
    sprite_status *platform = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_dai_k_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->cddat = 64;
    platform->xposi.w.h = 1000;
    player->xposi.w.h = 1008;
    player->r_no0 = 0;
    player->cddat = 4;
    player->direc.b.h = 64;
    chibi_flag = 1;
    ride_on_chk_result = 1;
    swdata1.b.l = 112;
    sinset_sin = -256;
    sinset_cos = 0;

    dai_k_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
}

TEST_MAIN_BEGIN;
test_dai_k_initializes_and_runs_common_callbacks(&ctx);
test_dai_k_skips_animation_when_time_is_stopped(&ctx);
test_dai_k_move_waits_for_visible_and_ride_state(&ctx);
test_dai_k_move_starts_player_orbit_on_first_ride(&ctx);
test_dai_k_move_starts_counterclockwise_orbit_from_left(&ctx);
test_k_move_changes_radius_by_side_and_input(&ctx);
test_dai_k_move_increments_radius_on_64_step_angle(&ctx);
test_dai_k_move_keeps_existing_orbit_when_player_state_is_late(&ctx);
test_dai_k_jump_uses_normal_player_dimensions(&ctx);
test_dai_k_jump_uses_normal_ducking_dimensions(&ctx);
test_dai_k_jump_uses_chibi_standing_dimensions(&ctx);
test_dai_k_jump_uses_chibi_ducking_dimensions(&ctx);
TEST_MAIN_END;

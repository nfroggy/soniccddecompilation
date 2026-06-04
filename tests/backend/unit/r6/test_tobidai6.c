#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
Uint8 clchgcnt[7];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;
static Sint16 ride_on_chk_result;
static int emycol_u_count;
static int emycol_d_count;
static Sint16 emycol_u_result;
static Sint16 emycol_d_result;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 emycol_u(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/r6/tobidai6.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return ride_on_chk_result;
}

Sint16 emycol_u(sprite_status *pActwk) {
    ++emycol_u_count;
    (void)pActwk;
    return emycol_u_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    (void)pActwk;
    return emycol_d_result;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_tobidai6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    memset(clchgcnt, 0, sizeof(clchgcnt));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
    ride_on_chk_result = 0;
    emycol_u_count = 0;
    emycol_d_count = 0;
    emycol_u_result = 0;
    emycol_d_result = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_tobidai6_init_sets_platform_layout(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();

    tobidai6_init(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17384, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == tobidai6pat);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 32, platform->sprhsize);
}

static void test_yuka_chk_matches_time_and_color_counter_gates(
    test_context *ctx) {
    reset_tobidai6_state();

    TEST_ASSERT_EQ_INT(ctx, 0, yuka_chk());

    clchgcnt[3] = 60;
    TEST_ASSERT_EQ_INT(ctx, -1, yuka_chk());

    time_flag = 1;
    clchgcnt[3] = 60;
    TEST_ASSERT_EQ_INT(ctx, 0, yuka_chk());

    clchgcnt[3] = 90;
    TEST_ASSERT_EQ_INT(ctx, -1, yuka_chk());

    time_flag = 2;
    TEST_ASSERT_EQ_INT(ctx, 0, yuka_chk());
}

static void test_tobidai6_move0_launches_when_yuka_allows_it(
    test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;

    tobidai6_move0(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1536, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
}

static void test_tobidai6_move0_waits_when_yuka_is_locked(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    clchgcnt[3] = 60;

    tobidai6_move0(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_tobidai6_move1_handles_upward_collision(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    platform->yposi.l = 100 << 16;
    tobidai6_move0(platform);
    emycol_u_result = -2;

    tobidai6_move1(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_u_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + (-1520 << 8), platform->yposi.l);
}

static void test_tobidai6_move1_bottom_collision_relaunches_with_sound(
    test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    platform->actflg = 128;
    tobidai6_move0(platform);
    emycol_d_result = -1;

    tobidai6_move1(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1536, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 180, soundset_requests[0]);
}

static void test_tobidai6_move1_bottom_collision_enters_fall_when_locked(
    test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    emycol_d_result = -1;
    clchgcnt[3] = 60;

    tobidai6_move1(platform);

    TEST_ASSERT_EQ_INT(ctx, 6, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -384, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_tobidai6_move2_lands_when_descending(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    platform->r_no0 = 6;
    platform->yspeed.w = -16;
    emycol_d_result = -1;

    tobidai6_move2(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
}

static void test_tobidai6_move2_skips_floor_check_while_rising(
    test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    platform->yspeed.w = -100;

    tobidai6_move2(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -84, platform->yspeed.w);
}

static void test_tobidai6_speedset_clamps_positive_speed(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    platform->yspeed.w = 1530;
    platform->yposi.l = 100 << 16;

    tobidai6_speedset(platform);

    TEST_ASSERT_EQ_INT(ctx, 1536, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + (1536 << 8), platform->yposi.l);
}

static void test_yuka_ridechk_adjusts_collision_height(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->yspeed.w = -10;
    platform->sprvsize = 16;
    actwk[0].yspeed.w = -100;

    yuka_ridechk(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, tobidai6_get_work(platform)->ride_height_adjust);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_tobidai6_state();
    platform->yspeed.w = 0;
    platform->sprvsize = 16;
    actwk[0].yspeed.w = -2000;

    yuka_ridechk(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprvsize);
}

static void test_chk_restores_player_speed_when_not_riding(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    actwk[0].yspeed.w = 123;

    chk(platform, &actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].yspeed.w);
}

static void test_chk_zeroes_flagged_player_before_ride_failure(
    test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    actwk[0].yspeed.w = 123;
    actwk[0].cddat = 8;

    chk(platform, &actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].yspeed.w);

    actwk[0].cddat = 10;
    chk(platform, &actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].yspeed.w);
}

static void test_chk_calls_ride_on_when_collision_succeeds(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->yposi.l = 200 << 16;
    platform->yspeed.w = -64;
    actwk[0].sprvsize = 16;
    actwk[0].yspeed.w = 200;
    ride_on_chk_result = 1;

    chk(platform, &actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 168, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -64, actwk[0].yspeed.w);
}

static void test_yuka_ride_on_places_or_stops_player(test_context *ctx) {
    sprite_status *platform = &actwk[6];
    sprite_status *player = &actwk[0];

    reset_tobidai6_state();
    platform->yposi.l = 200 << 16;
    platform->yspeed.w = -300;
    player->sprvsize = 24;
    player->yposi.l = 999 << 16;
    player->yspeed.w = 10;

    yuka_ride_on(platform, player);

    TEST_ASSERT_EQ_INT(ctx, 160, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -300, player->yspeed.w);

    platform->yspeed.w = 50;
    player->yspeed.w = 10;

    yuka_ride_on(platform, player);

    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);

    player->r_no0 = 6;
    player->yspeed.w = 10;

    yuka_ride_on(platform, player);

    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);
}

static void test_tobidai6_entry_dispatches_callbacks(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();

    tobidai6(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == platform);
}

static void test_tobidai6_entry_dispatches_active_states(test_context *ctx) {
    sprite_status *platform = &actwk[6];

    reset_tobidai6_state();
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    platform->r_no0 = 4;

    tobidai6(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_tobidai6_state();
    platform = &actwk[6];
    platform->r_no0 = 2;
    tobidai6_move0(platform);
    platform->r_no0 = 6;
    platform->yspeed.w = -16;
    emycol_d_result = -1;

    tobidai6(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_tobidai6_init_sets_platform_layout(&ctx);
    test_yuka_chk_matches_time_and_color_counter_gates(&ctx);
    test_tobidai6_move0_launches_when_yuka_allows_it(&ctx);
    test_tobidai6_move0_waits_when_yuka_is_locked(&ctx);
    test_tobidai6_move1_handles_upward_collision(&ctx);
    test_tobidai6_move1_bottom_collision_relaunches_with_sound(&ctx);
    test_tobidai6_move1_bottom_collision_enters_fall_when_locked(&ctx);
    test_tobidai6_move2_lands_when_descending(&ctx);
    test_tobidai6_move2_skips_floor_check_while_rising(&ctx);
    test_tobidai6_speedset_clamps_positive_speed(&ctx);
    test_yuka_ridechk_adjusts_collision_height(&ctx);
    test_chk_restores_player_speed_when_not_riding(&ctx);
    test_chk_zeroes_flagged_player_before_ride_failure(&ctx);
    test_chk_calls_ride_on_when_collision_succeeds(&ctx);
    test_yuka_ride_on_places_or_stops_player(&ctx);
    test_tobidai6_entry_dispatches_callbacks(&ctx);
    test_tobidai6_entry_dispatches_active_states(&ctx);
TEST_MAIN_END

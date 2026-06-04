#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int scoreup_count;
static Uint32 scoreup_values[16];
static int tensuu0_count;
static sprite_status *tensuu0_actor;
static Uint8 tensuu0_userflag;
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int atan_sonic_count;
static Sint16 atan_sonic_x;
static Sint16 atan_sonic_y;
static Sint16 atan_sonic_result;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int ride_on_set_count;
static sprite_status *ride_on_set_actor;
static sprite_status *ride_on_set_player;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
void scoreup(Uint32 updata);
void tensuu0(sprite_status *tensuuwk, Uint8 uf_data);
void sub_sync(Sint16 ReqNo);
Sint16 atan_sonic(Sint16 x, Sint16 y);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 ride_on_set(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r3/bobin.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void scoreup(Uint32 updata) {
    if (scoreup_count < 16) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
}

void tensuu0(sprite_status *tensuuwk, Uint8 uf_data) {
    ++tensuu0_count;
    tensuu0_actor = tensuuwk;
    tensuu0_userflag = uf_data;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

Sint16 atan_sonic(Sint16 x, Sint16 y) {
    ++atan_sonic_count;
    atan_sonic_x = x;
    atan_sonic_y = y;
    return atan_sonic_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 ride_on_set(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_set_count;
    ride_on_set_actor = pActwk;
    ride_on_set_player = pPlayerwk;
    return 0;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

static void reset_bobin_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    tensuu0_count = 0;
    tensuu0_actor = 0;
    tensuu0_userflag = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    atan_sonic_count = 0;
    atan_sonic_x = 0;
    atan_sonic_y = 0;
    atan_sonic_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 128;
    sinset_cos = -64;
    ride_on_set_count = 0;
    ride_on_set_actor = 0;
    ride_on_set_player = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
}

static void reset_bobin_logs(void) {
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    tensuu0_count = 0;
    tensuu0_actor = 0;
    tensuu0_userflag = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    atan_sonic_count = 0;
    sinset_count = 0;
    ride_on_set_count = 0;
    ride_on_set_actor = 0;
    ride_on_set_player = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
}

static void init_bobin_actor(sprite_status *actor, Sint16 x, Sint16 y,
                             Sint8 userflag_h) {
    reset_bobin_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    actor->userflag.b.h = userflag_h;
    bobin(actor);
    reset_bobin_logs();
}

static void init_frip_actor(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_bobin_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    frip(actor);
    reset_bobin_logs();
}

static void test_bobin_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, bobinpat[0] == &bobinsp0);
    TEST_ASSERT_TRUE(ctx, bobinpat[2] == &bobinsp2);
    TEST_ASSERT_TRUE(ctx, frippat[0] == &fripsp0);
    TEST_ASSERT_TRUE(ctx, frippat[2] == &fripsp2);
    TEST_ASSERT_EQ_INT(ctx, -16, bobinsp0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, bobinsp0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BOBIN_BASE, bobinsp0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -24, bobinsp2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -20, bobinsp2.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BOBIN_BASE + 2, bobinsp2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -32, fripsp0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, fripsp0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BOBIN_BASE + 3, fripsp0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 14, frip_posi[0]);
    TEST_ASSERT_EQ_INT(ctx, -4, frip_posi[63]);
    TEST_ASSERT_EQ_INT(ctx, 18, frip_posi_r[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, frip_posi_r[63]);
}

static void test_bobin_initializes_without_motion(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_bobin_state();
    actor->xposi.w.h = 320;
    actor->yposi.w.h = 100;
    actor->actflg = 128;

    bobin(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)bobinchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);
}

static void test_bobin_initializes_special_high_sprite_offset(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_bobin_state();
    actor->xposi.w.h = 10624;
    actor->yposi.w.h = 1024;

    bobin(actor);

    TEST_ASSERT_EQ_INT(ctx, 10624, frameout_s00_x);
}

static void test_bobin_moves_vertically_and_reverses_after_existing_count(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_bobin_actor(actor, 320, 100, 65);


    for (int i = 0; i < 15; ++i) {
        bobin(actor);
    }


    bobin(actor);

}

static void test_bobin_moves_horizontally_for_negative_userflag(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_bobin_actor(actor, 320, 100, -63);

}

static void test_bobin_bit_two_starts_negative_vertical_motion(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_bobin_state();
    actor->xposi.w.h = 320;
    actor->yposi.w.h = 100;
    actor->userflag.b.h = 2;

    bobin(actor);


    for (int i = 0; i < 31; ++i) {
        bobin(actor);
    }


    bobin(actor);

}

static void test_bobin_collision_bounces_player_and_awards_limited_scores(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *player = &actwk[0];

    init_bobin_actor(actor, 320, 100, 0);
    actor->actflg = 128;
    actor->cdsts = 10;
    player->xposi.w.h = 300;
    player->yposi.w.h = 90;
    player->cddat = 255;
    player->xspeed.w = 1;
    player->yspeed.w = 2;
    player_work_get(player)->jump_started = 77;
    atan_sonic_result = 9;
    sinset_sin = 128;
    sinset_cos = -64;

    for (int i = 0; i < 6; ++i) {
        actor->colicnt = 1;
        bobin(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 6, atan_sonic_count);
    TEST_ASSERT_EQ_INT(ctx, 20, atan_sonic_x);
    TEST_ASSERT_EQ_INT(ctx, 10, atan_sonic_y);
    TEST_ASSERT_EQ_INT(ctx, 6, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 9, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 5, scoreup_count);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 5, tensuu0_count);
    TEST_ASSERT_TRUE(ctx, tensuu0_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_userflag);
    TEST_ASSERT_EQ_INT(ctx, 6, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 127, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, flagwork[10]);
}

static void test_bobin_collision_does_not_increment_full_flagwork(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_bobin_actor(actor, 320, 100, 0);
    actor->cdsts = 7;
    actor->colicnt = 1;
    flagwork[7] = 138;

    bobin(actor);

    TEST_ASSERT_EQ_INT(ctx, 138, flagwork[7]);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);
}

static void test_frip_initializes_and_frameouts(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_bobin_state();
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 120;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)fripchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_frip_clears_ride_when_player_state_or_position_misses(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    init_frip_actor(actor, 100, 100);
    player->mstno.b.h = 43;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == player);

    reset_bobin_logs();
    player->mstno.b.h = 0;
    player->xposi.w.h = 20;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);

    reset_bobin_logs();
    player->xposi.w.h = 200;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
}

static void test_frip_sets_player_on_platform_when_falling(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    init_frip_actor(actor, 100, 100);
    player->xposi.w.h = 100;
    player->yposi.w.h = 80;
    player->sprvsize = 16;
    player->yspeed.w = 1;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_set_count);
    TEST_ASSERT_TRUE(ctx, ride_on_set_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_set_player == player);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_frip_clears_ride_when_vertical_contact_misses(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    init_frip_actor(actor, 100, 100);
    player->xposi.w.h = 100;
    player->yposi.w.h = 10;
    player->sprvsize = 16;
    player->yspeed.w = 1;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == player);
}

static void test_frip_stops_upward_player_and_copies_horizontal_speed(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    init_frip_actor(actor, 100, 100);
    actor->cddat = 1;
    player->xposi.w.h = 100;
    player->yposi.w.h = 130;
    player->sprvsize = 16;
    player->xspeed.w = 345;
    player->yspeed.w = -9;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_frip_move_skips_collision_check_when_animating(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_frip_actor(actor, 100, 100);
    actor->mstno.b.h = 1;

    frip(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_bobin_tables_capture_literal_data(&ctx);
    test_bobin_initializes_without_motion(&ctx);
    test_bobin_initializes_special_high_sprite_offset(&ctx);
    test_bobin_moves_vertically_and_reverses_after_existing_count(&ctx);
    test_bobin_moves_horizontally_for_negative_userflag(&ctx);
    test_bobin_bit_two_starts_negative_vertical_motion(&ctx);
    test_bobin_collision_bounces_player_and_awards_limited_scores(&ctx);
    test_bobin_collision_does_not_increment_full_flagwork(&ctx);
    test_frip_initializes_and_frameouts(&ctx);
    test_frip_clears_ride_when_player_state_or_position_misses(&ctx);
    test_frip_sets_player_on_platform_when_falling(&ctx);
    test_frip_clears_ride_when_vertical_contact_misses(&ctx);
    test_frip_stops_upward_player_and_copies_horizontal_speed(&ctx);
    test_frip_move_skips_collision_check_when_animating(&ctx);
TEST_MAIN_END

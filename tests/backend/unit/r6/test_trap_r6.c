#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r6/trap_r6.c"

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
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_optbr6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_optbr6_init_sets_layout_and_direction(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;

    optbr6_init(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, bridge->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17424, bridge->sproffset);
    TEST_ASSERT_TRUE(ctx, bridge->patbase == optbr6pat);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, bridge->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_optbr6_state();
    bridge = &actwk[4];
    bridge->userflag.b.h = 1;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 200;

    optbr6_init(bridge);

    TEST_ASSERT_EQ_INT(ctx, 5, bridge->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, bridge->cddat);
}

static void test_optbr6_chk1_opens_when_player_enters_from_front(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->actflg = 128;
    bridge->r_no0 = 2;
    actwk[0].xposi.w.h = 70;
    actwk[0].yposi.w.h = 220;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 4, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 164, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_optbr6_chk1_waits_outside_trigger_window(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 2;
    actwk[0].xposi.w.h = 130;
    actwk[0].yposi.w.h = 300;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_optbr6_state();
    bridge = &actwk[4];
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 2;
    actwk[0].yposi.w.h = 200;
    actwk[0].xposi.w.h = 110;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);

    reset_optbr6_state();
    bridge = &actwk[4];
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 2;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
}

static void test_optbr6_chk1_reversed_direction_uses_opposite_side(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->userflag.b.h = 1;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 2;
    actwk[0].xposi.w.h = 130;
    actwk[0].yposi.w.h = 200;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 4, bridge->r_no0);
}

static void test_optbr6_mov1_counts_frames_and_reaches_check2(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->r_no0 = 4;
    bridge->patno = 2;
    optbr6_get_work(bridge)->animation_counter_low = 128;

    optbr6_mov1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 3, bridge->patno);
    TEST_ASSERT_EQ_INT(ctx, 6, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
}

static void test_optbr6_chk2_closes_after_player_exits(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 6;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;

    optbr6_chk2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 8, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 164, soundset_requests[0]);

    reset_optbr6_state();
    bridge = &actwk[4];
    bridge->userflag.b.h = 1;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 6;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 200;

    optbr6_chk2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 8, bridge->r_no0);
}

static void test_optbr6_chk2_waits_inside_or_outside_y_window(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 6;
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 200;

    optbr6_chk2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 6, bridge->r_no0);

    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 300;

    optbr6_chk2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 6, bridge->r_no0);
}

static void test_optbr6_rejects_far_and_wrong_side_edges(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 200;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 2;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    optbr6_chk1(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);

    reset_optbr6_state();
    bridge = &actwk[4];
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    bridge->r_no0 = 6;
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 200;

    optbr6_chk2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 6, bridge->r_no0);
}

static void test_optbr6_mov2_counts_down_and_returns_to_check1(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->r_no0 = 8;
    bridge->patno = 1;
    optbr6_get_work(bridge)->animation_counter_low = 128;

    optbr6_mov2(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, bridge->patno);
    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_tobira_cnt_waits_until_counter_wraps_negative(
    test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->patno = 2;
    optbr6_get_work(bridge)->animation_counter_low = 0;

    tobira_cnt(bridge);

    TEST_ASSERT_EQ_INT(ctx, 64, optbr6_get_work(bridge)->animation_counter_low);
    TEST_ASSERT_EQ_INT(ctx, 2, bridge->patno);
}

static void test_optbr6_ride_check_skips_when_not_solid(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->patno = 1;

    optbr6_ridechk(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
}

static void test_optbr6_entry_dispatches_callbacks(test_context *ctx) {
    sprite_status *bridge = &actwk[4];

    reset_optbr6_state();
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 200;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;

    optbr6(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bridge);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == bridge);
}

TEST_MAIN_BEGIN;
    test_optbr6_init_sets_layout_and_direction(&ctx);
    test_optbr6_chk1_opens_when_player_enters_from_front(&ctx);
    test_optbr6_chk1_waits_outside_trigger_window(&ctx);
    test_optbr6_chk1_reversed_direction_uses_opposite_side(&ctx);
    test_optbr6_mov1_counts_frames_and_reaches_check2(&ctx);
    test_optbr6_chk2_closes_after_player_exits(&ctx);
    test_optbr6_chk2_waits_inside_or_outside_y_window(&ctx);
    test_optbr6_rejects_far_and_wrong_side_edges(&ctx);
    test_optbr6_mov2_counts_down_and_returns_to_check1(&ctx);
    test_tobira_cnt_waits_until_counter_wraps_negative(&ctx);
    test_optbr6_ride_check_skips_when_not_solid(&ctx);
    test_optbr6_entry_dispatches_callbacks(&ctx);
TEST_MAIN_END

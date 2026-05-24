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

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r6/udblk6.c"

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

static void reset_udblk6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
}

static void test_udblk6_init_sets_block_layout(test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->yposi.w.h = 240;

    udblk6_init(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, block->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, block->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17640, block->sproffset);
    TEST_ASSERT_TRUE(ctx, block->patbase == udblk6pat);
    TEST_ASSERT_EQ_INT(ctx, 80, block->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 48, block->sprhsize);
}

static void test_udblk6_chk1_triggers_when_player_is_left_and_near(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 300;
    block->r_no0 = 2;
    block->actfree[18] = 99;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    udblk6_chk1(block);

    TEST_ASSERT_EQ_INT(ctx, 4, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, block->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == block);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
}

static void test_udblk6_chk1_rejects_vertical_negative_and_far_x(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 100;
    block->r_no0 = 2;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 300;

    udblk6_chk1(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);

    actwk[0].yposi.w.h = 100;
    actwk[0].xposi.w.h = 300;

    udblk6_chk1(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);

    actwk[0].xposi.w.h = 0;

    udblk6_chk1(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);
}

static void test_udblk6_chk2_triggers_when_player_exits_right(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 300;
    block->r_no0 = 6;
    actwk[0].xposi.w.h = 340;
    actwk[0].yposi.w.h = 300;

    udblk6_chk2(block);

    TEST_ASSERT_EQ_INT(ctx, 8, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_udblk6_chk2_rejects_inside_or_wrong_side(test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 300;
    block->r_no0 = 6;
    actwk[0].xposi.w.h = 250;
    actwk[0].yposi.w.h = 300;

    udblk6_chk2(block);

    TEST_ASSERT_EQ_INT(ctx, 6, block->r_no0);

    actwk[0].xposi.w.h = 150;

    udblk6_chk2(block);

    TEST_ASSERT_EQ_INT(ctx, 6, block->r_no0);

    actwk[0].xposi.w.h = 340;
    actwk[0].yposi.w.h = 100;

    udblk6_chk2(block);

    TEST_ASSERT_EQ_INT(ctx, 6, block->r_no0);
}

static void test_udblk6_chk2_handles_player_below_block(test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 100;
    block->r_no0 = 6;
    actwk[0].xposi.w.h = 340;
    actwk[0].yposi.w.h = 300;

    udblk6_chk2(block);

    TEST_ASSERT_EQ_INT(ctx, 6, block->r_no0);
}

static void test_udblk6_mov1_initializes_and_finishes_two_phase_motion(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->r_no0 = 4;
    block->yposi.l = 300 << 16;

    udblk6_mov1(block);

    TEST_ASSERT_EQ_INT(ctx, 64, block->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, block->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, block->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 4, block->r_no0);

    block->actfree[16] = 1;
    block->actfree[17] = 1;

    udblk6_mov1(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 6, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
}

static void test_udblk6_mov2_initializes_reverse_and_returns_to_check1(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->r_no0 = 8;
    block->yposi.l = 300 << 16;

    udblk6_mov2(block);

    TEST_ASSERT_EQ_INT(ctx, 64, block->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, block->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, block->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 8, block->r_no0);

    block->actfree[16] = 1;
    block->actfree[17] = 1;

    udblk6_mov2(block);

    TEST_ASSERT_EQ_INT(ctx, 2, block->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 2, block->r_no0);
}

static void test_movecnt_applies_visible_speed_and_acceleration(
    test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->yposi.l = 300 << 16;
    block->yspeed.w = 5;
    block->actfree[16] = 1;

    movecnt(block);

    TEST_ASSERT_EQ_INT(ctx, (300 << 16) + (5 << 8), block->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 5, block->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, block->actfree[17]);
}

static void test_udblk6_entry_dispatches_callbacks(test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->xposi.w.h = 200;
    block->yposi.w.h = 300;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 300;

    udblk6(block);

    TEST_ASSERT_EQ_INT(ctx, 4, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == block);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == block);
}

static void test_udblk6_entry_dispatches_active_states(test_context *ctx) {
    sprite_status *block = &actwk[7];

    reset_udblk6_state();
    block->r_no0 = 4;

    udblk6(block);

    TEST_ASSERT_EQ_INT(ctx, 4, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_udblk6_state();
    block = &actwk[7];
    block->r_no0 = 6;
    block->xposi.w.h = 200;
    block->yposi.w.h = 300;
    actwk[0].xposi.w.h = 340;
    actwk[0].yposi.w.h = 300;

    udblk6(block);

    TEST_ASSERT_EQ_INT(ctx, 8, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_udblk6_state();
    block = &actwk[7];
    block->r_no0 = 8;

    udblk6(block);

    TEST_ASSERT_EQ_INT(ctx, 8, block->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_udblk6_init_sets_block_layout(&ctx);
    test_udblk6_chk1_triggers_when_player_is_left_and_near(&ctx);
    test_udblk6_chk1_rejects_vertical_negative_and_far_x(&ctx);
    test_udblk6_chk2_triggers_when_player_exits_right(&ctx);
    test_udblk6_chk2_rejects_inside_or_wrong_side(&ctx);
    test_udblk6_chk2_handles_player_below_block(&ctx);
    test_udblk6_mov1_initializes_and_finishes_two_phase_motion(&ctx);
    test_udblk6_mov2_initializes_reverse_and_returns_to_check1(&ctx);
    test_movecnt_applies_visible_speed_and_acceleration(&ctx);
    test_udblk6_entry_dispatches_callbacks(&ctx);
    test_udblk6_entry_dispatches_active_states(&ctx);
TEST_MAIN_END

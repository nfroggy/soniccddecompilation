#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

short_union stageno;
sprite_status actwk[128];
Sint16 scralim_down;
Sint16 scralim_n_down;
int_union scra_v_posit;
Uint8 limmoveflag;
int_union scra_h_posit;
Sint16 scralim_left;
Sint16 scralim_n_left;
Sint16 scralim_right;
Uint8 bossflag;

#include "src/r4/scrchk4.c"

static void reset_scrchk4_state(void) {
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    scralim_down = 0;
    scralim_n_down = 0;
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    limmoveflag = 0;
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    scralim_left = 0;
    scralim_n_left = 0;
    scralim_right = 0;
    bossflag = 0;
}

static void test_scrchk4_stage_41_keeps_matching_limit(test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 0;
    scralim_down = 1296;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk4_moves_limit_up_by_four(test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 0;
    scralim_down = 1400;
    scra_v_posit.w.h = 1000;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1396, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk4_clamps_upward_limit_to_even_camera_y(
    test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 0;
    scralim_down = 1400;
    scra_v_posit.w.h = 1301;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk4_downward_move_writes_step_value(test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 2;
    scralim_down = 1000;
    scralim_n_down = 1808;
    scra_v_posit.w.h = 900;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 4, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk4_downward_move_writes_fast_step_when_grounded(
    test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 2;
    scralim_down = 1000;
    scralim_n_down = 1808;
    scra_v_posit.w.h = 992;
    actwk[0].cddat = 2;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 16, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_zone4chk_dispatches_stage_targets(test_context *ctx) {
    reset_scrchk4_state();
    stageno.b.l = 0;

    zone4chk();

    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk4_state();
    stageno.b.l = 1;

    zone4chk();

    TEST_ASSERT_EQ_INT(ctx, 2048, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 2048, scralim_n_down);

    reset_scrchk4_state();
    stageno.b.l = 2;
    bossflag = 1;
    actwk[0].xposi.w.h = 1000;

    zone4chk();

    TEST_ASSERT_EQ_INT(ctx, 1264, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1264, scralim_n_down);
}

static void test_zone42chk_uses_1808_after_player_state_or_scroll(
    test_context *ctx) {
    reset_scrchk4_state();
    actwk[0].mstno.b.h = 43;

    zone42chk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk4_state();
    actwk[0].r_no0 = 6;

    zone42chk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk4_state();
    scra_h_posit.w.h = 512;

    zone42chk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
}

static void test_zone43chk_ignores_clear_path_when_bossflag_is_clear(
    test_context *ctx) {
    reset_scrchk4_state();
    actwk[0].xposi.w.h = 1000;

    zone43chk();

    TEST_ASSERT_EQ_INT(ctx, 0, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_down);
}

static void test_zone43chk_boss_clear_scrolls_left_boundary(test_context *ctx) {
    reset_scrchk4_state();
    bossflag = 1;
    actwk[0].xposi.w.h = 3000;
    scralim_left = 0;
    scralim_right = 4000;
    scralim_down = 1214;

    zone43chk();

    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_n_left);
}

static void test_bossclr_scr_waits_until_player_reaches_x(test_context *ctx) {
    Sint32 result;

    reset_scrchk4_state();
    actwk[0].xposi.w.h = 2807;

    result = bossclr_scr(2808, 1216);

    TEST_ASSERT_EQ_INT(ctx, 0, result);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_down);
}

static void test_bossclr_scrset_clamps_left_to_right_limit(test_context *ctx) {
    Sint32 result;

    reset_scrchk4_state();
    actwk[0].xposi.w.h = 3000;
    scralim_left = 0;
    scralim_right = 2000;
    scralim_down = 1216;

    result = bossclr_scrset(1216);

    TEST_ASSERT_EQ_INT(ctx, 1, result);
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 2000, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2000, scralim_n_left);
}

static void test_bossclr_scrset_handles_negative_down_delta(test_context *ctx) {
    Sint32 result;

    reset_scrchk4_state();
    actwk[0].xposi.w.h = 500;
    scralim_left = 0;
    scralim_right = 2000;
    scralim_down = 1300;

    result = bossclr_scrset(1216);

    TEST_ASSERT_EQ_INT(ctx, 1, result);
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1300, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 340, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 340, scralim_n_left);
}

static void test_bossclr_scrset_leaves_left_boundary_when_before_it(
    test_context *ctx) {
    Sint32 result;

    reset_scrchk4_state();
    actwk[0].xposi.w.h = 1000;
    scralim_left = 900;
    scralim_right = 2000;
    scralim_down = 1000;

    result = bossclr_scrset(1216);

    TEST_ASSERT_EQ_INT(ctx, 1, result);
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1000, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 900, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_left);
}

TEST_MAIN_BEGIN;
    test_scrchk4_stage_41_keeps_matching_limit(&ctx);
    test_scrchk4_moves_limit_up_by_four(&ctx);
    test_scrchk4_clamps_upward_limit_to_even_camera_y(&ctx);
    test_scrchk4_downward_move_writes_step_value(&ctx);
    test_scrchk4_downward_move_writes_fast_step_when_grounded(&ctx);
    test_zone4chk_dispatches_stage_targets(&ctx);
    test_zone42chk_uses_1808_after_player_state_or_scroll(&ctx);
    test_zone43chk_ignores_clear_path_when_bossflag_is_clear(&ctx);
    test_zone43chk_boss_clear_scrolls_left_boundary(&ctx);
    test_bossclr_scr_waits_until_player_reaches_x(&ctx);
    test_bossclr_scrset_clamps_left_to_right_limit(&ctx);
    test_bossclr_scrset_handles_negative_down_delta(&ctx);
    test_bossclr_scrset_leaves_left_boundary_when_before_it(&ctx);
TEST_MAIN_END

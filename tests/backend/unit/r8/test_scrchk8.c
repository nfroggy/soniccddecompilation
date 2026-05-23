#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

short_union stageno;
sprite_status actwk[128];
Sint16 scralim_down;
Sint16 scralim_n_down;
int_union scra_v_posit;
Uint8 limmoveflag;
Uint8 bossflag;

#include "src/r8/scrchk8.c"

static void reset_scrchk8_state(void) {
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    scralim_down = 0;
    scralim_n_down = 0;
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    limmoveflag = 0;
    bossflag = 0;
}

static void test_scrchk8_stage_81_and_82_target_1808(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 0;
    scralim_down = 1808;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);

    reset_scrchk8_state();
    stageno.b.l = 1;
    scralim_down = 1808;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk8_stage_83_targets_784_before_boss(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 2;
    bossflag = 0;
    scralim_down = 784;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk8_stage_83_boss_forces_limit_268(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 2;
    bossflag = 1;
    scralim_down = 999;
    scralim_n_down = 111;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 268, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 268, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk8_moves_limit_down_by_four(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 0;
    scralim_down = 1000;
    scra_v_posit.w.h = 900;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1004, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk8_moves_limit_down_fast_when_grounded_at_camera(
    test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 0;
    scralim_down = 1000;
    scra_v_posit.w.h = 992;
    actwk[0].cddat = 2;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1016, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk8_moves_limit_up_by_four(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 2;
    bossflag = 0;
    scralim_down = 1000;
    scra_v_posit.w.h = 700;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 996, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk8_clamps_upward_move_to_even_camera_y(test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 2;
    bossflag = 0;
    scralim_down = 1000;
    scra_v_posit.w.h = 901;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 896, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk8_unknown_stage_leaves_target_unchanged(
    test_context *ctx) {
    reset_scrchk8_state();
    stageno.b.l = 9;
    scralim_n_down = 456;
    scralim_down = 456;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 456, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 456, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

TEST_MAIN_BEGIN;
    test_scrchk8_stage_81_and_82_target_1808(&ctx);
    test_scrchk8_stage_83_targets_784_before_boss(&ctx);
    test_scrchk8_stage_83_boss_forces_limit_268(&ctx);
    test_scrchk8_moves_limit_down_by_four(&ctx);
    test_scrchk8_moves_limit_down_fast_when_grounded_at_camera(&ctx);
    test_scrchk8_moves_limit_up_by_four(&ctx);
    test_scrchk8_clamps_upward_move_to_even_camera_y(&ctx);
    test_scrchk8_unknown_stage_leaves_target_unchanged(&ctx);
TEST_MAIN_END

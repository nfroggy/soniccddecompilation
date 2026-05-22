#include <string.h>

#include "support/test_runner.h"
#include "types.h"

short_union stageno;
sprite_status actwk[128];
Sint16 scralim_down;
Sint16 scralim_n_down;
int_union scra_v_posit;
Uint8 limmoveflag;
Uint8 time_flag;
Sint16 scra_vline;
int_union scra_h_posit;
Uint8 pltime_f;
Sint16 scralim_left;
Sint16 scralim_n_left;
Uint8 bossflag;
Sint16 scralim_up;
Sint16 scralim_n_up;
Sint16 scralim_right;

#include "src/scrchk.c"

static void reset_scrchk_state(void) {
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    scralim_down = 0;
    scralim_n_down = 0;
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    limmoveflag = 0;
    time_flag = 0;
    scra_vline = 0;
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    pltime_f = 0;
    scralim_left = 0;
    scralim_n_left = 0;
    bossflag = 0;
    scralim_up = 0;
    scralim_n_up = 0;
    scralim_right = 0;
}

static void test_scrchk_returns_when_limit_is_unchanged(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 0;
    stageno.b.l = 1;
    scralim_down = 784;
    scralim_n_down = 123;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 784, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk_forces_down_limit_to_camera_for_mstno_43(
    test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 2;
    stageno.b.l = 0;
    actwk[0].mstno.b.h = 43;
    scra_v_posit.w.h = 333;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 333, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 333, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scrchk_moves_down_limit_upward_by_four(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 4;
    bossflag = 0;
    scralim_down = 2000;
    scra_v_posit.w.h = 100;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1996, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk_clamps_upward_move_to_camera_first(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 4;
    scralim_down = 2000;
    scra_v_posit.w.h = 1903;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1898, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk_moves_down_limit_downward_by_four(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 0;
    stageno.b.l = 0;
    time_flag = 1;
    scralim_down = 1000;
    scra_v_posit.w.h = 900;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1004, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk_uses_fast_downward_move_when_player_is_grounded(
    test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 0;
    stageno.b.l = 0;
    time_flag = 1;
    scralim_down = 1000;
    scra_v_posit.w.h = 992;
    actwk[0].cddat = 2;

    scrchk();

    TEST_ASSERT_EQ_INT(ctx, 1016, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
}

static void test_scrchk_dispatches_remaining_zone_families(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.h = 1;
    stageno.b.l = 0;
    scralim_down = 1296;
    scrchk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);

    reset_scrchk_state();
    stageno.b.h = 2;
    stageno.b.l = 0;
    scralim_down = 1296;
    scrchk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.h = 3;
    stageno.b.l = 0;
    scralim_down = 784;
    scrchk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);

    reset_scrchk_state();
    stageno.b.h = 5;
    stageno.b.l = 0;
    scralim_down = 1296;
    scrchk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.h = 6;
    stageno.b.l = 0;
    scralim_down = 1808;
    scrchk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);
}

static void test_zone1_dispatches_rounds(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 1;
    zone1chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    zone1chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
}

static void test_zone11_present_time_special_windows(test_context *ctx) {
    reset_scrchk_state();
    time_flag = 1;
    actwk[0].xposi.w.h = 8000;
    scra_h_posit.w.h = 2200;
    pltime_f = 1;
    scralim_left = 1000;

    zone11chk();

    TEST_ASSERT_EQ_INT(ctx, 136, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 1040, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 2080, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2080, scralim_n_left);

    scra_h_posit.w.h = 3600;
    zone11chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
}

static void test_zone11_ignores_special_windows_outside_present_time(
    test_context *ctx) {
    reset_scrchk_state();
    time_flag = 0;
    actwk[0].xposi.w.h = 8000;
    scra_h_posit.w.h = 3600;

    zone11chk();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
}

static void test_zone13_respects_boss_flag_and_boss_clear(test_context *ctx) {
    reset_scrchk_state();
    bossflag = 1;
    zone13chk();
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_down);

    reset_scrchk_state();
    actwk[0].xposi.w.h = 3600;
    scralim_down = 783;
    scralim_left = 0;
    scralim_right = 4000;
    zone13chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 3440, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 3440, scralim_n_left);
}

static void test_zone3_dispatch_and_boss_clear(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 0;
    zone3chk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    zone3chk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 0;
    zone3chk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 1;
    actwk[0].xposi.w.h = 500;
    scralim_left = 0;
    scralim_right = 1000;
    zone3chk();
    TEST_ASSERT_EQ_INT(ctx, 96, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 340, scralim_left);
}

static void test_zone4_dispatches_and_zone42_boundaries(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 0;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 1;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 1264, scralim_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    actwk[0].mstno.b.h = 43;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    actwk[0].r_no0 = 6;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    scra_h_posit.w.h = 511;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 2048, scralim_down);

    scra_h_posit.w.h = 512;
    zone4chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
}

static void test_zone43_boss_paths(test_context *ctx) {
    reset_scrchk_state();
    bossflag = 1;
    actwk[0].xposi.w.h = 1000;
    zone43chk();
    TEST_ASSERT_EQ_INT(ctx, 1264, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1264, scralim_n_down);

    reset_scrchk_state();
    bossflag = 1;
    actwk[0].xposi.w.h = 3000;
    scralim_left = 0;
    scralim_right = 4000;
    zone43chk();
    TEST_ASSERT_EQ_INT(ctx, 1216, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_left);
}

static void test_zone5_dispatch_and_boss_boundaries(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 1;
    zone5chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 0;
    actwk[0].xposi.w.h = 1000;
    zone5chk();
    TEST_ASSERT_EQ_INT(ctx, 800, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 1;
    actwk[0].xposi.w.h = 1000;
    zone5chk();
    TEST_ASSERT_EQ_INT(ctx, 504, scralim_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 1;
    actwk[0].xposi.w.h = 3700;
    scralim_left = 0;
    scralim_right = 4000;
    zone5chk();
    TEST_ASSERT_EQ_INT(ctx, 3540, scralim_left);
}

static void test_zone6_default_and_boss_y_bands(test_context *ctx) {
    reset_scrchk_state();
    bossflag = 0;
    zone6chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk_state();
    bossflag = 16;
    actwk[0].xposi.w.h = 2000;
    actwk[0].yposi.w.h = 664;
    scralim_down = 462;
    zone6chk();
    TEST_ASSERT_EQ_INT(ctx, 464, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 464, scralim_n_down);

    actwk[0].yposi.w.h = 1176;
    scralim_down = 1000;
    zone6chk();
    TEST_ASSERT_EQ_INT(ctx, 976, scralim_n_down);

    actwk[0].yposi.w.h = 1177;
    zone6chk();
    TEST_ASSERT_EQ_INT(ctx, 1488, scralim_n_down);
}

static void test_zone6_boss_clear_gate(test_context *ctx) {
    reset_scrchk_state();
    bossflag = 16;
    actwk[0].xposi.w.h = 3000;
    scralim_down = 464;
    scralim_left = 0;
    scralim_right = 4000;

    zone6chk();

    TEST_ASSERT_EQ_INT(ctx, 464, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_left);
}

static void test_zone8_dispatch_and_boss_lock(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 0;
    zone8chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    zone8chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    bossflag = 0;
    zone8chk();
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);

    bossflag = 1;
    zone8chk();
    TEST_ASSERT_EQ_INT(ctx, 268, scralim_up);
    TEST_ASSERT_EQ_INT(ctx, 268, scralim_n_up);
    TEST_ASSERT_EQ_INT(ctx, 268, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 268, scralim_n_down);
}

static void test_zone7_dispatch_and_zone73_windows(test_context *ctx) {
    reset_scrchk_state();
    stageno.b.l = 0;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 1;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_n_down);

    reset_scrchk_state();
    stageno.b.l = 2;
    actwk[0].xposi.w.h = 2351;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 528, scralim_n_down);

    actwk[0].xposi.w.h = 3520;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 528, scralim_n_down);

    actwk[0].xposi.w.h = 3000;
    scralim_down = 286;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 288, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 288, scralim_n_down);

    scralim_down = 290;
    zone7chk();
    TEST_ASSERT_EQ_INT(ctx, 288, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 288, scralim_n_down);
}

static void test_bossclr_scr_returns_zero_before_gate(test_context *ctx) {
    reset_scrchk_state();
    actwk[0].xposi.w.h = 999;

    TEST_ASSERT_EQ_INT(ctx, 0, bossclr_scr(1000, 200));
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_down);
}

static void test_bossclr_scrset_left_limit_branches(test_context *ctx) {
    reset_scrchk_state();
    actwk[0].xposi.w.h = 1000;
    scralim_down = 203;
    scralim_left = 900;
    scralim_right = 1200;

    TEST_ASSERT_EQ_INT(ctx, 1, bossclr_scrset(200));
    TEST_ASSERT_EQ_INT(ctx, 200, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 200, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 900, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_n_left);

    reset_scrchk_state();
    actwk[0].xposi.w.h = 2000;
    scralim_left = 0;
    scralim_right = 1500;
    TEST_ASSERT_EQ_INT(ctx, 1, bossclr_scrset(300));
    TEST_ASSERT_EQ_INT(ctx, 1500, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 1500, scralim_n_left);
}

TEST_MAIN_BEGIN;
    test_scrchk_returns_when_limit_is_unchanged(&ctx);
    test_scrchk_forces_down_limit_to_camera_for_mstno_43(&ctx);
    test_scrchk_moves_down_limit_upward_by_four(&ctx);
    test_scrchk_clamps_upward_move_to_camera_first(&ctx);
    test_scrchk_moves_down_limit_downward_by_four(&ctx);
    test_scrchk_uses_fast_downward_move_when_player_is_grounded(&ctx);
    test_scrchk_dispatches_remaining_zone_families(&ctx);
    test_zone1_dispatches_rounds(&ctx);
    test_zone11_present_time_special_windows(&ctx);
    test_zone11_ignores_special_windows_outside_present_time(&ctx);
    test_zone13_respects_boss_flag_and_boss_clear(&ctx);
    test_zone3_dispatch_and_boss_clear(&ctx);
    test_zone4_dispatches_and_zone42_boundaries(&ctx);
    test_zone43_boss_paths(&ctx);
    test_zone5_dispatch_and_boss_boundaries(&ctx);
    test_zone6_default_and_boss_y_bands(&ctx);
    test_zone6_boss_clear_gate(&ctx);
    test_zone8_dispatch_and_boss_lock(&ctx);
    test_zone7_dispatch_and_zone73_windows(&ctx);
    test_bossclr_scr_returns_zero_before_gate(&ctx);
    test_bossclr_scrset_left_limit_branches(&ctx);
TEST_MAIN_END;

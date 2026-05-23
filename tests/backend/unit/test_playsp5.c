#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;
Uint8 time_flag;
Uint8 generate_flag;
Uint8 colrevflag;

static int scramapad_count;
static Sint16 scramapad_x[4];
static Sint16 scramapad_y[4];
static Sint16 scramapad_results[4];
static int dircol_count;
static sprite_status *dircol_actor;
static char dircol_direc[4];
static Sint16 dircol_result;

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dircol(sprite_status *pActwk, char *cpDirec);

#include "src/r5/playsp5.c"

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    (void)pActwk;
    if (scramapad_count < 4) {
        scramapad_x[scramapad_count] = iXposi;
        scramapad_y[scramapad_count] = iYposi;
    }
    return scramapad_results[scramapad_count++];
}

Sint16 dircol(sprite_status *pActwk, char *cpDirec) {
    ++dircol_count;
    dircol_actor = pActwk;
    if (dircol_count <= 4) {
        dircol_direc[dircol_count - 1] = *cpDirec;
    }
    return dircol_result;
}

static void reset_playsp5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    bossflag = 0;
    time_flag = 0;
    generate_flag = 0;
    colrevflag = 0;
    scramapad_count = 0;
    memset(scramapad_x, 0, sizeof(scramapad_x));
    memset(scramapad_y, 0, sizeof(scramapad_y));
    memset(scramapad_results, 0, sizeof(scramapad_results));
    dircol_count = 0;
    dircol_actor = 0;
    memset(dircol_direc, 0, sizeof(dircol_direc));
    dircol_result = 0;
}

static void setup_player_for_belt(void) {
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 2000;
    actwk[0].sprhs = 12;
    actwk[0].sprvsize = 18;
}

static void test_playsp_routes_to_belt_and_honors_boss_gate(test_context *ctx) {
    reset_playsp5_state();
    setup_player_for_belt();
    bossflag = 1;
    scramapad_results[0] = 1;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);

    reset_playsp5_state();
    setup_player_for_belt();
    scramapad_results[0] = 1;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, actwk[0].xposi.w.l);
}

static void test_belt_checks_left_then_right_probe_coordinates(
    test_context *ctx) {
    reset_playsp5_state();
    setup_player_for_belt();
    time_flag = 1;
    scramapad_results[0] = 999;
    scramapad_results[1] = 450;

    belt();

    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 988, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 2020, scramapad_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 1012, scramapad_x[1]);
    TEST_ASSERT_EQ_INT(ctx, 2020, scramapad_y[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 1001, actwk[0].xposi.w.h);
}

static void test_belt_returns_when_both_probes_miss(test_context *ctx) {
    reset_playsp5_state();
    setup_player_for_belt();
    scramapad_results[0] = 999;
    scramapad_results[1] = 998;

    belt();

    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);
}

static void test_belt_effect_uses_time_speed_and_collision_correction(
    test_context *ctx) {
    reset_playsp5_state();
    setup_player_for_belt();
    time_flag = 0;
    dircol_result = 0;

    belt_effect();

    TEST_ASSERT_EQ_INT(ctx, 1, dircol_count);
    TEST_ASSERT_TRUE(ctx, dircol_actor == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, -64, dircol_direc[0]);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, actwk[0].xposi.w.l);

    reset_playsp5_state();
    setup_player_for_belt();
    time_flag = 1;
    dircol_result = -3;

    belt_effect();

    TEST_ASSERT_EQ_INT(ctx, 998, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0x0000, actwk[0].xposi.w.l);

    reset_playsp5_state();
    setup_player_for_belt();
    time_flag = 3;
    dircol_result = -5;

    belt_effect();

    TEST_ASSERT_EQ_INT(ctx, 997, actwk[0].xposi.w.h);
}

static void test_belt_effect_reverses_motion_and_collision_when_colrevflag(
    test_context *ctx) {
    reset_playsp5_state();
    setup_player_for_belt();
    time_flag = 1;
    colrevflag = 1;
    dircol_result = 0;

    belt_effect();

    TEST_ASSERT_EQ_INT(ctx, 1, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 64, dircol_direc[0]);
    TEST_ASSERT_EQ_INT(ctx, 999, actwk[0].xposi.w.h);

    reset_playsp5_state();
    setup_player_for_belt();
    colrevflag = 1;
    dircol_result = -4;

    belt_effect();

    TEST_ASSERT_EQ_INT(ctx, 1003, actwk[0].xposi.w.h);
}

static void test_belt_chk_uses_time_and_generate_tables(test_context *ctx) {
    reset_playsp5_state();

    time_flag = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_chk(485));
    TEST_ASSERT_EQ_INT(ctx, 0, belt_chk(500));

    time_flag = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_chk(450));
    TEST_ASSERT_EQ_INT(ctx, 0, belt_chk(10));

    time_flag = 2;
    generate_flag = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_chk(500));
    TEST_ASSERT_EQ_INT(ctx, 0, belt_chk(493));

    time_flag = 2;
    generate_flag = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_chk(493));
    TEST_ASSERT_EQ_INT(ctx, 0, belt_chk(500));

    time_flag = 129;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_chk(450));
}

TEST_MAIN_BEGIN;
test_playsp_routes_to_belt_and_honors_boss_gate(&ctx);
test_belt_checks_left_then_right_probe_coordinates(&ctx);
test_belt_returns_when_both_probes_miss(&ctx);
test_belt_effect_uses_time_speed_and_collision_correction(&ctx);
test_belt_effect_reverses_motion_and_collision_when_colrevflag(&ctx);
test_belt_chk_uses_time_and_generate_tables(&ctx);
TEST_MAIN_END;

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#include "src/r4/screw_c.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void reset_screw_c_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void assert_actionsub_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_screw_c_patterns_capture_horizontal_and_vertical_tiles(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_screw0[0] == &pat_screw00);
    TEST_ASSERT_TRUE(ctx, pat_screw0[1] == &pat_screw01);
    TEST_ASSERT_TRUE(ctx, pat_screw1[0] == &pat_screw02);
    TEST_ASSERT_TRUE(ctx, pat_screw1[1] == &pat_screw03);
    TEST_ASSERT_EQ_INT(ctx, 1, pat_screw00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_screw00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_screw00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWC_BASE, pat_screw00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_screw01.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWC_BASE + 1, pat_screw01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_screw02.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_screw02.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWC_BASE + 2, pat_screw02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWC_BASE + 3, pat_screw03.spra[0].index);
}

static void test_screw_c_initializes_horizontal_screw(test_context *ctx) {
    sprite_status *screw_actor = &actwk[2];

    reset_screw_c_state();
    actwk[0].actno = 77;

    screw(screw_actor);

    assert_actionsub_only(ctx, screw_actor);
}

static void test_screw_c_initializes_vertical_flipped_screw(test_context *ctx) {
    sprite_status *screw_actor = &actwk[3];

    reset_screw_c_state();
    actwk[0].actno = 88;
    screw_actor->userflag.b.h = 131;

    screw(screw_actor);

    assert_actionsub_only(ctx, screw_actor);
}

static void test_screw_c_switchflag_toggle_path_runs_once(test_context *ctx) {
    sprite_status *screw_actor = &actwk[4];

    reset_screw_c_state();
    actwk[0].actno = 99;
    screw_actor->userflag.b.l = 5;
    switchflag[5] = 128;

    screw(screw_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;

    screw(screw_actor);
    assert_actionsub_only(ctx, screw_actor);
}

static void test_screw_c_switchflag_high_userflag_skips_toggle_gate(
    test_context *ctx) {
    sprite_status *screw_actor = &actwk[5];

    reset_screw_c_state();
    actwk[0].actno = 100;
    screw_actor->userflag.b.l = 133;
    switchflag[5] = 128;

    screw(screw_actor);

    assert_actionsub_only(ctx, screw_actor);
}

static void test_screw_c_existing_actor_with_live_parent_runs_action(
    test_context *ctx) {
    sprite_status *screw_actor = &actwk[6];

    reset_screw_c_state();
    actwk[0].actno = 101;
    screw_actor->r_no0 = 2;
    screw_actor->actfree[2] = 101;

    screw(screw_actor);

    assert_actionsub_only(ctx, screw_actor);
}

static void test_screw_c_frames_out_when_parent_actor_changes(
    test_context *ctx) {
    sprite_status *screw_actor = &actwk[7];

    reset_screw_c_state();
    actwk[0].actno = 1;
    screw_actor->r_no0 = 2;
    screw_actor->actfree[2] = 2;

    screw(screw_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == screw_actor);
}

TEST_MAIN_BEGIN;
    test_screw_c_patterns_capture_horizontal_and_vertical_tiles(&ctx);
    test_screw_c_initializes_horizontal_screw(&ctx);
    test_screw_c_initializes_vertical_flipped_screw(&ctx);
    test_screw_c_switchflag_toggle_path_runs_once(&ctx);
    test_screw_c_switchflag_high_userflag_skips_toggle_gate(&ctx);
    test_screw_c_existing_actor_with_live_parent_runs_action(&ctx);
    test_screw_c_frames_out_when_parent_actor_changes(&ctx);
TEST_MAIN_END

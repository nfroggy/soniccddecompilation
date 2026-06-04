#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_patterns;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);

#include "src/r4/screw_a.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_patterns = pat_dat;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

static void reset_screw_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_patterns = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
}

static void assert_action_and_frameout(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void assert_patchg_called_with_screw_change(test_context *ctx,
                                                   sprite_status *actor,
                                                   int expected_count) {
    TEST_ASSERT_EQ_INT(ctx, expected_count, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_patterns == pchg);
}

static void test_screw_patterns_capture_all_animation_tiles(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg_00);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg_00[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, pchg_00[5]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_00[6]);

    TEST_ASSERT_TRUE(ctx, pat_screw0[0] == &screw0_pat00);
    TEST_ASSERT_TRUE(ctx, pat_screw0[4] == &screw0_pat04);
    TEST_ASSERT_TRUE(ctx, pat_screw1[0] == &screw1_pat00);
    TEST_ASSERT_TRUE(ctx, pat_screw1[4] == &screw1_pat04);

    TEST_ASSERT_EQ_INT(ctx, -16, screw0_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, screw0_pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWA_BASE, screw0_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWA_BASE + 4, screw0_pat04.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, screw1_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, screw1_pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWA_BASE + 5, screw1_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SCREWA_BASE + 9, screw1_pat04.spra[0].index);
}

static void test_screw_initializes_inactive_horizontal_screw(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_screw_state();

    screw(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_action_and_frameout(ctx, actor);
}

static void test_screw_initializes_vertical_flipped_active_screw(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_screw_state();
    actor->userflag.b.h = 131;

    screw(actor);

    assert_patchg_called_with_screw_change(ctx, actor, 1);
    assert_action_and_frameout(ctx, actor);
}

static void test_screw_switchflag_starts_animation_then_latches(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_screw_state();
    actor->userflag.b.l = 4;
    switchflag[4] = 128;

    screw(actor);
    assert_patchg_called_with_screw_change(ctx, actor, 1);
    assert_action_and_frameout(ctx, actor);

    patchg_count = 0;
    patchg_actor = 0;
    patchg_patterns = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;

    screw(actor);

    assert_patchg_called_with_screw_change(ctx, actor, 1);
    assert_action_and_frameout(ctx, actor);
}

static void test_screw_high_switch_index_skips_switchflag_gate(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_screw_state();
    actor->userflag.b.l = 132;
    switchflag[4] = 128;

    screw(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_action_and_frameout(ctx, actor);
}

static void test_screw_existing_active_actor_animates_without_init(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_screw_state();
    actor->r_no0 = 2;
    screw_a_work_get(actor)->animation_enabled = 255;

    screw(actor);

    assert_patchg_called_with_screw_change(ctx, actor, 1);
    assert_action_and_frameout(ctx, actor);
}

TEST_MAIN_BEGIN;
    test_screw_patterns_capture_all_animation_tiles(&ctx);
    test_screw_initializes_inactive_horizontal_screw(&ctx);
    test_screw_initializes_vertical_flipped_active_screw(&ctx);
    test_screw_switchflag_starts_animation_then_latches(&ctx);
    test_screw_high_switch_index_skips_switchflag_gate(&ctx);
    test_screw_existing_active_actor_animates_without_init(&ctx);
TEST_MAIN_END

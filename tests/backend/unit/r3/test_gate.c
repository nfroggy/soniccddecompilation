#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r3/gate.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

static void reset_gate_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
}

static void test_gate_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, gate_pat[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, gate_pat[7] == &pat07);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, 48, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 619, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -64, pat07.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 626, pat07.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg0[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, pchg0[3]);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg0[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, pchg0[5]);
    TEST_ASSERT_EQ_INT(ctx, 5, pchg0[6]);
    TEST_ASSERT_EQ_INT(ctx, 6, pchg0[7]);
    TEST_ASSERT_EQ_INT(ctx, 7, pchg0[8]);
    TEST_ASSERT_EQ_INT(ctx, 7, pchg0[9]);
}

static void test_gate_initializes_closed_target_and_waits_for_animation(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->actflg = 128;
    actor->patno = 3;

    gate(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
}

static void test_gate_open_state_finishes_and_moves_in_same_call(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->r_no0 = 2;
    gate_work_get(actor)->target_patno = 7;
    actor->patno = 7;
    actor->actflg = 128;
    ridechk_result = 1;

    gate(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
}

static void test_gate_open_state_keeps_waiting_when_pattern_not_done(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->r_no0 = 2;
    gate_work_get(actor)->target_patno = 7;
    actor->patno = 6;
    actor->actflg = 0;

    gate(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
}

static void test_gate_state_four_moves_before_and_after_dispatch(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->r_no0 = 4;
    actor->actflg = 128;
    ridechk_result = 1;

    gate(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_gate_state_six_reinitializes_without_public_short_target(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->r_no0 = 6;
    actor->patno = 7;

    gate(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_act_init6_uses_short_target_and_public_init10_keeps_target(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_gate_state();
    actor->r_no0 = 44;
    actor->patno = 5;

    act_init6(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_gate_state();
    actor = &actwk[2];
    actor->r_no0 = 8;
    gate_work_get(actor)->target_patno = 5;
    actor->patno = 3;

    act_init10(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

TEST_MAIN_BEGIN;
    test_gate_tables_capture_literal_data(&ctx);
    test_gate_initializes_closed_target_and_waits_for_animation(&ctx);
    test_gate_open_state_finishes_and_moves_in_same_call(&ctx);
    test_gate_open_state_keeps_waiting_when_pattern_not_done(&ctx);
    test_gate_state_four_moves_before_and_after_dispatch(&ctx);
    test_gate_state_six_reinitializes_without_public_short_target(&ctx);
    test_act_init6_uses_short_target_and_public_init10_keeps_target(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r3/stopper.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
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

static void reset_stopper_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
}

static void test_stopper_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, stopper_pat[0] == &patstop1);
    TEST_ASSERT_TRUE(ctx, stopper_pat[1] == &patstop2);
    TEST_ASSERT_TRUE(ctx, stopper_pat[2] == &patstop3);
    TEST_ASSERT_EQ_INT(ctx, 1, patstop1.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, patstop1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, patstop1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, patstop1.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 567, patstop1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 568, patstop2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 569, patstop3.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_TRUE(ctx, pchg[1] == pchg1);
    TEST_ASSERT_EQ_INT(ctx, 9, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg0[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, pchg0[3]);
    TEST_ASSERT_EQ_INT(ctx, -4, pchg0[4]);
    TEST_ASSERT_EQ_INT(ctx, 9, pchg1[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, pchg1[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg1[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg1[3]);
    TEST_ASSERT_EQ_INT(ctx, -4, pchg1[4]);
}

static void test_stopper_initializes_and_draws_when_parent_is_alive(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->actflg = 128;
    actor->patno = 1;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_stopper_frameouts_when_parent_is_missing(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 0;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_stopper_move_ignores_non_trigger_pattern(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 2;
    actor->patno = 1;
    actwk[0].xspeed.w = 123;
    actwk[0].yspeed.w = 456;
    actwk[0].cddat = 255;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 456, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 255, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_stopper_move_waits_when_ride_check_fails(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 2;
    actor->patno = 2;
    actwk[0].xspeed.w = 123;
    actwk[0].yspeed.w = 456;
    actwk[0].cddat = 255;
    ridechk_result = 0;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 456, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 255, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_stopper_move_launches_player_when_riding(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 2;
    actor->patno = 2;
    actwk[0].xspeed.w = 123;
    actwk[0].yspeed.w = 456;
    actwk[0].cddat = 255;
    ridechk_result = 1;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 199, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_stopper_open_and_close_states_select_animation(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 4;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_stopper_state();
    actwk[0].actno = 54;
    actor = &actwk[2];
    actor->r_no0 = 6;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_stopper_animation_state_only_patches(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 8;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_stopper_late_move_state_uses_same_ride_logic(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();
    actwk[0].actno = 54;
    actor->r_no0 = 10;
    actor->patno = 2;
    actwk[0].cddat = 240;
    ridechk_result = 1;

    stopper(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_public_act_move1_patches_directly(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_stopper_state();

    act_move1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_stopper_tables_capture_literal_data(&ctx);
    test_stopper_initializes_and_draws_when_parent_is_alive(&ctx);
    test_stopper_frameouts_when_parent_is_missing(&ctx);
    test_stopper_move_ignores_non_trigger_pattern(&ctx);
    test_stopper_move_waits_when_ride_check_fails(&ctx);
    test_stopper_move_launches_player_when_riding(&ctx);
    test_stopper_open_and_close_states_select_animation(&ctx);
    test_stopper_animation_state_only_patches(&ctx);
    test_stopper_late_move_state_uses_same_ride_logic(&ctx);
    test_public_act_move1_patches_directly(&ctx);
TEST_MAIN_END

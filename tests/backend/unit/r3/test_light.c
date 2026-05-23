#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#include "src/r3/light.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void reset_light_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void test_light_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, light_pat[0] == &light_pat0);
    TEST_ASSERT_EQ_INT(ctx, 1, light_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, light_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, light_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, light_pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 570, light_pat0.spra[0].index);
}

static void test_light_initializes_and_draws_while_parent_is_alive(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_light_state();
    actwk[0].actno = 54;

    light(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_light_frameouts_when_parent_actor_changes(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_light_state();
    actwk[0].actno = 0;

    light(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_light_move_state_only_checks_parent_and_draws(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_light_state();
    actor->r_no0 = 2;
    actor->actflg = 128;
    actor->sprpri = 9;
    actwk[0].actno = 54;

    light(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_light_die_state_calls_act_die_then_parent_check(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_light_state();
    actor->r_no0 = 4;
    actwk[0].actno = 54;

    light(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_act_die_frameouts_directly(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_light_state();

    act_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_light_tables_capture_literal_data(&ctx);
    test_light_initializes_and_draws_while_parent_is_alive(&ctx);
    test_light_frameouts_when_parent_actor_changes(&ctx);
    test_light_move_state_only_checks_parent_and_draws(&ctx);
    test_light_die_state_calls_act_die_then_parent_check(&ctx);
    test_act_die_frameouts_directly(&ctx);
TEST_MAIN_END

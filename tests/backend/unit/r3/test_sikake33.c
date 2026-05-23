#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int kaitenban_count;
static int light_count;
static int stopper_count;
static sprite_status *kaitenban_actor;
static sprite_status *light_actor;
static sprite_status *stopper_actor;

void kaitenban(sprite_status *banwk);
void light(sprite_status *actionwk);
void stopper(sprite_status *actionwk);

#include "src/r3/sikake33.c"

void kaitenban(sprite_status *banwk) {
    ++kaitenban_count;
    kaitenban_actor = banwk;
}

void light(sprite_status *actionwk) {
    ++light_count;
    light_actor = actionwk;
}

void stopper(sprite_status *actionwk) {
    ++stopper_count;
    stopper_actor = actionwk;
}

static void reset_sikake33_state(void) {
    kaitenban_count = 0;
    light_count = 0;
    stopper_count = 0;
    kaitenban_actor = 0;
    light_actor = 0;
    stopper_actor = 0;
}

static void test_sikake33_negative_userflag_dispatches_to_kaitenban(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_sikake33_state();
    actor.userflag.b.h = -1;

    sikake33(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, kaitenban_count);
    TEST_ASSERT_TRUE(ctx, kaitenban_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, light_count);
    TEST_ASSERT_EQ_INT(ctx, 0, stopper_count);
}

static void test_sikake33_zero_userflag_dispatches_to_light(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_sikake33_state();
    actor.userflag.b.h = 0;

    sikake33(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, kaitenban_count);
    TEST_ASSERT_EQ_INT(ctx, 1, light_count);
    TEST_ASSERT_TRUE(ctx, light_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, stopper_count);
}

static void test_sikake33_positive_userflag_dispatches_to_stopper(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_sikake33_state();
    actor.userflag.b.h = 1;

    sikake33(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, kaitenban_count);
    TEST_ASSERT_EQ_INT(ctx, 0, light_count);
    TEST_ASSERT_EQ_INT(ctx, 1, stopper_count);
    TEST_ASSERT_TRUE(ctx, stopper_actor == &actor);
}

TEST_MAIN_BEGIN;
    test_sikake33_negative_userflag_dispatches_to_kaitenban(&ctx);
    test_sikake33_zero_userflag_dispatches_to_light(&ctx);
    test_sikake33_positive_userflag_dispatches_to_stopper(&ctx);
TEST_MAIN_END

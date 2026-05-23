#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int frameout_count;
static sprite_status *frameout_actor;

void frameout(sprite_status *pActwk);

#include "src/r3/miracle.c"

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void reset_miracle_state(void) {
    frameout_count = 0;
    frameout_actor = 0;
}

static void test_miracle_frameouts_actor_without_mutating_visible_fields(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_miracle_state();
    actor.actno = 55;
    actor.r_no0 = 6;
    actor.xposi.w.h = 1234;
    actor.yposi.w.h = 567;
    actor.userflag.b.h = -4;

    miracle(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 55, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 6, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1234, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 567, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4, actor.userflag.b.h);
}

TEST_MAIN_BEGIN;
    test_miracle_frameouts_actor_without_mutating_visible_fields(&ctx);
TEST_MAIN_END

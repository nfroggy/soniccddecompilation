#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);

#include "src/r4/tekkyu1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
}

static void test_tekkyu1_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tekkyu1[0] == &pat0);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU1_BASE, pat0.spra[0].index);
}

static void test_tekkyu1_initializes_then_renders(test_context *ctx) {
    sprite_status actor;

    reset_logs();
    memset(&actor, 0, sizeof(actor));

    tekkyu1(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 169, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 880, actor.sproffset);
    TEST_ASSERT_TRUE(ctx, actor.patbase == pat_tekkyu1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == &actor);
}

static void test_tekkyu1_existing_actor_only_renders(test_context *ctx) {
    sprite_status actor;

    reset_logs();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actor.actflg = 99;
    actor.colino = 7;
    actor.sproffset = 123;
    actor.patbase = 0;

    tekkyu1(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 99, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 7, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, 123, actor.sproffset);
    TEST_ASSERT_TRUE(ctx, actor.patbase == 0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == &actor);
}

TEST_MAIN_BEGIN;
    test_tekkyu1_patterns_capture_literal_data(&ctx);
    test_tekkyu1_initializes_then_renders(&ctx);
    test_tekkyu1_existing_actor_only_renders(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 time_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);

#include "src/r7/tekkyu7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

static void reset_tekkyu7_state(void) {
    time_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
}

static void test_tekkyu7_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tekkyu7[0] == &tekkyu7_pat0);
    TEST_ASSERT_EQ_INT(ctx, 1, tekkyu7_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, tekkyu7_pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 462, tekkyu7_pat0.spra[0].index);
}

static void test_tekkyu7_init_uses_horizontal_fast_table(test_context *ctx) {
    sprite_status actor;

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    time_flag = 0;
    actor.userflag.b.h = 0;

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 181, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 902, actor.sproffset);
    TEST_ASSERT_TRUE(ctx, actor.patbase == pat_tekkyu7);
    TEST_ASSERT_EQ_INT(ctx, 104, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_tekkyu7_init_uses_vertical_fast_table(test_context *ctx) {
    sprite_status actor;

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    time_flag = 0;
    actor.userflag.b.h = 1;

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 204, actor.yposi.w.h);
}

static void test_tekkyu7_init_uses_mid_and_slow_time_tables(test_context *ctx) {
    sprite_status actor;

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    time_flag = 1;
    actor.userflag.b.h = 0;

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 102, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    time_flag = 2;
    actor.userflag.b.h = 1;

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 201, actor.yposi.w.h);
}

static void test_tekkyu7_move_reverses_when_move_counter_expires(
    test_context *ctx) {
    sprite_status actor;
    int i;

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    tekkyu7(&actor);
    for (i = 0; i < 10; ++i) {
        tekkyu7(&actor);
    }

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 12, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 12, frameout_s00_count);
}

static void test_tekkyu7_stop_waits_then_returns_to_move(test_context *ctx) {
    sprite_status actor;
    int i;

    reset_tekkyu7_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    tekkyu7(&actor);
    for (i = 0; i < 11; ++i) {
        tekkyu7(&actor);
    }

    tekkyu7(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);

    for (i = 0; i < 59; ++i) {
        tekkyu7(&actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
}

TEST_MAIN_BEGIN;
    test_tekkyu7_tables_capture_literal_data(&ctx);
    test_tekkyu7_init_uses_horizontal_fast_table(&ctx);
    test_tekkyu7_init_uses_vertical_fast_table(&ctx);
    test_tekkyu7_init_uses_mid_and_slow_time_tables(&ctx);
    test_tekkyu7_move_reverses_when_move_counter_expires(&ctx);
    test_tekkyu7_stop_waits_then_returns_to_move(&ctx);
TEST_MAIN_END

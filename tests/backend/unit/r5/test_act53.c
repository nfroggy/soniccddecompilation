#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int frameout_count;
static sprite_status *frameout_actor;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int stub_call_count;
static sprite_status *stub_actor;

void frameout(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);

#define DECLARE_ACT_STUB(name) void name(sprite_status *pActwk)
DECLARE_ACT_STUB(play00);
DECLARE_ACT_STUB(baria);
DECLARE_ACT_STUB(test_act);
DECLARE_ACT_STUB(shooter);
DECLARE_ACT_STUB(spring);
DECLARE_ACT_STUB(exit2);
DECLARE_ACT_STUB(exit2_set);
DECLARE_ACT_STUB(futa);
DECLARE_ACT_STUB(mizukiri);
DECLARE_ACT_STUB(koma);
DECLARE_ACT_STUB(ring);
DECLARE_ACT_STUB(flyring);
DECLARE_ACT_STUB(bigring);
DECLARE_ACT_STUB(gene);
DECLARE_ACT_STUB(mosugu);
DECLARE_ACT_STUB(goal);
DECLARE_ACT_STUB(bakuha);
DECLARE_ACT_STUB(item);
DECLARE_ACT_STUB(item2);
DECLARE_ACT_STUB(iwa);
DECLARE_ACT_STUB(score);
DECLARE_ACT_STUB(flower);
DECLARE_ACT_STUB(egg5hibana);
DECLARE_ACT_STUB(egg5hari);
DECLARE_ACT_STUB(egg5belt);
DECLARE_ACT_STUB(egg5bakuha);
DECLARE_ACT_STUB(kuzure5);
DECLARE_ACT_STUB(dair5);
DECLARE_ACT_STUB(egg5);
DECLARE_ACT_STUB(egg5meca1);
DECLARE_ACT_STUB(egg5meca2);
DECLARE_ACT_STUB(egg5meca3);
DECLARE_ACT_STUB(egg5pipe);
DECLARE_ACT_STUB(egg5catch);
DECLARE_ACT_STUB(egg5bomb);
DECLARE_ACT_STUB(egg5bomb2);
DECLARE_ACT_STUB(clear);
DECLARE_ACT_STUB(over);
DECLARE_ACT_STUB(title);
#undef DECLARE_ACT_STUB

#include "src/r5/act53.c"

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

static void stub_record(sprite_status *pActwk) {
    ++stub_call_count;
    stub_actor = pActwk;
}

#define DEFINE_ACT_STUB(name)                                                  \
    void name(sprite_status *pActwk) { stub_record(pActwk); }
DEFINE_ACT_STUB(play00)
DEFINE_ACT_STUB(baria)
DEFINE_ACT_STUB(test_act)
DEFINE_ACT_STUB(shooter)
DEFINE_ACT_STUB(spring)
DEFINE_ACT_STUB(exit2)
DEFINE_ACT_STUB(exit2_set)
DEFINE_ACT_STUB(futa)
DEFINE_ACT_STUB(mizukiri)
DEFINE_ACT_STUB(koma)
DEFINE_ACT_STUB(ring)
DEFINE_ACT_STUB(flyring)
DEFINE_ACT_STUB(bigring)
DEFINE_ACT_STUB(gene)
DEFINE_ACT_STUB(mosugu)
DEFINE_ACT_STUB(goal)
DEFINE_ACT_STUB(bakuha)
DEFINE_ACT_STUB(item)
DEFINE_ACT_STUB(item2)
DEFINE_ACT_STUB(iwa)
DEFINE_ACT_STUB(score)
DEFINE_ACT_STUB(flower)
DEFINE_ACT_STUB(egg5hibana)
DEFINE_ACT_STUB(egg5hari)
DEFINE_ACT_STUB(egg5belt)
DEFINE_ACT_STUB(egg5bakuha)
DEFINE_ACT_STUB(kuzure5)
DEFINE_ACT_STUB(dair5)
DEFINE_ACT_STUB(egg5)
DEFINE_ACT_STUB(egg5meca1)
DEFINE_ACT_STUB(egg5meca2)
DEFINE_ACT_STUB(egg5meca3)
DEFINE_ACT_STUB(egg5pipe)
DEFINE_ACT_STUB(egg5catch)
DEFINE_ACT_STUB(egg5bomb)
DEFINE_ACT_STUB(egg5bomb2)
DEFINE_ACT_STUB(clear)
DEFINE_ACT_STUB(over)
DEFINE_ACT_STUB(title)
#undef DEFINE_ACT_STUB

static void reset_act53_state(void) {
    frameout_count = 0;
    frameout_actor = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void assert_act_entry(test_context *ctx, int index,
                             void (*expected)(sprite_status *)) {
    TEST_ASSERT_TRUE(ctx, act_tbl[index] == expected);
}

static void test_act53_table_captures_all_dispatch_slots(test_context *ctx) {
    assert_act_entry(ctx, 0, play00);
    assert_act_entry(ctx, 1, play00);
    assert_act_entry(ctx, 2, baria);
    assert_act_entry(ctx, 3, noact);
    assert_act_entry(ctx, 4, noact);
    assert_act_entry(ctx, 5, test_act);
    assert_act_entry(ctx, 6, shooter);
    assert_act_entry(ctx, 7, noact);
    assert_act_entry(ctx, 8, noact);
    assert_act_entry(ctx, 9, spring);
    assert_act_entry(ctx, 10, exit2);
    assert_act_entry(ctx, 11, exit2_set);
    assert_act_entry(ctx, 12, futa);
    assert_act_entry(ctx, 13, mizukiri);
    assert_act_entry(ctx, 14, koma);
    assert_act_entry(ctx, 15, ring);
    assert_act_entry(ctx, 16, flyring);
    assert_act_entry(ctx, 17, noact);
    assert_act_entry(ctx, 18, noact);
    assert_act_entry(ctx, 19, bigring);
    assert_act_entry(ctx, 20, gene);
    assert_act_entry(ctx, 21, mosugu);
    assert_act_entry(ctx, 22, goal);
    assert_act_entry(ctx, 23, bakuha);
    assert_act_entry(ctx, 24, item);
    assert_act_entry(ctx, 25, item2);
    assert_act_entry(ctx, 26, iwa);
    assert_act_entry(ctx, 27, score);
    assert_act_entry(ctx, 28, noact);
    assert_act_entry(ctx, 29, noact);
    assert_act_entry(ctx, 30, flower);
    assert_act_entry(ctx, 31, egg5hibana);
    assert_act_entry(ctx, 32, egg5hari);
    assert_act_entry(ctx, 33, egg5belt);
    assert_act_entry(ctx, 34, egg5bakuha);
    assert_act_entry(ctx, 35, noact);
    assert_act_entry(ctx, 36, noact);
    assert_act_entry(ctx, 37, noact);
    assert_act_entry(ctx, 38, noact);
    assert_act_entry(ctx, 39, kuzure5);
    assert_act_entry(ctx, 40, noact);
    assert_act_entry(ctx, 41, noact);
    assert_act_entry(ctx, 42, noact);
    assert_act_entry(ctx, 43, noact);
    assert_act_entry(ctx, 44, noact);
    assert_act_entry(ctx, 45, noact);
    assert_act_entry(ctx, 46, noact);
    assert_act_entry(ctx, 47, dair5);
    assert_act_entry(ctx, 48, noact);
    assert_act_entry(ctx, 49, egg5);
    assert_act_entry(ctx, 50, egg5meca1);
    assert_act_entry(ctx, 51, egg5meca2);
    assert_act_entry(ctx, 52, egg5meca3);
    assert_act_entry(ctx, 53, egg5pipe);
    assert_act_entry(ctx, 54, egg5catch);
    assert_act_entry(ctx, 55, egg5bomb);
    assert_act_entry(ctx, 56, egg5bomb2);
    assert_act_entry(ctx, 57, clear);
    assert_act_entry(ctx, 58, over);
    assert_act_entry(ctx, 59, title);
}

static void test_act53_table_entries_dispatch_to_selected_handlers(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act53_state();

    act_tbl[31](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    act_tbl[49](&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[56](&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
}

static void test_debugact_increments_and_wraps_positions(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act53_state();
    actor.xposi.w.h = 299;
    actor.yposi.w.h = 199;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 300, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_act53_state();
    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_noact_frameouts_actor(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act53_state();
    actor.actno = 9;
    actor.xposi.w.h = 111;
    actor.yposi.w.h = 222;

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, stub_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 111, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 222, actor.yposi.w.h);
}

TEST_MAIN_BEGIN;
    test_act53_table_captures_all_dispatch_slots(&ctx);
    test_act53_table_entries_dispatch_to_selected_handlers(&ctx);
    test_debugact_increments_and_wraps_positions(&ctx);
    test_noact_frameouts_actor(&ctx);
TEST_MAIN_END

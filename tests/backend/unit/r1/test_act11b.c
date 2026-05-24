#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int stub_call_count;
static sprite_status *stub_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#define DECLARE_ACT_STUB(name) void name(sprite_status *pActwk)
DECLARE_ACT_STUB(play00);
DECLARE_ACT_STUB(baria);
DECLARE_ACT_STUB(taki);
DECLARE_ACT_STUB(jisin);
DECLARE_ACT_STUB(test_act);
DECLARE_ACT_STUB(shooter);
DECLARE_ACT_STUB(jisin_set);
DECLARE_ACT_STUB(dai_k);
DECLARE_ACT_STUB(spring);
DECLARE_ACT_STUB(exit2);
DECLARE_ACT_STUB(exit2_set);
DECLARE_ACT_STUB(futa);
DECLARE_ACT_STUB(mizukiri);
DECLARE_ACT_STUB(koma);
DECLARE_ACT_STUB(ring);
DECLARE_ACT_STUB(flyring);
DECLARE_ACT_STUB(block);
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
DECLARE_ACT_STUB(kuzureru_tikei);
DECLARE_ACT_STUB(dodai);
DECLARE_ACT_STUB(ene_kamemusi);
DECLARE_ACT_STUB(ene_tama);
DECLARE_ACT_STUB(friend);
DECLARE_ACT_STUB(hariyama);
DECLARE_ACT_STUB(tobita1);
DECLARE_ACT_STUB(marker);
DECLARE_ACT_STUB(et);
DECLARE_ACT_STUB(movie1);
DECLARE_ACT_STUB(ana);
DECLARE_ACT_STUB(eda);
DECLARE_ACT_STUB(kage);
DECLARE_ACT_STUB(ball);
DECLARE_ACT_STUB(vfuta);
DECLARE_ACT_STUB(kowasi);
DECLARE_ACT_STUB(clear);
DECLARE_ACT_STUB(over);
DECLARE_ACT_STUB(title);
DECLARE_ACT_STUB(ene_ka);
DECLARE_ACT_STUB(ene_chou);
DECLARE_ACT_STUB(ene_ari);
DECLARE_ACT_STUB(ene_tagame_a);
#undef DECLARE_ACT_STUB

#include "src/r1/act11b.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void stub_record(sprite_status *pActwk) {
    ++stub_call_count;
    stub_actor = pActwk;
}

#define DEFINE_ACT_STUB(name)                                                  \
    void name(sprite_status *pActwk) { stub_record(pActwk); }
DEFINE_ACT_STUB(play00)
DEFINE_ACT_STUB(baria)
DEFINE_ACT_STUB(taki)
DEFINE_ACT_STUB(jisin)
DEFINE_ACT_STUB(test_act)
DEFINE_ACT_STUB(shooter)
DEFINE_ACT_STUB(jisin_set)
DEFINE_ACT_STUB(dai_k)
DEFINE_ACT_STUB(spring)
DEFINE_ACT_STUB(exit2)
DEFINE_ACT_STUB(exit2_set)
DEFINE_ACT_STUB(futa)
DEFINE_ACT_STUB(mizukiri)
DEFINE_ACT_STUB(koma)
DEFINE_ACT_STUB(ring)
DEFINE_ACT_STUB(flyring)
DEFINE_ACT_STUB(block)
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
DEFINE_ACT_STUB(kuzureru_tikei)
DEFINE_ACT_STUB(dodai)
DEFINE_ACT_STUB(ene_kamemusi)
DEFINE_ACT_STUB(ene_tama)
DEFINE_ACT_STUB(friend)
DEFINE_ACT_STUB(hariyama)
DEFINE_ACT_STUB(tobita1)
DEFINE_ACT_STUB(marker)
DEFINE_ACT_STUB(et)
DEFINE_ACT_STUB(movie1)
DEFINE_ACT_STUB(ana)
DEFINE_ACT_STUB(eda)
DEFINE_ACT_STUB(kage)
DEFINE_ACT_STUB(ball)
DEFINE_ACT_STUB(vfuta)
DEFINE_ACT_STUB(kowasi)
DEFINE_ACT_STUB(clear)
DEFINE_ACT_STUB(over)
DEFINE_ACT_STUB(title)
DEFINE_ACT_STUB(ene_ka)
DEFINE_ACT_STUB(ene_chou)
DEFINE_ACT_STUB(ene_ari)
DEFINE_ACT_STUB(ene_tagame_a)
#undef DEFINE_ACT_STUB

static void reset_act11b_state(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void assert_act_entry(test_context *ctx, int index,
                             void (*expected)(sprite_status *)) {
    TEST_ASSERT_TRUE(ctx, act_tbl[index] == expected);
}

static void test_act11b_table_captures_dispatch_slots(test_context *ctx) {
    assert_act_entry(ctx, 0, play00);
    assert_act_entry(ctx, 1, play00);
    assert_act_entry(ctx, 2, baria);
    assert_act_entry(ctx, 3, taki);
    assert_act_entry(ctx, 4, jisin);
    assert_act_entry(ctx, 5, test_act);
    assert_act_entry(ctx, 6, shooter);
    assert_act_entry(ctx, 7, jisin_set);
    assert_act_entry(ctx, 8, dai_k);
    assert_act_entry(ctx, 9, spring);
    assert_act_entry(ctx, 10, exit2);
    assert_act_entry(ctx, 11, exit2_set);
    assert_act_entry(ctx, 12, futa);
    assert_act_entry(ctx, 13, mizukiri);
    assert_act_entry(ctx, 14, koma);
    assert_act_entry(ctx, 15, ring);
    assert_act_entry(ctx, 16, flyring);
    assert_act_entry(ctx, 17, block);
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
    assert_act_entry(ctx, 31, kuzureru_tikei);
    assert_act_entry(ctx, 32, dodai);
    assert_act_entry(ctx, 33, ene_kamemusi);
    assert_act_entry(ctx, 34, ene_tama);
    assert_act_entry(ctx, 35, friend);
    assert_act_entry(ctx, 36, noact);
    assert_act_entry(ctx, 37, hariyama);
    assert_act_entry(ctx, 38, noact);
    assert_act_entry(ctx, 39, tobita1);
    assert_act_entry(ctx, 40, noact);
    assert_act_entry(ctx, 41, noact);
    assert_act_entry(ctx, 42, marker);
    assert_act_entry(ctx, 43, noact);
    assert_act_entry(ctx, 44, et);
    assert_act_entry(ctx, 45, movie1);
    assert_act_entry(ctx, 46, noact);
    assert_act_entry(ctx, 47, noact);
    assert_act_entry(ctx, 48, ana);
    assert_act_entry(ctx, 49, eda);
    assert_act_entry(ctx, 50, kage);
    assert_act_entry(ctx, 51, ball);
    assert_act_entry(ctx, 52, vfuta);
    assert_act_entry(ctx, 53, kowasi);
    assert_act_entry(ctx, 54, noact);
    assert_act_entry(ctx, 55, noact);
    assert_act_entry(ctx, 56, noact);
    assert_act_entry(ctx, 57, clear);
    assert_act_entry(ctx, 58, over);
    assert_act_entry(ctx, 59, title);
    assert_act_entry(ctx, 60, ene_ka);
    assert_act_entry(ctx, 61, ene_chou);
    assert_act_entry(ctx, 62, ene_ari);
    assert_act_entry(ctx, 63, ene_tagame_a);
}

static void test_act11b_table_entries_dispatch_to_handlers(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act11b_state();

    act_tbl[0](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    act_tbl[31](&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[53](&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[18](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

static void test_debugact_wraps_coordinates_and_draws(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act11b_state();
    actor.xposi.w.h = 300;
    actor.yposi.w.h = 200;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);

    reset_act11b_state();
    actor.xposi.w.h = 299;
    actor.yposi.w.h = 199;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 300, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_noact_frameouts_actor(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act11b_state();
    actor.actno = 9;
    actor.xposi.w.h = 111;
    actor.yposi.w.h = 222;

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, stub_call_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 111, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 222, actor.yposi.w.h);
}

TEST_MAIN_BEGIN;
    test_act11b_table_captures_dispatch_slots(&ctx);
    test_act11b_table_entries_dispatch_to_handlers(&ctx);
    test_debugact_wraps_coordinates_and_draws(&ctx);
    test_noact_frameouts_actor(&ctx);
TEST_MAIN_END

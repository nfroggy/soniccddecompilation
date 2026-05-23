#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int frameout_count;
static sprite_status *frameout_actor;
static int stub_call_count;
static sprite_status *stub_actor;

void frameout(sprite_status *pActwk);

#define DECLARE_ACT_STUB(name) void name(sprite_status *pActwk)
DECLARE_ACT_STUB(play00);
DECLARE_ACT_STUB(baria);
DECLARE_ACT_STUB(test_act);
DECLARE_ACT_STUB(spring);
DECLARE_ACT_STUB(exit2);
DECLARE_ACT_STUB(exit2_set);
DECLARE_ACT_STUB(futa);
DECLARE_ACT_STUB(mizukiri);
DECLARE_ACT_STUB(koma);
DECLARE_ACT_STUB(ring);
DECLARE_ACT_STUB(flyring);
DECLARE_ACT_STUB(block);
DECLARE_ACT_STUB(marker);
DECLARE_ACT_STUB(bigring);
DECLARE_ACT_STUB(gene);
DECLARE_ACT_STUB(mosugu);
DECLARE_ACT_STUB(goal);
DECLARE_ACT_STUB(bakuha);
DECLARE_ACT_STUB(item);
DECLARE_ACT_STUB(item2);
DECLARE_ACT_STUB(iwa);
DECLARE_ACT_STUB(score);
DECLARE_ACT_STUB(bobin);
DECLARE_ACT_STUB(frip);
DECLARE_ACT_STUB(flower);
DECLARE_ACT_STUB(harir3);
DECLARE_ACT_STUB(frdr3);
DECLARE_ACT_STUB(trapdr3);
DECLARE_ACT_STUB(for3);
DECLARE_ACT_STUB(ene_kama);
DECLARE_ACT_STUB(tama_kama);
DECLARE_ACT_STUB(getdair3);
DECLARE_ACT_STUB(gandair3);
DECLARE_ACT_STUB(drumr3);
DECLARE_ACT_STUB(bobinbreak);
DECLARE_ACT_STUB(banpa);
DECLARE_ACT_STUB(miracle);
DECLARE_ACT_STUB(togeball);
DECLARE_ACT_STUB(ga);
DECLARE_ACT_STUB(tentou);
DECLARE_ACT_STUB(pocket);
DECLARE_ACT_STUB(ball);
DECLARE_ACT_STUB(et);
DECLARE_ACT_STUB(movie);
DECLARE_ACT_STUB(friend);
DECLARE_ACT_STUB(clear);
DECLARE_ACT_STUB(over);
DECLARE_ACT_STUB(title);
#undef DECLARE_ACT_STUB

#include "src/r3/act32a.c"

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
DEFINE_ACT_STUB(test_act)
DEFINE_ACT_STUB(spring)
DEFINE_ACT_STUB(exit2)
DEFINE_ACT_STUB(exit2_set)
DEFINE_ACT_STUB(futa)
DEFINE_ACT_STUB(mizukiri)
DEFINE_ACT_STUB(koma)
DEFINE_ACT_STUB(ring)
DEFINE_ACT_STUB(flyring)
DEFINE_ACT_STUB(block)
DEFINE_ACT_STUB(marker)
DEFINE_ACT_STUB(bigring)
DEFINE_ACT_STUB(gene)
DEFINE_ACT_STUB(mosugu)
DEFINE_ACT_STUB(goal)
DEFINE_ACT_STUB(bakuha)
DEFINE_ACT_STUB(item)
DEFINE_ACT_STUB(item2)
DEFINE_ACT_STUB(iwa)
DEFINE_ACT_STUB(score)
DEFINE_ACT_STUB(bobin)
DEFINE_ACT_STUB(frip)
DEFINE_ACT_STUB(flower)
DEFINE_ACT_STUB(harir3)
DEFINE_ACT_STUB(frdr3)
DEFINE_ACT_STUB(trapdr3)
DEFINE_ACT_STUB(for3)
DEFINE_ACT_STUB(ene_kama)
DEFINE_ACT_STUB(tama_kama)
DEFINE_ACT_STUB(getdair3)
DEFINE_ACT_STUB(gandair3)
DEFINE_ACT_STUB(drumr3)
DEFINE_ACT_STUB(bobinbreak)
DEFINE_ACT_STUB(banpa)
DEFINE_ACT_STUB(miracle)
DEFINE_ACT_STUB(togeball)
DEFINE_ACT_STUB(ga)
DEFINE_ACT_STUB(tentou)
DEFINE_ACT_STUB(pocket)
DEFINE_ACT_STUB(ball)
DEFINE_ACT_STUB(et)
DEFINE_ACT_STUB(movie)
DEFINE_ACT_STUB(friend)
DEFINE_ACT_STUB(clear)
DEFINE_ACT_STUB(over)
DEFINE_ACT_STUB(title)
#undef DEFINE_ACT_STUB

static void reset_act32a_state(void) {
    frameout_count = 0;
    frameout_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void assert_act_entry(test_context *ctx, int index,
                             void (*expected)(sprite_status *)) {
    TEST_ASSERT_TRUE(ctx, act_tbl[index] == expected);
}

static void test_act32a_table_captures_all_dispatch_slots(test_context *ctx) {
    assert_act_entry(ctx, 0, play00);
    assert_act_entry(ctx, 1, play00);
    assert_act_entry(ctx, 2, baria);
    assert_act_entry(ctx, 3, noact);
    assert_act_entry(ctx, 4, noact);
    assert_act_entry(ctx, 5, test_act);
    assert_act_entry(ctx, 6, noact);
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
    assert_act_entry(ctx, 17, block);
    assert_act_entry(ctx, 18, marker);
    assert_act_entry(ctx, 19, bigring);
    assert_act_entry(ctx, 20, gene);
    assert_act_entry(ctx, 21, mosugu);
    assert_act_entry(ctx, 22, goal);
    assert_act_entry(ctx, 23, bakuha);
    assert_act_entry(ctx, 24, item);
    assert_act_entry(ctx, 25, item2);
    assert_act_entry(ctx, 26, iwa);
    assert_act_entry(ctx, 27, score);
    assert_act_entry(ctx, 28, bobin);
    assert_act_entry(ctx, 29, frip);
    assert_act_entry(ctx, 30, flower);
    assert_act_entry(ctx, 31, harir3);
    assert_act_entry(ctx, 32, frdr3);
    assert_act_entry(ctx, 33, trapdr3);
    assert_act_entry(ctx, 34, for3);
    assert_act_entry(ctx, 35, ene_kama);
    assert_act_entry(ctx, 36, tama_kama);
    assert_act_entry(ctx, 37, getdair3);
    assert_act_entry(ctx, 38, gandair3);
    assert_act_entry(ctx, 39, drumr3);
    assert_act_entry(ctx, 40, bobinbreak);
    assert_act_entry(ctx, 41, banpa);
    assert_act_entry(ctx, 42, miracle);
    assert_act_entry(ctx, 43, togeball);
    assert_act_entry(ctx, 44, ga);
    assert_act_entry(ctx, 45, tentou);
    assert_act_entry(ctx, 46, pocket);
    assert_act_entry(ctx, 47, noact);
    assert_act_entry(ctx, 48, noact);
    assert_act_entry(ctx, 49, noact);
    assert_act_entry(ctx, 50, noact);
    assert_act_entry(ctx, 51, noact);
    assert_act_entry(ctx, 52, ball);
    assert_act_entry(ctx, 53, noact);
    assert_act_entry(ctx, 54, et);
    assert_act_entry(ctx, 55, movie);
    assert_act_entry(ctx, 56, friend);
    assert_act_entry(ctx, 57, clear);
    assert_act_entry(ctx, 58, over);
    assert_act_entry(ctx, 59, title);
}

static void test_act32a_table_entries_dispatch_to_selected_handlers(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act32a_state();

    act_tbl[0](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    act_tbl[18](&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stub_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[52](&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
}

static void test_noact_frameouts_actor(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_act32a_state();
    actor.actno = 10;
    actor.xposi.w.h = 333;
    actor.yposi.w.h = 444;

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, stub_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 333, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 444, actor.yposi.w.h);
}

TEST_MAIN_BEGIN;
    test_act32a_table_captures_all_dispatch_slots(&ctx);
    test_act32a_table_entries_dispatch_to_selected_handlers(&ctx);
    test_noact_frameouts_actor(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#include "src/r8/act83.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

#define DEFINE_ACTOR_STUB(name)                                                \
    void name(sprite_status *pActwk) { (void)pActwk; }

DEFINE_ACTOR_STUB(play00)
DEFINE_ACTOR_STUB(baria)
DEFINE_ACTOR_STUB(test_act)
DEFINE_ACTOR_STUB(chibi_coli)
DEFINE_ACTOR_STUB(spring)
DEFINE_ACTOR_STUB(exit2)
DEFINE_ACTOR_STUB(exit2_set)
DEFINE_ACTOR_STUB(futa)
DEFINE_ACTOR_STUB(mizukiri)
DEFINE_ACTOR_STUB(koma8)
DEFINE_ACTOR_STUB(ring)
DEFINE_ACTOR_STUB(flyring)
DEFINE_ACTOR_STUB(bigring)
DEFINE_ACTOR_STUB(gene)
DEFINE_ACTOR_STUB(mosugu)
DEFINE_ACTOR_STUB(goal)
DEFINE_ACTOR_STUB(bakuha)
DEFINE_ACTOR_STUB(item)
DEFINE_ACTOR_STUB(item2)
DEFINE_ACTOR_STUB(iwa)
DEFINE_ACTOR_STUB(score)
DEFINE_ACTOR_STUB(bobin)
DEFINE_ACTOR_STUB(flower)
DEFINE_ACTOR_STUB(sw)
DEFINE_ACTOR_STUB(haguruma)
DEFINE_ACTOR_STUB(kabuto)
DEFINE_ACTOR_STUB(kuzure8)
DEFINE_ACTOR_STUB(nokogiri)
DEFINE_ACTOR_STUB(tobira)
DEFINE_ACTOR_STUB(konbea)
DEFINE_ACTOR_STUB(jettoge)
DEFINE_ACTOR_STUB(dai8)
DEFINE_ACTOR_STUB(friend)
DEFINE_ACTOR_STUB(togeita)
DEFINE_ACTOR_STUB(piston)
DEFINE_ACTOR_STUB(togedair8)
DEFINE_ACTOR_STUB(hotaru8)
DEFINE_ACTOR_STUB(clear)
DEFINE_ACTOR_STUB(over)
DEFINE_ACTOR_STUB(title)
DEFINE_ACTOR_STUB(shut)
DEFINE_ACTOR_STUB(kaiten)
DEFINE_ACTOR_STUB(egg8)
DEFINE_ACTOR_STUB(egg8hane)
DEFINE_ACTOR_STUB(egg8meca)
DEFINE_ACTOR_STUB(egg8hibana)
DEFINE_ACTOR_STUB(emie8)

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void assert_act_entry(test_context *ctx, int index,
                             void (*expected)(sprite_status *)) {
    TEST_ASSERT_TRUE(ctx, act_tbl[index] == expected);
}

static void test_act83_table_captures_exact_actor_routing(test_context *ctx) {
    assert_act_entry(ctx, 0, play00);
    assert_act_entry(ctx, 1, play00);
    assert_act_entry(ctx, 2, baria);
    assert_act_entry(ctx, 3, noact);
    assert_act_entry(ctx, 4, noact);
    assert_act_entry(ctx, 5, test_act);
    assert_act_entry(ctx, 6, noact);
    assert_act_entry(ctx, 7, noact);
    assert_act_entry(ctx, 8, chibi_coli);
    assert_act_entry(ctx, 9, spring);
    assert_act_entry(ctx, 10, exit2);
    assert_act_entry(ctx, 11, exit2_set);
    assert_act_entry(ctx, 12, futa);
    assert_act_entry(ctx, 13, mizukiri);
    assert_act_entry(ctx, 14, koma8);
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
    assert_act_entry(ctx, 28, bobin);
    assert_act_entry(ctx, 29, noact);
    assert_act_entry(ctx, 30, flower);
    assert_act_entry(ctx, 31, noact);
    assert_act_entry(ctx, 32, sw);
    assert_act_entry(ctx, 33, haguruma);
    assert_act_entry(ctx, 34, noact);
    assert_act_entry(ctx, 35, kabuto);
    assert_act_entry(ctx, 36, kuzure8);
    assert_act_entry(ctx, 37, noact);
    assert_act_entry(ctx, 38, nokogiri);
    assert_act_entry(ctx, 39, noact);
    assert_act_entry(ctx, 40, tobira);
    assert_act_entry(ctx, 41, konbea);
    assert_act_entry(ctx, 42, jettoge);
    assert_act_entry(ctx, 43, dai8);
    assert_act_entry(ctx, 44, noact);
    assert_act_entry(ctx, 45, noact);
    assert_act_entry(ctx, 46, noact);
    assert_act_entry(ctx, 47, noact);
    assert_act_entry(ctx, 48, noact);
    assert_act_entry(ctx, 49, friend);
    assert_act_entry(ctx, 50, togeita);
    assert_act_entry(ctx, 51, piston);
    assert_act_entry(ctx, 52, noact);
    assert_act_entry(ctx, 53, togedair8);
    assert_act_entry(ctx, 54, noact);
    assert_act_entry(ctx, 55, noact);
    assert_act_entry(ctx, 56, hotaru8);
    assert_act_entry(ctx, 57, clear);
    assert_act_entry(ctx, 58, over);
    assert_act_entry(ctx, 59, title);
    assert_act_entry(ctx, 60, shut);
    assert_act_entry(ctx, 61, kaiten);
    assert_act_entry(ctx, 62, egg8);
    assert_act_entry(ctx, 63, egg8hane);
    assert_act_entry(ctx, 64, egg8meca);
    assert_act_entry(ctx, 65, egg8hibana);
    assert_act_entry(ctx, 66, emie8);
}

static void test_debugact_increments_then_wraps_positions(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_logs();
    actor.xposi.w.h = 299;
    actor.yposi.w.h = 199;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 300, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);

    reset_logs();
    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_noact_frames_out_actor(test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    reset_logs();

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_act83_table_captures_exact_actor_routing(&ctx);
    test_debugact_increments_then_wraps_positions(&ctx);
    test_noact_frames_out_actor(&ctx);
TEST_MAIN_END

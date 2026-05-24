#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int frameout_count;
static sprite_status *frameout_actor;
static int stub_call_count;
static sprite_status *stub_actor;

void frameout(sprite_status *pActwk);

#define DECLARE_ACTOR_STUB(name) void name(sprite_status *actor)
DECLARE_ACTOR_STUB(play00);
DECLARE_ACTOR_STUB(baria);
DECLARE_ACTOR_STUB(catapalt);
DECLARE_ACTOR_STUB(gas);
DECLARE_ACTOR_STUB(test_act);
DECLARE_ACTOR_STUB(shooter);
DECLARE_ACTOR_STUB(megami);
DECLARE_ACTOR_STUB(spring);
DECLARE_ACTOR_STUB(exit2);
DECLARE_ACTOR_STUB(exit2_set);
DECLARE_ACTOR_STUB(futa);
DECLARE_ACTOR_STUB(mizukiri);
DECLARE_ACTOR_STUB(koma);
DECLARE_ACTOR_STUB(ring);
DECLARE_ACTOR_STUB(flyring);
DECLARE_ACTOR_STUB(marker);
DECLARE_ACTOR_STUB(bigring);
DECLARE_ACTOR_STUB(gene);
DECLARE_ACTOR_STUB(mosugu);
DECLARE_ACTOR_STUB(goal);
DECLARE_ACTOR_STUB(bakuha);
DECLARE_ACTOR_STUB(item);
DECLARE_ACTOR_STUB(item2);
DECLARE_ACTOR_STUB(iwa);
DECLARE_ACTOR_STUB(score);
DECLARE_ACTOR_STUB(ball);
DECLARE_ACTOR_STUB(flower);
DECLARE_ACTOR_STUB(piston6);
DECLARE_ACTOR_STUB(beem6);
DECLARE_ACTOR_STUB(dair6);
DECLARE_ACTOR_STUB(tobidai6);
DECLARE_ACTOR_STUB(sw6);
DECLARE_ACTOR_STUB(tobira6);
DECLARE_ACTOR_STUB(optbr6);
DECLARE_ACTOR_STUB(kdai6);
DECLARE_ACTOR_STUB(friend);
DECLARE_ACTOR_STUB(block6);
DECLARE_ACTOR_STUB(eggz6);
DECLARE_ACTOR_STUB(seesaw6);
DECLARE_ACTOR_STUB(et);
DECLARE_ACTOR_STUB(movie);
DECLARE_ACTOR_STUB(batta);
DECLARE_ACTOR_STUB(hachi6);
DECLARE_ACTOR_STUB(semi);
DECLARE_ACTOR_STUB(minomusi);
DECLARE_ACTOR_STUB(clear);
DECLARE_ACTOR_STUB(over);
DECLARE_ACTOR_STUB(title);

#include "src/r6/act61a.c"

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

#define DEFINE_ACTOR_STUB(name)                                                \
    void name(sprite_status *actor) {                                          \
        ++stub_call_count;                                                     \
        stub_actor = actor;                                                    \
    }

DEFINE_ACTOR_STUB(play00)
DEFINE_ACTOR_STUB(baria)
DEFINE_ACTOR_STUB(catapalt)
DEFINE_ACTOR_STUB(gas)
DEFINE_ACTOR_STUB(test_act)
DEFINE_ACTOR_STUB(shooter)
DEFINE_ACTOR_STUB(megami)
DEFINE_ACTOR_STUB(spring)
DEFINE_ACTOR_STUB(exit2)
DEFINE_ACTOR_STUB(exit2_set)
DEFINE_ACTOR_STUB(futa)
DEFINE_ACTOR_STUB(mizukiri)
DEFINE_ACTOR_STUB(koma)
DEFINE_ACTOR_STUB(ring)
DEFINE_ACTOR_STUB(flyring)
DEFINE_ACTOR_STUB(marker)
DEFINE_ACTOR_STUB(bigring)
DEFINE_ACTOR_STUB(gene)
DEFINE_ACTOR_STUB(mosugu)
DEFINE_ACTOR_STUB(goal)
DEFINE_ACTOR_STUB(bakuha)
DEFINE_ACTOR_STUB(item)
DEFINE_ACTOR_STUB(item2)
DEFINE_ACTOR_STUB(iwa)
DEFINE_ACTOR_STUB(score)
DEFINE_ACTOR_STUB(ball)
DEFINE_ACTOR_STUB(flower)
DEFINE_ACTOR_STUB(piston6)
DEFINE_ACTOR_STUB(beem6)
DEFINE_ACTOR_STUB(dair6)
DEFINE_ACTOR_STUB(tobidai6)
DEFINE_ACTOR_STUB(sw6)
DEFINE_ACTOR_STUB(tobira6)
DEFINE_ACTOR_STUB(optbr6)
DEFINE_ACTOR_STUB(kdai6)
DEFINE_ACTOR_STUB(friend)
DEFINE_ACTOR_STUB(block6)
DEFINE_ACTOR_STUB(eggz6)
DEFINE_ACTOR_STUB(seesaw6)
DEFINE_ACTOR_STUB(et)
DEFINE_ACTOR_STUB(movie)
DEFINE_ACTOR_STUB(batta)
DEFINE_ACTOR_STUB(hachi6)
DEFINE_ACTOR_STUB(semi)
DEFINE_ACTOR_STUB(minomusi)
DEFINE_ACTOR_STUB(clear)
DEFINE_ACTOR_STUB(over)
DEFINE_ACTOR_STUB(title)

static void reset_act61a_state(void) {
    frameout_count = 0;
    frameout_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void test_act61a_table_matches_current_handlers(test_context *ctx) {
    void (*expected[60])(sprite_status *) = {
        play00, play00,   baria,   catapalt, gas,       test_act, shooter,
        megami, noact,    spring,  exit2,    exit2_set, futa,     mizukiri,
        koma,   ring,     flyring, noact,    marker,    bigring,  gene,
        mosugu, goal,     bakuha,  item,     item2,     iwa,      score,
        noact,  ball,     flower,  piston6,  beem6,     dair6,    tobidai6,
        sw6,    tobira6,  noact,   optbr6,   kdai6,     friend,   block6,
        eggz6,  seesaw6,  noact,   et,       movie,     batta,    hachi6,
        semi,   minomusi, noact,   noact,    noact,     noact,    noact,
        noact,  clear,    over,    title};

    for (int i = 0; i < 60; ++i) {
        TEST_ASSERT_TRUE(ctx, act_tbl[i] == expected[i]);
    }
}

static void test_noact_delegates_to_frameout(test_context *ctx) {
    sprite_status actor;

    reset_act61a_state();
    memset(&actor, 0, sizeof(actor));

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

static void test_table_entries_are_callable(test_context *ctx) {
    sprite_status actor;

    reset_act61a_state();
    memset(&actor, 0, sizeof(actor));

    act_tbl[0](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[8](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);

    act_tbl[59](&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
}

TEST_MAIN_BEGIN;
    test_act61a_table_matches_current_handlers(&ctx);
    test_noact_delegates_to_frameout(&ctx);
    test_table_entries_are_callable(&ctx);
TEST_MAIN_END

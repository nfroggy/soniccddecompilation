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
DECLARE_ACTOR_STUB(test_act);
DECLARE_ACTOR_STUB(shooter);
DECLARE_ACTOR_STUB(spring);
DECLARE_ACTOR_STUB(exit2);
DECLARE_ACTOR_STUB(exit2_set);
DECLARE_ACTOR_STUB(futa);
DECLARE_ACTOR_STUB(mizukiri);
DECLARE_ACTOR_STUB(koma);
DECLARE_ACTOR_STUB(ring);
DECLARE_ACTOR_STUB(flyring);
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
DECLARE_ACTOR_STUB(udblk6);
DECLARE_ACTOR_STUB(optbr6);
DECLARE_ACTOR_STUB(kdai6);
DECLARE_ACTOR_STUB(friend);
DECLARE_ACTOR_STUB(block6);
DECLARE_ACTOR_STUB(eggz6);
DECLARE_ACTOR_STUB(seesaw6);
DECLARE_ACTOR_STUB(togeball);
DECLARE_ACTOR_STUB(et);
DECLARE_ACTOR_STUB(movie);
DECLARE_ACTOR_STUB(egg6boss);
DECLARE_ACTOR_STUB(egg6meca0);
DECLARE_ACTOR_STUB(egg6meca1);
DECLARE_ACTOR_STUB(egg6meca2);
DECLARE_ACTOR_STUB(egg6toge);
DECLARE_ACTOR_STUB(egg6gareki);
DECLARE_ACTOR_STUB(clear);
DECLARE_ACTOR_STUB(over);
DECLARE_ACTOR_STUB(title);
DECLARE_ACTOR_STUB(egg6yuka);
DECLARE_ACTOR_STUB(egg6door);
DECLARE_ACTOR_STUB(egg6bakuha);
DECLARE_ACTOR_STUB(egg6beam);

#include "src/r6/act63.c"

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
DEFINE_ACTOR_STUB(test_act)
DEFINE_ACTOR_STUB(shooter)
DEFINE_ACTOR_STUB(spring)
DEFINE_ACTOR_STUB(exit2)
DEFINE_ACTOR_STUB(exit2_set)
DEFINE_ACTOR_STUB(futa)
DEFINE_ACTOR_STUB(mizukiri)
DEFINE_ACTOR_STUB(koma)
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
DEFINE_ACTOR_STUB(ball)
DEFINE_ACTOR_STUB(flower)
DEFINE_ACTOR_STUB(piston6)
DEFINE_ACTOR_STUB(beem6)
DEFINE_ACTOR_STUB(dair6)
DEFINE_ACTOR_STUB(tobidai6)
DEFINE_ACTOR_STUB(sw6)
DEFINE_ACTOR_STUB(tobira6)
DEFINE_ACTOR_STUB(udblk6)
DEFINE_ACTOR_STUB(optbr6)
DEFINE_ACTOR_STUB(kdai6)
DEFINE_ACTOR_STUB(friend)
DEFINE_ACTOR_STUB(block6)
DEFINE_ACTOR_STUB(eggz6)
DEFINE_ACTOR_STUB(seesaw6)
DEFINE_ACTOR_STUB(togeball)
DEFINE_ACTOR_STUB(et)
DEFINE_ACTOR_STUB(movie)
DEFINE_ACTOR_STUB(egg6boss)
DEFINE_ACTOR_STUB(egg6meca0)
DEFINE_ACTOR_STUB(egg6meca1)
DEFINE_ACTOR_STUB(egg6meca2)
DEFINE_ACTOR_STUB(egg6toge)
DEFINE_ACTOR_STUB(egg6gareki)
DEFINE_ACTOR_STUB(clear)
DEFINE_ACTOR_STUB(over)
DEFINE_ACTOR_STUB(title)
DEFINE_ACTOR_STUB(egg6yuka)
DEFINE_ACTOR_STUB(egg6door)
DEFINE_ACTOR_STUB(egg6bakuha)
DEFINE_ACTOR_STUB(egg6beam)

static void reset_act63_state(void) {
    frameout_count = 0;
    frameout_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void test_act63_table_matches_current_handlers(test_context *ctx) {
    void (*expected[64])(sprite_status *) = {
        play00,    play00,   baria,      noact,    noact,     test_act,
        shooter,   noact,    noact,      spring,   exit2,     exit2_set,
        futa,      mizukiri, koma,       ring,     flyring,   noact,
        noact,     bigring,  gene,       mosugu,   goal,      bakuha,
        item,      item2,    iwa,        score,    noact,     ball,
        flower,    piston6,  beem6,      dair6,    tobidai6,  sw6,
        tobira6,   udblk6,   optbr6,     kdai6,    friend,    block6,
        eggz6,     seesaw6,  togeball,   et,       movie,     noact,
        noact,     noact,    noact,      egg6boss, egg6meca0, egg6meca1,
        egg6meca2, egg6toge, egg6gareki, clear,    over,      title,
        egg6yuka,  egg6door, egg6bakuha, egg6beam};

    for (int i = 0; i < 64; ++i) {
        TEST_ASSERT_TRUE(ctx, act_tbl[i] == expected[i]);
    }
}

static void test_noact_delegates_to_frameout(test_context *ctx) {
    sprite_status actor;

    reset_act63_state();
    memset(&actor, 0, sizeof(actor));

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

static void test_boss_specific_table_entries_are_callable(test_context *ctx) {
    sprite_status actor;

    reset_act63_state();
    memset(&actor, 0, sizeof(actor));

    act_tbl[51](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[60](&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[63](&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);
}

TEST_MAIN_BEGIN;
    test_act63_table_matches_current_handlers(&ctx);
    test_noact_delegates_to_frameout(&ctx);
    test_boss_specific_table_entries_are_callable(&ctx);
TEST_MAIN_END

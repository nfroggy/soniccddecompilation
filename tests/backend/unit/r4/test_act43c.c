#include <stddef.h>
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
DECLARE_ACT_STUB(wave);
DECLARE_ACT_STUB(test_act);
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
DECLARE_ACT_STUB(awa);
DECLARE_ACT_STUB(plawa);
DECLARE_ACT_STUB(tonbo);
DECLARE_ACT_STUB(friend4);
DECLARE_ACT_STUB(wall1);
DECLARE_ACT_STUB(clear);
DECLARE_ACT_STUB(over);
DECLARE_ACT_STUB(title);
DECLARE_ACT_STUB(renketu4);
DECLARE_ACT_STUB(wall4);
DECLARE_ACT_STUB(egg4air);
DECLARE_ACT_STUB(egg4airhead);
DECLARE_ACT_STUB(egg4);
DECLARE_ACT_STUB(egg4meca);
DECLARE_ACT_STUB(egg4tama);
DECLARE_ACT_STUB(egg4awa);

#include "src/r4/act43c.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

#define DEFINE_ACT_STUB(name)                                                 \
    void name(sprite_status *pActwk) {                                        \
        ++stub_call_count;                                                    \
        stub_actor = pActwk;                                                  \
    }

DEFINE_ACT_STUB(play00)
DEFINE_ACT_STUB(baria)
DEFINE_ACT_STUB(wave)
DEFINE_ACT_STUB(test_act)
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
DEFINE_ACT_STUB(awa)
DEFINE_ACT_STUB(plawa)
DEFINE_ACT_STUB(tonbo)
DEFINE_ACT_STUB(friend4)
DEFINE_ACT_STUB(wall1)
DEFINE_ACT_STUB(clear)
DEFINE_ACT_STUB(over)
DEFINE_ACT_STUB(title)
DEFINE_ACT_STUB(renketu4)
DEFINE_ACT_STUB(wall4)
DEFINE_ACT_STUB(egg4air)
DEFINE_ACT_STUB(egg4airhead)
DEFINE_ACT_STUB(egg4)
DEFINE_ACT_STUB(egg4meca)
DEFINE_ACT_STUB(egg4tama)
DEFINE_ACT_STUB(egg4awa)

static void reset_state(sprite_status *actor) {
    memset(actor, 0, sizeof(*actor));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    stub_call_count = 0;
    stub_actor = 0;
}

static void test_debugact_advances_coordinates_and_draws(test_context *ctx) {
    sprite_status actor;
    reset_state(&actor);
    actor.xposi.w.h = 42;
    actor.yposi.w.h = 99;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 43, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_debugact_wraps_at_existing_limits(test_context *ctx) {
    sprite_status actor;
    reset_state(&actor);
    actor.xposi.w.h = 300;
    actor.yposi.w.h = 200;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_noact_frames_actor_out(test_context *ctx) {
    sprite_status actor;
    reset_state(&actor);

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_dispatch_table_preserves_r43c_slots(test_context *ctx) {
    sprite_status actor;
    reset_state(&actor);

    TEST_ASSERT_TRUE(ctx, act_tbl[0] == &play00);
    TEST_ASSERT_TRUE(ctx, act_tbl[4] == &noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[19] == &bigring);
    TEST_ASSERT_TRUE(ctx, act_tbl[31] == &awa);
    TEST_ASSERT_TRUE(ctx, act_tbl[42] == &tonbo);
    TEST_ASSERT_TRUE(ctx, act_tbl[46] == &friend4);
    TEST_ASSERT_TRUE(ctx, act_tbl[51] == &wall1);
    TEST_ASSERT_TRUE(ctx, act_tbl[57] == &clear);
    TEST_ASSERT_TRUE(ctx, act_tbl[61] == &renketu4);
    TEST_ASSERT_TRUE(ctx, act_tbl[70] == &wall4);
    TEST_ASSERT_TRUE(ctx, act_tbl[73] == &egg4air);
    TEST_ASSERT_TRUE(ctx, act_tbl[74] == &egg4airhead);
    TEST_ASSERT_TRUE(ctx, act_tbl[75] == &egg4);
    TEST_ASSERT_TRUE(ctx, act_tbl[76] == &egg4meca);
    TEST_ASSERT_TRUE(ctx, act_tbl[77] == &egg4tama);
    TEST_ASSERT_TRUE(ctx, act_tbl[78] == &egg4awa);
    TEST_ASSERT_TRUE(ctx, act_tbl[79] == &noact);

    act_tbl[75](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[79](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

TEST_MAIN_BEGIN;
test_debugact_advances_coordinates_and_draws(&ctx);
test_debugact_wraps_at_existing_limits(&ctx);
test_noact_frames_actor_out(&ctx);
test_dispatch_table_preserves_r43c_slots(&ctx);
TEST_MAIN_END

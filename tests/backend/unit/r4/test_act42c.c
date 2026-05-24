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
DECLARE_ACT_STUB(bou);
DECLARE_ACT_STUB(test_act);
DECLARE_ACT_STUB(spring);
DECLARE_ACT_STUB(exit2);
DECLARE_ACT_STUB(exit2_set);
DECLARE_ACT_STUB(futa);
DECLARE_ACT_STUB(mizukiri);
DECLARE_ACT_STUB(koma);
DECLARE_ACT_STUB(ring);
DECLARE_ACT_STUB(flyring);
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
DECLARE_ACT_STUB(flower);
DECLARE_ACT_STUB(awa);
DECLARE_ACT_STUB(plawa);
DECLARE_ACT_STUB(harir4);
DECLARE_ACT_STUB(udblk4);
DECLARE_ACT_STUB(lrblk4);
DECLARE_ACT_STUB(tobiras4);
DECLARE_ACT_STUB(sw4);
DECLARE_ACT_STUB(tobira4);
DECLARE_ACT_STUB(tonbo);
DECLARE_ACT_STUB(amenbo);
DECLARE_ACT_STUB(tagameb);
DECLARE_ACT_STUB(yago);
DECLARE_ACT_STUB(friend4);
DECLARE_ACT_STUB(walls);
DECLARE_ACT_STUB(wall1);
DECLARE_ACT_STUB(screw);
DECLARE_ACT_STUB(tekkyu);
DECLARE_ACT_STUB(tekkyu1);
DECLARE_ACT_STUB(tekkyu4);
DECLARE_ACT_STUB(kuzure4);
DECLARE_ACT_STUB(clear);
DECLARE_ACT_STUB(over);
DECLARE_ACT_STUB(title);
DECLARE_ACT_STUB(renketu4);
DECLARE_ACT_STUB(baneiwa);
DECLARE_ACT_STUB(daid4);
DECLARE_ACT_STUB(kowasi4);

#include "src/r4/act42c.c"

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
DEFINE_ACT_STUB(bou)
DEFINE_ACT_STUB(test_act)
DEFINE_ACT_STUB(spring)
DEFINE_ACT_STUB(exit2)
DEFINE_ACT_STUB(exit2_set)
DEFINE_ACT_STUB(futa)
DEFINE_ACT_STUB(mizukiri)
DEFINE_ACT_STUB(koma)
DEFINE_ACT_STUB(ring)
DEFINE_ACT_STUB(flyring)
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
DEFINE_ACT_STUB(flower)
DEFINE_ACT_STUB(awa)
DEFINE_ACT_STUB(plawa)
DEFINE_ACT_STUB(harir4)
DEFINE_ACT_STUB(udblk4)
DEFINE_ACT_STUB(lrblk4)
DEFINE_ACT_STUB(tobiras4)
DEFINE_ACT_STUB(sw4)
DEFINE_ACT_STUB(tobira4)
DEFINE_ACT_STUB(tonbo)
DEFINE_ACT_STUB(amenbo)
DEFINE_ACT_STUB(tagameb)
DEFINE_ACT_STUB(yago)
DEFINE_ACT_STUB(friend4)
DEFINE_ACT_STUB(walls)
DEFINE_ACT_STUB(wall1)
DEFINE_ACT_STUB(screw)
DEFINE_ACT_STUB(tekkyu)
DEFINE_ACT_STUB(tekkyu1)
DEFINE_ACT_STUB(tekkyu4)
DEFINE_ACT_STUB(kuzure4)
DEFINE_ACT_STUB(clear)
DEFINE_ACT_STUB(over)
DEFINE_ACT_STUB(title)
DEFINE_ACT_STUB(renketu4)
DEFINE_ACT_STUB(baneiwa)
DEFINE_ACT_STUB(daid4)
DEFINE_ACT_STUB(kowasi4)

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
    actor.xposi.w.h = 7;
    actor.yposi.w.h = 8;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 9, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_debugact_wraps_existing_limits(test_context *ctx) {
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

static void test_dispatch_table_preserves_r42c_slots(test_context *ctx) {
    sprite_status actor;
    reset_state(&actor);

    TEST_ASSERT_TRUE(ctx, act_tbl[0] == &play00);
    TEST_ASSERT_TRUE(ctx, act_tbl[6] == &noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[34] == &udblk4);
    TEST_ASSERT_TRUE(ctx, act_tbl[35] == &lrblk4);
    TEST_ASSERT_TRUE(ctx, act_tbl[36] == &tobiras4);
    TEST_ASSERT_TRUE(ctx, act_tbl[38] == &sw4);
    TEST_ASSERT_TRUE(ctx, act_tbl[39] == &tobira4);
    TEST_ASSERT_TRUE(ctx, act_tbl[53] == &tekkyu);
    TEST_ASSERT_TRUE(ctx, act_tbl[54] == &tekkyu1);
    TEST_ASSERT_TRUE(ctx, act_tbl[55] == &tekkyu4);
    TEST_ASSERT_TRUE(ctx, act_tbl[56] == &kuzure4);
    TEST_ASSERT_TRUE(ctx, act_tbl[61] == &renketu4);
    TEST_ASSERT_TRUE(ctx, act_tbl[71] == &baneiwa);
    TEST_ASSERT_TRUE(ctx, act_tbl[72] == &daid4);
    TEST_ASSERT_TRUE(ctx, act_tbl[79] == &kowasi4);

    act_tbl[79](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stub_call_count);
    TEST_ASSERT_TRUE(ctx, stub_actor == &actor);

    act_tbl[78](&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

TEST_MAIN_BEGIN;
test_debugact_advances_coordinates_and_draws(&ctx);
test_debugact_wraps_existing_limits(&ctx);
test_noact_frames_actor_out(&ctx);
test_dispatch_table_preserves_r42c_slots(&ctx);
TEST_MAIN_END

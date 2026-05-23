#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

void play00(sprite_status *pActwk);
void baria(sprite_status *pActwk);
void ami(sprite_status *pActwk);
void test_act(sprite_status *pActwk);
void spring(sprite_status *pActwk);
void exit2(sprite_status *pActwk);
void exit2_set(sprite_status *pActwk);
void futa(sprite_status *pActwk);
void mizukiri(sprite_status *pActwk);
void koma(sprite_status *pActwk);
void ring(sprite_status *pActwk);
void flyring(sprite_status *pActwk);
void bigring(sprite_status *pActwk);
void gene(sprite_status *pActwk);
void mosugu(sprite_status *pActwk);
void goal(sprite_status *pActwk);
void bakuha(sprite_status *pActwk);
void item(sprite_status *pActwk);
void item2(sprite_status *pActwk);
void iwa(sprite_status *pActwk);
void score(sprite_status *pActwk);
void flower(sprite_status *pActwk);
void branko7(sprite_status *pActwk);
void wall7(sprite_status *pActwk);
void tekkyu7(sprite_status *pActwk);
void tekkyu7j(sprite_status *pActwk);
void movie(sprite_status *pActwk);
void friend(sprite_status *pActwk);
void wall73(sprite_status *pActwk);
void egg7(sprite_status *pActwk);
void egg7jet(sprite_status *pActwk);
void egg7beam(sprite_status *pActwk);
void msnc(sprite_status *pActwk);
void msnc_ele(sprite_status *pActwk);
void msnc_bara(sprite_status *pActwk);
void hari73(sprite_status *pActwk);
void emie7(sprite_status *pActwk);
void heart7(sprite_status *pActwk);
void chgwall7(sprite_status *pActwk);
void gaitou73(sprite_status *pActwk);
void hahen73(sprite_status *pActwk);
void clear(sprite_status *pActwk);
void over(sprite_status *pActwk);
void title(sprite_status *pActwk);

#include "src/r7/act73.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

#define STUB_ACTOR(fn_name)                                                    \
    void fn_name(sprite_status *pActwk) { (void)pActwk; }

STUB_ACTOR(play00)
STUB_ACTOR(baria)
STUB_ACTOR(ami)
STUB_ACTOR(test_act)
STUB_ACTOR(spring)
STUB_ACTOR(exit2)
STUB_ACTOR(exit2_set)
STUB_ACTOR(futa)
STUB_ACTOR(mizukiri)
STUB_ACTOR(koma)
STUB_ACTOR(ring)
STUB_ACTOR(flyring)
STUB_ACTOR(bigring)
STUB_ACTOR(gene)
STUB_ACTOR(mosugu)
STUB_ACTOR(goal)
STUB_ACTOR(bakuha)
STUB_ACTOR(item)
STUB_ACTOR(item2)
STUB_ACTOR(iwa)
STUB_ACTOR(score)
STUB_ACTOR(flower)
STUB_ACTOR(branko7)
STUB_ACTOR(wall7)
STUB_ACTOR(tekkyu7)
STUB_ACTOR(tekkyu7j)
STUB_ACTOR(movie)
STUB_ACTOR(friend)
STUB_ACTOR(wall73)
STUB_ACTOR(egg7)
STUB_ACTOR(egg7jet)
STUB_ACTOR(egg7beam)
STUB_ACTOR(msnc)
STUB_ACTOR(msnc_ele)
STUB_ACTOR(msnc_bara)
STUB_ACTOR(hari73)
STUB_ACTOR(emie7)
STUB_ACTOR(heart7)
STUB_ACTOR(chgwall7)
STUB_ACTOR(gaitou73)
STUB_ACTOR(hahen73)
STUB_ACTOR(clear)
STUB_ACTOR(over)
STUB_ACTOR(title)

static void reset_act73_state(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void test_act73_table_captures_actor_dispatch_order(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, act_tbl[0] == play00);
    TEST_ASSERT_TRUE(ctx, act_tbl[1] == play00);
    TEST_ASSERT_TRUE(ctx, act_tbl[2] == baria);
    TEST_ASSERT_TRUE(ctx, act_tbl[3] == ami);
    TEST_ASSERT_TRUE(ctx, act_tbl[4] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[5] == test_act);
    TEST_ASSERT_TRUE(ctx, act_tbl[6] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[7] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[8] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[9] == spring);
    TEST_ASSERT_TRUE(ctx, act_tbl[10] == exit2);
    TEST_ASSERT_TRUE(ctx, act_tbl[11] == exit2_set);
    TEST_ASSERT_TRUE(ctx, act_tbl[12] == futa);
    TEST_ASSERT_TRUE(ctx, act_tbl[13] == mizukiri);
    TEST_ASSERT_TRUE(ctx, act_tbl[14] == koma);
    TEST_ASSERT_TRUE(ctx, act_tbl[15] == ring);
    TEST_ASSERT_TRUE(ctx, act_tbl[16] == flyring);
    TEST_ASSERT_TRUE(ctx, act_tbl[17] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[18] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[19] == bigring);
    TEST_ASSERT_TRUE(ctx, act_tbl[20] == gene);
    TEST_ASSERT_TRUE(ctx, act_tbl[21] == mosugu);
    TEST_ASSERT_TRUE(ctx, act_tbl[22] == goal);
    TEST_ASSERT_TRUE(ctx, act_tbl[23] == bakuha);
    TEST_ASSERT_TRUE(ctx, act_tbl[24] == item);
    TEST_ASSERT_TRUE(ctx, act_tbl[25] == item2);
    TEST_ASSERT_TRUE(ctx, act_tbl[26] == iwa);
    TEST_ASSERT_TRUE(ctx, act_tbl[27] == score);
    TEST_ASSERT_TRUE(ctx, act_tbl[28] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[29] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[30] == flower);
    TEST_ASSERT_TRUE(ctx, act_tbl[31] == branko7);
    TEST_ASSERT_TRUE(ctx, act_tbl[32] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[33] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[34] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[35] == wall7);
    TEST_ASSERT_TRUE(ctx, act_tbl[36] == tekkyu7);
    TEST_ASSERT_TRUE(ctx, act_tbl[37] == tekkyu7j);
    TEST_ASSERT_TRUE(ctx, act_tbl[38] == movie);
    TEST_ASSERT_TRUE(ctx, act_tbl[39] == friend);
    TEST_ASSERT_TRUE(ctx, act_tbl[40] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[41] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[42] == wall73);
    TEST_ASSERT_TRUE(ctx, act_tbl[43] == egg7);
    TEST_ASSERT_TRUE(ctx, act_tbl[44] == egg7jet);
    TEST_ASSERT_TRUE(ctx, act_tbl[45] == egg7beam);
    TEST_ASSERT_TRUE(ctx, act_tbl[46] == msnc);
    TEST_ASSERT_TRUE(ctx, act_tbl[47] == msnc_ele);
    TEST_ASSERT_TRUE(ctx, act_tbl[48] == msnc_bara);
    TEST_ASSERT_TRUE(ctx, act_tbl[49] == hari73);
    TEST_ASSERT_TRUE(ctx, act_tbl[50] == emie7);
    TEST_ASSERT_TRUE(ctx, act_tbl[51] == heart7);
    TEST_ASSERT_TRUE(ctx, act_tbl[52] == chgwall7);
    TEST_ASSERT_TRUE(ctx, act_tbl[53] == gaitou73);
    TEST_ASSERT_TRUE(ctx, act_tbl[54] == hahen73);
    TEST_ASSERT_TRUE(ctx, act_tbl[55] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[56] == noact);
    TEST_ASSERT_TRUE(ctx, act_tbl[57] == clear);
    TEST_ASSERT_TRUE(ctx, act_tbl[58] == over);
    TEST_ASSERT_TRUE(ctx, act_tbl[59] == title);
}

static void test_debugact_increments_visible_position_and_wraps(
    test_context *ctx) {
    sprite_status actor;

    reset_act73_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 299;
    actor.yposi.w.h = 199;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 300, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);

    reset_act73_state();
    actor.xposi.w.h = 300;
    actor.yposi.w.h = 200;

    debugact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_noact_frames_out_actor(test_context *ctx) {
    sprite_status actor;

    reset_act73_state();
    memset(&actor, 0, sizeof(actor));

    noact(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_act73_table_captures_actor_dispatch_order(&ctx);
    test_debugact_increments_visible_position_and_wraps(&ctx);
    test_noact_frames_out_actor(&ctx);
TEST_MAIN_END

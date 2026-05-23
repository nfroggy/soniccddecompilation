#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int frameout_count;
static sprite_status *frameout_actor;
static int scoreup_count;
static Uint32 scoreup_values[8];
static int tensuu0_count;
static sprite_status *tensuu0_actor;
static Uint8 tensuu0_values[8];

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void frameout(sprite_status *pActwk);
void scoreup(Uint32 updata);
void tensuu0(sprite_status *tensuuwk, Uint8 uf_data);

#include "src/r3/bobinb.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void scoreup(Uint32 updata) {
    if (scoreup_count < 8) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
}

void tensuu0(sprite_status *tensuuwk, Uint8 uf_data) {
    if (tensuu0_count < 8) {
        tensuu0_values[tensuu0_count] = uf_data;
    }
    ++tensuu0_count;
    tensuu0_actor = tensuuwk;
}

static void reset_bobinbreak_state(sprite_status *actor) {
    memset(actor, 0, sizeof(*actor));
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    frameout_count = 0;
    frameout_actor = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    tensuu0_count = 0;
    tensuu0_actor = 0;
    memset(tensuu0_values, 0, sizeof(tensuu0_values));
}

static void test_bobinbreak_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 2, pat00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00[4]);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00[5]);
    TEST_ASSERT_EQ_INT(ctx, 252, pat00[6]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pat00);
    TEST_ASSERT_TRUE(ctx, pat_bobinbreak[0] == &bbb00);
    TEST_ASSERT_TRUE(ctx, pat_bobinbreak[1] == &bbb01);
    TEST_ASSERT_EQ_INT(ctx, 1, bbb00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, bbb00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, bbb00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, bbb00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 313, bbb00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 314, bbb01.spra[0].index);
}

static void test_bobinbreak_initializes_scores_and_draws(test_context *ctx) {
    sprite_status actor;

    reset_bobinbreak_state(&actor);
    actor.actflg = 128;
    actor.xposi.w.h = 640;
    actor.yposi.w.h = 320;

    bobinbreak(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 132, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprvsize);
    TEST_ASSERT_TRUE(ctx, actor.patbase == pat_bobinbreak);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, tensuu0_count);
    TEST_ASSERT_TRUE(ctx, tensuu0_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_bobinbreak_move_animates_without_scoring(test_context *ctx) {
    sprite_status actor;

    reset_bobinbreak_state(&actor);
    actor.r_no0 = 2;
    actor.patbase = pat_bobinbreak;

    bobinbreak(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tensuu0_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_bobinbreak_die_frameouts_then_draws(test_context *ctx) {
    sprite_status actor;

    reset_bobinbreak_state(&actor);
    actor.r_no0 = 4;
    actor.xposi.w.h = 512;
    actor.yposi.w.h = 128;

    bobinbreak(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 512, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, actor.yposi.w.h);
}

TEST_MAIN_BEGIN;
    test_bobinbreak_tables_capture_literal_data(&ctx);
    test_bobinbreak_initializes_scores_and_draws(&ctx);
    test_bobinbreak_move_animates_without_scoring(&ctx);
    test_bobinbreak_die_frameouts_then_draws(&ctx);
TEST_MAIN_END

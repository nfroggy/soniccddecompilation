#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 prio_flag;

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);

#include "src/r8/jettoge8.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

static void reset_logs(void) {
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
}

static void reset_jettoge8_state(sprite_status *actor) {
    prio_flag = 0;
    memset(actor, 0, sizeof(*actor));
    reset_logs();
}

static void assert_move_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
}

static void test_jettoge8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[3]);
    TEST_ASSERT_TRUE(ctx, pat_jettoge[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_jettoge[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -76, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 475, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1, pat01.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat01.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -76, pat01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 476, pat01.spra[0].index);
}

static void test_jettoge8_initial_call_initializes_moves_and_flips(
    test_context *ctx) {
    sprite_status actor;

    reset_jettoge8_state(&actor);
    actor.xposi.w.h = 100;
    actor.actflg = 128;
    actor.cddat = 3;

    jettoge(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 133, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.cddat);
    TEST_ASSERT_TRUE(ctx, actor.patbase == pat_jettoge);
    TEST_ASSERT_EQ_INT(ctx, 33856, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 76, actor.sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 177, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 20480, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
    assert_move_callbacks(ctx, &actor);
}

static void test_jettoge8_prio_flag_suppresses_collision_without_flipping(
    test_context *ctx) {
    sprite_status actor;

    reset_jettoge8_state(&actor);
    actor.xposi.w.h = 100;
    actor.actflg = 128;
    actor.cddat = 3;
    jettoge(&actor);
    reset_logs();
    prio_flag = 1;

    jettoge(&actor);

    TEST_ASSERT_EQ_INT(ctx, 133, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
    assert_move_callbacks(ctx, &actor);
}

static void test_jettoge8_timer_flips_after_exact_410_followup_moves(
    test_context *ctx) {
    sprite_status actor;
    Sint32 origin_x;
    Sint32 after_first_x;
    Uint8 first_flip_actflg;
    Uint8 first_flip_cddat;

    reset_jettoge8_state(&actor);
    actor.xposi.w.h = 100;
    jettoge(&actor);

    origin_x = 100 << 16;
    after_first_x = origin_x + 20480;
    first_flip_actflg = actor.actflg;
    first_flip_cddat = actor.cddat;

    for (int i = 0; i < 409; ++i) {
        jettoge(&actor);
    }

    TEST_ASSERT_EQ_INT(ctx, first_flip_actflg, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, first_flip_cddat, actor.cddat);
    TEST_ASSERT_EQ_INT(ctx, after_first_x - (409 * 20480), actor.xposi.l);

    jettoge(&actor);

    TEST_ASSERT_EQ_INT(ctx, first_flip_actflg ^ 1, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, first_flip_cddat ^ 1, actor.cddat);
    TEST_ASSERT_EQ_INT(ctx, after_first_x - (410 * 20480), actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_jettoge8_ignores_unhandled_routine_number(test_context *ctx) {
    sprite_status actor;

    reset_jettoge8_state(&actor);
    actor.r_no0 = 4;
    actor.xposi.w.h = 100;
    actor.actflg = 128;
    actor.cddat = 3;
    actor.colino = 177;

    jettoge(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 128, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.cddat);
    TEST_ASSERT_EQ_INT(ctx, 177, actor.colino);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

TEST_MAIN_BEGIN;
    test_jettoge8_patterns_capture_literal_data(&ctx);
    test_jettoge8_initial_call_initializes_moves_and_flips(&ctx);
    test_jettoge8_prio_flag_suppresses_collision_without_flipping(&ctx);
    test_jettoge8_timer_flips_after_exact_410_followup_moves(&ctx);
    test_jettoge8_ignores_unhandled_routine_number(&ctx);
TEST_MAIN_END

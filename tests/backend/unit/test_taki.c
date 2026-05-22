#include <string.h>

#include "support/test_runner.h"
#include "types.h"

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int block_wrt_count;
static Uint16 block_wrt_block[16];
static Uint16 block_wrt_x[16];
static Uint16 block_wrt_y[16];

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs);

#define R11A
#include "src/taki.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs) {
    if (block_wrt_count < 16) {
        block_wrt_block[block_wrt_count] = BlockNo;
        block_wrt_x[block_wrt_count] = xOffs;
        block_wrt_y[block_wrt_count] = yOffs;
    }
    ++block_wrt_count;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    block_wrt_count = 0;
    memset(block_wrt_block, 0, sizeof(block_wrt_block));
    memset(block_wrt_x, 0, sizeof(block_wrt_x));
    memset(block_wrt_y, 0, sizeof(block_wrt_y));
}

static void reset_taki_state(sprite_status *actor) {
    memset(actor, 0, sizeof(*actor));
    reset_logs();
}

static void init_taki_actor(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_taki_state(actor);
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    taki(actor);
    reset_logs();
}

static void test_taki_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, taki_move_tbl[0] == taki_init);
    TEST_ASSERT_TRUE(ctx, taki_move_tbl[1] == taki_move);
    TEST_ASSERT_TRUE(ctx, shibukichg == (Uint8 *)shibukichg0);
    TEST_ASSERT_EQ_INT(ctx, 4, shibukichg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, shibukichg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, shibukichg0[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, shibukichg0[3]);
    TEST_ASSERT_TRUE(ctx, shibukipat[0] == &shibuki0);
    TEST_ASSERT_TRUE(ctx, shibukipat[1] == &shibuki1);
    TEST_ASSERT_EQ_INT(ctx, 1, shibuki0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -96, shibuki0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, shibuki0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, shibuki0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 465, shibuki0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 466, shibuki1.spra[0].index);
}

static void test_taki_initializes_shape_and_aligned_start(test_context *ctx) {
    sprite_status actor;
    reset_taki_state(&actor);
    actor.r_no0 = 0;
    actor.xposi.w.h = 200;
    actor.yposi.w.h = 31;

    taki(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_TRUE(ctx, actor.patbase == shibukipat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 954, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 16, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == &shibukichg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_taki_move_advances_without_block_write_on_unaligned_row(
    test_context *ctx) {
    sprite_status actor;
    init_taki_actor(&actor, 200, 0);
    actor.yposi.w.h = 1;

    taki(&actor);

    TEST_ASSERT_EQ_INT(ctx, 5, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == &shibukichg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_taki_move_writes_twelve_blocks_on_aligned_row(
    test_context *ctx) {
    sprite_status actor;
    init_taki_actor(&actor, 200, 0);
    actor.yposi.w.h = 12;

    taki(&actor);

    TEST_ASSERT_EQ_INT(ctx, 16, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 12, block_wrt_count);
    for (int i = 0; i < 12; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 8194, block_wrt_block[i]);
        TEST_ASSERT_EQ_INT(ctx, 104 + i * 16, block_wrt_x[i]);
        TEST_ASSERT_EQ_INT(ctx, 16, block_wrt_y[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_taki_move_frameouts_at_initialized_limit(test_context *ctx) {
    sprite_status actor;
    init_taki_actor(&actor, 200, 0);
    actor.yposi.w.h = 380;

    taki(&actor);

    TEST_ASSERT_EQ_INT(ctx, 380, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_jisin_helpers_do_not_change_actor_or_call_out(
    test_context *ctx) {
    sprite_status actor;
    reset_taki_state(&actor);
    actor.actno = 9;
    actor.r_no0 = 4;
    actor.xposi.w.h = 123;
    actor.yposi.w.h = 456;

    jisin_set(&actor);
    jisin(&actor);

    TEST_ASSERT_EQ_INT(ctx, 9, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 123, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
}

TEST_MAIN_BEGIN;
    test_taki_tables_capture_literal_data(&ctx);
    test_taki_initializes_shape_and_aligned_start(&ctx);
    test_taki_move_advances_without_block_write_on_unaligned_row(&ctx);
    test_taki_move_writes_twelve_blocks_on_aligned_row(&ctx);
    test_taki_move_frameouts_at_initialized_limit(&ctx);
    test_jisin_helpers_do_not_change_actor_or_call_out(&ctx);
TEST_MAIN_END

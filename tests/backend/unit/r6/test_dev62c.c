#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 cgchgcnt[6];
Uint8 cgchgtim[6];

typedef struct tile_call {
    Sint32 tile_start;
    Sint32 bmp_no;
} tile_call;

static tile_call tile_calls[32];
static int tile_call_count;

static void record_change_tile_bmp(Sint32 tile_start, Sint32 bmp_no) {
    tile_calls[tile_call_count].tile_start = tile_start;
    tile_calls[tile_call_count].bmp_no = bmp_no;
    ++tile_call_count;
}

void (*ChangeTileBmp)(Sint32, Sint32) = record_change_tile_bmp;

#include "src/r6/dev62c.c"

static void reset_dev62c_state(void) {
    memset(cgchgcnt, 0, sizeof(cgchgcnt));
    memset(cgchgtim, 0, sizeof(cgchgtim));
    memset(tile_calls, 0, sizeof(tile_calls));
    tile_call_count = 0;
    ChangeTileBmp = record_change_tile_bmp;
}

static void assert_map_init_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.CGdata);
    TEST_ASSERT_EQ_INT(ctx, 3, mapinittbl.iniCGno1);
    TEST_ASSERT_EQ_INT(ctx, 2, mapinittbl.stCGno1);
    TEST_ASSERT_EQ_INT(ctx, 4, mapinittbl.colorno1);
    TEST_ASSERT_EQ_INT(ctx, 4, mapinittbl.colorno2);
    TEST_ASSERT_EQ_INT(ctx, 129, mapinittbl.soundNo);
}

static void test_tables_capture_tile_animation_contract(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_thund.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 347, tbl_thund.TileStart);
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_null.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 347, tbl_null.TileStart);
    TEST_ASSERT_EQ_INT(ctx, 2, tbl_sp.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 343, tbl_sp.TileStart);
    TEST_ASSERT_EQ_INT(ctx, 0, nullcg[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, nullcg[7]);
    TEST_ASSERT_EQ_INT(ctx, 1, sp1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 8, sp2cg[3]);
    TEST_ASSERT_EQ_INT(ctx, 9, t1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, t3cg[7]);
}

static void test_cg_chg1_waits_advances_and_wraps(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[8] = {-1};
    Sint32 tile_start = -1;
    int i;

    reset_dev62c_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;
    TEST_ASSERT_EQ_INT(ctx, 0,
                       cg_chg1(&tbl_thund, 7, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, -1, bmp_no[0]);

    reset_dev62c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    tim = cgchgtim;
    cnt = cgchgcnt;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_thund, 7, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 347, tile_start);
    for (i = 0; i < 8; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 17 + i, bmp_no[i]);
    }

    reset_dev62c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_sp, 3, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 343, tile_start);
    for (i = 0; i < 4; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 1 + i, bmp_no[i]);
    }
}

static void test_cg_change_wait_path_makes_no_tile_calls(test_context *ctx) {
    reset_dev62c_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    cgchgtim[1] = 3;
    cgchgcnt[1] = 0;
    cgchgtim[2] = 4;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgtim[2]);
}

static void test_cg_change_emits_sp_and_thunder_sequence(test_context *ctx) {
    int i;

    reset_dev62c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 0;
    cgchgtim[2] = 2;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 12, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[2]);
    for (i = 0; i < 4; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 343 + i, tile_calls[i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, 5 + i, tile_calls[i].bmp_no);
    }
    for (i = 0; i < 8; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 347 + i, tile_calls[4 + i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, 17 + i, tile_calls[4 + i].bmp_no);
    }
}

static void test_cg_change_timer_toggle_and_null_sequence(test_context *ctx) {
    int i;

    reset_dev62c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 0;
    cgchgtim[2] = 1;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 12, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgtim[2]);
    for (i = 0; i < 4; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 343 + i, tile_calls[i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, 5 + i, tile_calls[i].bmp_no);
    }
    for (i = 0; i < 8; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 347 + i, tile_calls[4 + i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[4 + i].bmp_no);
    }

    reset_dev62c_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 2;
    cgchgcnt[1] = 0;
    cgchgtim[2] = 255;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 30, cgchgtim[2]);
}

TEST_MAIN_BEGIN;
assert_map_init_data(&ctx);
test_tables_capture_tile_animation_contract(&ctx);
test_cg_chg1_waits_advances_and_wraps(&ctx);
test_cg_change_wait_path_makes_no_tile_calls(&ctx);
test_cg_change_emits_sp_and_thunder_sequence(&ctx);
test_cg_change_timer_toggle_and_null_sequence(&ctx);
TEST_MAIN_END

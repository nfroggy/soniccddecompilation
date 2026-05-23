#include "support/test_runner.h"
#include "src/types.h"

#include <string.h>

Uint8 cgchgcnt[6];
Uint8 cgchgtim[6];

typedef struct tile_call {
    Sint32 tile_start;
    Sint32 bmp_no;
} tile_call;

static tile_call tile_calls[16];
static int tile_call_count;

static void record_change_tile_bmp(Sint32 tile_start, Sint32 bmp_no) {
    tile_calls[tile_call_count].tile_start = tile_start;
    tile_calls[tile_call_count].bmp_no = bmp_no;
    ++tile_call_count;
}

void (*ChangeTileBmp)(Sint32, Sint32) = record_change_tile_bmp;

#include "src/r8/dev82b.c"

static void reset_dev82b_state(void) {
    memset(cgchgcnt, 0, sizeof(cgchgcnt));
    memset(cgchgtim, 0, sizeof(cgchgtim));
    memset(tile_calls, 0, sizeof(tile_calls));
    tile_call_count = 0;
    ChangeTileBmp = record_change_tile_bmp;
}

static void test_dev82b_map_init_data_captures_literal_values(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.CGdata);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.Blkdata);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.Mapdata);
    TEST_ASSERT_EQ_INT(ctx, 3, mapinittbl.iniCGno1);
    TEST_ASSERT_EQ_INT(ctx, 2, mapinittbl.stCGno1);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.iniCGno2);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.stCGno2);
    TEST_ASSERT_EQ_INT(ctx, 5, mapinittbl.colorno1);
    TEST_ASSERT_EQ_INT(ctx, 5, mapinittbl.colorno2);
    TEST_ASSERT_EQ_INT(ctx, 129, mapinittbl.soundNo);
}

static void test_dev82b_tables_capture_literal_values(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, pan1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, pan1cg[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, pan2cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 11, pan3cg[3]);
    TEST_ASSERT_EQ_INT(ctx, 16, pan5cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, pan6cg[3]);
    TEST_ASSERT_EQ_INT(ctx, 6, tbl_pan.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl_pan.dmy);
    TEST_ASSERT_EQ_INT(ctx, 4, tbl_pan.dattbl[0].tim);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl_pan.dattbl[0].no);
    TEST_ASSERT_EQ_INT(ctx, 4, tbl_pan.dattbl[5].tim);
    TEST_ASSERT_EQ_INT(ctx, 5, tbl_pan.dattbl[5].no);
    TEST_ASSERT_TRUE(ctx, tbl_pan.TileNoTbl[0] == pan1cg);
    TEST_ASSERT_TRUE(ctx, tbl_pan.TileNoTbl[5] == pan6cg);
    TEST_ASSERT_EQ_INT(ctx, 444, tbl_pan.TileStart);
}

static void test_dev82b_cg_chg1_waits_while_timer_remains(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[4] = {-1};
    Sint32 tile_start = -1;

    reset_dev82b_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 0,
                       cg_chg1(&tbl_pan, 3, &tim, &cnt, bmp_no, &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, -1, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, tile_start);
}

static void test_dev82b_cg_chg1_advances_to_next_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[4];
    Sint32 tile_start = -1;

    reset_dev82b_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_pan, 3, &tim, &cnt, bmp_no, &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 444, tile_start);
    TEST_ASSERT_EQ_INT(ctx, 4, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, bmp_no[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, bmp_no[2]);
    TEST_ASSERT_EQ_INT(ctx, 7, bmp_no[3]);
}

static void test_dev82b_cg_chg1_wraps_to_first_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[4];
    Sint32 tile_start = -1;

    reset_dev82b_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 5;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_pan, 3, &tim, &cnt, bmp_no, &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 444, tile_start);
    TEST_ASSERT_EQ_INT(ctx, 0, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, bmp_no[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, bmp_no[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, bmp_no[3]);
}

static void test_dev82b_cg_change_wait_path_makes_no_tile_calls(
    test_context *ctx) {
    reset_dev82b_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
}

static void test_dev82b_cg_change_emits_exact_tile_sequence(test_context *ctx) {
    reset_dev82b_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 2;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 4, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 444, tile_calls[0].tile_start);
    TEST_ASSERT_EQ_INT(ctx, 12, tile_calls[0].bmp_no);
    TEST_ASSERT_EQ_INT(ctx, 445, tile_calls[1].tile_start);
    TEST_ASSERT_EQ_INT(ctx, 13, tile_calls[1].bmp_no);
    TEST_ASSERT_EQ_INT(ctx, 446, tile_calls[2].tile_start);
    TEST_ASSERT_EQ_INT(ctx, 14, tile_calls[2].bmp_no);
    TEST_ASSERT_EQ_INT(ctx, 447, tile_calls[3].tile_start);
    TEST_ASSERT_EQ_INT(ctx, 15, tile_calls[3].bmp_no);
}

TEST_MAIN_BEGIN;
    test_dev82b_map_init_data_captures_literal_values(&ctx);
    test_dev82b_tables_capture_literal_values(&ctx);
    test_dev82b_cg_chg1_waits_while_timer_remains(&ctx);
    test_dev82b_cg_chg1_advances_to_next_frame(&ctx);
    test_dev82b_cg_chg1_wraps_to_first_frame(&ctx);
    test_dev82b_cg_change_wait_path_makes_no_tile_calls(&ctx);
    test_dev82b_cg_change_emits_exact_tile_sequence(&ctx);
TEST_MAIN_END

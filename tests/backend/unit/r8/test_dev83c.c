#include "support/test_runner.h"
#include "src/types.h"

#include <string.h>

Uint8 cgchgcnt[6];
Uint8 cgchgtim[6];
Uint8 shut_flag;

typedef struct tile_call {
    Sint32 tile_start;
    Sint32 bmp_no;
} tile_call;

static tile_call tile_calls[40];
static int tile_call_count;

static void record_change_tile_bmp(Sint32 tile_start, Sint32 bmp_no) {
    tile_calls[tile_call_count].tile_start = tile_start;
    tile_calls[tile_call_count].bmp_no = bmp_no;
    ++tile_call_count;
}

void (*ChangeTileBmp)(Sint32, Sint32) = record_change_tile_bmp;

#include "src/r8/dev83c.c"

static void reset_dev83c_state(void) {
    memset(cgchgcnt, 0, sizeof(cgchgcnt));
    memset(cgchgtim, 0, sizeof(cgchgtim));
    memset(tile_calls, 0, sizeof(tile_calls));
    tile_call_count = 0;
    shut_flag = 0;
    ChangeTileBmp = record_change_tile_bmp;
}

static void test_dev83c_map_init_data_captures_literal_values(
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

static void test_dev83c_tables_capture_representative_literal_values(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, pis1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 15, pis2cg[7]);
    TEST_ASSERT_EQ_INT(ctx, 16, pis3cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 24, com1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 55, com4cg[7]);
    TEST_ASSERT_EQ_INT(ctx, 56, mot1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 119, mot4cg[15]);
    TEST_ASSERT_EQ_INT(ctx, 120, pan1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 143, pan6cg[3]);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_pis.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 6, tbl_pis.dattbl[0].tim);
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_pis.dattbl[3].no);
    TEST_ASSERT_TRUE(ctx, tbl_pis.TileNoTbl[3] == pis2cg);
    TEST_ASSERT_EQ_INT(ctx, 287, tbl_pis.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_com.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_com.dattbl[0].tim);
    TEST_ASSERT_TRUE(ctx, tbl_com.TileNoTbl[3] == com4cg);
    TEST_ASSERT_EQ_INT(ctx, 295, tbl_com.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_mot.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 4, tbl_mot.dattbl[0].tim);
    TEST_ASSERT_TRUE(ctx, tbl_mot.TileNoTbl[3] == mot4cg);
    TEST_ASSERT_EQ_INT(ctx, 303, tbl_mot.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 6, tbl_pan.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 4, tbl_pan.dattbl[5].tim);
    TEST_ASSERT_TRUE(ctx, tbl_pan.TileNoTbl[5] == pan6cg);
    TEST_ASSERT_EQ_INT(ctx, 319, tbl_pan.TileStart);
}

static void test_dev83c_cg_chg1_waits_while_timer_remains(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[16] = {-1};
    Sint32 tile_start = -1;

    reset_dev83c_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 0,
                       cg_chg1(&tbl_mot, 15, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, -1, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, tile_start);
}

static void test_dev83c_cg_chg1_advances_to_next_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[8];
    Sint32 tile_start = -1;

    reset_dev83c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_pis, 7, &tim, &cnt, bmp_no, &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 6, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 287, tile_start);
    for (int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 8 + i, bmp_no[i]);
    }
}

static void test_dev83c_cg_chg1_wraps_to_first_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[4];
    Sint32 tile_start = -1;

    reset_dev83c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 5;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_pan, 3, &tim, &cnt, bmp_no, &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 319, tile_start);
    TEST_ASSERT_EQ_INT(ctx, 120, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, 121, bmp_no[1]);
    TEST_ASSERT_EQ_INT(ctx, 122, bmp_no[2]);
    TEST_ASSERT_EQ_INT(ctx, 123, bmp_no[3]);
}

static void test_dev83c_cg_change_skips_pan_when_shut_flag_set_and_pan_count_zero(
    test_context *ctx) {
    reset_dev83c_state();
    shut_flag = 1;
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 0;
    cgchgtim[2] = 0;
    cgchgcnt[2] = 0;
    cgchgtim[3] = 0;
    cgchgcnt[3] = 0;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 32, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgtim[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[3]);
}

static void test_dev83c_cg_change_wait_path_makes_no_tile_calls(
    test_context *ctx) {
    reset_dev83c_state();
    for (int i = 0; i < 4; ++i) {
        cgchgtim[i] = 2;
        cgchgcnt[i] = 1;
    }

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[i]);
        TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[i]);
    }
}

static void assert_tile_range(test_context *ctx, int base_call, Sint32 top,
                              Sint32 first_bmp, int count) {
    for (int i = 0; i < count; ++i) {
        TEST_ASSERT_EQ_INT(ctx, top + i, tile_calls[base_call + i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, first_bmp + i, tile_calls[base_call + i].bmp_no);
    }
}

static void test_dev83c_cg_change_emits_exact_tile_sequence(test_context *ctx) {
    reset_dev83c_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 1;
    cgchgtim[2] = 0;
    cgchgcnt[2] = 2;
    cgchgtim[3] = 0;
    cgchgcnt[3] = 5;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 36, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgcnt[2]);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[3]);

    assert_tile_range(ctx, 0, 287, 8, 8);
    assert_tile_range(ctx, 8, 295, 40, 8);
    assert_tile_range(ctx, 16, 303, 104, 16);
    assert_tile_range(ctx, 32, 319, 120, 4);
}

TEST_MAIN_BEGIN;
    test_dev83c_map_init_data_captures_literal_values(&ctx);
    test_dev83c_tables_capture_representative_literal_values(&ctx);
    test_dev83c_cg_chg1_waits_while_timer_remains(&ctx);
    test_dev83c_cg_chg1_advances_to_next_frame(&ctx);
    test_dev83c_cg_chg1_wraps_to_first_frame(&ctx);
    test_dev83c_cg_change_skips_pan_when_shut_flag_set_and_pan_count_zero(&ctx);
    test_dev83c_cg_change_wait_path_makes_no_tile_calls(&ctx);
    test_dev83c_cg_change_emits_exact_tile_sequence(&ctx);
TEST_MAIN_END

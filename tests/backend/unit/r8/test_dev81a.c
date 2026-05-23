#include "support/test_runner.h"
#include "src/types.h"

#include <string.h>

Uint8 cgchgcnt[6];
Uint8 cgchgtim[6];

typedef struct tile_call {
    Sint32 tile_start;
    Sint32 bmp_no;
} tile_call;

static tile_call tile_calls[56];
static int tile_call_count;

static void record_change_tile_bmp(Sint32 tile_start, Sint32 bmp_no) {
    tile_calls[tile_call_count].tile_start = tile_start;
    tile_calls[tile_call_count].bmp_no = bmp_no;
    ++tile_call_count;
}

void (*ChangeTileBmp)(Sint32, Sint32) = record_change_tile_bmp;

#include "src/r8/dev81a.c"

static void reset_dev81a_state(void) {
    memset(cgchgcnt, 0, sizeof(cgchgcnt));
    memset(cgchgtim, 0, sizeof(cgchgtim));
    memset(tile_calls, 0, sizeof(tile_calls));
    tile_call_count = 0;
    ChangeTileBmp = record_change_tile_bmp;
}

static void test_dev81a_map_init_data_captures_literal_values(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.CGdata);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.Blkdata);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.Mapdata);
    TEST_ASSERT_EQ_INT(ctx, 3, mapinittbl.iniCGno1);
    TEST_ASSERT_EQ_INT(ctx, 2, mapinittbl.stCGno1);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.iniCGno2);
    TEST_ASSERT_EQ_INT(ctx, 0, mapinittbl.stCGno2);
    TEST_ASSERT_EQ_INT(ctx, 4, mapinittbl.colorno1);
    TEST_ASSERT_EQ_INT(ctx, 4, mapinittbl.colorno2);
    TEST_ASSERT_EQ_INT(ctx, 129, mapinittbl.soundNo);
}

static void test_dev81a_tables_capture_representative_literal_values(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, pis1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 15, pis2cg[7]);
    TEST_ASSERT_EQ_INT(ctx, 16, pis3cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 24, com1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 55, com4cg[7]);
    TEST_ASSERT_EQ_INT(ctx, 56, mot1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 119, mot4cg[15]);
    TEST_ASSERT_EQ_INT(ctx, 120, spo1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 147, spo7cg[3]);
    TEST_ASSERT_EQ_INT(ctx, 148, bal1cg[0]);
    TEST_ASSERT_EQ_INT(ctx, 211, bal4cg[15]);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_pis.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 6, tbl_pis.dattbl[0].tim);
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_pis.dattbl[3].no);
    TEST_ASSERT_TRUE(ctx, tbl_pis.TileNoTbl[3] == pis2cg);
    TEST_ASSERT_EQ_INT(ctx, 333, tbl_pis.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_com.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 3, tbl_com.dattbl[0].tim);
    TEST_ASSERT_TRUE(ctx, tbl_com.TileNoTbl[3] == com4cg);
    TEST_ASSERT_EQ_INT(ctx, 341, tbl_com.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 4, tbl_mot.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 4, tbl_mot.dattbl[0].tim);
    TEST_ASSERT_TRUE(ctx, tbl_mot.TileNoTbl[3] == mot4cg);
    TEST_ASSERT_EQ_INT(ctx, 349, tbl_mot.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 7, tbl_spo.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 7, tbl_spo.dattbl[6].tim);
    TEST_ASSERT_TRUE(ctx, tbl_spo.TileNoTbl[6] == spo7cg);
    TEST_ASSERT_EQ_INT(ctx, 365, tbl_spo.TileStart);

    TEST_ASSERT_EQ_INT(ctx, 16, tbl_bal.chgcnt);
    TEST_ASSERT_EQ_INT(ctx, 50, tbl_bal.dattbl[12].tim);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl_bal.dattbl[12].no);
    TEST_ASSERT_TRUE(ctx, tbl_bal.TileNoTbl[3] == bal4cg);
    TEST_ASSERT_EQ_INT(ctx, 529, tbl_bal.TileStart);
}

static void test_dev81a_cg_chg1_waits_while_timer_remains(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[16] = {-1};
    Sint32 tile_start = -1;

    reset_dev81a_state();
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

static void test_dev81a_cg_chg1_advances_to_next_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[8];
    Sint32 tile_start = -1;

    reset_dev81a_state();
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
    TEST_ASSERT_EQ_INT(ctx, 333, tile_start);
    for (int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 8 + i, bmp_no[i]);
    }
}

static void test_dev81a_cg_chg1_wraps_to_first_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[16];
    Sint32 tile_start = -1;

    reset_dev81a_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 15;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_bal, 15, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 529, tile_start);
    for (int i = 0; i < 16; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 148 + i, bmp_no[i]);
    }
}

static void test_dev81a_cg_change_wait_path_makes_no_tile_calls(
    test_context *ctx) {
    reset_dev81a_state();
    for (int i = 0; i < 5; ++i) {
        cgchgtim[i] = 2;
        cgchgcnt[i] = 1;
    }

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    for (int i = 0; i < 5; ++i) {
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

static void test_dev81a_cg_change_emits_exact_tile_sequence(test_context *ctx) {
    reset_dev81a_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 1;
    cgchgtim[2] = 0;
    cgchgcnt[2] = 2;
    cgchgtim[3] = 0;
    cgchgcnt[3] = 11;
    cgchgtim[4] = 0;
    cgchgcnt[4] = 5;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 52, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, cgchgtim[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, cgchgcnt[2]);
    TEST_ASSERT_EQ_INT(ctx, 50, cgchgtim[3]);
    TEST_ASSERT_EQ_INT(ctx, 12, cgchgcnt[3]);
    TEST_ASSERT_EQ_INT(ctx, 7, cgchgtim[4]);
    TEST_ASSERT_EQ_INT(ctx, 6, cgchgcnt[4]);

    assert_tile_range(ctx, 0, 333, 8, 8);
    assert_tile_range(ctx, 8, 341, 40, 8);
    assert_tile_range(ctx, 16, 349, 104, 16);
    assert_tile_range(ctx, 32, 529, 148, 16);
    assert_tile_range(ctx, 48, 365, 144, 4);
}

TEST_MAIN_BEGIN;
    test_dev81a_map_init_data_captures_literal_values(&ctx);
    test_dev81a_tables_capture_representative_literal_values(&ctx);
    test_dev81a_cg_chg1_waits_while_timer_remains(&ctx);
    test_dev81a_cg_chg1_advances_to_next_frame(&ctx);
    test_dev81a_cg_chg1_wraps_to_first_frame(&ctx);
    test_dev81a_cg_change_wait_path_makes_no_tile_calls(&ctx);
    test_dev81a_cg_change_emits_exact_tile_sequence(&ctx);
TEST_MAIN_END

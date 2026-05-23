#include "support/test_runner.h"
#include "src/types.h"

#include <string.h>

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

#include "src/r3/dev32a.c"

static void reset_dev32a_state(void) {
    memset(cgchgcnt, 0, sizeof(cgchgcnt));
    memset(cgchgtim, 0, sizeof(cgchgtim));
    memset(tile_calls, 0, sizeof(tile_calls));
    tile_call_count = 0;
    ChangeTileBmp = record_change_tile_bmp;
}

static void assert_map_init_data(test_context *ctx) {
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

static void test_cg_chg1_waits_while_timer_remains(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[10] = {-1};
    Sint32 tile_start = -1;

    reset_dev32a_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 0,
                       cg_chg1(&tbl_drum00, 9, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, -1, bmp_no[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, tile_start);
}

static void test_cg_chg1_advances_to_next_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[10];
    Sint32 tile_start = -1;
    int i;

    reset_dev32a_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_drum00, 9, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 10, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 436, tile_start);
    TEST_ASSERT_TRUE(ctx, tim == &cgchgtim[1]);
    TEST_ASSERT_TRUE(ctx, cnt == &cgchgcnt[1]);
    for (i = 0; i < 10; ++i) {
        TEST_ASSERT_EQ_INT(ctx, i, bmp_no[i]);
    }
}

static void test_cg_chg1_wraps_to_first_frame(test_context *ctx) {
    Uint8 *tim;
    Uint8 *cnt;
    Sint32 bmp_no[6];
    Sint32 tile_start = -1;
    int i;

    reset_dev32a_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 1;
    tim = cgchgtim;
    cnt = cgchgcnt;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       cg_chg1(&tbl_drum01, 5, &tim, &cnt, bmp_no,
                               &tile_start));
    TEST_ASSERT_EQ_INT(ctx, 10, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 462, tile_start);
    for (i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 26 + i, bmp_no[i]);
    }
}

static void test_cg_change_wait_path_makes_no_tile_calls(test_context *ctx) {
    reset_dev32a_state();
    cgchgtim[0] = 2;
    cgchgcnt[0] = 1;
    cgchgtim[1] = 3;
    cgchgcnt[1] = 0;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, cgchgcnt[1]);
}

static void test_cg_change_emits_exact_tile_sequence(test_context *ctx) {
    int i;

    reset_dev32a_state();
    cgchgtim[0] = 0;
    cgchgcnt[0] = 0;
    cgchgtim[1] = 0;
    cgchgcnt[1] = 0;

    cg_change();

    TEST_ASSERT_EQ_INT(ctx, 16, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, cgchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, cgchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, cgchgcnt[1]);
    for (i = 0; i < 10; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 436 + i, tile_calls[i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, i, tile_calls[i].bmp_no);
    }
    for (i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 462 + i, tile_calls[10 + i].tile_start);
        TEST_ASSERT_EQ_INT(ctx, 10 + i, tile_calls[10 + i].bmp_no);
    }
}

TEST_MAIN_BEGIN;
    assert_map_init_data(&ctx);
    test_cg_chg1_waits_while_timer_remains(&ctx);
    test_cg_chg1_advances_to_next_frame(&ctx);
    test_cg_chg1_wraps_to_first_frame(&ctx);
    test_cg_change_wait_path_makes_no_tile_calls(&ctx);
    test_cg_change_emits_exact_tile_sequence(&ctx);
TEST_MAIN_END

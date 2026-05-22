#include "support/test_runner.h"
#include "types.h"

#define R11A
#include "src/bmp.c"

static void test_bmp_globals_are_zero_initialized_and_writable(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, bCarry);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[0].xs);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[0].ys);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[0].ofs);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[699].xs);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[699].ys);
    TEST_ASSERT_EQ_INT(ctx, 0, SprBmp[699].ofs);

    bCarry = 1;
    SprBmp[0].xs = 12;
    SprBmp[0].ys = 34;
    SprBmp[0].ofs = 56;
    SprBmp[699].xs = 78;
    SprBmp[699].ys = 90;
    SprBmp[699].ofs = 123;

    TEST_ASSERT_EQ_INT(ctx, 1, bCarry);
    TEST_ASSERT_EQ_INT(ctx, 12, SprBmp[0].xs);
    TEST_ASSERT_EQ_INT(ctx, 34, SprBmp[0].ys);
    TEST_ASSERT_EQ_INT(ctx, 56, SprBmp[0].ofs);
    TEST_ASSERT_EQ_INT(ctx, 78, SprBmp[699].xs);
    TEST_ASSERT_EQ_INT(ctx, 90, SprBmp[699].ys);
    TEST_ASSERT_EQ_INT(ctx, 123, SprBmp[699].ofs);
}

TEST_MAIN_BEGIN;
    test_bmp_globals_are_zero_initialized_and_writable(&ctx);
TEST_MAIN_END;

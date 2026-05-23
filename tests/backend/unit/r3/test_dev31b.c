#include "support/test_runner.h"
#include "src/types.h"

#include "src/r3/dev31b.c"

static void test_dev31b_map_init_data_matches_existing_record(
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

TEST_MAIN_BEGIN;
    test_dev31b_map_init_data_matches_existing_record(&ctx);
TEST_MAIN_END

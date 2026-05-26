#include "support/test_runner.h"
#include "types.h"

#include "src/title/besttime/bestgrid.c"

static void test_grid_create_returns_zero_for_any_index(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(0));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(1));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(65535));
}

static void test_grid_delete_functions_are_noops(test_context *ctx) {
    int sentinel = 12345;

    OEGridDelete(0);
    OEGridDelete(65535);
    OEAllGridDelete();

    TEST_ASSERT_EQ_INT(ctx, 12345, sentinel);
}

TEST_MAIN_BEGIN;
test_grid_create_returns_zero_for_any_index(&ctx);
test_grid_delete_functions_are_noops(&ctx);
TEST_MAIN_END

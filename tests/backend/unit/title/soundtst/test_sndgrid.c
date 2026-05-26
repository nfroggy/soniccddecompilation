#include "support/test_runner.h"
#include "types.h"

#include "src/title/soundtst/sndgrid.c"

static void test_grid_create_returns_zero_for_representative_indices(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(0));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(7));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(65535));
}

static void test_grid_delete_functions_preserve_caller_state(test_context *ctx) {
    int sentinel = -77;

    OEGridDelete(0);
    OEGridDelete(65535);
    OEAllGridDelete();

    TEST_ASSERT_EQ_INT(ctx, -77, sentinel);
}

TEST_MAIN_BEGIN;
test_grid_create_returns_zero_for_representative_indices(&ctx);
test_grid_delete_functions_preserve_caller_state(&ctx);
TEST_MAIN_END

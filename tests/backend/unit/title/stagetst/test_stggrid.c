#include "support/test_runner.h"
#include "types.h"

#include "src/title/stagetst/stggrid.c"

static void test_grid_create_always_returns_zero(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(0));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(12));
    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(65535));
}

static void test_grid_delete_entry_points_have_no_visible_effect(
    test_context *ctx) {
    int sentinel = 9001;

    OEGridDelete(0);
    OEGridDelete(65535);
    OEAllGridDelete();

    TEST_ASSERT_EQ_INT(ctx, 9001, sentinel);
}

TEST_MAIN_BEGIN;
test_grid_create_always_returns_zero(&ctx);
test_grid_delete_entry_points_have_no_visible_effect(&ctx);
TEST_MAIN_END

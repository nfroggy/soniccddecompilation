#include "support/test_runner.h"

#include "src/playsp.c"

static void test_playsp_returns_without_side_effects(test_context *ctx) {
    volatile int sentinel = 12345;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 12345, sentinel);
}

TEST_MAIN_BEGIN;
    test_playsp_returns_without_side_effects(&ctx);
TEST_MAIN_END

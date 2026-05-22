#include "support/test_runner.h"

#include "src/dllmain.c"

TEST_MAIN_BEGIN;

TEST_ASSERT_EQ_INT(&ctx, 1, LibMain(0, 0, 0, 0));
TEST_ASSERT_EQ_INT(&ctx, 1, LibMain((void *)1, 2, 3, "cmd"));
TEST_ASSERT_EQ_INT(&ctx, 1, WEP(0));
TEST_ASSERT_EQ_INT(&ctx, 1, WEP(1));

TEST_MAIN_END


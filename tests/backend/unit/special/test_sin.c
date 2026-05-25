#include "support/test_runner.h"

#include "src/special/sin.c"

static void test_sp_sin_matches_quadrant_table_values(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, sp_sin(0));
    TEST_ASSERT_EQ_INT(ctx, 3, sp_sin(1));
    TEST_ASSERT_EQ_INT(ctx, 181, sp_sin(64));
    TEST_ASSERT_EQ_INT(ctx, 256, sp_sin(127));
    TEST_ASSERT_EQ_INT(ctx, 256, sp_sin(128));
    TEST_ASSERT_EQ_INT(ctx, 179, sp_sin(192));
    TEST_ASSERT_EQ_INT(ctx, 0, sp_sin(255));
}

static void test_sp_sin_negates_lower_half_and_masks_angle(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, sp_sin(256));
    TEST_ASSERT_EQ_INT(ctx, -181, sp_sin(320));
    TEST_ASSERT_EQ_INT(ctx, -256, sp_sin(383));
    TEST_ASSERT_EQ_INT(ctx, -256, sp_sin(384));
    TEST_ASSERT_EQ_INT(ctx, -179, sp_sin(448));
    TEST_ASSERT_EQ_INT(ctx, 0, sp_sin(511));
    TEST_ASSERT_EQ_INT(ctx, 0, sp_sin(512));
    TEST_ASSERT_EQ_INT(ctx, 256, sp_sin(640));
}

static void test_sp_cos_is_sine_with_quarter_turn_offset(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 256, sp_cos(0));
    TEST_ASSERT_EQ_INT(ctx, 179, sp_cos(64));
    TEST_ASSERT_EQ_INT(ctx, 0, sp_cos(128));
    TEST_ASSERT_EQ_INT(ctx, -181, sp_cos(192));
    TEST_ASSERT_EQ_INT(ctx, -256, sp_cos(256));
    TEST_ASSERT_EQ_INT(ctx, 0, sp_cos(384));
    TEST_ASSERT_EQ_INT(ctx, 256, sp_cos(511));
}

TEST_MAIN_BEGIN;
test_sp_sin_matches_quadrant_table_values(&ctx);
test_sp_sin_negates_lower_half_and_masks_angle(&ctx);
test_sp_cos_is_sine_with_quarter_turn_offset(&ctx);
TEST_MAIN_END

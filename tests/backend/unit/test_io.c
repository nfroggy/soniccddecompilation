#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Sint32 ranum;
ushort_union swdata1;
ushort_union swdata2;
int_union vscroll;
int_union hscroll;

#include "src/io.c"

static void reset_io_state(void) {
    ranum = 0;
    memset(&swdata1, 0, sizeof(swdata1));
    memset(&swdata2, 0, sizeof(swdata2));
    memset(&vscroll, 0, sizeof(vscroll));
    memset(&hscroll, 0, sizeof(hscroll));
}

static void test_swdata_set_copies_controller_words(test_context *ctx) {
    ushort_union sw1;
    ushort_union sw2;

    reset_io_state();
    sw1.w = 0xA55A;
    sw2.w = 0x5AA5;

    SWdataSet(sw1, sw2);

    TEST_ASSERT_EQ_INT(ctx, 0xA55A, swdata1.w);
    TEST_ASSERT_EQ_INT(ctx, 0x5AA5, swdata2.w);
}

static void test_scrinit_clears_scroll_accumulators(test_context *ctx) {
    reset_io_state();
    vscroll.l = 0x12345678;
    hscroll.l = (Sint32)0x87654321;

    scrinit();

    TEST_ASSERT_EQ_INT(ctx, 0, vscroll.l);
    TEST_ASSERT_EQ_INT(ctx, 0, hscroll.l);
}

static void test_random_uses_default_seed_when_ranum_is_zero(
    test_context *ctx) {
    Sint32 result;

    reset_io_state();
    ranum = 0;

    result = random();

    TEST_ASSERT_EQ_INT(ctx, 711833153, result);
    TEST_ASSERT_EQ_INT(ctx, -1237207958, ranum);
}

static void test_random_positive_seed_with_positive_intermediate(
    test_context *ctx) {
    Sint32 result;

    reset_io_state();
    ranum = 1;

    result = random();

    TEST_ASSERT_EQ_INT(ctx, 42, result);
    TEST_ASSERT_EQ_INT(ctx, 2752553, ranum);
}

static void test_random_negative_seed_uses_signed_paths(test_context *ctx) {
    Sint32 result;

    reset_io_state();
    ranum = -1;

    result = random();

    TEST_ASSERT_EQ_INT(ctx, -43, result);
    TEST_ASSERT_EQ_INT(ctx, -2752553, ranum);
}

static void test_random_updates_seed_across_sequence(test_context *ctx) {
    reset_io_state();
    ranum = 1;

    TEST_ASSERT_EQ_INT(ctx, 42, random());
    TEST_ASSERT_EQ_INT(ctx, 2752553, ranum);
    TEST_ASSERT_EQ_INT(ctx, 2755956, random());
    TEST_ASSERT_EQ_INT(ctx, 225707665, ranum);
    TEST_ASSERT_EQ_INT(ctx, 225721183, random());
    TEST_ASSERT_EQ_INT(ctx, 996085049, ranum);
    TEST_ASSERT_EQ_INT(ctx, 996126099, random());
    TEST_ASSERT_EQ_INT(ctx, -1382867423, ranum);
    TEST_ASSERT_EQ_INT(ctx, -1382826503, random());
    TEST_ASSERT_EQ_INT(ctx, -1107700919, ranum);
}

TEST_MAIN_BEGIN;
    test_swdata_set_copies_controller_words(&ctx);
    test_scrinit_clears_scroll_accumulators(&ctx);
    test_random_uses_default_seed_when_ranum_is_zero(&ctx);
    test_random_positive_seed_with_positive_intermediate(&ctx);
    test_random_negative_seed_uses_signed_paths(&ctx);
    test_random_updates_seed_across_sequence(&ctx);
TEST_MAIN_END;

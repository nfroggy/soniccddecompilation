#include "support/test_runner.h"
#include "src/types.h"

#include "src/r7/emie7cg.c"

static void assert_pattern(test_context *ctx, sprite_pattern *pattern,
                           int xoff, int yoff, int etc, int tile_index) {
    TEST_ASSERT_EQ_INT(ctx, 1, pattern->cnt);
    TEST_ASSERT_EQ_INT(ctx, xoff, pattern->spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, yoff, pattern->spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, etc, pattern->spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, tile_index, pattern->spra[0].index);
}

static void assert_char_table(test_context *ctx, char *table,
                              const int *expected, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        TEST_ASSERT_EQ_INT(ctx, expected[i], table[i]);
    }
}

static void test_emie7cg_patterns_match_existing_sprite_records(
    test_context *ctx) {
    sprite_pattern *expected_pointers[20] = {
        &pat0, &pat1, &pat2, &pat3, &pat4, &pat5, &pat6,
        &pat7, &pat8, &pat9, &patA, &patB, &patC, &patD,
        &patE, &patF, &pat10, &pat11, &pat12, &pat13};
    int i;

    assert_pattern(ctx, &pat0, -16, -24, 0, 421);
    assert_pattern(ctx, &pat1, -12, -24, 0, 422);
    assert_pattern(ctx, &pat2, -16, -24, 0, 423);
    assert_pattern(ctx, &pat3, -12, -23, 0, 424);
    assert_pattern(ctx, &pat4, -12, -23, 0, 425);
    assert_pattern(ctx, &pat5, -12, -23, 0, 426);
    assert_pattern(ctx, &pat6, -16, -24, 0, 427);
    assert_pattern(ctx, &pat7, -16, -24, 0, 428);
    assert_pattern(ctx, &pat8, -8, -8, 0, 429);
    assert_pattern(ctx, &pat9, -8, -8, 0, 430);
    assert_pattern(ctx, &patA, -8, -8, 0, 431);
    assert_pattern(ctx, &patB, -12, -20, 0, 432);
    assert_pattern(ctx, &patC, -21, -20, 0, 433);
    assert_pattern(ctx, &patD, -12, -24, 0, 434);
    assert_pattern(ctx, &patE, -16, -20, 0, 435);
    assert_pattern(ctx, &patF, -12, -24, 0, 436);
    assert_pattern(ctx, &pat10, -16, -20, 0, 437);
    assert_pattern(ctx, &pat11, -12, -20, 0, 438);
    assert_pattern(ctx, &pat12, -12, -20, 0, 439);
    assert_pattern(ctx, &pat13, -12, -20, 0, 440);

    for (i = 0; i < 20; ++i) {
        TEST_ASSERT_TRUE(ctx, emie7pat[i] == expected_pointers[i]);
    }
}

static void test_emie7cg_animation_tables_match_existing_scripts(
    test_context *ctx) {
    const int moji_expected[9] = {3, 11, 4, 11, 5, 11, 4, 11, -1};
    const int run_expected[9] = {0, 3, 1, 2, 2, 3, 1, 2, -1};
    const int kiss_expected[3] = {14, 9, -1};
    const int stnd2_expected[5] = {11, 19, 12, 19, -1};
    const int moji2_expected[5] = {6, 9, 7, 9, -1};
    const int kyoro_expected[9] = {11, 19, 12, 19, 11, 19, 12, 19, -1};
    const int help_expected[9] = {17, 7, 18, 7, 17, 7, 19, 7, -1};

    assert_char_table(ctx, moji_pchg, moji_expected, 9);
    assert_char_table(ctx, run_pchg, run_expected, 9);
    assert_char_table(ctx, kiss_pchg, kiss_expected, 3);
    assert_char_table(ctx, stnd2_pchg, stnd2_expected, 5);
    assert_char_table(ctx, moji2_pchg, moji2_expected, 5);
    assert_char_table(ctx, kyoro_pchg, kyoro_expected, 9);
    assert_char_table(ctx, help_pchg, help_expected, 9);

    TEST_ASSERT_TRUE(ctx, em_pchg[0] == stnd2_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[1] == moji_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[2] == run_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[3] == kiss_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[4] == moji2_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[5] == kyoro_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[6] == help_pchg);
}

TEST_MAIN_BEGIN;
test_emie7cg_patterns_match_existing_sprite_records(&ctx);
test_emie7cg_animation_tables_match_existing_scripts(&ctx);
TEST_MAIN_END

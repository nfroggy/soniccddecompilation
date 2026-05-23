#include "support/test_runner.h"
#include "src/types.h"

#define R31A
#include "src/r3/msnccg.c"

static void assert_sprite(test_context *ctx, sprite_pattern *pattern,
                          int count, int xoff, int yoff, int etc,
                          int index) {
    TEST_ASSERT_EQ_INT(ctx, count, pattern->cnt);
    TEST_ASSERT_EQ_INT(ctx, xoff, pattern->spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, yoff, pattern->spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, etc, pattern->spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, index, pattern->spra[0].index);
}

static void assert_bytes(test_context *ctx, const Uint8 *actual,
                         const Uint8 *expected, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        TEST_ASSERT_EQ_INT(ctx, expected[i], actual[i]);
    }
}

static void test_mecasnc3_patterns(test_context *ctx) {
    sprite_pattern *expected_pointers[16] = {
        &msnc3_pat0, &msnc3_pat1, &msnc3_pat2, &msnc3_pat3,
        &msnc3_pat4, &msnc3_pat5, &msnc3_pat6, &msnc3_pat7,
        &msnc3_pat8, &msnc3_pat9, &msnc3_patA, &msnc3_patB,
        &msnc3_patC, &msnc3_patD, &msnc3_patE, &msnc3_patF};
    const int expected[16][5] = {
        {1, -24, -12, 0, 418}, {1, -24, -12, 0, 419},
        {1, -32, -24, 0, 420}, {1, -32, -24, 0, 421},
        {1, -32, -24, 0, 422}, {1, -32, -24, 0, 423},
        {1, -32, -24, 0, 424}, {1, -40, -24, 0, 425},
        {1, -40, -32, 0, 426}, {1, -48, -32, 0, 427},
        {1, -8, -8, 0, 428},   {1, -8, -8, 0, 429},
        {1, -12, -12, 0, 430}, {1, -16, -16, 0, 431},
        {1, -20, -24, 0, 432}, {0, -28, -24, 0, 433}};
    int i;

    for (i = 0; i < 16; ++i) {
        TEST_ASSERT_TRUE(ctx, mecasnc3pat[i] == expected_pointers[i]);
        assert_sprite(ctx, mecasnc3pat[i], expected[i][0], expected[i][1],
                      expected[i][2], expected[i][3], expected[i][4]);
    }
}

static void test_hari3x_patterns(test_context *ctx) {
    sprite_pattern *expected_pointers[15] = {
        &hari3x_pat0, &hari3x_pat1, &hari3x_pat2, &hari3x_pat3,
        &hari3x_pat4, &hari3x_pat5, &hari3x_pat6, &hari3x_pat7,
        &hari3x_pat8, &hari3x_pat9, &hari3x_pata, &hari3x_patb,
        &hari3x_patc, &hari3x_patd, &hari3x_pate};
    const int expected[15][5] = {
        {1, -16, -16, 0, 450}, {1, -8, -8, 0, 451},
        {1, -8, -8, 0, 452},   {1, -8, -8, 0, 453},
        {1, -8, -8, 0, 454},   {1, -8, -8, 0, 455},
        {1, -8, -8, 0, 456},   {1, -8, -8, 0, 457},
        {1, -8, -8, 0, 458},   {1, -16, -16, 0, 459},
        {1, -8, -8, 0, 460},   {1, -8, -8, 0, 461},
        {1, -8, -8, 0, 462},   {1, -8, -8, 0, 463},
        {1, -16, -16, 0, 464}};
    int i;

    for (i = 0; i < 15; ++i) {
        TEST_ASSERT_TRUE(ctx, hari3xpat[i] == expected_pointers[i]);
        assert_sprite(ctx, hari3xpat[i], expected[i][0], expected[i][1],
                      expected[i][2], expected[i][3], expected[i][4]);
    }
}

static void test_emie3_patterns_for_r31a_branch(test_context *ctx) {
    sprite_pattern *expected_pointers[14] = {
        &emie3_pat0, &emie3_pat1, &emie3_pat2, &emie3_pat3, &emie3_pat4,
        &emie3_pat5, &emie3_pat6, &emie3_pat7, &emie3_pat8, &emie3_pat9,
        &emie3_patA, &emie3_patB, &emie3_patC, &emie3_patD};
    const int expected[14][5] = {
        {1, -16, -20, 0, 433}, {1, -12, -20, 0, 434},
        {1, -16, -20, 0, 435}, {1, -12, -20, 0, 436},
        {1, -12, -20, 0, 437}, {1, -12, -20, 0, 438},
        {0, -8, -8, 0, 439},   {0, -8, -8, 0, 440},
        {1, -8, -8, 0, 441},   {1, -8, -8, 0, 442},
        {1, -8, -8, 0, 443},   {1, -12, -20, 0, 444},
        {1, -12, -20, 0, 445}, {0, -8, -8, 0, 446}};
    int i;

    for (i = 0; i < 14; ++i) {
        TEST_ASSERT_TRUE(ctx, emie3pat[i] == expected_pointers[i]);
        assert_sprite(ctx, emie3pat[i], expected[i][0], expected[i][1],
                      expected[i][2], expected[i][3], expected[i][4]);
    }
}

static void test_emie_animation_tables(test_context *ctx) {
    const Uint8 expected_moji[9] = {3, 39, 4, 9, 5, 39, 4, 9, 255};
    const Uint8 expected_run[9] = {0, 3, 1, 2, 2, 3, 1, 2, 255};
    const Uint8 expected_help[5] = {11, 13, 12, 13, 255};
    const Uint8 expected_kiss[5] = {6, 19, 7, 9, 255};

    assert_bytes(ctx, moji_pchg, expected_moji, 9);
    assert_bytes(ctx, run_pchg, expected_run, 9);
    assert_bytes(ctx, help_pchg, expected_help, 5);
    assert_bytes(ctx, kiss_pchg, expected_kiss, 5);
    TEST_ASSERT_TRUE(ctx, em_pchg[0] == moji_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[1] == run_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[2] == help_pchg);
    TEST_ASSERT_TRUE(ctx, em_pchg[3] == kiss_pchg);
}

TEST_MAIN_BEGIN;
test_mecasnc3_patterns(&ctx);
test_hari3x_patterns(&ctx);
test_emie3_patterns_for_r31a_branch(&ctx);
test_emie_animation_tables(&ctx);
TEST_MAIN_END

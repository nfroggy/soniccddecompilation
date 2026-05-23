#include "support/test_runner.h"
#include "src/types.h"

#include "src/r7/zonetbl7.c"

static void test_zonetbl7_title_table_matches_existing_layout(
    test_context *ctx) {
    static const Uint16 expected[] = {
        304, 552, 360, 346, 256, 568, 376, 602,
        256, 576, 384, 602, 256, 584, 392, 602,
        288, 560, 368, 858, 320, 584, 392, 1114,
        256, 464, 272, 1882, 256, 464, 272, 2138,
    };
    int i;

    TEST_ASSERT_EQ_INT(ctx, 32,
                       (int)(sizeof(title_tbl) / sizeof(title_tbl[0])));
    for (i = 0; i < 32; ++i) {
        TEST_ASSERT_EQ_INT(ctx, expected[i], title_tbl[i]);
    }
}

static void assert_title_pattern(test_context *ctx, int index, Sint8 xoff,
                                 Sint8 yoff, Uint8 etc, Uint16 tile_index) {
    sprite_pattern *pattern = title_pat[index];

    TEST_ASSERT_EQ_INT(ctx, 1, pattern->cnt);
    TEST_ASSERT_EQ_INT(ctx, xoff, pattern->spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, yoff, pattern->spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, etc, pattern->spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, tile_index, pattern->spra[0].index);
}

static void test_zonetbl7_title_patterns_match_existing_sprites(
    test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 9, (int)(sizeof(title_pat) / sizeof(title_pat[0])));

    assert_title_pattern(ctx, 0, -16, -112, 0, 339);
    assert_title_pattern(ctx, 1, -24, -8, 0, 340);
    assert_title_pattern(ctx, 2, -4, -24, 0, 341);
    assert_title_pattern(ctx, 3, -80, -8, 0, 342);
    assert_title_pattern(ctx, 4, -24, -24, 0, 343);
    assert_title_pattern(ctx, 5, -24, -24, 0, 344);
    assert_title_pattern(ctx, 6, -24, -24, 0, 345);
    assert_title_pattern(ctx, 7, 0, -56, 0, 346);
    assert_title_pattern(ctx, 8, 0, 0, 0, 347);
}

TEST_MAIN_BEGIN;
    test_zonetbl7_title_table_matches_existing_layout(&ctx);
    test_zonetbl7_title_patterns_match_existing_sprites(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static PALETTEENTRY palette[80];
PALETTEENTRY *lpcolorwk;

#include "src/title/ta/tacolor.c"

static void reset_palette(void) {
    int i;

    lpcolorwk = palette;
    for (i = 0; i < 80; ++i) {
        palette[i].peRed = (Uint8)(i + 1);
        palette[i].peGreen = (Uint8)(i + 2);
        palette[i].peBlue = (Uint8)(i + 3);
        palette[i].peFlags = (Uint8)(i + 4);
    }
    memset(clchgcnt, 0, sizeof(clchgcnt));
}

static void assert_palette_entry(test_context *ctx, PALETTEENTRY expected,
                                 PALETTEENTRY actual) {
    TEST_ASSERT_EQ_INT(ctx, expected.peRed, actual.peRed);
    TEST_ASSERT_EQ_INT(ctx, expected.peGreen, actual.peGreen);
    TEST_ASSERT_EQ_INT(ctx, expected.peBlue, actual.peBlue);
    TEST_ASSERT_EQ_INT(ctx, expected.peFlags, actual.peFlags);
}

static PALETTEENTRY make_entry(Uint8 red, Uint8 green, Uint8 blue, Uint8 flags) {
    PALETTEENTRY entry;

    entry.peRed = red;
    entry.peGreen = green;
    entry.peBlue = blue;
    entry.peFlags = flags;
    return entry;
}

static void test_tacolorset_copies_all_sixty_four_entries(test_context *ctx) {
    int i;

    reset_palette();

    TAColorSet(123);

    for (i = 0; i < 64; ++i) {
        assert_palette_entry(ctx, tacolor[i], palette[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 65, palette[64].peRed);
    TEST_ASSERT_EQ_INT(ctx, 66, palette[64].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 67, palette[64].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 68, palette[64].peFlags);
}

static void test_tapiccolorset_copies_stage_bank_only(test_context *ctx) {
    int i;

    reset_palette();

    TAPicColorSet(1, 7);

    TEST_ASSERT_EQ_INT(ctx, 32, palette[31].peRed);
    TEST_ASSERT_EQ_INT(ctx, 49, palette[48].peRed);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, ta_stage_col[1][7][i], palette[32 + i]);
    }
}

static void test_cntplus_returns_new_value_and_wraps_above_limit(
    test_context *ctx) {
    Uint16 work;

    work = 7;
    TEST_ASSERT_EQ_INT(ctx, 12, cntplus(&work, 5, 3, 12));
    TEST_ASSERT_EQ_INT(ctx, 12, work);

    TEST_ASSERT_EQ_INT(ctx, 3, cntplus(&work, 1, 3, 12));
    TEST_ASSERT_EQ_INT(ctx, 3, work);
}

static void test_tacolorchgsp6_advances_color_window(test_context *ctx) {
    reset_palette();

    TAColorChgSP6();

    TEST_ASSERT_EQ_INT(ctx, 33, palette[32].peRed);
    assert_palette_entry(ctx, make_entry(239, 142, 15, 1), palette[33]);
    assert_palette_entry(ctx, make_entry(239, 206, 15, 1), palette[34]);
    assert_palette_entry(ctx, make_entry(175, 234, 79, 1), palette[35]);
    assert_palette_entry(ctx, make_entry(143, 232, 143, 1), palette[36]);
    assert_palette_entry(ctx, make_entry(79, 228, 207, 1), palette[37]);
    TEST_ASSERT_EQ_INT(ctx, 39, palette[38].peRed);

    TAColorChgSP6();

    assert_palette_entry(ctx, make_entry(239, 206, 15, 1), palette[33]);
    assert_palette_entry(ctx, make_entry(79, 164, 239, 1), palette[37]);
}

static void test_tacolorchgsp6_wraps_counter_to_first_color(test_context *ctx) {
    reset_palette();
    clchgcnt[0] = 24;

    TAColorChgSP6();

    assert_palette_entry(ctx, make_entry(239, 78, 15, 1), palette[33]);
    assert_palette_entry(ctx, make_entry(239, 142, 15, 1), palette[34]);
    assert_palette_entry(ctx, make_entry(143, 232, 143, 1), palette[37]);
}

TEST_MAIN_BEGIN;
test_tacolorset_copies_all_sixty_four_entries(&ctx);
test_tapiccolorset_copies_stage_bank_only(&ctx);
test_cntplus_returns_new_value_and_wraps_above_limit(&ctx);
test_tacolorchgsp6_advances_color_window(&ctx);
test_tacolorchgsp6_wraps_counter_to_first_color(&ctx);
TEST_MAIN_END

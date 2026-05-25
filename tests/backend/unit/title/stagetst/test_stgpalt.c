#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static PALETTEENTRY palette[80];
PALETTEENTRY *lpColorwk;

#include "src/title/stagetst/stgpalt.c"

static void reset_palette(void) {
    int i;

    lpColorwk = palette;
    for (i = 0; i < 80; ++i) {
        palette[i].peRed = (Uint8)(i + 1);
        palette[i].peGreen = (Uint8)(i + 2);
        palette[i].peBlue = (Uint8)(i + 3);
        palette[i].peFlags = (Uint8)(i + 4);
    }
}

static void assert_palette_entry(test_context *ctx, PALETTEENTRY expected,
                                 PALETTEENTRY actual) {
    TEST_ASSERT_EQ_INT(ctx, expected.peRed, actual.peRed);
    TEST_ASSERT_EQ_INT(ctx, expected.peGreen, actual.peGreen);
    TEST_ASSERT_EQ_INT(ctx, expected.peBlue, actual.peBlue);
    TEST_ASSERT_EQ_INT(ctx, expected.peFlags, actual.peFlags);
}

static void test_oeclrset_copies_sixteen_entries_to_selected_bank(
    test_context *ctx) {
    PALETTEENTRY before_bank;
    PALETTEENTRY after_bank;
    int i;

    reset_palette();
    before_bank = palette[15];
    after_bank = palette[32];

    OEClrset(tblPal2, 1);

    assert_palette_entry(ctx, before_bank, palette[15]);
    assert_palette_entry(ctx, after_bank, palette[32]);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, tblPal2[i], palette[16 + i]);
    }
}

static void test_oeclrset_uses_source_pointer_not_fixed_tables(
    test_context *ctx) {
    PALETTEENTRY custom[16];
    int i;

    reset_palette();
    for (i = 0; i < 16; ++i) {
        custom[i].peRed = (Uint8)(180 - i);
        custom[i].peGreen = (Uint8)(20 + i * 2);
        custom[i].peBlue = (Uint8)(70 + i);
        custom[i].peFlags = (Uint8)(3 + i);
    }

    OEClrset(custom, 4);

    TEST_ASSERT_EQ_INT(ctx, 64, palette[63].peRed);
    TEST_ASSERT_EQ_INT(ctx, 65, palette[63].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 66, palette[63].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 67, palette[63].peFlags);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, custom[i], palette[64 + i]);
    }
}

TEST_MAIN_BEGIN;
test_oeclrset_copies_sixteen_entries_to_selected_bank(&ctx);
test_oeclrset_uses_source_pointer_not_fixed_tables(&ctx);
TEST_MAIN_END

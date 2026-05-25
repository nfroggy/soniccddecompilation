#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static PALETTEENTRY palette[80];
PALETTEENTRY *lpColorwk;

#include "src/title/opening/opnpalt.c"

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
    before_bank = palette[31];
    after_bank = palette[48];

    OEClrset(tblPal2a, 2);

    assert_palette_entry(ctx, before_bank, palette[31]);
    assert_palette_entry(ctx, after_bank, palette[48]);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, tblPal2a[i], palette[32 + i]);
    }
}

static void test_oeclrset_uses_source_pointer_not_fixed_tables(
    test_context *ctx) {
    PALETTEENTRY custom[16];
    int i;

    reset_palette();
    for (i = 0; i < 16; ++i) {
        custom[i].peRed = (Uint8)(11 + i);
        custom[i].peGreen = (Uint8)(140 + i * 3);
        custom[i].peBlue = (Uint8)(230 - i);
        custom[i].peFlags = (Uint8)(90 + i);
    }

    OEClrset(custom, 0);

    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, custom[i], palette[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 17, palette[16].peRed);
    TEST_ASSERT_EQ_INT(ctx, 18, palette[16].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 19, palette[16].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 20, palette[16].peFlags);
}

TEST_MAIN_BEGIN;
test_oeclrset_copies_sixteen_entries_to_selected_bank(&ctx);
test_oeclrset_uses_source_pointer_not_fixed_tables(&ctx);
TEST_MAIN_END

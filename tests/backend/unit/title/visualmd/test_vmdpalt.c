#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static PALETTEENTRY palette[96];
PALETTEENTRY *lpColorwk;

#include "src/title/visualmd/vmdpalt.c"

static void reset_palette(void) {
    int i;

    lpColorwk = palette;
    for (i = 0; i < 96; ++i) {
        palette[i].peRed = (Uint8)(i + 21);
        palette[i].peGreen = (Uint8)(i + 22);
        palette[i].peBlue = (Uint8)(i + 23);
        palette[i].peFlags = (Uint8)(i + 24);
    }
}

static void assert_palette_entry(test_context *ctx, PALETTEENTRY expected,
                                 PALETTEENTRY actual) {
    TEST_ASSERT_EQ_INT(ctx, expected.peRed, actual.peRed);
    TEST_ASSERT_EQ_INT(ctx, expected.peGreen, actual.peGreen);
    TEST_ASSERT_EQ_INT(ctx, expected.peBlue, actual.peBlue);
    TEST_ASSERT_EQ_INT(ctx, expected.peFlags, actual.peFlags);
}

static void test_oeclrset_copies_visual_mode_palette_bank(test_context *ctx) {
    PALETTEENTRY before_bank;
    PALETTEENTRY after_bank;
    int i;

    reset_palette();
    before_bank = palette[63];
    after_bank = palette[80];

    OEClrset(tblPal4, 4);

    assert_palette_entry(ctx, before_bank, palette[63]);
    assert_palette_entry(ctx, after_bank, palette[80]);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, tblPal4[i], palette[64 + i]);
    }
}

static void test_oeclrset_does_not_depend_on_builtin_tables(test_context *ctx) {
    PALETTEENTRY custom[16];
    int i;

    reset_palette();
    for (i = 0; i < 16; ++i) {
        custom[i].peRed = (Uint8)(180 - i);
        custom[i].peGreen = (Uint8)(33 + i);
        custom[i].peBlue = (Uint8)(99 - i);
        custom[i].peFlags = (Uint8)(150 + i);
    }

    OEClrset(custom, 2);

    TEST_ASSERT_EQ_INT(ctx, 52, palette[31].peRed);
    TEST_ASSERT_EQ_INT(ctx, 53, palette[31].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 54, palette[31].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 55, palette[31].peFlags);
    for (i = 0; i < 16; ++i) {
        assert_palette_entry(ctx, custom[i], palette[32 + i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 69, palette[48].peRed);
    TEST_ASSERT_EQ_INT(ctx, 70, palette[48].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 71, palette[48].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 72, palette[48].peFlags);
}

TEST_MAIN_BEGIN;
test_oeclrset_copies_visual_mode_palette_bank(&ctx);
test_oeclrset_does_not_depend_on_builtin_tables(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static PALETTEENTRY palette[64];
Sint32 fade_flag;
Sint32 *lpFadeFlag;
PALETTEENTRY *lpcolorwk;
PALETTEENTRY colorsave[64];
Uint8 clfflg;
Uint8 intflg;

#include "src/special/fadein.c"

static void reset_fade_state(void) {
    int i;

    memset(palette, 0, sizeof(palette));
    memset(colorsave, 0, sizeof(colorsave));
    fade_flag = -1;
    lpFadeFlag = &fade_flag;
    lpcolorwk = palette;
    clfflg = 0;
    intflg = 0;
    byMskwk = 0;
    wColwk = 0;
    FadeCount = 0;
    for (i = 0; i < 64; ++i) {
        palette[i].peFlags = (Uint8)(i + 1);
    }
}

static void test_woutcolor_steps_selected_channel_only(test_context *ctx) {
    reset_fade_state();
    palette[0].peRed = 191;
    palette[0].peGreen = 20;
    palette[0].peBlue = 30;
    palette[1].peRed = 224;
    palette[1].peGreen = 223;
    palette[1].peBlue = 222;

    woutcolor(1);
    TEST_ASSERT_EQ_INT(ctx, 223, palette[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 20, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 30, palette[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 224, palette[1].peRed);
    TEST_ASSERT_EQ_INT(ctx, 223, palette[1].peGreen);

    woutcolor(5);
    TEST_ASSERT_EQ_INT(ctx, 52, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 255, palette[1].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 222, palette[1].peBlue);

    woutcolor(9);
    TEST_ASSERT_EQ_INT(ctx, 62, palette[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 254, palette[1].peBlue);
}

static void test_clwout_sequences_red_green_blue_then_clears_flag(
    test_context *ctx) {
    int i;

    reset_fade_state();
    clwout();
    TEST_ASSERT_EQ_INT(ctx, 1, clfflg);
    TEST_ASSERT_EQ_INT(ctx, 2, fade_flag);

    for (i = 0; i < 9; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 0, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, palette[0].peBlue);

    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 1, wColwk);

    for (i = 0; i < 9; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peGreen);

    FadeProc();
    for (i = 0; i < 9; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peBlue);

    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 3, wColwk);
    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 0, fade_flag);
}

static void test_clwin_init_saves_palette_and_whitens_work_palette(
    test_context *ctx) {
    reset_fade_state();
    palette[0].peRed = 10;
    palette[0].peGreen = 20;
    palette[0].peBlue = 30;
    palette[0].peFlags = 40;
    palette[63].peRed = 100;
    palette[63].peGreen = 110;
    palette[63].peBlue = 120;
    palette[63].peFlags = 130;

    clwin_init();

    TEST_ASSERT_EQ_INT(ctx, 10, colorsave[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 20, colorsave[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 30, colorsave[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 40, colorsave[0].peFlags);
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 1, palette[0].peFlags);
    TEST_ASSERT_EQ_INT(ctx, 100, colorsave[63].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, palette[63].peRed);
}

static void test_wincolor_restores_channels_down_to_saved_values(
    test_context *ctx) {
    reset_fade_state();
    colorsave[0].peRed = 10;
    colorsave[0].peGreen = 20;
    colorsave[0].peBlue = 30;
    colorsave[1].peRed = 220;
    colorsave[1].peGreen = 221;
    colorsave[1].peBlue = 222;

    wincolor(96, 1);
    TEST_ASSERT_EQ_INT(ctx, 96, palette[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 0, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, palette[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 220, palette[1].peRed);
    TEST_ASSERT_EQ_INT(ctx, 2, intflg & 2);

    intflg = 0;
    wincolor(16, 5);
    TEST_ASSERT_EQ_INT(ctx, 20, palette[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 221, palette[1].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 2, intflg & 2);

    intflg = 0;
    wincolor(64, 9);
    TEST_ASSERT_EQ_INT(ctx, 64, palette[0].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 222, palette[1].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 2, intflg & 2);
}

static void test_clwin_sequences_mask_and_clears_flag(test_context *ctx) {
    int i;

    reset_fade_state();
    colorsave[0].peRed = 10;
    colorsave[0].peGreen = 20;
    colorsave[0].peBlue = 30;

    clwin();
    TEST_ASSERT_EQ_INT(ctx, 1, fade_flag);
    TEST_ASSERT_EQ_INT(ctx, 224, byMskwk);

    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 224, palette[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 192, byMskwk);

    for (i = 0; i < 7; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 10, palette[0].peRed);

    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 1, wColwk);
    TEST_ASSERT_EQ_INT(ctx, 224, byMskwk);

    for (i = 0; i < 9; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 20, palette[0].peGreen);

    FadeProc();
    for (i = 0; i < 9; ++i) {
        FadeProc();
    }
    TEST_ASSERT_EQ_INT(ctx, 30, palette[0].peBlue);

    FadeProc();
    FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 0, fade_flag);
}

static void test_fadeproc_ignores_unknown_flags(test_context *ctx) {
    reset_fade_state();
    fade_flag = 99;
    palette[0].peRed = 7;

    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 99, fade_flag);
    TEST_ASSERT_EQ_INT(ctx, 7, palette[0].peRed);
}

TEST_MAIN_BEGIN;
test_woutcolor_steps_selected_channel_only(&ctx);
test_clwout_sequences_red_green_blue_then_clears_flag(&ctx);
test_clwin_init_saves_palette_and_whitens_work_palette(&ctx);
test_wincolor_restores_channels_down_to_saved_values(&ctx);
test_clwin_sequences_mask_and_clears_flag(&ctx);
test_fadeproc_ignores_unknown_flags(&ctx);
TEST_MAIN_END

#include <stdio.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 startcolor;
Uint8 colorcnt;
Uint8 int_flg;
Sint32 fade_flag_storage;
Sint32 *lpFadeFlag = &fade_flag_storage;
PALETTEENTRY colorwk[256];
PALETTEENTRY colorwk2[256];
PALETTEENTRY colorwk3[256];
PALETTEENTRY colorwk4[256];
PALETTEENTRY *lpcolorwk = colorwk;
PALETTEENTRY *lpcolorwk2 = colorwk2;
PALETTEENTRY *lpcolorwk3 = colorwk3;
PALETTEENTRY *lpcolorwk4 = colorwk4;
Uint8 clchgtim[7];
Uint8 clchgcnt[7];
short_union stageno;

#include "src/r1/col1d.c"

static void reset_col1d_state(void) {
    memset(colorwk, 0, sizeof(colorwk));
    memset(colorwk2, 0, sizeof(colorwk2));
    memset(colorwk3, 0, sizeof(colorwk3));
    memset(colorwk4, 0, sizeof(colorwk4));
    memset(clchgtim, 0, sizeof(clchgtim));
    memset(clchgcnt, 0, sizeof(clchgcnt));
    memset(&stageno, 0, sizeof(stageno));
    startcolor = 0;
    colorcnt = 0;
    int_flg = 0;
    fade_flag_storage = 0;
    FadeCount = 0;
}

static void assert_palette(test_context *ctx, PALETTEENTRY actual, int red,
                           int green, int blue, int flags,
                           const char *label) {
    if (actual.peRed != red || actual.peGreen != green ||
        actual.peBlue != blue || actual.peFlags != flags) {
        ++ctx->failures;
        fprintf(stderr,
                "%s: expected {%d,%d,%d,%d}, got {%u,%u,%u,%u}\n", label,
                red, green, blue, flags, actual.peRed, actual.peGreen,
                actual.peBlue, actual.peFlags);
    }
}

static void test_col1d_tables_capture_palette_contract(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 3,
                       (int)(sizeof(clst1_1d0) / sizeof(clst1_1d0[0])));
    TEST_ASSERT_EQ_INT(ctx, 3,
                       (int)(sizeof(clst1_1d1) / sizeof(clst1_1d1[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor) / sizeof(advacolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor2) / sizeof(advacolor2[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(gamecolor) / sizeof(gamecolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone1colord) / sizeof(zone1colord[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(zone1colboss1) / sizeof(zone1colboss1[0])));
    TEST_ASSERT_EQ_INT(ctx, 18, (int)(sizeof(col1d0cnt) / sizeof(col1d0cnt[0])));

    assert_palette(ctx, clst1_1d0[0][0], 128, 128, 128, 1, "clst1_1d0");
    assert_palette(ctx, clst1_1d1[2][3], 192, 128, 192, 1, "clst1_1d1");
    assert_palette(ctx, gamecolor[0], 96, 64, 96, 1, "gamecolor[0]");
    assert_palette(ctx, zone1colord[47], 224, 160, 0, 1, "zone1colord[47]");
    assert_palette(ctx, zone1colboss1[15], 224, 224, 0, 1,
                   "zone1colboss1[15]");
    TEST_ASSERT_TRUE(ctx, colortbl[4].PeSrc == zone1colord);
    TEST_ASSERT_TRUE(ctx, colortbl[5].PeSrc == zone1colboss1);
    TEST_ASSERT_EQ_INT(ctx, 16, colortbl[4].offset);
    TEST_ASSERT_EQ_INT(ctx, 48, colortbl[4].cnt);
}

static void test_clchgctr_updates_strips_red_cycle_and_wraps(
    test_context *ctx) {
    reset_col1d_state();

    clchgctr();

    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[2]);
    TEST_ASSERT_EQ_INT(ctx, 7, clchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, clchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[2]);
    assert_palette(ctx, colorwk[53], 64, 64, 64, 1, "strip 53 first");
    assert_palette(ctx, colorwk[56], 96, 96, 96, 1, "strip 56 first");
    assert_palette(ctx, colorwk[44], 192, 128, 192, 1, "strip 44 first");
    assert_palette(ctx, colorwk[47], 224, 160, 192, 1, "strip 47 first");
    assert_palette(ctx, colorwk[34], 192, 0, 0, 1, "red cycle first");

    reset_col1d_state();
    clchgcnt[0] = 2;
    clchgcnt[1] = 2;
    clchgcnt[2] = 7;
    clchgctr();

    TEST_ASSERT_EQ_INT(ctx, 0, clchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, clchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, clchgcnt[2]);
    assert_palette(ctx, colorwk[53], 128, 128, 128, 1, "strip 53 wrapped");
    assert_palette(ctx, colorwk[44], 224, 160, 192, 1, "strip 44 wrapped");
    assert_palette(ctx, colorwk[34], 224, 0, 0, 1, "red cycle wrapped");
}

static void test_clchg_sub0_waits_updates_and_wraps(test_context *ctx) {
    Uint8 cnt_tbl[] = {4, 2, 7, 1, 9, 0};
    PALETTEENTRY colors[] = {{11, 22, 33, 1}, {44, 55, 66, 1}};
    Uint8 change_time;
    Uint8 change_count;

    reset_col1d_state();
    change_time = 3;
    change_count = 0;
    colorwk[4] = (PALETTEENTRY){1, 2, 3, 4};

    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);
    TEST_ASSERT_EQ_INT(ctx, 2, change_time);
    TEST_ASSERT_EQ_INT(ctx, 0, change_count);
    assert_palette(ctx, colorwk[4], 1, 2, 3, 4, "waiting color");

    change_time = 0;
    change_count = 0;
    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);
    TEST_ASSERT_EQ_INT(ctx, 9, change_time);
    TEST_ASSERT_EQ_INT(ctx, 1, change_count);
    assert_palette(ctx, colorwk[4], 11, 22, 33, 1, "advanced color");

    change_time = 0;
    change_count = 1;
    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);
    TEST_ASSERT_EQ_INT(ctx, 7, change_time);
    TEST_ASSERT_EQ_INT(ctx, 0, change_count);
    assert_palette(ctx, colorwk[4], 44, 55, 66, 1, "wrapped color");
}

static void test_fadein_and_fadeproc_capture_range_steps(test_context *ctx) {
    reset_col1d_state();
    startcolor = 4;
    colorcnt = 2;
    colorwk[2] = (PALETTEENTRY){99, 88, 77, 0};
    colorwk[4] = (PALETTEENTRY){99, 88, 77, 0};

    fadein0();

    assert_palette(ctx, colorwk[2], 0, 0, 0, 1, "fadein0 first");
    assert_palette(ctx, colorwk[4], 0, 0, 0, 1, "fadein0 last");
    TEST_ASSERT_EQ_INT(ctx, 1, fade_flag_storage);

    colorwk2[2] = (PALETTEENTRY){64, 32, 96, 1};
    colorwk2[3] = (PALETTEENTRY){64, 0, 0, 1};
    colorwk4[2] = (PALETTEENTRY){0, 0, 64, 1};
    colorwk4[3] = (PALETTEENTRY){0, 64, 0, 1};
    colorwk4[4] = (PALETTEENTRY){64, 0, 0, 1};

    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 18, int_flg);
    assert_palette(ctx, colorwk[2], 0, 0, 32, 1, "fadein blue first");
    assert_palette(ctx, colorwk[3], 32, 0, 0, 1, "fadein red first");
    assert_palette(ctx, colorwk3[2], 0, 0, 32, 0, "fadein wk3 blue");
    assert_palette(ctx, colorwk3[3], 0, 32, 0, 0, "fadein wk3 green");
    assert_palette(ctx, colorwk3[4], 32, 0, 0, 0, "fadein wk3 red");

    stageno.b.h = 1;
    colorwk3[2] = (PALETTEENTRY){0, 0, 0, 0};
    colorwk4[2] = (PALETTEENTRY){0, 0, 64, 1};
    TEST_ASSERT_EQ_INT(ctx, 0, fadein0_new());
    assert_palette(ctx, colorwk3[2], 0, 0, 0, 0, "fadein stage skip");

    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, fadein0_new());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);
}

static void test_fadeout_flashin_and_flashout_paths(test_context *ctx) {
    reset_col1d_state();
    colorwk[0] = (PALETTEENTRY){96, 64, 32, 1};
    colorwk[1] = (PALETTEENTRY){0, 64, 32, 1};
    colorwk[2] = (PALETTEENTRY){0, 0, 32, 1};
    colorwk3[0] = (PALETTEENTRY){32, 0, 0, 1};

    fadeout();
    TEST_ASSERT_EQ_INT(ctx, 2, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 64, 64, 32, 1, "fadeout red");
    assert_palette(ctx, colorwk[1], 0, 32, 32, 1, "fadeout green");
    assert_palette(ctx, colorwk[2], 0, 0, 0, 1, "fadeout blue");
    assert_palette(ctx, colorwk3[0], 0, 0, 0, 1, "fadeout wk3 red");
    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());

    reset_col1d_state();
    colorwk2[0] = (PALETTEENTRY){128, 96, 64, 1};
    colorwk2[1] = (PALETTEENTRY){224, 128, 224, 1};
    colorwk2[2] = (PALETTEENTRY){128, 224, 224, 1};
    colorwk4[0] = (PALETTEENTRY){224, 224, 192, 1};
    flashin();
    TEST_ASSERT_EQ_INT(ctx, 3, fade_flag_storage);
    assert_palette(ctx, colorwk[0], 224, 224, 224, 1, "flashin white");
    colorwk3[0] = (PALETTEENTRY){224, 224, 224, 0};
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 224, 224, 192, 1, "flashin blue down");
    assert_palette(ctx, colorwk[1], 224, 192, 224, 1, "flashin green down");
    assert_palette(ctx, colorwk[2], 192, 224, 224, 1, "flashin red down");
    assert_palette(ctx, colorwk3[0], 224, 224, 192, 0, "flashin wk3 blue");

    stageno.b.h = 1;
    colorwk3[0] = (PALETTEENTRY){224, 224, 224, 0};
    colorwk4[0] = (PALETTEENTRY){0, 0, 0, 1};
    TEST_ASSERT_EQ_INT(ctx, 0, flashin_new());
    assert_palette(ctx, colorwk3[0], 224, 224, 224, 0, "flashin stage skip");
    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, flashin_new());

    reset_col1d_state();
    colorwk[0] = (PALETTEENTRY){0, 0, 0, 1};
    colorwk[1] = (PALETTEENTRY){224, 0, 0, 1};
    colorwk[2] = (PALETTEENTRY){224, 224, 0, 1};
    colorwk3[0] = (PALETTEENTRY){0, 0, 0, 1};
    flashout();
    TEST_ASSERT_EQ_INT(ctx, 4, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 32, 0, 0, 1, "flashout red");
    assert_palette(ctx, colorwk[1], 224, 32, 0, 1, "flashout green");
    assert_palette(ctx, colorwk[2], 224, 224, 32, 1, "flashout blue");
    assert_palette(ctx, colorwk3[0], 32, 0, 0, 1, "flashout wk3 red");
    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
}

static void test_direct_helpers_and_colorset_routes(test_context *ctx) {
    PALETTEENTRY entry = {32, 64, 96, 7};
    PALETTEENTRY same = {32, 64, 96, 1};
    PALETTEENTRY white = {224, 224, 224, 3};

    fadein2(&entry, &same);
    assert_palette(ctx, entry, 32, 64, 96, 7, "fadein2 same");
    flashin2(&entry, &same);
    assert_palette(ctx, entry, 32, 64, 96, 7, "flashin2 same");
    fadeout2(&entry);
    fadeout2(&entry);
    fadeout2(&entry);
    assert_palette(ctx, entry, 0, 0, 96, 7, "fadeout2 priority");
    flashout2(&white);
    assert_palette(ctx, white, 224, 224, 224, 3, "flashout2 same");

    reset_col1d_state();
    colorset(3);
    assert_palette(ctx, colorwk2[0], 96, 64, 96, 1, "colorset work 1");
    assert_palette(ctx, colorwk2[15], 224, 224, 0, 1, "colorset work 1 end");
    colorset2(4);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset2 work 0");
    assert_palette(ctx, colorwk[63], 224, 160, 0, 1, "colorset2 work 0 end");
    colorset3(5);
    assert_palette(ctx, colorwk[16], 64, 96, 96, 1, "colorset3 work 0");
    assert_palette(ctx, colorwk[31], 224, 224, 0, 1, "colorset3 work 0 end");
    colorset00(0, 2);
    assert_palette(ctx, colorwk3[0], 0, 32, 160, 1, "colorset00 work 2");
    colorset00(0, 3);
    assert_palette(ctx, colorwk4[63], 224, 192, 0, 1,
                   "colorset00 default end");
}

static void test_fadeproc_unknown_flag_returns_complete(test_context *ctx) {
    reset_col1d_state();
    fade_flag_storage = 99;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
}

TEST_MAIN_BEGIN;
    test_col1d_tables_capture_palette_contract(&ctx);
    test_clchgctr_updates_strips_red_cycle_and_wraps(&ctx);
    test_clchg_sub0_waits_updates_and_wraps(&ctx);
    test_fadein_and_fadeproc_capture_range_steps(&ctx);
    test_fadeout_flashin_and_flashout_paths(&ctx);
    test_direct_helpers_and_colorset_routes(&ctx);
    test_fadeproc_unknown_flag_returns_complete(&ctx);
TEST_MAIN_END

#include <stdio.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 startcolor;
Uint8 colorcnt;
Uint8 int_flg;
Uint8 bossstart;
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

#include "src/r8/col8c.c"

static void reset_col8c_state(void) {
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
    bossstart = 0;
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

static void test_col8c_tables_capture_palette_contract(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor) / sizeof(advacolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor2) / sizeof(advacolor2[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone83ccolor) / sizeof(zone83ccolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone82ccolor) / sizeof(zone82ccolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(boss8col_1) / sizeof(boss8col_1[0])));
    TEST_ASSERT_EQ_INT(ctx, 40,
                       (int)(sizeof(boss8_col1) / sizeof(boss8_col1[0])));
    TEST_ASSERT_EQ_INT(ctx, 12, (int)(sizeof(colortbl) / sizeof(colortbl[0])));

    assert_palette(ctx, zone8colora[22], 96, 224, 224, 1, "zone8colora[22]");
    assert_palette(ctx, zone83ccolor[16], 32, 0, 160, 1,
                   "zone83ccolor[16]");
    assert_palette(ctx, zone82ccolor[36], 32, 32, 0, 1,
                   "zone82ccolor[36]");
    assert_palette(ctx, boss8col_1[0], 224, 224, 32, 1, "boss8col_1[0]");
    assert_palette(ctx, boss8_col1[35], 224, 96, 0, 1, "boss8_col1[35]");
    assert_palette(ctx, emie8col[4], 224, 96, 224, 1, "emie8col[4]");

    TEST_ASSERT_TRUE(ctx, colortbl[0].PeSrc == advacolor);
    TEST_ASSERT_TRUE(ctx, colortbl[5].PeSrc == zone83ccolor);
    TEST_ASSERT_TRUE(ctx, colortbl[6].PeSrc == zone82ccolor);
    TEST_ASSERT_TRUE(ctx, colortbl[7].PeSrc == boss8col_1);
    TEST_ASSERT_TRUE(ctx, colortbl[8].PeSrc == boss8col_0);
    TEST_ASSERT_TRUE(ctx, colortbl[10].PeSrc == 0);
    TEST_ASSERT_TRUE(ctx, colortbl[11].PeSrc == emie8col);
    TEST_ASSERT_EQ_INT(ctx, 80, colortbl[7].offset);
    TEST_ASSERT_EQ_INT(ctx, 16, colortbl[11].cnt);
}

static void test_colchg8c_updates_or_skips_stage_cycle(test_context *ctx) {
    reset_col8c_state();

    clchgctr();

    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, clchgtim[3]);
    assert_palette(ctx, colorwk[38], 0, 160, 192, 1, "cycle color 38");
    assert_palette(ctx, colorwk[39], 0, 96, 160, 1, "cycle color 39");
    assert_palette(ctx, colorwk[40], 96, 224, 224, 1, "cycle color 40");
    assert_palette(ctx, colorwk[49], 0, 224, 224, 1, "normal cycle color 49");

    reset_col8c_state();
    stageno.b.l = 1;

    colchg8c();

    TEST_ASSERT_EQ_INT(ctx, 0, clchgcnt[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, clchgtim[3]);
    assert_palette(ctx, colorwk[49], 0, 0, 0, 0, "skipped cycle color 49");
}

static void test_colchg_boss8_waits_advances_and_wraps(test_context *ctx) {
    Uint8 index;
    Uint8 timer;

    reset_col8c_state();
    colorwk[24] = (PALETTEENTRY){1, 2, 3, 4};
    index = 0;
    timer = 3;

    colchg_boss8(&index, &timer);

    TEST_ASSERT_EQ_INT(ctx, 0, index);
    TEST_ASSERT_EQ_INT(ctx, 3, timer);
    assert_palette(ctx, colorwk[24], 1, 2, 3, 4, "boss inactive");

    bossstart = 1;
    colchg_boss8(&index, &timer);

    TEST_ASSERT_EQ_INT(ctx, 0, timer);
    TEST_ASSERT_EQ_INT(ctx, 4, index);
    assert_palette(ctx, colorwk[24], 0, 0, 0, 1, "boss advanced 24");
    assert_palette(ctx, colorwk[27], 0, 0, 0, 1, "boss advanced 27");

    index = 36;
    timer = 3;
    colorwk[24] = (PALETTEENTRY){9, 9, 9, 9};

    colchg_boss8(&index, &timer);

    TEST_ASSERT_EQ_INT(ctx, 0, index);
    TEST_ASSERT_EQ_INT(ctx, 0, timer);
    assert_palette(ctx, colorwk[24], 0, 0, 0, 1, "boss wrapped 24");
    assert_palette(ctx, colorwk[25], 224, 96, 0, 1, "boss wrapped 25");
    assert_palette(ctx, colorwk[26], 0, 0, 224, 1, "boss wrapped 26");
    assert_palette(ctx, colorwk[27], 224, 0, 224, 1, "boss wrapped 27");

    timer = 1;
    colorwk[24] = (PALETTEENTRY){5, 6, 7, 8};
    colchg_boss8(&index, &timer);

    TEST_ASSERT_EQ_INT(ctx, 2, timer);
    TEST_ASSERT_EQ_INT(ctx, 0, index);
    assert_palette(ctx, colorwk[24], 5, 6, 7, 8, "boss wait");
}

static void test_fadein_and_boss_fade_steps_toward_sources(test_context *ctx) {
    PALETTEENTRY boss_dest[16];
    PALETTEENTRY boss_src[16];

    reset_col8c_state();
    startcolor = 4;
    colorcnt = 2;
    colorwk[2] = (PALETTEENTRY){99, 88, 77, 0};

    fadein0();

    assert_palette(ctx, colorwk[2], 0, 0, 0, 1, "fadein0 first");
    TEST_ASSERT_EQ_INT(ctx, 1, fade_flag_storage);

    colorwk2[2] = (PALETTEENTRY){64, 32, 96, 1};
    colorwk2[3] = (PALETTEENTRY){64, 0, 0, 1};
    colorwk4[2] = (PALETTEENTRY){0, 0, 64, 1};
    colorwk4[3] = (PALETTEENTRY){0, 64, 0, 1};

    TEST_ASSERT_EQ_INT(ctx, 0, fadein0_new());
    TEST_ASSERT_EQ_INT(ctx, 18, int_flg);
    assert_palette(ctx, colorwk[2], 0, 0, 32, 1, "fadein blue");
    assert_palette(ctx, colorwk[3], 32, 0, 0, 1, "fadein red");
    assert_palette(ctx, colorwk3[2], 0, 0, 32, 0, "fadein wk3 blue");
    assert_palette(ctx, colorwk3[3], 0, 32, 0, 0, "fadein wk3 green");

    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, fadein0_new());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);

    reset_col8c_state();
    colorwk[16] = (PALETTEENTRY){0, 0, 0, 1};
    colorwk2[16] = (PALETTEENTRY){0, 0, 96, 1};

    fadein_boss8();

    TEST_ASSERT_EQ_INT(ctx, 18, int_flg);
    assert_palette(ctx, colorwk[16], 0, 0, 32, 1, "fadein_boss8 blue");

    memset(boss_dest, 0, sizeof(boss_dest));
    memset(boss_src, 0, sizeof(boss_src));
    boss_src[0] = (PALETTEENTRY){0, 64, 0, 1};
    boss_src[1] = (PALETTEENTRY){96, 0, 0, 1};

    fadein1_boss8(boss_dest, boss_src);

    assert_palette(ctx, boss_dest[0], 0, 32, 0, 0, "fadein1_boss8 green");
    assert_palette(ctx, boss_dest[1], 32, 0, 0, 0, "fadein1_boss8 red");
}

static void test_fadeout_flash_and_direct_helpers(test_context *ctx) {
    PALETTEENTRY entry = {32, 64, 96, 7};
    PALETTEENTRY same = {32, 64, 96, 1};
    PALETTEENTRY white = {224, 224, 224, 3};

    reset_col8c_state();
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

    reset_col8c_state();
    colorwk2[0] = (PALETTEENTRY){128, 96, 64, 1};
    colorwk2[1] = (PALETTEENTRY){224, 128, 224, 1};
    colorwk2[2] = (PALETTEENTRY){128, 224, 224, 1};

    flashin();

    TEST_ASSERT_EQ_INT(ctx, 3, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 224, 224, 192, 1, "flashin blue down");
    assert_palette(ctx, colorwk[1], 224, 192, 224, 1, "flashin green down");
    assert_palette(ctx, colorwk[2], 192, 224, 224, 1, "flashin red down");

    reset_col8c_state();
    colorwk[0] = (PALETTEENTRY){0, 0, 0, 1};
    colorwk[1] = (PALETTEENTRY){224, 0, 0, 1};
    colorwk[2] = (PALETTEENTRY){224, 224, 0, 1};

    flashout();

    TEST_ASSERT_EQ_INT(ctx, 4, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 32, 0, 0, 1, "flashout red");
    assert_palette(ctx, colorwk[1], 224, 32, 0, 1, "flashout green");
    assert_palette(ctx, colorwk[2], 224, 224, 32, 1, "flashout blue");

    fadein2(&entry, &same);
    flashin2(&entry, &same);
    assert_palette(ctx, entry, 32, 64, 96, 7, "matching helpers");

    fadeout2(&entry);
    fadeout2(&entry);
    fadeout2(&entry);
    assert_palette(ctx, entry, 0, 0, 96, 7, "fadeout2 priority");

    flashout2(&white);
    assert_palette(ctx, white, 224, 224, 224, 3, "flashout2 same");
}

static void test_colorset_routes_to_palette_work_buffers(test_context *ctx) {
    reset_col8c_state();

    colorset(3);
    assert_palette(ctx, colorwk2[0], 0, 0, 0, 1, "colorset game");
    assert_palette(ctx, colorwk2[15], 224, 224, 0, 1, "colorset game end");

    colorset2(5);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset2 zone83c");
    assert_palette(ctx, colorwk[63], 0, 0, 96, 1, "colorset2 zone83c end");

    colorset3(6);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset3 zone82c");
    assert_palette(ctx, colorwk[63], 0, 0, 96, 1, "colorset3 zone82c end");

    colorset(7);
    assert_palette(ctx, colorwk3[16], 224, 224, 32, 1, "colorset boss");
    assert_palette(ctx, colorwk3[31], 224, 0, 0, 1, "colorset boss end");

    colorset2(8);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset boss blank");
    assert_palette(ctx, colorwk[31], 224, 0, 0, 1,
                   "colorset boss blank end");

    colorset3(11);
    assert_palette(ctx, colorwk[16], 96, 96, 192, 1, "colorset emie");
    assert_palette(ctx, colorwk[31], 224, 0, 0, 1, "colorset emie end");

    colorset00(0, 3);
    assert_palette(ctx, colorwk4[0], 0, 32, 160, 1, "colorset00 default");
    assert_palette(ctx, colorwk4[63], 224, 192, 0, 1,
                   "colorset00 default end");
}

static void test_fadeproc_unknown_flag_returns_complete(test_context *ctx) {
    reset_col8c_state();
    fade_flag_storage = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());

    reset_col8c_state();
    fade_flag_storage = 99;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
}

static void test_col8c_remaining_palette_edge_paths(test_context *ctx) {
    Uint8 timer = 0;
    Uint8 count = 0;
    Uint8 cycle[] = {10, 1, 7, 0};
    PALETTEENTRY samples[] = {{11, 12, 13, 14}};

    reset_col8c_state();
    clchg_sub0(&timer, &count, cycle, samples);

    TEST_ASSERT_EQ_INT(ctx, 0, count);
    TEST_ASSERT_EQ_INT(ctx, 7, timer);
    assert_palette(ctx, colorwk[10], 11, 12, 13, 14, "clchg wrap");

    reset_col8c_state();
    stageno.b.h = 1;
    startcolor = 4;
    colorcnt = 0;
    colorwk2[2] = (PALETTEENTRY){64, 0, 0, 1};
    colorwk4[2] = (PALETTEENTRY){0, 64, 0, 1};

    TEST_ASSERT_EQ_INT(ctx, 0, fadein0_new());

    assert_palette(ctx, colorwk[2], 32, 0, 0, 0, "fadein stage high");
    assert_palette(ctx, colorwk3[2], 0, 0, 0, 0, "fadein skipped wk3");

    reset_col8c_state();
    fade_flag_storage = 2;
    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);

    reset_col8c_state();
    stageno.b.h = 1;
    fade_flag_storage = 3;
    FadeCount = 21;
    colorwk[0] = (PALETTEENTRY){224, 224, 224, 1};
    colorwk2[0] = (PALETTEENTRY){128, 128, 128, 1};
    colorwk4[0] = (PALETTEENTRY){64, 64, 64, 1};
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);
    assert_palette(ctx, colorwk[0], 224, 224, 192, 1, "flashin complete");
    assert_palette(ctx, colorwk3[0], 0, 0, 0, 0, "flashin skipped wk3");

    reset_col8c_state();
    fade_flag_storage = 4;
    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);

    reset_col8c_state();
    colorset00(0, 1);
    assert_palette(ctx, colorwk2[0], 0, 32, 160, 1, "colorset00 wk2");

    colorset00(0, 2);
    assert_palette(ctx, colorwk3[0], 0, 32, 160, 1, "colorset00 wk3");
}

TEST_MAIN_BEGIN;
test_col8c_tables_capture_palette_contract(&ctx);
test_colchg8c_updates_or_skips_stage_cycle(&ctx);
test_colchg_boss8_waits_advances_and_wraps(&ctx);
test_fadein_and_boss_fade_steps_toward_sources(&ctx);
test_fadeout_flash_and_direct_helpers(&ctx);
test_colorset_routes_to_palette_work_buffers(&ctx);
test_fadeproc_unknown_flag_returns_complete(&ctx);
test_col8c_remaining_palette_edge_paths(&ctx);
TEST_MAIN_END

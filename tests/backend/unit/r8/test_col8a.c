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

#include "src/r8/col8a.c"

static void reset_col8a_state(void) {
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

static void test_col8a_tables_capture_palette_contract(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor) / sizeof(advacolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor2) / sizeof(advacolor2[0])));
    TEST_ASSERT_EQ_INT(ctx, 16,
                       (int)(sizeof(gamecolor) / sizeof(gamecolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone8colora) / sizeof(zone8colora[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone82colora) / sizeof(zone82colora[0])));
    TEST_ASSERT_EQ_INT(ctx, 6, (int)(sizeof(colortbl) / sizeof(colortbl[0])));

    assert_palette(ctx, advacolor[0], 0, 32, 160, 1, "advacolor[0]");
    assert_palette(ctx, advacolor[63], 224, 192, 0, 1, "advacolor[63]");
    assert_palette(ctx, advacolor2[33], 224, 224, 0, 1, "advacolor2[33]");
    assert_palette(ctx, gamecolor[15], 224, 224, 0, 1, "gamecolor[15]");
    assert_palette(ctx, zone8colora[47], 224, 96, 0, 1, "zone8colora[47]");
    assert_palette(ctx, zone82colora[47], 192, 0, 224, 1,
                   "zone82colora[47]");

    TEST_ASSERT_TRUE(ctx, colortbl[0].PeSrc == advacolor);
    TEST_ASSERT_TRUE(ctx, colortbl[2].PeSrc == advacolor2);
    TEST_ASSERT_TRUE(ctx, colortbl[3].PeSrc == gamecolor);
    TEST_ASSERT_TRUE(ctx, colortbl[4].PeSrc == zone8colora);
    TEST_ASSERT_TRUE(ctx, colortbl[5].PeSrc == zone82colora);
    TEST_ASSERT_EQ_INT(ctx, 16, colortbl[4].offset);
    TEST_ASSERT_EQ_INT(ctx, 48, colortbl[5].cnt);

    TEST_ASSERT_EQ_INT(ctx, 38, col8a0cnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 49, col8a3cnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 51, col8a8cnt[0]);
    assert_palette(ctx, col8a0col[1], 128, 0, 128, 1, "col8a0col[1]");
    assert_palette(ctx, col8a6col[4], 0, 0, 0, 1, "col8a6col[4]");
}

static void test_clchg_sub0_waits_updates_and_wraps(test_context *ctx) {
    Uint8 cnt_tbl[] = {4, 2, 7, 1, 9, 0};
    PALETTEENTRY colors[] = {{11, 22, 33, 1}, {44, 55, 66, 1}};
    Uint8 change_time;
    Uint8 change_count;

    reset_col8a_state();
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

static void test_colchg8a_updates_normal_and_stage_one_cycles(
    test_context *ctx) {
    reset_col8a_state();

    clchgctr();

    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[5]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, clchgtim[5]);
    assert_palette(ctx, colorwk[38], 128, 0, 128, 1, "cycle color 38");
    assert_palette(ctx, colorwk[39], 64, 0, 64, 1, "cycle color 39");
    assert_palette(ctx, colorwk[40], 224, 0, 224, 1, "cycle color 40");
    assert_palette(ctx, colorwk[49], 0, 0, 0, 1, "normal cycle color 49");
    assert_palette(ctx, colorwk[50], 0, 0, 0, 1, "normal cycle color 50");
    assert_palette(ctx, colorwk[51], 0, 0, 0, 1, "normal cycle color 51");

    reset_col8a_state();
    stageno.b.l = 1;

    colchg8a();

    TEST_ASSERT_EQ_INT(ctx, 10, clchgtim[3]);
    TEST_ASSERT_EQ_INT(ctx, 10, clchgtim[4]);
    TEST_ASSERT_EQ_INT(ctx, 5, clchgtim[5]);
    assert_palette(ctx, colorwk[49], 0, 160, 0, 1, "stage1 cycle color 49");
    assert_palette(ctx, colorwk[50], 224, 96, 0, 1, "stage1 cycle color 50");
    assert_palette(ctx, colorwk[51], 224, 0, 0, 1, "stage1 cycle color 51");
}

static void test_fadein_initializes_range_and_steps_toward_sources(
    test_context *ctx) {
    reset_col8a_state();
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
    colorwk2[4] = (PALETTEENTRY){0, 0, 0, 1};
    colorwk4[2] = (PALETTEENTRY){0, 0, 64, 1};
    colorwk4[3] = (PALETTEENTRY){0, 64, 0, 1};
    colorwk4[4] = (PALETTEENTRY){64, 0, 0, 1};

    TEST_ASSERT_EQ_INT(ctx, 0, fadein0_new());
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

static void test_fadeout_steps_red_green_blue_and_finishes(test_context *ctx) {
    reset_col8a_state();
    colorwk[0] = (PALETTEENTRY){96, 64, 32, 1};
    colorwk[1] = (PALETTEENTRY){0, 64, 32, 1};
    colorwk[2] = (PALETTEENTRY){0, 0, 32, 1};
    colorwk3[0] = (PALETTEENTRY){32, 0, 0, 1};

    fadeout();

    TEST_ASSERT_EQ_INT(ctx, 0, startcolor);
    TEST_ASSERT_EQ_INT(ctx, 63, colorcnt);
    TEST_ASSERT_EQ_INT(ctx, 2, fade_flag_storage);

    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 64, 64, 32, 1, "fadeout red");
    assert_palette(ctx, colorwk[1], 0, 32, 32, 1, "fadeout green");
    assert_palette(ctx, colorwk[2], 0, 0, 0, 1, "fadeout blue");
    assert_palette(ctx, colorwk3[0], 0, 0, 0, 1, "fadeout wk3 red");

    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);
}

static void test_flashin_and_flashout_capture_white_transitions(
    test_context *ctx) {
    reset_col8a_state();
    colorwk2[0] = (PALETTEENTRY){128, 96, 64, 1};
    colorwk2[1] = (PALETTEENTRY){224, 128, 224, 1};
    colorwk2[2] = (PALETTEENTRY){128, 224, 224, 1};
    colorwk4[0] = (PALETTEENTRY){224, 224, 192, 1};

    flashin();

    assert_palette(ctx, colorwk[0], 224, 224, 224, 1, "flashin white");
    TEST_ASSERT_EQ_INT(ctx, 3, fade_flag_storage);
    colorwk3[0] = (PALETTEENTRY){224, 224, 224, 0};

    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk[0], 224, 224, 192, 1, "flashin blue down");
    assert_palette(ctx, colorwk[1], 224, 192, 224, 1, "flashin green down");
    assert_palette(ctx, colorwk[2], 192, 224, 224, 1, "flashin red down");
    assert_palette(ctx, colorwk3[0], 224, 224, 192, 0, "flashin wk3 blue");

    stageno.b.h = 1;
    colorwk3[0] = (PALETTEENTRY){224, 224, 224, 0};
    colorwk4[0] = (PALETTEENTRY){0, 0, 0, 1};
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());
    assert_palette(ctx, colorwk3[0], 224, 224, 224, 0, "flashin stage skip");

    FadeCount = 21;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);

    reset_col8a_state();
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
    TEST_ASSERT_EQ_INT(ctx, 0, FadeCount);
}

static void test_direct_fade_helpers_leave_matching_palette_entries_alone(
    test_context *ctx) {
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
}

static void test_colorset_routes_to_palette_work_buffers(test_context *ctx) {
    reset_col8a_state();

    colorset(3);
    assert_palette(ctx, colorwk2[0], 0, 0, 0, 1, "colorset work 1");
    assert_palette(ctx, colorwk2[15], 224, 224, 0, 1, "colorset work 1 end");

    colorset2(4);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset2 work 0");
    assert_palette(ctx, colorwk[63], 224, 96, 0, 1, "colorset2 work 0 end");

    colorset3(5);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset3 work 0");
    assert_palette(ctx, colorwk[63], 192, 0, 224, 1, "colorset3 work 0 end");

    colorset00(0, 2);
    assert_palette(ctx, colorwk3[0], 0, 32, 160, 1, "colorset00 work 2");
    assert_palette(ctx, colorwk3[63], 224, 192, 0, 1,
                   "colorset00 work 2 end");

    colorset00(0, 3);
    assert_palette(ctx, colorwk4[0], 0, 32, 160, 1, "colorset00 default");
    assert_palette(ctx, colorwk4[63], 224, 192, 0, 1,
                   "colorset00 default end");
}

static void test_fadeproc_unknown_flag_returns_complete(test_context *ctx) {
    reset_col8a_state();
    fade_flag_storage = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());

    reset_col8a_state();
    fade_flag_storage = 99;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
}

TEST_MAIN_BEGIN;
test_col8a_tables_capture_palette_contract(&ctx);
test_clchg_sub0_waits_updates_and_wraps(&ctx);
test_colchg8a_updates_normal_and_stage_one_cycles(&ctx);
test_fadein_initializes_range_and_steps_toward_sources(&ctx);
test_fadeout_steps_red_green_blue_and_finishes(&ctx);
test_flashin_and_flashout_capture_white_transitions(&ctx);
test_direct_fade_helpers_leave_matching_palette_entries_alone(&ctx);
test_colorset_routes_to_palette_work_buffers(&ctx);
test_fadeproc_unknown_flag_returns_complete(&ctx);
TEST_MAIN_END

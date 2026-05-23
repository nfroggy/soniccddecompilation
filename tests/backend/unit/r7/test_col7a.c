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

#include "src/r7/col7a.c"

static void reset_col7a_state(void) {
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

static void test_col7a_tables_capture_literal_palette_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(advacolor) / sizeof(advacolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 64,
                       (int)(sizeof(gamecolor) / sizeof(gamecolor[0])));
    TEST_ASSERT_EQ_INT(ctx, 48,
                       (int)(sizeof(zone82colora) / sizeof(zone82colora[0])));
    TEST_ASSERT_EQ_INT(ctx, 6, (int)(sizeof(colortbl) / sizeof(colortbl[0])));

    assert_palette(ctx, advacolor[0], 0, 0, 0, 1, "advacolor[0]");
    assert_palette(ctx, advacolor[33], 160, 224, 160, 1, "advacolor[33]");
    assert_palette(ctx, advacolor[63], 0, 224, 0, 1, "advacolor[63]");
    assert_palette(ctx, gamecolor[33], 192, 192, 192, 1, "gamecolor[33]");
    assert_palette(ctx, gamecolor[47], 0, 0, 224, 1, "gamecolor[47]");
    assert_palette(ctx, zone82colora[17], 192, 192, 192, 1,
                   "zone82colora[17]");
    assert_palette(ctx, zone82colora[47], 0, 224, 0, 1,
                   "zone82colora[47]");

    TEST_ASSERT_TRUE(ctx, colortbl[0].PeSrc == advacolor);
    TEST_ASSERT_TRUE(ctx, colortbl[1].PeSrc == advacolor);
    TEST_ASSERT_TRUE(ctx, colortbl[2].PeSrc == gamecolor);
    TEST_ASSERT_TRUE(ctx, colortbl[3].PeSrc == gamecolor);
    TEST_ASSERT_TRUE(ctx, colortbl[4].PeSrc == &gamecolor[16]);
    TEST_ASSERT_TRUE(ctx, colortbl[5].PeSrc == zone82colora);
    TEST_ASSERT_EQ_INT(ctx, 16, colortbl[4].offset);
    TEST_ASSERT_EQ_INT(ctx, 48, colortbl[5].cnt);
}

static void test_clchg_sub0_waits_updates_and_wraps(test_context *ctx) {
    char cnt_tbl[] = {4, 2, 7, 1, 9, 0};
    PALETTEENTRY colors[] = {{11, 22, 33, 1}, {44, 55, 66, 1},
                             {77, 88, 99, 1}, {111, 122, 133, 1}};
    Uint8 change_time_storage[2] = {3, 99};
    Uint8 change_count_storage[2] = {0, 99};
    Uint8 *change_time = change_time_storage;
    Uint8 *change_count = change_count_storage;

    reset_col7a_state();
    colorwk[4] = (PALETTEENTRY){1, 2, 3, 4};

    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);

    TEST_ASSERT_EQ_INT(ctx, 2, change_time_storage[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, change_count_storage[0]);
    TEST_ASSERT_TRUE(ctx, change_time == &change_time_storage[1]);
    TEST_ASSERT_TRUE(ctx, change_count == &change_count_storage[1]);
    assert_palette(ctx, colorwk[4], 1, 2, 3, 4, "waiting color");

    change_time = change_time_storage;
    change_count = change_count_storage;
    change_time_storage[0] = 0;
    change_count_storage[0] = 0;

    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);

    TEST_ASSERT_EQ_INT(ctx, 9, change_time_storage[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, change_count_storage[0]);
    assert_palette(ctx, colorwk[4], 11, 22, 33, 1, "advanced colorwk");
    assert_palette(ctx, colorwk3[4], 77, 88, 99, 1, "advanced colorwk3");

    change_time = change_time_storage;
    change_count = change_count_storage;
    change_time_storage[0] = 0;
    change_count_storage[0] = 1;

    clchg_sub0(&change_time, &change_count, cnt_tbl, colors);

    TEST_ASSERT_EQ_INT(ctx, 7, change_time_storage[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, change_count_storage[0]);
    assert_palette(ctx, colorwk[4], 44, 55, 66, 1, "wrapped colorwk");
    assert_palette(ctx, colorwk3[4], 111, 122, 133, 1, "wrapped colorwk3");
}

static void test_colchg7a_updates_three_cycle_slots(test_context *ctx) {
    reset_col7a_state();

    clchgctr();

    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgcnt[2]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, clchgtim[2]);
    assert_palette(ctx, colorwk[41], 0, 0, 0, 1, "cycle color 41");
    assert_palette(ctx, colorwk[62], 32, 0, 0, 1, "cycle color 62");
    assert_palette(ctx, colorwk[63], 32, 0, 0, 1, "cycle color 63");
    assert_palette(ctx, colorwk3[41], 0, 0, 0, 1, "cycle color3 41");
    assert_palette(ctx, colorwk3[62], 32, 0, 0, 1, "cycle color3 62");
    assert_palette(ctx, colorwk3[63], 32, 0, 0, 1, "cycle color3 63");
}

static void test_fadein_initializes_range_and_steps_toward_sources(
    test_context *ctx) {
    reset_col7a_state();
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
    reset_col7a_state();
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
    reset_col7a_state();
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

    reset_col7a_state();
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

static void test_colorset_routes_to_all_palette_work_buffers(test_context *ctx) {
    reset_col7a_state();

    colorset(3);
    assert_palette(ctx, colorwk2[0], 0, 0, 0, 1, "colorset work 1");
    assert_palette(ctx, colorwk2[15], 224, 224, 0, 1, "colorset work 1 end");

    colorset2(4);
    assert_palette(ctx, colorwk[16], 0, 0, 0, 1, "colorset2 work 0");
    assert_palette(ctx, colorwk[63], 0, 224, 0, 1, "colorset2 work 0 end");

    colorset3(5);
    assert_palette(ctx, colorwk2[16], 0, 0, 0, 1, "colorset3 work 1");
    assert_palette(ctx, colorwk2[63], 0, 224, 0, 1, "colorset3 work 1 end");

    colorset4(5);
    assert_palette(ctx, colorwk3[16], 0, 0, 0, 1, "colorset4 work 2");
    assert_palette(ctx, colorwk3[63], 0, 224, 0, 1, "colorset4 work 2 end");

    colorset00(0, 3);
    assert_palette(ctx, colorwk4[0], 0, 0, 0, 1, "colorset00 default");
    assert_palette(ctx, colorwk4[63], 0, 224, 0, 1, "colorset00 default end");
}

static void test_fadeproc_unknown_flag_returns_complete(test_context *ctx) {
    reset_col7a_state();
    fade_flag_storage = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, FadeProc());

    reset_col7a_state();
    fade_flag_storage = 99;
    TEST_ASSERT_EQ_INT(ctx, 1, FadeProc());
}

TEST_MAIN_BEGIN;
test_col7a_tables_capture_literal_palette_data(&ctx);
test_clchg_sub0_waits_updates_and_wraps(&ctx);
test_colchg7a_updates_three_cycle_slots(&ctx);
test_fadein_initializes_range_and_steps_toward_sources(&ctx);
test_fadeout_steps_red_green_blue_and_finishes(&ctx);
test_flashin_and_flashout_capture_white_transitions(&ctx);
test_direct_fade_helpers_leave_matching_palette_entries_alone(&ctx);
test_colorset_routes_to_all_palette_work_buffers(&ctx);
test_fadeproc_unknown_flag_returns_complete(&ctx);
TEST_MAIN_END

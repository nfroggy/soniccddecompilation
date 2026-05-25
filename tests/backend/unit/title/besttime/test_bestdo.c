#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

typedef struct {
    Sint32 y;
    const char *str;
} menu_string_call;

Uint16 nSequenceNum;
Sint32 MENUITEMMAX;
Uint16 swData1;
Sint32 nTimerCunt;
void (*CDPause)(Sint16);
void (*CDPlay)(Sint16);

static Sint32 draw_call_count;
static Sint32 grid_create_call_count;
static Uint16 grid_create_index;
static Sint32 spr_create_call_count;
static Sint32 spr_create_indices[32];
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[8];
static Sint32 key_wait_call_count;
static Sint32 cd_play_call_count;
static Sint16 cd_play_values[4];
static Sint32 cd_pause_call_count;
static Sint16 cd_pause_values[4];
static char menu_strings[12][2];
static menu_string_call strmenu_calls[32];
static menu_string_call strmenu2_calls[32];
static Sint32 strmenu_call_count;
static Sint32 strmenu2_call_count;

#include "src/title/besttime/bestdo.c"

bestitem menuTbl[12];

void srfDraw(void) { ++draw_call_count; }

Sint32 OEGridCreate(Uint16 indx) {
    grid_create_index = indx;
    ++grid_create_call_count;
    return 0;
}

Sint32 OESprCreateIndx(Sint32 indx) {
    spr_create_indices[spr_create_call_count] = indx;
    ++spr_create_call_count;
    return 0;
}

void OESetSequenceNum(Uint16 num) {
    set_sequence_values[set_sequence_call_count] = num;
    ++set_sequence_call_count;
    nSequenceNum = num;
}

void KeyWait(void) { ++key_wait_call_count; }

void StrMenu(Sint32 y, char *lpStr) {
    strmenu_calls[strmenu_call_count].y = y;
    strmenu_calls[strmenu_call_count].str = lpStr;
    ++strmenu_call_count;
}

void StrMenu2(Sint32 y, char *lpStr) {
    strmenu2_calls[strmenu2_call_count].y = y;
    strmenu2_calls[strmenu2_call_count].str = lpStr;
    ++strmenu2_call_count;
}

static void fake_cd_play(Sint16 value) {
    cd_play_values[cd_play_call_count] = value;
    ++cd_play_call_count;
}

static void fake_cd_pause(Sint16 value) {
    cd_pause_values[cd_pause_call_count] = value;
    ++cd_pause_call_count;
}

static void reset_fixture(void) {
    Sint32 i;

    memset(menuTbl, 0, sizeof(menuTbl));
    memset(spr_create_indices, 0, sizeof(spr_create_indices));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));
    memset(cd_play_values, 0, sizeof(cd_play_values));
    memset(cd_pause_values, 0, sizeof(cd_pause_values));
    memset(menu_strings, 0, sizeof(menu_strings));
    memset(strmenu_calls, 0, sizeof(strmenu_calls));
    memset(strmenu2_calls, 0, sizeof(strmenu2_calls));

    for (i = 0; i < 12; ++i) {
        menuTbl[i].color = (Uint16)(i & 1);
        menu_strings[i][0] = (char)('A' + i);
        menuTbl[i].str = menu_strings[i];
    }

    nSequenceNum = 0;
    MENUITEMMAX = 12;
    swData1 = 0;
    nTimerCunt = 1;
    ScrollCount = 0;
    nMenuIndx = 0;
    CDPlay = fake_cd_play;
    CDPause = fake_cd_pause;
    draw_call_count = 0;
    grid_create_call_count = 0;
    grid_create_index = 999;
    spr_create_call_count = 0;
    set_sequence_call_count = 0;
    key_wait_call_count = 0;
    cd_play_call_count = 0;
    cd_pause_call_count = 0;
    strmenu_call_count = 0;
    strmenu2_call_count = 0;
}

static void test_stgdraw_only_draws_after_sequence_one(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    STGDraw();
    TEST_ASSERT_EQ_INT(ctx, 0, draw_call_count);

    nSequenceNum = 2;
    STGDraw();
    TEST_ASSERT_EQ_INT(ctx, 1, draw_call_count);
}

static void test_stgmenumaxcheck_wraps_at_menu_item_max(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 11, STGMenuMaxCheck(11));
    TEST_ASSERT_EQ_INT(ctx, 0, STGMenuMaxCheck(12));
    TEST_ASSERT_EQ_INT(ctx, 3, STGMenuMaxCheck(15));
}

static void test_stgstart_creates_grid_sprites_menu_and_sequence(
    test_context *ctx) {
    reset_fixture();
    ScrollCount = -2;
    nMenuIndx = 5;

    STGStart();

    TEST_ASSERT_EQ_INT(ctx, 1, cd_play_call_count);
    TEST_ASSERT_EQ_INT(ctx, 29, cd_play_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_create_index);
    TEST_ASSERT_EQ_INT(ctx, 27, spr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 73, spr_create_indices[0]);
    TEST_ASSERT_EQ_INT(ctx, 88, spr_create_indices[15]);
    TEST_ASSERT_EQ_INT(ctx, 92, spr_create_indices[16]);
    TEST_ASSERT_EQ_INT(ctx, 99, spr_create_indices[23]);
    TEST_ASSERT_EQ_INT(ctx, 89, spr_create_indices[24]);
    TEST_ASSERT_EQ_INT(ctx, 90, spr_create_indices[25]);
    TEST_ASSERT_EQ_INT(ctx, 91, spr_create_indices[26]);
    TEST_ASSERT_EQ_INT(ctx, 0, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 4, strmenu_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, strmenu2_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, strmenu2_calls[0].y);
    TEST_ASSERT_TRUE(ctx, strmenu2_calls[0].str == menuTbl[0].str);
    TEST_ASSERT_EQ_INT(ctx, 1, strmenu_calls[0].y);
    TEST_ASSERT_TRUE(ctx, strmenu_calls[0].str == menuTbl[1].str);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
}

static void test_stgmenu_up_ignored_at_top_without_scroll(test_context *ctx) {
    reset_fixture();
    swData1 = 256;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 0, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 0, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, strmenu_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, strmenu2_call_count);
}

static void test_stgmenu_up_scrolls_toward_zero_and_updates(test_context *ctx) {
    reset_fixture();
    swData1 = 256;
    ScrollCount = -1;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 0, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 4, strmenu_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, strmenu2_call_count);
}

static void test_stgmenu_up_wraps_scroll_and_decrements_menu_index(
    test_context *ctx) {
    reset_fixture();
    swData1 = 256;
    nMenuIndx = 2;
    ScrollCount = 2;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu_calls[0].str == menuTbl[1].str);
}

static void test_stgmenu_down_scrolls_and_wraps_index(test_context *ctx) {
    reset_fixture();
    swData1 = 512;
    nMenuIndx = 0;
    ScrollCount = -2;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu2_calls[0].str == menuTbl[2].str);
}

static void test_stgmenu_down_boundary_allows_partial_scroll(
    test_context *ctx) {
    reset_fixture();
    swData1 = 512;
    nMenuIndx = MENUITEMMAX - 10;
    ScrollCount = -1;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, MENUITEMMAX - 10, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, -2, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
}

static void test_stgmenu_down_ignored_past_boundary(test_context *ctx) {
    reset_fixture();
    swData1 = 512;
    nMenuIndx = MENUITEMMAX - 10;
    ScrollCount = -2;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, MENUITEMMAX - 10, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, -2, ScrollCount);
    TEST_ASSERT_EQ_INT(ctx, 0, key_wait_call_count);
}

static void test_stgmenu_exit_sets_sequence_even_after_scroll(test_context *ctx) {
    reset_fixture();
    swData1 = 512 | 4096;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 998, set_sequence_values[0]);
}

static void test_stgend2_waits_until_timer_zero(test_context *ctx) {
    reset_fixture();

    nTimerCunt = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, STGEnd2());
    TEST_ASSERT_EQ_INT(ctx, 0, cd_pause_call_count);

    nTimerCunt = 0;
    TEST_ASSERT_EQ_INT(ctx, 9, STGEnd2());
    TEST_ASSERT_EQ_INT(ctx, 1, cd_pause_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, cd_pause_values[0]);
}

TEST_MAIN_BEGIN;
test_stgdraw_only_draws_after_sequence_one(&ctx);
test_stgmenumaxcheck_wraps_at_menu_item_max(&ctx);
test_stgstart_creates_grid_sprites_menu_and_sequence(&ctx);
test_stgmenu_up_ignored_at_top_without_scroll(&ctx);
test_stgmenu_up_scrolls_toward_zero_and_updates(&ctx);
test_stgmenu_up_wraps_scroll_and_decrements_menu_index(&ctx);
test_stgmenu_down_scrolls_and_wraps_index(&ctx);
test_stgmenu_down_boundary_allows_partial_scroll(&ctx);
test_stgmenu_down_ignored_past_boundary(&ctx);
test_stgmenu_exit_sets_sequence_even_after_scroll(&ctx);
test_stgend2_waits_until_timer_zero(&ctx);
TEST_MAIN_END

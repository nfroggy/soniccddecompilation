#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint16 nSequenceNum;
Uint16 swData1;
Sint32 MENUITEMMAX;
Uint16 *lpSelectedStage;
Sint32 nTimerCunt;

static Sint32 draw_call_count;
static Sint32 grid_create_call_count;
static Uint16 grid_create_index;
static Sint32 spr_create_call_count;
static Sint32 spr_create_indices[32];
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[8];
static Sint32 key_wait_call_count;
static Sint32 strmenu_call_count;
static char *strmenu_values[16];
static Uint16 selected_stage;

#include "src/title/stagetst/stgdo.c"

stgitem menuTbl[4];

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
    nTimerCunt = -1;
}

void KeyWait(void) { ++key_wait_call_count; }

void StrMenu(char *lpStr) {
    strmenu_values[strmenu_call_count] = lpStr;
    ++strmenu_call_count;
}

static void reset_fixture(void) {
    Sint32 i;
    static char names[4][4] = {"AAA", "BBB", "CCC", "DDD"};

    memset(spr_create_indices, 0, sizeof(spr_create_indices));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));
    memset(strmenu_values, 0, sizeof(strmenu_values));

    for (i = 0; i < 4; ++i) {
        menuTbl[i].retcode = 400 + i;
        menuTbl[i].str = names[i];
    }

    nSequenceNum = 0;
    nMenuIndx = 0;
    swData1 = 0;
    MENUITEMMAX = 4;
    selected_stage = 999;
    lpSelectedStage = &selected_stage;
    nTimerCunt = 1;
    draw_call_count = 0;
    grid_create_call_count = 0;
    grid_create_index = 999;
    spr_create_call_count = 0;
    set_sequence_call_count = 0;
    key_wait_call_count = 0;
    strmenu_call_count = 0;
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

static void test_stgstart_creates_grid_sprites_menu_and_sequence(
    test_context *ctx) {
    Sint32 i;

    reset_fixture();
    nMenuIndx = 3;

    STGStart();

    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_create_index);
    TEST_ASSERT_EQ_INT(ctx, 27, spr_create_call_count);
    for (i = 0; i < 16; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 48 + i, spr_create_indices[i]);
    }
    for (i = 0; i < 11; ++i) {
        TEST_ASSERT_EQ_INT(ctx, i, spr_create_indices[16 + i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 0, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 1, strmenu_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu_values[0] == menuTbl[0].str);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
}

static void test_stgmenu_up_wraps_to_last_entry(test_context *ctx) {
    reset_fixture();
    swData1 = 256;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 3, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu_values[0] == menuTbl[3].str);
}

static void test_stgmenu_up_decrements_nonzero_entry(test_context *ctx) {
    reset_fixture();
    nMenuIndx = 2;
    swData1 = 256;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu_values[0] == menuTbl[1].str);
}

static void test_stgmenu_down_wraps_and_increments(test_context *ctx) {
    reset_fixture();
    nMenuIndx = 3;
    swData1 = 512;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 0, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_TRUE(ctx, strmenu_values[0] == menuTbl[0].str);

    reset_fixture();
    nMenuIndx = 1;
    swData1 = 512;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 2, nMenuIndx);
    TEST_ASSERT_TRUE(ctx, strmenu_values[0] == menuTbl[2].str);
}

static void test_stgmenu_select_and_cancel_sequences(test_context *ctx) {
    reset_fixture();

    swData1 = 16384;
    STGMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 999, set_sequence_values[0]);

    reset_fixture();
    swData1 = 4096;
    STGMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 998, set_sequence_values[0]);

    reset_fixture();
    swData1 = 16384 | 4096;
    STGMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 999, set_sequence_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 998, set_sequence_values[1]);
}

static void test_stgmenu_no_input_does_nothing(test_context *ctx) {
    reset_fixture();
    nMenuIndx = 2;

    STGMenu();

    TEST_ASSERT_EQ_INT(ctx, 2, nMenuIndx);
    TEST_ASSERT_EQ_INT(ctx, 0, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, strmenu_call_count);
}

static void test_stgend_waits_for_timer_then_sets_selected_stage(
    test_context *ctx) {
    reset_fixture();
    nMenuIndx = 2;
    nTimerCunt = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, STGEnd());
    TEST_ASSERT_EQ_INT(ctx, 999, selected_stage);

    nTimerCunt = 0;

    TEST_ASSERT_EQ_INT(ctx, 15, STGEnd());
    TEST_ASSERT_EQ_INT(ctx, 402, selected_stage);
}

static void test_stgend2_waits_for_timer(test_context *ctx) {
    reset_fixture();

    nTimerCunt = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, STGEnd2());

    nTimerCunt = 0;
    TEST_ASSERT_EQ_INT(ctx, 9, STGEnd2());
}

TEST_MAIN_BEGIN;
test_stgdraw_only_draws_after_sequence_one(&ctx);
test_stgstart_creates_grid_sprites_menu_and_sequence(&ctx);
test_stgmenu_up_wraps_to_last_entry(&ctx);
test_stgmenu_up_decrements_nonzero_entry(&ctx);
test_stgmenu_down_wraps_and_increments(&ctx);
test_stgmenu_select_and_cancel_sequences(&ctx);
test_stgmenu_no_input_does_nothing(&ctx);
test_stgend_waits_for_timer_then_sets_selected_stage(&ctx);
test_stgend2_waits_for_timer(&ctx);
TEST_MAIN_END

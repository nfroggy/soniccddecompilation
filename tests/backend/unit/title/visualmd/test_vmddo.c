#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

typedef struct {
    Sint32 kind;
} string_call;

Uint16 nSequenceNum;
Uint16 swData1;
Uint32 *lpbVisualmode;
Sint32 nTimerCunt;

static Sint32 draw_call_count;
static Sint32 grid_create_call_count;
static Uint16 grid_create_index;
static Sint32 spr_create_call_count;
static Sint32 spr_create_indices[16];
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[8];
static Sint32 key_wait_call_count;
static string_call exit_calls[16];
static string_call opening_calls[16];
static string_call good_calls[16];
static string_call bad_calls[16];
static string_call pencil_calls[16];
static Sint32 exit_call_count;
static Sint32 opening_call_count;
static Sint32 good_call_count;
static Sint32 bad_call_count;
static Sint32 pencil_call_count;
static Uint32 visual_mode;

#include "src/title/visualmd/vmddo.c"

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

void StrEXIT(Sint32 kind) {
    exit_calls[exit_call_count].kind = kind;
    ++exit_call_count;
}

void StrOPENING(Sint32 kind) {
    opening_calls[opening_call_count].kind = kind;
    ++opening_call_count;
}

void StrGOODENDING(Sint32 kind) {
    good_calls[good_call_count].kind = kind;
    ++good_call_count;
}

void StrBADENDING(Sint32 kind) {
    bad_calls[bad_call_count].kind = kind;
    ++bad_call_count;
}

void StrPENCILTEST(Sint32 kind) {
    pencil_calls[pencil_call_count].kind = kind;
    ++pencil_call_count;
}

static void reset_fixture(void) {
    memset(spr_create_indices, 0, sizeof(spr_create_indices));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));
    memset(exit_calls, 0, sizeof(exit_calls));
    memset(opening_calls, 0, sizeof(opening_calls));
    memset(good_calls, 0, sizeof(good_calls));
    memset(bad_calls, 0, sizeof(bad_calls));
    memset(pencil_calls, 0, sizeof(pencil_calls));

    nSequenceNum = 0;
    nMenuKind = 1;
    swData1 = 0;
    nTimerCunt = 1;
    visual_mode = 123;
    lpbVisualmode = &visual_mode;
    draw_call_count = 0;
    grid_create_call_count = 0;
    grid_create_index = 999;
    spr_create_call_count = 0;
    set_sequence_call_count = 0;
    key_wait_call_count = 0;
    exit_call_count = 0;
    opening_call_count = 0;
    good_call_count = 0;
    bad_call_count = 0;
    pencil_call_count = 0;
}

static void assert_latest_menu_kinds(test_context *ctx, Sint32 exit_kind,
                                     Sint32 opening_kind, Sint32 good_kind,
                                     Sint32 bad_kind, Sint32 pencil_kind) {
    TEST_ASSERT_EQ_INT(ctx, exit_kind, exit_calls[exit_call_count - 1].kind);
    TEST_ASSERT_EQ_INT(ctx, opening_kind,
                       opening_calls[opening_call_count - 1].kind);
    TEST_ASSERT_EQ_INT(ctx, good_kind, good_calls[good_call_count - 1].kind);
    TEST_ASSERT_EQ_INT(ctx, bad_kind, bad_calls[bad_call_count - 1].kind);
    TEST_ASSERT_EQ_INT(ctx, pencil_kind,
                       pencil_calls[pencil_call_count - 1].kind);
}

static void test_vmddraw_only_draws_after_sequence_one(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    VMDDraw();
    TEST_ASSERT_EQ_INT(ctx, 0, draw_call_count);

    nSequenceNum = 2;
    VMDDraw();
    TEST_ASSERT_EQ_INT(ctx, 1, draw_call_count);
}

static void test_vmdstart_creates_grid_sprites_menu_and_sequence(
    test_context *ctx) {
    Sint32 i;

    reset_fixture();
    nMenuKind = 4;

    VMDStart();

    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_create_index);
    TEST_ASSERT_EQ_INT(ctx, 16, spr_create_call_count);
    for (i = 0; i < 16; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 52 + i, spr_create_indices[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, exit_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, opening_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, good_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, bad_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, pencil_call_count);
    assert_latest_menu_kinds(ctx, 1, 0, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
}

static void test_vmdmenu_down_moves_until_five_then_stops(test_context *ctx) {
    reset_fixture();
    nMenuKind = 4;
    swData1 = 512;

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 5, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    assert_latest_menu_kinds(ctx, 0, 0, 0, 0, 1);

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 5, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, exit_call_count);
}

static void test_vmdmenu_down_middle_selection_uses_increment_branch(
    test_context *ctx) {
    reset_fixture();
    nMenuKind = 3;
    swData1 = 512;

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 4, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    assert_latest_menu_kinds(ctx, 0, 0, 0, 1, 0);
}

static void test_vmdmenu_up_moves_until_one_then_stops(test_context *ctx) {
    reset_fixture();
    nMenuKind = 2;
    swData1 = 256;

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    assert_latest_menu_kinds(ctx, 1, 0, 0, 0, 0);

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, exit_call_count);
}

static void test_vmdmenu_up_middle_selection_uses_decrement_branch(
    test_context *ctx) {
    reset_fixture();
    nMenuKind = 3;
    swData1 = 256;

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
    assert_latest_menu_kinds(ctx, 0, 1, 0, 0, 0);
}

static void test_updatemenu_marks_good_ending_selected(test_context *ctx) {
    reset_fixture();
    nMenuKind = 3;

    UpdateMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, exit_call_count);
    assert_latest_menu_kinds(ctx, 0, 0, 1, 0, 0);
}

static void test_vmdmenu_exit_buttons_set_sequence(test_context *ctx) {
    reset_fixture();

    swData1 = 4096;
    VMDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 999, set_sequence_values[0]);

    swData1 = 16384;
    VMDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 999, set_sequence_values[1]);
}

static void test_vmdmenu_no_input_does_nothing(test_context *ctx) {
    reset_fixture();
    nMenuKind = 3;

    VMDMenu();

    TEST_ASSERT_EQ_INT(ctx, 3, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 0, key_wait_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, exit_call_count);
}

static void test_vmdend_waits_for_timer_zero(test_context *ctx) {
    reset_fixture();
    nMenuKind = 2;
    nTimerCunt = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, VMDEnd());
}

static void test_vmdend_returns_selection_codes(test_context *ctx) {
    Sint32 expected[6] = {0, 9, 8, 10, 11, 12};
    Sint32 menu;

    reset_fixture();
    nTimerCunt = 0;

    for (menu = 1; menu <= 5; ++menu) {
        nMenuKind = (Uint16)menu;
        visual_mode = 123;
        TEST_ASSERT_EQ_INT(ctx, expected[menu], VMDEnd());
        if (menu == 1) {
            TEST_ASSERT_EQ_INT(ctx, 0, visual_mode);
        } else {
            TEST_ASSERT_EQ_INT(ctx, 123, visual_mode);
        }
    }
}

static void test_vmdend_unknown_selection_returns_zero(test_context *ctx) {
    reset_fixture();
    nTimerCunt = 0;
    nMenuKind = 6;

    TEST_ASSERT_EQ_INT(ctx, 0, VMDEnd());
    TEST_ASSERT_EQ_INT(ctx, 123, visual_mode);
}

TEST_MAIN_BEGIN;
test_vmddraw_only_draws_after_sequence_one(&ctx);
test_vmdstart_creates_grid_sprites_menu_and_sequence(&ctx);
test_vmdmenu_down_moves_until_five_then_stops(&ctx);
test_vmdmenu_down_middle_selection_uses_increment_branch(&ctx);
test_vmdmenu_up_moves_until_one_then_stops(&ctx);
test_vmdmenu_up_middle_selection_uses_decrement_branch(&ctx);
test_updatemenu_marks_good_ending_selected(&ctx);
test_vmdmenu_exit_buttons_set_sequence(&ctx);
test_vmdmenu_no_input_does_nothing(&ctx);
test_vmdend_waits_for_timer_zero(&ctx);
test_vmdend_returns_selection_codes(&ctx);
test_vmdend_unknown_selection_returns_zero(&ctx);
TEST_MAIN_END

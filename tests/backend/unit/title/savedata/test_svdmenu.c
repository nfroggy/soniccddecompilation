#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/title/common/score_data_types.h"
#include "src/types.h"

sprite_bmp infoSprtBmp[256];
Uint16 nMenuKind;
Uint32 bMenuChosed;
Uint16 swData1;
char editBuf[12];
Sint32 nTimerCunt;
void (*sMemCpy)(void *, void *, Sint32);
void (*sMemSet)(void *, Uint8, Sint32);
void (*WaveRequest)(Sint16);

static Uint32 sprite_handles[256];
static Sint32 wave_call_count;
static Sint16 wave_values[128];
static Sint32 update_file_select_call_count;
static Sint32 update_file_select_values[64];
static Sint32 run_sonic_call_count;
static POINT run_sonic_points[64];
static Sint32 key_wait_call_count;
static Sint32 delete_file_select_call_count;
static Sint32 default_message_call_count;
static Sint32 update_message_call_count;
static char update_message_values[64][128];
static Sint32 wait_key_off_call_count;
static Sint32 update_select_mark_call_count;
static Uint16 update_select_mark_values[64];
static Sint32 update_file_view_call_count;
static Uint16 update_file_view_values[64];
static Sint32 wait_default_call_count;
static Sint32 wait_default_values[64];
static Sint32 disp_cursor_call_count;
static Sint32 mov_cursor_call_count;
static Sint32 mov_cursor_values[64];
static Sint32 hide_cursor_call_count;
static Sint32 edit_call_count;
static Sint32 edit_return;
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[16];

#include "src/title/savedata/svdmenu.c"

static void fake_wave_request(Sint16 wave) {
    wave_values[wave_call_count] = wave;
    ++wave_call_count;
}

static void fake_mem_cpy(void *dst, void *src, Sint32 bytes) {
    memcpy(dst, src, (size_t)bytes);
}

static void fake_mem_set(void *dst, Uint8 value, Sint32 bytes) {
    memset(dst, value, (size_t)bytes);
}

void UpdateFileSelect(Uint16 nNewNumber) {
    update_file_select_values[update_file_select_call_count] = nNewNumber;
    ++update_file_select_call_count;
}

void RunSonicCursol(POINT point) {
    run_sonic_points[run_sonic_call_count] = point;
    ++run_sonic_call_count;
}

void KeyWait(void) { ++key_wait_call_count; }

void DeleteFileSelect(void) { ++delete_file_select_call_count; }

void DefaultMessage(void) { ++default_message_call_count; }

void UpdateMessage(char *msg) {
    strcpy(update_message_values[update_message_call_count], msg);
    ++update_message_call_count;
}

void WaitKeyOff(void) { ++wait_key_off_call_count; }

void UpdateSelectSonicMark(Uint16 nNewNumber) {
    update_select_mark_values[update_select_mark_call_count] = nNewNumber;
    ++update_select_mark_call_count;
}

void UpdateFileView(Uint16 nFileNumber) {
    update_file_view_values[update_file_view_call_count] = nFileNumber;
    ++update_file_view_call_count;
}

void WaitAndDfltMsg(Sint32 time) {
    wait_default_values[wait_default_call_count] = time;
    ++wait_default_call_count;
}

void DispCursol(void) { ++disp_cursor_call_count; }

void MovCursol(Sint32 x) {
    mov_cursor_values[mov_cursor_call_count] = x;
    ++mov_cursor_call_count;
}

void HideCursol(void) { ++hide_cursor_call_count; }

Sint32 Edit(void) {
    ++edit_call_count;
    return edit_return;
}

void OESetSequenceNum(Uint16 num) {
    set_sequence_values[set_sequence_call_count] = num;
    ++set_sequence_call_count;
}

static void reset_fixture(void) {
    Sint32 i;

    memset(infoSprtBmp, 0, sizeof(infoSprtBmp));
    memset(sprite_handles, 0, sizeof(sprite_handles));
    memset(saveData, 0, sizeof(saveData));
    memset(editBuf, 0, sizeof(editBuf));
    memset(wave_values, 0, sizeof(wave_values));
    memset(update_file_select_values, 0, sizeof(update_file_select_values));
    memset(run_sonic_points, 0, sizeof(run_sonic_points));
    memset(update_message_values, 0, sizeof(update_message_values));
    memset(update_select_mark_values, 0, sizeof(update_select_mark_values));
    memset(update_file_view_values, 0, sizeof(update_file_view_values));
    memset(wait_default_values, 0, sizeof(wait_default_values));
    memset(mov_cursor_values, 0, sizeof(mov_cursor_values));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));

    for (i = 0; i < 256; ++i) {
        sprite_handles[i] = (Uint32)i;
        infoSprtBmp[i].lphSpr = &sprite_handles[i];
        infoSprtBmp[i].point.x = i * 3;
        infoSprtBmp[i].point.y = i * 5;
        infoSprtBmp[i].w = 10 + i;
    }

    nMenuKind = 1;
    bMenuChosed = 1;
    swData1 = 0;
    bFileChosed = 0;
    bDestFileChosed = 0;
    nFileNumber = 2;
    nSelectFileNumber = 0;
    nSrcFileNumber = 0;
    nTimerCunt = 0;
    WaveRequest = fake_wave_request;
    sMemCpy = fake_mem_cpy;
    sMemSet = fake_mem_set;
    wave_call_count = 0;
    update_file_select_call_count = 0;
    run_sonic_call_count = 0;
    key_wait_call_count = 0;
    delete_file_select_call_count = 0;
    default_message_call_count = 0;
    update_message_call_count = 0;
    wait_key_off_call_count = 0;
    update_select_mark_call_count = 0;
    update_file_view_call_count = 0;
    wait_default_call_count = 0;
    disp_cursor_call_count = 0;
    mov_cursor_call_count = 0;
    hide_cursor_call_count = 0;
    edit_call_count = 0;
    edit_return = 0;
    set_sequence_call_count = 0;
}

static void assert_point(test_context *ctx, POINT point, Sint32 x, Sint32 y) {
    TEST_ASSERT_EQ_INT(ctx, x, point.x);
    TEST_ASSERT_EQ_INT(ctx, y, point.y);
}

static void test_file_select_moves_selects_cancels_and_clamps(
    test_context *ctx) {
    Sint32 file = 2;

    reset_fixture();
    swData1 = 512;
    TEST_ASSERT_EQ_INT(ctx, 0, SDFileSelect(&file));
    TEST_ASSERT_EQ_INT(ctx, 3, file);
    TEST_ASSERT_EQ_INT(ctx, 9, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, update_file_select_values[0]);
    assert_point(ctx, run_sonic_points[0], infoSprtBmp[196].point.x - 16,
                 infoSprtBmp[196].point.y);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);

    file = 5;
    SDFileSelect(&file);
    TEST_ASSERT_EQ_INT(ctx, 5, file);
    TEST_ASSERT_EQ_INT(ctx, 1, update_file_select_call_count);

    swData1 = 256;
    file = 2;
    SDFileSelect(&file);
    TEST_ASSERT_EQ_INT(ctx, 1, file);
    TEST_ASSERT_EQ_INT(ctx, 1, update_file_select_values[1]);

    file = 0;
    SDFileSelect(&file);
    TEST_ASSERT_EQ_INT(ctx, 0, file);
    TEST_ASSERT_EQ_INT(ctx, 2, update_file_select_call_count);

    swData1 = 16384;
    TEST_ASSERT_EQ_INT(ctx, 1, SDFileSelect(&file));

    swData1 = 4096;
    nMenuKind = 3;
    bMenuChosed = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, SDFileSelect(&file));
    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 25, wave_values[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_file_select_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, default_message_call_count);
    assert_point(ctx, run_sonic_points[2],
                 infoSprtBmp[4].point.x + infoSprtBmp[4].w,
                 infoSprtBmp[4].point.y - 8);
}

static void test_menu_select_empty_saved_confirm_and_cancel(test_context *ctx) {
    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 0;

    SDMenuSelect();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuSelect();
    TEST_ASSERT_EQ_INT(ctx, 1, bFileChosed);
    TEST_ASSERT_TRUE(ctx,
                     strcmp(update_message_values[0],
                            "CONTINUE WITH THIS FILE?") == 0);
    TEST_ASSERT_EQ_INT(ctx, 26, wave_values[0]);

    SDMenuSelect();
    TEST_ASSERT_EQ_INT(ctx, 1, nSelectFileNumber);
    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 5, wave_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, update_select_mark_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_file_select_call_count);
    assert_point(ctx, run_sonic_points[0], infoSprtBmp[0].point.x +
                                                infoSprtBmp[0].w,
                 infoSprtBmp[0].point.y - 8);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuSelect();
    swData1 = 4096;
    SDMenuSelect();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 25, wave_values[1]);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[1],
                                 "SELECT FILE TO USE IN GAME.") == 0);
}

static void test_menu_copy_source_dest_confirm_and_cancel(test_context *ctx) {
    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 0;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    strcpy(saveData[1].player, "SRC");
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 1, bFileChosed);
    TEST_ASSERT_TRUE(ctx,
                     strcmp(update_message_values[0],
                            "SELECT FILE TO COPY TO.") == 0);

    nFileNumber = 1;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bDestFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[1]);

    nFileNumber = 2;
    saveData[2].saved = 0;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 1, bDestFileChosed);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[1], "OK TO COPY?") ==
                              0);

    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, saveData[1].saved, saveData[2].saved);
    TEST_ASSERT_TRUE(ctx, strcmp(saveData[2].player, "SRC") == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, update_file_view_values[0]);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[2], "COPY COMPLETED.") ==
                              0);
    TEST_ASSERT_EQ_INT(ctx, 60, wait_default_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuCopy();
    nFileNumber = 2;
    saveData[2].saved = 1;
    SDMenuCopy();
    TEST_ASSERT_TRUE(ctx,
                     strcmp(update_message_values[1],
                            "OK TO OVERWRITE FILE?") == 0);
    swData1 = 4096;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_TRUE(ctx,
                     strcmp(update_message_values[2],
                            "SELECT FILE TO COPY FROM.") == 0);
}

static void test_menu_rename_empty_saved_edit_finish_and_cancel(
    test_context *ctx) {
    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 0;
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    strcpy(saveData[1].player, "OLDNAME");
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 1, bFileChosed);
    TEST_ASSERT_TRUE(ctx, memcmp(editBuf, saveData[1].player, 12) == 0);
    TEST_ASSERT_EQ_INT(ctx, 1, disp_cursor_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, mov_cursor_values[0]);

    swData1 = 0;
    nTimerCunt = 10;
    edit_return = 0;
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 1, edit_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, disp_cursor_call_count);

    nTimerCunt = 20;
    edit_return = 1;
    strcpy(editBuf, "NEWNAME");
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_TRUE(ctx, memcmp(saveData[1].player, editBuf, 12) == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, hide_cursor_call_count);
    assert_point(ctx, run_sonic_points[0], infoSprtBmp[4].point.x +
                                                infoSprtBmp[4].w,
                 infoSprtBmp[4].point.y - 8);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuRename();
    nTimerCunt = 10;
    edit_return = -1;
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
}

static void test_menu_delete_empty_confirm_reselect_and_cancel(
    test_context *ctx) {
    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 0;
    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    nSelectFileNumber = 1;
    saveData[1].saved = 1;
    saveData[3].saved = 1;
    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 1, bFileChosed);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[0], "OK TO DELETE?") ==
                              0);

    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 0, saveData[1].saved);
    TEST_ASSERT_EQ_INT(ctx, 3, nSelectFileNumber);
    TEST_ASSERT_EQ_INT(ctx, 3, update_select_mark_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, update_file_view_values[0]);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[1], "FILE DELETED.") ==
                              0);
    TEST_ASSERT_EQ_INT(ctx, 60, wait_default_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    nSelectFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuDelete();
    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 0, nSelectFileNumber);
    TEST_ASSERT_EQ_INT(ctx, 0, update_select_mark_values[0]);

    reset_fixture();
    swData1 = 16384;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuDelete();
    swData1 = 4096;
    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_TRUE(ctx, strcmp(update_message_values[1],
                                  "SELECT FILE TO DELETE.") == 0);
}

static void test_menu_functions_return_while_waiting_for_selection(
    test_context *ctx) {
    reset_fixture();
    swData1 = 0;
    nFileNumber = 1;
    saveData[1].saved = 1;

    SDMenuSelect();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);

    reset_fixture();
    swData1 = 0;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);

    swData1 = 16384;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 1, bFileChosed);
    swData1 = 0;
    SDMenuCopy();
    TEST_ASSERT_EQ_INT(ctx, 0, bDestFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_call_count);

    reset_fixture();
    swData1 = 0;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuRename();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);

    reset_fixture();
    swData1 = 0;
    nFileNumber = 1;
    saveData[1].saved = 1;
    SDMenuDelete();
    TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);
}

static void test_menu_exit_sets_end_sequence(test_context *ctx) {
    reset_fixture();

    SDMenuExit();

    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 999, set_sequence_values[0]);
}

TEST_MAIN_BEGIN;
test_file_select_moves_selects_cancels_and_clamps(&ctx);
test_menu_select_empty_saved_confirm_and_cancel(&ctx);
test_menu_copy_source_dest_confirm_and_cancel(&ctx);
test_menu_rename_empty_saved_edit_finish_and_cancel(&ctx);
test_menu_delete_empty_confirm_reselect_and_cancel(&ctx);
test_menu_functions_return_while_waiting_for_selection(&ctx);
test_menu_exit_sets_end_sequence(&ctx);
TEST_MAIN_END

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/score_data_types.h"
#include "src/types.h"

Uint16 swData1;
Sint32 nFileNumber;
void (*WaveRequest)(Sint16);
void (*sPrintf)(char *, const char *, ...);
score_data saveData[6];
Sint32 nSelectFileNumber;

static Sint32 cursor_position;
static Sint32 wave_call_count;
static Sint16 wave_values[64];
static Sint32 disp_cursor_call_count;
static Sint32 mov_cursor_call_count;
static Sint32 mov_cursor_values[64];
static Sint32 wait_call_count;
static Sint32 wait_values[64];
static Sint32 wait_key_off_call_count;
static Sint32 delete_char_call_count;
static Sint32 delete_char_blocks[64];
static Sint32 delete_char_sprites[64];
static Sint32 create_char_call_count;
static char create_char_codes[64];
static Sint32 create_char_kinds[64];
static Sint32 create_char_blocks[64];
static Sint32 create_char_sprites[64];
static Sint32 create_string_call_count;
static char create_string_values[128][256];
static Sint32 create_string_kinds[128];
static Sint32 create_string_blocks[128];
static Sint32 create_string_starts[128];
static Sint32 oespr_create_call_count;
static Sint32 oespr_create_values[64];
static Sint32 oespr_delete_call_count;
static Sint32 oespr_delete_values[64];
static Sint32 default_message_call_count;
static Sint32 update_select_mark_call_count;
static Uint16 update_select_mark_values[16];

#include "src/title/savedata/svdedit.c"

static void fake_wave_request(Sint16 wave) {
    wave_values[wave_call_count] = wave;
    ++wave_call_count;
}

static void fake_sprintf(char *dst, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vsprintf(dst, fmt, args);
    va_end(args);
}

Sint32 GetCursolPositon(void) { return cursor_position; }

void MovCursol(Sint32 x) {
    mov_cursor_values[mov_cursor_call_count] = x;
    cursor_position = x;
    ++mov_cursor_call_count;
}

void DispCursol(void) { ++disp_cursor_call_count; }

void Wait(Sint32 time) {
    wait_values[wait_call_count] = time;
    ++wait_call_count;
}

void WaitKeyOff(void) { ++wait_key_off_call_count; }

void DeleteCharSprt(Sint32 nBlockNo, Sint32 nSprNum) {
    delete_char_blocks[delete_char_call_count] = nBlockNo;
    delete_char_sprites[delete_char_call_count] = nSprNum;
    ++delete_char_call_count;
}

Uint32 CreateCharSprt(char c, Sint32 kind, Sint32 nBlockNo, Sint32 nSprNum) {
    create_char_codes[create_char_call_count] = c;
    create_char_kinds[create_char_call_count] = kind;
    create_char_blocks[create_char_call_count] = nBlockNo;
    create_char_sprites[create_char_call_count] = nSprNum;
    ++create_char_call_count;
    return 1;
}

Uint32 CreateStringSprt(char *str, Sint32 kind, Sint32 nBlockNo,
                        Sint32 nStartSprNum) {
    strcpy(create_string_values[create_string_call_count], str);
    create_string_kinds[create_string_call_count] = kind;
    create_string_blocks[create_string_call_count] = nBlockNo;
    create_string_starts[create_string_call_count] = nStartSprNum;
    ++create_string_call_count;
    return 1;
}

Sint32 OESprCreateIndx(Sint32 indx) {
    oespr_create_values[oespr_create_call_count] = indx;
    ++oespr_create_call_count;
    return 0;
}

void OESprDeleteIndx(Sint32 indx) {
    oespr_delete_values[oespr_delete_call_count] = indx;
    ++oespr_delete_call_count;
}

void DefaultMessage(void) { ++default_message_call_count; }

void UpdateSelectSonicMark(Uint16 nNewNumber) {
    update_select_mark_values[update_select_mark_call_count] = nNewNumber;
    ++update_select_mark_call_count;
}

static void reset_fixture(void) {
    memset(saveData, 0, sizeof(saveData));
    memset(editBuf, 0, sizeof(editBuf));
    memset(wave_values, 0, sizeof(wave_values));
    memset(mov_cursor_values, 0, sizeof(mov_cursor_values));
    memset(wait_values, 0, sizeof(wait_values));
    memset(delete_char_blocks, 0, sizeof(delete_char_blocks));
    memset(delete_char_sprites, 0, sizeof(delete_char_sprites));
    memset(create_char_codes, 0, sizeof(create_char_codes));
    memset(create_char_kinds, 0, sizeof(create_char_kinds));
    memset(create_char_blocks, 0, sizeof(create_char_blocks));
    memset(create_char_sprites, 0, sizeof(create_char_sprites));
    memset(create_string_values, 0, sizeof(create_string_values));
    memset(create_string_kinds, 0, sizeof(create_string_kinds));
    memset(create_string_blocks, 0, sizeof(create_string_blocks));
    memset(create_string_starts, 0, sizeof(create_string_starts));
    memset(oespr_create_values, 0, sizeof(oespr_create_values));
    memset(oespr_delete_values, 0, sizeof(oespr_delete_values));
    memset(update_select_mark_values, 0, sizeof(update_select_mark_values));

    swData1 = 0;
    nFileNumber = 2;
    nSelectFileNumber = 4;
    cursor_position = 0;
    bEditMode = 0;
    WaveRequest = fake_wave_request;
    sPrintf = fake_sprintf;
    wave_call_count = 0;
    disp_cursor_call_count = 0;
    mov_cursor_call_count = 0;
    wait_call_count = 0;
    wait_key_off_call_count = 0;
    delete_char_call_count = 0;
    create_char_call_count = 0;
    create_string_call_count = 0;
    oespr_create_call_count = 0;
    oespr_delete_call_count = 0;
    default_message_call_count = 0;
    update_select_mark_call_count = 0;
}

static void test_edit_moves_cursor_left_and_right_with_clamps(
    test_context *ctx) {
    reset_fixture();

    swData1 = 1024;
    cursor_position = 4;
    TEST_ASSERT_EQ_INT(ctx, 0, Edit());
    TEST_ASSERT_EQ_INT(ctx, 54, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, disp_cursor_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, mov_cursor_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, wait_values[0]);

    cursor_position = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, Edit());
    TEST_ASSERT_EQ_INT(ctx, 0, mov_cursor_values[1]);

    swData1 = 2048;
    cursor_position = 7;
    TEST_ASSERT_EQ_INT(ctx, 0, Edit());
    TEST_ASSERT_EQ_INT(ctx, 8, mov_cursor_values[2]);

    cursor_position = 9;
    TEST_ASSERT_EQ_INT(ctx, 0, Edit());
    TEST_ASSERT_EQ_INT(ctx, 9, mov_cursor_values[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, wait_call_count);
}

static void assert_last_char_edit(test_context *ctx, Sint32 call, char code,
                                  Sint32 sprite) {
    TEST_ASSERT_EQ_INT(ctx, nFileNumber + 2, delete_char_blocks[call]);
    TEST_ASSERT_EQ_INT(ctx, sprite, delete_char_sprites[call]);
    TEST_ASSERT_EQ_INT(ctx, code, create_char_codes[call]);
    TEST_ASSERT_EQ_INT(ctx, 0, create_char_kinds[call]);
    TEST_ASSERT_EQ_INT(ctx, nFileNumber + 2, create_char_blocks[call]);
    TEST_ASSERT_EQ_INT(ctx, sprite, create_char_sprites[call]);
}

static void test_edit_cycles_characters_backward(test_context *ctx) {
    static const char starts[] = {' ', '-', '0', 'A', 'C'};
    static const char expected[] = {'Z', ' ', '-', '9', 'B'};
    Sint32 i;

    reset_fixture();
    swData1 = 512;
    for (i = 0; i < 5; ++i) {
        cursor_position = i;
        editBuf[i] = starts[i];
        TEST_ASSERT_EQ_INT(ctx, 0, Edit());
        TEST_ASSERT_EQ_INT(ctx, expected[i], editBuf[i]);
        assert_last_char_edit(ctx, i, expected[i], i);
    }
    TEST_ASSERT_EQ_INT(ctx, 5, wave_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, wait_call_count);
}

static void test_edit_cycles_characters_forward(test_context *ctx) {
    static const char starts[] = {' ', '-', '9', 'Z', 'C'};
    static const char expected[] = {'-', '0', 'A', ' ', 'D'};
    Sint32 i;

    reset_fixture();
    swData1 = 256;
    for (i = 0; i < 5; ++i) {
        cursor_position = i;
        editBuf[i] = starts[i];
        TEST_ASSERT_EQ_INT(ctx, 0, Edit());
        TEST_ASSERT_EQ_INT(ctx, expected[i], editBuf[i]);
        assert_last_char_edit(ctx, i, expected[i], i);
    }
    TEST_ASSERT_EQ_INT(ctx, 5, wave_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, wait_call_count);
}

static void test_edit_confirm_cancel_and_idle_modes(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, Edit());
    TEST_ASSERT_EQ_INT(ctx, 1, bEditMode);
    TEST_ASSERT_EQ_INT(ctx, 0, wait_key_off_call_count);

    swData1 = 16384;
    TEST_ASSERT_EQ_INT(ctx, 1, Edit());
    TEST_ASSERT_EQ_INT(ctx, 0, bEditMode);
    TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);

    swData1 = 4096;
    TEST_ASSERT_EQ_INT(ctx, -1, Edit());
    TEST_ASSERT_EQ_INT(ctx, 0, bEditMode);
    TEST_ASSERT_EQ_INT(ctx, 2, wait_key_off_call_count);
}

static void test_disp_save_data_formats_saved_empty_and_clamped_rounds(
    test_context *ctx) {
    reset_fixture();
    saveData[0].saved = 1;
    strcpy(saveData[0].player, "SONIC");
    saveData[0].year = 1996;
    saveData[0].month = 7;
    saveData[0].date = 4;
    saveData[0].roundNo = 2;
    saveData[0].hour = 1;
    saveData[0].min = 2;
    saveData[0].sec = 3;

    saveData[1].saved = 1;
    strcpy(saveData[1].player, "TAILS");
    saveData[1].year = 2001;
    saveData[1].month = 12;
    saveData[1].date = 31;
    saveData[1].roundNo = 99;
    saveData[1].hour = 9;
    saveData[1].min = 8;
    saveData[1].sec = 7;

    DispSaveData(0);
    TEST_ASSERT_EQ_INT(ctx, 4, create_string_call_count);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[0], "SONIC") == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, create_string_blocks[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, create_string_starts[0]);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[1], "1996/ 7/ 4") == 0);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[2], "ROUND-3") == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, create_string_kinds[2]);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[3], "01\"02\"03") == 0);
    TEST_ASSERT_EQ_INT(ctx, 1, oespr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 193, oespr_create_values[0]);

    DispSaveData(1);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[6], "ROUND-7") == 0);
    TEST_ASSERT_EQ_INT(ctx, 194, oespr_create_values[1]);

    DispSaveData(2);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[8], "NO DATA-FILE") == 0);
    TEST_ASSERT_EQ_INT(ctx, 4, create_string_blocks[8]);
    TEST_ASSERT_EQ_INT(ctx, 44, create_string_starts[8]);
    TEST_ASSERT_EQ_INT(ctx, 1, oespr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 195, oespr_delete_values[0]);
}

static void test_disp_all_draws_menu_labels_files_and_selected_mark(
    test_context *ctx) {
    reset_fixture();
    saveData[0].saved = 1;
    strcpy(saveData[0].player, "AAA");
    nSelectFileNumber = 3;

    DispAll();

    TEST_ASSERT_EQ_INT(ctx, 1, default_message_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, oespr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, oespr_create_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, oespr_create_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, oespr_create_values[2]);
    TEST_ASSERT_EQ_INT(ctx, 6, oespr_create_values[3]);
    TEST_ASSERT_EQ_INT(ctx, 8, oespr_create_values[4]);
    TEST_ASSERT_EQ_INT(ctx, 193, oespr_create_values[5]);
    TEST_ASSERT_EQ_INT(ctx, 1, update_select_mark_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, update_select_mark_values[0]);
    TEST_ASSERT_TRUE(ctx, create_string_call_count >= 9);
}

TEST_MAIN_BEGIN;
test_edit_moves_cursor_left_and_right_with_clamps(&ctx);
test_edit_cycles_characters_backward(&ctx);
test_edit_cycles_characters_forward(&ctx);
test_edit_confirm_cancel_and_idle_modes(&ctx);
test_disp_save_data_formats_saved_empty_and_clamped_rounds(&ctx);
test_disp_all_draws_menu_labels_files_and_selected_mark(&ctx);
TEST_MAIN_END

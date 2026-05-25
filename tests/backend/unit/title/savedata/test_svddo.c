#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/title/common/score_data_types.h"
#include "src/types.h"

Uint16 nSequenceNum;
sprite_bmp infoSprtBmp[256];
draw_context context;
draw_context *s_ctx;
score_data saveData[6];
Sint32 nFileNumber;
Sint32 nSelectFileNumber;
void (*WaveRequest)(Sint16);
Uint32 bDestFileChosed;
Uint32 bFileChosed;
Uint16 swData1;
Sint32 nTimerCunt;
Sint32 keybordInputTime;
Uint16 swData2;
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);

static hmx_sprite *sprite_table[1000];
static char sprite_storage[1000];
static Uint32 sprite_handles[256];
static Sint32 draw_call_count;
static Sint32 spr_create_call_count;
static Sint32 spr_create_values[64];
static Sint32 spr_delete_call_count;
static Sint32 spr_delete_values[64];
static Sint32 read_data_call_count;
static Sint32 disp_all_call_count;
static Sint32 set_sequence_call_count;
static Uint16 set_sequence_values[16];
static Sint32 disp_sonic_call_count;
static POINT disp_sonic_points[16];
static Sint32 position_call_count;
static hmx_sprite *position_sprites[32];
static Sint32 position_x[32];
static Sint32 position_y[32];
static Sint32 delete_string_call_count;
static Sint32 delete_string_lengths[16];
static Sint32 delete_string_blocks[16];
static Sint32 delete_string_starts[16];
static Sint32 disp_save_data_call_count;
static Sint32 disp_save_data_values[16];
static Sint32 create_string_call_count;
static char create_string_values[32][128];
static Sint32 create_string_kinds[32];
static Sint32 create_string_blocks[32];
static Sint32 create_string_starts[32];
static Sint32 wave_call_count;
static Sint16 wave_values[32];
static Sint32 run_sonic_call_count;
static POINT run_sonic_points[32];
static Sint32 key_wait_call_count;
static Sint32 wait_key_off_call_count;
static Sint32 menu_select_call_count;
static Sint32 menu_copy_call_count;
static Sint32 menu_rename_call_count;
static Sint32 menu_delete_call_count;
static Sint32 menu_exit_call_count;
static Sint32 write_data_call_count;

#include "src/title/savedata/svddo.c"

void srfDraw(void) { ++draw_call_count; }

Sint32 OESprCreateIndx(Sint32 indx) {
    spr_create_values[spr_create_call_count] = indx;
    ++spr_create_call_count;
    return 0;
}

void OESprDeleteIndx(Sint32 indx) {
    spr_delete_values[spr_delete_call_count] = indx;
    ++spr_delete_call_count;
}

void ReadDataFile(void) { ++read_data_call_count; }

void DispAll(void) { ++disp_all_call_count; }

void OESetSequenceNum(Uint16 num) {
    set_sequence_values[set_sequence_call_count] = num;
    ++set_sequence_call_count;
    nSequenceNum = num;
    nTimerCunt = -1;
}

void DispSonicCursol(POINT point) {
    disp_sonic_points[disp_sonic_call_count] = point;
    ++disp_sonic_call_count;
}

void DeleteStringSprt(Sint32 strLength, Sint32 nBlockNo, Sint32 nStartSprNum) {
    delete_string_lengths[delete_string_call_count] = strLength;
    delete_string_blocks[delete_string_call_count] = nBlockNo;
    delete_string_starts[delete_string_call_count] = nStartSprNum;
    ++delete_string_call_count;
}

void DispSaveData(Sint32 indx) {
    disp_save_data_values[disp_save_data_call_count] = indx;
    ++disp_save_data_call_count;
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

static void fake_wave_request(Sint16 wave) {
    wave_values[wave_call_count] = wave;
    ++wave_call_count;
}

void RunSonicCursol(POINT point) {
    run_sonic_points[run_sonic_call_count] = point;
    ++run_sonic_call_count;
}

void KeyWait(void) { ++key_wait_call_count; }

void WaitKeyOff(void) { ++wait_key_off_call_count; }

void SDMenuSelect(void) { ++menu_select_call_count; }

void SDMenuCopy(void) { ++menu_copy_call_count; }

void SDMenuRename(void) { ++menu_rename_call_count; }

void SDMenuDelete(void) { ++menu_delete_call_count; }

void SDMenuExit(void) { ++menu_exit_call_count; }

void WriteDataFile(void) { ++write_data_call_count; }

static hmx_sprite *sprite_handle(Sint32 handle) {
    return (hmx_sprite *)&sprite_storage[handle];
}

static void fake_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    position_sprites[position_call_count] = sprite;
    position_x[position_call_count] = x;
    position_y[position_call_count] = y;
    ++position_call_count;
}

static void reset_fixture(void) {
    Sint32 i;

    memset(infoSprtBmp, 0, sizeof(infoSprtBmp));
    memset(&context, 0, sizeof(context));
    memset(saveData, 0, sizeof(saveData));
    memset(sprite_handles, 0, sizeof(sprite_handles));
    memset(spr_create_values, 0, sizeof(spr_create_values));
    memset(spr_delete_values, 0, sizeof(spr_delete_values));
    memset(set_sequence_values, 0, sizeof(set_sequence_values));
    memset(disp_sonic_points, 0, sizeof(disp_sonic_points));
    memset(position_sprites, 0, sizeof(position_sprites));
    memset(position_x, 0, sizeof(position_x));
    memset(position_y, 0, sizeof(position_y));
    memset(delete_string_lengths, 0, sizeof(delete_string_lengths));
    memset(delete_string_blocks, 0, sizeof(delete_string_blocks));
    memset(delete_string_starts, 0, sizeof(delete_string_starts));
    memset(disp_save_data_values, 0, sizeof(disp_save_data_values));
    memset(create_string_values, 0, sizeof(create_string_values));
    memset(create_string_kinds, 0, sizeof(create_string_kinds));
    memset(create_string_blocks, 0, sizeof(create_string_blocks));
    memset(create_string_starts, 0, sizeof(create_string_starts));
    memset(wave_values, 0, sizeof(wave_values));
    memset(run_sonic_points, 0, sizeof(run_sonic_points));

    for (i = 0; i < 1000; ++i) {
        sprite_table[i] = sprite_handle(i);
    }
    for (i = 0; i < 256; ++i) {
        sprite_handles[i] = (Uint32)i;
        infoSprtBmp[i].lphSpr = &sprite_handles[i];
        infoSprtBmp[i].point.x = i * 3;
        infoSprtBmp[i].point.y = i * 5;
        infoSprtBmp[i].w = 10 + i;
    }
    context.sprites = sprite_table;
    s_ctx = &context;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    WaveRequest = fake_wave_request;

    nSequenceNum = 0;
    nMenuKind = 1;
    bMenuChosed = 0;
    nFileNumber = 0;
    nSelectFileNumber = 2;
    bDestFileChosed = 0;
    bFileChosed = 0;
    swData1 = 0;
    nTimerCunt = 0;
    keybordInputTime = 0;
    swData2 = 0;
    draw_call_count = 0;
    spr_create_call_count = 0;
    spr_delete_call_count = 0;
    read_data_call_count = 0;
    disp_all_call_count = 0;
    set_sequence_call_count = 0;
    disp_sonic_call_count = 0;
    position_call_count = 0;
    delete_string_call_count = 0;
    disp_save_data_call_count = 0;
    create_string_call_count = 0;
    wave_call_count = 0;
    run_sonic_call_count = 0;
    key_wait_call_count = 0;
    wait_key_off_call_count = 0;
    menu_select_call_count = 0;
    menu_copy_call_count = 0;
    menu_rename_call_count = 0;
    menu_delete_call_count = 0;
    menu_exit_call_count = 0;
    write_data_call_count = 0;
}

static void assert_point(test_context *ctx, POINT point, Sint32 x, Sint32 y) {
    TEST_ASSERT_EQ_INT(ctx, x, point.x);
    TEST_ASSERT_EQ_INT(ctx, y, point.y);
}

static void test_draw_and_start_initialization(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    SDDraw();
    TEST_ASSERT_EQ_INT(ctx, 0, draw_call_count);

    nSequenceNum = 2;
    SDDraw();
    TEST_ASSERT_EQ_INT(ctx, 1, draw_call_count);

    SDStart();
    TEST_ASSERT_EQ_INT(ctx, 4, spr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 199, spr_create_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 200, spr_create_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 201, spr_create_values[2]);
    TEST_ASSERT_EQ_INT(ctx, 202, spr_create_values[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, read_data_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, disp_all_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, set_sequence_call_count);
    TEST_ASSERT_EQ_INT(ctx, 2, set_sequence_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, disp_sonic_call_count);
    assert_point(ctx, disp_sonic_points[0], 10, -8);
}

static void test_file_select_mark_file_view_and_messages(test_context *ctx) {
    reset_fixture();

    DeleteFileSelect();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 192, spr_delete_values[0]);

    UpdateFileSelect(3);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, spr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 192, spr_create_values[0]);
    TEST_ASSERT_TRUE(ctx, position_sprites[0] == sprite_handle(192));
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[192].point.x, position_x[0]);
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[192].point.y + 96, position_y[0]);

    sprite_handles[10] = 0;
    saveData[2].saved = 0;
    UpdateSelectSonicMark(2);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_call_count);

    sprite_handles[10] = 10;
    UpdateSelectSonicMark(2);
    TEST_ASSERT_EQ_INT(ctx, 3, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, spr_delete_values[2]);

    saveData[2].saved = 1;
    sprite_handles[10] = 0;
    UpdateSelectSonicMark(2);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, spr_create_values[1]);
    TEST_ASSERT_TRUE(ctx, position_sprites[1] == sprite_handle(0));
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[10].point.y + 64, position_y[1]);

    UpdateFileView(4);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_string_call_count);
    TEST_ASSERT_EQ_INT(ctx, 56, delete_string_lengths[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, delete_string_blocks[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_string_starts[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, disp_save_data_call_count);
    TEST_ASSERT_EQ_INT(ctx, 4, disp_save_data_values[0]);

    UpdateMessage("HELLO");
    TEST_ASSERT_EQ_INT(ctx, 2, delete_string_call_count);
    TEST_ASSERT_EQ_INT(ctx, 27, delete_string_lengths[1]);
    TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[0], "HELLO") == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, create_string_kinds[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, create_string_blocks[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, create_string_starts[0]);

    DefaultMessage();
    TEST_ASSERT_TRUE(ctx,
                     strcmp(create_string_values[1], "SONIC CD DATA-FILES") ==
                         0);
}

static void test_chk_save_file(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, ChkSaveFile());
    saveData[5].saved = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, ChkSaveFile());
}

static void test_update_menu_same_selection_is_noop(test_context *ctx) {
    reset_fixture();

    UpdateMenu(3, 3);

    TEST_ASSERT_EQ_INT(ctx, 0, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, spr_create_call_count);
}

static void test_sdmenu_navigation_and_boundaries(test_context *ctx) {
    reset_fixture();
    swData1 = 512;
    nMenuKind = 1;

    SDMenu();

    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_call_count);
    TEST_ASSERT_EQ_INT(ctx, 9, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, spr_create_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, spr_create_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, run_sonic_call_count);
    assert_point(ctx, run_sonic_points[0],
                 infoSprtBmp[2].point.x + infoSprtBmp[2].w,
                 infoSprtBmp[2].point.y - 8);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);

    reset_fixture();
    swData1 = 512;
    nMenuKind = 5;
    SDMenu();
    TEST_ASSERT_EQ_INT(ctx, 5, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);

    reset_fixture();
    swData1 = 256;
    nMenuKind = 3;
    SDMenu();
    TEST_ASSERT_EQ_INT(ctx, 2, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 9, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, spr_delete_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, spr_create_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, spr_create_values[1]);

    reset_fixture();
    swData1 = 256;
    nMenuKind = 1;
    SDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, nMenuKind);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);
}

static void test_sdmenu_select_rejects_empty_non_exit_menu(test_context *ctx) {
    reset_fixture();
    swData1 = 16384;
    nMenuKind = 1;

    SDMenu();

    TEST_ASSERT_EQ_INT(ctx, 0, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_call_count);
    TEST_ASSERT_EQ_INT(ctx, 30, wave_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, key_wait_call_count);
}

static void test_sdmenu_selects_each_menu_kind(test_context *ctx) {
    static const char *messages[] = {"SELECT FILE TO USE IN GAME.",
                                     "SELECT FILE TO COPY FROM.",
                                     "SELECT FILE TO RENAME.",
                                     "SELECT FILE TO DELETE."};
    Sint32 kind;

    for (kind = 1; kind <= 4; ++kind) {
        reset_fixture();
        swData1 = 16384;
        nMenuKind = (Uint16)kind;
        nSelectFileNumber = 2;
        saveData[0].saved = 1;

        SDMenu();

        TEST_ASSERT_EQ_INT(ctx, 1, bMenuChosed);
        TEST_ASSERT_EQ_INT(ctx, 0, bFileChosed);
        TEST_ASSERT_EQ_INT(ctx, 0, bDestFileChosed);
        TEST_ASSERT_EQ_INT(ctx, 2, nFileNumber);
        TEST_ASSERT_EQ_INT(ctx, 26, wave_values[0]);
        TEST_ASSERT_TRUE(ctx, strcmp(create_string_values[0],
                                     messages[kind - 1]) == 0);
        TEST_ASSERT_EQ_INT(ctx, 192, spr_delete_values[0]);
        TEST_ASSERT_EQ_INT(ctx, 192, spr_create_values[0]);
        assert_point(ctx, run_sonic_points[0],
                     infoSprtBmp[195].point.x - 16,
                     infoSprtBmp[195].point.y);
        TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);
    }

    reset_fixture();
    swData1 = 16384;
    nMenuKind = 5;
    SDMenu();
    TEST_ASSERT_EQ_INT(ctx, 1, bMenuChosed);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, wait_key_off_call_count);
}

static void test_sdmenu_dispatches_chosen_menu_handlers(test_context *ctx) {
    reset_fixture();
    bMenuChosed = 1;

    nMenuKind = 1;
    SDMenu();
    nMenuKind = 2;
    SDMenu();
    nMenuKind = 3;
    SDMenu();
    nMenuKind = 4;
    SDMenu();
    nMenuKind = 5;
    SDMenu();

    TEST_ASSERT_EQ_INT(ctx, 1, menu_select_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, menu_copy_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, menu_rename_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, menu_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, menu_exit_call_count);
}

static void test_end_and_auto_end(test_context *ctx) {
    reset_fixture();

    nTimerCunt = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, SDEnd());
    TEST_ASSERT_EQ_INT(ctx, 0, write_data_call_count);

    nTimerCunt = 0;
    TEST_ASSERT_EQ_INT(ctx, 9, SDEnd());
    TEST_ASSERT_EQ_INT(ctx, 1, write_data_call_count);

    keybordInputTime = 10;
    nTimerCunt = 36010;
    TEST_ASSERT_EQ_INT(ctx, 0, CheckAutoEnd());

    nTimerCunt = 36011;
    TEST_ASSERT_EQ_INT(ctx, 4, CheckAutoEnd());

    swData1 = 1;
    nTimerCunt = 72000;
    TEST_ASSERT_EQ_INT(ctx, 0, CheckAutoEnd());

    swData1 = 0;
    swData2 = 1;
    nTimerCunt = 108000;
    TEST_ASSERT_EQ_INT(ctx, 0, CheckAutoEnd());
}

TEST_MAIN_BEGIN;
test_draw_and_start_initialization(&ctx);
test_file_select_mark_file_view_and_messages(&ctx);
test_chk_save_file(&ctx);
test_update_menu_same_selection_is_noop(&ctx);
test_sdmenu_navigation_and_boundaries(&ctx);
test_sdmenu_select_rejects_empty_non_exit_menu(&ctx);
test_sdmenu_selects_each_menu_kind(&ctx);
test_sdmenu_dispatches_chosen_menu_handlers(&ctx);
test_end_and_auto_end(&ctx);
TEST_MAIN_END

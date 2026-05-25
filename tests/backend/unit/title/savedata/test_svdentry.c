#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/engine_dll.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

static draw_context draw_ctx;
static char env_storage;
static char loader_storage;
PALETTEENTRY tblPal1[16];
PALETTEENTRY tblPal2[16];
PALETTEENTRY tblPal3[16];
PALETTEENTRY tblPal4[16];

static Sint32 clrset_call_count;
static PALETTEENTRY *clrset_palettes[4];
static Uint16 clrset_numbers[4];
static Sint32 create_ea_call_count;
static Sint32 delete_ea_call_count;
static Sint32 delete_all_char_call_count;
static Sint32 default_message_call_count;
static Sint32 sd_start_call_count;
static Sint32 sd_menu_call_count;
static Sint32 sd_draw_call_count;
static Sint32 sd_end_call_count;
static Sint32 sd_end_return;
static Sint32 update_sonic_call_count;
static Sint32 check_auto_end_call_count;
static Sint32 check_auto_end_return;
static Sint32 wave_request_call_count;
static Sint16 wave_request_values[4];
static draw_context *draw_context_return;

#include "src/title/savedata/svdentry.c"

void OEClrset(PALETTEENTRY *pPalet, Uint16 nNumber) {
    clrset_palettes[clrset_call_count] = pPalet;
    clrset_numbers[clrset_call_count] = nNumber;
    ++clrset_call_count;
}

Sint32 OECreateEA(void) {
    ++create_ea_call_count;
    return 0;
}

void OEDeleteEA(void) { ++delete_ea_call_count; }

void DeleteAllCharSprt(void) { ++delete_all_char_call_count; }

void DefaultMessage(void) { ++default_message_call_count; }

void SDStart(void) { ++sd_start_call_count; }

void SDMenu(void) { ++sd_menu_call_count; }

void SDDraw(void) { ++sd_draw_call_count; }

Sint32 SDEnd(void) {
    ++sd_end_call_count;
    return sd_end_return;
}

void UpdateSonicCursol(void) { ++update_sonic_call_count; }

Sint32 CheckAutoEnd(void) {
    ++check_auto_end_call_count;
    return check_auto_end_return;
}

static void fake_wave_request(Sint16 request) {
    wave_request_values[wave_request_call_count] = request;
    ++wave_request_call_count;
}

static Uint32 fake_score_read(Sint32 slot, char *data, Uint32 mode) {
    (void)slot;
    (void)data;
    (void)mode;
    return 0;
}

static Uint32 fake_score_write(Sint32 slot, char *data, Uint32 mode) {
    (void)slot;
    (void)data;
    (void)mode;
    return 0;
}

static Uint32 fake_score_open(Sint32 slot) {
    (void)slot;
    return 0;
}

static Uint32 fake_score_close(Uint32 handle) {
    (void)handle;
    return 0;
}

static Uint32 fake_score_create(void) { return 0; }

static Sint32 fake_index_read(Uint32 slot) {
    (void)slot;
    return 0;
}

static Uint32 fake_index_write(Sint32 slot, Uint32 value) {
    (void)slot;
    (void)value;
    return 0;
}

static draw_context *fake_get_draw_context(void) { return draw_context_return; }

static void fake_void_function(void) {}
static void *fake_alloc(Sint32 size) {
    (void)size;
    return 0;
}
static void fake_mem_free(void *ptr) { (void)ptr; }
static void fake_mem_set(void *dst, Uint8 value, Sint32 bytes) {
    memset(dst, value, (size_t)bytes);
}
static void fake_mem_cpy(void *dst, void *src, Sint32 bytes) {
    memcpy(dst, src, (size_t)bytes);
}
static Sint32 fake_open_file(char *path) {
    (void)path;
    return 0;
}
static Sint32 fake_int_function(void) { return 0; }
static void fake_sprite_set_flags(hmx_sprite *sprite, Sint32 flags) {
    (void)sprite;
    (void)flags;
}

static void reset_fixture(void) {
    memset(&draw_ctx, 0, sizeof(draw_ctx));
    memset(tblPal1, 0, sizeof(tblPal1));
    memset(tblPal2, 0, sizeof(tblPal2));
    memset(tblPal3, 0, sizeof(tblPal3));
    memset(tblPal4, 0, sizeof(tblPal4));
    memset(clrset_palettes, 0, sizeof(clrset_palettes));
    memset(clrset_numbers, 0, sizeof(clrset_numbers));
    memset(wave_request_values, 0, sizeof(wave_request_values));

    nSequenceNum = 0;
    nTimerCunt = 0;
    keybordInputTime = 0;
    hSurf = 0;
    hWnd = 0;
    lpColorwk = 0;
    swData1 = 0;
    swData2 = 0;
    lpCrntScorData = 0;
    bDefaultMsgFlag = 0;
    bWaitKeyOff = 0;
    nSaveSequenceNum = 0;
    nWaitTime = 0;
    s_ctx = 0;
    ReadScore = 0;
    WriteScore = 0;
    OpenScore = 0;
    CloseScore = 0;
    CreateScore = 0;
    ReadIndx = 0;
    WriteIndx = 0;
    WaveRequest = 0;
    get_draw_context_module = 0;
    FlipToScreen_module = 0;
    ld_load_cmpfile_module = 0;
    ld_bitmap_4to8_module = 0;
    hmx_background_set_background_module = 0;
    hmx_bitmap_create_module = 0;
    hmx_bitmap_release_module = 0;
    hmx_bitmap_get_scan0_module = 0;
    hmx_bitmap_set_transparency_module = 0;
    hmx_free_module = 0;
    hmx_grid_create_module = 0;
    hmx_grid_release_module = 0;
    hmx_grid_base_module = 0;
    hmx_grid_set_position_module = 0;
    hmx_grid_set_view_module = 0;
    hmx_grid_set_tile_module = 0;
    hmx_grid_set_horz_offsets_module = 0;
    hmx_renderer_context_add_module = 0;
    hmx_renderer_context_clear_module = 0;
    hmx_renderer_context_draw_module = 0;
    hmx_sprite_base_module = 0;
    hmx_sprite_set_position_module = 0;
    hmx_sprite_set_bitmap_module = 0;
    hmx_sprite_set_flags_module = 0;
    hmx_ddagrid_create_module = 0;
    hmx_ddagrid_release_module = 0;
    hmx_ddagrid_set_scan_module = 0;
    hmx_ddagrid_set_tile_module = 0;
    sMemAlloc = 0;
    sMemFree = 0;
    sMemCpy = 0;
    sMemSet = 0;
    sGetFileSize = 0;
    sOpenFile = 0;
    sReadFile = 0;
    sCloseFile = 0;
    sPrintf = 0;
    sOutputDebugString = 0;
    g_loader_module = 0;
    g_env_module = 0;

    clrset_call_count = 0;
    create_ea_call_count = 0;
    delete_ea_call_count = 0;
    delete_all_char_call_count = 0;
    default_message_call_count = 0;
    sd_start_call_count = 0;
    sd_menu_call_count = 0;
    sd_draw_call_count = 0;
    sd_end_call_count = 0;
    sd_end_return = 9;
    update_sonic_call_count = 0;
    check_auto_end_call_count = 0;
    check_auto_end_return = 0;
    wave_request_call_count = 0;
    draw_context_return = &draw_ctx;
}

static void fill_func_table(void **funcs) {
    Sint32 i;

    for (i = 0; i < 64; ++i) {
        funcs[i] = fake_void_function;
    }

    funcs[11] = fake_alloc;
    funcs[12] = fake_mem_free;
    funcs[13] = fake_mem_set;
    funcs[14] = fake_mem_cpy;
    funcs[20] = fake_void_function;
    funcs[21] = fake_void_function;
    funcs[22] = fake_open_file;
    funcs[23] = fake_int_function;
    funcs[24] = fake_void_function;
    funcs[25] = fake_int_function;
    funcs[26] = fake_get_draw_context;
    funcs[49] = fake_sprite_set_flags;
}

static void fill_buffer_table(char ***buffers, char **env_slot,
                              char **loader_slot) {
    Sint32 i;
    static char *dummy = 0;
    static char **dummy_slot = &dummy;
    static char *env_ptr;
    static char *loader_ptr;

    env_ptr = &env_storage;
    loader_ptr = &loader_storage;
    *env_slot = env_ptr;
    *loader_slot = loader_ptr;

    for (i = 0; i < 16; ++i) {
        buffers[i] = dummy_slot;
    }
    buffers[11] = env_slot;
    buffers[12] = loader_slot;
}

static void test_wait_helpers_set_saved_state(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 5;
    nTimerCunt = 99;
    OESetSequenceNum(6);
    TEST_ASSERT_EQ_INT(ctx, 6, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, -1, nTimerCunt);

    KeyWait();
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 6, nSaveSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 6, nWaitTime);
    TEST_ASSERT_EQ_INT(ctx, 0, bWaitKeyOff);

    nSequenceNum = 8;
    WaitKeyOff();
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 8, nSaveSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 1, bWaitKeyOff);

    nSequenceNum = 9;
    Wait(12);
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 9, nSaveSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 12, nWaitTime);
    TEST_ASSERT_EQ_INT(ctx, 0, bWaitKeyOff);

    nSequenceNum = 10;
    WaitAndDfltMsg(14);
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 10, nSaveSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 1, bWaitKeyOff);
    TEST_ASSERT_EQ_INT(ctx, 1, bDefaultMsgFlag);
    TEST_ASSERT_EQ_INT(ctx, 14, nWaitTime);
}

static void test_sdwait_restores_sequence_and_default_message(
    test_context *ctx) {
    reset_fixture();

    nSequenceNum = 10;
    WaitAndDfltMsg(5);
    nTimerCunt = 5;
    DLLMain();
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 0, default_message_call_count);

    nTimerCunt = 6;
    swData1 = 0;
    DLLMain();
    TEST_ASSERT_EQ_INT(ctx, 0, bWaitKeyOff);
    TEST_ASSERT_EQ_INT(ctx, 10, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 1, default_message_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bDefaultMsgFlag);
}

static void test_dllinit_binds_imports_callbacks_and_initializes(
    test_context *ctx) {
    engine_dll dll;
    void *funcs[64];
    char **buffers[16];
    char *env_slot;
    char *loader_slot;
    PALETTEENTRY palette[64];
    score_data current_score;

    reset_fixture();
    memset(&dll, 0, sizeof(dll));
    memset(funcs, 0, sizeof(funcs));
    memset(&current_score, 0, sizeof(current_score));
    fill_func_table(funcs);
    fill_buffer_table(buffers, &env_slot, &loader_slot);
    dll.hWnd = 101;
    dll.hSurf = 202;
    dll.lpColorwk = palette;
    dll.lpCrntScorData = &current_score;
    dll.ReadScoreData = fake_score_read;
    dll.WriteScoreData = fake_score_write;
    dll.OpenScoreData = fake_score_open;
    dll.CloseScoreData = fake_score_close;
    dll.CreateScoreData = fake_score_create;
    dll.ReadScoreIndx = fake_index_read;
    dll.WriteScoreIndx = fake_index_write;
    dll.WaveRequest = fake_wave_request;

    DLLInit(&dll, buffers, funcs);

    TEST_ASSERT_TRUE(ctx, sMemAlloc == fake_alloc);
    TEST_ASSERT_TRUE(ctx, sMemFree == fake_mem_free);
    TEST_ASSERT_TRUE(ctx, sMemSet == fake_mem_set);
    TEST_ASSERT_TRUE(ctx, sMemCpy == fake_mem_cpy);
    TEST_ASSERT_TRUE(ctx, sOpenFile == fake_open_file);
    TEST_ASSERT_TRUE(ctx, get_draw_context_module == fake_get_draw_context);
    TEST_ASSERT_TRUE(ctx, hmx_sprite_set_flags_module == fake_sprite_set_flags);
    TEST_ASSERT_TRUE(ctx, g_env_module == (hmx_environment *)&env_storage);
    TEST_ASSERT_TRUE(ctx, g_loader_module == (hmx_environment *)&loader_storage);
    TEST_ASSERT_TRUE(ctx, s_ctx == &draw_ctx);
    TEST_ASSERT_EQ_INT(ctx, 101, hWnd);
    TEST_ASSERT_EQ_INT(ctx, 202, hSurf);
    TEST_ASSERT_TRUE(ctx, lpColorwk == palette);
    TEST_ASSERT_TRUE(ctx, lpCrntScorData == &current_score);
    TEST_ASSERT_TRUE(ctx, ReadScore == fake_score_read);
    TEST_ASSERT_TRUE(ctx, WriteScore == fake_score_write);
    TEST_ASSERT_TRUE(ctx, OpenScore == fake_score_open);
    TEST_ASSERT_TRUE(ctx, CloseScore == fake_score_close);
    TEST_ASSERT_TRUE(ctx, CreateScore == fake_score_create);
    TEST_ASSERT_TRUE(ctx, ReadIndx == fake_index_read);
    TEST_ASSERT_TRUE(ctx, WriteIndx == fake_index_write);
    TEST_ASSERT_TRUE(ctx, WaveRequest == fake_wave_request);
    TEST_ASSERT_EQ_INT(ctx, 4, clrset_call_count);
    TEST_ASSERT_TRUE(ctx, clrset_palettes[0] == tblPal1);
    TEST_ASSERT_TRUE(ctx, clrset_palettes[1] == tblPal2);
    TEST_ASSERT_TRUE(ctx, clrset_palettes[2] == tblPal3);
    TEST_ASSERT_TRUE(ctx, clrset_palettes[3] == tblPal4);
    TEST_ASSERT_EQ_INT(ctx, 0, clrset_numbers[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, clrset_numbers[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, clrset_numbers[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, clrset_numbers[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, create_ea_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 0, nTimerCunt);
}

static void test_dllend_deletes_engine_and_character_sprites(test_context *ctx) {
    reset_fixture();

    DLLEnd();

    TEST_ASSERT_EQ_INT(ctx, 1, delete_ea_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_all_char_call_count);
}

static void test_swdata_set_copies_both_words(test_context *ctx) {
    ushort_union sw1;
    ushort_union sw2;

    reset_fixture();
    sw1.w = 0x1234;
    sw2.w = 0x4321;

    SWdataSet(sw1, sw2);

    TEST_ASSERT_EQ_INT(ctx, 0x1234, swData1);
    TEST_ASSERT_EQ_INT(ctx, 0x4321, swData2);
}

static void test_dllmain_dispatches_sequences_and_auto_end(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    nTimerCunt = 10;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, sd_start_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sd_draw_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, update_sonic_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, check_auto_end_call_count);
    TEST_ASSERT_EQ_INT(ctx, 11, nTimerCunt);

    nSequenceNum = 2;
    check_auto_end_return = 4;
    TEST_ASSERT_EQ_INT(ctx, 4, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, sd_menu_call_count);

    nSequenceNum = 999;
    check_auto_end_return = 4;
    TEST_ASSERT_EQ_INT(ctx, 9, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, sd_end_call_count);

    nSequenceNum = 0;
    sd_end_return = 0;
    check_auto_end_return = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());

    nSequenceNum = 4;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());

    nSequenceNum = 1000;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
}

static void test_dllmain_wraps_max_timer_to_5184000(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 0;
    nTimerCunt = 0x7FFFFFFF;

    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());

    TEST_ASSERT_EQ_INT(ctx, 5184000, nTimerCunt);
}

TEST_MAIN_BEGIN;
test_wait_helpers_set_saved_state(&ctx);
test_sdwait_restores_sequence_and_default_message(&ctx);
test_dllinit_binds_imports_callbacks_and_initializes(&ctx);
test_dllend_deletes_engine_and_character_sprites(&ctx);
test_swdata_set_copies_both_words(&ctx);
test_dllmain_dispatches_sequences_and_auto_end(&ctx);
test_dllmain_wraps_max_timer_to_5184000(&ctx);
TEST_MAIN_END

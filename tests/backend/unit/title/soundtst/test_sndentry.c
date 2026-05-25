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
Sint32 bKakusi;

static Sint32 clrset_call_count;
static PALETTEENTRY *clrset_palettes[4];
static Uint16 clrset_numbers[4];
static Sint32 create_ea_call_count;
static Sint32 delete_ea_call_count;
static Sint32 delete_all_char_call_count;
static Sint32 sonic_delete_call_count;
static Sint32 snd_start_call_count;
static Sint32 snd_menu_call_count;
static Sint32 snd_kakusi_call_count;
static Sint32 special_mes_call_count;
static Sint32 snd_draw_call_count;
static Sint32 snd_end_call_count;
static Sint32 snd_end2_call_count;
static Sint32 snd_end3_call_count;
static Sint32 snd_end_return;
static Sint32 snd_end2_return;
static Sint32 snd_end3_return;
static Sint32 cd_play_call_count;
static Sint16 cd_play_values[4];
static Sint32 cd_pause_call_count;
static Sint32 wave_request_call_count;
static Sint16 wave_request_values[4];
static draw_context *draw_context_return;

#include "src/title/soundtst/sndentry.c"

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

Sint32 MC_SONICDelete(void) {
    ++sonic_delete_call_count;
    return 0;
}

void SNDStart(void) { ++snd_start_call_count; }

void SNDMenu(void) { ++snd_menu_call_count; }

void SNDKakusi(void) { ++snd_kakusi_call_count; }

void SNDSpecialMes(void) { ++special_mes_call_count; }

void SNDDraw(void) { ++snd_draw_call_count; }

Sint32 SNDEnd(void) {
    ++snd_end_call_count;
    return snd_end_return;
}

Sint32 SNDEnd2(void) {
    ++snd_end2_call_count;
    return snd_end2_return;
}

Sint32 SNDEnd3(void) {
    ++snd_end3_call_count;
    return snd_end3_return;
}

static void fake_cd_play(Sint16 track) {
    cd_play_values[cd_play_call_count] = track;
    ++cd_play_call_count;
}

static void fake_cd_pause(void) { ++cd_pause_call_count; }

static void fake_wave_request(Sint16 wave) {
    wave_request_values[wave_request_call_count] = wave;
    ++wave_request_call_count;
}

static draw_context *fake_get_draw_context(void) { return draw_context_return; }

static void fake_void_function(void) {}
static void *fake_alloc(Sint32 size) {
    (void)size;
    return 0;
}
static void fake_mem_free(void *ptr) { (void)ptr; }
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
    memset(cd_play_values, 0, sizeof(cd_play_values));
    memset(wave_request_values, 0, sizeof(wave_request_values));

    nSequenceNum = 0;
    nTimerCunt = 0;
    hSurf = 0;
    hWnd = 0;
    lpColorwk = 0;
    swData1 = 0;
    swData2 = 0;
    s_ctx = 0;
    get_draw_context_module = 0;
    FlipToScreen_module = 0;
    ld_load_cmpfile_module = 0;
    ld_bitmap_4to8_module = 0;
    hmx_sprite_set_flags_module = 0;
    sMemAlloc = 0;
    sMemFree = 0;
    sMemCpy = 0;
    sGetFileSize = 0;
    sOpenFile = 0;
    sReadFile = 0;
    sCloseFile = 0;
    sPrintf = 0;
    sOutputDebugString = 0;
    CDPlay = 0;
    CDPause = 0;
    WaveRequest = 0;
    nSaveSequenceNum = 0;
    nWaitTime = 0;
    bWaitKeyOff = 0;
    g_loader_module = 0;
    g_env_module = 0;
    bKakusi = 0;

    clrset_call_count = 0;
    create_ea_call_count = 0;
    delete_ea_call_count = 0;
    delete_all_char_call_count = 0;
    sonic_delete_call_count = 0;
    snd_start_call_count = 0;
    snd_menu_call_count = 0;
    snd_kakusi_call_count = 0;
    special_mes_call_count = 0;
    snd_draw_call_count = 0;
    snd_end_call_count = 0;
    snd_end2_call_count = 0;
    snd_end3_call_count = 0;
    snd_end_return = 9;
    snd_end2_return = 18;
    snd_end3_return = 19;
    cd_play_call_count = 0;
    cd_pause_call_count = 0;
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
    funcs[14] = fake_void_function;
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

static void test_sequence_wait_helpers_set_saved_state(test_context *ctx) {
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
}

static void test_sndwait_restores_after_timer_and_handles_key_off(
    test_context *ctx) {
    reset_fixture();

    Wait(5);
    nTimerCunt = 5;
    SNDWait();
    TEST_ASSERT_EQ_INT(ctx, 3, nSequenceNum);

    nTimerCunt = 6;
    SNDWait();
    TEST_ASSERT_EQ_INT(ctx, 0, nSequenceNum);

    nSequenceNum = 4;
    WaitKeyOff();
    swData1 = 1;
    SNDWait();
    TEST_ASSERT_EQ_INT(ctx, 1, bWaitKeyOff);

    swData1 = 0;
    SNDWait();
    TEST_ASSERT_EQ_INT(ctx, 0, bWaitKeyOff);
}

static void test_dllinit_binds_imports_sets_globals_and_initializes(
    test_context *ctx) {
    engine_dll dll;
    void *funcs[64];
    char **buffers[16];
    char *env_slot;
    char *loader_slot;
    PALETTEENTRY palette[64];

    reset_fixture();
    memset(&dll, 0, sizeof(dll));
    memset(funcs, 0, sizeof(funcs));
    fill_func_table(funcs);
    fill_buffer_table(buffers, &env_slot, &loader_slot);
    dll.hWnd = 101;
    dll.hSurf = 202;
    dll.lpColorwk = palette;
    dll.CDPlay = fake_cd_play;
    dll.CDPause = fake_cd_pause;
    dll.WaveRequest = fake_wave_request;

    DLLInit(&dll, buffers, funcs);

    TEST_ASSERT_TRUE(ctx, sMemAlloc == fake_alloc);
    TEST_ASSERT_TRUE(ctx, sMemFree == fake_mem_free);
    TEST_ASSERT_TRUE(ctx, sOpenFile == fake_open_file);
    TEST_ASSERT_TRUE(ctx, get_draw_context_module == fake_get_draw_context);
    TEST_ASSERT_TRUE(ctx, hmx_sprite_set_flags_module == fake_sprite_set_flags);
    TEST_ASSERT_TRUE(ctx, g_env_module == (hmx_environment *)&env_storage);
    TEST_ASSERT_TRUE(ctx, g_loader_module == (hmx_environment *)&loader_storage);
    TEST_ASSERT_TRUE(ctx, s_ctx == &draw_ctx);
    TEST_ASSERT_EQ_INT(ctx, 101, hWnd);
    TEST_ASSERT_EQ_INT(ctx, 202, hSurf);
    TEST_ASSERT_TRUE(ctx, lpColorwk == palette);
    TEST_ASSERT_TRUE(ctx, CDPlay == fake_cd_play);
    TEST_ASSERT_TRUE(ctx, CDPause == fake_cd_pause);
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

static void test_dllend_normal_and_hidden_cleanup_paths(test_context *ctx) {
    reset_fixture();

    DLLEnd();
    TEST_ASSERT_EQ_INT(ctx, 1, delete_ea_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_all_char_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sonic_delete_call_count);

    reset_fixture();
    bKakusi = 2;
    DLLEnd();
    TEST_ASSERT_EQ_INT(ctx, 0, delete_ea_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_all_char_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sonic_delete_call_count);
}

static void test_swdata_set_copies_both_words(test_context *ctx) {
    ushort_union sw1;
    ushort_union sw2;

    reset_fixture();
    sw1.w = 0x1357;
    sw2.w = 0x7531;

    SWdataSet(sw1, sw2);

    TEST_ASSERT_EQ_INT(ctx, 0x1357, swData1);
    TEST_ASSERT_EQ_INT(ctx, 0x7531, swData2);
}

static void test_dllmain_dispatches_all_sequences(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    nTimerCunt = 10;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_start_call_count);
    TEST_ASSERT_EQ_INT(ctx, 11, nTimerCunt);
    TEST_ASSERT_EQ_INT(ctx, 1, snd_draw_call_count);

    nSequenceNum = 2;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_menu_call_count);

    nSequenceNum = 6;
    Wait(1);
    nTimerCunt = 2;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 6, nSequenceNum);

    nSequenceNum = 4;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_kakusi_call_count);

    nSequenceNum = 999;
    TEST_ASSERT_EQ_INT(ctx, 9, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_end_call_count);

    nSequenceNum = 5;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, special_mes_call_count);

    nSequenceNum = 6;
    TEST_ASSERT_EQ_INT(ctx, 18, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_end2_call_count);

    nSequenceNum = 7;
    TEST_ASSERT_EQ_INT(ctx, 19, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, snd_end3_call_count);

    nSequenceNum = 1000;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    nSequenceNum = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 10, snd_draw_call_count);
}

static void test_dllmain_wraps_max_timer_to_5184000(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 777;
    nTimerCunt = 0x7FFFFFFF;

    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());

    TEST_ASSERT_EQ_INT(ctx, 5184000, nTimerCunt);
    TEST_ASSERT_EQ_INT(ctx, 1, snd_draw_call_count);
}

TEST_MAIN_BEGIN;
test_sequence_wait_helpers_set_saved_state(&ctx);
test_sndwait_restores_after_timer_and_handles_key_off(&ctx);
test_dllinit_binds_imports_sets_globals_and_initializes(&ctx);
test_dllend_normal_and_hidden_cleanup_paths(&ctx);
test_swdata_set_copies_both_words(&ctx);
test_dllmain_dispatches_all_sequences(&ctx);
test_dllmain_wraps_max_timer_to_5184000(&ctx);
TEST_MAIN_END

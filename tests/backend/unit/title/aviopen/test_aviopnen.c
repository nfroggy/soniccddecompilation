#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/engine_dll.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

static draw_context draw_ctx;
static char env_storage;
static char loader_storage;

static Sint32 read_dib_call_count;
static char *read_dib_file_names[4];
static Sint32 free_dib_call_count;
static Sint32 avi_end_call_count;
static Sint32 avi_paint_call_count;
static Uint32 avi_paint_values[4];
static Sint32 avi_opening_move_call_count;
static Sint32 movie_play_call_count;
static Sint32 movie_stop_call_count;
static Sint32 movie_stat_call_count;
static draw_context *draw_context_return;

#define AVIOPEN
#include "src/title/aviopen/aviopnen.c"
#undef AVIOPEN

void ReadDIB(char *file_name) {
    read_dib_file_names[read_dib_call_count] = file_name;
    ++read_dib_call_count;
}

void FreeDIB(void) { ++free_dib_call_count; }

void AVIEnd(void) { ++avi_end_call_count; }

void AVIPaint(Uint32 hdc) {
    avi_paint_values[avi_paint_call_count] = hdc;
    ++avi_paint_call_count;
}

void AVIOpeningMove(void) { ++avi_opening_move_call_count; }

static Sint32 fake_movie_play(Sint32 movie) {
    (void)movie;
    ++movie_play_call_count;
    return 0;
}

static Sint32 fake_movie_stop(void) {
    ++movie_stop_call_count;
    return 0;
}

static Sint32 fake_movie_stat(void) {
    ++movie_stat_call_count;
    return 0;
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
    memset(read_dib_file_names, 0, sizeof(read_dib_file_names));
    memset(avi_paint_values, 0, sizeof(avi_paint_values));

    g_env_module = 0;
    g_loader_module = 0;
    nTimerCunt = 0;
    nSequenceNum = 0;
    wMCIDeviceID[0] = 0;
    s_ctx = 0;
    get_draw_context_module = 0;
    sMovieStat = 0;
    sMovieStop = 0;
    sMoviePlay = 0;
    hmx_ddagrid_set_tile_module = 0;
    hmx_ddagrid_set_scan_module = 0;
    hmx_ddagrid_release_module = 0;
    hmx_ddagrid_create_module = 0;
    hmx_sprite_set_flags_module = 0;
    hmx_sprite_set_bitmap_module = 0;
    hmx_sprite_set_position_module = 0;
    hmx_sprite_base_module = 0;
    hmx_renderer_context_draw_module = 0;
    hmx_renderer_context_clear_module = 0;
    hmx_renderer_context_add_module = 0;
    hmx_grid_set_horz_offsets_module = 0;
    hmx_grid_set_tile_module = 0;
    hmx_grid_set_view_module = 0;
    hmx_grid_set_position_module = 0;
    hmx_grid_base_module = 0;
    hmx_grid_release_module = 0;
    hmx_grid_create_module = 0;
    hmx_free_module = 0;
    hmx_bitmap_set_transparency_module = 0;
    hmx_bitmap_get_scan0_module = 0;
    hmx_bitmap_release_module = 0;
    hmx_bitmap_create_module = 0;
    hmx_background_set_background_module = 0;
    ld_load_cmpfile_module = 0;
    ld_bitmap_4to8_module = 0;
    FlipToScreen_module = 0;
    sGetFileSize = 0;
    sCloseFile = 0;
    sReadFile = 0;
    sOpenFile = 0;
    sOutputDebugString = 0;
    sPrintf = 0;
    sMemCpy = 0;
    sMemFree = 0;
    sMemAlloc = 0;
    swData2 = 0;
    swData1 = 0;
    WriteIndx = 0;
    ReadIndx = 0;
    CreateScore = 0;
    CloseScore = 0;
    OpenScore = 0;
    WriteScore = 0;
    ReadScore = 0;
    WaveRequest = 0;
    CDPause = 0;
    CDPlay = 0;
    lpbFullScreen = 0;
    hWnd = 0;

    read_dib_call_count = 0;
    free_dib_call_count = 0;
    avi_end_call_count = 0;
    avi_paint_call_count = 0;
    avi_opening_move_call_count = 0;
    movie_play_call_count = 0;
    movie_stop_call_count = 0;
    movie_stat_call_count = 0;
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
    funcs[55] = fake_movie_play;
    funcs[56] = fake_movie_stop;
    funcs[57] = fake_movie_stat;
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

static void test_oeset_sequence_sets_timer_to_minus_one(test_context *ctx) {
    reset_fixture();

    nTimerCunt = 44;
    OESetSequenceNum(7);

    TEST_ASSERT_EQ_INT(ctx, 7, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, -1, nTimerCunt);
}

static void test_dllinit_binds_imports_loads_dib_and_initializes_state(
    test_context *ctx) {
    engine_dll dll;
    void *funcs[64];
    char **buffers[16];
    char *env_slot;
    char *loader_slot;
    Uint32 full_screen = 1;

    reset_fixture();
    memset(&dll, 0, sizeof(dll));
    memset(funcs, 0, sizeof(funcs));
    fill_func_table(funcs);
    fill_buffer_table(buffers, &env_slot, &loader_slot);
    dll.hWnd = 707;
    dll.lpbFullScreen = &full_screen;

    DLLInit(&dll, buffers, funcs);

    TEST_ASSERT_TRUE(ctx, sMemAlloc == fake_alloc);
    TEST_ASSERT_TRUE(ctx, sMemFree == fake_mem_free);
    TEST_ASSERT_TRUE(ctx, sOpenFile == fake_open_file);
    TEST_ASSERT_TRUE(ctx, get_draw_context_module == fake_get_draw_context);
    TEST_ASSERT_TRUE(ctx, hmx_sprite_set_flags_module == fake_sprite_set_flags);
    TEST_ASSERT_TRUE(ctx, sMoviePlay == fake_movie_play);
    TEST_ASSERT_TRUE(ctx, sMovieStop == fake_movie_stop);
    TEST_ASSERT_TRUE(ctx, sMovieStat == fake_movie_stat);
    TEST_ASSERT_TRUE(ctx, g_env_module == (hmx_environment *)&env_storage);
    TEST_ASSERT_TRUE(ctx, g_loader_module == (hmx_environment *)&loader_storage);
    TEST_ASSERT_TRUE(ctx, s_ctx == &draw_ctx);
    TEST_ASSERT_EQ_INT(ctx, 1, read_dib_call_count);
    TEST_ASSERT_TRUE(ctx, read_dib_file_names[0] == fileName);
    TEST_ASSERT_EQ_INT(ctx, 1, nSequenceNum);
    TEST_ASSERT_EQ_INT(ctx, 0, nTimerCunt);
    TEST_ASSERT_EQ_INT(ctx, 65535, wMCIDeviceID[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hWnd);
    TEST_ASSERT_TRUE(ctx, lpbFullScreen == 0);
}

static void test_dllend_paint_and_misc_exports(test_context *ctx) {
    reset_fixture();

    DLLPaint(1234);
    TEST_ASSERT_EQ_INT(ctx, 1, avi_paint_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1234, avi_paint_values[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, DLLNotify(55, 66));
    DLLAVISizeChange();
    DLLAVIRealize();
    AVIPause();
    AVIResume();
    TEST_ASSERT_EQ_INT(ctx, 0, free_dib_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, avi_end_call_count);

    DLLEnd();
    TEST_ASSERT_EQ_INT(ctx, 1, free_dib_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, avi_end_call_count);
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

static void test_dllmain_dispatches_opening_and_skip_input(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 1;
    nTimerCunt = 10;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, avi_opening_move_call_count);
    TEST_ASSERT_EQ_INT(ctx, 11, nTimerCunt);

    nSequenceNum = 0;
    swData1 = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, avi_opening_move_call_count);

    nSequenceNum = 2;
    swData1 = 16;
    TEST_ASSERT_EQ_INT(ctx, 9, DLLMain());
    TEST_ASSERT_EQ_INT(ctx, 1, avi_opening_move_call_count);
}

static void test_dllmain_wraps_max_timer_to_5184000(test_context *ctx) {
    reset_fixture();

    nSequenceNum = 0;
    nTimerCunt = 0x7FFFFFFF;

    TEST_ASSERT_EQ_INT(ctx, 0, DLLMain());

    TEST_ASSERT_EQ_INT(ctx, 5184000, nTimerCunt);
}

TEST_MAIN_BEGIN;
test_oeset_sequence_sets_timer_to_minus_one(&ctx);
test_dllinit_binds_imports_loads_dib_and_initializes_state(&ctx);
test_dllend_paint_and_misc_exports(&ctx);
test_swdata_set_copies_both_words(&ctx);
test_dllmain_dispatches_opening_and_skip_input(&ctx);
test_dllmain_wraps_max_timer_to_5184000(&ctx);
TEST_MAIN_END

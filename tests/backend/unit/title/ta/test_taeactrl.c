#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"
#include "src/title/common/hmx_types.h"
#include "src/title/common/score_data_types.h"

static draw_context draw_ctx;
draw_context *s_ctx = &draw_ctx;
hmx_environment *g_env_module = (hmx_environment *)0x1000;
hmx_environment *g_loader_module = (hmx_environment *)0x2000;
score_data score;
score_data *lpScoreData = &score;
Sint32 gMenuRound;
Sint32 gMenu1;
Sint32 gNewMenu2;
Sint32 gMenuZone;
Sint32 gMenu2;
Sint32 gRankY;
Sint32 gRankX;
Uint32 gTimer;
Sint32 gMove;
Sint32 gNewRankX;
Sint32 gNewMenuZone;
Sint32 gNewMenuRound;
Sint32 gNewMenu1;

void (*FlipToScreen_module)(void);
void (*hmx_renderer_context_draw_module)(hmx_renderer_context *, hmx_surface *);
void (*hmx_renderer_context_add_module)(hmx_renderer_context *, Sint32,
                                        hmx_renderer_base *);
hmx_renderer_base *(*hmx_sprite_base_module)(hmx_sprite *);
hmx_renderer_base *(*hmx_grid_base_module)(hmx_grid *);
void (*hmx_renderer_context_clear_module)(hmx_renderer_context *);
void (*hmx_free_module)(hmx_environment *, void *);
void (*hmx_bitmap_set_transparency_module)(hmx_bitmap *, Sint32);
void (*ld_bitmap_4to8_module)(void *, void *, Sint32, Sint32, Sint32, Sint32,
                              Sint32);
void *(*hmx_bitmap_get_scan0_module)(hmx_bitmap *);
hmx_bitmap *(*hmx_bitmap_create_module)(hmx_environment *, Sint32, Sint32);
void *(*ld_load_cmpfile_module)(hmx_environment *, char *);
void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32, Sint32);
void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32, Sint32,
                                    Sint32);
void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);
void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32, hmx_bitmap *,
                                 Sint32);
void (*hmx_background_set_background_module)(hmx_background *, Sint32);
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
void (*hmx_bitmap_release_module)(hmx_environment *, hmx_bitmap *);

void (*CDPause)(Sint16);
void (*CDPlay)(Sint16);
void (*ChangeTileBmp)(Sint32, Sint32);
void (*sCloseFile)(Sint32);
void (*ClrSpriteDebug)(void);
void (*EAsprset)(Sint16, Sint16, Uint16, Uint16, Uint16);
Sint32 (*sGetFileSize)(Sint32);
void *(*sMemAlloc)(Sint32);
Sint32 (*sMemCmp)(void *, void *, Sint32);
void (*sMemCpy)(void *, void *, Sint32);
void (*sMemFree)(void *);
void (*sMemSet)(void *, Uint8, Sint32);
Sint32 (*sReadFile)(Sint32, void *, Sint32);
Sint32 (*ReadIndx)(Uint32);
Uint32 (*ReadScore)(Sint32, char *, Uint32);
Sint32 (*sOpenFile)(char *);
Uint32 (*OpenScore)(Sint32);
void (*sOutputDebugString)(char *);
void (*sPrintf)(char *, const char *, ...);
Sint32 (*sRandom)(void);
Sint32 (*SetGrid)(Sint32, Sint32, Sint32, Sint32, Sint32);
char *(*sStrcpy)(char *, char *);
void (*WaveAllStop)(void);
void (*WaveRequest)(Sint16);
Uint32 (*WriteIndx)(Sint32, Uint32);
Uint32 (*WriteScore)(Sint32, char *, Uint32);

#include "src/title/ta/taeactrl.c"

typedef struct {
    hmx_grid *grid;
    Sint32 x;
    Sint32 y;
    hmx_bitmap *bitmap;
    Sint32 tile;
} tile_call;

typedef struct {
    hmx_grid *grid;
    Sint32 x;
    Sint32 y;
    Sint32 w;
    Sint32 h;
} view_call;

typedef struct {
    hmx_sprite *sprite;
    Sint32 x;
    Sint32 y;
} sprite_pos_call;

static hmx_sprite *sprites[256];
static hmx_sprite *sprite_handles[256];
static hmx_grid *grid_handles[16];
static hmx_bitmap *bitmap_handles[4096];
static unsigned char bitmap_scan[4096][16];
static tile_call tile_calls[4096];
static view_call view_calls[256];
static sprite_pos_call sprite_pos_calls[256];
static Sint32 renderer_add_levels[512];
static int tile_call_count;
static int view_call_count;
static int sprite_pos_call_count;
static int grid_create_count;
static int grid_release_count;
static int bitmap_create_count;
static int bitmap_release_count;
static int bitmap_transparency_count;
static int renderer_clear_count;
static int renderer_draw_count;
static int renderer_add_count;
static int flip_count;
static int background_set_count;
static Sint32 last_background_color;
static int free_count;
static int mem_alloc_count;
static int mem_free_count;
static int open_file_result;
static int open_file_count;
static int read_file_count;
static int close_file_count;
static char debug_message[512];
static void *cmpfile_result;
static void *last_freed_cmpfile;
static Uint16 file_words[4096];
static int tapic_color_calls;
static Sint32 last_tapic_mode;
static Sint32 last_tapic_round;
static int mem_alloc_fail_on;

void TAPicColorSet(Sint32 mode, Sint32 round) {
    ++tapic_color_calls;
    last_tapic_mode = mode;
    last_tapic_round = round;
}

static void stub_printf(char *dst, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vsprintf(dst, fmt, args);
    va_end(args);
}

static void stub_output_debug_string(char *message) {
    strncpy(debug_message, message, sizeof(debug_message) - 1);
    debug_message[sizeof(debug_message) - 1] = 0;
}

static void stub_flip(void) { ++flip_count; }

static void stub_renderer_clear(hmx_renderer_context *ctx) {
    (void)ctx;
    ++renderer_clear_count;
}

static void stub_renderer_draw(hmx_renderer_context *ctx, hmx_surface *surface) {
    (void)ctx;
    (void)surface;
    ++renderer_draw_count;
}

static void stub_renderer_add(hmx_renderer_context *ctx, Sint32 level,
                              hmx_renderer_base *base) {
    (void)ctx;
    (void)base;
    renderer_add_levels[renderer_add_count++] = level;
}

static hmx_renderer_base *stub_sprite_base(hmx_sprite *sprite) {
    return (hmx_renderer_base *)sprite;
}

static hmx_renderer_base *stub_grid_base(hmx_grid *grid) {
    return (hmx_renderer_base *)grid;
}

static void stub_hmx_free(hmx_environment *env, void *mem) {
    (void)env;
    ++free_count;
    last_freed_cmpfile = mem;
}

static hmx_bitmap *stub_bitmap_create(hmx_environment *env, Sint32 w,
                                      Sint32 h) {
    (void)env;
    (void)w;
    (void)h;
    return bitmap_handles[bitmap_create_count++];
}

static void stub_bitmap_release(hmx_environment *env, hmx_bitmap *bitmap) {
    (void)env;
    (void)bitmap;
    ++bitmap_release_count;
}

static void *stub_bitmap_get_scan0(hmx_bitmap *bitmap) {
    int i;

    for (i = 0; i < 4096; ++i) {
        if (bitmap_handles[i] == bitmap)
            return bitmap_scan[i];
    }
    return bitmap_scan[0];
}

static void stub_bitmap_set_transparency(hmx_bitmap *bitmap, Sint32 value) {
    (void)bitmap;
    (void)value;
    ++bitmap_transparency_count;
}

static void stub_ld_bitmap_4to8(void *dst, void *src, Sint32 wx, Sint32 wy,
                                Sint32 palet, Sint32 a, Sint32 b) {
    unsigned char *out = dst;

    (void)src;
    (void)wx;
    (void)wy;
    (void)a;
    (void)b;
    out[0] = (unsigned char)palet;
}

static void *stub_load_cmpfile(hmx_environment *env, char *path) {
    (void)env;
    (void)path;
    return cmpfile_result;
}

static hmx_grid *stub_grid_create(hmx_environment *env, Sint32 a, Sint32 b,
                                  Sint32 c, Sint32 d) {
    (void)env;
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    return grid_handles[grid_create_count++];
}

static void stub_grid_release(hmx_environment *env, hmx_grid *grid) {
    (void)env;
    (void)grid;
    ++grid_release_count;
}

static void stub_grid_set_position(hmx_grid *grid, Sint32 x, Sint32 y) {
    (void)grid;
    (void)x;
    (void)y;
}

static void stub_grid_set_view(hmx_grid *grid, Sint32 x, Sint32 y, Sint32 w,
                               Sint32 h) {
    view_calls[view_call_count].grid = grid;
    view_calls[view_call_count].x = x;
    view_calls[view_call_count].y = y;
    view_calls[view_call_count].w = w;
    view_calls[view_call_count].h = h;
    ++view_call_count;
}

static void stub_grid_set_tile(hmx_grid *grid, Sint32 x, Sint32 y,
                               hmx_bitmap *bitmap, Sint32 tile) {
    tile_calls[tile_call_count].grid = grid;
    tile_calls[tile_call_count].x = x;
    tile_calls[tile_call_count].y = y;
    tile_calls[tile_call_count].bitmap = bitmap;
    tile_calls[tile_call_count].tile = tile;
    ++tile_call_count;
}

static void stub_background_set(hmx_background *background, Sint32 color) {
    (void)background;
    ++background_set_count;
    last_background_color = color;
}

static void stub_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bitmap) {
    (void)sprite;
    (void)bitmap;
}

static void stub_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    sprite_pos_calls[sprite_pos_call_count].sprite = sprite;
    sprite_pos_calls[sprite_pos_call_count].x = x;
    sprite_pos_calls[sprite_pos_call_count].y = y;
    ++sprite_pos_call_count;
}

static void *stub_mem_alloc(Sint32 size) {
    ++mem_alloc_count;
    if (mem_alloc_fail_on != 0 && mem_alloc_count == mem_alloc_fail_on)
        return 0;
    return calloc(1, (size_t)size);
}

static void stub_mem_free(void *mem) {
    ++mem_free_count;
    free(mem);
}

static Sint32 stub_open_file(char *path) {
    (void)path;
    ++open_file_count;
    return open_file_result;
}

static Sint32 stub_read_file(Sint32 file, void *buffer, Sint32 size) {
    (void)file;
    ++read_file_count;
    memcpy(buffer, file_words, (size_t)size);
    return size;
}

static void stub_close_file(Sint32 file) {
    (void)file;
    ++close_file_count;
}

static void reset_state(void) {
    int i;

    memset(&draw_ctx, 0, sizeof(draw_ctx));
    memset(&score, 0, sizeof(score));
    memset(bitmap_scan, 0, sizeof(bitmap_scan));
    memset(tile_calls, 0, sizeof(tile_calls));
    memset(view_calls, 0, sizeof(view_calls));
    memset(sprite_pos_calls, 0, sizeof(sprite_pos_calls));
    memset(renderer_add_levels, 0, sizeof(renderer_add_levels));
    memset(file_words, 0, sizeof(file_words));
    memset(debug_message, 0, sizeof(debug_message));

    for (i = 0; i < 256; ++i) {
        sprite_handles[i] = (hmx_sprite *)(size_t)(0x3000 + i);
        sprites[i] = sprite_handles[i];
    }
    for (i = 0; i < 16; ++i) {
        grid_handles[i] = (hmx_grid *)(size_t)(0x4000 + i);
    }
    for (i = 0; i < 4096; ++i) {
        bitmap_handles[i] = (hmx_bitmap *)(size_t)(0x5000 + i);
        draw_ctx.grd_bitmaps[i] = bitmap_handles[i];
    }
    draw_ctx.sprites = sprites;
    draw_ctx.context = (hmx_renderer_context *)0x6000;
    draw_ctx.screen = (hmx_surface *)0x7000;
    draw_ctx.background = (hmx_background *)0x8000;
    for (i = 0; i < 8; ++i) {
        draw_ctx.grids[i] = grid_handles[i];
    }
    for (i = 0; i < 2; ++i) {
        draw_ctx.spr_bitmaps[i] = bitmap_handles[3000 + i];
    }

    FlipToScreen_module = stub_flip;
    hmx_renderer_context_draw_module = stub_renderer_draw;
    hmx_renderer_context_add_module = stub_renderer_add;
    hmx_sprite_base_module = stub_sprite_base;
    hmx_grid_base_module = stub_grid_base;
    hmx_renderer_context_clear_module = stub_renderer_clear;
    hmx_free_module = stub_hmx_free;
    hmx_bitmap_set_transparency_module = stub_bitmap_set_transparency;
    ld_bitmap_4to8_module = stub_ld_bitmap_4to8;
    hmx_bitmap_get_scan0_module = stub_bitmap_get_scan0;
    hmx_bitmap_create_module = stub_bitmap_create;
    ld_load_cmpfile_module = stub_load_cmpfile;
    hmx_grid_set_view_module = stub_grid_set_view;
    hmx_grid_set_position_module = stub_grid_set_position;
    hmx_grid_create_module = stub_grid_create;
    hmx_grid_release_module = stub_grid_release;
    hmx_grid_set_tile_module = stub_grid_set_tile;
    hmx_background_set_background_module = stub_background_set;
    hmx_sprite_set_bitmap_module = stub_sprite_set_bitmap;
    hmx_sprite_set_position_module = stub_sprite_set_position;
    hmx_bitmap_release_module = stub_bitmap_release;
    sPrintf = stub_printf;
    sOutputDebugString = stub_output_debug_string;
    sMemAlloc = stub_mem_alloc;
    sMemFree = stub_mem_free;
    sOpenFile = stub_open_file;
    sReadFile = stub_read_file;
    sCloseFile = stub_close_file;

    tile_call_count = 0;
    view_call_count = 0;
    sprite_pos_call_count = 0;
    grid_create_count = 0;
    grid_release_count = 0;
    bitmap_create_count = 0;
    bitmap_release_count = 0;
    bitmap_transparency_count = 0;
    renderer_clear_count = 0;
    renderer_draw_count = 0;
    renderer_add_count = 0;
    flip_count = 0;
    background_set_count = 0;
    last_background_color = -1;
    free_count = 0;
    mem_alloc_count = 0;
    mem_free_count = 0;
    open_file_result = 5;
    open_file_count = 0;
    read_file_count = 0;
    close_file_count = 0;
    cmpfile_result = 0;
    last_freed_cmpfile = 0;
    tapic_color_calls = 0;
    last_tapic_mode = -1;
    last_tapic_round = -1;
    mem_alloc_fail_on = 0;

    TileCnt = 768;
    gMenuRound = 1;
    gMenu1 = 0;
    gNewMenu2 = 0;
    gMenuZone = 0;
    gMenu2 = 0;
    gRankY = 0;
    gRankX = 0;
    gTimer = 0;
    gMove = 0;
    gNewRankX = 0;
    gNewMenuZone = 0;
    gNewMenuRound = 0;
    gNewMenu1 = 0;
    gZure = 0;
    for (i = 0; i < 8; ++i) {
        GridRect[i].x = 0;
        GridRect[i].y = 0;
        GridRect[i].dx = 512;
        GridRect[i].dy = 256;
        hGrid[i] = (Uint32)(size_t)grid_handles[i];
    }
}

static void make_scroll_cmp(unsigned char *raw, Sint32 count) {
    ld_scroll_header *header = (ld_scroll_header *)raw;
    int i;

    memset(raw, 0, 512);
    header->header.count = count;
    header->header.offset = sizeof(ld_pack_header) + 4 * sizeof(Sint16) +
                            count * sizeof(ld_bitmap_inf);
    header->plt[0] = 2;
    header->plt[1] = 2;
    for (i = 0; i < count; ++i) {
        header->bmp[i].wx = 2;
        header->bmp[i].wy = 2;
    }
}

static void allocate_pic_buffers(void) {
    int mode;
    int round;

    for (mode = 0; mode < 2; ++mode) {
        for (round = 0; round < 8; ++round) {
            if (mode == 1 && round == 0) {
                lpPicBits4[mode][round] = lpPicBits4[0][0];
            } else {
                lpPicBits4[mode][round] = calloc(1, 9216);
            }
        }
    }
    lpPicBits8 = calloc(1, 18432);
}

static void free_pic_buffers(void) {
    int mode;
    int round;

    lpPicBits4[1][0] = 0;
    for (mode = 0; mode < 2; ++mode) {
        for (round = 0; round < 8; ++round) {
            free(lpPicBits4[mode][round]);
            lpPicBits4[mode][round] = 0;
        }
    }
    free(lpPicBits8);
    lpPicBits8 = 0;
}

static void test_error_messages_name_known_error_codes(test_context *ctx) {
    reset_state();

    EAError(3, 77, "thing");

    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_BAD_POINTER") != 0);
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "77") != 0);
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "thing") != 0);

    EAError(1, 1, "a");
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_FAILED") != 0);
    EAError(2, 2, "b");
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_OUT_OF_MEMORY") != 0);
    EAError(4, 4, "d");
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_OUT_OF_RANGE") != 0);
    EAError(5, 5, "e");
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_INVALID_SIZE") != 0);
    EAError(6, 6, "f");
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "EA_BAD_TYPE") != 0);
}

static void test_srfdraw_adds_active_grids_and_sprites_then_flips(
    test_context *ctx) {
    reset_state();
    memset(hGrid, 0, sizeof(hGrid));
    hGrid[0] = (Uint32)(size_t)grid_handles[0];
    hGrid[2] = (Uint32)(size_t)grid_handles[2];
    draw_ctx.spr_level[1] = 12;
    draw_ctx.spr_level[4] = 7;

    srfDraw();

    TEST_ASSERT_EQ_INT(ctx, 1, renderer_clear_count);
    TEST_ASSERT_EQ_INT(ctx, 4, renderer_add_count);
    TEST_ASSERT_EQ_INT(ctx, 8, renderer_add_levels[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, renderer_add_levels[1]);
    TEST_ASSERT_EQ_INT(ctx, 7, renderer_add_levels[2]);
    TEST_ASSERT_EQ_INT(ctx, 12, renderer_add_levels[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, renderer_draw_count);
    TEST_ASSERT_EQ_INT(ctx, 1, flip_count);

    reset_state();
    draw_ctx.context = 0;
    srfDraw();
    TEST_ASSERT_EQ_INT(ctx, 0, renderer_clear_count);
    TEST_ASSERT_EQ_INT(ctx, 1, flip_count);
}

static void test_load_grid_module_decodes_tiles_and_handles_missing_file(
    test_context *ctx) {
    unsigned char raw[128];
    ld_scroll_header *header = (ld_scroll_header *)raw;
    hmx_bitmap *out[4];

    reset_state();
    memset(raw, 0, sizeof(raw));
    header->header.count = 2;
    header->header.offset = sizeof(ld_pack_header) + 4 * sizeof(Sint16) +
                            2 * sizeof(ld_bitmap_inf);
    header->plt[0] = 1;
    header->plt[1] = 1;
    header->bmp[0].wx = 2;
    header->bmp[0].wy = 2;
    header->bmp[1].wx = 4;
    header->bmp[1].wy = 2;
    cmpfile_result = raw;
    out[0] = (hmx_bitmap *)1;
    out[1] = (hmx_bitmap *)1;
    out[2] = (hmx_bitmap *)1;
    out[3] = (hmx_bitmap *)1;

    TEST_ASSERT_EQ_INT(ctx, 2,
                       ld_load_grid_module(g_loader_module, "x", g_env_module,
                                           out, 4));

    TEST_ASSERT_TRUE(ctx, out[0] == bitmap_handles[0]);
    TEST_ASSERT_TRUE(ctx, out[1] == bitmap_handles[1]);
    TEST_ASSERT_TRUE(ctx, out[2] == 0);
    TEST_ASSERT_TRUE(ctx, out[3] == 0);
    TEST_ASSERT_EQ_INT(ctx, 2, bitmap_create_count);
    TEST_ASSERT_EQ_INT(ctx, 2, bitmap_transparency_count);
    TEST_ASSERT_EQ_INT(ctx, 1, free_count);
    TEST_ASSERT_TRUE(ctx, last_freed_cmpfile == raw);
    TEST_ASSERT_EQ_INT(ctx, 16, bitmap_scan[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 32, bitmap_scan[1][0]);

    reset_state();
    out[0] = (hmx_bitmap *)1;
    TEST_ASSERT_EQ_INT(ctx, -1,
                       ld_load_grid_module(g_loader_module, "x", g_env_module,
                                           out, 1));
    TEST_ASSERT_TRUE(ctx, out[0] == 0);

    reset_state();
    memset(raw, 0, sizeof(raw));
    header->header.count = 6;
    header->header.offset = sizeof(ld_pack_header) + 4 * sizeof(Sint16) +
                            6 * sizeof(ld_bitmap_inf);
    header->plt[0] = 1;
    header->plt[1] = 1;
    header->plt[2] = 1;
    header->plt[3] = 1;
    for (int i = 0; i < 6; ++i) {
        header->bmp[i].wx = 2;
        header->bmp[i].wy = 2;
    }
    cmpfile_result = raw;
    TEST_ASSERT_EQ_INT(ctx, 4,
                       ld_load_grid_module(g_loader_module, "x", g_env_module,
                                           out, 4));
    TEST_ASSERT_EQ_INT(ctx, 48, bitmap_scan[2][0]);
    TEST_ASSERT_EQ_INT(ctx, 64, bitmap_scan[3][0]);
}

static void test_grid_and_background_creation_callbacks(test_context *ctx) {
    reset_state();
    GridRect[0].dx = 512;
    GridRect[0].dy = 256;
    GridRect[1].dx = 0;
    GridRect[2].dx = 144;
    GridRect[2].dy = 224;

    TEST_ASSERT_EQ_INT(ctx, 0, TAGridCreate());
    TEST_ASSERT_EQ_INT(ctx, 3, grid_release_count);
    TEST_ASSERT_EQ_INT(ctx, 7, grid_create_count);
    TEST_ASSERT_TRUE(ctx, hGrid[0] != 0);
    TEST_ASSERT_TRUE(ctx, hGrid[2] != 0);

    TEST_ASSERT_EQ_INT(ctx, 0, BkgCreate_TA());
    TEST_ASSERT_EQ_INT(ctx, 1, background_set_count);
    TEST_ASSERT_EQ_INT(ctx, 16, last_background_color);
    TEST_ASSERT_EQ_INT(ctx, 0, BkgBmpCreate_TA());
    TEST_ASSERT_EQ_INT(ctx, 0, BkgBmpDelete_TA());
    TEST_ASSERT_EQ_INT(ctx, 0, BkgDelete_TA());
}

static void test_grid_bitmap_create_and_init_paths(test_context *ctx) {
    unsigned char raw[512];

    reset_state();
    make_scroll_cmp(raw, 3);
    cmpfile_result = raw;

    TEST_ASSERT_EQ_INT(ctx, 0, TAGridBmpCreate());
    TEST_ASSERT_EQ_INT(ctx, 3, draw_ctx.grd_bitmap_count);
    TEST_ASSERT_EQ_INT(ctx, 3, TileCnt);

    reset_state();
    cmpfile_result = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, TAGridBmpCreate());

    reset_state();
    TileCnt = 768;
    TEST_ASSERT_EQ_INT(ctx, 0, TAGridInit());
    TEST_ASSERT_EQ_INT(ctx, 3, open_file_count);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 1000);

    reset_state();
    open_file_result = -1;
    TEST_ASSERT_EQ_INT(ctx, 0, TAGridInit());
    TEST_ASSERT_EQ_INT(ctx, 1, open_file_count);
}

static void test_grid_init_sub_reads_tiles_and_reports_errors(test_context *ctx) {
    map_info info = {3, 1, 2, 2, 2, 10, 1, "MAP.BIN"};

    reset_state();
    file_words[0] = 0;
    file_words[1] = 1;
    file_words[2] = 2000;
    file_words[3] = 2;

    TEST_ASSERT_EQ_INT(ctx, 0, GridInitSub("P\\", &info));

    TEST_ASSERT_EQ_INT(ctx, 1, open_file_count);
    TEST_ASSERT_EQ_INT(ctx, 1, read_file_count);
    TEST_ASSERT_EQ_INT(ctx, 1, close_file_count);
    TEST_ASSERT_EQ_INT(ctx, 1, mem_alloc_count);
    TEST_ASSERT_EQ_INT(ctx, 1, mem_free_count);
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 3);
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "Out of Tile Data") != 0);

    reset_state();
    open_file_result = -1;
    TEST_ASSERT_EQ_INT(ctx, -1, GridInitSub("P\\", &info));
    TEST_ASSERT_TRUE(ctx, strstr(debug_message, "Read Error") != 0);
    TEST_ASSERT_EQ_INT(ctx, 1, mem_free_count);
}

static void test_font_and_time_formatting_helpers(test_context *ctx) {
    char buf[9];

    TEST_ASSERT_EQ_INT(ctx, 527, GetLFont(39));
    TEST_ASSERT_EQ_INT(ctx, 529, GetLFont(32));
    TEST_ASSERT_EQ_INT(ctx, 525, GetLFont(34));
    TEST_ASSERT_EQ_INT(ctx, 453, GetLFont('0'));
    TEST_ASSERT_EQ_INT(ctx, 473, GetLFont('A'));
    TEST_ASSERT_EQ_INT(ctx, 0, GetLFont('a'));

    TEST_ASSERT_EQ_INT(ctx, 698, GetSFont(39));
    TEST_ASSERT_EQ_INT(ctx, 696, GetSFont(34));
    TEST_ASSERT_EQ_INT(ctx, 700, GetSFont(32));
    TEST_ASSERT_EQ_INT(ctx, 676, GetSFont('0'));
    TEST_ASSERT_EQ_INT(ctx, 0, GetSFont('A'));

    TimeToAsc8(3723, buf);
    TEST_ASSERT_EQ_INT(ctx, '0', buf[0]);
    TEST_ASSERT_EQ_INT(ctx, '1', buf[1]);
    TEST_ASSERT_EQ_INT(ctx, 39, buf[2]);
    TEST_ASSERT_EQ_INT(ctx, '0', buf[3]);
    TEST_ASSERT_EQ_INT(ctx, '2', buf[4]);
    TEST_ASSERT_EQ_INT(ctx, 34, buf[5]);
    TEST_ASSERT_EQ_INT(ctx, '0', buf[6]);
    TEST_ASSERT_EQ_INT(ctx, '5', buf[7]);

    TimeToAsc8(3600 * 120, buf);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[0]);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[1]);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[3]);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[7]);

    TimeToAsc9(3600 * 12 + 61, buf);
    TEST_ASSERT_EQ_INT(ctx, '0', buf[0]);
    TEST_ASSERT_EQ_INT(ctx, '1', buf[1]);
    TEST_ASSERT_EQ_INT(ctx, '2', buf[2]);
    TEST_ASSERT_EQ_INT(ctx, '0', buf[4]);
    TEST_ASSERT_EQ_INT(ctx, '1', buf[5]);

    TimeToAsc9(3600 * 1200, buf);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[0]);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[1]);
    TEST_ASSERT_EQ_INT(ctx, '9', buf[2]);
}

static void test_picture_loading_creation_and_scroll_copy(test_context *ctx) {
    unsigned char *raw;
    bmp_header *header;
    int i;

    reset_state();
    raw = calloc(1, sizeof(bmp_header) + 9216 * 15);
    header = (bmp_header *)raw;
    header->dwOff = sizeof(bmp_header);
    for (i = 0; i < 9216 * 15; ++i) {
        raw[sizeof(bmp_header) + i] = (unsigned char)(i + 1);
    }
    allocate_pic_buffers();
    cmpfile_result = raw;

    TEST_ASSERT_EQ_INT(ctx, 0, LoadPicBmp());
    TEST_ASSERT_EQ_INT(ctx, 1, lpPicBits4[0][0][0]);
    TEST_ASSERT_EQ_INT(ctx, 1, lpPicBits4[1][0][0]);
    TEST_ASSERT_EQ_INT(ctx, 1, free_count);

    free_pic_buffers();
    free(raw);

    reset_state();
    TEST_ASSERT_EQ_INT(ctx, 1, CreatePic());
    TEST_ASSERT_TRUE(ctx, lpPicBits4[1][0] == lpPicBits4[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 2, bitmap_create_count);
    TEST_ASSERT_EQ_INT(ctx, 2, sprite_pos_call_count);
    DeleteEA();
    TEST_ASSERT_TRUE(ctx, mem_free_count >= 15);

    reset_state();
    mem_alloc_fail_on = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, CreatePic());

    reset_state();
    mem_alloc_fail_on = 16;
    TEST_ASSERT_EQ_INT(ctx, 0, CreatePic());

    reset_state();
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    lpPicBits4[0][0][0] = 0x12;
    lpPicBits4[0][0][1] = 0x34;
    SetPic(0, 0, 1);
    TEST_ASSERT_EQ_INT(ctx, 52, bitmap_scan[1][0]);
    TEST_ASSERT_EQ_INT(ctx, 49, bitmap_scan[1][1]);
    free_pic_buffers();
}

static void test_ta_eacreate_builds_assets_and_menu_overlay(test_context *ctx) {
    static score_data padded_scores[3];

    reset_state();
    memset(padded_scores, 0, sizeof(padded_scores));
    lpScoreData = &padded_scores[1];
    gMenu1 = 0;
    gMenu2 = 3;
    gNewMenu2 = 2;
    gMenuRound = 1;
    gMenuZone = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, TA_EACreate());

    TEST_ASSERT_EQ_INT(ctx, 1, background_set_count);
    TEST_ASSERT_TRUE(ctx, grid_create_count > 0);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 2000);
    TEST_ASSERT_EQ_INT(ctx, 1, tapic_color_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, last_tapic_mode);
    TEST_ASSERT_EQ_INT(ctx, 1, last_tapic_round);
    TEST_ASSERT_EQ_INT(ctx, 0, GridRect[2].x);

    DeleteEA();

    reset_state();
    memset(padded_scores, 0, sizeof(padded_scores));
    lpScoreData = &padded_scores[1];
    gMenu1 = 1;
    gMenu2 = 3;
    gNewMenu2 = 1;
    gMenuRound = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, TA_EACreate());
    TEST_ASSERT_EQ_INT(ctx, 144, GridRect[2].x);
    TEST_ASSERT_EQ_INT(ctx, 1, last_tapic_mode);

    DeleteEA();
    lpScoreData = &score;
}

static void test_print_helpers_write_expected_tile_regions(test_context *ctx) {
    reset_state();
    score.timeattack[0][0][0].time = 3600;
    score.timeattack[0][1][0].time = 120;
    score.timeattack[0][2][0].time = 60;
    score.special[0][0].time = 60;
    score.timeattack[0][0][0].name[0] = 'A';
    score.timeattack[0][0][0].name[1] = 'B';
    score.timeattack[0][0][0].name[2] = 'C';

    PrintTotalTime(0, 1);
    TEST_ASSERT_EQ_INT(ctx, 18, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 46, tile_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 21, tile_calls[0].y);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == draw_ctx.grd_bitmaps[453]);

    reset_state();
    PrintRoundName(0, 1, 1, 3);
    TEST_ASSERT_EQ_INT(ctx, 36, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 40, tile_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 5, tile_calls[0].y);

    reset_state();
    PrintRoundName(0, 1, 0, 2);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == 0);

    reset_state();
    PrintRoundName(0, 0, 0, 3);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 0);

    reset_state();
    score.special[0][0].time = 120;
    PrintTotalTime(1, 0);
    TEST_ASSERT_EQ_INT(ctx, 18, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 8, tile_calls[0].x);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == draw_ctx.grd_bitmaps[529]);

    reset_state();
    score.special[0][0].time = 120;
    PrintRoundName(1, 1, 1, 3);
    TEST_ASSERT_EQ_INT(ctx, 36, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[0].x);

    reset_state();
    PrintZoneNo(0, 2);
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 31, tile_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 9, tile_calls[0].y);

    reset_state();
    PrintZoneNo(1, 2);
    TEST_ASSERT_EQ_INT(ctx, 30, tile_calls[0].x);

    reset_state();
    PrintRank(0, 1, 1);
    TEST_ASSERT_EQ_INT(ctx, 6, tile_call_count);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == draw_ctx.grd_bitmaps[188]);

    reset_state();
    PrintRank(1, 1, 0);
    TEST_ASSERT_EQ_INT(ctx, 6, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 21, tile_calls[0].x);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == 0);

    reset_state();
    PrintRankName(0, 2, 1, 'A', 1);
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 35, tile_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 21, tile_calls[0].y);

    reset_state();
    PrintRankName(1, 2, 1, 'A', 0);
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 34, tile_calls[0].x);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == draw_ctx.grd_bitmaps[188]);
}

static void test_zone_record_and_menu_printing_modes(test_context *ctx) {
    reset_state();
    score.special[0][0].time = 60;
    score.special[0][0].name[0] = 'D';
    score.special[0][0].name[1] = 'E';
    score.special[0][0].name[2] = 'F';

    PrintZoneRecord(1, 1, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 24, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 24, tile_calls[0].x);

    reset_state();
    score.timeattack[0][0][0].time = 60;
    PrintZoneSelect(0, 1, 0);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 600);
    TEST_ASSERT_EQ_INT(ctx, 22, tile_calls[0].x);

    reset_state();
    PrintZoneSelect(1, 1, 0);
    TEST_ASSERT_TRUE(ctx, tile_call_count > 600);
    TEST_ASSERT_EQ_INT(ctx, 18, tile_calls[0].x);

    reset_state();
    PrintMenu2(0, 1, 0);
    TEST_ASSERT_EQ_INT(ctx, 13, tile_call_count);
    TEST_ASSERT_TRUE(ctx, tile_calls[1].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(0, 2, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[6].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(0, 3, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[11].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(0, 99, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[11].bitmap == draw_ctx.grd_bitmaps[639]);
    TEST_ASSERT_TRUE(ctx, tile_calls[12].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(1, 3, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[3].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(1, 1, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[6].bitmap == draw_ctx.grd_bitmaps[639]);

    reset_state();
    PrintMenu2(1, 99, 0);
    TEST_ASSERT_TRUE(ctx, tile_calls[6].bitmap == draw_ctx.grd_bitmaps[639]);
    TEST_ASSERT_TRUE(ctx, tile_calls[9].bitmap == draw_ctx.grd_bitmaps[639]);
}

static void test_tenmetu_blinks_current_menu_regions(test_context *ctx) {
    reset_state();
    gTimer = 30;
    gMenu2 = 0;
    gMenuRound = 1;
    Tenmetu();
    TEST_ASSERT_TRUE(ctx, tile_call_count > 0);

    reset_state();
    gTimer = 30;
    gMenu2 = 4;
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 6, tile_call_count);

    reset_state();
    gTimer = 40;
    gMenu2 = 0;
    gMenuRound = 1;
    Tenmetu();
    TEST_ASSERT_TRUE(ctx, tile_call_count > 0);

    reset_state();
    gTimer = 40;
    gMenu1 = 0;
    gMenu2 = 5;
    gMenuRound = 1;
    score.timeattack[0][0][0].name[0] = 'Y';
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);

    reset_state();
    gTimer = 40;
    gMenu2 = 4;
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 6, tile_call_count);

    reset_state();
    gTimer = 30;
    gMenu2 = 5;
    gMenuRound = 1;
    score.timeattack[0][0][0].name[0] = 'Z';
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);

    reset_state();
    gTimer = 30;
    gMenu1 = 1;
    gMenu2 = 5;
    gMenuRound = 1;
    score.special[0][0].name[0] = 'Q';
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);

    reset_state();
    gTimer = 40;
    gMenu1 = 1;
    gMenu2 = 5;
    gMenuRound = 1;
    score.special[0][0].name[0] = 'R';
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 2, tile_call_count);

    reset_state();
    gTimer = 30;
    gMenu2 = 2;
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 13, tile_call_count);

    reset_state();
    gTimer = 40;
    gMenu2 = 2;
    Tenmetu();
    TEST_ASSERT_EQ_INT(ctx, 13, tile_call_count);
}

static void test_drawta_updates_sprite_positions_and_grid_views(
    test_context *ctx) {
    reset_state();
    GridRect[2].x = 16;
    GridRect[2].y = 24;
    GridRect[3].x = 32;
    GridRect[3].y = 40;

    DrawTA();

    TEST_ASSERT_EQ_INT(ctx, 2, sprite_pos_call_count);
    TEST_ASSERT_EQ_INT(ctx, 136, sprite_pos_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 136, sprite_pos_calls[0].y);
    TEST_ASSERT_EQ_INT(ctx, 136, sprite_pos_calls[1].x);
    TEST_ASSERT_EQ_INT(ctx, 64, sprite_pos_calls[1].y);
    TEST_ASSERT_TRUE(ctx, view_call_count >= 8);
    TEST_ASSERT_EQ_INT(ctx, -16, view_calls[2].x);
    TEST_ASSERT_EQ_INT(ctx, 576, view_calls[2].h);
    TEST_ASSERT_EQ_INT(ctx, 1, flip_count);
}

static void test_tamove_core_transitions(test_context *ctx) {
    reset_state();
    gMove = 10;
    gMenu2 = 1;
    gNewMenu2 = 2;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gMenu2);

    reset_state();
    gMove = 11;
    gMenu2 = 4;
    gNewMenu2 = 5;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 5, gMenu2);

    reset_state();
    gMove = 12;
    gMenuRound = 1;
    gNewRankX = 2;
    gRankX = 1;
    score.timeattack[0][0][0].name[1] = 'B';
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gRankX);

    reset_state();
    gMove = 1234;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
}

static void test_tamove_scrolling_and_picture_transitions(test_context *ctx) {
    reset_state();
    gMove = 1;
    gNewMenu2 = 2;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].x = 16;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, (gMove & 32768) != 0);

    reset_state();
    gMove = 1 | 32768;
    gNewMenu2 = 2;
    GridRect[2].x = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 504, GridRect[2].x);

    reset_state();
    gMove = 1 | 32768;
    gNewMenu2 = 2;
    GridRect[2].x = 8;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gMenu2);

    reset_state();
    gMove = 2;
    gNewMenu2 = 1;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].x = 16;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, (gMove & 32768) != 0);

    reset_state();
    gMove = 2 | 32768;
    gNewMenu2 = 1;
    GridRect[2].x = 136;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gMenu2);

    reset_state();
    gMove = 3;
    GridRect[2].x = 136;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);

    reset_state();
    gMove = 4;
    GridRect[2].x = 8;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);

    reset_state();
    gMove = 5;
    gNewMenu1 = 1;
    gMenuRound = 1;
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, (gMove & 32768) != 0);
    TEST_ASSERT_EQ_INT(ctx, 1, tapic_color_calls);
    free_pic_buffers();

    reset_state();
    gMove = 5 | 32768;
    gNewMenu1 = 1;
    GridRect[3].x = 8;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gMenu1);

    reset_state();
    gMove = 6;
    gMenu1 = 1;
    gNewMenu1 = 0;
    gMenuRound = 1;
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, (gMove & 32768) != 0);
    TEST_ASSERT_EQ_INT(ctx, 1, tapic_color_calls);
    free_pic_buffers();

    reset_state();
    gMove = 6 | 32768;
    gMenu1 = 1;
    gNewMenu1 = 0;
    GridRect[3].x = 136;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 0, gMenu1);

    reset_state();
    gMove = 6 | 32768;
    gMenu1 = 1;
    gNewMenu1 = 0;
    gMenuRound = 1;
    gZure = 3;
    GridRect[3].x = 8;
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 2, gZure);
    free_pic_buffers();

    reset_state();
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    gMove = 7;
    gNewMenuRound = 2;
    gMenuRound = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, (gMove & 32768) != 0);
    TEST_ASSERT_EQ_INT(ctx, 2, gMenuRound);
    TEST_ASSERT_EQ_INT(ctx, 1, tapic_color_calls);
    while (gMove != 0) {
        TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    }
    TEST_ASSERT_EQ_INT(ctx, 0, gZure);
    free_pic_buffers();

    reset_state();
    gMove = 5 | 32768;
    gNewMenu1 = 1;
    gMenuRound = 1;
    gZure = 3;
    GridRect[3].x = 136;
    allocate_pic_buffers();
    draw_ctx.spr_bitmaps[0] = bitmap_handles[0];
    draw_ctx.spr_bitmaps[1] = bitmap_handles[1];
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 2, gZure);
    free_pic_buffers();

    reset_state();
    gMove = 8;
    gNewMenuZone = 2;
    GridRect[2].y = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 248, GridRect[2].y);

    reset_state();
    gMove = 8;
    gNewMenuZone = 2;
    GridRect[2].y = 8;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 2, gMenuZone);

    reset_state();
    gMove = 8;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 136;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 2);

    reset_state();
    gMove = 8;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 192;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 8;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 240;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 8;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 216;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 9;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 56;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 2);

    reset_state();
    gMove = 9;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 120;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 9;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 144;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 9;
    gNewMenuZone = 1;
    gMenuRound = 1;
    GridRect[2].y = 168;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_TRUE(ctx, tile_call_count >= 24);

    reset_state();
    gMove = 9;
    gNewMenuZone = 1;
    GridRect[2].y = 248;
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);
    TEST_ASSERT_EQ_INT(ctx, 1, gMenuZone);

    reset_state();
    gMove = 12;
    gMenu1 = 1;
    gMenuRound = 1;
    gNewMenu2 = 3;
    score.special[0][0].name[0] = 'S';
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 3, gMenu2);
    TEST_ASSERT_EQ_INT(ctx, 0, gMove);

    reset_state();
    gMove = 12;
    gMenuRound = 1;
    gNewRankX = 0;
    gRankX = 0;
    score.timeattack[0][0][0].name[0] = 'T';
    TEST_ASSERT_EQ_INT(ctx, -1, TAMove());
    TEST_ASSERT_EQ_INT(ctx, 0, gRankX);
}

TEST_MAIN_BEGIN;
test_error_messages_name_known_error_codes(&ctx);
test_srfdraw_adds_active_grids_and_sprites_then_flips(&ctx);
test_load_grid_module_decodes_tiles_and_handles_missing_file(&ctx);
test_grid_and_background_creation_callbacks(&ctx);
test_grid_bitmap_create_and_init_paths(&ctx);
test_grid_init_sub_reads_tiles_and_reports_errors(&ctx);
test_font_and_time_formatting_helpers(&ctx);
test_picture_loading_creation_and_scroll_copy(&ctx);
test_ta_eacreate_builds_assets_and_menu_overlay(&ctx);
test_print_helpers_write_expected_tile_regions(&ctx);
test_zone_record_and_menu_printing_modes(&ctx);
test_tenmetu_blinks_current_menu_regions(&ctx);
test_drawta_updates_sprite_positions_and_grid_views(&ctx);
test_tamove_core_transitions(&ctx);
test_tamove_scrolling_and_picture_transitions(&ctx);
TEST_MAIN_END

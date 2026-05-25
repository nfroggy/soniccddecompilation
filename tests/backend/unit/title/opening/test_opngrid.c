#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

typedef struct {
    hmx_grid *grid;
    Sint32 x;
    Sint32 y;
    hmx_bitmap *bitmap;
    Sint32 flags;
} grid_tile_call;

typedef struct {
    hmx_ddagrid *grid;
    Sint32 x;
    Sint32 y;
    hmx_bitmap *bitmap;
    Sint32 flags;
} dda_tile_call;

static draw_context context;
draw_context *s_ctx;
grid_bmp infoGridBmp[3];
hmx_environment *g_env_module;
void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32, Sint32);
void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32, hmx_bitmap *,
                                 Sint32);
hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32, Sint32,
                                    Sint32);
hmx_ddagrid *(*hmx_ddagrid_create_module)(hmx_environment *, Sint32, Sint32,
                                          Sint32, Sint32, Sint32, Sint32);
void (*hmx_ddagrid_set_tile_module)(hmx_ddagrid *, Sint32, Sint32, hmx_bitmap *,
                                    Sint32);
void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);

static char env_storage;
static char grid_storage[3];
static char bitmap_storage[900];
static Uint32 grid_handles[3];
static Uint32 tile0_handle;
static Uint32 tile1_handles[320];
static Uint32 tile2_handles[512];
static Uint32 *bitmap0_handles[1];
static Uint32 *bitmap1_handles[320];
static Uint32 *bitmap2_handles[512];
static grid_tile_call grid_tile_calls[1120];
static dda_tile_call dda_tile_calls[640];
static Sint32 grid_tile_call_count;
static Sint32 dda_tile_call_count;
static Sint32 grid_create_call_count;
static Sint32 grid_create_args[4];
static Sint32 dda_create_call_count;
static Sint32 dda_create_args[6];
static hmx_environment *create_env;
static Sint32 position_call_count;
static hmx_grid *position_grid;
static Sint32 position_args[2];
static Sint32 view_call_count;
static hmx_grid *view_grid;
static Sint32 view_args[4];
static Sint32 release_call_count;
static hmx_grid *release_grid[4];
static hmx_environment *release_env[4];

#include "src/title/opening/opngrid.c"

static hmx_grid *fake_grid_create(hmx_environment *env, Sint32 a, Sint32 b,
                                  Sint32 c, Sint32 d) {
    create_env = env;
    grid_create_args[0] = a;
    grid_create_args[1] = b;
    grid_create_args[2] = c;
    grid_create_args[3] = d;
    ++grid_create_call_count;
    return (hmx_grid *)&grid_storage[grid_create_call_count - 1];
}

static hmx_ddagrid *fake_ddagrid_create(hmx_environment *env, Sint32 a,
                                        Sint32 b, Sint32 c, Sint32 d, Sint32 e,
                                        Sint32 f) {
    create_env = env;
    dda_create_args[0] = a;
    dda_create_args[1] = b;
    dda_create_args[2] = c;
    dda_create_args[3] = d;
    dda_create_args[4] = e;
    dda_create_args[5] = f;
    ++dda_create_call_count;
    return (hmx_ddagrid *)&grid_storage[2];
}

static void fake_grid_set_tile(hmx_grid *grid, Sint32 x, Sint32 y,
                               hmx_bitmap *bitmap, Sint32 flags) {
    grid_tile_calls[grid_tile_call_count].grid = grid;
    grid_tile_calls[grid_tile_call_count].x = x;
    grid_tile_calls[grid_tile_call_count].y = y;
    grid_tile_calls[grid_tile_call_count].bitmap = bitmap;
    grid_tile_calls[grid_tile_call_count].flags = flags;
    ++grid_tile_call_count;
}

static void fake_ddagrid_set_tile(hmx_ddagrid *grid, Sint32 x, Sint32 y,
                                  hmx_bitmap *bitmap, Sint32 flags) {
    dda_tile_calls[dda_tile_call_count].grid = grid;
    dda_tile_calls[dda_tile_call_count].x = x;
    dda_tile_calls[dda_tile_call_count].y = y;
    dda_tile_calls[dda_tile_call_count].bitmap = bitmap;
    dda_tile_calls[dda_tile_call_count].flags = flags;
    ++dda_tile_call_count;
}

static void fake_grid_set_position(hmx_grid *grid, Sint32 x, Sint32 y) {
    position_grid = grid;
    position_args[0] = x;
    position_args[1] = y;
    ++position_call_count;
}

static void fake_grid_set_view(hmx_grid *grid, Sint32 x, Sint32 y, Sint32 dx,
                               Sint32 dy) {
    view_grid = grid;
    view_args[0] = x;
    view_args[1] = y;
    view_args[2] = dx;
    view_args[3] = dy;
    ++view_call_count;
}

static void fake_grid_release(hmx_environment *env, hmx_grid *grid) {
    release_env[release_call_count] = env;
    release_grid[release_call_count] = grid;
    ++release_call_count;
}

static hmx_bitmap *bitmap_handle(int index) {
    return (hmx_bitmap *)&bitmap_storage[index];
}

static void reset_fixture(void) {
    int i;

    memset(&context, 0, sizeof(context));
    memset(infoGridBmp, 0, sizeof(infoGridBmp));
    memset(grid_handles, 0, sizeof(grid_handles));
    memset(tile1_handles, 0, sizeof(tile1_handles));
    memset(tile2_handles, 0, sizeof(tile2_handles));
    memset(grid_tile_calls, 0, sizeof(grid_tile_calls));
    memset(dda_tile_calls, 0, sizeof(dda_tile_calls));
    memset(grid_create_args, 0, sizeof(grid_create_args));
    memset(dda_create_args, 0, sizeof(dda_create_args));
    memset(position_args, 0, sizeof(position_args));
    memset(view_args, 0, sizeof(view_args));
    memset(release_grid, 0, sizeof(release_grid));
    memset(release_env, 0, sizeof(release_env));

    s_ctx = &context;
    g_env_module = (hmx_environment *)&env_storage;
    hmx_grid_create_module = fake_grid_create;
    hmx_ddagrid_create_module = fake_ddagrid_create;
    hmx_grid_set_tile_module = fake_grid_set_tile;
    hmx_ddagrid_set_tile_module = fake_ddagrid_set_tile;
    hmx_grid_set_position_module = fake_grid_set_position;
    hmx_grid_set_view_module = fake_grid_set_view;
    hmx_grid_release_module = fake_grid_release;

    bitmap0_handles[0] = (Uint32 *)bitmap_handle(0);
    infoGridBmp[0].w = 8;
    infoGridBmp[0].h = 12;
    infoGridBmp[0].point.x = 5;
    infoGridBmp[0].point.y = 7;
    infoGridBmp[0].lphBmp = bitmap0_handles;
    infoGridBmp[0].lphGrid = &grid_handles[0];
    infoGridBmp[0].lphTile = &tile0_handle;

    for (i = 0; i < 320; ++i) {
        bitmap1_handles[i] = (Uint32 *)bitmap_handle(100 + i);
    }
    infoGridBmp[1].w = 9;
    infoGridBmp[1].h = 11;
    infoGridBmp[1].point.y = 23;
    infoGridBmp[1].lphBmp = bitmap1_handles;
    infoGridBmp[1].lphGrid = &grid_handles[1];
    infoGridBmp[1].lphTile = tile1_handles;

    for (i = 0; i < 512; ++i) {
        bitmap2_handles[i] = (Uint32 *)bitmap_handle(400 + i);
    }
    infoGridBmp[2].lphBmp = bitmap2_handles;
    infoGridBmp[2].lphGrid = &grid_handles[2];
    infoGridBmp[2].lphTile = tile2_handles;

    tile0_handle = 0;
    grid_tile_call_count = 0;
    dda_tile_call_count = 0;
    grid_create_call_count = 0;
    dda_create_call_count = 0;
    create_env = 0;
    position_call_count = 0;
    position_grid = 0;
    view_call_count = 0;
    view_grid = 0;
    release_call_count = 0;
}

static void test_create_grid_zero_sets_tiles_position_and_view(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(0));

    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_TRUE(ctx, create_env == g_env_module);
    TEST_ASSERT_EQ_INT(ctx, 8, grid_create_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 8, grid_create_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 40, grid_create_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 28, grid_create_args[3]);
    TEST_ASSERT_EQ_INT(ctx, (Uint32)context.grids[0], grid_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1120, grid_tile_call_count);
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[0].bitmap == bitmap_handle(0));
    TEST_ASSERT_EQ_INT(ctx, 39, grid_tile_calls[39].x);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_tile_calls[39].y);
    TEST_ASSERT_EQ_INT(ctx, 39, grid_tile_calls[1119].x);
    TEST_ASSERT_EQ_INT(ctx, 27, grid_tile_calls[1119].y);
    TEST_ASSERT_EQ_INT(ctx, 1, position_call_count);
    TEST_ASSERT_EQ_INT(ctx, 5, position_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 7, position_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, view_call_count);
    TEST_ASSERT_EQ_INT(ctx, 320, view_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 336, view_args[3]);
}

static void test_create_grid_one_wraps_forty_tiles_across_eighty_columns(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(1));

    TEST_ASSERT_EQ_INT(ctx, 1, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 80, grid_create_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 8, grid_create_args[3]);
    TEST_ASSERT_EQ_INT(ctx, 640, grid_tile_call_count);
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[0].bitmap == bitmap_handle(380));
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[39].bitmap == bitmap_handle(419));
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[40].bitmap == bitmap_handle(380));
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[79].bitmap == bitmap_handle(419));
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[80].bitmap == bitmap_handle(340));
    TEST_ASSERT_TRUE(ctx, grid_tile_calls[639].bitmap == bitmap_handle(139));
    TEST_ASSERT_EQ_INT(ctx, 1, view_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, view_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, view_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 369, view_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 88, view_args[3]);
}

static void test_create_grid_two_sets_ddagrid_tiles_with_extra_columns(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(2));

    TEST_ASSERT_EQ_INT(ctx, 1, dda_create_call_count);
    TEST_ASSERT_TRUE(ctx, create_env == g_env_module);
    TEST_ASSERT_EQ_INT(ctx, 3, dda_create_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, dda_create_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, dda_create_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 4, dda_create_args[3]);
    TEST_ASSERT_EQ_INT(ctx, 320, dda_create_args[4]);
    TEST_ASSERT_EQ_INT(ctx, 92, dda_create_args[5]);
    TEST_ASSERT_EQ_INT(ctx, (Uint32)context.grids[2], grid_handles[2]);
    TEST_ASSERT_EQ_INT(ctx, 640, dda_tile_call_count);
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[0].bitmap == bitmap_handle(880));
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[31].bitmap == bitmap_handle(911));
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[32].bitmap == bitmap_handle(880));
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[39].bitmap == bitmap_handle(887));
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[600].bitmap == bitmap_handle(400));
    TEST_ASSERT_TRUE(ctx, dda_tile_calls[639].bitmap == bitmap_handle(407));
}

static void test_create_other_index_has_no_side_effects(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(3));

    TEST_ASSERT_EQ_INT(ctx, 0, grid_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, dda_create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, dda_tile_call_count);
}

static void test_delete_grid_zero_releases_and_clears_single_tile(
    test_context *ctx) {
    reset_fixture();
    context.grids[0] = (hmx_grid *)&grid_storage[0];
    grid_handles[0] = 10;
    tile0_handle = 20;

    OEGridDelete(0);

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, release_grid[0] == (hmx_grid *)&grid_storage[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile0_handle);
}

static void test_delete_grid_one_releases_and_clears_tile_array(
    test_context *ctx) {
    reset_fixture();
    context.grids[1] = (hmx_grid *)&grid_storage[1];
    grid_handles[1] = 11;
    tile1_handles[0] = 21;
    tile1_handles[100] = 22;

    OEGridDelete(1);

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, release_grid[0] == (hmx_grid *)&grid_storage[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile1_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile1_handles[100]);
}

static void test_delete_grid_two_releases_and_clears_tile_array(
    test_context *ctx) {
    reset_fixture();
    context.grids[2] = (hmx_grid *)&grid_storage[2];
    grid_handles[2] = 12;
    tile2_handles[0] = 31;
    tile2_handles[511] = 32;

    OEGridDelete(2);

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, release_grid[0] == (hmx_grid *)&grid_storage[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile2_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile2_handles[511]);
}

static void test_delete_empty_grid_runs_tile_cleanup_without_release(
    test_context *ctx) {
    reset_fixture();
    tile1_handles[5] = 66;

    OEGridDelete(1);

    TEST_ASSERT_EQ_INT(ctx, 0, release_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tile1_handles[5]);
}

static void test_all_delete_deletes_all_three_indices(test_context *ctx) {
    reset_fixture();
    context.grids[0] = (hmx_grid *)&grid_storage[0];
    context.grids[1] = (hmx_grid *)&grid_storage[1];
    context.grids[2] = (hmx_grid *)&grid_storage[2];
    grid_handles[0] = 1;
    grid_handles[1] = 2;
    grid_handles[2] = 3;
    tile0_handle = 4;
    tile1_handles[1] = 5;
    tile2_handles[2] = 6;

    OEAllGridDelete();

    TEST_ASSERT_EQ_INT(ctx, 3, release_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile0_handle);
    TEST_ASSERT_EQ_INT(ctx, 0, tile1_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile2_handles[2]);
}

TEST_MAIN_BEGIN;
test_create_grid_zero_sets_tiles_position_and_view(&ctx);
test_create_grid_one_wraps_forty_tiles_across_eighty_columns(&ctx);
test_create_grid_two_sets_ddagrid_tiles_with_extra_columns(&ctx);
test_create_other_index_has_no_side_effects(&ctx);
test_delete_grid_zero_releases_and_clears_single_tile(&ctx);
test_delete_grid_one_releases_and_clears_tile_array(&ctx);
test_delete_grid_two_releases_and_clears_tile_array(&ctx);
test_delete_empty_grid_runs_tile_cleanup_without_release(&ctx);
test_all_delete_deletes_all_three_indices(&ctx);
TEST_MAIN_END

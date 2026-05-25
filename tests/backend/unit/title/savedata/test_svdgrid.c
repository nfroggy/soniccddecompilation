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
} tile_call;

static draw_context context;
draw_context *s_ctx;
grid_bmp infoGridBmp[3];
hmx_environment *g_env_module;
Uint16 IDX_GDBMP_BACK;
void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32, Sint32);
void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32, hmx_bitmap *,
                                 Sint32);
hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32, Sint32,
                                    Sint32);
void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);

static char env_storage;
static char grid_storage[3];
static char bitmap_storage;
static Uint32 grid_handles[3];
static Uint32 tile_handles[3];
static Uint32 *bitmap_handles[3];
static tile_call tile_calls[1120];
static Sint32 tile_call_count;
static Sint32 create_call_count;
static hmx_environment *create_env;
static Sint32 create_args[4];
static Sint32 position_call_count;
static hmx_grid *position_grid;
static Sint32 position_args[2];
static Sint32 view_call_count;
static hmx_grid *view_grid;
static Sint32 view_args[4];
static Sint32 release_call_count;
static hmx_environment *release_env;
static hmx_grid *release_grid;

#include "src/title/savedata/svdgrid.c"

static hmx_grid *fake_grid_create(hmx_environment *env, Sint32 a, Sint32 b,
                                  Sint32 c, Sint32 d) {
    create_env = env;
    create_args[0] = a;
    create_args[1] = b;
    create_args[2] = c;
    create_args[3] = d;
    ++create_call_count;
    return (hmx_grid *)&grid_storage[IDX_GDBMP_BACK];
}

static void fake_grid_set_tile(hmx_grid *grid, Sint32 x, Sint32 y,
                               hmx_bitmap *bitmap, Sint32 flags) {
    tile_calls[tile_call_count].grid = grid;
    tile_calls[tile_call_count].x = x;
    tile_calls[tile_call_count].y = y;
    tile_calls[tile_call_count].bitmap = bitmap;
    tile_calls[tile_call_count].flags = flags;
    ++tile_call_count;
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
    release_env = env;
    release_grid = grid;
    ++release_call_count;
}

static void reset_fixture(void) {
    int i;

    memset(&context, 0, sizeof(context));
    memset(infoGridBmp, 0, sizeof(infoGridBmp));
    memset(grid_handles, 0, sizeof(grid_handles));
    memset(tile_handles, 0, sizeof(tile_handles));
    memset(bitmap_handles, 0, sizeof(bitmap_handles));
    memset(tile_calls, 0, sizeof(tile_calls));
    memset(create_args, 0, sizeof(create_args));
    memset(position_args, 0, sizeof(position_args));
    memset(view_args, 0, sizeof(view_args));

    s_ctx = &context;
    g_env_module = (hmx_environment *)&env_storage;
    IDX_GDBMP_BACK = 1;
    hmx_grid_create_module = fake_grid_create;
    hmx_grid_set_tile_module = fake_grid_set_tile;
    hmx_grid_set_position_module = fake_grid_set_position;
    hmx_grid_set_view_module = fake_grid_set_view;
    hmx_grid_release_module = fake_grid_release;

    for (i = 0; i < 3; ++i) {
        bitmap_handles[i] = (Uint32 *)&bitmap_storage;
        infoGridBmp[i].lphBmp = &bitmap_handles[i];
        infoGridBmp[i].lphGrid = &grid_handles[i];
        infoGridBmp[i].lphTile = &tile_handles[i];
    }
    infoGridBmp[1].w = 10;
    infoGridBmp[1].h = 6;
    infoGridBmp[1].point.x = 13;
    infoGridBmp[1].point.y = 21;

    tile_call_count = 0;
    create_call_count = 0;
    create_env = 0;
    position_call_count = 0;
    position_grid = 0;
    view_call_count = 0;
    view_grid = 0;
    release_call_count = 0;
    release_env = 0;
    release_grid = 0;
}

static void test_oegridcreate_back_index_builds_grid_tiles_position_and_view(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(1));

    TEST_ASSERT_EQ_INT(ctx, 1, create_call_count);
    TEST_ASSERT_TRUE(ctx, create_env == g_env_module);
    TEST_ASSERT_EQ_INT(ctx, 8, create_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 8, create_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 40, create_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 28, create_args[3]);
    TEST_ASSERT_TRUE(ctx, context.grids[1] == (hmx_grid *)&grid_storage[1]);
    TEST_ASSERT_EQ_INT(ctx, (Uint32)context.grids[1], grid_handles[1]);

    TEST_ASSERT_EQ_INT(ctx, 1120, tile_call_count);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].grid == context.grids[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[0].y);
    TEST_ASSERT_TRUE(ctx, tile_calls[0].bitmap == (hmx_bitmap *)&bitmap_storage);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[0].flags);
    TEST_ASSERT_EQ_INT(ctx, 39, tile_calls[39].x);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[39].y);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_calls[40].x);
    TEST_ASSERT_EQ_INT(ctx, 1, tile_calls[40].y);
    TEST_ASSERT_EQ_INT(ctx, 39, tile_calls[1119].x);
    TEST_ASSERT_EQ_INT(ctx, 27, tile_calls[1119].y);

    TEST_ASSERT_EQ_INT(ctx, 1, position_call_count);
    TEST_ASSERT_TRUE(ctx, position_grid == context.grids[1]);
    TEST_ASSERT_EQ_INT(ctx, 13, position_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 21, position_args[1]);

    TEST_ASSERT_EQ_INT(ctx, 1, view_call_count);
    TEST_ASSERT_TRUE(ctx, view_grid == context.grids[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, view_args[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, view_args[1]);
    TEST_ASSERT_EQ_INT(ctx, 400, view_args[2]);
    TEST_ASSERT_EQ_INT(ctx, 168, view_args[3]);
}

static void test_oegridcreate_other_index_returns_without_side_effects(
    test_context *ctx) {
    reset_fixture();
    context.grids[0] = (hmx_grid *)&grid_storage[0];

    TEST_ASSERT_EQ_INT(ctx, 0, OEGridCreate(0));

    TEST_ASSERT_EQ_INT(ctx, 0, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, position_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, view_call_count);
    TEST_ASSERT_TRUE(ctx, context.grids[0] == (hmx_grid *)&grid_storage[0]);
}

static void test_oegriddelete_back_index_releases_and_clears_tile_handle(
    test_context *ctx) {
    reset_fixture();
    context.grids[1] = (hmx_grid *)&grid_storage[1];
    grid_handles[1] = 78;
    tile_handles[1] = 90;

    OEGridDelete(1);

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, release_env == g_env_module);
    TEST_ASSERT_TRUE(ctx, release_grid == (hmx_grid *)&grid_storage[1]);
    TEST_ASSERT_TRUE(ctx, context.grids[1] == 0);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_handles[1]);
}

static void test_oegriddelete_other_index_keeps_tile_handle(test_context *ctx) {
    reset_fixture();
    context.grids[0] = (hmx_grid *)&grid_storage[0];
    grid_handles[0] = 12;
    tile_handles[0] = 34;

    OEGridDelete(0);

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, context.grids[0] == 0);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 34, tile_handles[0]);
}

static void test_oegriddelete_empty_grid_keeps_empty_state(test_context *ctx) {
    reset_fixture();

    OEGridDelete(1);

    TEST_ASSERT_EQ_INT(ctx, 0, release_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_handles[1]);
}

static void test_oeallgriddelete_uses_back_index(test_context *ctx) {
    reset_fixture();
    context.grids[1] = (hmx_grid *)&grid_storage[1];
    grid_handles[1] = 44;
    tile_handles[1] = 55;

    OEAllGridDelete();

    TEST_ASSERT_EQ_INT(ctx, 1, release_call_count);
    TEST_ASSERT_TRUE(ctx, context.grids[1] == 0);
    TEST_ASSERT_EQ_INT(ctx, 0, grid_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, tile_handles[1]);
}

TEST_MAIN_BEGIN;
test_oegridcreate_back_index_builds_grid_tiles_position_and_view(&ctx);
test_oegridcreate_other_index_returns_without_side_effects(&ctx);
test_oegriddelete_back_index_releases_and_clears_tile_handle(&ctx);
test_oegriddelete_other_index_keeps_tile_handle(&ctx);
test_oegriddelete_empty_grid_keeps_empty_state(&ctx);
test_oeallgriddelete_uses_back_index(&ctx);
TEST_MAIN_END

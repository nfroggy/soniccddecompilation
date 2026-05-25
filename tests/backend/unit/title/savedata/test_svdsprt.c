#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

draw_context context;
draw_context *s_ctx;
Uint32 hSprFile[6][56];
Sint32 PRIO_MAX;
sprite_bmp infoSprtBmp[256];
POINT nPosiXSprFile[56];
Uint32 hSprMes[32];
Sint32 nPosiXSprMes[32];
Sint32 nFileNumber;
Sint32 PLAYERCHAR_W;
Uint32 bMenuChosed;
Sint32 nTimerCunt;
void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);

static hmx_sprite *sprite_table[1000];
static char sprite_storage[1000];
static char bitmap_storage[1000];
static Uint32 sprite_handles[256];
static Sint32 next_sprite_handle;
static Sint32 freed_handles[1000];
static Sint32 position_call_count;
static hmx_sprite *position_sprites[128];
static Sint32 position_x[128];
static Sint32 position_y[128];
static Sint32 bitmap_call_count;
static hmx_sprite *bitmap_sprites[128];
static hmx_bitmap *bitmap_values[128];
static Sint32 flags_call_count;
static hmx_sprite *flags_sprites[128];
static Sint32 flags_values[128];
static Sint32 spr_create_call_count;
static Sint32 spr_delete_call_count;
static Sint32 spr_delete_values[128];
static Sint32 spr_create_index_call_count;
static Sint32 spr_create_indices[128];
static Sint32 spr_delete_index_call_count;
static Sint32 spr_delete_indices[128];

#include "src/title/savedata/svdsprt.c"

mark_info markTable[] = {{'.', 90, 10, 50, 130},
                         {'-', 91, 11, 51, 131},
                         {0, 0, 0, 0, 0}};

static hmx_sprite *sprite_handle(Sint32 handle) {
    return (hmx_sprite *)&sprite_storage[handle];
}

static hmx_bitmap *bitmap_handle(Sint32 handle) {
    return (hmx_bitmap *)&bitmap_storage[handle];
}

static void fake_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    position_sprites[position_call_count] = sprite;
    position_x[position_call_count] = x;
    position_y[position_call_count] = y;
    ++position_call_count;
}

static void fake_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bitmap) {
    bitmap_sprites[bitmap_call_count] = sprite;
    bitmap_values[bitmap_call_count] = bitmap;
    ++bitmap_call_count;
}

static void fake_sprite_set_flags(hmx_sprite *sprite, Sint32 flags) {
    flags_sprites[flags_call_count] = sprite;
    flags_values[flags_call_count] = flags;
    ++flags_call_count;
}

void sprCreate(Sint32 *handle) {
    *handle = next_sprite_handle++;
    ++spr_create_call_count;
}

void sprDelete(Sint32 handle) {
    freed_handles[handle] = 1;
    spr_delete_values[spr_delete_call_count] = handle;
    ++spr_delete_call_count;
}

Sint32 OESprCreateIndx(Sint32 indx) {
    spr_create_indices[spr_create_index_call_count] = indx;
    ++spr_create_index_call_count;
    if (*infoSprtBmp[indx].lphSpr == 0) {
        *infoSprtBmp[indx].lphSpr = next_sprite_handle++;
    }
    return 0;
}

void OESprDeleteIndx(Sint32 indx) {
    spr_delete_indices[spr_delete_index_call_count] = indx;
    ++spr_delete_index_call_count;
    if (*infoSprtBmp[indx].lphSpr != 0) {
        *infoSprtBmp[indx].lphSpr = 0;
    }
}

static void reset_fixture(void) {
    Sint32 i;

    memset(&context, 0, sizeof(context));
    memset(hSprFile, 0, sizeof(hSprFile));
    memset(hSprMes, 0, sizeof(hSprMes));
    memset(sprite_handles, 0, sizeof(sprite_handles));
    memset(infoSprtBmp, 0, sizeof(infoSprtBmp));
    memset(nPosiXSprFile, 0, sizeof(nPosiXSprFile));
    memset(nPosiXSprMes, 0, sizeof(nPosiXSprMes));
    memset(freed_handles, 0, sizeof(freed_handles));
    memset(position_sprites, 0, sizeof(position_sprites));
    memset(position_x, 0, sizeof(position_x));
    memset(position_y, 0, sizeof(position_y));
    memset(bitmap_sprites, 0, sizeof(bitmap_sprites));
    memset(bitmap_values, 0, sizeof(bitmap_values));
    memset(flags_sprites, 0, sizeof(flags_sprites));
    memset(flags_values, 0, sizeof(flags_values));
    memset(spr_delete_values, 0, sizeof(spr_delete_values));
    memset(spr_create_indices, 0, sizeof(spr_create_indices));
    memset(spr_delete_indices, 0, sizeof(spr_delete_indices));

    for (i = 0; i < 1000; ++i) {
        sprite_table[i] = sprite_handle(i);
        context.spr_bitmaps[i] = bitmap_handle(i);
    }
    for (i = 0; i < 256; ++i) {
        sprite_handles[i] = 0;
        infoSprtBmp[i].lphSpr = &sprite_handles[i];
        infoSprtBmp[i].order = (Uint16)(i % 16);
    }
    for (i = 0; i < 56; ++i) {
        nPosiXSprFile[i].x = 128 + i * 8;
        nPosiXSprFile[i].y = i % 4;
    }
    for (i = 0; i < 32; ++i) {
        nPosiXSprMes[i] = 12 + i * 8;
    }

    context.sprites = sprite_table;
    s_ctx = &context;
    PRIO_MAX = 160;
    nFileNumber = 2;
    PLAYERCHAR_W = 8;
    bMenuChosed = 1;
    nTimerCunt = 0;
    cursolPosiIndxX = 0;
    nSonicKind = 203;
    nSonicMode = 0;
    posiSonicCursol.x = 0;
    posiSonicCursol.y = 0;
    posiTarget.x = 0;
    posiTarget.y = 0;
    fSonicIncX = 0;
    fSonicIncY = 0;
    nStandStartTimerCunt = 0;
    posiStartXOld.l = 0;
    posiStartYOld.l = 0;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    hmx_sprite_set_bitmap_module = fake_sprite_set_bitmap;
    hmx_sprite_set_flags_module = fake_sprite_set_flags;
    next_sprite_handle = 1;
    position_call_count = 0;
    bitmap_call_count = 0;
    flags_call_count = 0;
    spr_create_call_count = 0;
    spr_delete_call_count = 0;
    spr_create_index_call_count = 0;
    spr_delete_index_call_count = 0;
}

static void test_create_char_sprites_for_message_and_file_blocks(
    test_context *ctx) {
    Sint32 handle;

    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('0', 0, 1, 0));
    handle = hSprMes[0];
    TEST_ASSERT_EQ_INT(ctx, 1, handle);
    TEST_ASSERT_TRUE(ctx, position_sprites[0] == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, nPosiXSprMes[0], position_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, position_y[0]);
    TEST_ASSERT_TRUE(ctx, bitmap_values[0] == bitmap_handle(11));
    TEST_ASSERT_EQ_INT(ctx, PRIO_MAX - infoSprtBmp[11].order,
                       context.spr_level[handle]);
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('0', 0, 1, 0));

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('A', 1, 2, 3));
    handle = hSprFile[0][3];
    TEST_ASSERT_TRUE(ctx, position_sprites[1] == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, nPosiXSprFile[3].x, position_x[1]);
    TEST_ASSERT_EQ_INT(ctx, 32 + nPosiXSprFile[3].y, position_y[1]);
    TEST_ASSERT_TRUE(ctx, bitmap_values[1] == bitmap_handle(57));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('A', 1, 2, 3));
}

static void test_create_char_sprites_font_mapping_and_rejections(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('9', 0, 1, 0));
    TEST_ASSERT_TRUE(ctx, bitmap_values[0] == bitmap_handle(20));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('Z', 0, 1, 1));
    TEST_ASSERT_TRUE(ctx, bitmap_values[1] == bitmap_handle(46));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('.', 0, 1, 2));
    TEST_ASSERT_TRUE(ctx, bitmap_values[2] == bitmap_handle(10));

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('9', 1, 1, 3));
    TEST_ASSERT_TRUE(ctx, bitmap_values[3] == bitmap_handle(56));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('.', 1, 1, 4));
    TEST_ASSERT_TRUE(ctx, bitmap_values[4] == bitmap_handle(50));

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('9', 2, 1, 5));
    TEST_ASSERT_TRUE(ctx, bitmap_values[5] == bitmap_handle(92));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('Z', 2, 1, 6));
    TEST_ASSERT_TRUE(ctx, bitmap_values[6] == bitmap_handle(118));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('.', 2, 1, 7));
    TEST_ASSERT_TRUE(ctx, bitmap_values[7] == bitmap_handle(90));

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('9', 3, 1, 8));
    TEST_ASSERT_TRUE(ctx, bitmap_values[8] == bitmap_handle(128));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('Z', 3, 1, 9));
    TEST_ASSERT_TRUE(ctx, bitmap_values[9] == bitmap_handle(154));
    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt('.', 3, 1, 10));
    TEST_ASSERT_TRUE(ctx, bitmap_values[10] == bitmap_handle(130));

    TEST_ASSERT_EQ_INT(ctx, -1, GetMarkfontIndx('.', 99));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('?', 0, 1, 11));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('?', 1, 1, 11));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('?', 2, 1, 11));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('?', 3, 1, 11));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt('A', 99, 1, 11));
}

static void test_create_string_skips_spaces_and_delete_helpers(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateStringSprt("A 1", 0, 1, 4));
    TEST_ASSERT_EQ_INT(ctx, 2, spr_create_call_count);
    TEST_ASSERT_TRUE(ctx, hSprMes[4] != 0);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMes[5]);
    TEST_ASSERT_TRUE(ctx, hSprMes[6] != 0);

    DeleteCharSprt(1, 5);
    TEST_ASSERT_EQ_INT(ctx, 0, spr_delete_call_count);
    DeleteCharSprt(1, 4);
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMes[4]);

    CreateCharSprt('B', 0, 2, 0);
    DeleteCharSprt(2, 1);
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_call_count);
    DeleteCharSprt(2, 0);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprFile[0][0]);

    CreateStringSprt("ABC", 0, 1, 0);
    DeleteStringSprt(3, 1, 0);
    TEST_ASSERT_EQ_INT(ctx, 5, spr_delete_call_count);

    CreateCharSprt('A', 0, 1, 0);
    CreateCharSprt('A', 0, 2, 0);
    CreateCharSprt('A', 0, 7, 0);
    DeleteAllCharSprt();
    TEST_ASSERT_EQ_INT(ctx, 9, spr_delete_call_count);
}

static void test_text_cursor_position_show_and_hide(test_context *ctx) {
    reset_fixture();
    sprite_handles[191] = 191;

    MovCursol(3);
    TEST_ASSERT_EQ_INT(ctx, 3, GetCursolPositon());
    TEST_ASSERT_TRUE(ctx, position_sprites[0] == sprite_handle(191));
    TEST_ASSERT_EQ_INT(ctx, nPosiXSprFile[0].x + PLAYERCHAR_W * 3,
                       position_x[0]);
    TEST_ASSERT_EQ_INT(ctx, nFileNumber * 32 + 32, position_y[0]);

    sprite_handles[191] = 0;
    DispCursol();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_create_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 191, spr_create_indices[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, position_call_count);

    HideCursol();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 191, spr_delete_indices[0]);
}

static void test_sonic_cursor_stand_animation_and_flags(test_context *ctx) {
    POINT point;

    reset_fixture();
    point.x = 40;
    point.y = 50;
    bMenuChosed = 0;

    DispSonicCursol(point);
    TEST_ASSERT_EQ_INT(ctx, 203, spr_create_indices[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, flags_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, nSonicMode);

    nTimerCunt = 19;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_create_index_call_count);
    nTimerCunt = 20;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_create_index_call_count);
    nTimerCunt = 80;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 1, spr_delete_index_call_count);
    TEST_ASSERT_EQ_INT(ctx, 204, spr_create_indices[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, flags_values[1]);

    bMenuChosed = 1;
    nTimerCunt = 100;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 205, spr_create_indices[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, flags_values[2]);
    nTimerCunt = 120;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 204, spr_create_indices[3]);

    nSonicKind = 208;
    nTimerCunt = 140;
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 203, spr_create_indices[4]);
}

static void test_sonic_cursor_running_movement_and_sprite_cycle(
    test_context *ctx) {
    POINT point;
    Sint32 create_start;
    Sint32 flags_start;

    reset_fixture();
    point.x = 10;
    point.y = 10;
    DispSonicCursol(point);

    point.x = 50;
    point.y = 10;
    RunSonicCursol(point);
    UpdateSonicCursol();
    TEST_ASSERT_EQ_INT(ctx, 206, spr_create_indices[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, flags_values[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, nSonicMode);

    point.x = 50;
    point.y = 10;
    DispSonicCursol(point);
    point.y = 30;
    RunSonicCursol(point);
    TEST_ASSERT_EQ_INT(ctx, 0, fSonicIncX);
    TEST_ASSERT_TRUE(ctx, fSonicIncY > 0);

    nSonicKind = 208;
    fSonicIncX = -1;
    point.x = 8;
    point.y = 10;
    create_start = spr_create_index_call_count;
    flags_start = flags_call_count;
    MovSonicCursol(point);
    TEST_ASSERT_EQ_INT(ctx, 206, spr_create_indices[create_start]);
    TEST_ASSERT_EQ_INT(ctx, 1, flags_values[flags_start]);

    nSonicKind = 207;
    fSonicIncX = 1;
    bMenuChosed = 1;
    point.x = 12;
    point.y = 10;
    create_start = spr_create_index_call_count;
    flags_start = flags_call_count;
    MovSonicCursol(point);
    TEST_ASSERT_EQ_INT(ctx, 208, spr_create_indices[create_start]);
    TEST_ASSERT_EQ_INT(ctx, 0, flags_values[flags_start]);

    point.x = -20;
    point.y = -20;
    RunSonicCursol(point);
    while (nSonicMode == 2) {
        UpdateSonicCursol();
    }
    TEST_ASSERT_EQ_INT(ctx, -20, posiSonicCursol.x);
    TEST_ASSERT_EQ_INT(ctx, -20, posiSonicCursol.y);
    TEST_ASSERT_EQ_INT(ctx, 1, nSonicMode);
}

static void test_isqrt_zero_and_positive_values(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 0, isqrt(0));
    TEST_ASSERT_EQ_INT(ctx, 4, isqrt(16));
    TEST_ASSERT_EQ_INT(ctx, 12, isqrt(145));
}

TEST_MAIN_BEGIN;
test_create_char_sprites_for_message_and_file_blocks(&ctx);
test_create_char_sprites_font_mapping_and_rejections(&ctx);
test_create_string_skips_spaces_and_delete_helpers(&ctx);
test_text_cursor_position_show_and_hide(&ctx);
test_sonic_cursor_stand_animation_and_flags(&ctx);
test_sonic_cursor_running_movement_and_sprite_cycle(&ctx);
test_isqrt_zero_and_positive_values(&ctx);
TEST_MAIN_END

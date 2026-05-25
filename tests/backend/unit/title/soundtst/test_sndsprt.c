#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/title/common/hmx_types.h"
#include "src/types.h"

typedef struct {
    hmx_sprite *sprite;
    Sint32 x;
    Sint32 y;
} position_call;

typedef struct {
    hmx_sprite *sprite;
    hmx_bitmap *bitmap;
} bitmap_call;

typedef struct {
    hmx_sprite *sprite;
    Sint32 flags;
} flags_call;

static draw_context context;
static hmx_sprite *sprite_table[1000];
static char sprite_storage[1000];
static char bitmap_storage[1000];
draw_context *s_ctx;
sprite_bmp infoSprtBmp[1000];
void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
void (*sPrintf)(char *, const char *, ...);

static position_call position_calls[256];
static bitmap_call bitmap_calls[256];
static flags_call flags_calls[256];
static Sint32 created_handles[256];
static Sint32 deleted_handles[256];
static Sint32 position_call_count;
static Sint32 bitmap_call_count;
static Sint32 flags_call_count;
static Sint32 create_call_count;
static Sint32 delete_call_count;
static Sint32 next_handle;

#include "src/title/soundtst/sndsprt.c"

void sprCreate(Sint32 *handle) {
    *handle = next_handle;
    created_handles[create_call_count] = next_handle;
    ++create_call_count;
    ++next_handle;
}

void sprDelete(Sint32 handle) {
    deleted_handles[delete_call_count] = handle;
    ++delete_call_count;
}

static void fake_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y) {
    position_calls[position_call_count].sprite = sprite;
    position_calls[position_call_count].x = x;
    position_calls[position_call_count].y = y;
    ++position_call_count;
}

static void fake_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bitmap) {
    bitmap_calls[bitmap_call_count].sprite = sprite;
    bitmap_calls[bitmap_call_count].bitmap = bitmap;
    ++bitmap_call_count;
}

static void fake_sprite_set_flags(hmx_sprite *sprite, Sint32 flags) {
    flags_calls[flags_call_count].sprite = sprite;
    flags_calls[flags_call_count].flags = flags;
    ++flags_call_count;
}

static void fake_sprintf(char *dst, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vsprintf(dst, fmt, args);
    va_end(args);
}

static hmx_sprite *sprite_handle(Sint32 handle) {
    return (hmx_sprite *)&sprite_storage[handle];
}

static hmx_bitmap *bitmap_index(Sint32 index) {
    return (hmx_bitmap *)&bitmap_storage[index];
}

static void reset_fixture(void) {
    Sint32 i;

    memset(&context, 0, sizeof(context));
    memset(sprite_table, 0, sizeof(sprite_table));
    memset(infoSprtBmp, 0, sizeof(infoSprtBmp));
    memset(hSprMenu, 0, sizeof(hSprMenu));
    memset(position_calls, 0, sizeof(position_calls));
    memset(bitmap_calls, 0, sizeof(bitmap_calls));
    memset(flags_calls, 0, sizeof(flags_calls));
    memset(created_handles, 0, sizeof(created_handles));
    memset(deleted_handles, 0, sizeof(deleted_handles));

    for (i = 0; i < 1000; ++i) {
        sprite_table[i] = sprite_handle(i);
        context.spr_bitmaps[i] = bitmap_index(i);
        infoSprtBmp[i].order = i % 19;
    }

    context.sprites = sprite_table;
    s_ctx = &context;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    hmx_sprite_set_bitmap_module = fake_sprite_set_bitmap;
    hmx_sprite_set_flags_module = fake_sprite_set_flags;
    sPrintf = fake_sprintf;
    hMesMenu = 0;
    position_call_count = 0;
    bitmap_call_count = 0;
    flags_call_count = 0;
    create_call_count = 0;
    delete_call_count = 0;
    next_handle = 30;
}

static void assert_created_call(test_context *ctx, Sint32 call, Sint32 menu,
                                Sint32 colom, Sint32 code_index) {
    Sint32 handle = created_handles[call];

    TEST_ASSERT_EQ_INT(ctx, (Uint32)handle, hSprMenu[menu][colom]);
    TEST_ASSERT_TRUE(ctx, position_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, nMenuPosiX[menu] + colom * 8,
                       position_calls[call].x);
    TEST_ASSERT_EQ_INT(ctx, 80, position_calls[call].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].bitmap == bitmap_index(code_index));
    TEST_ASSERT_TRUE(ctx, flags_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[call].flags);
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[code_index].order,
                       context.spr_level[handle]);
}

static void assert_string_rendered(test_context *ctx, Sint32 menu,
                                   const Sint32 *columns, const Sint32 *codes,
                                   Sint32 count) {
    Sint32 i;

    TEST_ASSERT_EQ_INT(ctx, count, create_call_count);
    for (i = 0; i < count; ++i) {
        assert_created_call(ctx, i, menu, columns[i], codes[i]);
    }
}

static void test_create_char_color_mappings_and_replacement(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(0, 0, '0', 1));
    assert_created_call(ctx, 0, 0, 0, 45);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(0, 1, 'A', 1));
    assert_created_call(ctx, 1, 0, 1, 55);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(0, 2, '.', 1));
    assert_created_call(ctx, 2, 0, 2, 82);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(1, 0, '0', 2));
    assert_created_call(ctx, 3, 1, 0, 9);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(1, 1, 'A', 2));
    assert_created_call(ctx, 4, 1, 1, 19);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(1, 2, '.', 2));
    assert_created_call(ctx, 5, 1, 2, 81);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(1, 2, 'B', 2));
    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 35, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 36, created_handles[6]);
    TEST_ASSERT_EQ_INT(ctx, 36, hSprMenu[1][2]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[6].sprite == sprite_handle(35));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[6].bitmap == 0);
    TEST_ASSERT_TRUE(ctx, flags_calls[6].sprite == sprite_handle(35));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[6].flags);
    TEST_ASSERT_TRUE(ctx, position_calls[6].sprite == sprite_handle(36));
    TEST_ASSERT_EQ_INT(ctx, nMenuPosiX[1] + 2 * 8, position_calls[6].x);
    TEST_ASSERT_EQ_INT(ctx, 80, position_calls[6].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[7].sprite == sprite_handle(36));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[7].bitmap == bitmap_index(20));
    TEST_ASSERT_TRUE(ctx, flags_calls[7].sprite == sprite_handle(36));
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[20].order, context.spr_level[36]);
}

static void test_delete_all_and_message_delete_cover_empty_and_existing_slots(
    test_context *ctx) {
    reset_fixture();
    hSprMenu[0][0] = 41;
    hSprMenu[2][9] = 42;
    context.spr_level[41] = 11;
    context.spr_level[42] = 12;

    DeleteAllCharSprt();

    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 41, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 42, deleted_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[2][9]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[41]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[42]);

    DeleteCharMes();
    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);

    hMesMenu = 43;
    context.spr_level[43] = 13;
    DeleteCharMes();
    TEST_ASSERT_EQ_INT(ctx, 3, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 43, deleted_handles[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, hMesMenu);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[43]);
}

static void test_menu_labels_and_numbers_render_exact_codes(test_context *ctx) {
    static const Sint32 pcm_cols[] = {0, 1, 2, 4, 5, 6};
    static const Sint32 pcm_codes[] = {70, 57, 67, 68, 69, 82};
    static const Sint32 pcm_highlight_codes[] = {34, 21, 31, 32, 33, 81};
    static const Sint32 da_cols[] = {0, 1, 3, 4, 5};
    static const Sint32 da_codes[] = {58, 55, 68, 69, 82};

    reset_fixture();
    StrPCM(1);
    assert_string_rendered(ctx, 0, pcm_cols, pcm_codes, 6);

    reset_fixture();
    StrDA(1);
    assert_string_rendered(ctx, 1, da_cols, da_codes, 5);

    reset_fixture();
    StrPCM(2);
    assert_string_rendered(ctx, 0, pcm_cols, pcm_highlight_codes, 6);

    reset_fixture();
    StrPCMNO(7);
    TEST_ASSERT_EQ_INT(ctx, 2, create_call_count);
    assert_created_call(ctx, 0, 0, 7, 45);
    assert_created_call(ctx, 1, 0, 8, 52);

    reset_fixture();
    StrDANO(42);
    TEST_ASSERT_EQ_INT(ctx, 2, create_call_count);
    assert_created_call(ctx, 0, 1, 6, 49);
    assert_created_call(ctx, 1, 1, 7, 47);
}

static void test_welcome_message_create_and_delete(test_context *ctx) {
    Sint32 handle;

    reset_fixture();

    StrWelcome();

    TEST_ASSERT_EQ_INT(ctx, 1, create_call_count);
    handle = created_handles[0];
    TEST_ASSERT_EQ_INT(ctx, (Uint32)handle, hMesMenu);
    TEST_ASSERT_TRUE(ctx, position_calls[0].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, 36, position_calls[0].x);
    TEST_ASSERT_EQ_INT(ctx, 160, position_calls[0].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[0].bitmap == bitmap_index(99));
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[99].order, context.spr_level[handle]);

    DeleteCharMes();

    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, handle, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hMesMenu);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[handle]);
}

TEST_MAIN_BEGIN;
test_create_char_color_mappings_and_replacement(&ctx);
test_delete_all_and_message_delete_cover_empty_and_existing_slots(&ctx);
test_menu_labels_and_numbers_render_exact_codes(&ctx);
test_welcome_message_create_and_delete(&ctx);
TEST_MAIN_END

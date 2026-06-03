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

#define __attribute__(x)
#include "src/title/visualmd/vmdsprt.c"
#undef __attribute__

void sprCreate(Uint32 *handle) {
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
        infoSprtBmp[i].order = i % 13;
    }

    context.sprites = sprite_table;
    s_ctx = &context;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    hmx_sprite_set_bitmap_module = fake_sprite_set_bitmap;
    hmx_sprite_set_flags_module = fake_sprite_set_flags;
    position_call_count = 0;
    bitmap_call_count = 0;
    flags_call_count = 0;
    create_call_count = 0;
    delete_call_count = 0;
    next_handle = 20;
}

static void assert_created_call(test_context *ctx, Sint32 call, Sint32 menu,
                                Sint32 colom, Sint32 code) {
    Sint32 handle = created_handles[call];

    TEST_ASSERT_EQ_INT(ctx, (Uint32)handle, hSprMenu[menu][colom]);
    TEST_ASSERT_TRUE(ctx, position_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, colom * 8 + 192, position_calls[call].x);
    TEST_ASSERT_EQ_INT(ctx, nMenuPosiY[menu], position_calls[call].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].bitmap == bitmap_index(code));
    TEST_ASSERT_TRUE(ctx, flags_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[call].flags);
    TEST_ASSERT_EQ_INT(ctx, 60 - infoSprtBmp[code].order,
                       context.spr_level[handle]);
}

static void assert_string_rendered(test_context *ctx, Sint32 menu,
                                   const Sint32 *columns, const Sint32 *codes,
                                   Sint32 count) {
    Sint32 i;

    TEST_ASSERT_EQ_INT(ctx, count, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, count, position_call_count);
    TEST_ASSERT_EQ_INT(ctx, count, bitmap_call_count);
    TEST_ASSERT_EQ_INT(ctx, count, flags_call_count);
    for (i = 0; i < count; ++i) {
        assert_created_call(ctx, i, menu, columns[i], codes[i]);
    }
}

static void test_create_char_sets_exact_sprite_fields(test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(2, 5, 34));

    TEST_ASSERT_EQ_INT(ctx, 1, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 20, created_handles[0]);
    assert_created_call(ctx, 0, 2, 5, 34);
}

static void test_create_existing_slot_returns_zero_without_side_effects(
    test_context *ctx) {
    reset_fixture();

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(1, 2, 40));
    TEST_ASSERT_EQ_INT(ctx, 0, CreateCharSprt(1, 2, 41));

    TEST_ASSERT_EQ_INT(ctx, 1, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, bitmap_call_count);
    TEST_ASSERT_EQ_INT(ctx, 20, hSprMenu[1][2]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[0].bitmap == bitmap_index(40));
}

static void test_delete_char_clears_existing_sprite_and_ignores_empty(
    test_context *ctx) {
    reset_fixture();

    DeleteCharSprt(0, 0);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_call_count);

    TEST_ASSERT_EQ_INT(ctx, 1, CreateCharSprt(0, 0, 4));
    DeleteCharSprt(0, 0);

    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 20, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[20]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].sprite == sprite_handle(20));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].bitmap == 0);
    TEST_ASSERT_TRUE(ctx, flags_calls[1].sprite == sprite_handle(20));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[1].flags);
}

static void test_delete_all_walks_every_menu_slot(test_context *ctx) {
    reset_fixture();
    hSprMenu[0][0] = 30;
    hSprMenu[4][10] = 31;
    context.spr_level[30] = 66;
    context.spr_level[31] = 77;

    DeleteAllCharSprt();

    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 30, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 31, deleted_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[4][10]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[30]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[31]);
}

static void test_str_exit_normal_and_highlighted_codes(test_context *ctx) {
    static const Sint32 cols[] = {0, 1, 2, 3};
    static const Sint32 normal[] = {4, 23, 8, 19};
    static const Sint32 highlighted[] = {30, 49, 34, 45};

    reset_fixture();
    StrEXIT(0);
    assert_string_rendered(ctx, 0, cols, normal, 4);

    reset_fixture();
    StrEXIT(1);
    assert_string_rendered(ctx, 0, cols, highlighted, 4);
}

static void test_str_opening_normal_and_highlighted_codes(test_context *ctx) {
    static const Sint32 cols[] = {0, 1, 2, 3, 4, 5, 6};
    static const Sint32 normal[] = {14, 15, 4, 13, 8, 13, 6};
    static const Sint32 highlighted[] = {40, 41, 30, 39, 34, 39, 32};

    reset_fixture();
    StrOPENING(0);
    assert_string_rendered(ctx, 1, cols, normal, 7);

    reset_fixture();
    StrOPENING(1);
    assert_string_rendered(ctx, 1, cols, highlighted, 7);
}

static void test_str_goodending_normal_and_highlighted_codes(
    test_context *ctx) {
    static const Sint32 cols[] = {0, 1, 2, 3, 5, 6, 7, 8, 9, 10};
    static const Sint32 normal[] = {6, 14, 14, 3, 4, 13, 3, 8, 13, 6};
    static const Sint32 highlighted[] = {32, 40, 40, 29, 30,
                                         39, 29, 34, 39, 32};

    reset_fixture();
    StrGOODENDING(0);
    assert_string_rendered(ctx, 2, cols, normal, 10);

    reset_fixture();
    StrGOODENDING(1);
    assert_string_rendered(ctx, 2, cols, highlighted, 10);
}

static void test_str_badending_normal_and_highlighted_codes(test_context *ctx) {
    static const Sint32 cols[] = {0, 1, 2, 4, 5, 6, 7, 8, 9};
    static const Sint32 normal[] = {1, 0, 3, 4, 13, 3, 8, 13, 6};
    static const Sint32 highlighted[] = {27, 26, 29, 30, 39,
                                         29, 34, 39, 32};

    reset_fixture();
    StrBADENDING(0);
    assert_string_rendered(ctx, 3, cols, normal, 9);

    reset_fixture();
    StrBADENDING(1);
    assert_string_rendered(ctx, 3, cols, highlighted, 9);
}

static void test_str_penciltest_normal_and_highlighted_codes(
    test_context *ctx) {
    static const Sint32 cols[] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10};
    static const Sint32 normal[] = {15, 4, 13, 2, 8, 11, 19, 4, 18, 19};
    static const Sint32 highlighted[] = {41, 30, 39, 28, 34,
                                         37, 45, 30, 44, 45};

    reset_fixture();
    StrPENCILTEST(0);
    assert_string_rendered(ctx, 4, cols, normal, 10);

    reset_fixture();
    StrPENCILTEST(1);
    assert_string_rendered(ctx, 4, cols, highlighted, 10);
}

TEST_MAIN_BEGIN;
test_create_char_sets_exact_sprite_fields(&ctx);
test_create_existing_slot_returns_zero_without_side_effects(&ctx);
test_delete_char_clears_existing_sprite_and_ignores_empty(&ctx);
test_delete_all_walks_every_menu_slot(&ctx);
test_str_exit_normal_and_highlighted_codes(&ctx);
test_str_opening_normal_and_highlighted_codes(&ctx);
test_str_goodending_normal_and_highlighted_codes(&ctx);
test_str_badending_normal_and_highlighted_codes(&ctx);
test_str_penciltest_normal_and_highlighted_codes(&ctx);
TEST_MAIN_END

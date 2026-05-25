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
Sint32 ScrollCount;
sprite_bmp infoSprtBmp[1000];
void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);

static position_call position_calls[64];
static bitmap_call bitmap_calls[64];
static flags_call flags_calls[64];
static Sint32 created_handles[64];
static Sint32 deleted_handles[64];
static Sint32 position_call_count;
static Sint32 bitmap_call_count;
static Sint32 flags_call_count;
static Sint32 create_call_count;
static Sint32 delete_call_count;
static Sint32 next_handle;

#include "src/title/besttime/bestsprt.c"

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
        infoSprtBmp[i].order = i % 17;
    }

    context.sprites = sprite_table;
    s_ctx = &context;
    ScrollCount = 0;
    hmx_sprite_set_position_module = fake_sprite_set_position;
    hmx_sprite_set_bitmap_module = fake_sprite_set_bitmap;
    hmx_sprite_set_flags_module = fake_sprite_set_flags;
    position_call_count = 0;
    bitmap_call_count = 0;
    flags_call_count = 0;
    create_call_count = 0;
    delete_call_count = 0;
    next_handle = 10;
}

static void assert_created_char(test_context *ctx, Sint32 call, Sint32 handle,
                                Sint32 colom, Sint32 y, Sint32 bitmap,
                                Sint32 expected_y) {
    TEST_ASSERT_EQ_INT(ctx, handle, created_handles[call]);
    TEST_ASSERT_EQ_INT(ctx, (Uint32)handle, hSprMenu[y][colom]);
    TEST_ASSERT_TRUE(ctx, position_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, colom * 8 + 56, position_calls[call].x);
    TEST_ASSERT_EQ_INT(ctx, expected_y, position_calls[call].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].bitmap == bitmap_index(bitmap));
    TEST_ASSERT_TRUE(ctx, flags_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[call].flags);
    TEST_ASSERT_EQ_INT(ctx, 60 - infoSprtBmp[bitmap].order,
                       context.spr_level[handle]);
}

static void test_strmenu_maps_characters_and_scrolls_positions(
    test_context *ctx) {
    char text[] = " ,-7A.";

    reset_fixture();
    ScrollCount = 4;

    StrMenu(2, text);

    TEST_ASSERT_EQ_INT(ctx, 6, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, position_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, bitmap_call_count);
    TEST_ASSERT_EQ_INT(ctx, 6, flags_call_count);
    assert_created_char(ctx, 0, 10, 0, 2, 89, 87);
    assert_created_char(ctx, 1, 11, 1, 2, 90, 87);
    assert_created_char(ctx, 2, 12, 2, 2, 72, 87);
    assert_created_char(ctx, 3, 13, 3, 2, 7, 87);
    assert_created_char(ctx, 4, 14, 4, 2, 10, 87);
    assert_created_char(ctx, 5, 15, 5, 2, 91, 87);
}

static void test_strmenu2_uses_alternate_digit_and_letter_mapping(
    test_context *ctx) {
    char text[] = " ,-0A.";

    reset_fixture();
    ScrollCount = -1;

    StrMenu2(3, text);

    TEST_ASSERT_EQ_INT(ctx, 6, create_call_count);
    assert_created_char(ctx, 0, 10, 0, 3, 89, 71);
    assert_created_char(ctx, 1, 11, 1, 3, 90, 71);
    assert_created_char(ctx, 2, 12, 2, 3, 72, 71);
    assert_created_char(ctx, 3, 13, 3, 3, 36, 71);
    assert_created_char(ctx, 4, 14, 4, 3, 46, 71);
    assert_created_char(ctx, 5, 15, 5, 3, 91, 71);
}

static void test_recreating_existing_character_deletes_old_sprite_first(
    test_context *ctx) {
    char first[] = "A";
    char second[] = "B";

    reset_fixture();

    StrMenu(1, first);
    TEST_ASSERT_EQ_INT(ctx, 10, hSprMenu[1][0]);

    StrMenu(1, second);

    TEST_ASSERT_EQ_INT(ctx, 2, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, deleted_handles[0]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].sprite == sprite_handle(10));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].bitmap == 0);
    TEST_ASSERT_TRUE(ctx, flags_calls[1].sprite == sprite_handle(10));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[1].flags);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[10]);
    TEST_ASSERT_EQ_INT(ctx, 11, hSprMenu[1][0]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[2].bitmap == bitmap_index(11));
}

static void test_strmenu2_recreating_existing_character_deletes_old_sprite(
    test_context *ctx) {
    char first[] = "A";
    char second[] = "C";

    reset_fixture();

    StrMenu2(4, first);
    TEST_ASSERT_EQ_INT(ctx, 10, hSprMenu[4][0]);

    StrMenu2(4, second);

    TEST_ASSERT_EQ_INT(ctx, 2, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 10, deleted_handles[0]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].sprite == sprite_handle(10));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].bitmap == 0);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[10]);
    TEST_ASSERT_EQ_INT(ctx, 11, hSprMenu[4][0]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[2].bitmap == bitmap_index(48));
}

static void test_delete_all_removes_existing_sprites_and_ignores_empty_slots(
    test_context *ctx) {
    reset_fixture();
    hSprMenu[0][0] = 20;
    hSprMenu[10][27] = 21;
    context.spr_level[20] = 77;
    context.spr_level[21] = 88;

    DeleteAllCharSprt();

    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 20, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 21, deleted_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[10][27]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[21]);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[0].bitmap == 0);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[1].bitmap == 0);
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[0].flags);
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[1].flags);
}

TEST_MAIN_BEGIN;
test_strmenu_maps_characters_and_scrolls_positions(&ctx);
test_strmenu2_uses_alternate_digit_and_letter_mapping(&ctx);
test_recreating_existing_character_deletes_old_sprite_first(&ctx);
test_strmenu2_recreating_existing_character_deletes_old_sprite(&ctx);
test_delete_all_removes_existing_sprites_and_ignores_empty_slots(&ctx);
TEST_MAIN_END

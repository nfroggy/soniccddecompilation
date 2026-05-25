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

static position_call position_calls[32];
static bitmap_call bitmap_calls[32];
static flags_call flags_calls[32];
static Sint32 created_handles[32];
static Sint32 deleted_handles[32];
static Sint32 position_call_count;
static Sint32 bitmap_call_count;
static Sint32 flags_call_count;
static Sint32 create_call_count;
static Sint32 delete_call_count;
static Sint32 next_handle;

#include "src/title/stagetst/stgsprt.c"

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
        infoSprtBmp[i].order = 2000 + i;
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
    next_handle = 40;
}

static void assert_created_char(test_context *ctx, Sint32 call, Sint32 column,
                                Sint32 code_index) {
    Sint32 handle = created_handles[call];

    TEST_ASSERT_EQ_INT(ctx, handle, (Sint32)hSprMenu[column]);
    TEST_ASSERT_TRUE(ctx, position_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, column * 8 + 120, position_calls[call].x);
    TEST_ASSERT_EQ_INT(ctx, 80, position_calls[call].y);
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_TRUE(ctx, bitmap_calls[call].bitmap == bitmap_index(code_index));
    TEST_ASSERT_TRUE(ctx, flags_calls[call].sprite == sprite_handle(handle));
    TEST_ASSERT_EQ_INT(ctx, 0, flags_calls[call].flags);
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[code_index].order,
                       context.spr_level[handle]);
}

static void test_strmenu_maps_digits_letters_and_hyphen(test_context *ctx) {
    char menu[] = "A0-";

    reset_fixture();

    StrMenu(menu);

    TEST_ASSERT_EQ_INT(ctx, 3, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 0, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, position_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, bitmap_call_count);
    TEST_ASSERT_EQ_INT(ctx, 3, flags_call_count);
    assert_created_char(ctx, 0, 0, 21);
    assert_created_char(ctx, 1, 1, 11);
    assert_created_char(ctx, 2, 2, 47);
}

static void test_strmenu_replaces_existing_column_before_creating_new_sprite(
    test_context *ctx) {
    char first[] = "A";
    char second[] = "B";

    reset_fixture();

    StrMenu(first);
    TEST_ASSERT_EQ_INT(ctx, 40, (Sint32)hSprMenu[0]);
    TEST_ASSERT_EQ_INT(ctx, infoSprtBmp[21].order, context.spr_level[40]);

    StrMenu(second);

    TEST_ASSERT_EQ_INT(ctx, 2, create_call_count);
    TEST_ASSERT_EQ_INT(ctx, 1, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 40, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[40]);
    assert_created_char(ctx, 1, 0, 22);
}

static void test_delete_all_clears_only_existing_menu_sprites(
    test_context *ctx) {
    reset_fixture();
    hSprMenu[0] = 50;
    hSprMenu[11] = 51;
    context.spr_level[50] = 8;
    context.spr_level[51] = 9;

    DeleteAllCharSprt();

    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);
    TEST_ASSERT_EQ_INT(ctx, 50, deleted_handles[0]);
    TEST_ASSERT_EQ_INT(ctx, 51, deleted_handles[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, hSprMenu[11]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[50]);
    TEST_ASSERT_EQ_INT(ctx, 0, context.spr_level[51]);

    DeleteAllCharSprt();
    TEST_ASSERT_EQ_INT(ctx, 2, delete_call_count);
}

TEST_MAIN_BEGIN;
test_strmenu_maps_digits_letters_and_hyphen(&ctx);
test_strmenu_replaces_existing_column_before_creating_new_sprite(&ctx);
test_delete_all_clears_only_existing_menu_sprites(&ctx);
TEST_MAIN_END

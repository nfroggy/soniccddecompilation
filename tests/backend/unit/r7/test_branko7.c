#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_results[4];
static int hitchk_result_count;
static int hitchk_result_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin_value;
static Sint16 sinset_cos_value;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r7/branko7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    if (hitchk_result_index < hitchk_result_count) {
        return hitchk_results[hitchk_result_index++];
    }
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin_value;
    *cos = sinset_cos_value;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_hitchk_result(Sint16 value) {
    hitchk_results[hitchk_result_count++] = value;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actor_byte(sprite_status *actor, int index, Sint8 value) {
    ((Sint8 *)actor)[index] = value;
}

static void reset_branko7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    memset(hitchk_results, 0, sizeof(hitchk_results));
    hitchk_result_count = 0;
    hitchk_result_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin_value = 0;
    sinset_cos_value = 0;
}

static void test_branko7_tables_and_patterns(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, -16384, branko7_initbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, branko7_initbl[1]);
    TEST_ASSERT_EQ_INT(ctx, -32768, branko7_initbl[24]);
    TEST_ASSERT_EQ_INT(ctx, 0, branko7_initbl[27]);
    TEST_ASSERT_TRUE(ctx, branko7pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, branko7pat[1] == &pat1);
    TEST_ASSERT_TRUE(ctx, branko7pat[2] == &pat2);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BRANKO7_BASE, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BRANKO7_BASE + 1, pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BRANKO7_BASE + 2, pat2.spra[0].index);
}

static void test_branko7_initializes_master_and_children(test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko7_state();
    swing->xposi.w.h = 320;
    swing->yposi.w.h = 120;
    swing->userflag.b.h = 2;
    sinset_sin_value = 0;
    sinset_cos_value = 0;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    branko7(swing);

    TEST_ASSERT_EQ_INT(ctx, 32, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].patno);
    TEST_ASSERT_EQ_INT(ctx, 169, actwk[10].colino);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == swing);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == swing);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);

    reset_branko7_state();
    swing->xposi.w.h = 320;
    swing->yposi.w.h = 120;
    swing->userflag.b.h = 3;
    queue_actor(&actwk[10]);
    branko7_init(swing);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[10].actno);
}

static void test_branko7_initializes_existing_child_and_table_variant(
    test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko7_state();
    swing->xposi.w.h = 200;
    swing->yposi.w.h = 80;
    swing->actfree[18] = 1;
    swing->userflag.b.h = 0x60;
    sinset_sin_value = 0;
    sinset_cos_value = 0;
    branko7_init(swing);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_branko7_ride_check_paths(test_context *ctx) {
    sprite_status *swing = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_branko7_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 200;
    queue_hitchk_result(0);
    queue_hitchk_result(0);
    branko7_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 2, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == swing);
    TEST_ASSERT_TRUE(ctx, hitchk_player == player);

    reset_branko7_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 200;
    queue_hitchk_result(0);
    queue_hitchk_result(1);
    branko7_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 2, hitchk_count);

    reset_branko7_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 90;
    player->sprvsize = 14;
    queue_hitchk_result(1);
    branko7_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_branko7_positioning_and_direction_toggles(test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko7_state();
    set_actor_word(swing, 26, 254);
    set_actor_word(swing, 28, 256);
    set_actor_word(swing, 30, -256);
    set_actor_word(swing, 31, 1000);
    set_actor_word(swing, 33, 2);
    set_actor_word(swing, 27, 50);
    set_actor_word(swing, 29, 100);
    set_actor_byte(swing, 63, 7);
    set_actor_byte(swing, 64, 2);
    sinset_sin_value = 256;
    sinset_cos_value = -128;
    branko7_posiset(swing);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_angle);

    reset_branko7_state();
    swing->actfree[19] = 1;
    set_actor_word(swing, 26, -254);
    set_actor_word(swing, 28, 256);
    set_actor_word(swing, 30, -256);
    set_actor_word(swing, 31, 1000);
    set_actor_word(swing, 33, 2);
    set_actor_word(swing, 27, 50);
    set_actor_word(swing, 29, 100);
    set_actor_byte(swing, 64, 2);
    sinset_sin_value = -256;
    sinset_cos_value = 128;
    branko7_posiset(swing);
}

static void test_branko7_move_sets_speed_only_for_ride_piece(
    test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko7_state();
    swing->xposi.l = 100 << 16;
    swing->yposi.l = 50 << 16;
    swing->actfree[5] = 1;
    swing->actfree[18] = 0;
    set_actor_word(swing, 27, 50);
    set_actor_word(swing, 29, 100);
    set_actor_byte(swing, 64, 2);
    sinset_sin_value = 128;
    sinset_cos_value = 128;
    branko7_move(swing);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);

    reset_branko7_state();
    swing->xposi.l = 100 << 16;
    swing->yposi.l = 50 << 16;
    swing->actfree[5] = 2;
    swing->actfree[18] = 1;
    set_actor_word(swing, 27, 50);
    set_actor_word(swing, 29, 100);
    set_actor_byte(swing, 64, 2);
    sinset_sin_value = 128;
    sinset_cos_value = 128;
    queue_hitchk_result(0);
    queue_hitchk_result(0);
    branko7_move(swing);
    TEST_ASSERT_EQ_INT(ctx, 2, hitchk_count);
}

TEST_MAIN_BEGIN;
test_branko7_tables_and_patterns(&ctx);
test_branko7_initializes_master_and_children(&ctx);
test_branko7_initializes_existing_child_and_table_variant(&ctx);
test_branko7_ride_check_paths(&ctx);
test_branko7_positioning_and_direction_toggles(&ctx);
test_branko7_move_sets_speed_only_for_ride_piece(&ctx);
TEST_MAIN_END

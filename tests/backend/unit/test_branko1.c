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
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_results[4];
static int ridechk_result_count;
static int ridechk_result_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin_value;
static Sint16 sinset_cos_value;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/branko1.c"

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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    if (ridechk_result_index < ridechk_result_count)
        return ridechk_results[ridechk_result_index++];
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

static void queue_ridechk_result(Sint16 value) {
    ridechk_results[ridechk_result_count++] = value;
}

static void set_actfree_word(sprite_status *actor, int legacy_word,
                             Sint16 value) {
    int offset = (legacy_word - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_branko1_state(void) {
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
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    memset(ridechk_results, 0, sizeof(ridechk_results));
    ridechk_result_count = 0;
    ridechk_result_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin_value = 0;
    sinset_cos_value = 0;
}

static void test_branko1_tables_and_patterns(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, -16384, branko1_initbl[0].angle);
    TEST_ASSERT_EQ_INT(ctx, 8, branko1_initbl[0].accel);
    TEST_ASSERT_EQ_INT(ctx, -32768, branko1_initbl[6].angle);
    TEST_ASSERT_EQ_INT(ctx, 0, branko1_initbl[7].area2);
    TEST_ASSERT_TRUE(ctx, branko1pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, branko1pat[1] == &pat1);
    TEST_ASSERT_TRUE(ctx, branko1pat[2] == &pat2);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 515, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 516, pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 517, pat2.spra[0].index);
}

static void test_branko1_initializes_master_and_children(test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko1_state();
    swing->xposi.w.h = 320;
    swing->yposi.w.h = 120;
    swing->userflag.b.h = 2;
    sinset_sin_value = 0;
    sinset_cos_value = 0;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);

    branko1(swing);

    TEST_ASSERT_EQ_INT(ctx, 2, swing->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, swing->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, swing->sprpri);
    TEST_ASSERT_TRUE(ctx, swing->patbase == branko1pat);
    TEST_ASSERT_EQ_INT(ctx, 24, swing->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, swing->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].patno);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == swing);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == swing);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);
}

static void test_branko1_initializes_existing_child_without_spawning(
    test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko1_state();
    swing->xposi.w.h = 200;
    swing->yposi.w.h = 80;
    swing->actfree[18] = 1;
    swing->userflag.b.h = 0x60;
    sinset_sin_value = 0;
    sinset_cos_value = 0;

    branko1_init(swing);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, swing->r_no0);
    TEST_ASSERT_TRUE(ctx, swing->patbase == branko1pat);
}

static void test_branko1_ride_check_paths(test_context *ctx) {
    sprite_status *swing = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_branko1_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 200;
    queue_ridechk_result(0);
    queue_ridechk_result(0);
    branko1_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 4, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == swing);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[1]);

    reset_branko1_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 200;
    queue_ridechk_result(1);
    branko1_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 3, ridechk_count);

    reset_branko1_state();
    swing->yposi.w.h = 100;
    player->yposi.w.h = 90;
    player->sprvsize = 14;
    queue_ridechk_result(1);
    branko1_ridechk(swing);
    TEST_ASSERT_EQ_INT(ctx, 3, ridechk_count);
}

static void test_branko1_positioning_and_direction_toggles(test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko1_state();
    set_actfree_word(swing, 26, 510);
    set_actfree_word(swing, 28, 512);
    set_actfree_word(swing, 30, -512);
    set_actfree_word(swing, 31, 1000);
    set_actfree_word(swing, 33, 2);
    set_actfree_word(swing, 27, 50);
    set_actfree_word(swing, 29, 100);
    swing->actfree[18] = 2;
    sinset_sin_value = 256;
    sinset_cos_value = -128;
    branko1_posiset(swing);
    TEST_ASSERT_EQ_INT(ctx, 255, swing->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 5, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 132, swing->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 34, swing->yposi.w.h);

    reset_branko1_state();
    swing->actfree[19] = 1;
    set_actfree_word(swing, 26, -510);
    set_actfree_word(swing, 28, 512);
    set_actfree_word(swing, 30, -512);
    set_actfree_word(swing, 31, 1000);
    set_actfree_word(swing, 33, 2);
    set_actfree_word(swing, 27, 50);
    set_actfree_word(swing, 29, 100);
    swing->actfree[18] = 2;
    sinset_sin_value = -256;
    sinset_cos_value = 128;
    branko1_posiset(swing);
    TEST_ASSERT_EQ_INT(ctx, 0, swing->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 68, swing->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 66, swing->yposi.w.h);
}

static void test_branko1_move_sets_speed_only_for_ride_piece(
    test_context *ctx) {
    sprite_status *swing = &actwk[2];

    reset_branko1_state();
    swing->xposi.l = 100 << 16;
    swing->yposi.l = 50 << 16;
    swing->actfree[5] = 1;
    swing->actfree[18] = 0;
    set_actfree_word(swing, 27, 50);
    set_actfree_word(swing, 29, 100);
    sinset_sin_value = 128;
    sinset_cos_value = 128;
    branko1_move(swing);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);

    reset_branko1_state();
    swing->xposi.l = 100 << 16;
    swing->yposi.l = 50 << 16;
    swing->actfree[5] = 2;
    swing->actfree[18] = 2;
    set_actfree_word(swing, 27, 50);
    set_actfree_word(swing, 29, 100);
    sinset_sin_value = 128;
    sinset_cos_value = 128;
    queue_ridechk_result(0);
    queue_ridechk_result(0);
    branko1_move(swing);
    TEST_ASSERT_EQ_INT(ctx, 4, ridechk_count);
}

TEST_MAIN_BEGIN;
    test_branko1_tables_and_patterns(&ctx);
    test_branko1_initializes_master_and_children(&ctx);
    test_branko1_initializes_existing_child_without_spawning(&ctx);
    test_branko1_ride_check_paths(&ctx);
    test_branko1_positioning_and_direction_toggles(&ctx);
    test_branko1_move_sets_speed_only_for_ride_piece(&ctx);
TEST_MAIN_END

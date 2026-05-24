#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_results[4];
static int hitchk_result_count;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r5/iwa5roll.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
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

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    Sint16 result = 0;

    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    if (hitchk_count <= hitchk_result_count) {
        result = hitchk_results[hitchk_count - 1];
    }
    return result;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 8;
    sinset_cos = 16;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    memset(hitchk_results, 0, sizeof(hitchk_results));
    hitchk_result_count = 0;
}

static void reset_iwa5roll_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    reset_logs();
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_roll_children(int first_index, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        queue_actor(&actwk[first_index + i]);
    }
}

static void set_hitchk_result(int index, Sint16 value) {
    hitchk_results[index] = value;
    if (hitchk_result_count < index + 1) {
        hitchk_result_count = index + 1;
    }
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void assert_move_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_pattern_table_captures_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_iwa5roll[0] == &pat_iwa5roll_00);
    TEST_ASSERT_EQ_INT(ctx, 1, pat_iwa5roll_00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_iwa5roll_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_iwa5roll_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 429, pat_iwa5roll_00.spra[0].index);
}

static void test_init_allocates_full_orbit_for_time_variants(
    test_context *ctx) {
    sprite_status *main = &actwk[3];
    int i;
    int expected_angles[8] = {0, 2048, 16384, 18432,
                              -32768, -30720, -16384, -14336};

    reset_iwa5roll_state();
    main->actno = 39;
    main->xposi.l = 100 << 16;
    main->yposi.l = 200 << 16;
    queue_roll_children(20, 7);

    iwa5roll(main);

    TEST_ASSERT_EQ_INT(ctx, 7, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, main->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, main->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, main->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, main->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, main->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17152, main->sproffset);
    TEST_ASSERT_TRUE(ctx, main->patbase == pat_iwa5roll);
    TEST_ASSERT_EQ_INT(ctx, 0, get_actfree_word(main, 0));
    TEST_ASSERT_EQ_INT(ctx, 128, get_actfree_word(main, 12));
    TEST_ASSERT_EQ_INT(ctx, expected_angles[0], get_actfree_word(main, 10));
    TEST_ASSERT_EQ_INT(ctx, 100 << 16, get_actfree_long(main, 2));
    TEST_ASSERT_EQ_INT(ctx, 200 << 16, get_actfree_long(main, 6));

    for (i = 0; i < 7; ++i) {
        sprite_status *child = &actwk[20 + i];
        TEST_ASSERT_EQ_INT(ctx, 39, child->actno);
        TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
        TEST_ASSERT_EQ_INT(ctx, 3, get_actfree_word(child, 0));
        TEST_ASSERT_EQ_INT(ctx, 128, get_actfree_word(child, 12));
        TEST_ASSERT_EQ_INT(ctx, expected_angles[i + 1],
                           get_actfree_word(child, 10));
        TEST_ASSERT_EQ_INT(ctx, 100 << 16, get_actfree_long(child, 2));
        TEST_ASSERT_EQ_INT(ctx, 200 << 16, get_actfree_long(child, 6));
    }

    reset_iwa5roll_state();
    time_flag = 1;
    main->actno = 39;
    main->userflag.b.h = 1;
    main->xposi.l = 10 << 16;
    main->yposi.l = 20 << 16;

    iwa5roll(main);

    TEST_ASSERT_EQ_INT(ctx, -192, get_actfree_word(main, 12));

    reset_iwa5roll_state();
    time_flag = 2;
    main->actno = 39;
    main->xposi.l = 10 << 16;
    main->yposi.l = 20 << 16;

    iwa5roll(main);

    TEST_ASSERT_EQ_INT(ctx, 256, get_actfree_word(main, 12));
}

static void test_init_stops_cleanly_when_child_allocation_fails(
    test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_iwa5roll_state();
    main->actno = 39;
    main->xposi.l = 100 << 16;
    main->yposi.l = 200 << 16;
    queue_roll_children(20, 2);

    iwa5roll(main);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, get_actfree_word(main, 0));
    TEST_ASSERT_EQ_INT(ctx, 39, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 39, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[22].actno);
}

static void test_move_without_parent_updates_position_and_frameout_origin(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_iwa5roll_state();
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    actor->yposi.l = 200 << 16;
    actor->sprvsize = 8;
    set_actfree_word(actor, 0, 0);
    set_actfree_long(actor, 2, 100 << 16);
    set_actfree_long(actor, 6, 200 << 16);
    set_actfree_word(actor, 10, 0);
    set_actfree_word(actor, 12, 128);

    iwa5roll(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 106, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 203, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_move_callbacks(ctx, actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_move_collision_hit_runs_second_hitchk_with_restored_speed(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_iwa5roll_state();
    actor->r_no0 = 2;
    actor->xposi.l = 100 << 16;
    actor->yposi.l = 200 << 16;
    set_actfree_long(actor, 2, 100 << 16);
    set_actfree_long(actor, 6, 200 << 16);
    set_actfree_word(actor, 10, 0);
    set_actfree_word(actor, 12, 128);
    set_hitchk_result(0, 1);

    iwa5roll(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    assert_move_callbacks(ctx, actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_child_move_requires_live_parent(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_iwa5roll_state();
    parent->actno = 39;
    child->r_no0 = 2;
    child->xposi.l = 100 << 16;
    child->yposi.l = 200 << 16;
    set_actfree_word(child, 0, 3);
    set_actfree_long(child, 2, 100 << 16);
    set_actfree_long(child, 6, 200 << 16);
    set_actfree_word(child, 10, 0);
    set_actfree_word(child, 12, 128);

    iwa5roll(child);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);

    reset_logs();
    parent->actno = 0;

    iwa5roll(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_pattern_table_captures_literal_data(&ctx);
    test_init_allocates_full_orbit_for_time_variants(&ctx);
    test_init_stops_cleanly_when_child_allocation_fails(&ctx);
    test_move_without_parent_updates_position_and_frameout_origin(&ctx);
    test_move_collision_hit_runs_second_hitchk_with_restored_speed(&ctx);
    test_child_move_requires_live_parent(&ctx);
TEST_MAIN_END

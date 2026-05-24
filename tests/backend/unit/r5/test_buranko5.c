#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 colrevflag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r5/buranko5.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

static void reset_buranko5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    colrevflag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 128;
    sinset_cos = 256;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
}

static void reset_buranko5_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)((bits >> 24) & 255);
}

static Sint16 actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void queue_chain(sprite_status *base, int count) {
    for (int i = 0; i < count; ++i) {
        queue_actor(&base[i]);
    }
}

static void test_controller_init_allocates_chain_and_marks_radius(
    test_context *ctx) {
    sprite_status *ctrl = &actwk[3];

    reset_buranko5_state();
    ctrl->actno = 42;
    ctrl->xposi.w.h = 100;
    ctrl->yposi.w.h = 200;
    queue_chain(&actwk[40], 6);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 2, ctrl->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, ctrl->actflg);
    TEST_ASSERT_EQ_INT(ctx, 8, ctrl->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, ctrl->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, ctrl->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 3, ctrl->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 856, ctrl->sproffset);
    TEST_ASSERT_TRUE(ctx, ctrl->patbase == pat_buranko5);
    TEST_ASSERT_EQ_INT(ctx, 5, actfree_word(ctrl, 12));
    TEST_ASSERT_EQ_INT(ctx, 0, actfree_word(ctrl, 4));
    TEST_ASSERT_EQ_INT(ctx, 256, actfree_word(ctrl, 8));
    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk2_count);

    for (int i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 40 + i, ctrl->actfree[14 + i]);
        TEST_ASSERT_EQ_INT(ctx, 42, actwk[40 + i].actno);
        TEST_ASSERT_EQ_INT(ctx, 3, actfree_word(&actwk[40 + i], 0));
        TEST_ASSERT_EQ_INT(ctx, 100, actwk[40 + i].xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 200, actwk[40 + i].yposi.w.h);
        TEST_ASSERT_TRUE(ctx, actwk[40 + i].patbase == pat_buranko5);
    }
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[45].userflag.b.l);
}

static void test_controller_init_variants_and_allocation_failure(
    test_context *ctx) {
    sprite_status *ctrl = &actwk[3];

    reset_buranko5_state();
    ctrl->actno = 42;
    ctrl->userflag.b.h = 2;
    queue_chain(&actwk[40], 6);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 5, actfree_word(ctrl, 12));
    TEST_ASSERT_EQ_INT(ctx, -32768, actfree_word(ctrl, 4));

    reset_buranko5_state();
    ctrl->actno = 42;
    ctrl->userflag.b.h = 1;
    set_actfree_long(ctrl, 4, 123);
    set_actfree_long(ctrl, 8, -456);
    queue_chain(&actwk[40], 6);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 0, actfree_long(ctrl, 4));
    TEST_ASSERT_EQ_INT(ctx, 65280, actfree_long(ctrl, 8));

    reset_buranko5_state();
    ctrl->actno = 42;
    ctrl->userflag.b.h = 4;
    queue_chain(&actwk[40], 8);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 7, actfree_word(ctrl, 12));
    TEST_ASSERT_EQ_INT(ctx, 16384, actfree_word(ctrl, 4));
    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[47].userflag.b.l);

    reset_buranko5_state();
    ctrl->actno = 42;

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == ctrl);
}

static void test_controller_move_places_chain_and_clamps_swing(
    test_context *ctx) {
    sprite_status *ctrl = &actwk[3];

    reset_buranko5_state();
    ctrl->actno = 42;
    ctrl->xposi.l = 100 << 16;
    ctrl->yposi.l = 200 << 16;
    queue_chain(&actwk[40], 6);
    buranko5(ctrl);
    reset_buranko5_logs();
    set_actfree_long(ctrl, 0, 8);
    set_actfree_long(ctrl, 4, 10);
    set_actfree_long(ctrl, 8, 4);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 10, actfree_long(ctrl, 0));
    TEST_ASSERT_EQ_INT(ctx, -4, actfree_long(ctrl, 8));
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    for (int i = 0; i < 6; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 116 + i * 16, actwk[40 + i].xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 208 + i * 8, actwk[40 + i].yposi.w.h);
    }
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[45].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == ctrl);

    reset_buranko5_logs();
    set_actfree_long(ctrl, 0, -4);
    set_actfree_long(ctrl, 4, 10);
    set_actfree_long(ctrl, 8, -4);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 0, actfree_long(ctrl, 0));
    TEST_ASSERT_EQ_INT(ctx, 4, actfree_long(ctrl, 8));

    reset_buranko5_logs();
    set_actfree_long(ctrl, 0, 0);
    set_actfree_long(ctrl, 4, -10);
    set_actfree_long(ctrl, 8, 4);

    buranko5(ctrl);

    TEST_ASSERT_EQ_INT(ctx, 0, actfree_long(ctrl, 0));
    TEST_ASSERT_EQ_INT(ctx, -4, actfree_long(ctrl, 8));
}

static void test_middle_initializes_and_tracks_controller_liveness(
    test_context *ctx) {
    sprite_status *middle_actor = &actwk[40];

    reset_buranko5_state();
    middle_actor->userflag.b.l = 1;

    buranko5(middle_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, middle_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, middle_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 8, middle_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, middle_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, middle_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 3, middle_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 856, middle_actor->sproffset);

    reset_buranko5_logs();
    middle_actor->actfree[0] = 3;
    actwk[3].actno = 42;

    buranko5(middle_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == middle_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_buranko5_logs();
    actwk[3].actno = 0;

    buranko5(middle_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == middle_actor);
}

static void test_radius_ride_push_and_liveness_paths(test_context *ctx) {
    sprite_status *radius_actor = &actwk[45];

    reset_buranko5_state();
    radius_actor->userflag.b.l = -1;
    radius_actor->actfree[0] = 3;
    radius_actor->xspeed.w = 123;
    actwk[3].actno = 42;
    actwk[0].xposi.w.h = 1000;
    ridechk_result = 1;

    buranko5(radius_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, radius_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, radius_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 32, radius_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 32, radius_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, radius_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, radius_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17180, radius_actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 123, radius_actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == radius_actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1002, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == radius_actor);

    reset_buranko5_logs();
    colrevflag = 1;
    ridechk_result = 1;
    actwk[0].xposi.w.h = 1000;

    buranko5(radius_actor);

    TEST_ASSERT_EQ_INT(ctx, 998, actwk[0].xposi.w.h);

    reset_buranko5_logs();
    ridechk_result = 0;

    buranko5(radius_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_buranko5_logs();
    actwk[3].actno = 0;

    buranko5(radius_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == radius_actor);
}

TEST_MAIN_BEGIN;
    test_controller_init_allocates_chain_and_marks_radius(&ctx);
    test_controller_init_variants_and_allocation_failure(&ctx);
    test_controller_move_places_chain_and_clamps_swing(&ctx);
    test_middle_initializes_and_tracks_controller_liveness(&ctx);
    test_radius_ride_push_and_liveness_paths(&ctx);
TEST_MAIN_END

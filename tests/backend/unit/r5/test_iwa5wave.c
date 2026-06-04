#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r5/iwa5wave.c"

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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

static void reset_iwa5wave_state(void) {
    memset(actwk, 0, sizeof(actwk));
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
    sinset_sin = 0;
    sinset_cos = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
}

static void reset_iwa5wave_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_init_sets_main_and_child_wave_segments(test_context *ctx) {
    sprite_status *main = &actwk[3];
    sprite_status *child = &actwk[9];

    reset_iwa5wave_state();
    main->actno = 41;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;
    queue_actor(child);

    iwa5wave(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, main->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, main->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, main->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 32, main->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, main->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17543, main->sproffset);
    TEST_ASSERT_TRUE(ctx, main->patbase == pat_iwa5wave);
    TEST_ASSERT_EQ_INT(ctx, 164, main->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, get_work(main)->origin_x);
    TEST_ASSERT_EQ_INT(ctx, 200, get_work(main)->origin_y);
    TEST_ASSERT_EQ_INT(ctx, -32768, get_work(main)->angle);
    TEST_ASSERT_EQ_INT(ctx, ((100 << 16) - 0x800000),
                       get_work(main)->left_bound);
    TEST_ASSERT_EQ_INT(ctx, ((100 << 16) + 0x800000),
                       get_work(main)->right_bound);

    TEST_ASSERT_EQ_INT(ctx, 41, child->actno);
    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 36, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, get_work(child)->parent_actor);
    TEST_ASSERT_EQ_INT(ctx, 200, get_work(child)->origin_y);
    TEST_ASSERT_EQ_INT(ctx, -32768, get_work(child)->angle);
    TEST_ASSERT_EQ_INT(ctx, get_work(main)->left_bound,
                       get_work(child)->left_bound);
    TEST_ASSERT_EQ_INT(ctx, get_work(main)->right_bound,
                       get_work(child)->right_bound);
}

static void test_init_allocation_failure_keeps_only_main_segment(
    test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_iwa5wave_state();
    main->actno = 41;
    main->userflag.b.h = 1;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;

    iwa5wave(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 164, main->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, get_work(main)->angle);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[9].r_no0);
}

static void test_main_move_wraps_x_sets_wave_y_and_calls_frameout_origin(
    test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_iwa5wave_state();
    main->actno = 41;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;
    iwa5wave(main);
    reset_iwa5wave_logs();
    ridechk_result = 1;

    iwa5wave(main);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 129, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 200, main->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -128, main->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 16, main->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == main);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);

    reset_iwa5wave_logs();
    main->xposi.l = get_work(main)->left_bound;

    iwa5wave(main);

    TEST_ASSERT_EQ_INT(ctx, get_work(main)->right_bound, main->xposi.l);
    TEST_ASSERT_EQ_INT(ctx,
                       (Sint16)((get_work(main)->right_bound -
                                 get_work(main)->left_bound) >>
                                8),
                       main->xspeed.w);
}

static void test_child_moves_with_live_parent_or_frames_out_when_parent_gone(
    test_context *ctx) {
    sprite_status *main = &actwk[3];
    sprite_status *child = &actwk[9];

    reset_iwa5wave_state();
    main->actno = 41;
    main->xposi.w.h = 100;
    main->yposi.w.h = 200;
    queue_actor(child);
    iwa5wave(main);
    reset_iwa5wave_logs();

    iwa5wave(child);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 129, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_iwa5wave_logs();
    main->actno = 0;

    iwa5wave(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_init_sets_main_and_child_wave_segments(&ctx);
    test_init_allocation_failure_keeps_only_main_segment(&ctx);
    test_main_move_wraps_x_sets_wave_y_and_calls_frameout_origin(&ctx);
    test_child_moves_with_live_parent_or_frames_out_when_parent_gone(&ctx);
TEST_MAIN_END

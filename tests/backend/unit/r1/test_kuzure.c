#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_results[32];
static int ridechk_result_count;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int actwkchk_count;
static sprite_status *actwkchk_queue[32];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r1/kuzur11a.c"
#include "src/r1/kuzure.c"

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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    Sint16 result = 0;

    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    if (ridechk_count <= ridechk_result_count) {
        result = ridechk_results[ridechk_count - 1];
    }
    return result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_ridechk_result(int index, Sint16 value) {
    ridechk_results[index] = value;
    if (ridechk_result_count < index + 1) {
        ridechk_result_count = index + 1;
    }
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    memset(ridechk_results, 0, sizeof(ridechk_results));
    ridechk_result_count = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void assert_actionsub_for(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void assert_frameout_s_for(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_acta_init_positive_uses_a_shape(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();

    acta_init(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17598, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == patbase_kuzure_a);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 40, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 40, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 34, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
}

static void test_acta_init_negative_uses_b_shape_and_flip(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->userflag.b.h = -16;

    acta_init(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_TRUE(ctx, actor->patbase == patbase_kuzure_b);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->sprvsize);
}

static void test_acta_init_negative_height_table_abs_value(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    Uint8 custom_table[2] = {2, 255};
    Uint8 *saved_tblb0;

    reset_state();
    actor->userflag.b.h = -16;
    saved_tblb0 = tblb[0];
    tblb[0] = custom_table;

    acta_init(actor);

    tblb[0] = saved_tblb0;
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->sprvsize);
}

static void test_kuzureru_tikei_initializes_and_frames_nearby(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();

    kuzureru_tikei(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    assert_actionsub_for(ctx, actor);
    assert_frameout_s_for(ctx, actor);
}

static void test_acta_check_without_ride_waits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->r_no0 = 2;

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_acta_check_positive_ride_spawns_a_pieces(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *first_visible = &actwk[22];

    reset_state();
    actor->actno = 77;
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    set_ridechk_result(0, 1);
    for (int i = 0; i < 20; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, first_visible->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, first_visible->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, first_visible->sprpri);
    TEST_ASSERT_TRUE(ctx, first_visible->patbase == patc);
    TEST_ASSERT_EQ_INT(ctx, 0x20000,
                       kuzure_work_get(first_visible)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 77, first_visible->actno);
    TEST_ASSERT_EQ_INT(ctx, 4, first_visible->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, first_visible->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 224, first_visible->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 33, kuzure_work_get(first_visible)->wait_timer);
}

static void test_acta_check_flipped_ride_spawns_a_pieces(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *first_visible = &actwk[22];

    reset_state();
    actor->actno = 88;
    actor->r_no0 = 2;
    actor->userflag.b.h = 16;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    set_ridechk_result(0, 1);
    for (int i = 0; i < 20; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 20, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 5, first_visible->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, first_visible->cddat);
    TEST_ASSERT_EQ_INT(ctx, 100, first_visible->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 33, kuzure_work_get(first_visible)->wait_timer);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_acta_check_negative_ride_spawns_b_pieces(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *first_piece = &actwk[20];

    reset_state();
    actor->actno = 91;
    actor->r_no0 = 2;
    actor->userflag.b.h = -32;
    actor->xposi.w.h = 120;
    actor->yposi.w.h = 240;
    set_ridechk_result(0, 1);
    for (int i = 0; i < 6; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_TRUE(ctx, first_piece->patbase == patd);
    TEST_ASSERT_EQ_INT(ctx, 0x20000,
                       kuzure_work_get(first_piece)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 91, first_piece->actno);
    TEST_ASSERT_EQ_INT(ctx, 4, first_piece->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 240, first_piece->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8, first_piece->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 18, first_piece->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, first_piece->patno);
    TEST_ASSERT_EQ_INT(ctx, 80, first_piece->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 9, kuzure_work_get(first_piece)->wait_timer);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_acta_check_negative_player_speed_reverses_b_pieces(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *first_piece = &actwk[20];

    reset_state();
    actor->actno = 91;
    actor->r_no0 = 2;
    actor->userflag.b.h = -64;
    actor->xposi.w.h = 120;
    actor->yposi.w.h = 240;
    actwk[0].xspeed.w = -5;
    set_ridechk_result(0, 1);
    for (int i = 0; i < 6; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, first_piece->patno);
    TEST_ASSERT_EQ_INT(ctx, 160, first_piece->xposi.w.h);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_acta_check_negative_shift_reverses_b_pieces(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *first_piece = &actwk[20];

    reset_state();
    actor->actno = 91;
    actor->r_no0 = 2;
    actor->userflag.b.h = -96;
    actor->xposi.w.h = 120;
    actor->yposi.w.h = 240;
    set_ridechk_result(0, 1);
    for (int i = 0; i < 6; ++i) {
        queue_actwkchk(&actwk[20 + i]);
    }

    acta_check(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, first_piece->patno);
    TEST_ASSERT_EQ_INT(ctx, 160, first_piece->xposi.w.h);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_actb_wait_timer_runs_out_and_clears_ride(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->r_no0 = 4;
    kuzure_work_get(actor)->top_piece = 255;
    kuzure_work_get(actor)->wait_timer = 1;
    set_ridechk_result(0, 1);

    actb_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, kuzure_work_get(actor)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
}

static void test_actb_wait_nonzero_timer_keeps_waiting(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->r_no0 = 4;
    kuzure_work_get(actor)->top_piece = 255;
    kuzure_work_get(actor)->wait_timer = 2;
    set_ridechk_result(0, 1);

    actb_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, kuzure_work_get(actor)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_actb_wait_without_top_piece_skips_ride_check(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->r_no0 = 4;
    kuzure_work_get(actor)->wait_timer = 1;

    actb_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_actb_down_accelerates_until_far_below_player(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->yposi.l = 100 << 16;
    actwk[0].yposi.w.h = 90;
    kuzure_work_get(actor)->y_velocity = 0x20000;

    actb_down(actor);

    TEST_ASSERT_EQ_INT(ctx, 102, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0x24000, kuzure_work_get(actor)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    actor->yposi.l = 700 << 16;
    actwk[0].yposi.w.h = 100;

    actb_down(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_actb_init_a_allocator_failure_clears_ride(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->actno = 11;
    actor->r_no0 = 4;
    set_ridechk_result(0, 1);

    actb_init_a(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_actb_init_b_allocator_failure_without_ride(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->actno = 11;
    actor->r_no0 = 4;
    actor->userflag.b.h = -32;

    actb_init_b(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_actb_init_b_allocator_failure_clears_ride(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_state();
    actor->actno = 11;
    actor->r_no0 = 4;
    actor->userflag.b.h = -32;
    set_ridechk_result(0, 1);

    actb_init_b(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

TEST_MAIN_BEGIN;
    test_acta_init_positive_uses_a_shape(&ctx);
    test_acta_init_negative_uses_b_shape_and_flip(&ctx);
    test_acta_init_negative_height_table_abs_value(&ctx);
    test_kuzureru_tikei_initializes_and_frames_nearby(&ctx);
    test_acta_check_without_ride_waits(&ctx);
    test_acta_check_positive_ride_spawns_a_pieces(&ctx);
    test_acta_check_flipped_ride_spawns_a_pieces(&ctx);
    test_acta_check_negative_ride_spawns_b_pieces(&ctx);
    test_acta_check_negative_player_speed_reverses_b_pieces(&ctx);
    test_acta_check_negative_shift_reverses_b_pieces(&ctx);
    test_actb_wait_timer_runs_out_and_clears_ride(&ctx);
    test_actb_wait_nonzero_timer_keeps_waiting(&ctx);
    test_actb_wait_without_top_piece_skips_ride_check(&ctx);
    test_actb_down_accelerates_until_far_below_player(&ctx);
    test_actb_init_a_allocator_failure_clears_ride(&ctx);
    test_actb_init_b_allocator_failure_without_ride(&ctx);
    test_actb_init_b_allocator_failure_clears_ride(&ctx);
TEST_MAIN_END

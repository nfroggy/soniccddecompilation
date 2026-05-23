#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[16];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int sinset_count;
static Uint8 sinset_requests[16];
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 enemy_suicide(sprite_status *pActwk);

#include "src/r8/hachi8.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_actor = pActwk;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < 16) {
        sinset_requests[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    ++enemy_suicide_count;
    (void)pActwk;
    return enemy_suicide_result;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    sinset_count = 0;
    memset(sinset_requests, 0, sizeof(sinset_requests));
    sinset_sin = 64;
    sinset_cos = 32;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
}

static void reset_hachi8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void spawn_electric_body(sprite_status *body_actor,
                                sprite_status *ball0,
                                sprite_status *ball1) {
    queue_actwkchk2(ball0);
    queue_actwkchk2(ball1);
    body_actor->actno = 46;
    body_actor->actflg = 128;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;

    hachi8(body_actor);
    reset_logs();
}

static void spawn_b_body(sprite_status *body_actor, sprite_status *ball0) {
    queue_actwkchk2(ball0);
    body_actor->actno = 46;
    body_actor->actflg = 128;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;
    body_actor->userflag.b.h = 1;

    body(body_actor);
    reset_logs();
}

static void spawn_ball_with_links(sprite_status *ball_actor, int link_count) {
    int i;

    for (i = 0; i < link_count; ++i) {
        queue_actwkchk2(&actwk[30 + i]);
    }
    hachi8(ball_actor);
    reset_logs();
}

static void drive_hachi8(sprite_status *actor) {
    reset_logs();
    hachi8(actor);
}

static void assert_move_callbacks(test_context *ctx, sprite_status *actor,
                                  Sint16 frameout_x) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, frameout_x, frameout_s00_xpos);
}

static void test_hachi8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_EQ_INT(ctx, 2, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[3]);
    TEST_ASSERT_TRUE(ctx, pat_hachi8_e[0] == &body_pat0);
    TEST_ASSERT_TRUE(ctx, pat_hachi8_e[3] == &ball_pat0);
    TEST_ASSERT_TRUE(ctx, pat_hachi8_b[2] == &kusari_01);
    TEST_ASSERT_TRUE(ctx, pat_hachi8_b[3] == &ball_pat1);
    TEST_ASSERT_EQ_INT(ctx, -19, body_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -20, body_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 469, body_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 470, body_pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 471, kusari_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 472, ball_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 473, kusari_01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 474, ball_pat1.spra[0].index);
}

static void test_hachi8_body_suicide_return_leaves_actor_untouched(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_hachi8_state();
    actor->actno = 46;
    actor->xposi.w.h = 100;
    enemy_suicide_result = -1;

    hachi8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_hachi8_body_initializes_electric_body_and_two_children(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];
    sprite_status *ball0 = &actwk[20];
    sprite_status *ball1 = &actwk[21];

    reset_hachi8_state();
    queue_actwkchk2(ball0);
    queue_actwkchk2(ball1);
    body_actor->actno = 46;
    body_actor->actflg = 128;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;

    hachi8(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == body_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_hachi8_body_second_child_failure_frames_body_out(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];
    sprite_status *ball0 = &actwk[20];

    reset_hachi8_state();
    queue_actwkchk2(ball0);
    body_actor->actno = 46;

    body(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body_actor);
}

static void test_hachi8_body_first_child_failure_frames_body_out(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];

    reset_hachi8_state();
    body_actor->actno = 46;

    body(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_hachi8_body_b_type_spawns_one_child_then_moves(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];
    sprite_status *ball0 = &actwk[20];

    reset_hachi8_state();
    queue_actwkchk2(ball0);
    body_actor->actno = 46;
    body_actor->actflg = 128;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;
    body_actor->userflag.b.h = 1;

    body(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 3, sinset_requests[0]);
    assert_move_callbacks(ctx, body_actor, 100);
}

static void test_hachi8_body_move_waits_then_flips_on_negative_timer(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];
    sprite_status *ball0 = &actwk[20];
    sprite_status *ball1 = &actwk[21];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball0, ball1);

    body_move(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 4, sinset_requests[0]);
    assert_move_callbacks(ctx, body_actor, 100);

    drive_hachi8(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 8, sinset_requests[0]);
    assert_move_callbacks(ctx, body_actor, 100);
}

static void test_hachi8_body_existing_routine_dispatches_to_move(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[3];
    sprite_status *ball0 = &actwk[20];
    sprite_status *ball1 = &actwk[21];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball0, ball1);

    hachi8(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    assert_move_callbacks(ctx, body_actor, 100);
}

static void prepare_parent_for_ball(Sint16 x, Sint16 y) {
    actwk[4].actno = 46;
    actwk[4].xposi.w.h = x;
    actwk[4].yposi.w.h = y;
}

static void test_hachi8_ball_type_minus_one_spawns_ten_chain_links(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    for (int i = 0; i < 10; ++i) {
        queue_actwkchk2(&actwk[20 + i]);
    }

    hachi8(ball_actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == ball_actor);
}

static void test_hachi8_ball_type_minus_two_spawns_four_chain_links(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball0 = &actwk[5];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball0, ball1);
    ball_actor = ball1;
    for (int i = 0; i < 4; ++i) {
        queue_actwkchk2(&actwk[20 + i]);
    }

    hachi8(ball_actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 16, sinset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_hachi8_ball_allocator_failure_frames_out(test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    queue_actwkchk2(&actwk[20]);

    hachi8(ball_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == ball_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_hachi8_ball_move_reverses_at_positive_threshold(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];
    int i;

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    spawn_ball_with_links(ball_actor, 10);
    for (i = 0; i < 58; ++i) {
        drive_hachi8(ball_actor);
    }

    reset_logs();
    hachi8(ball_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_hachi8_ball_move_reverses_at_negative_threshold(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];
    int i;

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    spawn_ball_with_links(ball_actor, 10);
    for (i = 0; i < 188; ++i) {
        drive_hachi8(ball_actor);
    }

    reset_logs();
    hachi8(ball_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_hachi8_kusari_initializes_and_moves_when_parent_alive(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];
    sprite_status *link = &actwk[30];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    spawn_ball_with_links(ball_actor, 10);

    hachi8(link);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == link);
}

static void test_hachi8_kusari_existing_routine_dispatches_to_move(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];
    sprite_status *link = &actwk[30];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    spawn_ball_with_links(ball_actor, 10);

    hachi8(link);
    reset_logs();
    hachi8(link);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == link);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_hachi8_kusari_frames_out_when_parent_is_not_hachi(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[4];
    sprite_status *ball_actor = &actwk[5];
    sprite_status *ball1 = &actwk[6];
    sprite_status *link = &actwk[30];

    reset_hachi8_state();
    spawn_electric_body(body_actor, ball_actor, ball1);
    spawn_ball_with_links(ball_actor, 10);
    body_actor->actno = 45;

    hachi8(link);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == link);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_hachi8_patterns_capture_literal_data(&ctx);
    test_hachi8_body_suicide_return_leaves_actor_untouched(&ctx);
    test_hachi8_body_initializes_electric_body_and_two_children(&ctx);
    test_hachi8_body_second_child_failure_frames_body_out(&ctx);
    test_hachi8_body_first_child_failure_frames_body_out(&ctx);
    test_hachi8_body_b_type_spawns_one_child_then_moves(&ctx);
    test_hachi8_body_move_waits_then_flips_on_negative_timer(&ctx);
    test_hachi8_body_existing_routine_dispatches_to_move(&ctx);
    test_hachi8_ball_type_minus_one_spawns_ten_chain_links(&ctx);
    test_hachi8_ball_type_minus_two_spawns_four_chain_links(&ctx);
    test_hachi8_ball_allocator_failure_frames_out(&ctx);
    test_hachi8_ball_move_reverses_at_positive_threshold(&ctx);
    test_hachi8_ball_move_reverses_at_negative_threshold(&ctx);
    test_hachi8_kusari_initializes_and_moves_when_parent_alive(&ctx);
    test_hachi8_kusari_existing_routine_dispatches_to_move(&ctx);
    test_hachi8_kusari_frames_out_when_parent_is_not_hachi(&ctx);
TEST_MAIN_END

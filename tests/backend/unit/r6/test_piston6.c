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
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r6/piston6.c"

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
    return 0;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_piston6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
}

static void test_piston6_init_sets_layout_and_two_side_children(
    test_context *ctx) {
    sprite_status *piston = &actwk[5];
    sprite_status *left = &actwk[20];
    sprite_status *right = &actwk[21];

    reset_piston6_state();
    piston->actno = 32;
    piston->xposi.w.h = 100;
    piston->yposi.w.h = 200;
    queue_actor(left);
    queue_actor(right);

    piston6_init(piston);

    TEST_ASSERT_EQ_INT(ctx, 2, piston->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, piston->actflg);
    TEST_ASSERT_EQ_INT(ctx, 832, piston->sproffset);
    TEST_ASSERT_TRUE(ctx, piston->patbase == piston6pat);
    TEST_ASSERT_EQ_INT(ctx, 40, piston->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 32, piston->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 32, left->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, left->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 132, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, left->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, left->actflg);
    TEST_ASSERT_TRUE(ctx, left->patbase == piston6pat);
    TEST_ASSERT_EQ_INT(ctx, 40, left->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, left->patno);

    TEST_ASSERT_EQ_INT(ctx, 32, right->actno);
    TEST_ASSERT_EQ_INT(ctx, 68, right->xposi.w.h);
}

static void test_piston6_init_allocation_failure_keeps_parent_only(
    test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston->xposi.w.h = 100;
    piston->yposi.w.h = 200;

    piston6_init(piston);

    TEST_ASSERT_EQ_INT(ctx, 2, piston->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);
}

static void test_piston6_move_extends_and_uses_ride_check(test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston->yposi.w.h = 200;
    piston6_init(piston);

    piston6_move(piston);

    TEST_ASSERT_EQ_INT(ctx, 8, piston6_get_work(piston)->extension);
    TEST_ASSERT_EQ_INT(ctx, 192, piston->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == piston);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
}

static void test_piston6_move_clears_ride_when_extended(test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston->yposi.w.h = 200;
    piston6_init(piston);
    piston6_get_work(piston)->extension = 40;

    piston6_move(piston);

    TEST_ASSERT_EQ_INT(ctx, 48, piston6_get_work(piston)->extension);
    TEST_ASSERT_EQ_INT(ctx, 152, piston->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == piston);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
}

static void test_piston6_move_retracting_uses_ride_check_at_end(
    test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston->yposi.w.h = 200;
    piston6_init(piston);
    piston6_get_work(piston)->wait_timer = 1;
    piston6_get_work(piston)->extension = 10;
    piston6_get_work(piston)->retracting = 1;

    piston6_move(piston);

    TEST_ASSERT_EQ_INT(ctx, 9, piston6_get_work(piston)->extension);
    TEST_ASSERT_EQ_INT(ctx, 191, piston->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
}

static void test_piston6_sub_caps_and_restarts_motion_phases(
    test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston6_get_work(piston)->extension = 72;

    piston6_sub(piston);

    TEST_ASSERT_EQ_INT(ctx, 80, piston6_get_work(piston)->extension);
    TEST_ASSERT_EQ_INT(ctx, 1, piston6_get_work(piston)->retracting);
    TEST_ASSERT_EQ_INT(ctx, 60, piston6_get_work(piston)->wait_timer);

    piston6_get_work(piston)->wait_timer = 0;
    piston6_get_work(piston)->extension = 1;
    piston6_get_work(piston)->retracting = 1;

    piston6_sub(piston);

    TEST_ASSERT_EQ_INT(ctx, 0, piston6_get_work(piston)->extension);
    TEST_ASSERT_EQ_INT(ctx, 0, piston6_get_work(piston)->retracting);
    TEST_ASSERT_EQ_INT(ctx, 60, piston6_get_work(piston)->wait_timer);

    piston6_get_work(piston)->wait_timer = 2;
    piston6_get_work(piston)->extension = 12;

    piston6_sub(piston);

    TEST_ASSERT_EQ_INT(ctx, 1, piston6_get_work(piston)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 12, piston6_get_work(piston)->extension);
}

static void test_piston6_side_tracks_live_parent(test_context *ctx) {
    sprite_status *piston = &actwk[5];
    sprite_status *side = &actwk[20];

    reset_piston6_state();
    piston->actno = 32;
    piston->xposi.w.h = 100;
    piston->yposi.w.h = 200;
    queue_actor(side);
    piston6_init(piston);
    piston->yposi.w.h = 150;

    piston6(side);

    TEST_ASSERT_EQ_INT(ctx, 150, side->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == side);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_piston6_side_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *piston = &actwk[5];
    sprite_status *side = &actwk[20];

    reset_piston6_state();
    piston->actno = 32;
    piston->xposi.w.h = 100;
    piston->yposi.w.h = 200;
    queue_actor(side);
    piston6_init(piston);
    piston->actno = 0;

    piston6(side);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == side);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_piston6_entry_dispatches_parent_callbacks(test_context *ctx) {
    sprite_status *piston = &actwk[5];

    reset_piston6_state();
    piston->yposi.w.h = 200;

    piston6(piston);

    TEST_ASSERT_EQ_INT(ctx, 2, piston->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 192, piston->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == piston);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == piston);
}

TEST_MAIN_BEGIN;
    test_piston6_init_sets_layout_and_two_side_children(&ctx);
    test_piston6_init_allocation_failure_keeps_parent_only(&ctx);
    test_piston6_move_extends_and_uses_ride_check(&ctx);
    test_piston6_move_clears_ride_when_extended(&ctx);
    test_piston6_move_retracting_uses_ride_check_at_end(&ctx);
    test_piston6_sub_caps_and_restarts_motion_phases(&ctx);
    test_piston6_side_tracks_live_parent(&ctx);
    test_piston6_side_frames_out_when_parent_is_gone(&ctx);
    test_piston6_entry_dispatches_parent_callbacks(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Sint16 waterposi_m;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_source;
static sprite_status *actwkchk2_queue[2];
static int actwkchk2_queue_count;
static int actwkchk2_queue_pos;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint16 emycol_d(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/harid4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_source = pActwk;
    if (actwkchk2_queue_pos >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return 1;
    }

    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_pos++];
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return 0;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    waterposi_m = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_source = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_pos = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void test_harid4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_harid4[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_harid4[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 2, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -64, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_HARID4_BASE + 1, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, 32, pat00.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_HARID4_BASE, pat00.spra[1].index);
}

static void test_harid4_init_above_water_spawns_child(test_context *ctx) {
    sprite_status *spike = &actwk[5];
    sprite_status *child = &actwk[20];

    reset_state();
    spike->actno = 50;
    spike->xposi.w.h = 100;
    spike->yposi.w.h = 200;
    waterposi_m = 220;
    queue_actwkchk2(child);

    harid4(spike);

    TEST_ASSERT_EQ_INT(ctx, 2, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, spike->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, spike->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, spike->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, spike->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 64, spike->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 928, spike->sproffset);
    TEST_ASSERT_TRUE(ctx, spike->patbase == pat_harid4);
    TEST_ASSERT_EQ_INT(ctx, 12288, harid4_work_get(spike)->acceleration);
    TEST_ASSERT_EQ_INT(ctx, 20, harid4_work_get(spike)->child_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_source == spike);

    TEST_ASSERT_EQ_INT(ctx, 50, child->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, child->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 100, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 248, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5, harid4_work_get(child)->parent_actor);
    TEST_ASSERT_EQ_INT(ctx, 176, child->colino);
    TEST_ASSERT_EQ_INT(ctx, 4, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, child->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 928, child->sproffset);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_harid4);
    TEST_ASSERT_EQ_INT(ctx, 1, child->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == spike);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_harid4_init_below_water_uses_slow_acceleration(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_state();
    spike->xposi.w.h = 100;
    spike->yposi.w.h = 200;
    waterposi_m = 100;

    act_init(spike);

    TEST_ASSERT_EQ_INT(ctx, 4096, harid4_work_get(spike)->acceleration);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
}

static void test_harid4_inactive_initialized_actor_only_draws(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_state();
    spike->r_no0 = 2;
    spike->actflg = 0;

    harid4(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_harid4_child_entry_frames_when_parent_missing(
    test_context *ctx) {
    sprite_status *child = &actwk[20];
    sprite_status *parent = &actwk[5];

    reset_state();
    child->userflag.b.h = -1;
    harid4_work_get(child)->parent_actor = 5;
    parent->actno = 0;

    harid4(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_state();
    child->userflag.b.h = -1;
    harid4_work_get(child)->parent_actor = 5;
    parent->actno = 50;

    harid4(child);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_harid4_wait_detection_edges(test_context *ctx) {
    sprite_status *spike = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    spike->xposi.w.h = 100;
    spike->yposi.w.h = 200;
    player->xposi.w.h = 100;
    player->yposi.w.h = 250;

    act_wait(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, spike->r_no0);

    player->yposi.w.h = 264;
    player->xposi.w.h = 120;
    act_wait(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, spike->r_no0);

    player->xposi.w.h = 100;
    act_wait(spike);

    TEST_ASSERT_EQ_INT(ctx, 2, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 256, spike->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 20, harid4_work_get(spike)->timer);
}

static void test_harid4_slide_updates_parent_and_child(test_context *ctx) {
    sprite_status *spike = &actwk[5];
    sprite_status *child = &actwk[20];

    reset_state();
    spike->r_no0 = 4;
    spike->yposi.l = 200 << 16;
    child->yposi.l = 248 << 16;
    harid4_work_get(spike)->child_actor = 20;
    harid4_work_get(spike)->timer = 0;
    harid4_work_get(spike)->y_velocity = 65536;

    act_slide(spike);

    TEST_ASSERT_EQ_INT(ctx, 6, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 201, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 249, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, harid4_work_get(spike)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 30, harid4_work_get(spike)->timer);
}

static void test_harid4_slide1_counts_down_then_advances(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_state();
    spike->r_no0 = 6;
    harid4_work_get(spike)->timer = 1;

    act_slide1(spike);

    TEST_ASSERT_EQ_INT(ctx, 6, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, harid4_work_get(spike)->timer);

    act_slide1(spike);

    TEST_ASSERT_EQ_INT(ctx, 8, spike->r_no0);
}

static void test_harid4_down_hits_floor_and_removes_child(test_context *ctx) {
    sprite_status *spike = &actwk[5];
    sprite_status *child = &actwk[20];

    reset_state();
    spike->r_no0 = 8;
    spike->yposi.l = 200 << 16;
    child->yposi.l = 248 << 16;
    harid4_work_get(spike)->child_actor = 20;
    harid4_work_get(spike)->y_velocity = 65536;
    harid4_work_get(spike)->acceleration = 4096;
    emycol_d_result = -3;

    act_down(spike);

    TEST_ASSERT_EQ_INT(ctx, 10, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 198, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 249, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, spike->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2097152, harid4_work_get(spike)->stop_distance);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_state();
    spike->r_no0 = 8;
    harid4_work_get(spike)->child_actor = 20;
    harid4_work_get(spike)->y_velocity = 65536;
    harid4_work_get(spike)->acceleration = 4096;
    emycol_d_result = 2;

    act_down(spike);

    TEST_ASSERT_EQ_INT(ctx, 8, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_harid4_down1_and_stop_are_stable(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_state();
    spike->r_no0 = 10;
    spike->yposi.l = 200 << 16;
    harid4_work_get(spike)->y_velocity = 65536;
    harid4_work_get(spike)->stop_distance = 65536;

    act_down1(spike);

    TEST_ASSERT_EQ_INT(ctx, 10, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 201, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, harid4_work_get(spike)->stop_distance);

    act_down1(spike);

    TEST_ASSERT_EQ_INT(ctx, 12, spike->r_no0);

    act_stop(spike);
    TEST_ASSERT_EQ_INT(ctx, 12, spike->r_no0);
}

TEST_MAIN_BEGIN;
    test_harid4_patterns_capture_literal_data(&ctx);
    test_harid4_init_above_water_spawns_child(&ctx);
    test_harid4_init_below_water_uses_slow_acceleration(&ctx);
    test_harid4_inactive_initialized_actor_only_draws(&ctx);
    test_harid4_child_entry_frames_when_parent_missing(&ctx);
    test_harid4_wait_detection_edges(&ctx);
    test_harid4_slide_updates_parent_and_child(&ctx);
    test_harid4_slide1_counts_down_then_advances(&ctx);
    test_harid4_down_hits_floor_and_removes_child(&ctx);
    test_harid4_down1_and_stop_are_stable(&ctx);
TEST_MAIN_END

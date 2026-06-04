#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int soundset_count;
static Sint16 soundset_request;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r4/kuzure4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_pos >= actwkchk_queue_count) {
        *ppActwk = 0;
        return 1;
    }

    *ppActwk = actwkchk_queue[actwkchk_queue_pos++];
    return 0;
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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_request = ReqNo;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    soundset_count = 0;
    soundset_request = 0;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_kuzure4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[0] == &pat_kuzure_00);
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[8] == &pat_kuzure_08);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_kuzure_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_kuzure_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE4_BASE, pat_kuzure_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -64, pat_kuzure_07.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE4_BASE + 7, pat_kuzure_07.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_kuzure_08.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE4_BASE + 8, pat_kuzure_08.spra[0].index);
}

static void test_kuzure4_init_without_ride_sets_platform_shape(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->userflag.b.h = 3;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_EQ_INT(ctx, 17252, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == pat_kuzure_a);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 24, platform->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kuzure4_init_with_ride_enters_wait_state(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->userflag.b.h = 4;
    platform->cdsts = 99;
    actwk[0].xspeed.w = -1;
    ridechk_result = 1;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->cdsts);
    TEST_ASSERT_EQ_INT(ctx, 8, kuzure4_work_get(platform)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, -8, kuzure4_work_get(platform)->break_step);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_request);

    reset_state();
    platform->userflag.b.h = 4;
    actwk[0].xspeed.w = 0;
    ridechk_result = 1;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 8, kuzure4_work_get(platform)->break_step);
}

static void test_kuzure4_wait_counts_down_to_break_state(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 4;
    kuzure4_work_get(platform)->wait_timer = 1;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, kuzure4_work_get(platform)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 6, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1, kuzure4_work_get(platform)->wait_timer);
}

static void test_kuzure4_break_spawns_parts_and_continues_platform(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *upper = &actwk[20];
    sprite_status *lower = &actwk[21];

    reset_state();
    platform->actno = 50;
    platform->r_no0 = 6;
    platform->actflg = 4;
    platform->sproffset = 17252;
    platform->patbase = pat_kuzure_a;
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    platform->userflag.b.h = 3;
    platform->patno = 2;
    platform->sprhs = 24;
    platform->sprhsize = 24;
    kuzure4_work_get(platform)->break_step = 8;
    queue_actwkchk(upper);
    queue_actwkchk(lower);
    ridechk_result = 1;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 50, upper->actno);
    TEST_ASSERT_EQ_INT(ctx, 4, upper->actflg);
    TEST_ASSERT_EQ_INT(ctx, 17252, upper->sproffset);
    TEST_ASSERT_TRUE(ctx, upper->patbase == pat_kuzure_a);
    TEST_ASSERT_EQ_INT(ctx, -1, upper->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 84, upper->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, upper->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, kuzure4_work_get(upper)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 84, lower->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, lower->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, platform->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 7, kuzure4_work_get(platform)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 108, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_kuzure4_break_last_segment_frames_out(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 6;
    platform->userflag.b.h = 1;
    kuzure4_work_get(platform)->break_step = -8;

    kuzure4(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_kuzure4_parts_init_and_wait(test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_state();
    part->userflag.b.l = -1;

    kuzure4(part);

    TEST_ASSERT_EQ_INT(ctx, 2, part->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, part->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, part->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    kuzure4_work_get(part)->wait_timer = 0;
    kuzure4(part);

    TEST_ASSERT_EQ_INT(ctx, 4, part->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1, kuzure4_work_get(part)->wait_timer);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
}

static void test_kuzure4_parts_fall_moves_or_frames_out(test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_state();
    part->userflag.b.l = -1;
    part->r_no0 = 4;
    part->actflg = 128;
    part->yposi.l = 100 << 16;
    kuzure4_work_get(part)->y_velocity = 0;

    kuzure4(part);

    TEST_ASSERT_EQ_INT(ctx, 16384, kuzure4_work_get(part)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 100, part->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    kuzure4_work_get(part)->y_velocity = 1441792;
    kuzure4(part);

    TEST_ASSERT_EQ_INT(ctx, 1441792, kuzure4_work_get(part)->y_velocity);

    reset_state();
    part->userflag.b.l = -1;
    part->r_no0 = 4;
    part->actflg = 0;

    kuzure4(part);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == part);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_kuzure4_patterns_capture_literal_data(&ctx);
    test_kuzure4_init_without_ride_sets_platform_shape(&ctx);
    test_kuzure4_init_with_ride_enters_wait_state(&ctx);
    test_kuzure4_wait_counts_down_to_break_state(&ctx);
    test_kuzure4_break_spawns_parts_and_continues_platform(&ctx);
    test_kuzure4_break_last_segment_frames_out(&ctx);
    test_kuzure4_parts_init_and_wait(&ctx);
    test_kuzure4_parts_fall_moves_or_frames_out(&ctx);
TEST_MAIN_END

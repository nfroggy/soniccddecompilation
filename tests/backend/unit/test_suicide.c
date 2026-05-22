#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Uint8 generate_flag;
Uint8 time_flag;
Uint8 projector_flag;

static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[4];

void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/suicide.c"

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    memset(pActwk, 0, sizeof(*pActwk));
    pActwk->xposi.w.h = -321;
    pActwk->yposi.w.h = -654;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    memset(pActwk, 0, sizeof(*pActwk));
    return 1234;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_suicide_state(sprite_status *actor) {
    memset(actor, 0, sizeof(*actor));
    generate_flag = 0;
    time_flag = 0;
    projector_flag = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_enemy_suicide_ignores_when_generation_disabled(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    actor.actno = 77;
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;

    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 77, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_enemy_suicide_ignores_present_time_without_userflag(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    generate_flag = 1;
    time_flag = 1;
    actor.userflag.b.h = 0;
    actor.actno = 77;

    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 77, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_enemy_suicide_frames_out_and_restores_position(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    generate_flag = 1;
    time_flag = 0;
    actor.actno = 77;
    actor.actflg = 255;
    actor.xposi.w.h = 123;
    actor.yposi.w.h = 456;

    TEST_ASSERT_EQ_INT(ctx, -1, enemy_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 123, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 24, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_enemy_suicide_present_time_with_userflag_runs(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    generate_flag = 1;
    time_flag = 1;
    actor.userflag.b.h = 1;
    actor.xposi.w.h = -12;
    actor.yposi.w.h = 34;

    TEST_ASSERT_EQ_INT(ctx, -1, enemy_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, -12, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 34, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 24, actor.actno);
}

static void test_friend_suicide_ignores_negative_userflag(test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    actor.userflag.b.h = -1;
    actor.actno = 44;

    TEST_ASSERT_EQ_INT(ctx, 0, friend_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 44, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_friend_suicide_ignores_projector_before_future(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    time_flag = 1;
    projector_flag = 1;
    actor.actno = 44;

    TEST_ASSERT_EQ_INT(ctx, 0, friend_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 44, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_friend_suicide_uses_short_frameout_before_future(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    time_flag = 1;
    projector_flag = 0;
    actor.actno = 44;

    TEST_ASSERT_EQ_INT(ctx, -1, friend_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 0, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == &actor);
}

static void test_friend_suicide_ignores_future_when_generation_enabled(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    time_flag = 2;
    generate_flag = 1;
    actor.actno = 44;

    TEST_ASSERT_EQ_INT(ctx, 0, friend_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 44, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_friend_suicide_uses_full_frameout_in_future_without_generation(
    test_context *ctx) {
    sprite_status actor;
    reset_suicide_state(&actor);
    time_flag = 2;
    generate_flag = 0;
    actor.actno = 44;

    TEST_ASSERT_EQ_INT(ctx, -1, friend_suicide(&actor));

    TEST_ASSERT_EQ_INT(ctx, 0, actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_enemy_suicide_ignores_when_generation_disabled(&ctx);
    test_enemy_suicide_ignores_present_time_without_userflag(&ctx);
    test_enemy_suicide_frames_out_and_restores_position(&ctx);
    test_enemy_suicide_present_time_with_userflag_runs(&ctx);
    test_friend_suicide_ignores_negative_userflag(&ctx);
    test_friend_suicide_ignores_projector_before_future(&ctx);
    test_friend_suicide_uses_short_frameout_before_future(&ctx);
    test_friend_suicide_ignores_future_when_generation_enabled(&ctx);
    test_friend_suicide_uses_full_frameout_in_future_without_generation(&ctx);
TEST_MAIN_END

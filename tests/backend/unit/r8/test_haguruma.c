#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int frameout_s_count;
static sprite_status *frameout_s_actor;

Sint32 frameout_s(sprite_status *pActwk);

#include "src/r8/haguruma.c"

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

static void reset_haguruma_state(void) {
    memset(actwk, 0, sizeof(actwk));
    frameout_s_count = 0;
    frameout_s_actor = 0;
}

static void assert_frameout_s_called_for(test_context *ctx,
                                         sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void initialize_wheel_with_player_away(sprite_status *wheel, Sint16 x,
                                              Sint16 y) {
    wheel->xposi.w.h = x;
    wheel->yposi.w.h = y;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 0;
    haguruma(wheel);
    frameout_s_count = 0;
    frameout_s_actor = 0;
}

static void test_haguruma_initializes_large_clockwise_wheel(test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    wheel->xposi.w.h = 100;
    wheel->yposi.w.h = 200;
    wheel->cddat = 3;
    actwk[0].xposi.w.h = -200;
    actwk[0].yposi.w.h = -200;

    haguruma(wheel);

    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_initializes_small_counter_wheel(test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    wheel->xposi.w.h = 100;
    wheel->yposi.w.h = 200;
    wheel->userflag.b.h = -15;
    wheel->cddat = 1;
    actwk[0].xposi.w.h = -200;
    actwk[0].yposi.w.h = -200;

    haguruma(wheel);

    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_player_outside_resets_prior_contact(test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 1000, 1000);
    haguruma_get_work(wheel)->engaged = 1;
    player_work_get(&actwk[0])->jump_lock = 1;
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 300;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_lock);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_player_outside_without_contact_only_frames(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 1000, 1000);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 300;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_lock);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_player_inside_but_riding_clears_contact(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].cddat = 2;
    haguruma_get_work(wheel)->engaged = 1;
    player_work_get(&actwk[0])->jump_lock = 1;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(&actwk[0])->jump_lock);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_player_enters_positive_wheel_from_standstill(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].cddat = 32;
    actwk[0].mstno.b.h = 9;
    actwk[0].mstno.b.l = 8;
    actwk[0].mspeed.w = 0;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(&actwk[0])->jump_lock);
    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_positive_wheel_respects_cddat_4_entry(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].cddat = 36;
    actwk[0].mstno.b.h = 9;
    actwk[0].mstno.b.l = 8;
    actwk[0].mspeed.w = 4096;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 3840, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_positive_wheel_keeps_middle_speed_on_held_contact(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    initialize_wheel_with_player_away(wheel, 100, 200);
    haguruma_get_work(wheel)->engaged = 1;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mstno.b.h = 9;
    actwk[0].mstno.b.l = 8;
    actwk[0].mspeed.w = 2048;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_lock);
    TEST_ASSERT_EQ_INT(ctx, 2048, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_negative_wheel_clamps_up_to_minimum(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    wheel->userflag.b.h = -16;
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 0;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_negative_wheel_clamps_down_to_maximum(
    test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    wheel->userflag.b.h = -16;
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = -4096;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, -3840, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

static void test_haguruma_negative_wheel_keeps_middle_speed(test_context *ctx) {
    sprite_status *wheel = &actwk[3];

    reset_haguruma_state();
    wheel->userflag.b.h = -16;
    initialize_wheel_with_player_away(wheel, 100, 200);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = -2048;

    haguruma(wheel);

    TEST_ASSERT_EQ_INT(ctx, -2048, actwk[0].mspeed.w);
    assert_frameout_s_called_for(ctx, wheel);
}

TEST_MAIN_BEGIN;
    test_haguruma_initializes_large_clockwise_wheel(&ctx);
    test_haguruma_initializes_small_counter_wheel(&ctx);
    test_haguruma_player_outside_resets_prior_contact(&ctx);
    test_haguruma_player_outside_without_contact_only_frames(&ctx);
    test_haguruma_player_inside_but_riding_clears_contact(&ctx);
    test_haguruma_player_enters_positive_wheel_from_standstill(&ctx);
    test_haguruma_positive_wheel_respects_cddat_4_entry(&ctx);
    test_haguruma_positive_wheel_keeps_middle_speed_on_held_contact(&ctx);
    test_haguruma_negative_wheel_clamps_up_to_minimum(&ctx);
    test_haguruma_negative_wheel_clamps_down_to_maximum(&ctx);
    test_haguruma_negative_wheel_keeps_middle_speed(&ctx);
TEST_MAIN_END

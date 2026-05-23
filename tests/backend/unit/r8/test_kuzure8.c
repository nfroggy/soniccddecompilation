#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

short_union stageno;
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
static Sint16 ridechk_results[16];
static int ridechk_result_count;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r8/kuzure8.c"

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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
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

static void set_ridechk_results(Sint16 first, Sint16 second) {
    ridechk_results[0] = first;
    ridechk_results[1] = second;
    ridechk_result_count = 2;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void reset_logs(void) {
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
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_kuzure8_state(void) {
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void assert_actionsub_called_for(test_context *ctx,
                                        sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void assert_frameout_s_called_for(test_context *ctx,
                                         sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void initialize_platform(sprite_status *actor, Sint8 userflag_h) {
    actor->userflag.b.h = userflag_h;
    kuzure8(actor);
    reset_logs();
}

static void put_platform_in_break_state(sprite_status *actor, Sint8 pieces,
                                        Sint16 player_xspeed) {
    initialize_platform(actor, pieces);
    actwk[0].xspeed.w = player_xspeed;
    ridechk_results[0] = 1;
    ridechk_result_count = 1;
    kuzure8(actor);
    reset_logs();
    for (int i = 0; i < 9; ++i) {
        kuzure8(actor);
        reset_logs();
    }
}

static void test_kuzure8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[9] == &pat09);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 444, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -64, pat07.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 451, pat07.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat08.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 452, pat08.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 453, pat09.spra[0].index);
}

static void test_kuzure8_main_ini_positive_stage_sets_wait_state(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    actor->userflag.b.h = 3;

    kuzure8(actor);

}

static void test_kuzure8_main_ini_negative_stage_sets_hitchk_state(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    stageno.b.l = 2;
    actor->userflag.b.h = -4;

    kuzure8(actor);

}

static void test_kuzure8_main_check_without_ride_only_animates(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    initialize_platform(actor, 2);

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_kuzure8_main_check_ride_from_right_starts_timer(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    initialize_platform(actor, 2);
    actwk[0].xspeed.w = 10;
    ridechk_results[0] = 1;
    ridechk_result_count = 1;

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_kuzure8_main_check_ride_from_left_breaks_opposite_direction(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];
    sprite_status *part1 = &actwk[21];

    reset_kuzure8_state();
    put_platform_in_break_state(actor, 2, -10);
    queue_actwkchk(part0);
    queue_actwkchk(part1);
    set_ridechk_results(0, 0);

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    assert_actionsub_called_for(ctx, actor);
}

static void test_kuzure8_main_break_clears_ride_and_frames_last_piece(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];
    sprite_status *part1 = &actwk[21];

    reset_kuzure8_state();
    put_platform_in_break_state(actor, 1, 10);
    queue_actwkchk(part0);
    queue_actwkchk(part1);
    set_ridechk_results(1, 0);

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_kuzure8_main_break_handles_allocator_failure(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    put_platform_in_break_state(actor, 2, 10);
    set_ridechk_results(0, 0);

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    assert_actionsub_called_for(ctx, actor);
}

static void test_kuzure8_main_break_handles_second_allocator_failure(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];

    reset_kuzure8_state();
    put_platform_in_break_state(actor, 2, 10);
    queue_actwkchk(part0);
    set_ridechk_results(0, 0);

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[21].actno);
    assert_actionsub_called_for(ctx, actor);
}

static void test_kuzure8_main_hitchk_uses_player_collision(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure8_state();
    actor->r_no0 = 8;

    kuzure8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_kuzure8_parts_initialize_and_wait_before_falling(
    test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure8_state();
    part->userflag.b.l = -1;

    kuzure8(part);

    assert_actionsub_called_for(ctx, part);

    reset_logs();
    kuzure8(part);

    assert_actionsub_called_for(ctx, part);
}

static void test_kuzure8_parts_wait_holds_until_timer_expires(
    test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure8_state();
    part->userflag.b.l = -1;
    kuzure8(part);
    reset_logs();
    set_actfree_word(part, 0, 2);

    kuzure8(part);

    assert_actionsub_called_for(ctx, part);
}

static void test_kuzure8_parts_fall_accelerates_and_caps(test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure8_state();
    part->userflag.b.l = -1;
    part->r_no0 = 4;
    part->actflg = 128;
    part->yposi.l = 100 << 16;

    kuzure8(part);

    assert_actionsub_called_for(ctx, part);

    reset_logs();
    set_actfree_long(part, 2, 1441792);
    kuzure8(part);

    assert_actionsub_called_for(ctx, part);
}

static void test_kuzure8_parts_fall_frames_out_when_invisible(test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure8_state();
    part->userflag.b.l = -1;
    part->r_no0 = 4;
    part->actflg = 0;

    kuzure8(part);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == part);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_kuzure8_patterns_capture_literal_data(&ctx);
    test_kuzure8_main_ini_positive_stage_sets_wait_state(&ctx);
    test_kuzure8_main_ini_negative_stage_sets_hitchk_state(&ctx);
    test_kuzure8_main_check_without_ride_only_animates(&ctx);
    test_kuzure8_main_check_ride_from_right_starts_timer(&ctx);
    test_kuzure8_main_check_ride_from_left_breaks_opposite_direction(&ctx);
    test_kuzure8_main_break_clears_ride_and_frames_last_piece(&ctx);
    test_kuzure8_main_break_handles_allocator_failure(&ctx);
    test_kuzure8_main_break_handles_second_allocator_failure(&ctx);
    test_kuzure8_main_hitchk_uses_player_collision(&ctx);
    test_kuzure8_parts_initialize_and_wait_before_falling(&ctx);
    test_kuzure8_parts_wait_holds_until_timer_expires(&ctx);
    test_kuzure8_parts_fall_accelerates_and_caps(&ctx);
    test_kuzure8_parts_fall_frames_out_when_invisible(&ctx);
TEST_MAIN_END

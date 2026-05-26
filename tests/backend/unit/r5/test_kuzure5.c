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
static Sint16 ridechk_results[8];
static int ridechk_result_count;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
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
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r5/kuzure5.c"

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
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_kuzure5_state(void) {
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_ridechk_result(int index, Sint16 value) {
    ridechk_results[index] = value;
    if (ridechk_result_count < index + 1) {
        ridechk_result_count = index + 1;
    }
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
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

static void put_platform_in_break_state(sprite_status *actor, Sint8 pieces,
                                        Sint16 direction) {
    actor->actno = 39;
    actor->r_no0 = 6;
    actor->actflg = 4;
    actor->sproffset = 17232;
    actor->patbase = pat_kuzure_a;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = pieces;
    actor->patno = pieces - 1;
    actor->sprhs = actor->sprhsize = pieces * 8;
    set_actfree_word(actor, 2, direction);
}

static void test_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[0] == &pat_00);
    TEST_ASSERT_TRUE(ctx, pat_kuzure_a[9] == &pat_09);
    TEST_ASSERT_EQ_INT(ctx, 1, pat_00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE5_BASE, pat_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -64, pat_07.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE5_BASE + 7, pat_07.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_08.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE5_BASE + 8, pat_08.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KUZURE5_BASE + 9, pat_09.spra[0].index);
}

static void test_main_ini_uses_default_stage_offset_without_ride(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure5_state();
    actor->userflag.b.h = 3;

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kuzure_a);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17232, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_main_ini_stage_two_offset_and_ride_from_right(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure5_state();
    stageno.b.l = 2;
    actor->userflag.b.h = 2;
    actwk[0].xspeed.w = 10;
    set_ridechk_result(0, 1);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 17312, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 8, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 8, get_actfree_word(actor, 2));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_main_check_ride_from_left_sets_negative_break_direction(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure5_state();
    actor->r_no0 = 2;
    actwk[0].xspeed.w = -1;
    set_ridechk_result(0, 1);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -8, get_actfree_word(actor, 2));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    assert_actionsub_called_for(ctx, actor);
    assert_frameout_s_called_for(ctx, actor);
}

static void test_main_wait_holds_and_then_advances_to_break(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_kuzure5_state();
    actor->r_no0 = 4;
    set_actfree_word(actor, 0, 2);
    set_ridechk_result(0, 1);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_actionsub_called_for(ctx, actor);

    reset_logs();
    set_actfree_word(actor, 0, 0);
    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1, get_actfree_word(actor, 0));
    assert_actionsub_called_for(ctx, actor);
}

static void test_main_break_spawns_two_parts_and_shrinks_platform(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];
    sprite_status *part1 = &actwk[21];

    reset_kuzure5_state();
    put_platform_in_break_state(actor, 2, 8);
    queue_actor(part0);
    queue_actor(part1);
    set_ridechk_result(0, 1);
    set_ridechk_result(1, 0);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 7, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 108, actor->xposi.w.h);
    assert_actionsub_called_for(ctx, actor);

    TEST_ASSERT_EQ_INT(ctx, 39, part0->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, part0->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 92, part0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, part0->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, get_actfree_word(part0, 0));
    TEST_ASSERT_EQ_INT(ctx, 8, part0->patno);
    TEST_ASSERT_EQ_INT(ctx, 39, part1->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, part1->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 92, part1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, part1->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 9, part1->patno);
}

static void test_main_break_negative_direction_and_last_piece_frames_out(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];
    sprite_status *part1 = &actwk[21];

    reset_kuzure5_state();
    put_platform_in_break_state(actor, 1, -8);
    queue_actor(part0);
    queue_actor(part1);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, part0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, part1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_main_break_handles_allocator_failures(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *part0 = &actwk[20];

    reset_kuzure5_state();
    put_platform_in_break_state(actor, 2, 8);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    assert_actionsub_called_for(ctx, actor);

    reset_kuzure5_state();
    put_platform_in_break_state(actor, 2, 8);
    queue_actor(part0);

    kuzure5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[21].actno);
    assert_actionsub_called_for(ctx, actor);
}

static void test_parts_initialize_and_wait_paths(test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure5_state();
    part->userflag.b.l = -1;

    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 2, part->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, part->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, part->sprvsize);
    assert_actionsub_called_for(ctx, part);

    reset_logs();
    set_actfree_word(part, 0, 2);
    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 2, part->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, get_actfree_word(part, 0));
    assert_actionsub_called_for(ctx, part);

    reset_logs();
    set_actfree_word(part, 0, 0);
    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 4, part->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1, get_actfree_word(part, 0));
    assert_actionsub_called_for(ctx, part);
}

static void test_parts_fall_accelerates_caps_and_frames_when_invisible(
    test_context *ctx) {
    sprite_status *part = &actwk[20];

    reset_kuzure5_state();
    part->userflag.b.l = -1;
    part->r_no0 = 4;
    part->actflg = 128;
    part->yposi.l = 100 << 16;

    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 16384, get_actfree_long(part, 2));
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 16384, part->yposi.l);
    assert_actionsub_called_for(ctx, part);

    reset_logs();
    set_actfree_long(part, 2, 1441792);
    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 1441792, get_actfree_long(part, 2));
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 16384 + 1441792, part->yposi.l);
    assert_actionsub_called_for(ctx, part);

    reset_logs();
    part->actflg = 0;
    kuzure5(part);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == part);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_patterns_capture_literal_data(&ctx);
    test_main_ini_uses_default_stage_offset_without_ride(&ctx);
    test_main_ini_stage_two_offset_and_ride_from_right(&ctx);
    test_main_check_ride_from_left_sets_negative_break_direction(&ctx);
    test_main_wait_holds_and_then_advances_to_break(&ctx);
    test_main_break_spawns_two_parts_and_shrinks_platform(&ctx);
    test_main_break_negative_direction_and_last_piece_frames_out(&ctx);
    test_main_break_handles_allocator_failures(&ctx);
    test_parts_initialize_and_wait_paths(&ctx);
    test_parts_fall_accelerates_caps_and_frames_when_invisible(&ctx);
TEST_MAIN_END

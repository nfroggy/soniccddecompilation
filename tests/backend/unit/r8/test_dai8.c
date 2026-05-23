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
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r8/dai8.c"

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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
}

static void reset_dai8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void assert_action_and_frameout(test_context *ctx,
                                       sprite_status *actor,
                                       Sint16 expected_xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expected_xpos, frameout_s00_xpos);
}

static void init_platform(sprite_status *actor, Sint16 x, Sint16 y,
                          Sint8 type) {
    actor->actno = 44;
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    actor->userflag.b.h = type;

    dai8(actor);
    reset_logs();
}

static void test_dai8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg[1] == p01);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[46]);
    TEST_ASSERT_EQ_INT(ctx, 0, p01[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, p01[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, p01[151]);
    TEST_ASSERT_TRUE(ctx, pat_dai8[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_dai8[6] == &patnull);
    TEST_ASSERT_TRUE(ctx, pat_dai8[7] == &pat00);
    TEST_ASSERT_EQ_INT(ctx, 433, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 438, pat05.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, patnull.spra[0].index);
}

static void test_dai8_initializes_single_platform(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_dai8_state();
    actor->actno = 44;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = 0;

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_initializes_one_child_for_type1(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_dai8_state();
    actor->actno = 44;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = 1;
    queue_actwkchk(child);

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_initializes_two_children_for_type2(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child0 = &actwk[20];
    sprite_status *child1 = &actwk[21];

    reset_dai8_state();
    actor->actno = 44;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = 2;
    queue_actwkchk(child0);
    queue_actwkchk(child1);

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_initialization_frames_out_when_child_allocation_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_dai8_state();
    actor->actno = 44;
    actor->xposi.w.h = 1000;
    actor->userflag.b.h = 1;

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_child_frames_out_when_parent_is_missing(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_dai8_state();
    queue_actwkchk(child);
    init_platform(parent, 1000, 200, 1);
    parent->actno = 0;

    dai8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_dai8_child_frames_out_when_origin_words_differ(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_dai8_state();
    queue_actwkchk(child);
    init_platform(parent, 1000, 200, 1);
    set_actfree_word(child, 4, 1001);

    dai8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);

    reset_dai8_state();
    parent = &actwk[3];
    child = &actwk[20];
    queue_actwkchk(child);
    init_platform(parent, 1000, 200, 1);
    set_actfree_word(child, 6, 201);

    dai8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_dai8_child_with_valid_parent_runs_without_frameout_s00(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_dai8_state();
    queue_actwkchk(child);
    init_platform(parent, 1000, 200, 1);

    dai8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_dai8_wait_counts_down_and_advances_on_zero(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);

    dai8(actor);

    assert_action_and_frameout(ctx, actor, 1000);

    for (i = 0; i < 58; ++i) {
        reset_logs();
        dai8(actor);
    }
    reset_logs();
    dai8(actor);

    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_appear_waits_until_animation_reaches_patno_zero(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);
    for (i = 0; i < 60; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 3;
    reset_logs();

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    assert_action_and_frameout(ctx, actor, 1000);

    reset_logs();
    actor->patno = 0;

    dai8(actor);

    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_on_tracks_ride_without_expiring(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);
    for (i = 0; i < 60; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 0;
    reset_logs();
    dai8(actor);
    reset_logs();
    ridechk_result = 1;

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_on_expires_and_clears_ride_when_player_is_on(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);
    for (i = 0; i < 60; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 0;
    reset_logs();
    dai8(actor);
    for (i = 0; i < 119; ++i) {
        reset_logs();
        dai8(actor);
    }
    reset_logs();
    ridechk_result = 1;

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_on_expires_without_clearing_when_player_is_off(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);
    for (i = 0; i < 60; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 0;
    reset_logs();
    dai8(actor);
    for (i = 0; i < 119; ++i) {
        reset_logs();
        dai8(actor);
    }
    reset_logs();
    ridechk_result = 0;

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_dai8_off_waits_until_animation_reaches_patno_zero(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_dai8_state();
    init_platform(actor, 1000, 200, 0);
    for (i = 0; i < 60; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 0;
    reset_logs();
    dai8(actor);
    for (i = 0; i < 120; ++i) {
        reset_logs();
        dai8(actor);
    }
    actor->patno = 2;
    reset_logs();

    dai8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    assert_action_and_frameout(ctx, actor, 1000);

    reset_logs();
    actor->patno = 0;

    dai8(actor);

    assert_action_and_frameout(ctx, actor, 1000);
}

TEST_MAIN_BEGIN;
    test_dai8_patterns_capture_literal_data(&ctx);
    test_dai8_initializes_single_platform(&ctx);
    test_dai8_initializes_one_child_for_type1(&ctx);
    test_dai8_initializes_two_children_for_type2(&ctx);
    test_dai8_initialization_frames_out_when_child_allocation_fails(&ctx);
    test_dai8_child_frames_out_when_parent_is_missing(&ctx);
    test_dai8_child_frames_out_when_origin_words_differ(&ctx);
    test_dai8_child_with_valid_parent_runs_without_frameout_s00(&ctx);
    test_dai8_wait_counts_down_and_advances_on_zero(&ctx);
    test_dai8_appear_waits_until_animation_reaches_patno_zero(&ctx);
    test_dai8_on_tracks_ride_without_expiring(&ctx);
    test_dai8_on_expires_and_clears_ride_when_player_is_on(&ctx);
    test_dai8_on_expires_without_clearing_when_player_is_off(&ctx);
    test_dai8_off_waits_until_animation_reaches_patno_zero(&ctx);
TEST_MAIN_END

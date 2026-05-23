#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_results[8];
static int emycol_d_result_count;

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/propera8.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

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

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    if (emycol_d_count <= emycol_d_result_count) {
        return emycol_d_results[emycol_d_count - 1];
    }
    return 0;
}

static void reset_logs(void) {
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
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
    emycol_d_count = 0;
    emycol_d_actor = 0;
    memset(emycol_d_results, 0, sizeof(emycol_d_results));
    emycol_d_result_count = 0;
}

static void reset_propera8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_emycol_d_result(Sint16 result) {
    emycol_d_results[0] = result;
    emycol_d_result_count = 1;
}

static void assert_parent_callbacks(test_context *ctx, sprite_status *actor,
                                    Sint16 expected_xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expected_xpos, frameout_s00_xpos);
}

static void assert_child_actionsub(test_context *ctx, sprite_status *child) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
}

static void spawn_parent_and_children(sprite_status *parent,
                                      sprite_status *blade_a,
                                      sprite_status *blade_b) {
    parent->actno = 40;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    queue_actwkchk2(blade_a);
    queue_actwkchk2(blade_b);

    propera(parent);
    reset_logs();
}

static void drive_actor(sprite_status *actor) {
    reset_logs();
    propera(actor);
}

static void drive_child_times(sprite_status *child, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        drive_actor(child);
    }
}

static void test_propera8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg1[0] == p01);
    TEST_ASSERT_EQ_INT(ctx, 10, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[3]);
    TEST_ASSERT_EQ_INT(ctx, 2, p01[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, p01[6]);
    TEST_ASSERT_EQ_INT(ctx, -1, p01[7]);
    TEST_ASSERT_TRUE(ctx, pat_propera[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_propera[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -40, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 454, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 455, pat01.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pat_propera1[0] == &pat02);
    TEST_ASSERT_TRUE(ctx, pat_propera1[7] == &pat09);
    TEST_ASSERT_EQ_INT(ctx, -32, pat02.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 456, pat02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -12, pat09.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 463, pat09.spra[0].index);
}

static void test_propera_parent_initializes_and_spawns_two_children(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    parent->actno = 40;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    queue_actwkchk2(blade_a);
    queue_actwkchk2(blade_b);

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);
}

static void test_propera_parent_first_allocation_failure_frames_out(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];

    reset_propera8_state();
    parent->xposi.w.h = 100;

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);
}

static void test_propera_parent_second_allocation_failure_frames_out(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];

    reset_propera8_state();
    parent->actno = 40;
    parent->xposi.w.h = 100;
    queue_actwkchk2(blade_a);

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);
}

static void test_propera_parent_fall_waits_until_collision_is_negative(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, blade_a, blade_b);
    set_emycol_d_result(3);

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);

    reset_logs();
    set_emycol_d_result(-4);

    propera(parent);

    assert_parent_callbacks(ctx, parent, 100);
}

static void test_propera_parent_move_tracks_floor_and_reverses_at_timer(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *blade_b = &actwk[21];
    int i;

    reset_propera8_state();
    spawn_parent_and_children(parent, blade_a, blade_b);
    set_emycol_d_result(-1);
    propera(parent);
    reset_logs();
    set_emycol_d_result(5);

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);

    for (i = 0; i < 126; ++i) {
        drive_actor(parent);
    }

    reset_logs();
    set_emycol_d_result(-3);

    propera(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == parent);
    assert_parent_callbacks(ctx, parent, 100);
}

static void test_propera_child_frames_out_when_parent_is_missing(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, child, blade_b);
    parent->actno = 0;

    propera(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_propera_child_init_positive_starts_closing(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, child, blade_b);

    propera(child);

    assert_child_actionsub(ctx, child);
}

static void test_propera_child_init_negative_starts_opening(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *child = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, blade_a, child);

    propera(child);

    assert_child_actionsub(ctx, child);
}

static void test_propera_child_close1_waits_then_enters_closed(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, child, blade_b);

    propera(child);

    assert_child_actionsub(ctx, child);

    drive_child_times(child, 3);
    reset_logs();

    propera(child);

    assert_child_actionsub(ctx, child);
}

static void test_propera_child_closed1_waits_then_starts_opening(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];
    sprite_status *blade_b = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, child, blade_b);
    drive_child_times(child, 5);
    reset_logs();

    propera(child);

    assert_child_actionsub(ctx, child);

    drive_child_times(child, 71);
    reset_logs();

    propera(child);

    assert_child_actionsub(ctx, child);
}

static void test_propera_child_open1_waits_then_enters_opened(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *child = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, blade_a, child);
    propera(child);
    reset_logs();

    propera(child);

    assert_child_actionsub(ctx, child);

    drive_child_times(child, 3);
    child->colicnt = 9;
    reset_logs();

    propera(child);

    assert_child_actionsub(ctx, child);
}

static void test_propera_child_opened1_animates_until_close_cycle(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *blade_a = &actwk[20];
    sprite_status *child = &actwk[21];

    reset_propera8_state();
    spawn_parent_and_children(parent, blade_a, child);
    drive_child_times(child, 6);
    reset_logs();

    propera(child);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg1);
    assert_child_actionsub(ctx, child);

    drive_child_times(child, 71);
    child->colino = 168;
    reset_logs();

    propera(child);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_child_actionsub(ctx, child);
}

TEST_MAIN_BEGIN;
    test_propera8_patterns_capture_literal_data(&ctx);
    test_propera_parent_initializes_and_spawns_two_children(&ctx);
    test_propera_parent_first_allocation_failure_frames_out(&ctx);
    test_propera_parent_second_allocation_failure_frames_out(&ctx);
    test_propera_parent_fall_waits_until_collision_is_negative(&ctx);
    test_propera_parent_move_tracks_floor_and_reverses_at_timer(&ctx);
    test_propera_child_frames_out_when_parent_is_missing(&ctx);
    test_propera_child_init_positive_starts_closing(&ctx);
    test_propera_child_init_negative_starts_opening(&ctx);
    test_propera_child_close1_waits_then_enters_closed(&ctx);
    test_propera_child_closed1_waits_then_starts_opening(&ctx);
    test_propera_child_open1_waits_then_enters_opened(&ctx);
    test_propera_child_opened1_animates_until_close_cycle(&ctx);
TEST_MAIN_END

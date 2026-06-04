#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static Sint16 emycol_d_results[12];
static int emycol_d_result_count;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actionsub_count;
static sprite_status *actionsub_actor;

Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void actionsub(sprite_status *pActwk);

#include "src/r5/kemusi.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    Sint16 result = 0;

    (void)pActwk;
    ++emycol_d_count;
    if (emycol_d_count <= emycol_d_result_count) {
        result = emycol_d_results[emycol_d_count - 1];
    }
    return result;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

static void reset_logs(void) {
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_count = 0;
    memset(emycol_d_results, 0, sizeof(emycol_d_results));
    emycol_d_result_count = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
}

static void reset_kemusi_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_emycol(Sint16 result) {
    emycol_d_results[emycol_d_result_count++] = result;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void queue_body_segments(int first_index) {
    int i;

    for (i = 0; i < 3; ++i) {
        queue_actwkchk2(&actwk[first_index + i]);
    }
}

static void link_live_segments(sprite_status *actor, int first_index) {
    int i;

    kemusi_get_work(actor)->link_indices[0] = (Sint16)first_index;
    kemusi_get_work(actor)->link_indices[1] = (Sint16)(first_index + 1);
    kemusi_get_work(actor)->link_indices[2] = (Sint16)(first_index + 2);
    for (i = 0; i < 3; ++i) {
        actwk[first_index + i].actno = 34;
    }
}

static void assert_common_segment(test_context *ctx, sprite_status *actor,
                                  Sint16 origin) {
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 9214, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kemusi);
    TEST_ASSERT_EQ_INT(ctx, origin, kemusi_get_work(actor)->origin_x);
}

static void assert_visible_main_callbacks(test_context *ctx,
                                          sprite_status *actor,
                                          Sint16 origin) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin, frameout_s00_xpos);
}

static void test_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kemusi[0] == &kemusi_pat0);
    TEST_ASSERT_TRUE(ctx, pat_kemusi[3] == &kemusi_pat3);
    TEST_ASSERT_EQ_INT(ctx, 1, kemusi_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, kemusi_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, kemusi_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 431, kemusi_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 434, kemusi_pat3.spra[0].index);
}

static void test_enemy_suicide_short_circuits_main_only(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    enemy_suicide_result = 1;

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);

    reset_kemusi_state();
    actor = &actwk[4];
    actor->userflag.b.l = -1;

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_init_spawns_three_normal_segments(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    int i;

    reset_kemusi_state();
    actor->actno = 34;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    queue_body_segments(30);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 51, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 3, kemusi_get_work(actor)->step_delta);
    TEST_ASSERT_EQ_INT(ctx, 36, kemusi_get_work(actor)->move_duration);
    TEST_ASSERT_EQ_INT(ctx, 6, kemusi_get_work(actor)->step_reset);
    TEST_ASSERT_EQ_INT(ctx, 30, kemusi_get_work(actor)->link_indices[0]);
    TEST_ASSERT_EQ_INT(ctx, 31, kemusi_get_work(actor)->link_indices[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, kemusi_get_work(actor)->link_indices[2]);
    assert_common_segment(ctx, actor, 100);
    assert_visible_main_callbacks(ctx, actor, 100);

    for (i = 0; i < 3; ++i) {
        sprite_status *part = &actwk[30 + i];
        TEST_ASSERT_EQ_INT(ctx, 34, part->actno);
        TEST_ASSERT_EQ_INT(ctx, -1, part->userflag.b.l);
        TEST_ASSERT_EQ_INT(ctx, 112 + 12 * i, part->xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 50, part->yposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 2, part->patno);
        TEST_ASSERT_EQ_INT(ctx, 11, part->sprvsize);
        TEST_ASSERT_EQ_INT(ctx, 180, part->colino);
        TEST_ASSERT_EQ_INT(ctx, 2 - i, kemusi_get_work(part)->step_delta);
        TEST_ASSERT_EQ_INT(ctx, 36, kemusi_get_work(part)->move_duration);
        TEST_ASSERT_EQ_INT(ctx, 6, kemusi_get_work(part)->step_reset);
        assert_common_segment(ctx, part, 100);
    }

    TEST_ASSERT_EQ_INT(ctx, 4, kemusi_get_work(&actwk[30])->link_indices[0]);
    TEST_ASSERT_EQ_INT(ctx, 31, kemusi_get_work(&actwk[30])->link_indices[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, kemusi_get_work(&actwk[30])->link_indices[2]);
    TEST_ASSERT_EQ_INT(ctx, 30, kemusi_get_work(&actwk[32])->link_indices[1]);
}

static void test_init_time_variant_and_allocation_failure(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    actor->actno = 34;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 200;
    actor->yposi.w.h = 70;
    queue_body_segments(40);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 36, kemusi_get_work(actor)->move_duration);
    TEST_ASSERT_EQ_INT(ctx, 12, kemusi_get_work(actor)->step_reset);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[40].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 51, actwk[40].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[40].userflag.b.h);

    reset_kemusi_state();
    actor = &actwk[4];
    actor->actno = 34;
    actor->xposi.w.h = 100;
    queue_actwkchk2(&actwk[30]);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    assert_visible_main_callbacks(ctx, actor, 100);
}

static void test_live_segment_guard_frames_out_if_any_link_dies(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    actor->r_no0 = 2;
    link_live_segments(actor, 30);
    actwk[31].actno = 0;

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_fall_landing_counts_main_and_child_segments(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *child = &actwk[30];

    reset_kemusi_state();
    actor->r_no0 = 2;
    actor->yposi.w.h = 50;
    kemusi_get_work(actor)->origin_x = 100;
    link_live_segments(actor, 30);
    queue_emycol(1);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 51, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, kemusi_get_work(actor)->landed_count);

    reset_logs();
    queue_emycol(-3);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, kemusi_get_work(actor)->landed_count);

    reset_logs();
    actor->actno = 34;
    child->userflag.b.l = -1;
    child->r_no0 = 2;
    child->yposi.w.h = 70;
    kemusi_get_work(child)->link_indices[0] = 4;
    kemusi_get_work(child)->link_indices[1] = 31;
    kemusi_get_work(child)->link_indices[2] = 32;
    queue_emycol(-2);

    kemusi(child);

    TEST_ASSERT_EQ_INT(ctx, 4, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 69, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, kemusi_get_work(actor)->landed_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_stop_waits_for_all_segments_then_starts_move(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    actor->r_no0 = 4;
    actor->patno = 1;
    kemusi_get_work(actor)->step_delta = 3;
    kemusi_get_work(actor)->move_duration = 36;
    kemusi_get_work(actor)->step_reset = 6;
    kemusi_get_work(actor)->origin_x = 100;
    kemusi_get_work(actor)->landed_count = 3;
    link_live_segments(actor, 30);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);

    reset_logs();
    kemusi_get_work(actor)->landed_count = 4;

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[30].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[31].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[32].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, kemusi_get_work(actor)->step_delta);
    TEST_ASSERT_EQ_INT(ctx, 35, kemusi_get_work(actor)->move_timer);
    TEST_ASSERT_EQ_INT(ctx, 6, kemusi_get_work(actor)->step_counter);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
}

static void test_move_state_initializes_child_without_flipping_patno(
    test_context *ctx) {
    sprite_status *child = &actwk[30];

    reset_kemusi_state();
    child->userflag.b.l = -1;
    child->r_no0 = 6;
    child->patno = 3;
    kemusi_get_work(child)->step_delta = 2;
    kemusi_get_work(child)->move_duration = 36;
    kemusi_get_work(child)->step_reset = 6;
    kemusi_get_work(child)->origin_x = 100;
    kemusi_get_work(child)->link_indices[0] = 4;
    kemusi_get_work(child)->link_indices[1] = 31;
    kemusi_get_work(child)->link_indices[2] = 32;
    actwk[4].actno = 34;
    actwk[31].actno = 34;
    actwk[32].actno = 34;

    kemusi(child);

    TEST_ASSERT_EQ_INT(ctx, 8, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, kemusi_get_work(child)->step_delta);
    TEST_ASSERT_EQ_INT(ctx, 35, kemusi_get_work(child)->move_timer);
    TEST_ASSERT_EQ_INT(ctx, 5, kemusi_get_work(child)->step_counter);
    TEST_ASSERT_EQ_INT(ctx, 3, child->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_move1_timer_and_floor_adjust_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    actor->r_no0 = 8;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 40;
    kemusi_get_work(actor)->step_delta = 3;
    kemusi_get_work(actor)->move_timer = 2;
    kemusi_get_work(actor)->step_reset = 6;
    kemusi_get_work(actor)->step_counter = 5;
    kemusi_get_work(actor)->origin_x = 100;
    link_live_segments(actor, 30);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, kemusi_get_work(actor)->step_counter);
    TEST_ASSERT_EQ_INT(ctx, 1, kemusi_get_work(actor)->move_timer);
    TEST_ASSERT_EQ_INT(ctx, 100, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);

    reset_logs();
    kemusi_get_work(actor)->step_counter = 3;
    kemusi_get_work(actor)->move_timer = 1;
    queue_emycol(2);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, kemusi_get_work(actor)->step_counter);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 42, actor->yposi.w.h);
}

static void test_move1_reverses_at_origin_range_and_floor_edge(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kemusi_state();
    actor->r_no0 = 8;
    actor->xposi.w.h = 181;
    kemusi_get_work(actor)->step_delta = 3;
    kemusi_get_work(actor)->move_timer = 3;
    kemusi_get_work(actor)->step_reset = 6;
    kemusi_get_work(actor)->step_counter = 3;
    kemusi_get_work(actor)->origin_x = 100;
    link_live_segments(actor, 30);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 181, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);

    reset_kemusi_state();
    actor = &actwk[4];
    actor->r_no0 = 8;
    actor->xposi.w.h = 100;
    kemusi_get_work(actor)->step_delta = 3;
    kemusi_get_work(actor)->move_timer = 3;
    kemusi_get_work(actor)->step_reset = 6;
    kemusi_get_work(actor)->step_counter = 3;
    kemusi_get_work(actor)->origin_x = 100;
    link_live_segments(actor, 30);
    queue_emycol(7);

    kemusi(actor);

    TEST_ASSERT_EQ_INT(ctx, 100, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
}

TEST_MAIN_BEGIN;
    test_patterns_capture_literal_data(&ctx);
    test_enemy_suicide_short_circuits_main_only(&ctx);
    test_init_spawns_three_normal_segments(&ctx);
    test_init_time_variant_and_allocation_failure(&ctx);
    test_live_segment_guard_frames_out_if_any_link_dies(&ctx);
    test_fall_landing_counts_main_and_child_segments(&ctx);
    test_stop_waits_for_all_segments_then_starts_move(&ctx);
    test_move_state_initializes_child_without_flipping_patno(&ctx);
    test_move1_timer_and_floor_adjust_paths(&ctx);
    test_move1_reverses_at_origin_range_and_floor_edge(&ctx);
TEST_MAIN_END

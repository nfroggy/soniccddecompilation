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
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_results[8];
static int emycol_d_result_count;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[8];
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r5/denden.c"

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

Sint16 enemy_suicide(sprite_status *pActwk) {
    ++enemy_suicide_count;
    (void)pActwk;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    if (emycol_d_count <= emycol_d_result_count) {
        return emycol_d_results[emycol_d_count - 1];
    }
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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void reset_denden_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    memset(emycol_d_results, 0, sizeof(emycol_d_results));
    emycol_d_result_count = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_denden_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_denden_logs();
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_emycol(Sint16 result) {
    emycol_d_results[emycol_d_result_count++] = result;
}

static void assert_tail_callbacks(test_context *ctx, sprite_status *actor,
                                  Sint16 frameout_x) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, frameout_x, frameout_s00_xpos);
}

static void make_moving_actor(sprite_status *actor) {
    actor->actno = 32;
    actor->r_no0 = 4;
    actor->xposi.l = 100 << 16;
    actor->yposi.l = 200 << 16;
    denden_work_get(actor)->origin_x = 100;
    denden_work_get(actor)->x_velocity = 0;
}

static void spawn_projectile(sprite_status *parent, sprite_status *child) {
    reset_denden_state();
    make_moving_actor(parent);
    parent->actflg = 128;
    parent->userflag.b.l = 7;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    queue_actor(child);
    queue_emycol(0);

    denden(parent);
    reset_denden_logs();
}

static void test_pattern_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_denden_e[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_denden_e[3] == &pat03);
    TEST_ASSERT_TRUE(ctx, pat_denden_b[0] == &pat04);
    TEST_ASSERT_TRUE(ctx, pat_none[0] == &pat06);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 456, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3, pat03.cnt);
    TEST_ASSERT_EQ_INT(ctx, 463, pat03.spra[2].index);
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg[1] == p01);
}

static void test_enemy_suicide_short_circuits_dispatch(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    actor->actno = 32;
    actor->xposi.w.h = 100;
    enemy_suicide_result = 1;

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
}

static void test_init_sets_enemy_fields_and_lands_from_fall(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    actor->actno = 32;
    actor->xposi.w.h = 300;
    actor->yposi.l = 100 << 16;
    queue_emycol(-5);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 96, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, denden_work_get(actor)->origin_x);
    TEST_ASSERT_EQ_INT(ctx, -16384, denden_work_get(actor)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9104, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 15, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 47, actor->colino);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_denden_e);
    assert_tail_callbacks(ctx, actor, 300);

    reset_denden_state();
    actor->actno = 32;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 64;
    queue_emycol(4);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_denden_b);
    assert_tail_callbacks(ctx, actor, 64);
}

static void test_fall_state_keeps_waiting_when_floor_is_not_hit(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 77;
    actor->yposi.l = 10 << 16;
    denden_work_get(actor)->origin_x = 77;
    queue_emycol(6);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 11, actor->yposi.w.h);
    assert_tail_callbacks(ctx, actor, 77);
}

static void test_move_userflag_variant_tracks_floor_and_patches(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    make_moving_actor(actor);
    actor->userflag.b.h = 1;
    queue_emycol(3);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 203, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    assert_tail_callbacks(ctx, actor, 100);

    reset_denden_state();
    make_moving_actor(actor);
    actor->userflag.b.h = 1;
    actor->xposi.l = 90 << 16;
    queue_emycol(0);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 90, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, actor, 100);
}

static void test_move_reverses_at_range_and_floor_limits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    make_moving_actor(actor);
    actor->actflg = 4;
    actor->cddat = 2;
    actor->xposi.l = 180 << 16;
    denden_work_get(actor)->x_velocity = 65536;

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -65536, denden_work_get(actor)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_tail_callbacks(ctx, actor, 100);

    reset_denden_state();
    make_moving_actor(actor);
    actor->actflg = 1;
    actor->cddat = 1;
    denden_work_get(actor)->x_velocity = 32768;
    queue_emycol(7);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -32768, denden_work_get(actor)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_tail_callbacks(ctx, actor, 100);
}

static void test_move_spawns_reuses_and_clears_projectile(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[40];

    reset_denden_state();
    make_moving_actor(parent);
    parent->actflg = 128;
    parent->userflag.b.l = 9;
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 220;
    queue_actor(child);
    queue_emycol(0);

    denden(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 183, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, child->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, child->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 9, child->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, parent->xposi.w.h, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 20, child->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_none);
    TEST_ASSERT_EQ_INT(ctx, 176, child->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, parent, 100);

    reset_denden_logs();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    denden(parent);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, parent, 100);

    reset_denden_logs();
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    denden(parent);

    TEST_ASSERT_EQ_INT(ctx, 0, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, parent, 100);

    reset_denden_logs();
    actwk[0].xposi.w.h = 500;
    queue_emycol(0);

    denden(parent);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, parent, 100);
}

static void test_move_allocation_failure_still_patches(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_denden_state();
    make_moving_actor(actor);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, actor, 100);

    reset_denden_state();
    make_moving_actor(actor);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 500;
    queue_emycol(0);

    denden(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    assert_tail_callbacks(ctx, actor, 100);
}

static void test_projectile_sub_tracks_parent_or_frames_out(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[40];

    spawn_projectile(parent, child);
    parent->xposi.w.h = 321;
    parent->yposi.w.h = 654;

    denden(child);

    TEST_ASSERT_EQ_INT(ctx, 321, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 634, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    spawn_projectile(parent, child);
    parent->actno = 0;

    denden(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    spawn_projectile(parent, child);
    parent->userflag.b.l = 8;

    denden(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_pattern_tables_capture_literal_data(&ctx);
    test_enemy_suicide_short_circuits_dispatch(&ctx);
    test_init_sets_enemy_fields_and_lands_from_fall(&ctx);
    test_fall_state_keeps_waiting_when_floor_is_not_hit(&ctx);
    test_move_userflag_variant_tracks_floor_and_patches(&ctx);
    test_move_reverses_at_range_and_floor_limits(&ctx);
    test_move_spawns_reuses_and_clears_projectile(&ctx);
    test_move_allocation_failure_still_patches(&ctx);
    test_projectile_sub_tracks_parent_or_frames_out(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static Sint16 emycol_d_results[8];
static int emycol_d_result_count;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[8];

Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r5/sasori.c"

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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
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

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
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
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_sasori_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_emycol(Sint16 result) {
    emycol_d_results[emycol_d_result_count++] = result;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void assert_main_callbacks(test_context *ctx, sprite_status *actor,
                                  Sint16 origin) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, origin, frameout_s00_xpos);
}

static void setup_moving_main(sprite_status *actor, int tail_index) {
    actor->actno = 33;
    actor->r_no0 = 4;
    actor->xposi.l = 100 << 16;
    actor->yposi.w.h = 200;
    sasori_get_work(actor)->origin_x = 100;
    sasori_get_work(actor)->x_speed = -65536;
    sasori_get_work(actor)->tail_index = (Sint16)tail_index;
    sasori_get_work(actor)->tail_x_offset = 3;
    actwk[tail_index].actno = 33;
    actwk[0].yposi.w.h = 0;
}

static void test_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_sasori_e[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_sasori_e[1] == &pat01);
    TEST_ASSERT_TRUE(ctx, pat_sasori_b[0] == &pat02);
    TEST_ASSERT_TRUE(ctx, pat_tail[1] == &pat05);
    TEST_ASSERT_TRUE(ctx, pat_tama[0] == &pat06);
    TEST_ASSERT_EQ_INT(ctx, 3, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -21, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 493, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 502, pat03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 505, pat04.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 508, pat07.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 29, pchg00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg00[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg_tama00[0]);
}

static void test_enemy_suicide_short_circuits_main_actor(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    enemy_suicide_result = 1;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_init_records_origin_and_time_variant(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    actor->actno = 33;
    actor->xposi.w.h = 320;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 320, sasori_get_work(actor)->origin_x);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_sasori_e);
    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 3, sasori_get_work(actor)->tail_x_offset);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 9142, actor->sproffset);
    assert_main_callbacks(ctx, actor, 320);

    reset_sasori_state();
    actor = &actwk[3];
    actor->actno = 33;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 64;

    sasori(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_sasori_b);
    TEST_ASSERT_EQ_INT(ctx, -32768, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 7, sasori_get_work(actor)->tail_x_offset);
    assert_main_callbacks(ctx, actor, 64);
}

static void test_fall_lands_and_initializes_tail(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *tail = &actwk[30];

    reset_sasori_state();
    actor->actno = 33;
    actor->r_no0 = 2;
    actor->sprpri = 3;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    sasori_get_work(actor)->origin_x = 100;
    queue_emycol(-3);
    queue_actwkchk2(tail);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 30, sasori_get_work(actor)->tail_index);
    TEST_ASSERT_EQ_INT(ctx, 33, tail->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, tail->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 24, tail->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, tail->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, tail->sprvsize);
    TEST_ASSERT_TRUE(ctx, tail->patbase == pat_tail);
    TEST_ASSERT_EQ_INT(ctx, 1, tail->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, sasori_get_work(tail)->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 2, tail->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 4, tail->actflg);
    TEST_ASSERT_EQ_INT(ctx, 9142, tail->sproffset);
    assert_main_callbacks(ctx, actor, 100);
}

static void test_fall_without_ground_or_tail_slot(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    actor->actno = 33;
    actor->r_no0 = 2;
    actor->yposi.w.h = 50;
    sasori_get_work(actor)->origin_x = 100;
    queue_emycol(1);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 51, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    assert_main_callbacks(ctx, actor, 100);

    reset_sasori_state();
    actor = &actwk[3];
    actor->actno = 33;
    actor->r_no0 = 2;
    actor->yposi.w.h = 50;
    sasori_get_work(actor)->origin_x = 100;
    queue_emycol(-1);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    assert_main_callbacks(ctx, actor, 100);
}

static void test_move_updates_tail_without_reversal(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *tail = &actwk[30];

    reset_sasori_state();
    setup_moving_main(actor, 30);
    queue_emycol(0);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 102, tail->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 184, tail->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    assert_main_callbacks(ctx, actor, 100);
}

static void test_move_reverses_at_range_or_floor_edge(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    setup_moving_main(actor, 30);
    actor->xposi.l = 180 << 16;
    sasori_get_work(actor)->x_speed = 65536;
    actor->actflg = 4;
    actwk[30].actflg = 4;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 180, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, -3, sasori_get_work(actor)->tail_x_offset);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[30].actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[30].cddat);

    reset_sasori_state();
    actor = &actwk[3];
    setup_moving_main(actor, 30);
    sasori_get_work(actor)->x_speed = 65536;
    queue_emycol(7);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, -3, sasori_get_work(actor)->tail_x_offset);
}

static void test_move_player_range_can_wait_or_reverse(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    setup_moving_main(actor, 30);
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 3, sasori_get_work(actor)->tail_x_offset);

    reset_sasori_state();
    actor = &actwk[3];
    setup_moving_main(actor, 30);
    actwk[0].xposi.w.h = 110;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, -3, sasori_get_work(actor)->tail_x_offset);

    reset_sasori_state();
    actor = &actwk[3];
    setup_moving_main(actor, 30);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -65536, sasori_get_work(actor)->x_speed);

    reset_sasori_state();
    actor = &actwk[3];
    setup_moving_main(actor, 30);
    sasori_get_work(actor)->x_speed = 65536;
    actwk[0].xposi.w.h = 110;
    actwk[0].yposi.w.h = 200;
    queue_emycol(0);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 65536, sasori_get_work(actor)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 3, sasori_get_work(actor)->tail_x_offset);
}

static void test_waita_and_waitc_move_tail_for_both_directions(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *tail = &actwk[30];

    reset_sasori_state();
    actor->actno = 33;
    actor->r_no0 = 6;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    sasori_get_work(actor)->origin_x = 100;
    sasori_get_work(actor)->x_speed = -65536;
    sasori_get_work(actor)->tail_index = 30;
    sasori_get_work(actor)->tail_x_offset = 3;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 29, sasori_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 103, tail->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 184, tail->yposi.w.h);

    reset_logs();
    sasori_get_work(actor)->timer = 1;
    tail->xposi.w.h = 103;
    tail->yposi.w.h = 184;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 108, tail->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 179, tail->yposi.w.h);

    reset_logs();
    actor->r_no0 = 8;
    sasori_get_work(actor)->timer = 1;
    sasori_get_work(actor)->x_speed = 65536;
    tail->xposi.w.h = 103;
    tail->yposi.w.h = 184;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 98, tail->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 179, tail->yposi.w.h);

    reset_logs();
    actor->r_no0 = 16;
    sasori_get_work(actor)->timer = 1;
    sasori_get_work(actor)->x_speed = 65536;
    tail->xposi.w.h = 103;
    tail->yposi.w.h = 184;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 108, tail->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 189, tail->yposi.w.h);
}

static void test_waitb_spawns_projectile_and_handles_skips(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *shot = &actwk[40];

    reset_sasori_state();
    actor->actno = 33;
    actor->r_no0 = 10;
    sasori_get_work(actor)->origin_x = 100;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 9, sasori_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_logs();
    actor->actno = 33;
    actor->r_no0 = 12;
    actor->actflg = 128;
    actor->sprpri = 3;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    sasori_get_work(actor)->origin_x = 100;
    sasori_get_work(actor)->timer = 1;
    sasori_get_work(actor)->x_speed = -65536;
    queue_actwkchk(shot);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, -1, shot->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 178, shot->colino);
    TEST_ASSERT_TRUE(ctx, shot->patbase == pat_tama);
    TEST_ASSERT_EQ_INT(ctx, 3, sasori_get_work(shot)->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 96, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 176, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -196608, sasori_get_work(shot)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);

    reset_sasori_state();
    actor = &actwk[3];
    shot = &actwk[40];
    actor->actno = 33;
    actor->r_no0 = 12;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    sasori_get_work(actor)->origin_x = 100;
    sasori_get_work(actor)->timer = 1;
    sasori_get_work(actor)->x_speed = 65536;
    queue_actwkchk(shot);

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 104, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 196608, sasori_get_work(shot)->x_speed);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_sasori_state();
    actor = &actwk[3];
    actor->actno = 33;
    actor->r_no0 = 12;
    actor->userflag.b.h = 1;
    sasori_get_work(actor)->origin_x = 100;
    sasori_get_work(actor)->timer = 1;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_waitc_and_waitd_timer_entry_states(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sasori_state();
    actor->actno = 33;
    actor->r_no0 = 14;
    sasori_get_work(actor)->origin_x = 100;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 29, sasori_get_work(actor)->timer);

    reset_logs();
    actor->r_no0 = 18;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 20, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 29, sasori_get_work(actor)->timer);

    reset_logs();
    actor->r_no0 = 20;
    sasori_get_work(actor)->timer = 1;

    sasori(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
}

static void test_child_tail_projectile_and_missing_parent_paths(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[30];

    reset_sasori_state();
    parent->actno = 33;
    child->userflag.b.l = 1;
    sasori_get_work(child)->parent_index = 3;

    sasori(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_sasori_state();
    child = &actwk[30];
    child->userflag.b.l = 1;
    sasori_get_work(child)->parent_index = 3;

    sasori(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_sasori_state();
    parent = &actwk[3];
    child = &actwk[30];
    parent->actno = 33;
    child->userflag.b.l = -1;
    child->xposi.l = 100 << 16;
    sasori_get_work(child)->parent_index = 3;
    sasori_get_work(child)->x_speed = 196608;

    sasori(child);

    TEST_ASSERT_EQ_INT(ctx, 103, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_tama);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == child);
}

TEST_MAIN_BEGIN;
    test_patterns_capture_literal_data(&ctx);
    test_enemy_suicide_short_circuits_main_actor(&ctx);
    test_init_records_origin_and_time_variant(&ctx);
    test_fall_lands_and_initializes_tail(&ctx);
    test_fall_without_ground_or_tail_slot(&ctx);
    test_move_updates_tail_without_reversal(&ctx);
    test_move_reverses_at_range_or_floor_edge(&ctx);
    test_move_player_range_can_wait_or_reverse(&ctx);
    test_waita_and_waitc_move_tail_for_both_directions(&ctx);
    test_waitb_spawns_projectile_and_handles_skips(&ctx);
    test_waitc_and_waitd_timer_entry_states(&ctx);
    test_child_tail_projectile_and_missing_parent_paths(&ctx);
TEST_MAIN_END

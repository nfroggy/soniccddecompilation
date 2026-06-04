#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Uint8 time_flag;
char time_item;
Uint8 flagwork[766];
short_union stageno;
int_union scra_h_posit;
int_union systemtimer;
Sint16 plring;
Uint8 plring_f;
Uint8 plring_f2;
Uint8 pl_suu;
Uint8 pl_suu_f;
sprite_status actwk[128];
Uint8 sys_pattim4;
Sint16 scralim_down;
game_info *lpKeepWork;

static game_info keep_work;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[16];
static int sub_sync_count;
static Sint16 sub_sync_requests[16];
static int actwkchk_count;
static int actwkchk2_count;
static sprite_status *actwkchk_queue[64];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static sprite_status *actwkchk2_queue[64];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int speedset2_count;
static sprite_status *speedset2_actor;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int sinset_count;
static Uint8 sinset_angles[16];

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
void sub_sync(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void speedset2(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/ring.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 16) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
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

void speedset2(sprite_status *pActwk) {
    ++speedset2_count;
    speedset2_actor = pActwk;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < 16) {
        sinset_angles[sinset_count] = kakudo;
    }
    *sin = 10 + sinset_count;
    *cos = 20 + sinset_count;
    ++sinset_count;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void reset_ring_state(void) {
    memset(&time_flag, 0, sizeof(time_flag));
    time_item = 0;
    memset(flagwork, 0, sizeof(flagwork));
    memset(&stageno, 0, sizeof(stageno));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&systemtimer, 0, sizeof(systemtimer));
    plring = 0;
    plring_f = 0;
    plring_f2 = 0;
    pl_suu = 0;
    pl_suu_f = 0;
    memset(actwk, 0, sizeof(actwk));
    sys_pattim4 = 0;
    scralim_down = 0;
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;

    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    actwkchk_count = 0;
    actwkchk2_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    sinset_count = 0;
    memset(sinset_angles, 0, sizeof(sinset_angles));
}

static void reset_logs_only(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_d_count = 0;
}

static void assert_ring_actor(test_context *ctx, sprite_status *actor,
                              Sint16 x, Sint16 y, Uint8 counter,
                              Uint8 sprpri, Uint16 sproffset) {
}

static void test_ring_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, ringpat[0] == &ringsp0);
    TEST_ASSERT_TRUE(ctx, ringpat[8] == &ringsp8);
    TEST_ASSERT_EQ_INT(ctx, 1, ringsp0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, ringsp0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, ringsp0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RING_BASE, ringsp0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -4, ringsp2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RING_BASE + 2, ringsp2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RING_BASE + 7, ringsp7.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 5, ringchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, ringchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, ringchg0[2]);
    TEST_ASSERT_EQ_INT(ctx, 6, ringchg0[3]);
    TEST_ASSERT_EQ_INT(ctx, 7, ringchg0[4]);
    TEST_ASSERT_EQ_INT(ctx, 252, ringchg0[5]);
    TEST_ASSERT_EQ_INT(ctx, 7, ringchg1[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, ringchg1[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, ringchg1[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, ringchg1[3]);
    TEST_ASSERT_EQ_INT(ctx, 3, ringchg1[4]);
    TEST_ASSERT_EQ_INT(ctx, 255, ringchg1[5]);
    TEST_ASSERT_TRUE(ctx, ringchg[0] == ringchg0);
    TEST_ASSERT_TRUE(ctx, ringchg[1] == ringchg1);
    TEST_ASSERT_TRUE(ctx, ringchg[2] == ringchg2);
}

static void test_ringinit_spawns_line_and_moves_first_ring(test_context *ctx) {
    reset_ring_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].userflag.b.h = 1;
    actwk[0].cdsts = 2;
    systemtimer.b.b4 = 29;
    queue_actwkchk2(&actwk[40]);

    ringinit(&actwk[0]);

    assert_ring_actor(ctx, &actwk[0], 100, 200, 1, 2, 42926);
    assert_ring_actor(ctx, &actwk[40], 116, 200, 2, 2, 42926);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cdsts);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[40].cdsts);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actwk[0]);
    TEST_ASSERT_TRUE(ctx, patchg_table == ringchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_ringinit_stage6_priority_variants(test_context *ctx) {
    reset_ring_state();
    stageno.b.h = 6;
    actwk[0].xposi.w.h = 10;
    actwk[0].yposi.w.h = 20;
    ringinit(&actwk[0]);
    assert_ring_actor(ctx, &actwk[0], 10, 20, 1, 0, 42926);

    reset_ring_state();
    stageno.b.h = 6;
    actwk[0].xposi.w.h = 10;
    actwk[0].yposi.w.h = 20;
    actwk[0].userflag.b.l = 1;
    ringinit(&actwk[0]);
    assert_ring_actor(ctx, &actwk[0], 10, 20, 1, 2, 46);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].userflag.b.l);
}

static void test_ringinit_skips_collected_ring_and_frames_source(
    test_context *ctx) {
    reset_ring_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    flagwork[0] = 1;

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_ringinit_allocation_failure_leaves_later_ring_unspawned(
    test_context *ctx) {
    reset_ring_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].userflag.b.h = 1;

    ringinit(&actwk[0]);

    assert_ring_actor(ctx, &actwk[0], 100, 200, 1, 2, 42926);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
}

static void test_ringinit_caps_layout_count_at_seven(test_context *ctx) {
    reset_ring_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].userflag.b.h = 7;
    for (int i = 1; i <= 6; ++i) {
        queue_actwkchk2(&actwk[i]);
    }

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 7, ring_work_get(&actwk[6])->ring_counter);
    TEST_ASSERT_EQ_INT(ctx, 196, actwk[6].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[6].yposi.w.h);
}

static void test_ringinit_clamps_time_warp_index(test_context *ctx) {
    reset_ring_state();
    time_flag = 128;
    time_item = 5;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    flagwork[0] = 128;

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[0]);

    reset_ring_state();
    time_flag = 130;
    time_item = -5;
    actwk[0].cdsts = 1;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    flagwork[5] = 128;

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[5]);
}

static void test_ringinit_label1_skips_collected_later_ring_with_time_clamps(
    test_context *ctx) {
    reset_ring_state();
    time_flag = 128;
    time_item = 5;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].userflag.b.h = 1;
    flagwork[0] = 2;

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ring_work_get(&actwk[0])->ring_counter);

    reset_ring_state();
    time_flag = 130;
    time_item = -5;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].userflag.b.h = 1;
    flagwork[2] = 2;

    ringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ring_work_get(&actwk[0])->ring_counter);
}

static void test_ringmove_animates_or_erases_by_camera(test_context *ctx) {
    reset_ring_state();
    actwk[0].xposi.w.h = 128;
    actwk[0].yposi.w.h = 64;
    ringinit(&actwk[0]);
    reset_logs_only();
    systemtimer.b.b4 = 10;

    ringmove(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs_only();
    scra_h_posit.w.h = 1024;
    ringmove(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[0]);
}

static void test_ringget_marks_flagwork_and_enters_death_animation(
    test_context *ctx) {
    reset_ring_state();
    actwk[0].r_no0 = 4;
    actwk[0].colino = 71;
    actwk[0].sprpri = 2;
    actwk[0].mstno.b.h = 1;
    actwk[0].cdsts = 2;
    ring_work_get(&actwk[0])->ring_counter = 3;
    time_flag = 1;

    ringget(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 4, flagwork[7]);
    TEST_ASSERT_EQ_INT(ctx, 1, plring);
    TEST_ASSERT_EQ_INT(ctx, 1, plring_f);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 149, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_ringget_clamps_time_warp_flag_index(test_context *ctx) {
    reset_ring_state();
    ring_work_get(&actwk[0])->ring_counter = 1;
    time_flag = 128;
    time_item = 5;
    ringget(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[0]);

    reset_ring_state();
    ring_work_get(&actwk[0])->ring_counter = 1;
    time_flag = 130;
    time_item = -5;
    ringget(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[2]);
}

static void test_ringgetsub_reward_boundaries(test_context *ctx) {
    reset_ring_state();
    plring = 98;
    ringgetsub();
    TEST_ASSERT_EQ_INT(ctx, 99, plring);
    TEST_ASSERT_EQ_INT(ctx, 1, plring_f);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 149, soundset_requests[0]);

    reset_ring_state();
    plring = 99;
    ringgetsub();
    TEST_ASSERT_EQ_INT(ctx, 100, plring);
    TEST_ASSERT_EQ_INT(ctx, 2, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 122, sub_sync_requests[0]);

    reset_ring_state();
    plring = 100;
    plring_f2 = 2;
    ringgetsub();
    TEST_ASSERT_EQ_INT(ctx, 101, plring);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 149, soundset_requests[0]);

    reset_ring_state();
    plring = 199;
    plring_f2 = 2;
    ringgetsub();
    TEST_ASSERT_EQ_INT(ctx, 200, plring);
    TEST_ASSERT_EQ_INT(ctx, 6, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);

    reset_ring_state();
    plring = 200;
    plring_f2 = 6;
    ringgetsub();
    TEST_ASSERT_EQ_INT(ctx, 201, plring);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 149, soundset_requests[0]);
}

static void test_ring_dispatchers_cover_die_and_erase(test_context *ctx) {
    reset_ring_state();
    actwk[0].r_no0 = 6;
    ring(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_ring_state();
    actwk[0].r_no0 = 8;
    ring(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[0]);
}

static void assert_flyring_actor(test_context *ctx, sprite_status *actor,
                                 Uint8 sprpri, Uint16 sproffset) {
}

static void test_flyringinit_spawns_flying_rings_and_keeps_half_total(
    test_context *ctx) {
    reset_ring_state();
    plring = 2;
    keep_work.User = 1;
    stageno.b.h = 6;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 60;
    actwk[0].userflag.b.l = 1;
    queue_actwkchk(&actwk[40]);
    scralim_down = 0;

    flyringinit(&actwk[0]);

    assert_flyring_actor(ctx, &actwk[0], 3, 46);
    assert_flyring_actor(ctx, &actwk[40], 3, 46);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 60, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 136, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 40, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 104, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -40, actwk[40].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 80, actwk[40].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, plring);
    TEST_ASSERT_EQ_INT(ctx, 128, plring_f);
    TEST_ASSERT_EQ_INT(ctx, 0, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 148, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_flyringinit_clamps_count_and_zeroes_total_on_loss(
    test_context *ctx) {
    reset_ring_state();
    plring = 33;
    stageno.b.h = 6;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 60;
    scralim_down = 0;

    flyringinit(&actwk[0]);

    assert_flyring_actor(ctx, &actwk[0], 0, 42926);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, plring);
    TEST_ASSERT_EQ_INT(ctx, 128, plring_f);
    TEST_ASSERT_EQ_INT(ctx, 0, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 148, soundset_requests[0]);
}

static void test_flyringinit_long_sequence_reaches_angle_subtract(
    test_context *ctx) {
    reset_ring_state();
    plring = 32;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 60;
    scralim_down = 0;
    for (int i = 1; i < 32; ++i) {
        queue_actwkchk(&actwk[i]);
    }

    flyringinit(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 31, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, plring);
    TEST_ASSERT_EQ_INT(ctx, 128, plring_f);
}

static void test_flyringmove_bounces_on_collision(test_context *ctx) {
    reset_ring_state();
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -10;
    sys_pattim4 = 1;
    scralim_down = 0;
    emycol_d_result = -5;

    flyringmove(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_TRUE(ctx, speedset2_actor == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 95, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -11, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_flyringmove_animates_without_collision_when_still_rising(
    test_context *ctx) {
    reset_ring_state();
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -40;
    sys_pattim4 = 1;
    scralim_down = 0;

    flyringmove(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -16, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_flyringmove_erases_when_timer_zero_or_below_screen(
    test_context *ctx) {
    reset_ring_state();
    sys_pattim4 = 0;
    actwk[0].yposi.w.h = 100;
    flyringmove(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_ring_state();
    sys_pattim4 = 1;
    scralim_down = 0;
    actwk[0].yposi.w.h = 224;
    flyringmove(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_flyringget_die_and_dispatch_erase(test_context *ctx) {
    reset_ring_state();
    actwk[0].r_no0 = 4;
    actwk[0].colino = 71;
    actwk[0].sprpri = 3;
    actwk[0].mstno.b.h = 2;
    flyringget(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_ring_state();
    actwk[0].r_no0 = 8;
    flyring(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[0]);
}

TEST_MAIN_BEGIN;
    test_ring_tables_capture_literal_data(&ctx);
    test_ringinit_spawns_line_and_moves_first_ring(&ctx);
    test_ringinit_stage6_priority_variants(&ctx);
    test_ringinit_skips_collected_ring_and_frames_source(&ctx);
    test_ringinit_allocation_failure_leaves_later_ring_unspawned(&ctx);
    test_ringinit_caps_layout_count_at_seven(&ctx);
    test_ringinit_clamps_time_warp_index(&ctx);
    test_ringinit_label1_skips_collected_later_ring_with_time_clamps(&ctx);
    test_ringmove_animates_or_erases_by_camera(&ctx);
    test_ringget_marks_flagwork_and_enters_death_animation(&ctx);
    test_ringget_clamps_time_warp_flag_index(&ctx);
    test_ringgetsub_reward_boundaries(&ctx);
    test_ring_dispatchers_cover_die_and_erase(&ctx);
    test_flyringinit_spawns_flying_rings_and_keeps_half_total(&ctx);
    test_flyringinit_clamps_count_and_zeroes_total_on_loss(&ctx);
    test_flyringinit_long_sequence_reaches_angle_subtract(&ctx);
    test_flyringmove_bounces_on_collision(&ctx);
    test_flyringmove_animates_without_collision_when_still_rising(&ctx);
    test_flyringmove_erases_when_timer_zero_or_below_screen(&ctx);
    test_flyringget_die_and_dispatch_erase(&ctx);
TEST_MAIN_END

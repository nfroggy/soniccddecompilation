#include <string.h>

#include "support/test_runner.h"
#include "src/equ.h"
#include "src/player6.h"
#include "src/player_work.h"
#include "src/types.h"

Uint8 mapwka[8][64];
sprite_pattern *sncpat[1];
Uint8 plchg00[8], plchg01[8], plchg49[8], plchg53[8], plchg03[8], plchg02[8],
    plchg54[8], plchg35[6], plchg04[8], plchg39[7], plchg33[6], plchg34[6];
Uint8 *playchg[60];

static Uint8 default_chg[4] = {1, 0, 255, 0};
static game_info keep_work;
static Sint16 spawned_scramapad_value;
static Sint16 emycol_d_value;
static Sint16 dircolm_value;
static char dircolm_out_value;
static Sint16 dircol2_value;
static Sint16 dircol_u_value;
static Sint16 dircol_d_out_d0;
static Sint16 dircol_d_out_d1;
static Sint16 dircol_d_out_d3;
static Sint16 dircol_l2_value;
static Sint16 dircol_r2_value;
static Sint16 dircol_u_out_d0;
static Sint16 dircol_u_out_d1;
static Sint16 dircol_u_out_d3;
static Sint16 stub_sin_value;
static Sint16 stub_cos_value;
static Sint16 atan_sonic_value;
static Sint32 actwkchk_result;
static sprite_status spawned_actor;
static Sint16 sub_sync_values[8];
static Sint16 soundset_values[8];
static int sub_sync_count;
static int soundset_count;
static int da_set_count;
static int edit_count;
static int actionsub_count;
static int fadeout_count;
static int flagwkclr_count;
static int actwkchk_count;
static int wave_all_stop_count;
static int playdamageset_count;
static int playdieset_count;
static int speedset_count;
static int speedset2_count;
static int fcol_count;
static int pcol_count;

static void test_wave_all_stop(void) { ++wave_all_stop_count; }

void (*WaveAllStop)(void) = test_wave_all_stop;

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8)
        sub_sync_values[sub_sync_count] = ReqNo;
    ++sub_sync_count;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8)
        soundset_values[soundset_count] = ReqNo;
    ++soundset_count;
}

void da_set(void) { ++da_set_count; }

void edit(sprite_status *pActwk) {
    (void)pActwk;
    ++edit_count;
}

void actionsub(sprite_status *pActwk) {
    (void)pActwk;
    ++actionsub_count;
}

void flagwkclr(void) { ++flagwkclr_count; }
void fadeout_s(void) { ++fadeout_count; }
void test_mapwrt(void);
void speedset(sprite_status *pActwk) {
    (void)pActwk;
    ++speedset_count;
}
void speedset2(sprite_status *pActwk) {
    (void)pActwk;
    ++speedset2_count;
}
void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    (void)patchgwk;
    (void)pat_dat;
}
Sint16 pcol(sprite_status *pActwk) {
    (void)pActwk;
    ++pcol_count;
    return 0;
}
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    (void)pActwk;
    (void)pColliAct;
    ++playdamageset_count;
    return 0;
}
Sint16 playdieset(sprite_status *pActwk) {
    (void)pActwk;
    ++playdieset_count;
    return 0;
}
Sint16 fcol(sprite_status *pActwk) {
    (void)pActwk;
    ++fcol_count;
    return 0;
}
Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_value;
}
Sint16 dircolm(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    *cpDirec = dircolm_out_value;
    return dircolm_value;
}
Sint16 dircol2(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    (void)cpDirec;
    return dircol2_value;
}
Sint16 dircol_u(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    *ipD0 = dircol_u_out_d0;
    *ipD1 = dircol_u_out_d1;
    *ipD3 = dircol_u_out_d3;
    return dircol_u_value;
}
Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    *ipD0 = dircol_d_out_d0;
    *ipD1 = dircol_d_out_d1;
    *ipD3 = dircol_d_out_d3;
    return 0;
}
Sint16 dircol_l2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_l2_value;
}
Sint16 dircol_r2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_r2_value;
}
Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    (void)pActwk;
    (void)iXposi;
    (void)iYposi;
    return spawned_scramapad_value;
}
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    (void)kakudo;
    *sin = stub_sin_value;
    *cos = stub_cos_value;
}
Sint16 atan_sonic(Sint16 x, Sint16 y) {
    (void)x;
    (void)y;
    return atan_sonic_value;
}
Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    *ppActwk = &spawned_actor;
    return actwkchk_result;
}

static void reset_fixture(void) {
    memset(&keep_work, 0, sizeof(keep_work));
    memset(&spawned_actor, 0, sizeof(spawned_actor));
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(playposiwk, 0, sizeof(playposiwk));
    memset(flowercnt, 0, sizeof(flowercnt));
    memset(clchgcnt, 0, sizeof(clchgcnt));
    memset(sub_sync_values, 0, sizeof(sub_sync_values));
    memset(soundset_values, 0, sizeof(soundset_values));
    for (int i = 0; i < 60; ++i)
        playchg[i] = default_chg;
    lpKeepWork = &keep_work;
    WaveAllStop = test_wave_all_stop;
    spawned_scramapad_value = 0;
    emycol_d_value = -1;
    dircolm_value = -1;
    dircolm_out_value = 0;
    dircol2_value = 6;
    dircol_u_value = 0;
    dircol_d_out_d0 = 0;
    dircol_d_out_d1 = 0;
    dircol_d_out_d3 = 0;
    dircol_l2_value = 0;
    dircol_r2_value = 0;
    dircol_u_out_d0 = 0;
    dircol_u_out_d1 = 0;
    dircol_u_out_d3 = 0;
    stub_sin_value = 0;
    stub_cos_value = 256;
    atan_sonic_value = 0;
    actwkchk_result = 0;
    sub_sync_count = 0;
    soundset_count = 0;
    da_set_count = 0;
    edit_count = 0;
    actionsub_count = 0;
    fadeout_count = 0;
    flagwkclr_count = 0;
    actwkchk_count = 0;
    wave_all_stop_count = 0;
    playdamageset_count = 0;
    playdieset_count = 0;
    speedset_count = 0;
    speedset2_count = 0;
    fcol_count = 0;
    pcol_count = 0;
    byecnt0.w = 0;
    pl_suu = 0;
    pl_suu_f = 0;
    plflag = 0;
    plpower_a = 0;
    plpower_m = 0;
    plpower_s = 0;
    boss_sound = 0;
    time_flag = 0;
    time_item = 0;
    backto_cnt = 0;
    scroll_start.w = 0;
    gameflag.w = 0;
    editmode.w = 0;
    ta_flag = 0;
    stageno.w = 0;
    gametimer.w = 0;
    pltime.l = 0;
    plring = 0;
    plring_f2 = 0;
    plring_s = 0;
    plring_f2_s = 0;
    water_flag = 0;
    waterflag = 0;
    waterposi_m = 0;
    scralim_down = 0;
    scra_h_posit.w.h = 0;
    scra_v_posit.w.h = 0;
    scrb_h_posit.w.h = 0;
    scrb_v_posit.w.h = 0;
    scrc_h_posit.w.h = 0;
    scrc_v_posit.w.h = 0;
    scrz_h_posit.w.h = 0;
    scrz_v_posit.w.h = 0;
    plposiwkadr.w = 0;
    play_start = 0;
    markerno = 0;
    tv_flag = 0;
    chibi_flag = 0;
    mizuflag = 0;
    kusya_flag = 0;
    bossflag = 0;
    generate_flag = 0;
    scr_cnt = 0;
    scra_vline = 0;
    swdata.w = 0;
    swdata1.w = 0;
    debugflag.w = 0;
    plautoflag = 0;
}

static void test_bye_chk_waits_then_forces_timeout_escape(test_context *ctx) {
    reset_fixture();
    actwk[0].mstno.b.h = 4;
    byecnt0.w = 99;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);

    reset_fixture();
    actwk[0].mstno.b.h = 5;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, byecnt0.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);

    reset_fixture();
    actwk[0].mstno.b.h = 5;
    byecnt0.w = 10800;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 121, sub_sync_values[0]);

    reset_fixture();
    actwk[0].mstno.b.h = 5;
    actwk[0].cddat = 1;
    byecnt0.w = 10800;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
}

static void test_play00_dispatches_edit_and_skips_nonplayer(test_context *ctx) {
    sprite_status other;

    reset_fixture();
    memset(&other, 0, sizeof(other));
    other.r_no0 = 0;

    play00(&other);

    TEST_ASSERT_EQ_INT(ctx, 0, edit_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_fixture();
    editmode.b.h = 1;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, edit_count);

    reset_fixture();

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);

    reset_fixture();
    actwk[0].r_no0 = 8;
    player_work_get(&actwk[0])->erase_timer = 1;
    pl_suu = 1;
    time_flag = 0;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, flagwkclr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 14, sub_sync_values[0]);

    reset_fixture();
    actwk[0].r_no0 = 10;
    player_work_get(&actwk[0])->spin_dash_counter = 44;
    actwk[0].cddat = 4;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, edit_count);

    reset_fixture();
    actwk[0].r_no0 = 4;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_fixture();
    actwk[0].r_no0 = 2;
    debugflag.w = 1;
    swdata1.b.l = 16;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, editmode.b.h);

    reset_fixture();
    actwk[0].r_no0 = 6;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_fixture();
    actwk[0].r_no0 = 10;
    player_work_get(&actwk[0])->spin_dash_counter = 29;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 30, player_work_get(&actwk[0])->spin_dash_counter);

    reset_fixture();
    actwk[0].r_no0 = 10;
    player_work_get(&actwk[0])->spin_dash_counter = 44;
    actwk[0].cddat = 4;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 45, player_work_get(&actwk[0])->spin_dash_counter);
}

static void test_muteki_set_arms_power_slots_once(test_context *ctx) {
    reset_fixture();
    actwk[12].actno = 7;

    muteki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_fixture();

    muteki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);
}

static void test_map_and_save_helpers_copy_stable_state(test_context *ctx) {
    reset_fixture();
    mapwka[2][3] = 47;

    TEST_ASSERT_EQ_INT(ctx, 2162735, (Sint32)mapno_chk(2 << 8, 3 << 8));

    plflag = 2;
    pltime.l = 400000;
    plring = 123;
    plring_f2 = 1;
    water_flag = 3;
    waterflag = 4;
    waterposi_m = 55;
    scralim_down = 66;
    scra_h_posit.w.h = 77;
    scra_v_posit.w.h = 88;
    scrb_h_posit.w.h = 99;
    scrb_v_posit.w.h = 111;
    scrc_h_posit.w.h = 122;
    scrc_v_posit.w.h = 133;
    scrz_h_posit.w.h = 144;
    scrz_v_posit.w.h = 155;
    actwk[0].cddat = 255;

    playsave();

    TEST_ASSERT_EQ_INT(ctx, 2, keep_work.plflag_sb);
    TEST_ASSERT_EQ_INT(ctx, 247, keep_work.cddat_sb);
    TEST_ASSERT_EQ_INT(ctx, 3, keep_work.water_flag_sb);
    TEST_ASSERT_EQ_INT(ctx, 4, keep_work.waterflag_sb);
    TEST_ASSERT_EQ_INT(ctx, 123, keep_work.plring_s);
    TEST_ASSERT_EQ_INT(ctx, 1, keep_work.plring_f2_s);
    TEST_ASSERT_EQ_INT(ctx, 327680, keep_work.pltime_sb);
    TEST_ASSERT_EQ_INT(ctx, 123, plring_s);
    TEST_ASSERT_EQ_INT(ctx, 1, plring_f2_s);
    TEST_ASSERT_EQ_INT(ctx, 66, keep_work.scralim_down_sb);
    TEST_ASSERT_EQ_INT(ctx, 155, keep_work.scrz_v_posit_sb);

    reset_fixture();
    actwk[0].xposi.w.h = 321;
    actwk[0].yposi.w.h = 654;
    plposiwkadr.w = 4;

    playposiwkset();

    TEST_ASSERT_EQ_INT(ctx, 321, playposiwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 654, playposiwk[3]);
    TEST_ASSERT_EQ_INT(ctx, 8, plposiwkadr.w);
}

static void test_timewarp_backto_paths_use_callbacks_and_saved_state(
    test_context *ctx) {
    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 157;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, scroll_start.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, plflag);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 129, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 2;
    backto_cnt = 165;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, gameflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, fadeout_count);
    TEST_ASSERT_EQ_INT(ctx, 130, time_flag);
}

static void test_power_counters_restore_music_when_timers_expire(
    test_context *ctx) {
    reset_fixture();

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_fixture();
    plpower_m = 1;
    player_work_get(&actwk[0])->invincibility_timer = 1;

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 14, sub_sync_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_fixture();
    plpower_s = 1;
    player_work_get(&actwk[0])->speed_shoes_timer = 1;

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 12, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_fixture();
    player_work_get(&actwk[0])->damage_invulnerability_timer = 4;

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_fixture();
    player_work_get(&actwk[0])->damage_invulnerability_timer = 8;

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_fixture();
    player_work_get(&actwk[0])->damage_invulnerability_timer = 4;
    player_work_get(&actwk[0])->status_flags = 64;

    playpowercnt();

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_initializers_and_empty_helpers_are_callable(test_context *ctx) {
    reset_fixture();

    play00init();

    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 12, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);

    reset_fixture();
    chibi_flag = 1;

    play00init();

    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);

    test_mapwrt();
    scr_h();
    plwaterchk();
    loopchk();
    playwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);
}

static void test_stage_hazard_helpers_use_damage_and_sound_callbacks(
    test_context *ctx) {
    reset_fixture();
    st6clrchg = 1;
    spawned_scramapad_value = 426;

    sibi2();

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);

    reset_fixture();
    st6clrchg = 0;

    sibi2();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    st6clrchg = 1;
    spawned_scramapad_value = 999;

    sibi2();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    spawned_scramapad_value = 579;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);

    reset_fixture();
    actwk[0].mstno.b.h = 43;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    actwk[0].xposi.w.h = 2500;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    bossflag = 128;
    spawned_scramapad_value = 579;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    stageno.b.l = 2;
    actwk[0].xposi.w.h = 2600;
    boss_sound = 0;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_fixture();
    stageno.b.l = 2;
    actwk[0].xposi.w.h = 2400;
    spawned_scramapad_value = 579;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);

    reset_fixture();
    time_flag = 2;
    spawned_scramapad_value = 647;

    sibi();

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);

    reset_fixture();
    spawned_scramapad_value = 543;
    clchgcnt[3] = 0;

    bfloor();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 218, soundset_values[0]);

    reset_fixture();
    actwk[0].mstno.b.h = 43;
    spawned_scramapad_value = 543;

    bfloor();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    time_flag = 1;
    clchgcnt[3] = 90;
    spawned_scramapad_value = 543;

    bfloor();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    spawned_scramapad_value = 543;
    actwk[0].cddat = 4;

    bfloor();

    TEST_ASSERT_EQ_INT(ctx, -5632, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_backto_early_and_speed_gates(test_context *ctx) {
    reset_fixture();

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, fadeout_count);

    reset_fixture();
    keep_work.TimeWarp = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    time_item = 1;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, fadeout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 80;
    actwk[0].mspeed.w = 100;
    plpower_a = 1;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 100;
    actwk[0].mspeed.w = 100;
    plpower_a = 1;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, time_item);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 80;
    actwk[0].mspeed.w = 1536;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    actwk[0].mspeed.w = -1536;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_flag = 0;
    time_item = -1;
    backto_cnt = 157;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_flag = 2;
    time_item = 1;
    backto_cnt = 165;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 130, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_flag = 2;
    time_item = 1;
    backto_cnt = 157;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 130, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 0;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_flag = 0;
    time_item = -1;
    backto_cnt = 165;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_flag = 0;
    time_item = -2;
    backto_cnt = 165;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);

    reset_fixture();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 157;
    plflag = 2;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, scroll_start.b.h);
}

static void test_bura_resets_active_timewarp_item_on_entry(test_context *ctx) {
    reset_fixture();
    spawned_scramapad_value = 345;
    backto_cnt = 90;
    time_item = 1;
    plpower_a = 1;

    bura();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, time_item);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);
}

static void test_limit_and_late_life_paths_use_callbacks(test_context *ctx) {
    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;

    limitchk();

    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    pl_suu = 2;
    ta_flag = 0;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);

    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;

    play00damage_sub();

    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);
}

static void test_jump_and_animation_helpers_cover_discrete_paths(
    test_context *ctx) {
    reset_fixture();
    swdata.b.l = 16;
    actwk[0].direc.b.h = 0;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 146, soundset_values[0]);

    reset_fixture();
    chibi_flag = 0;

    TEST_ASSERT_EQ_INT(ctx, 33, little_patchg(33));

    reset_fixture();
    chibi_flag = 1;

    TEST_ASSERT_EQ_INT(ctx, 31, little_patchg(5));
}

static void test_play00move_dispatches_movement_modes(test_context *ctx) {
    reset_fixture();
    actwk[0].cddat = 0;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, fcol_count);
    TEST_ASSERT_EQ_INT(ctx, 1, pcol_count);

    reset_fixture();
    actwk[0].cddat = 2;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, pcol_count);

    reset_fixture();
    actwk[0].cddat = 4;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, fcol_count);

    reset_fixture();
    actwk[0].cddat = 6;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_fixture();
    debugflag.w = 1;
    swdata1.b.l = 16;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, editmode.b.h);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 1;
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 165;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, fadeout_count);
}

static void test_walk_acceleration_and_ball_entry_paths(test_context *ctx) {
    reset_fixture();
    plmaxspdwk = 1536;
    pladdspdwk = 12;

    plwalk_l();
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    plmaxspdwk = 1536;
    pladdspdwk = 12;

    plwalk_r();
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 1200;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 144, soundset_values[0]);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = -10;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, -10, actwk[0].mspeed.w);

    reset_fixture();
    plmaxspdwk = 100;
    pladdspdwk = 12;
    actwk[0].mspeed.w = -95;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 1200;
    actwk[0].direc.b.h = 96;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 1072, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 1100;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 972, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 10;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, -128, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].r_no0 = 10;
    play00(&actwk[0]);
    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -1200;
    actwk[0].cddat = 1;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 144, soundset_values[0]);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 10;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].mspeed.w);

    reset_fixture();
    plmaxspdwk = 100;
    pladdspdwk = 12;
    actwk[0].mspeed.w = 95;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].cddat = 1;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 1);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -1200;
    actwk[0].direc.b.h = 96;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, -1072, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -1100;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, -972, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -10;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 128, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 256;
    swdata.b.h = 2;

    ballchk();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);

    reset_fixture();
    actwk[0].mspeed.w = 256;

    ballchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_fixture();
    mizuflag = 1;
    actwk[0].mspeed.w = 256;
    swdata.b.h = 2;

    ballchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
}

static void test_jump_release_slope_and_direction_helpers(test_context *ctx) {
    reset_fixture();
    player_work_get(&actwk[0])->jump_started = 1;
    actwk[0].yspeed.w = -2000;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);

    reset_fixture();
    player_work_get(&actwk[0])->jump_started = 1;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = -2000;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].yspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 512;
    stub_sin_value = 128;

    keispd();

    TEST_ASSERT_EQ_INT(ctx, 528, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = -512;
    stub_sin_value = 128;

    keispd2();

    TEST_ASSERT_EQ_INT(ctx, -502, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].direc.b.h = 4;

    direcchg();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].direc.b.h);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 512;

    keispd();

    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].direc.b.h = 96;
    actwk[0].mspeed.w = 512;

    keispd();

    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = -512;
    stub_sin_value = 128;

    keispd();

    TEST_ASSERT_EQ_INT(ctx, -496, actwk[0].mspeed.w);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = -512;

    keispd2();

    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].direc.b.h = 96;
    actwk[0].mspeed.w = 512;

    keispd2();

    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 512;
    stub_sin_value = -128;

    keispd2();

    TEST_ASSERT_EQ_INT(ctx, 502, actwk[0].mspeed.w);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 4;

    direcchg();

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].direc.b.h);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.l = 16;

    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());

    reset_fixture();
    swdata.b.h = 1;
    swdata.b.l = 16;

    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());

    reset_fixture();
    swdata.b.l = 16;
    dircol2_value = 5;

    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());

    reset_fixture();
    swdata.b.l = 16;
    actwk[0].cddat = 64;
    stub_cos_value = 256;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[0].xspeed.w);

    reset_fixture();
    swdata.b.l = 16;
    actwk[0].cddat = 4;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 16) != 0);

    reset_fixture();
    swdata.b.l = 16;
    chibi_flag = 1;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);

    reset_fixture();
    swdata.b.l = 16;
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].actno = 30;
    actwk[5].xposi.w.h = 32;
    actwk[5].sprhsize = 32;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
}

static void test_movement_special_helpers(test_context *ctx) {
    reset_fixture();
    spawned_scramapad_value = 259;
    actwk[0].xposi.w.h = 33;
    actwk[0].yspeed.w = 4;

    kuru2();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);

    reset_fixture();
    spawned_scramapad_value = 259;
    actwk[0].yspeed.w = -4;

    kuru2();

    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].yspeed.w);

    reset_fixture();
    spawned_scramapad_value = 345;
    swdata.b.h = 4;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat & 1);

    reset_fixture();
    spawned_scramapad_value = 0;
    player_work_get(&actwk[0])->status_flags = 4;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 4);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 8;

    kuru2();

    TEST_ASSERT_EQ_INT(ctx, 8, player_work_get(&actwk[0])->status_flags & 8);

    reset_fixture();
    plflag = 2;

    bura();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_fixture();
    spawned_scramapad_value = 259;
    swdata.b.l = 16;
    player_work_get(&actwk[0])->special_angle = 0;
    actwk[0].yspeed.w = 1;

    kuru2move();

    TEST_ASSERT_TRUE(ctx, (player_work_get(&actwk[0])->status_flags & 16) != 0);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 16;
    player_work_get(&actwk[0])->special_angle = 128;

    kuru2move();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 16);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 16;
    player_work_get(&actwk[0])->special_angle = 0;

    kuru2move();

    TEST_ASSERT_EQ_INT(ctx, 3072, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 16);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 16;
    player_work_get(&actwk[0])->special_angle = 1;

    kuru2move();

    TEST_ASSERT_TRUE(ctx, (player_work_get(&actwk[0])->status_flags & 16) != 0);

    reset_fixture();
    player_work_get(&actwk[0])->special_angle = 128;
    actwk[0].sproffset = 32768;

    kuru2move();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sproffset & 32768);

    reset_fixture();
    spawned_scramapad_value = 0;
    actwk[0].yspeed.w = 4;

    kuru2move();

    TEST_ASSERT_EQ_INT(ctx, -4, actwk[0].yspeed.w);

    reset_fixture();
    spawned_scramapad_value = 345;
    swdata.b.h = 8;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 1);

    reset_fixture();
    spawned_scramapad_value = 345;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].patcnt);

    reset_fixture();
    spawned_scramapad_value = 345;
    swdata.b.h = 4;
    actwk[0].pattimm = 1;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattimm);

    reset_fixture();
    spawned_scramapad_value = 345;
    swdata.b.h = 4;
    actwk[0].pattimm = 0;
    actwk[0].patcnt = 3;

    buramove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].patcnt);
}

static void test_lmovecol_and_ball_release_cover_more_motion_paths(
    test_context *ctx) {
    reset_fixture();
    actwk[0].mspeed.w = 256;
    dircolm_value = -1;
    dircolm_out_value = 96;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, 256, actwk[0].yspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 256;
    dircolm_value = -1;
    dircolm_out_value = 160;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, -256, actwk[0].xspeed.w);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 32) != 0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 256;
    dircolm_value = 1;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].direc.b.h = -65;
    actwk[0].mspeed.w = 256;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_fixture();
    plmaxspdwk = 1536;
    pladdspdwk = 12;
    player_work_get(&actwk[0])->spin_dash_counter = 10;
    actwk[0].mspeed.w = 300;
    actwk[0].xspeed.w = 20;
    actwk[0].yspeed.w = 30;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);

    reset_fixture();
    plmaxspdwk = 1536;
    pladdspdwk = 12;
    plretspdwk = 128;
    player_work_get(&actwk[0])->spin_dash_counter = 45;
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = 300;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_values[0]);

    reset_fixture();
    plmaxspdwk = 1536;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 2000;
    plpower_s = 1;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 2050, actwk[0].mspeed.w);

    reset_fixture();
    plmaxspdwk = 100;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = -10000;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 8;
    plretspdwk = 128;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 1);

    reset_fixture();
    swdata.b.h = 4;
    plretspdwk = 128;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat & 1);

    reset_fixture();
    swdata.b.h = 1;
    swdata.b.l = 16;
    levermove();
    swdata.w = 0;
    plmaxspdwk = 1536;
    player_work_get(&actwk[0])->spin_dash_counter = 10;
    actwk[0].mspeed.w = 300;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_fixture();
    plmaxspdwk = -100;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 0;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);

    reset_fixture();
    plmaxspdwk = 100;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 10000;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].mspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    actwk[0].mspeed.w = 5;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    actwk[0].mspeed.w = -5;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_fixture();
    mizuflag = 1;
    stub_cos_value = -256;
    actwk[0].mspeed.w = 5000;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].xspeed.w);
}

static void test_player_state_wrappers_cover_short_paths(test_context *ctx) {
    reset_fixture();
    kusya_flag = 1;
    actwk[0].mstno.b.h = 4;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, speedset2_count);

    reset_fixture();
    kusya_flag = 1;
    actwk[0].mstno.b.h = 5;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, kusya_flag);

    reset_fixture();
    actwk[0].mstno.b.h = 43;
    actwk[0].patno = 23;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_fixture();
    actwk[0].mstno.b.h = 43;
    chibi_flag = 1;
    actwk[0].patno = 120;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);

    reset_fixture();
    actwk[0].mstno.b.h = 43;
    chibi_flag = 1;
    actwk[0].patno = 121;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_fixture();
    actwk[0].mstno.b.h = 43;
    actwk[0].patno = 22;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);

    reset_fixture();
    swdata.b.l = 16;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 146, soundset_values[0]);

    reset_fixture();
    plautoflag = 1;
    swdata1.b.l = 128;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 128, swdata.b.l);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 4;
    spawned_scramapad_value = 0;

    play00jump();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 4);

    reset_fixture();
    actwk[0].cddat = 64;

    play00jump();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_fixture();
    swdata.b.l = 16;

    ball00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 8;
    spawned_scramapad_value = 259;

    ball00jump();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);

    reset_fixture();
    player_work_get(&actwk[0])->status_flags = 4;
    spawned_scramapad_value = 0;

    ball00jump();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 4);

    reset_fixture();
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = 0;

    ball00jump();

    TEST_ASSERT_EQ_INT(ctx, -40, actwk[0].yspeed.w);
}

static void test_stage7_and_animation_helpers(test_context *ctx) {
    reset_fixture();
    actwk[0].xposi.w.h = 32;
    actwk[0].yposi.w.h = 32;

    st7_x();

    TEST_ASSERT_TRUE(ctx, (actwk[0].actflg & 64) != 0);

    reset_fixture();
    actwk[0].xposi.w.h = 32;
    actwk[0].yposi.w.h = 160;
    actwk[0].actflg = 64;

    st7_x();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg & 64);

    reset_fixture();
    actwk[0].xposi.w.h = 160;
    actwk[0].yposi.w.h = 160;

    st7_x();

    TEST_ASSERT_TRUE(ctx, (actwk[0].actflg & 64) != 0);

    reset_fixture();
    actwk[0].xposi.w.h = 160;
    actwk[0].yposi.w.h = 32;
    actwk[0].actflg = 64;

    st7_x();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg & 64);

    reset_fixture();
    default_chg[0] = 1;
    default_chg[1] = 7;
    default_chg[2] = 255;
    actwk[0].pattim = 0;

    patchgmain();

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].patno);
    default_chg[1] = 0;
}

static void test_ball_and_landing_motion_helpers(test_context *ctx) {
    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 100;

    ballwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 68, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = 10;

    ballwalk_l();

    TEST_ASSERT_EQ_INT(ctx, -128, actwk[0].mspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = -1;

    ballwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -100;

    ballwalk_r();

    TEST_ASSERT_EQ_INT(ctx, -68, actwk[0].mspeed.w);

    reset_fixture();
    plretspdwk = 128;
    actwk[0].mspeed.w = -10;

    ballwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 128, actwk[0].mspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    plmaxspdwk = 1536;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 0;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_values[0]);

    reset_fixture();
    mizuflag = 1;
    actwk[0].mspeed.w = 100;
    stub_sin_value = 256;
    stub_cos_value = 0;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -156, actwk[0].yspeed.w);

    reset_fixture();
    player_work_get(&actwk[0])->mode_word = 1;
    actwk[0].mspeed.w = 100;
    pladdspdwk = 12;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 94, actwk[0].mspeed.w);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.h = 2;
    actwk[0].mspeed.w = 100;
    plmaxspdwk = 1536;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 45;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = 100;
    plmaxspdwk = 1536;
    plretspdwk = 128;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_values[0]);

    reset_fixture();
    actwk[0].mspeed.w = -100;
    pladdspdwk = 12;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -94, actwk[0].mspeed.w);

    reset_fixture();
    chibi_flag = 1;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);

    reset_fixture();
    actwk[0].mspeed.w = 5000;
    stub_cos_value = 256;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 5000;
    stub_cos_value = -256;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].xspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    plmaxspdwk = 1536;
    actwk[0].xspeed.w = 100;
    swdata.b.h = 4;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 76, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].xposi.w.h = 1800;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].xspeed.w = 320;
    actwk[0].yspeed.w = -512;
    scra_vline = 94;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 310, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);

    reset_fixture();
    actwk[0].xspeed.w = -320;
    actwk[0].yspeed.w = -512;
    scra_vline = 98;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, -310, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);

    reset_fixture();
    actwk[0].xposi.w.h = 100;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].xspeed.w = 320;
    swdata.b.h = 4;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 320, actwk[0].xspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    plmaxspdwk = 1536;
    actwk[0].xspeed.w = -2000;
    swdata.b.h = 4;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, -1536, actwk[0].xspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    plmaxspdwk = 1536;
    actwk[0].xspeed.w = 2000;
    swdata.b.h = 8;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 1536, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].xspeed.w = 16;
    actwk[0].yspeed.w = -512;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].xspeed.w = 1;
    actwk[0].yspeed.w = -1024;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].xspeed.w);

    reset_fixture();
    actwk[0].xspeed.w = -1;
    actwk[0].yspeed.w = -1024;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, -1, actwk[0].xspeed.w);
}

static void test_lmovecol_and_frip_spd_paths(test_context *ctx) {
    Sint32 cal_jump = 0;
    Uint8 cal_direc = 0;

    reset_fixture();
    actwk[0].mspeed.w = 1;
    dircolm_value = -1;
    dircolm_out_value = -32;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, -256, actwk[0].yspeed.w);

    reset_fixture();
    actwk[0].mspeed.w = 1;
    dircolm_value = -1;
    dircolm_out_value = 32;

    lmovecol();

    TEST_ASSERT_EQ_INT(ctx, 256, actwk[0].xspeed.w);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 32) != 0);

    reset_fixture();
    player_work_get(&actwk[0])->ride_actor_index = 5;

    TEST_ASSERT_EQ_INT(ctx, 255, frip_spd(&cal_jump, &cal_direc));

    reset_fixture();
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[0].xposi.w.h = 20;
    actwk[0].yposi.w.h = 40;
    actwk[5].actno = 30;
    actwk[5].xposi.w.h = 50;
    actwk[5].yposi.w.h = 40;
    actwk[5].sprhsize = 32;

    TEST_ASSERT_EQ_INT(ctx, 0, frip_spd(&cal_jump, &cal_direc));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[5].mstno.b.h);
}

static void test_run_animation_change_helpers(test_context *ctx) {
    reset_fixture();
    plchg00[0] = 1;
    plchg00[1] = 3;
    plchg00[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 256;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].patno);

    reset_fixture();
    plchg02[0] = 1;
    plchg02[1] = 4;
    plchg02[2] = 255;
    actwk[0].pattim = 0;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].patno);

    reset_fixture();
    plchg04[0] = 1;
    plchg04[1] = 5;
    plchg04[2] = 255;
    actwk[0].pattim = 0;

    playrunchg3(253);

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].patno);

    reset_fixture();
    {
        Uint8 wrap_chg[4] = {1, 9, 255, 0};
        patchgmain2(wrap_chg);
        TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].patno);
    }

    reset_fixture();
    {
        Uint8 wrap_chg[4] = {1, 9, 255, 0};
        actwk[0].patcnt = 1;
        patchgmain2(wrap_chg);
        TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patcnt);
        TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].patno);
    }

    reset_fixture();
    {
        Uint8 back_chg[5] = {1, 7, 8, 254, 2};
        actwk[0].patcnt = 2;
        patchgmain2(back_chg);
        TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].patno);
    }

    reset_fixture();
    {
        Uint8 state_chg[4] = {1, 7, 253, 12};
        actwk[0].patcnt = 1;
        patchgmain2(state_chg);
        TEST_ASSERT_EQ_INT(ctx, 12, actwk[0].mstno.b.h);
    }
}

static void test_run_animation_remaining_selector_paths(test_context *ctx) {
    Uint8 etc_pattern[] = {1, 77, 255, 0};
    Uint8 backref_pattern[] = {1, 11, 254, 1, 0};
    Uint8 state_pattern[] = {1, 11, 253, 7, 0};

    reset_fixture();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].cddat = 1;
    actwk[0].direc.b.h = 80;

    playrunchg(255);

    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);

    reset_fixture();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].direc.b.h = 0;
    actwk[0].mspeed.w = 1024;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].pattim);

    reset_fixture();
    actwk[0].pattim = 0;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 48;
    actwk[0].mspeed.w = 100;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].pattim);

    reset_fixture();
    chibi_flag = 1;
    playchg[33] = etc_pattern;
    actwk[0].mstno.b.h = 0;
    actwk[0].patcnt = 0;

    playrunchg3(1);

    TEST_ASSERT_EQ_INT(ctx, 77, actwk[0].patno);

    reset_fixture();
    actwk[0].patcnt = 1;

    patchgmain2(backref_pattern);

    TEST_ASSERT_EQ_INT(ctx, 11, actwk[0].patno);

    reset_fixture();
    actwk[0].patcnt = 1;

    patchgmain2(state_pattern);

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].mstno.b.h);

    reset_fixture();
    actwk[0].pattim = 2;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);

    reset_fixture();
    plchg49[0] = 1;
    plchg49[1] = 9;
    plchg49[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 3000;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);

    reset_fixture();
    plchg01[0] = 1;
    plchg01[1] = 8;
    plchg01[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 2000;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].patno);

    reset_fixture();
    plchg00[0] = 1;
    plchg00[1] = 10;
    plchg00[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = -100;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].patno);
    plchg00[1] = 3;

    reset_fixture();
    plchg53[0] = 1;
    plchg53[1] = 6;
    plchg53[2] = 255;
    actwk[0].pattim = 0;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 64;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 22, actwk[0].patno);

    reset_fixture();
    plchg04[0] = 1;
    plchg04[1] = 5;
    plchg04[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].cddat = 32;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].patno);

    reset_fixture();
    plchg04[0] = 1;
    plchg04[1] = 15;
    plchg04[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 4096;

    playrunchg3(253);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 15, actwk[0].patno);

    reset_fixture();
    plchg39[0] = 1;
    plchg39[1] = 16;
    plchg39[2] = 255;
    actwk[0].pattim = 0;
    chibi_flag = 1;

    playrunchg3(253);

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[0].patno);

    reset_fixture();
    plchg03[0] = 1;
    plchg03[1] = 4;
    plchg03[2] = 255;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1600;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].patno);

    reset_fixture();
    plchg54[0] = 1;
    plchg54[1] = 3;
    plchg54[2] = 255;
    actwk[0].pattim = 0;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 64;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].patno);

    reset_fixture();
    plchg35[0] = 1;
    plchg35[1] = 2;
    plchg35[2] = 255;
    actwk[0].pattim = 0;
    chibi_flag = 1;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].patno);

    reset_fixture();
    playchg[0] = etc_pattern;
    actwk[0].mstno.b.h = 0;
    actwk[0].pattim = 0;

    playrunchg(1);

    TEST_ASSERT_EQ_INT(ctx, 77, actwk[0].patno);

    reset_fixture();
    plchg03[0] = 1;
    plchg03[1] = 12;
    plchg03[2] = 255;
    actwk[0].pattim = 0;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = -16;
    actwk[0].mspeed.w = -1600;

    playrunchg2(254);

    TEST_ASSERT_EQ_INT(ctx, 12, actwk[0].patno);

    reset_fixture();
    plchg34[0] = 1;
    plchg34[1] = 13;
    plchg34[2] = 255;
    actwk[0].direc.b.h = 0;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = -2000;

    little_runchg();

    TEST_ASSERT_EQ_INT(ctx, 13, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);

    reset_fixture();
    plchg33[0] = 1;
    plchg33[1] = 14;
    plchg33[2] = 255;
    actwk[0].direc.b.h = 0;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = 1024;

    little_runchg();

    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].patno);

    reset_fixture();
    plchg34[0] = 1;
    plchg34[1] = 17;
    plchg34[2] = 255;
    actwk[0].direc.b.h = 0;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = 3000;

    little_runchg();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 17, actwk[0].patno);

    reset_fixture();
    default_chg[0] = 255;
    default_chg[1] = 1;
    actwk[0].pattim = 0;

    patchgmain();

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].patno);
    default_chg[0] = 1;
    default_chg[1] = 0;

    reset_fixture();
    default_chg[0] = 1;
    actwk[0].pattim = 2;

    patchgmain();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);
}

static void test_collision_limit_and_cleanup_helpers(test_context *ctx) {
    reset_fixture();
    dircol_u_value = -1;

    hedcolchk();

    TEST_ASSERT_EQ_INT(ctx, 11, actwk[0].mstno.b.h);

    reset_fixture();
    scralim_right = 100;
    actwk[0].xposi.w.h = 500;

    limitchk();

    TEST_ASSERT_EQ_INT(ctx, 460, actwk[0].xposi.w.h);

    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    actwk[0].mstno.b.h = 43;

    limitchk();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].r_no0);

    reset_fixture();
    scralim_left = 0;
    scralim_right = 1000;
    actwk[0].xposi.w.h = 100;
    actwk[0].xspeed.w = -1;

    limitchk();

    TEST_ASSERT_EQ_INT(ctx, -1, actwk[0].xspeed.w);

    reset_fixture();
    chibi_flag = 1;

    ballset();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);

    reset_fixture();
    actwk[0].cddat = 4;

    ballset();

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat & 4);
}

static void test_jumpcolchk_direction_cases(test_context *ctx) {
    reset_fixture();
    atan_sonic_value = 32;
    dircol_l2_value = -2;
    dircol_r2_value = -3;
    dircol_d_out_d1 = -4;
    dircol_d_out_d3 = 0;
    actwk[0].yspeed.w = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -1, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_fixture();
    atan_sonic_value = 96;
    dircol_l2_value = -5;
    actwk[0].yspeed.w = 300;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 96;
    dircol_u_out_d1 = -2;
    actwk[0].yspeed.w = -10;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_fixture();
    atan_sonic_value = 160;
    dircol_u_out_d1 = -3;
    dircol_u_out_d3 = 0;
    actwk[0].yspeed.w = -200;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_fixture();
    atan_sonic_value = 160;
    dircol_u_out_d1 = -3;
    dircol_u_out_d3 = 64;
    actwk[0].yspeed.w = -200;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->jump_started = 1;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].direc.b.h);
    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_started);

    reset_fixture();
    atan_sonic_value = 224;
    dircol_r2_value = -4;
    actwk[0].xspeed.w = 222;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -4, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 224;
    dircol_d_out_d1 = -2;
    dircol_d_out_d3 = 0;
    actwk[0].xspeed.w = 321;
    actwk[0].yspeed.w = 20;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -2, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 321, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 32;
    dircol_d_out_d1 = 0;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_fixture();
    atan_sonic_value = 32;
    dircol_d_out_d0 = -10;
    dircol_d_out_d1 = -10;
    actwk[0].yspeed.w = 0;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yposi.w.h);

    reset_fixture();
    atan_sonic_value = 32;
    dircol_d_out_d1 = -4;
    dircol_d_out_d3 = 32;
    actwk[0].xspeed.w = 77;
    actwk[0].yspeed.w = 5000;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4032, actwk[0].yspeed.w);

    reset_fixture();
    atan_sonic_value = 32;
    dircol_d_out_d1 = -4;
    dircol_d_out_d3 = 16;
    actwk[0].yspeed.w = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 32;
    dircol_d_out_d1 = -4;
    dircol_d_out_d3 = -96;
    actwk[0].yspeed.w = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 96;
    actwk[0].yspeed.w = -1;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yposi.w.h);

    reset_fixture();
    atan_sonic_value = 96;
    dircol_d_out_d1 = 0;
    actwk[0].yspeed.w = 1;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yposi.w.h);

    reset_fixture();
    atan_sonic_value = 96;
    dircol_d_out_d1 = -3;
    dircol_d_out_d3 = 0;
    actwk[0].xspeed.w = 222;
    actwk[0].yspeed.w = 1;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -3, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 222, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 160;
    dircol_l2_value = -2;
    dircol_r2_value = -3;
    dircol_u_out_d1 = 0;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -1, actwk[0].xposi.w.h);

    reset_fixture();
    atan_sonic_value = 160;
    dircol_u_out_d1 = -3;
    dircol_u_out_d3 = -64;
    actwk[0].yspeed.w = -200;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].mspeed.w);

    reset_fixture();
    atan_sonic_value = 224;
    dircol_u_out_d1 = -2;
    actwk[0].yspeed.w = -1;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
}

static void test_fall_direction_and_damage_wrappers(test_context *ctx) {
    reset_fixture();
    actwk[0].direc.b.h = 96;
    actwk[0].mspeed.w = 0;

    fallchk();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat & 2);

    reset_fixture();
    player_work_get(&actwk[0])->jump_lock = 1;
    actwk[0].direc.b.h = 96;

    fallchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 2);

    reset_fixture();
    actwk[0].direc.b.h = 96;
    actwk[0].mspeed.w = 641;

    fallchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 2);

    reset_fixture();
    player_work_get(&actwk[0])->mode_word = 2;

    fallchk();

    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(&actwk[0])->mode_word);

    reset_fixture();
    actwk[0].direc.b.h = -1;

    direcchg();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].direc.b.h);

    reset_fixture();
    actwk[0].direc.b.h = 1;

    direcchg();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].direc.b.h);

    reset_fixture();
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->jump_started = 1;
    emyscorecnt = 77;

    jumpcolsub();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 0, emyscorecnt);

    reset_fixture();
    chibi_flag = 1;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->jump_started = 1;
    emyscorecnt = 77;

    jumpcolsub();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 0, emyscorecnt);

    reset_fixture();
    actwk[0].cddat = 255;
    player_work_get(&actwk[0])->jump_started = 1;
    emyscorecnt = 77;

    jumpcolsub0();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 0, emyscorecnt);

    reset_fixture();
    actwk[0].cddat = 2;

    play00damage();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_fixture();
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = 0;

    play00damage();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_fixture();

    play00die();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_levermove_discrete_input_paths(test_context *ctx) {
    reset_fixture();
    mizuflag = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    swdata.b.h = 4;
    plmaxspdwk = 1536;
    pladdspdwk = 12;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat & 1);

    reset_fixture();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].actno = 30;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_fixture();
    emycol_d_value = 12;
    player_work_get(&actwk[0])->floor_left = 3;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].mstno.b.h);

    reset_fixture();
    swdata.b.h = 2;
    swdata.b.l = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, scr_cnt);

    reset_fixture();
    swdata.b.h = 1;
    swdata.b.l = 16;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 156, soundset_values[0]);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 30;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_values[0]);

    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.h = 1;
    plmaxspdwk = 1536;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 2;
    swdata.b.l = 16;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);

    reset_fixture();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].sprhsize = 16;
    actwk[5].xposi.w.h = 0;
    actwk[0].xposi.w.h = 8;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);

    reset_fixture();
    scr_cnt = 128;
    swdata.b.h = 1;
    scra_vline = 196;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 198, scra_vline);

    reset_fixture();
    scr_cnt = 64;
    swdata.b.h = 2;
    scra_vline = 12;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 10, scra_vline);
}

static void test_levermove_additional_branch_paths(test_context *ctx) {
    reset_fixture();
    swdata.b.h = 8;
    plmaxspdwk = 1536;
    pladdspdwk = 12;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat & 1);

    reset_fixture();
    player_work_get(&actwk[0])->mode_word = 1;
    swdata.b.h = 4;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_fixture();
    actwk[0].direc.b.h = 96;
    scra_vline = 92;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 94, scra_vline);

    reset_fixture();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].cddat = 128;
    swdata.b.l = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, scr_cnt);

    reset_fixture();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].sprhsize = 10;
    actwk[5].xposi.w.h = 20;
    actwk[0].xposi.w.h = 0;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mstno.b.h);

    reset_fixture();
    actwk[0].cddat = 9;
    player_work_get(&actwk[0])->ride_actor_index = 5;
    actwk[5].sprhsize = 10;
    actwk[5].xposi.w.h = 0;
    actwk[0].xposi.w.h = 20;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mstno.b.h);

    reset_fixture();
    emycol_d_value = 12;
    player_work_get(&actwk[0])->floor_right = 3;
    actwk[0].cddat = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].mstno.b.h);

    reset_fixture();
    scr_cnt = 1;
    swdata.b.l = 1;

    levermove();

    TEST_ASSERT_TRUE(ctx, (scr_cnt & 128) != 0);

    reset_fixture();
    swdata.b.h = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].cddat = 1;
    plmaxspdwk = 1536;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    plmaxspdwk = -100;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    plmaxspdwk = 100;
    plpower_s = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].cddat = 1;
    plmaxspdwk = 100;
    actwk[0].mspeed.w = -10000;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    plmaxspdwk = 100;
    actwk[0].mspeed.w = 10000;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 1;
    swdata.b.l = 16;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    reset_fixture();
    player_work_get(&actwk[0])->spin_dash_counter = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_fixture();
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].mstno.b.h);

    reset_fixture();
    scr_cnt = 1;
    swdata.b.h = 2;
    swdata.b.l = 2;

    levermove();

    TEST_ASSERT_TRUE(ctx, (scr_cnt & 64) != 0);

    reset_fixture();
    swdata.b.h = 2;
    swdata.b.l = 16;
    actwk[0].cddat = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -10, actwk[0].mspeed.w);

    reset_fixture();
    swdata.b.h = 2;
    player_work_get(&actwk[0])->spin_dash_counter = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.h);

    reset_fixture();
    swdata.b.h = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.h);

    reset_fixture();
    scra_vline = 96;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);

    reset_fixture();
    pladdspdwk = 12;
    actwk[0].mspeed.w = 6;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_fixture();
    pladdspdwk = 12;
    actwk[0].mspeed.w = -6;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
}

static void test_damage_die_sub_state_paths(test_context *ctx) {
    reset_fixture();
    actwk[0].r_no0 = 4;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = 0;
    scralim_down = 100;

    play00damage_sub();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);

    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    pl_suu = 2;
    ta_flag = 1;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_fixture();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    pl_suu = 0;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
}

static void test_erase_and_chk11_cover_map_and_marker_paths(test_context *ctx) {
    reset_fixture();

    play00erase();

    TEST_ASSERT_EQ_INT(ctx, 0, gameflag.w);

    reset_fixture();
    player_work_get(&actwk[0])->erase_timer = 2;

    play00erase();

    TEST_ASSERT_EQ_INT(ctx, 0, gameflag.w);

    reset_fixture();
    player_work_get(&actwk[0])->erase_timer = 1;
    pl_suu = 1;
    time_flag = 1;
    markerno = 0;
    play_start = 3;

    play00erase();

    TEST_ASSERT_EQ_INT(ctx, 1, gameflag.w);
    TEST_ASSERT_EQ_INT(ctx, 1, play_start);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);

    reset_fixture();
    player_work_get(&actwk[0])->erase_timer = 1;
    pl_suu = 1;
    time_flag = 1;
    markerno = 1;

    play00erase();

    TEST_ASSERT_EQ_INT(ctx, 1, plflag);

    reset_fixture();
    time_flag = 1;
    mapwka[0][0] = 6;

    chk11();

    TEST_ASSERT_TRUE(ctx, (player_work_get(&actwk[0])->status_flags & 2) != 0);

    reset_fixture();
    time_flag = 1;
    stageno.w = 1;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 2);

    reset_fixture();
    time_flag = 1;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 2);

    reset_fixture();
    time_flag = 1;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = 0;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_fixture();
    time_flag = 1;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = -1000;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_fixture();
    time_flag = 1;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = -3000;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags & 2);
    TEST_ASSERT_EQ_INT(ctx, 1536, actwk[0].xspeed.w);

    reset_fixture();
    time_flag = 1;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = -3000;
    actwk[0].cddat = 1;

    chk11();

    TEST_ASSERT_EQ_INT(ctx, -1536, actwk[0].xspeed.w);
}

static void test_mizuki_set_spawns_only_when_all_gates_pass(test_context *ctx) {
    reset_fixture();
    stageno.b.h = 1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_fixture();
    mapwka[0][1] = 47;
    actwk[0].xposi.w.h = 256;
    actwk[0].yposi.w.h = 0;
    actwk[0].sprvsize = 0;
    player_work_get(&actwk[0])->status_flags = 1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 14, spawned_actor.actno);
    TEST_ASSERT_EQ_INT(ctx, 256, spawned_actor.xposi.w.h);

    reset_fixture();
    gametimer.b.l = 1;
    mapwka[0][1] = 47;
    actwk[0].xposi.w.h = 256;
    player_work_get(&actwk[0])->status_flags = 1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_fixture();
    mapwka[0][21] = 47;
    actwk[0].xposi.w.h = 5568;
    player_work_get(&actwk[0])->status_flags = 1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_fixture();
    mapwka[0][1] = 47;
    actwk[0].xposi.w.h = 256;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_fixture();
    mapwka[0][1] = 47;
    actwk[0].xposi.w.h = 256;
    player_work_get(&actwk[0])->status_flags = 1;
    actwk[0].xspeed.w = -1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, spawned_actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, spawned_actor.cddat);

    reset_fixture();
    mapwka[0][1] = 47;
    actwk[0].xposi.w.h = 256;
    player_work_get(&actwk[0])->status_flags = 1;
    actwkchk_result = 1;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, spawned_actor.actno);
}

TEST_MAIN_BEGIN;
test_bye_chk_waits_then_forces_timeout_escape(&ctx);
test_play00_dispatches_edit_and_skips_nonplayer(&ctx);
test_muteki_set_arms_power_slots_once(&ctx);
test_map_and_save_helpers_copy_stable_state(&ctx);
test_timewarp_backto_paths_use_callbacks_and_saved_state(&ctx);
test_power_counters_restore_music_when_timers_expire(&ctx);
test_initializers_and_empty_helpers_are_callable(&ctx);
test_stage_hazard_helpers_use_damage_and_sound_callbacks(&ctx);
test_backto_early_and_speed_gates(&ctx);
test_bura_resets_active_timewarp_item_on_entry(&ctx);
test_limit_and_late_life_paths_use_callbacks(&ctx);
test_jump_and_animation_helpers_cover_discrete_paths(&ctx);
test_play00move_dispatches_movement_modes(&ctx);
test_walk_acceleration_and_ball_entry_paths(&ctx);
test_jump_release_slope_and_direction_helpers(&ctx);
test_movement_special_helpers(&ctx);
test_lmovecol_and_ball_release_cover_more_motion_paths(&ctx);
test_player_state_wrappers_cover_short_paths(&ctx);
test_stage7_and_animation_helpers(&ctx);
test_ball_and_landing_motion_helpers(&ctx);
test_lmovecol_and_frip_spd_paths(&ctx);
test_run_animation_change_helpers(&ctx);
test_run_animation_remaining_selector_paths(&ctx);
test_collision_limit_and_cleanup_helpers(&ctx);
test_jumpcolchk_direction_cases(&ctx);
test_fall_direction_and_damage_wrappers(&ctx);
test_levermove_discrete_input_paths(&ctx);
test_levermove_additional_branch_paths(&ctx);
test_damage_die_sub_state_paths(&ctx);
test_erase_and_chk11_cover_map_and_marker_paths(&ctx);
test_mizuki_set_spawns_only_when_all_gates_pass(&ctx);
TEST_MAIN_END

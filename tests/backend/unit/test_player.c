#include <string.h>

#include "support/test_runner.h"
#include "src/equ.h"
#include "src/player.h"
#include "src/player_work.h"
#include "src/types.h"

Uint8 mapwka[8][64];
sprite_pattern *sncpat[1];

static game_info keep_work;
static int sub_sync_count;
static Sint16 sub_sync_requests[16];
static int plairset_count;
static int soundset_count;
static Sint16 soundset_request;
static int da_set_count;
static int playdieset_count;
static int speedset_count;
static int speedset2_count;
static int playsp_count;
static int pcol_count;
static int fcol_count;
static int actwkchk_count;
static int actwkchk_fail;
static sprite_status *actwkchk_actor;
static int flagwkclr_count;
static int fadeout_count;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int edit_count;
static sprite_status *edit_actor;
static int wave_all_stop_count;
static int dircolm_sets_direction;
static char dircolm_direction_out;
static Sint16 dircolm_result;
static Sint16 dircol2_result;
static Sint16 dircol_u_result;
static Sint16 dircol_l2_result;
static Sint16 dircol_r2_result;
static Sint16 dircol_d_d0;
static Sint16 dircol_d_d1;
static Sint16 dircol_d_d3;
static Sint16 dircol_u_d0;
static Sint16 dircol_u_d1;
static Sint16 dircol_u_d3;
static Sint16 emycol_d_result;
static Sint16 stub_sin_value;
static Sint16 stub_cos_value;
static Sint16 stub_atan_value;

void sub_sync(Sint16 ReqNo);
void playsp(void);
Sint16 pcol(sprite_status *pActwk);
void da_set(void);
void plairset(void);
void soundset(Sint16 ReqNo);
Sint16 playdieset(sprite_status *pActwk);
void speedset(sprite_status *pActwk);
void speedset2(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void flagwkclr(void);
void fadeout_s(void);
void edit(sprite_status *pActwk);
Sint16 fcol(sprite_status *pActwk);
Sint16 dircol(sprite_status *pActwk, char *cpDirec);
Sint16 dircolm(sprite_status *pActwk, char *cpDirec);
Sint16 dircol2(sprite_status *pActwk, char *cpDirec);
Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_l(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_l2(sprite_status *pActwk);
Sint16 dircol_r(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_r2(sprite_status *pActwk);
Sint16 dircol_u(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 emycol_d(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 atan_sonic(Sint16 x, Sint16 y);
void test_wave_all_stop(void);

void (*WaveAllStop)(void) = test_wave_all_stop;

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 16) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void playsp(void) { ++playsp_count; }
Sint16 pcol(sprite_status *pActwk) {
    (void)pActwk;
    ++pcol_count;
    return 0;
}
void da_set(void) { ++da_set_count; }
void plairset(void) { ++plairset_count; }
void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_request = ReqNo;
}
Sint16 playdieset(sprite_status *pActwk) {
    (void)pActwk;
    ++playdieset_count;
    return 0;
}
void speedset(sprite_status *pActwk) {
    (void)pActwk;
    ++speedset_count;
}
void speedset2(sprite_status *pActwk) {
    (void)pActwk;
    ++speedset2_count;
}
void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}
void frameout(sprite_status *pActwk) { (void)pActwk; }
Sint32 frameout_s(sprite_status *pActwk) {
    (void)pActwk;
    return 0;
}
void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    (void)patchgwk;
    (void)pat_dat;
}
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pActwk;
    (void)pPlayerwk;
    return 0;
}
Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_actor;
    return 0;
}
void flagwkclr(void) { ++flagwkclr_count; }
void fadeout_s(void) { ++fadeout_count; }
void edit(sprite_status *pActwk) {
    ++edit_count;
    edit_actor = pActwk;
}
Sint16 fcol(sprite_status *pActwk) {
    (void)pActwk;
    ++fcol_count;
    return 0;
}
Sint16 dircol(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    (void)cpDirec;
    return 0;
}
Sint16 dircolm(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    if (dircolm_sets_direction) {
        *cpDirec = dircolm_direction_out;
    }
    return dircolm_result;
}
Sint16 dircol2(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    (void)cpDirec;
    return dircol2_result;
}
Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    *ipD0 = dircol_d_d0;
    *ipD1 = dircol_d_d1;
    *ipD3 = dircol_d_d3;
    return 0;
}
Sint16 dircol_l(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    (void)ipD0;
    (void)ipD1;
    (void)ipD3;
    return 0;
}
Sint16 dircol_l2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_l2_result;
}
Sint16 dircol_r(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    (void)ipD0;
    (void)ipD1;
    (void)ipD3;
    return 0;
}
Sint16 dircol_r2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_r2_result;
}
Sint16 dircol_u(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    *ipD0 = dircol_u_d0;
    *ipD1 = dircol_u_d1;
    *ipD3 = dircol_u_d3;
    return dircol_u_result;
}
Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    (void)kakudo;
    *sin = stub_sin_value;
    *cos = stub_cos_value;
}
Sint16 atan_sonic(Sint16 x, Sint16 y) {
    (void)x;
    (void)y;
    return stub_atan_value;
}
void test_wave_all_stop(void) { ++wave_all_stop_count; }

static void reset_player_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(playposiwk, 0, sizeof(playposiwk));
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;
    byecnt0.w = 0;
    pl_suu = 0;
    pl_suu_f = 0;
    pltime_f = 0;
    ta_flag = 1;
    editmode.w = 0;
    chibi_flag = 0;
    plmaxspdwk = 0;
    pladdspdwk = 0;
    plretspdwk = 0;
    scra_hline = 160;
    backto_cnt = 0;
    plpower_a = 0;
    plpower_b = 0;
    plpower_m = 0;
    plpower_s = 0;
    boss_sound = 0;
    conbine_flag = 0;
    time_stop = 0;
    time_item = 0;
    stageno.w = 0;
    gametimer.w = 0;
    waterposi = 0;
    water_flag = 0;
    waterflag = 0;
    waterposi_m = 0;
    scralim_down = 0;
    plflag = 0;
    plring = 0;
    plring_f2 = 0;
    plring_s = 0;
    plring_f2_s = 0;
    pltime.l = 0;
    plposiwkadr.w = 0;
    gameflag.w = 0;
    scroll_start.w = 0;
    scra_vline = 96;
    scralim_left = 0;
    scralim_right = 0;
    bossstart = 0;
    flowercnt[0] = flowercnt[1] = flowercnt[2] = 0;
    tv_flag = 0;
    markerno = 0;
    time_flag = 0;
    play_start = 0;
    mizuflag = 0;
    scr_cnt = 0;
    swdata.w = 0;
    swdata1.w = 0;
    swdata2.w = 0;
    debugflag.w = 0;
    watercoliflag = 0;
    plautoflag = 0;
    loopmapno = 0;
    loopmapno2 = 0;
    ballmapno = 0;
    ballmapno2 = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    plairset_count = 0;
    soundset_count = 0;
    soundset_request = 0;
    da_set_count = 0;
    playdieset_count = 0;
    speedset_count = 0;
    speedset2_count = 0;
    playsp_count = 0;
    pcol_count = 0;
    fcol_count = 0;
    actwkchk_count = 0;
    actwkchk_fail = 0;
    actwkchk_actor = &actwk[20];
    flagwkclr_count = 0;
    fadeout_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    edit_count = 0;
    edit_actor = 0;
    wave_all_stop_count = 0;
    dircolm_sets_direction = 0;
    dircolm_direction_out = 0;
    dircolm_result = 0;
    dircol2_result = 0;
    dircol_u_result = 0;
    dircol_l2_result = 0;
    dircol_r2_result = 0;
    dircol_d_d0 = 0;
    dircol_d_d1 = 0;
    dircol_d_d3 = 0;
    dircol_u_d0 = 0;
    dircol_u_d1 = 0;
    dircol_u_d3 = 0;
    emycol_d_result = 0;
    stub_sin_value = 0;
    stub_cos_value = 256;
    stub_atan_value = 0;
}

static void set_player_work_word(int offset, Uint16 value) {
    player_work *work = player_work_get(&actwk[0]);

    switch (offset) {
    case 8:
        work->invincibility_timer = value;
        break;
    case 10:
        work->speed_shoes_timer = value;
        break;
    }
}

static void test_bye_chk_resets_counter_when_not_waiting(test_context *ctx) {
    reset_player_state();
    byecnt0.w = 400;
    actwk[0].mstno.b.h = 4;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);
}

static void test_bye_chk_starts_and_completes_goodbye_state(test_context *ctx) {
    reset_player_state();
    actwk[0].mstno.b.h = 5;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);

    byecnt0.w = 10800;
    actwk[0].cddat = 1;

    bye_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 43, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 32768, actwk[0].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, -1280, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 121, sub_sync_requests[0]);
}

static void test_play00_ignores_non_player_and_routes_edit_mode(
    test_context *ctx) {
    reset_player_state();

    play00(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, edit_count);

    ta_flag = 0;
    editmode.b.h = 1;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, edit_count);
    TEST_ASSERT_TRUE(ctx, edit_actor == &actwk[0]);
}

static void test_muteki_set_allocates_only_when_slot_is_free(test_context *ctx) {
    reset_player_state();

    muteki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[13].actno);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[12].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[15].mstno.b.h);

    reset_player_state();
    actwk[12].actno = 99;

    muteki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);
    TEST_ASSERT_EQ_INT(ctx, 99, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[13].actno);
}

static void test_play00init_sets_normal_and_chibi_dimensions(test_context *ctx) {
    reset_player_state();

    play00init();

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 1920, actwk[0].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[0].sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].actflg);
    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 12, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);

    reset_player_state();
    chibi_flag = 1;

    play00init();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
}

static void test_mapno_chk_reads_map_byte_with_base_value(test_context *ctx) {
    reset_player_state();
    mapwka[2][3] = 47;

    TEST_ASSERT_EQ_INT(ctx, 2162735, mapno_chk(512, 768));
}

static void test_mizuki_set_spawns_only_for_stage0_tile_47(test_context *ctx) {
    reset_player_state();
    stageno.b.h = 1;
    gametimer.b.l = 0;
    actwk[0].sprvsize = 10;
    actwk[0].xposi.w.h = 768;
    actwk[0].yposi.w.h = 502;
    actwk[0].xspeed.w = -20;
    player_work_get(&actwk[0])->status_flags = 1;
    mapwka[2][3] = 47;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    stageno.b.h = 0;
    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 768, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 502, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].cddat);

    reset_player_state();
    stageno.b.h = 0;
    gametimer.b.l = 0;
    actwk[0].sprvsize = 10;
    actwk[0].xposi.w.h = 768;
    actwk[0].yposi.w.h = 502;
    actwk[0].xspeed.w = 20;
    player_work_get(&actwk[0])->status_flags = 1;
    mapwka[2][3] = 47;

    mizuki_set();

    TEST_ASSERT_EQ_INT(ctx, 14, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].cddat);
}

static void test_mizuki_set_guards_odd_timer_position_and_allocation(
    test_context *ctx) {
    reset_player_state();
    stageno.b.h = 0;
    gametimer.b.l = 1;
    mapwka[2][3] = 47;
    actwk[0].sprvsize = 10;
    actwk[0].xposi.w.h = 768;
    actwk[0].yposi.w.h = 502;
    player_work_get(&actwk[0])->status_flags = 1;

    mizuki_set();
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    gametimer.b.l = 0;
    mapwka[2][3] = 46;
    mizuki_set();
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    mapwka[2][3] = 47;
    actwk[0].xposi.w.h = 5568;
    mapwka[2][21] = 47;
    mizuki_set();
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    actwk[0].xposi.w.h = 768;
    player_work_get(&actwk[0])->status_flags = 0;
    mizuki_set();
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    player_work_get(&actwk[0])->status_flags = 1;
    actwkchk_fail = 1;
    mizuki_set();
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);
}

static void test_scr_h_moves_toward_center_or_speed_edges(test_context *ctx) {
    reset_player_state();
    scra_hline = 170;
    actwk[0].mspeed.w = 100;

    scr_h();

    TEST_ASSERT_EQ_INT(ctx, 168, scra_hline);

    scra_hline = 150;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 152, scra_hline);

    scra_hline = 220;
    actwk[0].mspeed.w = -2000;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 222, scra_hline);

    scra_hline = 98;
    actwk[0].mspeed.w = 2000;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 96, scra_hline);

    scra_hline = 170;
    actwk[0].mspeed.w = 2000;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].xposi.w.h = 100;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 168, scra_hline);

    scra_hline = 224;
    player_work_get(&actwk[0])->status_flags = 0;
    actwk[0].mspeed.w = -2000;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 224, scra_hline);

    scra_hline = 95;
    actwk[0].mspeed.w = 2000;
    scr_h();
    TEST_ASSERT_EQ_INT(ctx, 96, scra_hline);
}

static void test_play00_and_play00move_dispatch_visible_states(test_context *ctx) {
    reset_player_state();

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[0].patbase == sncpat);

    reset_player_state();
    actwk[0].r_no0 = 2;
    scra_hline = 170;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 168, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, fcol_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);
    TEST_ASSERT_EQ_INT(ctx, 1, pcol_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 100, playposiwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 200, playposiwk[1]);

    reset_player_state();
    actwk[0].cddat = 2;
    actwk[0].yspeed.w = 10;
    play00move();
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);

    reset_player_state();
    actwk[0].cddat = 6;
    actwk[0].yspeed.w = 10;
    play00move();
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);

    reset_player_state();
    actwk[0].r_no0 = 8;
    play00(&actwk[0]);
    TEST_ASSERT_TRUE(ctx, actwk[0].patbase == sncpat);

    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 29;
    play00(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 30, player_work_get(&actwk[0])->spin_dash_counter);

    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 44;
    actwk[0].cddat = 4;
    play00(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 45, player_work_get(&actwk[0])->spin_dash_counter);

    reset_player_state();
    debugflag.w = 1;
    swdata1.b.l = 16;
    play00move();
    TEST_ASSERT_EQ_INT(ctx, 1, editmode.b.h);

    reset_player_state();
    actwk[0].r_no0 = 2;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);
    TEST_ASSERT_TRUE(ctx, actwk[0].patbase == sncpat);
}

static void test_play00move_auto_backto_and_water_state_paths(test_context *ctx) {
    reset_player_state();
    plautoflag = 1;
    swdata.b.l = 4;
    swdata1.b.l = 128;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 132, swdata.b.l);

    reset_player_state();
    player_work_get(&actwk[0])->status_flags = 1;
    stageno.b.h = 6;
    backto_cnt = 8;
    plpower_a = 1;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_player_state();
    watercoliflag = 1;
    player_work_get(&actwk[0])->status_flags = 1;
    stageno.b.h = 6;
    actwk[0].mstno.b.h = 0;
    actwk[0].mstno.b.l = 12;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 12, actwk[0].mstno.b.h);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 1;
    player_work_get(&actwk[0])->status_flags = 1;
    actwk[0].mspeed.w = 1536;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);

    reset_player_state();
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = 10;

    play00move();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);
}

static void test_playposiwkset_records_position_and_advances_ring(
    test_context *ctx) {
    reset_player_state();
    plposiwkadr.w = 4;
    actwk[0].xposi.w.h = 1234;
    actwk[0].yposi.w.h = -234;

    playposiwkset();

    TEST_ASSERT_EQ_INT(ctx, 1234, playposiwk[2]);
    TEST_ASSERT_EQ_INT(ctx, -234, playposiwk[3]);
    TEST_ASSERT_EQ_INT(ctx, 8, plposiwkadr.w);
}

static void test_plwaterchk_enters_water_and_spawns_splash(test_context *ctx) {
    reset_player_state();
    stageno.b.h = 2;
    waterposi = 100;
    actwk[0].yposi.w.h = 120;
    actwk[0].xposi.w.h = 300;
    actwk[0].xspeed.w = 1000;
    actwk[0].yspeed.w = 400;

    plwaterchk();

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, plairset_count);
    TEST_ASSERT_EQ_INT(ctx, 33, actwk[7].actno);
    TEST_ASSERT_EQ_INT(ctx, -127, actwk[7].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 768, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 6, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 64, plretspdwk);
    TEST_ASSERT_EQ_INT(ctx, 500, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[20].xposi.w.h);
}

static void test_plwaterchk_leaves_water_and_clamps_upward_speed(
    test_context *ctx) {
    reset_player_state();
    stageno.b.h = 2;
    waterposi = 200;
    actwk[0].yposi.w.h = 120;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = -3000;

    plwaterchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 12, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);
    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[20].actno);
}

static void test_plwaterchk_guards_stage_start_and_failed_splash_alloc(
    test_context *ctx) {
    reset_player_state();
    stageno.b.h = 2;
    stageno.b.l = 1;
    waterposi = 100;
    actwk[0].xposi.w.h = 199;
    actwk[0].yposi.w.h = 120;

    plwaterchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, plairset_count);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 100;
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 120;
    actwk[0].yspeed.w = 200;
    actwkchk_fail = 1;

    plwaterchk();

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 100;
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 120;
    actwk[0].yspeed.w = 0;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 200;
    actwk[0].yposi.w.h = 120;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = 10;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].cddat);

    reset_player_state();
    stageno.b.h = 1;
    waterposi = 100;
    actwk[0].yposi.w.h = 120;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 0, plairset_count);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 100;
    actwk[0].yposi.w.h = 120;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = 200;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, plairset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 200;
    actwk[0].yposi.w.h = 120;
    actwk[0].yspeed.w = -200;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_player_state();
    stageno.b.h = 2;
    waterposi = 200;
    actwk[0].yposi.w.h = 120;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = 0;
    plwaterchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_playsave_copies_visible_state_and_clamps_time(
    test_context *ctx) {
    reset_player_state();
    plflag = 7;
    actwk[0].xposi.w.h = 111;
    actwk[0].yposi.w.h = 222;
    actwk[0].mspeed.w = 333;
    actwk[0].xspeed.w = 444;
    actwk[0].yspeed.w = 555;
    actwk[0].cddat = 0xFF;
    water_flag = 2;
    scralim_down = 3333;
    scra_h_posit.w.h = 10;
    scra_v_posit.w.h = 20;
    scrb_h_posit.w.h = 30;
    scrb_v_posit.w.h = 40;
    scrc_h_posit.w.h = 50;
    scrc_v_posit.w.h = 60;
    scrz_h_posit.w.h = 70;
    scrz_v_posit.w.h = 80;
    waterposi_m = 90;
    waterflag = 3;
    plring = 44;
    plring_f2 = 5;
    pltime.l = 400000;
    chibi_flag = 1;

    playsave();

    TEST_ASSERT_EQ_INT(ctx, 7, keep_work.plflag_sb);
    TEST_ASSERT_EQ_INT(ctx, 111, keep_work.plxposi_sb);
    TEST_ASSERT_EQ_INT(ctx, 222, keep_work.plyposi_sb);
    TEST_ASSERT_EQ_INT(ctx, 333, keep_work.plmspd_sb);
    TEST_ASSERT_EQ_INT(ctx, 444, keep_work.plxspd_sb);
    TEST_ASSERT_EQ_INT(ctx, 555, keep_work.plyspd_sb);
    TEST_ASSERT_EQ_INT(ctx, 183, keep_work.cddat_sb);
    TEST_ASSERT_EQ_INT(ctx, 3333, keep_work.scralim_down_sb);
    TEST_ASSERT_EQ_INT(ctx, 10, keep_work.scra_h_posit_sb);
    TEST_ASSERT_EQ_INT(ctx, 80, keep_work.scrz_v_posit_sb);
    TEST_ASSERT_EQ_INT(ctx, 90, keep_work.waterposi_m_sb);
    TEST_ASSERT_EQ_INT(ctx, 3, keep_work.waterflag_sb);
    TEST_ASSERT_EQ_INT(ctx, 44, keep_work.plring_s);
    TEST_ASSERT_EQ_INT(ctx, 44, plring_s);
    TEST_ASSERT_EQ_INT(ctx, 5, keep_work.plring_f2_s);
    TEST_ASSERT_EQ_INT(ctx, 5, plring_f2_s);
    TEST_ASSERT_EQ_INT(ctx, 327680, keep_work.pltime_sb);
    TEST_ASSERT_EQ_INT(ctx, 1, keep_work.chibi_sb);

    pltime.l = 123456;
    playsave();
    TEST_ASSERT_EQ_INT(ctx, 123456, keep_work.pltime_sb);
}

static void test_backto_chk_saves_time_warp_and_fades_at_terminal_count(
    test_context *ctx) {
    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 157;
    time_item = 1;
    time_flag = 0;
    plflag = 0;
    plring = 7;
    actwk[0].xposi.w.h = 3000;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, scroll_start.b.h);
    TEST_ASSERT_EQ_INT(ctx, 129, time_flag);
    TEST_ASSERT_EQ_INT(ctx, 2, plflag);
    TEST_ASSERT_EQ_INT(ctx, 7, keep_work.plring_s);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_requests[0]);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 165;
    time_item = 1;
    actwk[0].xposi.w.h = 3000;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, gameflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, fadeout_count);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 165;
    time_item = -5;
    time_flag = 0;
    actwk[0].xposi.w.h = 3000;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 165;
    time_item = 5;
    time_flag = 0;
    actwk[0].xposi.w.h = 3000;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 130, time_flag);
}

static void test_backto_chk_resets_before_time_warp_threshold(test_context *ctx) {
    reset_player_state();
    keep_work.TimeWarp = 1;
    stageno.w = -255;
    time_flag = 0;
    time_item = 1;
    backto_cnt = 99;
    plpower_a = 1;
    actwk[0].xposi.w.h = 2000;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, time_item);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 1;
    backto_cnt = 90;
    plpower_a = 1;
    actwk[0].mspeed.w = 100;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, time_item);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 1;
    actwk[0].mspeed.w = 1536;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 1, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[12].actno);

    reset_player_state();
    keep_work.TimeWarp = 0;
    time_item = 1;
    backto_chk();
    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    backto_chk();
    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 0;
    backto_chk();
    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);

    reset_player_state();
    keep_work.TimeWarp = 1;
    time_item = 1;
    actwk[0].mspeed.w = -1536;
    backto_chk();
    TEST_ASSERT_EQ_INT(ctx, 1, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_a);
}

static void test_backto_chk_skips_repeat_save_when_flag_already_set(
    test_context *ctx) {
    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 157;
    time_item = 1;
    time_flag = 0;
    plflag = 2;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, scroll_start.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, time_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 157;
    time_item = -5;
    time_flag = 0;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 128, time_flag);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 157;
    time_item = 5;
    time_flag = 0;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 130, time_flag);

    reset_player_state();
    keep_work.TimeWarp = 1;
    backto_cnt = 89;
    time_item = 1;
    plpower_a = 1;
    actwk[0].mspeed.w = 100;

    backto_chk();

    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);
}

static void test_chk11_time_attack_exit_forces_signed_speed(test_context *ctx) {
    reset_player_state();
    time_flag = 1;
    stageno.w = 0;
    mapwka[0][0] = 6;

    chk11();

    mapwka[0][0] = 0;
    actwk[0].yspeed.w = -3000;
    actwk[0].cddat = 0;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 1536, actwk[0].xspeed.w);

    reset_player_state();
    time_flag = 1;
    mapwka[0][0] = 6;
    chk11();
    mapwka[0][0] = 0;
    actwk[0].yspeed.w = -3000;
    actwk[0].cddat = 1;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, -1536, actwk[0].xspeed.w);

    reset_player_state();
    mapwka[0][0] = 6;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags);

    reset_player_state();
    time_flag = 1;
    stageno.w = 1;
    mapwka[0][0] = 6;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->status_flags);

    reset_player_state();
    time_flag = 1;
    stageno.w = 0;
    actwk[0].yspeed.w = -3000;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = 10;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].yspeed.w = -500;
    chk11();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
}

static void test_limitchk_clamps_edges_and_routes_fall_death(
    test_context *ctx) {
    reset_player_state();
    scralim_left = 100;
    actwk[0].xposi.w.h = 0;
    actwk[0].xspeed.w = 50;
    actwk[0].mspeed.w = 60;

    limitchk();

    TEST_ASSERT_EQ_INT(ctx, 116, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    bossstart = 1;
    scralim_right = 100;
    actwk[0].xposi.w.h = 500;
    limitchk();
    TEST_ASSERT_EQ_INT(ctx, 404, actwk[0].xposi.w.h);

    reset_player_state();
    scralim_right = 100;
    actwk[0].xposi.w.h = 500;
    limitchk();
    TEST_ASSERT_EQ_INT(ctx, 460, actwk[0].xposi.w.h);

    reset_player_state();
    scralim_left = -1000;
    scralim_right = 1000;
    actwk[0].xposi.w.h = 100;
    actwk[0].xspeed.w = -1;
    limitchk();
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].xposi.w.h);

    reset_player_state();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    limitchk();
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_player_state();
    scralim_down = 100;
    actwk[0].yposi.w.h = 400;
    actwk[0].mstno.b.h = 43;
    limitchk();
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].r_no0);
}

static void test_ballset_and_ballchk_match_roll_setup(test_context *ctx) {
    reset_player_state();
    actwk[0].mspeed.w = 100;
    actwk[0].yposi.w.h = 200;

    ballset();

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 205, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].mspeed.w = -300;
    actwk[0].yposi.w.h = 200;
    swdata.b.h = 2;

    ballchk();

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 202, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -300, actwk[0].mspeed.w);
}

static void test_ballchk_guards_water_low_speed_and_direction(test_context *ctx) {
    reset_player_state();
    mizuflag = 1;
    actwk[0].mspeed.w = 300;
    swdata.b.h = 2;
    ballchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    reset_player_state();
    actwk[0].mspeed.w = 100;
    swdata.b.h = 2;
    ballchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    reset_player_state();
    actwk[0].mspeed.w = 300;
    swdata.b.h = 6;
    ballchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
}

static void test_jumpchk_sets_jump_motion_and_release_clamps_speed(
    test_context *ctx) {
    reset_player_state();
    dircol2_result = 6;
    actwk[0].sprvsize = 19;
    actwk[0].sprhs = 9;
    actwk[0].yposi.w.h = 100;
    swdata.b.l = 16;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());

    TEST_ASSERT_EQ_INT(ctx, 1664, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scr_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 146, soundset_request);

    actwk[0].yspeed.w = -2000;
    swdata.w = 0;
    jumpchk2();
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);

    reset_player_state();
    actwk[0].yspeed.w = -5000;
    jumpchk2();
    TEST_ASSERT_EQ_INT(ctx, -4032, actwk[0].yspeed.w);
}

static void test_jumpchk_rejects_missing_buttons_and_low_clearance(
    test_context *ctx) {
    reset_player_state();
    dircol2_result = 6;

    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.l = 16;
    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());

    reset_player_state();
    swdata.b.h = 1;
    actwk[0].mspeed.w = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());

    reset_player_state();
    swdata.b.l = 16;
    dircol2_result = 5;

    TEST_ASSERT_EQ_INT(ctx, 0, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    reset_player_state();
    swdata.b.l = 16;
    dircol2_result = 6;
    actwk[0].cddat = 64;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[0].xspeed.w);

    reset_player_state();
    chibi_flag = 1;
    swdata.b.l = 16;
    dircol2_result = 6;
    actwk[0].yposi.w.h = 100;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 102, actwk[0].yposi.w.h);
}

static void test_jumpchk_uses_flip_platform_jump_geometry(test_context *ctx) {
    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 3;
    actwk[0].xposi.w.h = 1030;
    actwk[0].yposi.w.h = 190;
    actwk[3].actno = 30;
    actwk[3].xposi.w.h = 1000;
    actwk[3].yposi.w.h = 200;
    actwk[3].sprhsize = 20;
    swdata.b.l = 16;

    TEST_ASSERT_EQ_INT(ctx, 255, jumpchk());

    TEST_ASSERT_EQ_INT(ctx, -3120, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 195, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
}

static void test_keispd_variants_apply_slope_speed_rules(test_context *ctx) {
    reset_player_state();
    stub_sin_value = 128;
    actwk[0].mspeed.w = 100;
    keispd();
    TEST_ASSERT_EQ_INT(ctx, 116, actwk[0].mspeed.w);

    reset_player_state();
    stub_sin_value = 128;
    actwk[0].mspeed.w = -100;
    keispd();
    TEST_ASSERT_EQ_INT(ctx, -84, actwk[0].mspeed.w);

    reset_player_state();
    stub_sin_value = -128;
    actwk[0].mspeed.w = 100;
    keispd2();
    TEST_ASSERT_EQ_INT(ctx, 90, actwk[0].mspeed.w);

    reset_player_state();
    stub_sin_value = 128;
    actwk[0].mspeed.w = -100;
    keispd2();
    TEST_ASSERT_EQ_INT(ctx, -90, actwk[0].mspeed.w);

    reset_player_state();
    stub_sin_value = 128;
    actwk[0].mspeed.w = 0;
    keispd();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    stub_sin_value = 0;
    actwk[0].mspeed.w = 100;
    keispd();
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 100;
    keispd();
    keispd2();
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = 100;
    actwk[0].mspeed.w = 100;
    keispd();
    keispd2();
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);
}

static void test_jumpchk2_release_and_fall_clamps(test_context *ctx) {
    reset_player_state();
    player_work_get(&actwk[0])->jump_started = 1;
    actwk[0].yspeed.w = -2000;
    swdata.b.h = 112;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -2000, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(&actwk[0])->jump_started);

    reset_player_state();
    player_work_get(&actwk[0])->jump_started = 1;
    actwk[0].yspeed.w = -2000;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->spin_dash_counter);

    reset_player_state();
    player_work_get(&actwk[0])->jump_started = 1;
    actwk[0].cddat = 64;
    actwk[0].yspeed.w = -2000;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].yspeed.w);

    reset_player_state();
    actwk[0].yspeed.w = -5000;

    jumpchk2();

    TEST_ASSERT_EQ_INT(ctx, -4032, actwk[0].yspeed.w);
}

static void test_walk_and_ballwalk_adjust_speed_and_direction(test_context *ctx) {
    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = -100;
    plwalk_l();
    plwalk_r();
    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = 200;
    pladdspdwk = 12;
    actwk[0].mspeed.w = -196;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    plmaxspdwk = 200;
    pladdspdwk = 12;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = 196;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = 20;
    ballwalk_l();
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[0].mspeed.w);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = -20;
    ballwalk_r();
    TEST_ASSERT_EQ_INT(ctx, 128, actwk[0].mspeed.w);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = -1200;

    plwalk_r();

    TEST_ASSERT_EQ_INT(ctx, -1072, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 144, soundset_request);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = -20;
    plwalk_r();
    TEST_ASSERT_EQ_INT(ctx, 128, actwk[0].mspeed.w);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = -1200;
    actwk[0].direc.b.h = 80;
    plwalk_r();
    TEST_ASSERT_EQ_INT(ctx, -1072, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = 20;
    plwalk_l();
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[0].mspeed.w);

    reset_player_state();
    plretspdwk = 128;
    actwk[0].mspeed.w = 1200;
    actwk[0].direc.b.h = 80;
    plwalk_l();
    TEST_ASSERT_EQ_INT(ctx, 1072, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
}

static void test_plwalk_brake_and_lever_crouch_jump_roll(test_context *ctx) {
    reset_player_state();
    play00(&actwk[0]);
    plretspdwk = 128;
    actwk[0].mspeed.w = 1200;

    plwalk_l();

    TEST_ASSERT_EQ_INT(ctx, 1072, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 144, soundset_request);

    reset_player_state();
    pladdspdwk = 0;
    swdata.b.h = 2;
    swdata.b.l = 16;
    actwk[0].yposi.w.h = 100;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 156, soundset_request);

    jumpcolsub();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_player_state();
    plretspdwk = 128;
    swdata.b.h = 12;
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = 200;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 168, actwk[0].mspeed.w);

    reset_player_state();
    pladdspdwk = 0;
    swdata.b.h = 2;
    swdata.b.l = 16;
    levermove();
    wave_all_stop_count = 0;
    pladdspdwk = 20;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 0;
    actwk[0].mspeed.w = 5;
    swdata.w = 0;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_player_state();
    mizuflag = 1;
    actwk[0].mspeed.w = 5000;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].xspeed.w);

    reset_player_state();
    mizuflag = 1;
    actwk[0].mspeed.w = -5000;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].xspeed.w);
}

static void test_levermove_release_and_camera_branches(test_context *ctx) {
    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 10;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_request);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 30;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_request);

    reset_player_state();
    scr_cnt = 128;
    scra_vline = 100;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 102, scra_vline);

    reset_player_state();
    scr_cnt = 64;
    scra_vline = 100;
    swdata.b.h = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 98, scra_vline);
}

static void test_levermove_platform_edges_and_charge_acceleration(
    test_context *ctx) {
    reset_player_state();
    mizuflag = 1;
    actwk[0].mspeed.w = 123;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);

    reset_player_state();
    swdata.b.h = 12;
    plretspdwk = 128;
    actwk[0].mspeed.w = 200;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 72, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = 80;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].mspeed.w = 100;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = 80;
    actwk[0].mspeed.w = 500;
    fallchk();
    actwk[0].mstno.b.h = 99;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 99, actwk[0].mstno.b.h);

    reset_player_state();
    emycol_d_result = 12;
    player_work_get(&actwk[0])->floor_left = 3;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_request);

    reset_player_state();
    pladdspdwk = 0;
    swdata.b.h = 1;
    swdata.b.l = 16;
    levermove();
    emycol_d_result = 12;
    actwk[0].cddat = 0;
    player_work_get(&actwk[0])->spin_dash_counter = 0;
    swdata.w = 0;
    wave_all_stop_count = 0;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_player_state();
    emycol_d_result = 12;
    actwk[0].cddat = 1;
    player_work_get(&actwk[0])->floor_right = 3;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].mstno.b.h);

    reset_player_state();
    plmaxspdwk = 200;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = 200;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].cddat = 1;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = -200;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -400, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = 200;
    plpower_s = 1;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 240;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 300, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[2].cddat = 128;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[2].actno = 30;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[0].xposi.w.h = 100;
    actwk[2].xposi.w.h = 0;
    actwk[2].sprhsize = 60;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[0].xposi.w.h = 10;
    actwk[2].xposi.w.h = 0;
    actwk[2].sprhsize = 60;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);

    reset_player_state();
    plmaxspdwk = 200;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].cddat = 1;
    actwk[0].mspeed.w = -500;
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -400, actwk[0].mspeed.w);

    reset_player_state();
    swdata.b.h = 1;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->spin_dash_counter);

    reset_player_state();
    pladdspdwk = 0;
    swdata.b.h = 1;
    swdata.b.l = 16;
    levermove();
    wave_all_stop_count = 0;
    swdata.w = 0;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);

    reset_player_state();
    scr_cnt = 1;
    swdata.b.l = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 66, scr_cnt);

    reset_player_state();
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    swdata.b.h = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.h);

    reset_player_state();
    swdata.b.h = 2;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[0])->spin_dash_counter);

    reset_player_state();
    actwk[0].cddat = 1;
    swdata.b.h = 2;
    swdata.b.l = 16;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, -22, actwk[0].mspeed.w);

    reset_player_state();
    scra_vline = 90;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 92, scra_vline);

    reset_player_state();
    pladdspdwk = 10;
    actwk[0].mspeed.w = 5;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    pladdspdwk = 10;
    actwk[0].mspeed.w = -5;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
}

static void test_levermove_ride_geometry_and_button_latches(test_context *ctx) {
    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[2].actno = 30;
    actwk[0].mstno.b.h = 9;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].cddat = 8;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[0].xposi.w.h = 95;
    actwk[2].xposi.w.h = 100;
    actwk[2].sprhsize = 8;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].cddat = 9;
    player_work_get(&actwk[0])->ride_actor_index = 2;
    actwk[0].xposi.w.h = 200;
    actwk[2].xposi.w.h = 100;
    actwk[2].sprhsize = 8;

    levermove();

    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mstno.b.h);

    reset_player_state();
    swdata.b.l = 1;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 1, scr_cnt);

    swdata.b.l = 1;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 130, scr_cnt);

    reset_player_state();
    swdata.b.l = 2;
    levermove();
    TEST_ASSERT_EQ_INT(ctx, 1, scr_cnt);
}

static void test_balllmove_stops_roll_and_restores_standing_shape(
    test_context *ctx) {
    reset_player_state();
    pladdspdwk = 8;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 4;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 95, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_request);
}

static void test_balllmove_charge_release_and_spin_dash_finish(
    test_context *ctx) {
    reset_player_state();
    plmaxspdwk = 200;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 180;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 255, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_player_state();
    plmaxspdwk = 200;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 45;
    actwk[0].mspeed.w = 100;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_request);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 175, actwk[0].xspeed.w);
}

static void test_balllmove_left_charge_release_and_water_motion(
    test_context *ctx) {
    reset_player_state();
    plmaxspdwk = 200;
    actwk[0].cddat = 5;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = -180;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -255, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = 200;
    actwk[0].cddat = 5;
    player_work_get(&actwk[0])->spin_dash_counter = 45;
    actwk[0].mspeed.w = -100;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -175, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].cddat);

    reset_player_state();
    mizuflag = 1;
    actwk[0].mspeed.w = 200;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
}

static void test_balllmove_abort_charge_and_power_s_cap(test_context *ctx) {
    reset_player_state();
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 100;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_request);

    reset_player_state();
    plpower_s = 1;
    plmaxspdwk = 200;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 240;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 300, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = -200;
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 100;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -400, actwk[0].mspeed.w);

    reset_player_state();
    plmaxspdwk = 200;
    actwk[0].cddat = 5;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = -500;
    swdata.b.h = 2;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, -400, actwk[0].mspeed.w);

    reset_player_state();
    pladdspdwk = 20;
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = 5;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    pladdspdwk = 20;
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = -5;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);

    reset_player_state();
    chibi_flag = 1;
    pladdspdwk = 20;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 8;
    actwk[0].sprhs = 5;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 5;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 98, actwk[0].yposi.w.h);

    reset_player_state();
    pladdspdwk = 0;
    swdata.b.h = 2;
    swdata.b.l = 16;
    levermove();
    actwk[0].cddat = 4;
    player_work_get(&actwk[0])->spin_dash_counter = 1;
    actwk[0].mspeed.w = 100;
    swdata.w = 0;

    balllmove();

    TEST_ASSERT_EQ_INT(ctx, 1, wave_all_stop_count);
}

static void test_jumpmove_air_control_and_camera_return(test_context *ctx) {
    reset_player_state();
    plmaxspdwk = 300;
    pladdspdwk = 10;
    scra_vline = 90;
    actwk[0].xspeed.w = 290;
    swdata.b.h = 8;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 300, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 92, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    reset_player_state();
    plmaxspdwk = 300;
    pladdspdwk = 10;
    scra_vline = 100;
    actwk[0].xspeed.w = -290;
    swdata.b.h = 4;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, -300, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 98, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].cddat);

    reset_player_state();
    actwk[0].xspeed.w = 320;
    actwk[0].yspeed.w = -1024;
    jumpmove();
    TEST_ASSERT_EQ_INT(ctx, 310, actwk[0].xspeed.w);

    reset_player_state();
    time_flag = 1;
    stageno.w = 0;
    actwk[0].xposi.w.h = 1800;
    actwk[0].xspeed.w = 123;
    swdata.b.h = 8;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);

    reset_player_state();
    time_flag = 1;
    stageno.w = 0;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].xposi.w.h = 1700;
    actwk[0].xspeed.w = 123;
    swdata.b.h = 8;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);

    reset_player_state();
    actwk[0].xspeed.w = -320;
    actwk[0].yspeed.w = -1024;
    jumpmove();
    TEST_ASSERT_EQ_INT(ctx, -310, actwk[0].xspeed.w);

    reset_player_state();
    actwk[0].xspeed.w = 20;
    actwk[0].yspeed.w = -1024;
    jumpmove();
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[0].xspeed.w);

    reset_player_state();
    time_flag = 1;
    stageno.w = 0;
    actwk[0].xposi.w.h = 1700;
    actwk[0].xspeed.w = 123;

    jumpmove();

    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].xspeed.w);
}

static void test_hedcolchk_direcchg_and_lmovecol_visible_adjustments(
    test_context *ctx) {
    reset_player_state();
    dircol_u_result = -1;
    actwk[0].xspeed.w = 10;
    actwk[0].yspeed.w = 20;
    actwk[0].mspeed.w = 30;

    hedcolchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].direc.b.h = -3;
    direcchg();
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[0].direc.b.h);
    direcchg();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].direc.b.h);

    reset_player_state();
    dircolm_result = -2;
    actwk[0].mspeed.w = 100;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, -512, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[0].cddat);

    reset_player_state();
    dircolm_result = -3;
    dircolm_sets_direction = 1;
    dircolm_direction_out = -32;
    actwk[0].mspeed.w = 100;
    actwk[0].yspeed.w = 10;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, -758, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    dircolm_result = -1;
    dircolm_sets_direction = 1;
    dircolm_direction_out = 96;
    actwk[0].mspeed.w = -100;
    actwk[0].yspeed.w = 10;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, 266, actwk[0].yspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = -65;
    actwk[0].mspeed.w = 100;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_player_state();
    dircolm_result = 0;
    actwk[0].mspeed.w = 100;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].mspeed.w);

    reset_player_state();
    dircolm_result = -2;
    dircolm_sets_direction = 1;
    dircolm_direction_out = 32;
    actwk[0].mspeed.w = 100;
    actwk[0].xspeed.w = 10;
    lmovecol();
    TEST_ASSERT_EQ_INT(ctx, 522, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
}

static void test_direcchg_positive_and_skip_cases(test_context *ctx) {
    reset_player_state();
    actwk[0].direc.b.h = 3;
    direcchg();
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].direc.b.h);
    direcchg();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].direc.b.h);

    reset_player_state();
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 10;
    direcchg();
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].direc.b.h);
}

static void test_jumpcolchk_lands_from_floor_and_ceiling_contacts(
    test_context *ctx) {
    reset_player_state();
    stub_atan_value = 32;
    dircol_l2_result = -3;
    dircol_r2_result = -4;
    dircol_d_d1 = -2;
    dircol_d_d3 = 0;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 100;
    actwk[0].xspeed.w = 80;
    actwk[0].yspeed.w = 100;
    actwk[0].mspeed.w = 25;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 49, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 93, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].sprhs);

    reset_player_state();
    stub_atan_value = 160;
    dircol_u_d1 = -5;
    dircol_u_d3 = 64;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -200;
    actwk[0].mspeed.w = 30;
    actwk[0].cddat = 6;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].direc.b.h);
    TEST_ASSERT_EQ_INT(ctx, -200, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
}

static void test_jumpcolchk_side_quadrants_use_side_and_floor_contacts(
    test_context *ctx) {
    reset_player_state();
    stub_atan_value = 96;
    dircol_l2_result = -4;
    actwk[0].xposi.w.h = 40;
    actwk[0].xspeed.w = -80;
    actwk[0].yspeed.w = 120;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 44, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 120, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 96;
    dircol_u_d1 = -5;
    actwk[0].yposi.w.h = 70;
    actwk[0].yspeed.w = -30;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 75, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_player_state();
    stub_atan_value = 96;
    dircol_d_d1 = -6;
    dircol_d_d3 = 0;
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 90;
    actwk[0].xspeed.w = 222;
    actwk[0].yspeed.w = 10;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 79, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 222, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 224;
    dircol_r2_result = -3;
    actwk[0].xposi.w.h = 40;
    actwk[0].xspeed.w = 80;
    actwk[0].yspeed.w = 120;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 37, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
}

static void test_jumpcolchk_ceiling_slope_and_right_floor_contacts(
    test_context *ctx) {
    reset_player_state();
    stub_atan_value = 160;
    dircol_l2_result = -2;
    dircol_r2_result = -3;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 70;
    actwk[0].yspeed.w = -100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 49, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 70, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);

    reset_player_state();
    stub_atan_value = 160;
    dircol_u_d1 = -4;
    dircol_u_d3 = 64;
    actwk[0].xposi.w.h = 50;
    actwk[0].yposi.w.h = 70;
    actwk[0].yspeed.w = -100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 74, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].direc.b.h);
    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 224;
    dircol_d_d1 = -5;
    dircol_d_d3 = 0;
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 100;
    actwk[0].xspeed.w = -222;
    actwk[0].yspeed.w = 20;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 90, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -222, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
}

static void test_jumpcolchk_floor_and_ceiling_guard_branches(
    test_context *ctx) {
    reset_player_state();
    stub_atan_value = 32;
    dircol_d_d1 = 0;
    actwk[0].yposi.w.h = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yposi.w.h);

    reset_player_state();
    stub_atan_value = 32;
    dircol_d_d0 = -20;
    dircol_d_d1 = -20;
    actwk[0].yspeed.w = 0;
    actwk[0].yposi.w.h = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yposi.w.h);

    reset_player_state();
    stub_atan_value = 32;
    dircol_d_d1 = -2;
    dircol_d_d3 = 32;
    actwk[0].yposi.w.h = 100;
    actwk[0].xspeed.w = 100;
    actwk[0].yspeed.w = 5000;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 98, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4032, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4032, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 32;
    dircol_d_d1 = -2;
    dircol_d_d3 = 16;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 98, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 32;
    dircol_d_d1 = -2;
    dircol_d_d3 = -32;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, -50, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 96;
    actwk[0].yspeed.w = -1;
    actwk[0].yposi.w.h = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yposi.w.h);

    reset_player_state();
    stub_atan_value = 96;
    actwk[0].yspeed.w = 1;
    dircol_d_d1 = 0;
    actwk[0].yposi.w.h = 100;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yposi.w.h);

    reset_player_state();
    stub_atan_value = 160;
    dircol_u_d1 = -5;
    dircol_u_d3 = 0;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -30;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);

    reset_player_state();
    stub_atan_value = 160;
    dircol_u_d1 = -5;
    dircol_u_d3 = -64;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -30;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[0].mspeed.w);

    reset_player_state();
    stub_atan_value = 224;
    dircol_u_d1 = -5;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -30;

    jumpcolchk();

    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
}

static void test_fallchk_sets_airborne_state_for_steep_low_speed(
    test_context *ctx) {
    reset_player_state();
    actwk[0].direc.b.h = 80;
    actwk[0].mspeed.w = 500;

    fallchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);

    reset_player_state();
    player_work_get(&actwk[0])->jump_lock = 1;
    actwk[0].direc.b.h = 80;
    actwk[0].mspeed.w = 500;
    fallchk();
    TEST_ASSERT_EQ_INT(ctx, 500, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = 80;
    actwk[0].mspeed.w = 700;
    fallchk();
    TEST_ASSERT_EQ_INT(ctx, 700, actwk[0].mspeed.w);

    reset_player_state();
    actwk[0].direc.b.h = 80;
    actwk[0].mspeed.w = 500;
    fallchk();
    actwk[0].mspeed.w = 123;
    fallchk();
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[0].mspeed.w);
}

static void test_play00walk_jump_and_ball_wrappers_call_expected_helpers(
    test_context *ctx) {
    reset_player_state();
    actwk[0].mstno.b.h = 43;
    actwk[0].patno = 23;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, speedset2_count);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].mstno.b.h = 43;
    actwk[0].patno = 120;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);

    reset_player_state();
    actwk[0].mstno.b.h = 43;
    actwk[0].patno = 22;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset_count);

    reset_player_state();
    swdata.b.l = 16;
    dircol2_result = 10;

    play00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[0].cddat);

    reset_player_state();
    actwk[0].yspeed.w = 10;
    actwk[0].mstno.b.h = 5;

    play00jump();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_player_state();
    actwk[0].cddat = 4;
    actwk[0].mspeed.w = 20;

    ball00walk();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, fcol_count);

    reset_player_state();
    actwk[0].cddat = 4;
    swdata.b.l = 16;
    dircol2_result = 10;

    ball00walk();

    TEST_ASSERT_EQ_INT(ctx, 0, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 22, actwk[0].cddat);

    reset_player_state();
    actwk[0].cddat = 6;
    actwk[0].yspeed.w = 10;

    ball00jump();

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);

    reset_player_state();
    actwk[0].cddat = 66;
    actwk[0].yspeed.w = 10;

    play00jump();

    TEST_ASSERT_EQ_INT(ctx, -30, actwk[0].yspeed.w);

    reset_player_state();
    actwk[0].cddat = 70;
    actwk[0].yspeed.w = 10;

    ball00jump();

    TEST_ASSERT_EQ_INT(ctx, -30, actwk[0].yspeed.w);
}

static void test_loopchk_toggles_priority_and_ball_tiles(test_context *ctx) {
    reset_player_state();
    stageno.b.h = 3;
    loopmapno = 9;
    loopmapno2 = 10;
    ballmapno = 11;
    ballmapno2 = 12;
    actwk[0].actflg = 64;
    mapwka[0][0] = 0;

    loopchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);

    mapwka[0][0] = loopmapno;
    actwk[0].xposi.b.b2 = 100;
    actwk[0].direc.b.h = 64;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);

    actwk[0].direc.b.h = -120;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);

    reset_player_state();
    stageno.b.h = 3;
    ballmapno = 11;
    mapwka[0][0] = ballmapno;
    actwk[0].mspeed.w = 200;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 512, actwk[0].mspeed.w);

    reset_player_state();
    stageno.b.h = 0;
    ballmapno = 11;
    mapwka[0][0] = ballmapno;
    actwk[0].yposi.w.h = 100;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);

    actwk[0].yposi.w.h = 144;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);

    reset_player_state();
    stageno.b.h = 3;
    ballmapno2 = 12;
    mapwka[0][0] = ballmapno2;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);

    reset_player_state();
    stageno.b.h = 3;
    loopmapno = 9;
    mapwka[0][0] = loopmapno;
    actwk[0].xposi.b.b2 = 100;
    actwk[0].actflg = 64;
    actwk[0].direc.b.h = 64;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);
}

static void test_loopchk_boundary_and_stage7_loop_cases(test_context *ctx) {
    reset_player_state();
    stageno.b.h = 3;
    loopmapno = 9;
    mapwka[0][0] = loopmapno;
    actwk[0].xposi.b.b2 = 43;
    actwk[0].actflg = 64;

    loopchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);

    actwk[0].xposi.b.b2 = -32;
    loopchk();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);

    reset_player_state();
    stageno.b.h = 5;
    loopmapno2 = 10;
    mapwka[0][0] = loopmapno2;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 40;

    loopchk();

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);

    reset_player_state();
    stageno.b.h = 3;
    loopmapno2 = 10;
    mapwka[0][0] = loopmapno2;
    actwk[0].cddat = 2;
    actwk[0].actflg = 64;

    loopchk();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);
}

static void test_st7_x_switches_priority_by_quadrant(test_context *ctx) {
    reset_player_state();
    actwk[0].yspeed.w = 0;
    stageno.b.h = 5;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 40;

    st7_x();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);

    actwk[0].yposi.w.h = 140;
    st7_x();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);

    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 140;
    st7_x();
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].actflg);

    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 40;
    actwk[0].actflg = 64;
    st7_x();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);

    actwk[0].yspeed.w = -1;
    actwk[0].actflg = 0;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 40;
    st7_x();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actflg);
}

static void test_jumpcolsub_restores_standing_shape(test_context *ctx) {
    reset_player_state();
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;
    actwk[0].yposi.w.h = 100;
    actwk[0].mstno.b.h = 2;
    emyscorecnt = 7;

    jumpcolsub();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 95, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, emyscorecnt);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_request);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].cddat = 4;
    actwk[0].sprvsize = 8;
    actwk[0].sprhs = 5;
    actwk[0].yposi.w.h = 100;
    jumpcolsub();
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 98, actwk[0].yposi.w.h);
}

static void test_frip_spd_uses_flip_actor_geometry(test_context *ctx) {
    Sint32 cal_jump = 0;
    Uint8 cal_direc = 123;

    reset_player_state();
    player_work_get(&actwk[0])->ride_actor_index = 3;

    TEST_ASSERT_EQ_INT(ctx, 255, frip_spd(&cal_jump, &cal_direc));
    TEST_ASSERT_EQ_INT(ctx, 0, cal_jump);
    TEST_ASSERT_EQ_INT(ctx, 123, cal_direc);

    actwk[3].actno = 30;
    actwk[3].xposi.w.h = 1000;
    actwk[3].yposi.w.h = 200;
    actwk[3].sprhsize = 20;
    actwk[3].cddat = 0;
    actwk[0].xposi.w.h = 1030;
    actwk[0].yposi.w.h = 190;

    TEST_ASSERT_EQ_INT(ctx, 0, frip_spd(&cal_jump, &cal_direc));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[3].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, cal_direc);
    TEST_ASSERT_EQ_INT(ctx, -3120, cal_jump);
}

static void test_play00damage_and_die_sub_update_visible_state(
    test_context *ctx) {
    reset_player_state();
    actwk[0].r_no0 = 4;
    actwk[0].cddat = 2;
    actwk[0].yspeed.w = 100;
    actwk[0].xposi.w.h = 10;
    actwk[0].yposi.w.h = 20;

    play00damage();

    TEST_ASSERT_EQ_INT(ctx, 148, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actwk[0]);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    actwk[0].mstno.b.h = 0;
    pl_suu = 2;
    pltime_f = 1;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, -56, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime_f);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    actwk[0].mstno.b.h = 43;
    pl_suu = 1;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 59, actwk[20].actno);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    actwk[0].mstno.b.h = 43;
    pl_suu = 2;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    pl_suu = 0;

    play00die_sub();

    TEST_ASSERT_EQ_INT(ctx, 8, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);
}

static void test_play00erase_completes_death_timer_paths(test_context *ctx) {
    int i;

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    pl_suu = 1;
    pltime_f = 1;
    time_flag = 1;
    play_start = 3;

    play00(&actwk[0]);
    for (i = 0; i < 60; ++i) {
        play00(&actwk[0]);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, gameflag.w);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwkclr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 14, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, play_start);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    actwk[0].mstno.b.h = 43;
    pl_suu = 2;
    pltime_f = 1;
    time_flag = 0;

    play00(&actwk[0]);
    for (i = 0; i < 480; ++i) {
        play00(&actwk[0]);
    }

    TEST_ASSERT_EQ_INT(ctx, 0, plflag);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;
    actwk[0].r_no0 = 6;
    actwk[0].mstno.b.h = 43;
    pl_suu = 2;
    pltime_f = 1;
    time_flag = 1;
    markerno = 3;

    play00(&actwk[0]);
    for (i = 0; i < 480; ++i) {
        play00(&actwk[0]);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, plflag);
}

static void collect_item_powerup(Uint8 mstno) {
    switch (mstno) {
    case 3:
        plpower_m = 1;
        set_player_work_word(8, 1320);
        break;
    case 4:
        plpower_s = 1;
        set_player_work_word(10, 1320);
        plmaxspdwk = 3072;
        pladdspdwk = 24;
        plretspdwk = 128;
        break;
    default:
        plpower_m = 1;
        set_player_work_word(8, 1320);
        plpower_s = 1;
        set_player_work_word(10, 1320);
        plmaxspdwk = 3072;
        pladdspdwk = 24;
        plretspdwk = 128;
        break;
    }
}

static void test_play00_damage_die_wrappers_and_grounded_damage_sub(
    test_context *ctx) {
    reset_player_state();
    actwk[0].r_no0 = 4;
    actwk[0].cddat = 66;
    actwk[0].yspeed.w = 100;

    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 116, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actwk[0].patbase == sncpat);

    reset_player_state();
    actwk[0].r_no0 = 6;
    play00(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actwk[0].patbase == sncpat);

    reset_player_state();
    actwk[0].r_no0 = 4;
    actwk[0].cddat = 0;
    actwk[0].xspeed.w = 11;
    actwk[0].yspeed.w = 12;
    actwk[0].mspeed.w = 13;
    actwk[0].mstno.b.h = 5;

    play00damage_sub();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);

    actionsub_count = 0;
    playpowercnt();
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    playpowercnt();
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_player_state();
    actwk[0].r_no0 = 4;
    actwk[0].xspeed.w = 11;
    actwk[0].yspeed.w = 12;
    actwk[0].mspeed.w = 13;
    play00damage_sub();
    actionsub_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;
    playpowercnt();
    playpowercnt();
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_player_state();
    scralim_down = 0;
    actwk[0].yposi.w.h = 300;

    play00damage_sub();

    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);
}

static void test_playpowercnt_expires_item_seeded_power_timers(
    test_context *ctx) {
    int i;

    reset_player_state();
    collect_item_powerup(3);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_m);
    sub_sync_count = 0;
    da_set_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;

    for (i = 0; i < 1320; ++i) {
        playpowercnt();
    }

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 14, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_player_state();
    collect_item_powerup(4);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_s);
    sub_sync_count = 0;
    da_set_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;

    for (i = 0; i < 1320; ++i) {
        playpowercnt();
    }

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 1536, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 12, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_player_state();
    collect_item_powerup(9);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_s);
    sub_sync_count = 0;
    da_set_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;

    for (i = 0; i < 1320; ++i) {
        playpowercnt();
    }

    TEST_ASSERT_EQ_INT(ctx, 0, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_player_state();
    time_flag = 1;
    collect_item_powerup(3);
    sub_sync_count = 0;
    da_set_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;

    for (i = 0; i < 1320; ++i) {
        playpowercnt();
    }

    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 1, da_set_count);

    reset_player_state();
    boss_sound = 1;
    collect_item_powerup(4);
    sub_sync_count = 0;
    da_set_count = 0;
    player_work_get(&actwk[0])->status_flags = 64;

    for (i = 0; i < 1320; ++i) {
        playpowercnt();
    }

    TEST_ASSERT_EQ_INT(ctx, 0, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 0, da_set_count);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_s);
}

static void test_patchgmain2_handles_next_loop_back_and_state_change(
    test_context *ctx) {
    Uint8 next_pattern[4] = {0, 7, 8, 9};
    Uint8 loop_pattern[5] = {0, 4, 5, 255, 0};
    Uint8 back_pattern[6] = {0, 1, 2, 254, 2, 0};
    Uint8 state_pattern[5] = {0, 1, 2, 253, 12};

    reset_player_state();
    patchgmain2(next_pattern);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patcnt);

    actwk[0].patcnt = 2;
    patchgmain2(loop_pattern);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].patno);

    actwk[0].patcnt = 2;
    patchgmain2(back_pattern);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patno);

    actwk[0].patcnt = 2;
    patchgmain2(state_pattern);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[0].mstno.b.h);

    reset_player_state();
    actwk[0].mstno.b.h = 5;
    actwk[0].mstno.b.l = 5;
    actwk[0].pattim = 2;
    patchgmain();
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);
}

static void test_animation_changers_select_exact_frames_and_timing(
    test_context *ctx) {
    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1600;

    playrunchg(255);

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 75, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].patcnt);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1600;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 512;
    actwk[0].cddat = 1;
    playrunchg3(253);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].actflg);

    reset_player_state();
    actwk[0].mstno.b.h = 10;
    actwk[0].patcnt = 0;
    actwk[0].patno = 99;
    playetc_chg();
    TEST_ASSERT_EQ_INT(ctx, 52, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 2000;
    actwk[0].direc.b.h = 0;
    little_runchg();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 118, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 512;
    actwk[0].cddat = 32;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 0;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 124, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].cddat = 1;
    actwk[0].direc.b.h = 32;
    little_runchg();
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[0].sprhs);
    TEST_ASSERT_EQ_INT(ctx, 124, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 2600;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 148, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 300;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = 64;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[0].patno);

    reset_player_state();
    actwk[0].mstno.b.h = 10;
    playrunchg2(10);
    TEST_ASSERT_EQ_INT(ctx, 52, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 2;
    actwk[0].patcnt = 0;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].patcnt);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1000;
    player_work_get(&actwk[0])->status_flags = 2;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 166, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1000;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 115, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 300;
    player_work_get(&actwk[0])->status_flags = 2;
    actwk[0].direc.b.h = -16;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = -1600;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 1000;
    playrunchg2(254);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 512;
    playrunchg3(253);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 115, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 3000;
    playrunchg3(253);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 3000;
    little_runchg();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 118, actwk[0].patno);

    reset_player_state();
    chibi_flag = 1;
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = -1000;
    little_runchg();
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 115, actwk[0].patno);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = -1600;
    actwk[0].cddat = 1;
    actwk[0].direc.b.h = 120;
    playrunchg(255);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 75, actwk[0].patno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].actflg);

    reset_player_state();
    actwk[0].pattim = 0;
    actwk[0].mspeed.w = 300;
    playrunchg(254);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].pattim);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[0].patno);
}

static void test_empty_player_hooks_are_callable(test_context *ctx) {
    reset_player_state();

    test_mapwrt();
    playwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actno);
}

static void test_little_patchg_maps_only_in_chibi_mode(test_context *ctx) {
    reset_player_state();

    TEST_ASSERT_EQ_INT(ctx, 10, little_patchg(10));

    chibi_flag = 1;
    TEST_ASSERT_EQ_INT(ctx, 35, little_patchg(2));
    TEST_ASSERT_EQ_INT(ctx, 48, little_patchg(43));
}

TEST_MAIN_BEGIN;
test_bye_chk_resets_counter_when_not_waiting(&ctx);
test_bye_chk_starts_and_completes_goodbye_state(&ctx);
test_play00_ignores_non_player_and_routes_edit_mode(&ctx);
test_muteki_set_allocates_only_when_slot_is_free(&ctx);
test_play00init_sets_normal_and_chibi_dimensions(&ctx);
test_mapno_chk_reads_map_byte_with_base_value(&ctx);
test_mizuki_set_spawns_only_for_stage0_tile_47(&ctx);
test_mizuki_set_guards_odd_timer_position_and_allocation(&ctx);
test_scr_h_moves_toward_center_or_speed_edges(&ctx);
test_play00_and_play00move_dispatch_visible_states(&ctx);
test_play00move_auto_backto_and_water_state_paths(&ctx);
test_playposiwkset_records_position_and_advances_ring(&ctx);
test_plwaterchk_enters_water_and_spawns_splash(&ctx);
test_plwaterchk_leaves_water_and_clamps_upward_speed(&ctx);
test_plwaterchk_guards_stage_start_and_failed_splash_alloc(&ctx);
test_playsave_copies_visible_state_and_clamps_time(&ctx);
test_backto_chk_saves_time_warp_and_fades_at_terminal_count(&ctx);
test_backto_chk_resets_before_time_warp_threshold(&ctx);
test_backto_chk_skips_repeat_save_when_flag_already_set(&ctx);
test_chk11_time_attack_exit_forces_signed_speed(&ctx);
test_limitchk_clamps_edges_and_routes_fall_death(&ctx);
test_ballset_and_ballchk_match_roll_setup(&ctx);
test_ballchk_guards_water_low_speed_and_direction(&ctx);
test_jumpchk_sets_jump_motion_and_release_clamps_speed(&ctx);
test_jumpchk_rejects_missing_buttons_and_low_clearance(&ctx);
test_jumpchk_uses_flip_platform_jump_geometry(&ctx);
test_keispd_variants_apply_slope_speed_rules(&ctx);
test_jumpchk2_release_and_fall_clamps(&ctx);
test_walk_and_ballwalk_adjust_speed_and_direction(&ctx);
test_plwalk_brake_and_lever_crouch_jump_roll(&ctx);
test_levermove_release_and_camera_branches(&ctx);
test_levermove_platform_edges_and_charge_acceleration(&ctx);
test_levermove_ride_geometry_and_button_latches(&ctx);
test_balllmove_stops_roll_and_restores_standing_shape(&ctx);
test_balllmove_charge_release_and_spin_dash_finish(&ctx);
test_balllmove_left_charge_release_and_water_motion(&ctx);
test_balllmove_abort_charge_and_power_s_cap(&ctx);
test_jumpmove_air_control_and_camera_return(&ctx);
test_hedcolchk_direcchg_and_lmovecol_visible_adjustments(&ctx);
test_direcchg_positive_and_skip_cases(&ctx);
test_jumpcolchk_lands_from_floor_and_ceiling_contacts(&ctx);
test_jumpcolchk_side_quadrants_use_side_and_floor_contacts(&ctx);
test_jumpcolchk_ceiling_slope_and_right_floor_contacts(&ctx);
test_jumpcolchk_floor_and_ceiling_guard_branches(&ctx);
test_fallchk_sets_airborne_state_for_steep_low_speed(&ctx);
test_play00walk_jump_and_ball_wrappers_call_expected_helpers(&ctx);
test_loopchk_toggles_priority_and_ball_tiles(&ctx);
test_loopchk_boundary_and_stage7_loop_cases(&ctx);
test_st7_x_switches_priority_by_quadrant(&ctx);
test_jumpcolsub_restores_standing_shape(&ctx);
test_frip_spd_uses_flip_actor_geometry(&ctx);
test_play00damage_and_die_sub_update_visible_state(&ctx);
test_play00erase_completes_death_timer_paths(&ctx);
test_play00_damage_die_wrappers_and_grounded_damage_sub(&ctx);
test_playpowercnt_expires_item_seeded_power_timers(&ctx);
test_patchgmain2_handles_next_loop_back_and_state_change(&ctx);
test_animation_changers_select_exact_frames_and_timing(&ctx);
test_empty_player_hooks_are_callable(&ctx);
test_little_patchg_maps_only_in_chibi_mode(&ctx);
TEST_MAIN_END;


#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
Uint8 flagworkcnt;
Uint8 flagwork[766];
Uint8 flowercnt[3];
int_union flowerposi[192];
Uint8 plpower_b;
Uint8 plpower_m;
Uint8 plpower_a;
Uint8 plflag;
Uint8 plflag_s;
Sint16 plxposi_s;
Sint16 plyposi_s;
Sint32 pltime_s;
Uint8 time_flag;
Sint16 scralim_down_s;
Sint16 scra_h_posit_s;
Sint16 scra_v_posit_s;
Sint16 scrb_h_posit_s;
Sint16 scrb_v_posit_s;
Sint16 scrc_h_posit_s;
Sint16 scrc_v_posit_s;
Sint16 scrz_h_posit_s;
Sint16 scrz_v_posit_s;
Sint16 waterposi_m_s;
Uint8 water_flag_s;
Uint8 waterflag_s;
Uint8 plsubchg_flag;
Uint8 markerno;
Uint8 chibi_s;
Uint8 prio_flag;
Uint8 chibi_flag;
Sint16 waterposi_m;
Uint8 water_flag;
Uint8 waterflag;
Sint16 scralim_down;
ushort_union plposiwkadr;
int_union scrz_v_posit;
int_union scrz_h_posit;
int_union scrc_v_posit;
int_union scrc_h_posit;
int_union scrb_v_posit;
int_union scrb_h_posit;
int_union scra_v_posit;
int_union scra_h_posit;
ushort_union swdata1;
Sint16 playposiwk[128];
short_union editmode;
int_union pltime;
short_union stageno;

static sprite_pattern dummy_pattern = {1, {{0, 0, 0, 1}}};
sprite_pattern *markerpat[] = {&dummy_pattern};
sprite_pattern *bariapat[] = {&dummy_pattern};
sprite_pattern *bakupat[] = {&dummy_pattern};
sprite_pattern *mizukipat[] = {&dummy_pattern};
sprite_pattern *futapat[] = {&dummy_pattern};
sprite_pattern *exit2pat[] = {&dummy_pattern};

Uint8 markerchg0[] = {1, 0, 255};
Uint8 bariachg0[] = {1, 1, 255};
Uint8 bakuchg0[] = {1, 2, 255};
Uint8 mizukichg0[] = {1, 3, 255};
Uint8 futachg0[] = {1, 4, 255};
Uint8 exit2chg0[] = {1, 5, 255};
Uint8 *markerchg[] = {markerchg0, markerchg0, markerchg0, markerchg0};
Uint8 *bariachg[] = {bariachg0, bariachg0, bariachg0, bariachg0, bariachg0,
                      bariachg0};
Uint8 *bakuchg[] = {bakuchg0, bakuchg0};
Uint8 *mizukichg[] = {mizukichg0};
Uint8 *futachg[] = {futachg0};
Uint8 *exit2chg[] = {exit2chg0, exit2chg0, exit2chg0};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[16];
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int ridechk_count;
static sprite_status *ridechk_platform;
static sprite_status *ridechk_player;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/playsub.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
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
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_platform = pActwk;
    ridechk_player = pPlayerwk;
    return 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_playsub_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    memset(flowercnt, 0, sizeof(flowercnt));
    memset(flowerposi, 0, sizeof(flowerposi));
    flagworkcnt = 0;
    plpower_b = 0;
    plpower_m = 0;
    plpower_a = 0;
    plflag = 0;
    plflag_s = 0;
    plxposi_s = 0;
    plyposi_s = 0;
    pltime_s = 0;
    time_flag = 0;
    scralim_down_s = 0;
    scra_h_posit_s = 0;
    scra_v_posit_s = 0;
    scrb_h_posit_s = 0;
    scrb_v_posit_s = 0;
    scrc_h_posit_s = 0;
    scrc_v_posit_s = 0;
    scrz_h_posit_s = 0;
    scrz_v_posit_s = 0;
    waterposi_m_s = 0;
    water_flag_s = 0;
    waterflag_s = 0;
    plsubchg_flag = 0;
    markerno = 0;
    chibi_s = 0;
    prio_flag = 0;
    chibi_flag = 0;
    waterposi_m = 0;
    water_flag = 0;
    waterflag = 0;
    scralim_down = 0;
    memset(&plposiwkadr, 0, sizeof(plposiwkadr));
    memset(&scrz_v_posit, 0, sizeof(scrz_v_posit));
    memset(&scrz_h_posit, 0, sizeof(scrz_h_posit));
    memset(&scrc_v_posit, 0, sizeof(scrc_v_posit));
    memset(&scrc_h_posit, 0, sizeof(scrc_h_posit));
    memset(&scrb_v_posit, 0, sizeof(scrb_v_posit));
    memset(&scrb_h_posit, 0, sizeof(scrb_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&swdata1, 0, sizeof(swdata1));
    memset(playposiwk, 0, sizeof(playposiwk));
    memset(&editmode, 0, sizeof(editmode));
    memset(&pltime, 0, sizeof(pltime));
    memset(&stageno, 0, sizeof(stageno));

    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 128;
    sinset_cos = 256;
    emycol_d_count = 0;
    emycol_d_result = 0;
    ridechk_count = 0;
    ridechk_platform = 0;
    ridechk_player = 0;
}

static void test_patchg_control_bytes(test_context *ctx) {
    Uint8 normal[] = {2, 33, 255};
    Uint8 reset[] = {1, 7, 255};
    Uint8 rewind[] = {1, 4, 5, 254, 2, 255};
    Uint8 set_mst[] = {1, 253, 3, 255};
    Uint8 next_rno[] = {1, 252, 255};
    Uint8 clear_rno1[] = {1, 251, 255};
    Uint8 next_rno1[] = {1, 250, 255};
    Uint8 zero_delay[] = {0, 9, 255};
    Uint8 end_252[] = {3, 2, 252};
    Uint8 *tables[] = {normal, reset, rewind, set_mst, next_rno, clear_rno1,
                       next_rno1, zero_delay, end_252};

    reset_playsub_state();
    actwk[1].mstno.b.h = 1;
    actwk[1].mstno.b.l = 0;
    actwk[1].pattim = 5;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].pattim);

    reset_playsub_state();
    actwk[1].pattim = 2;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].pattim);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patcnt);

    reset_playsub_state();
    actwk[1].cddat = 3;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].pattim);

    reset_playsub_state();
    actwk[1].mstno.b.h = 1;
    actwk[1].mstno.b.l = 1;
    actwk[1].patcnt = 1;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patcnt);

    reset_playsub_state();
    actwk[1].mstno.b.h = 2;
    actwk[1].mstno.b.l = 2;
    actwk[1].patcnt = 2;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patcnt);

    reset_playsub_state();
    actwk[1].mstno.b.h = 3;
    actwk[1].mstno.b.l = 3;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].mstno.b.h);

    reset_playsub_state();
    actwk[1].mstno.b.h = 4;
    actwk[1].mstno.b.l = 4;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_playsub_state();
    actwk[1].mstno.b.h = 5;
    actwk[1].mstno.b.l = 5;
    actwk[1].r_no1 = 5;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no1);

    reset_playsub_state();
    actwk[1].mstno.b.h = 6;
    actwk[1].mstno.b.l = 6;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no1);

    reset_playsub_state();
    actwk[1].mstno.b.h = 7;
    actwk[1].mstno.b.l = 7;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].pattim);

    reset_playsub_state();
    actwk[1].mstno.b.h = 8;
    actwk[1].mstno.b.l = 8;
    patchg(&actwk[1], tables);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].pattim);
}

static void test_playsave0_captures_state_and_caps_time(test_context *ctx) {
    reset_playsub_state();
    plflag = 2;
    actwk[0].xposi.w.h = 123;
    actwk[0].yposi.w.h = 456;
    water_flag = 3;
    scralim_down = 111;
    scra_h_posit.w.h = 10;
    scra_v_posit.w.h = 20;
    scrb_h_posit.w.h = 30;
    scrb_v_posit.w.h = 40;
    scrc_h_posit.w.h = 50;
    scrc_v_posit.w.h = 60;
    scrz_h_posit.w.h = 70;
    scrz_v_posit.w.h = 80;
    waterposi_m = 90;
    waterflag = 4;
    pltime.l = 327681;
    chibi_flag = 5;

    playsave0(&actwk[0]);

    TEST_ASSERT_EQ_INT(ctx, 2, plflag_s);
    TEST_ASSERT_EQ_INT(ctx, 123, plxposi_s);
    TEST_ASSERT_EQ_INT(ctx, 456, plyposi_s);
    TEST_ASSERT_EQ_INT(ctx, 3, water_flag_s);
    TEST_ASSERT_EQ_INT(ctx, 111, scralim_down_s);
    TEST_ASSERT_EQ_INT(ctx, 10, scra_h_posit_s);
    TEST_ASSERT_EQ_INT(ctx, 80, scrz_v_posit_s);
    TEST_ASSERT_EQ_INT(ctx, 90, waterposi_m_s);
    TEST_ASSERT_EQ_INT(ctx, 4, waterflag_s);
    TEST_ASSERT_EQ_INT(ctx, 327680, pltime_s);
    TEST_ASSERT_EQ_INT(ctx, 5, chibi_s);

    pltime.l = 1234;
    playsave0(&actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1234, pltime_s);
}

static void test_marker_paths(test_context *ctx) {
    reset_playsub_state();
    markerno = 2;
    actwk[1].userflag.b.h = 1;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    queue_actor(&actwk[40]);
    marker(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, marker_get_work(&actwk[1])->activated);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 168, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_playsub_state();
    actwk[1].userflag.b.h = 3;
    marker_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    actwk[1].userflag.b.h = 4;
    marker_get_work(&actwk[1])->activated = 1;
    marker_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    marker_get_work(&actwk[1])->activated = 0;
    marker_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, plflag);
    actwk[1].colicnt = 1;
    marker_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, marker_get_work(&actwk[1])->activated);
    TEST_ASSERT_EQ_INT(ctx, 4, markerno);
    TEST_ASSERT_EQ_INT(ctx, 1, plflag);
    TEST_ASSERT_EQ_INT(ctx, 174, soundset_requests[0]);

    reset_playsub_state();
    actwk[1].userflag.b.h = 1;
    actwk[1].xposi.w.h = 50;
    actwk[1].yposi.w.h = 100;
    queue_actor(&actwk[40]);
    marker_init(&actwk[1]);
    marker_move1(&actwk[40]);
    TEST_ASSERT_EQ_INT(ctx, 0, marker_get_work(&actwk[40])->activated);
    marker_get_work(&actwk[1])->activated = 1;
    sinset_sin = 128;
    sinset_cos = 0;
    marker_move1(&actwk[40]);
    TEST_ASSERT_EQ_INT(ctx, 1, marker_get_work(&actwk[40])->activated);
    TEST_ASSERT_EQ_INT(ctx, 8, marker_get_work(&actwk[40])->angle);
    TEST_ASSERT_EQ_INT(ctx, 54, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 76, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_angle);
    marker_get_work(&actwk[40])->angle = 248;
    marker_move1(&actwk[40]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[40].r_no0);

    reset_playsub_state();
    marker_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patno);

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    actwk[1].colicnt = 1;
    marker(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, marker_get_work(&actwk[1])->activated);

    reset_playsub_state();
    actwk[1].r_no0 = 4;
    marker_get_work(&actwk[1])->activated = 1;
    marker(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, marker_get_work(&actwk[1])->angle);

    reset_playsub_state();
    actwk[1].r_no0 = 6;
    marker(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patno);
}

static void test_test_act_tensuu_and_bakuha(test_context *ctx) {
    reset_playsub_state();
    actwk[1].cddat = 128;
    test_act(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    scra_h_posit.w.h = 128;
    test_act(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    scra_h_posit.w.h = 128;
    test_act_get_work(&actwk[1])->screen_x_swapped = 1024;
    test_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    actwk[1].xposi.w.h = 300;
    actwk[1].yposi.w.h = 400;
    actwk[1].r_no1 = 1;
    tensuu_set(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    queue_actor(&actwk[40]);
    tensuu0(&actwk[1], 3);
    TEST_ASSERT_EQ_INT(ctx, 28, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -125, actwk[40].userflag.b.h);

    reset_playsub_state();
    actwk[1].userflag.b.l = 1;
    actwk[1].userflag.b.h = 1;
    bakuha(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1664, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    actwk[1].r_no0 = 4;
    bakuha(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 31, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);
    actwk[1].r_no0 = 4;
    actwk[1].r_no1 = 1;
    baku_die(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_flower_paths(test_context *ctx) {
    reset_playsub_state();
    time_flag = 2;
    actwk[1].cdsts = 4;
    TEST_ASSERT_TRUE(ctx, flow_sub(&actwk[1]) == &flagwork[14]);
    TEST_ASSERT_EQ_INT(ctx, 129, flow_sub1(65));

    reset_playsub_state();
    time_flag = 1;
    actwk[1].cdsts = 2;
    flagwork[7] = 64;
    flow_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].mstno.b.h);

    reset_playsub_state();
    actwk[1].userflag.b.h = 1;
    actwk[1].userflag.b.l = 1;
    flow_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1751, actwk[1].sproffset);

    reset_playsub_state();
    time_flag = 1;
    emycol_d_result = -6;
    actwk[1].xposi.w.h = 444;
    actwk[1].yposi.w.h = 500;
    flow_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 494, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[1].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 444, flowerposi[64].w.h);
    TEST_ASSERT_EQ_INT(ctx, 494, flowerposi[64].w.l);

    reset_playsub_state();
    flow_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].yposi.w.h);

    reset_playsub_state();
    flow_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 9943, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].mstno.b.h);
    flow_move3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].mstno.b.h);
}

static void test_mizuki_futa_exit2_and_collision_helpers(test_context *ctx) {
    reset_playsub_state();
    mizukiri(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 926, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    time_flag = 1;
    mizuki_init(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 996, actwk[2].sproffset);
    mizuki_move2(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    actwk[1].sprhsize = 10;
    actwk[1].sprvsize = 20;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 1, col_chk(&actwk[1], &actwk[0]));
    actwk[0].xposi.w.h = 80;
    TEST_ASSERT_EQ_INT(ctx, 0, col_chk(&actwk[1], &actwk[0]));
    actwk[0].xposi.w.h = 120;
    TEST_ASSERT_EQ_INT(ctx, 0, col_chk(&actwk[1], &actwk[0]));
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 160;
    TEST_ASSERT_EQ_INT(ctx, 0, col_chk(&actwk[1], &actwk[0]));
    actwk[0].yposi.w.h = 240;
    TEST_ASSERT_EQ_INT(ctx, 0, col_chk(&actwk[1], &actwk[0]));

    reset_playsub_state();
    actwk[1].userflag.b.h = 2;
    futa(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[1].sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_playsub_state();
    actwk[1].sprhsize = 16;
    actwk[1].sprvsize = 8;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].yspeed.w = -1;
    queue_actor(&actwk[40]);
    ride_on_chk_f(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 164, soundset_requests[0]);
    actwk[1].r_no0 = 6;
    futa_move3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.l);

    reset_playsub_state();
    exit2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 162, soundset_requests[0]);
    actwk[2].userflag.b.h = 2;
    exit2_init(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 161, soundset_requests[1]);
    exit2_erase(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    exit2_set(&actwk[2]);
}

static void test_baria_and_ball_paths(test_context *ctx) {
    reset_playsub_state();
    bariainit(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1348, actwk[1].sproffset);
    actwk[2].mstno.b.h = 3;
    bariainit(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[2].r_no0);
    actwk[3].mstno.b.h = 5;
    bariainit(&actwk[3]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[3].r_no0);

    reset_playsub_state();
    bariamove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    plpower_b = 1;
    plpower_a = 1;
    bariamove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    plpower_b = 1;
    stageno.b.h = 6;
    prio_flag = 1;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].cddat = 3;
    actwk[1].r_no0 = 2;
    bariamove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].cddat);
    TEST_ASSERT_EQ_INT(ctx, 130, plsubchg_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    plpower_m = 1;
    plposiwkadr.w = 4;
    playposiwk[0] = 111;
    playposiwk[1] = 222;
    actwk[1].r_no0 = 4;
    actwk[1].mstno.b.h = 1;
    mutekimove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 111, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 222, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 132, plsubchg_flag);
    plpower_m = 0;
    mutekimove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    plpower_a = 1;
    plposiwkadr.w = 4;
    playposiwk[0] = 333;
    playposiwk[1] = 444;
    actwk[1].r_no0 = 6;
    actwk[1].mstno.b.h = 5;
    backtomove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 333, actwk[1].xposi.w.h);
    plsubchg_flag = 8;
    actionsub0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    actwk[0].sproffset = 32768;
    baria(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[1].sproffset & 32768) != 0);
    actwk[0].sproffset = 0;
    stageno.b.h = 6;
    baria(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[1].sproffset & 32768) != 0);

    reset_playsub_state();
    ball(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_playsub_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].xspeed.w = -100;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -2560, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -2560, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 105, actwk[0].yposi.w.h);

    reset_playsub_state();
    stageno.b.h = 5;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].xspeed.w = 100;
    actwk[0].direc.b.h = 96;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3328, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -3328, actwk[0].mspeed.w);

    reset_playsub_state();
    actwk[1].userflag.b.h = 1;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -100;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -3328, actwk[0].yspeed.w);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 2) != 0);

    reset_playsub_state();
    editmode.b.h = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, ball_coli(&actwk[1]));
    editmode.b.h = 0;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 60;
    actwk[0].yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 1, ball_coli(&actwk[1]));
    actwk[0].xposi.w.h = 59;
    TEST_ASSERT_EQ_INT(ctx, 0, ball_coli(&actwk[1]));
    actwk[0].xposi.w.h = 140;
    TEST_ASSERT_EQ_INT(ctx, 0, ball_coli(&actwk[1]));
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 59;
    TEST_ASSERT_EQ_INT(ctx, 0, ball_coli(&actwk[1]));
    actwk[0].yposi.w.h = 140;
    TEST_ASSERT_EQ_INT(ctx, 0, ball_coli(&actwk[1]));
}

static void test_ball_rd7_paths(test_context *ctx) {
    reset_playsub_state();
    actwk[1].userflag.b.h = 2;
    actwk[0].yspeed.w = 0;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);

    reset_playsub_state();
    actwk[1].userflag.b.h = 2;
    actwk[0].yspeed.w = -1;
    swdata1.b.h = 4;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -3328, actwk[0].xspeed.w);

    reset_playsub_state();
    actwk[1].userflag.b.h = 3;
    actwk[0].yspeed.w = 1;
    swdata1.b.h = 8;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3328, actwk[0].xspeed.w);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 2) != 0);

    reset_playsub_state();
    actwk[1].userflag.b.h = 3;
    actwk[0].yspeed.w = -1;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);

    reset_playsub_state();
    actwk[1].userflag.b.h = 4;
    actwk[0].xspeed.w = -1;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);

    reset_playsub_state();
    actwk[1].userflag.b.h = 4;
    actwk[0].xspeed.w = 1;
    swdata1.b.h = 1;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, -2560, actwk[0].yspeed.w);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 2) != 0);
}

static void test_remaining_state_dispatch_paths(test_context *ctx) {
    reset_playsub_state();
    plairset();

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    scra_h_posit.w.h = 128;
    test_act(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    bakuha(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_playsub_state();
    flower(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 2;
    flower(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 4;
    flower(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 6;
    flower(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 9943, actwk[1].sproffset);
    reset_playsub_state();
    actwk[1].r_no0 = 8;
    flower(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_playsub_state();
    mizukiri(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    reset_playsub_state();
    actwk[1].r_no0 = 2;
    mizukiri(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 4;
    mizukiri(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    futa(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    reset_playsub_state();
    actwk[1].r_no0 = 4;
    futa(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 6;
    futa(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    reset_playsub_state();
    futa_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].patno);

    reset_playsub_state();
    actwk[1].sprhsize = 16;
    actwk[1].sprvsize = 8;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 200;
    actwk[0].yspeed.w = -1;
    ride_on_chk_f(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    reset_playsub_state();
    actwk[1].sprhsize = 16;
    actwk[1].sprvsize = 8;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].userflag.b.h = 1;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].yspeed.w = -1;
    ride_on_chk_f(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    reset_playsub_state();
    actwk[1].sprhsize = 16;
    actwk[1].sprvsize = 8;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].yspeed.w = -1;
    ride_on_chk_f(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    exit2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    reset_playsub_state();
    actwk[1].r_no0 = 4;
    exit2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_playsub_state();
    actwk[1].r_no0 = 4;
    plpower_a = 1;
    baria(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    reset_playsub_state();
    actwk[1].r_no0 = 6;
    baria(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    reset_playsub_state();
    actwk[1].r_no0 = 6;
    backtomove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    reset_playsub_state();
    stageno.b.h = 6;
    prio_flag = 1;
    plposiwkadr.w = 4;
    playposiwk[0] = 777;
    playposiwk[1] = 888;
    actwk[1].mstno.b.h = 1;
    muteki_get_work(&actwk[1])->history_offset = 20;
    muteki_sub(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, muteki_get_work(&actwk[1])->history_offset);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].sproffset);

    reset_playsub_state();
    actwk[1].r_no0 = 2;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    ball(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    reset_playsub_state();
    actwk[0].mstno.b.h = 43;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    reset_playsub_state();
    actwk[0].r_no0 = 6;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    reset_playsub_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 100;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    reset_playsub_state();
    actwk[1].userflag.b.h = 2;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -1;
    swdata1.b.h = 8;
    ball_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 3328, actwk[0].xspeed.w);
    reset_playsub_state();
    actwk[0].cddat = 4;
    ball_ball();
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sprvsize);

    reset_playsub_state();
    actwk[1].userflag.b.h = 2;
    actwk[0].yspeed.w = -1;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);
    reset_playsub_state();
    actwk[1].userflag.b.h = 4;
    actwk[0].xspeed.w = 1;
    ball_rd7(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, (actwk[0].cddat & 4) != 0);
}

TEST_MAIN_BEGIN;
    test_patchg_control_bytes(&ctx);
    test_playsave0_captures_state_and_caps_time(&ctx);
    test_marker_paths(&ctx);
    test_test_act_tensuu_and_bakuha(&ctx);
    test_flower_paths(&ctx);
    test_mizuki_futa_exit2_and_collision_helpers(&ctx);
    test_baria_and_ball_paths(&ctx);
    test_ball_rd7_paths(&ctx);
    test_remaining_state_dispatch_paths(&ctx);
TEST_MAIN_END

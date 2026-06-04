#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
Uint8 ta_flag;
Uint8 pl_suu;
Uint8 plring_f2;
Uint8 pl_suu_f;
Uint8 plring_f;
Sint16 plring;
Uint8 plpower_b;
Uint8 plpower_m;
Uint8 plpower_s;
Uint8 conbine_flag;
Sint16 time_stop;
Uint8 prio_flag;
Sint16 plmaxspdwk;
Sint16 pladdspdwk;
Sint16 plretspdwk;
char time_item;
Sint16 backto_cnt;
short_union gametimer;
short_union stageno;
Uint8 time_flag;
Uint8 flagwork[766];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_platform;
static sprite_status *ride_on_chk_player;
static int speedset_count;
static sprite_status *speedset_actor;
static int speedset2_count;
static sprite_status *speedset2_actor;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[16];
static int sub_sync_count;
static Sint16 sub_sync_requests[16];

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
void speedset(sprite_status *pActwk);
void speedset2(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void sub_sync(Sint16 ReqNo);

#include "src/item.c"

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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_platform = pActwk;
    ride_on_chk_player = pPlayerwk;
    return 0;
}

void speedset(sprite_status *pActwk) {
    ++speedset_count;
    speedset_actor = pActwk;
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

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 16) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_item_state(void) {
    memset(actwk, 0, sizeof(actwk));
    ta_flag = 0;
    pl_suu = 0;
    plring_f2 = 0;
    pl_suu_f = 0;
    plring_f = 0;
    plring = 0;
    plpower_b = 0;
    plpower_m = 0;
    plpower_s = 0;
    conbine_flag = 0;
    time_stop = 0;
    prio_flag = 0;
    plmaxspdwk = 0;
    pladdspdwk = 0;
    plretspdwk = 0;
    time_item = 0;
    backto_cnt = 0;
    memset(&gametimer, 0, sizeof(gametimer));
    memset(&stageno, 0, sizeof(stageno));
    time_flag = 0;
    memset(flagwork, 0, sizeof(flagwork));

    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ride_on_chk_count = 0;
    ride_on_chk_platform = 0;
    ride_on_chk_player = 0;
    speedset_count = 0;
    speedset_actor = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, itemchg[0] == itemchg0);
    TEST_ASSERT_TRUE(ctx, itemchg[9] == itemchg9);
    TEST_ASSERT_TRUE(ctx, itempat[0] == &ite00);
    TEST_ASSERT_TRUE(ctx, itempat[19] == &ite13);
    TEST_ASSERT_TRUE(ctx, corepat[0] == &cor00);
    TEST_ASSERT_TRUE(ctx, corepat[15] == &cor0f);
    TEST_ASSERT_EQ_INT(ctx, 272, ite00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 287, ite11.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1, itemchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, itemchg9[9]);
}

static void test_timedisp_and_time_item_paths(test_context *ctx) {
    reset_item_state();
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 19, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 196, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 338, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_item_state();
    time_item = -1;
    backto_cnt = 89;
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 18, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    time_item = -1;
    actwk[1].r_no0 = 2;
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 18, actwk[1].patno);

    reset_item_state();
    time_item = 1;
    backto_cnt = 90;
    gametimer.b.l = 0;
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    time_item = 1;
    backto_cnt = 90;
    gametimer.b.l = 2;
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_item_state();
    actwk[1].r_no0 = 4;
    time_item = 1;
    gametimer.b.l = 0;
    timedisp(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].patno);

    reset_item_state();
    ta_flag = 1;
    t_item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_item_state();
    actwk[1].userflag.b.h = 10;
    t_item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_t_item_init_flag_and_collection_paths(test_context *ctx) {
    reset_item_state();
    stageno.b.h = 6;
    actwk[1].userflag.b.h = 8;
    actwk[1].userflag.b.l = 0;
    actwk[1].cdsts = 2;
    t_item_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32768, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 223, actwk[1].colino);

    reset_item_state();
    actwk[1].userflag.b.h = 9;
    actwk[1].cdsts = 1;
    flagwork[3] = 1;
    t_item_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[1].patno);

    reset_item_state();
    actwk[1].colicnt = 0;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 0;
    actwk[1].userflag.b.l = 1;
    actwk[1].colicnt = 1;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 1;
    actwk[1].userflag.b.l = 0;
    actwk[1].colicnt = 1;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_item_state();
    time_flag = 1;
    actwk[1].cdsts = 2;
    actwk[1].userflag.b.h = 8;
    actwk[1].colicnt = 1;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, time_item_get_work(&actwk[1])->collection_timer);
    TEST_ASSERT_EQ_INT(ctx, -1, time_item);
    TEST_ASSERT_EQ_INT(ctx, 119, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[7]);

    reset_item_state();
    actwk[1].userflag.b.h = 9;
    actwk[1].colicnt = 1;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, time_item);
    TEST_ASSERT_EQ_INT(ctx, 118, sub_sync_requests[0]);

    reset_item_state();
    time_item_get_work(&actwk[1])->collection_timer = 2;
    t_item_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, time_item_get_work(&actwk[1])->collection_timer);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_item_state();
    time_item_get_work(&actwk[1])->collection_timer = 1;
    actwk[1].userflag.b.h = 9;
    t_item_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[1].patno);

    reset_item_state();
    t_item_move2(&actwk[1]);

    reset_item_state();
    actwk[1].userflag.b.h = 8;
    t_item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 0;
    actwk[1].userflag.b.h = 9;
    actwk[1].userflag.b.l = 0;
    actwk[1].colicnt = 1;
    t_item_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_item_state();
    actwk[1].userflag.b.h = 8;
    actwk[1].r_no0 = 2;
    actwk[1].colicnt = 1;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_item_state();
    actwk[1].userflag.b.h = 8;
    actwk[1].r_no0 = 4;
    time_item_get_work(&actwk[1])->collection_timer = 1;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);

    reset_item_state();
    actwk[1].userflag.b.h = 8;
    actwk[1].r_no0 = 6;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_flagwkadr_and_ride_gate_paths(test_context *ctx) {
    reset_item_state();
    actwk[1].cdsts = 4;
    time_flag = 2;
    TEST_ASSERT_EQ_INT(ctx, 14, flagwkadr(&actwk[1]));

    time_flag = 128;
    time_item = 1;
    TEST_ASSERT_EQ_INT(ctx, 12, flagwkadr(&actwk[1]));

    time_flag = 128;
    time_item = -5;
    TEST_ASSERT_EQ_INT(ctx, 14, flagwkadr(&actwk[1]));

    time_flag = 128;
    time_item = 5;
    TEST_ASSERT_EQ_INT(ctx, 12, flagwkadr(&actwk[1]));

    reset_item_state();
    ride_on_chk_i(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 1;
    actwk[1].userflag.b.l = 0;
    ride_on_chk_i(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 0;
    actwk[1].userflag.b.l = 1;
    ride_on_chk_i(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);

    reset_item_state();
    stageno.b.h = 6;
    prio_flag = 0;
    actwk[1].userflag.b.l = 0;
    ride_on_chk_i(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_item_init_move_and_get_paths(test_context *ctx) {
    reset_item_state();
    ta_flag = 1;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_item_state();
    stageno.b.h = 6;
    actwk[1].userflag.b.l = 0;
    iteminit(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32768, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 70, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[1].patno);

    reset_item_state();
    flagwork[0] = 1;
    iteminit(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 17, actwk[1].patno);

    reset_item_state();
    actwk[1].patbase = corepat;
    itemmove(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == itempat);

    reset_item_state();
    actwk[1].patbase = itempat;
    actwk[1].actflg = 0;
    itemmove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_item_state();
    actwk[1].patbase = itempat;
    actwk[1].actflg = 128;
    actwk[1].r_no1 = 1;
    emycol_d_result = -3;
    itemmove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, -3, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_item_state();
    actwk[1].patbase = itempat;
    actwk[1].actflg = 128;
    actwk[1].r_no1 = 1;
    emycol_d_result = 0;
    itemmove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_item_state();
    actwk[1].patbase = itempat;
    actwk[1].actflg = 128;
    itemmove(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_item_state();
    time_stop = 1;
    itemmove2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_item_state();
    actwk[1].r_no0 = 2;
    actwk[1].patbase = itempat;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    actwk[1].r_no0 = 6;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    actwk[1].r_no0 = 8;
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    actwk[1].r_no0 = 4;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].mstno.b.h = 3;
    actwk[1].userflag.b.l = 1;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    item(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 150, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 26, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[41].actno);
    TEST_ASSERT_EQ_INT(ctx, 17, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[0]);

    reset_item_state();
    itemget(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
}

static void test_item2_powerup_cases(test_context *ctx) {
    reset_item_state();
    actwk[1].mstno.b.h = 0;
    item2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, -744, actwk[1].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_item_state();
    actwk[1].r_no0 = 2;
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 0;
    item2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 29, actwk[1].pattim);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 122, sub_sync_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 1;
    plring = 90;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 100, plring);
    TEST_ASSERT_EQ_INT(ctx, 2, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 122, sub_sync_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 1;
    plring = 100;
    plring_f2 = 2;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 110, plring);
    TEST_ASSERT_EQ_INT(ctx, 149, soundset_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 1;
    plring = 190;
    plring_f2 = 2;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 200, plring);
    TEST_ASSERT_EQ_INT(ctx, 6, plring_f2);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 2;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_b);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[6].actno);
    TEST_ASSERT_EQ_INT(ctx, 151, soundset_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 3;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[8].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[8].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 109, sub_sync_requests[1]);

    reset_item_state();
    time_flag = 1;
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 4;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 3072, plmaxspdwk);
    TEST_ASSERT_EQ_INT(ctx, 24, pladdspdwk);
    TEST_ASSERT_EQ_INT(ctx, 128, plretspdwk);
    TEST_ASSERT_EQ_INT(ctx, 108, sub_sync_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 4;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 108, sub_sync_requests[1]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 5;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 300, time_stop);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 6;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, conbine_flag);
    TEST_ASSERT_EQ_INT(ctx, 157, soundset_requests[0]);

    reset_item_state();
    actwk[1].yspeed.w = 0;
    actwk[1].mstno.b.h = 9;
    item2move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_b);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 1, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[6].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[8].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[11].actno);

    reset_item_state();
    actwk[1].r_no0 = 4;
    actwk[1].pattim = 1;
    item2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    actwk[1].pattim = 0;
    item2die(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_item_state();
    actwk[1].r_no0 = 0;
    actwk[1].mstno.w = 7;
    powerup_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].mstno.w);

    reset_item_state();
    actwk[1].r_no0 = 6;
    actwk[1].patno = 11;
    item2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_tables_capture_literal_data(&ctx);
    test_timedisp_and_time_item_paths(&ctx);
    test_t_item_init_flag_and_collection_paths(&ctx);
    test_flagwkadr_and_ride_gate_paths(&ctx);
    test_item_init_move_and_get_paths(&ctx);
    test_item2_powerup_cases(&ctx);
TEST_MAIN_END

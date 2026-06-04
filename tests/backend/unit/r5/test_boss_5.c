#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;
Uint8 bossstart;
Uint8 generate_flag;
Sint16 scra_vline;
Sint16 scralim_left;
Sint16 scralim_n_left;
Sint16 scralim_right;
Sint16 scralim_n_right;
Sint16 scralim_down;
Sint16 scralim_n_down;
int_union scra_h_posit;
int_union scra_v_posit;
Sint16 scr_dir_tbl[6] = {4, 0, 3735, 0, 800, 96};
static PALETTEENTRY colorwk[64];
PALETTEENTRY *lpcolorwk = colorwk;
sprite_pattern *bakupat[7];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int colorset2_count;
static Sint32 colorset2_value;
static int soundset_count;
static Sint16 soundset_requests[16];
static int scoreup_count;
static Sint32 scoreup_value;
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int genecolor_count;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin = 0;
static Sint16 sinset_cos = 0;
static int divdevset_count;

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void colorset2(Sint32 ColorNo);
void soundset(Sint16 ReqNo);
void scoreup(Uint32 score);
void sub_sync(Sint16 ReqNo);
void genecolor(void);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void divdevset(void);

#include "src/r5/boss_5.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail_after >= 0 && actwkchk_count > actwkchk_fail_after) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = &actwk[next_alloc_index++];
    return 0;
}

void colorset2(Sint32 ColorNo) {
    ++colorset2_count;
    colorset2_value = ColorNo;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) /
                               sizeof(soundset_requests[0]))) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void scoreup(Uint32 score) {
    ++scoreup_count;
    scoreup_value = (Sint32)score;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < (int)(sizeof(sub_sync_requests) /
                               sizeof(sub_sync_requests[0]))) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void genecolor(void) { ++genecolor_count; }

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

void divdevset(void) { ++divdevset_count; }

static void reset_boss5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    bossflag = 0;
    bossstart = 0;
    generate_flag = 0;
    scra_vline = 0;
    scralim_left = scralim_n_left = 0;
    scralim_right = scralim_n_right = 0;
    scralim_down = scralim_n_down = 0;
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(colorwk, 0, sizeof(colorwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    next_alloc_index = 20;
    actwkchk_fail_after = -1;
    colorset2_count = 0;
    colorset2_value = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    scoreup_count = 0;
    scoreup_value = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    genecolor_count = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 0;
    sinset_cos = 0;
    divdevset_count = 0;
}

static void test_egg5_setup_wait_and_escape_paths(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_boss5_state();
    actor->xposi.w.h = 3600;
    actor->yposi.w.h = 640;
    actwk[0].yposi.w.h = 700;

    egg5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 700, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 55, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 5, egg5_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 3472, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 572, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss5_state();
    actor = &actwk[5];
    egg5_get_work(actor)->parent_index = 7;
    actor->r_no0 = 2;
    actwk[7].patno = 4;
    actwk[7].yposi.w.h = 616;

    egg5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 68, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 616, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == egg5_pchg);

    reset_boss5_state();
    actor = &actwk[5];
    egg5_get_work(actor)->parent_index = 7;
    actor->r_no0 = 2;
    egg5_get_work(actor)->flags = 4;
    actor->xposi.w.h = 3615;
    actor->yposi.w.h = 600;

    egg5(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 606, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 100, scoreup_value);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_meca1_setup_scroll_and_normal_paths(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_boss5_state();
    actor->xposi.w.h = 3536;
    actor->yposi.w.h = 584;
    scra_vline = 196;
    actwk[0].xposi.w.h = 3300;
    scralim_left = 3000;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8892, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5meca1_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 6, colorset2_value);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 52, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 6, egg5_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 20, egg5_get_work(actor)->child_index);
    TEST_ASSERT_EQ_INT(ctx, 640, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 202, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 3140, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 3140, scralim_n_left);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[6];
    actor->r_no0 = 2;
    egg5_get_work(actor)->flags = 2;
    actwk[0].xposi.w.h = 3424;
    scralim_left = 3000;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, bossstart);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 103, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 3264, scralim_left);

    reset_boss5_state();
    actor = &actwk[6];
    actor->r_no0 = 4;
    scralim_right = 3730;
    scralim_n_right = 3730;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, bossstart);
    TEST_ASSERT_EQ_INT(ctx, 3735, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 1, genecolor_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 26, sub_sync_requests[0]);

    reset_boss5_state();
    actor = &actwk[6];
    actor->r_no0 = 6;
    egg5_get_work(actor)->child_index = 8;
    egg5_get_work(&actwk[8])->flags = 128;
    scralim_down = 100;
    scralim_n_down = 100;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 7, egg5_get_work(actor)->palette_index);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(&actwk[8])->flags & 128);
    TEST_ASSERT_EQ_INT(ctx, 98, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 98, scralim_n_down);
}

static void test_meca2_setup_hit_and_down_cycle(test_context *ctx) {
    sprite_status *actor = &actwk[9];

    reset_boss5_state();
    actor->xposi.w.h = 3536;
    actor->yposi.w.h = 584;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 112, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8892, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5meca2_pat);
    TEST_ASSERT_EQ_INT(ctx, 62, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 53, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 54, actwk[22].actno);
    TEST_ASSERT_EQ_INT(ctx, 34, actwk[23].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 2;
    actor->colino = 0;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 172, soundset_requests[0]);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 4;
    egg5_get_work(actor)->timer = 59;
    actor->yposi.w.h = 648;
    egg5_get_work(actor)->child_index = 8;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(actor)->phase);
    TEST_ASSERT_EQ_INT(ctx, 128, egg5_get_work(actor)->flags & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 180, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, egg5_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[8].mstno.b.h);
}

static void test_meca2_motion_kezu_escape_and_end_paths(test_context *ctx) {
    sprite_status *actor = &actwk[9];

    reset_boss5_state();
    actor->r_no0 = 4;
    egg5_get_work(actor)->timer = 1;
    actor->yposi.w.h = 700;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 702, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 4;
    egg5_get_work(actor)->timer = 3;
    actor->yposi.w.h = 700;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 698, actor->yposi.w.h);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 6;
    egg5_get_work(actor)->phase = 1;
    actor->yposi.w.h = 639;
    egg5_get_work(actor)->target_y_inner = 640;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, egg5_get_work(actor)->phase);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 180, soundset_requests[0]);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 6;
    egg5_get_work(actor)->phase = 1;
    actor->yposi.w.h = 640;
    egg5_get_work(actor)->target_y_inner = 640;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 639, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(actor)->phase);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 6;
    egg5_get_work(actor)->phase = 2;
    actor->patno = 0;
    actor->yposi.w.h = 648;
    egg5_get_work(actor)->child_index = 8;
    egg5_get_work(actor)->target_y_outer = 648;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->phase);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 2, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 180, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 196, soundset_requests[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[8].mstno.b.h);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 8;
    actor->yposi.w.h = 600;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, egg5_get_work(actor)->flags & 4);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 10;
    egg5_get_work(actor)->timer = 17;
    actor->xposi.w.h = 3900;
    actor->yposi.w.h = 600;
    actor->patno = 2;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, egg5_get_work(&actwk[20])->parent_index);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 10;
    egg5_get_work(actor)->timer = 59;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(actor)->anim_timer);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 12;
    egg5_get_work(actor)->timer = 3;
    actor->xposi.w.h = 3000;
    actor->yposi.w.h = 620;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 8, egg5_get_work(actor)->flags & 8);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 2968, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 628, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 12;
    egg5_get_work(actor)->timer = 1;
    egg5_get_work(actor)->flags = 8;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->flags & 8);
}

static void test_bomb_launch_hit_and_bomb2_motion(test_context *ctx) {
    sprite_status *parent = &actwk[8];
    sprite_status *actor = &actwk[10];

    reset_boss5_state();
    parent->xposi.w.h = 3500;
    egg5_get_work(actor)->parent_index = 8;
    actor->xposi.w.h = 3450;
    actor->yposi.w.h = 650;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8892, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5bomb_pat);
    TEST_ASSERT_EQ_INT(ctx, 253, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 3500, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    parent->xposi.w.h = 3500;
    parent->r_no0 = 10;
    actor->r_no0 = 2;
    actor->yposi.w.h = 696;
    egg5_get_work(parent)->flags = 64;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(parent)->flags & 64);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 180, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    actor->colicnt = 3;
    actor->xposi.w.h = 3510;
    actor->yposi.w.h = 660;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 255, actor->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 3510, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 660, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 3500;
    actor->yposi.w.h = 650;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 253, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 3504, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 642, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(actor)->timer);

    egg5_get_work(actor)->timer = 3;
    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
}

static void test_bomb2_terminal_paths(test_context *ctx) {
    sprite_status *parent = &actwk[8];
    sprite_status *actor = &actwk[10];

    reset_boss5_state();
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actor->colicnt = 3;
    actor->xposi.w.h = 3400;
    actor->yposi.w.h = 650;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 24, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    egg5_get_work(parent)->flags = 4;
    actor->r_no0 = 4;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actor->colino = 0;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actor->colino = 253;
    actor->yposi.w.h = 711;
    egg5_get_work(actor)->velocity = 65536;
    actor->yspeed.w = 1;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_catch_wait_go_and_fire_paths(test_context *ctx) {
    sprite_status *core = &actwk[7];
    sprite_status *parent = &actwk[8];
    sprite_status *actor = &actwk[9];

    reset_boss5_state();
    egg5_get_work(parent)->parent_index = 7;
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    core = &actwk[7];
    parent = &actwk[8];
    actor = &actwk[9];
    egg5meca2(core);
    egg5_get_work(parent)->parent_index = 7;
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    egg5_get_work(actor)->timer = 179;
    actor->xposi.w.h = 3520;
    actor->yposi.w.h = 600;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 56, actwk[24].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, egg5_get_work(&actwk[24])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 203, soundset_requests[0]);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 8;
    actor->xposi.w.h = 3489;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3488, actor->xposi.w.h);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 10;
    actor->xposi.w.h = 3296;
    actwk[0].xposi.w.h = 3300;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 196, soundset_requests[0]);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[9];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 12;
    egg5_get_work(actor)->timer = 119;
    parent->patno = 1;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->patno);
}

static void test_belt_setup_speed_sync_and_palette_paths(test_context *ctx) {
    sprite_status *parent = &actwk[8];
    sprite_status *actor = &actwk[11];

    reset_boss5_state();
    egg5_get_work(actor)->parent_index = 8;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[42].peRed);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[42].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[42].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[43].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[43].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 192, colorwk[43].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[11];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 2;
    parent->r_no0 = 4;
    actwk[0].xposi.w.h = 3468;
    actwk[0].xspeed.w = 50;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3464, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[11];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    egg5_get_work(actor)->anim_timer = 1;
    egg5_get_work(actor)->belt_speed = 512;
    egg5_get_work(actor)->progress = 1400;
    actwk[0].cddat = 2;
    actwk[0].xspeed.w = 300;
    actwk[0].yspeed.w = 20;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, egg5_get_work(actor)->flags & 16);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 511, egg5_get_work(actor)->belt_speed);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[11];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    egg5_get_work(parent)->flags = 4;
    egg5_get_work(actor)->belt_speed = 16;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[11];
    generate_flag = 1;
    egg5_get_work(actor)->belt_speed = 600;
    egg5_get_work(actor)->timer = 3;

    belt_anime(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[42].peRed);
    TEST_ASSERT_EQ_INT(ctx, 128, colorwk[42].peGreen);
}

static void test_remaining_boss5_boundary_branches(test_context *ctx) {
    sprite_status *actor = &actwk[9];
    int i;

    reset_boss5_state();
    actor->r_no0 = 2;
    egg5_get_work(actor)->parent_index = 7;
    egg5_get_work(actor)->timer = 21;
    actwk[7].patno = 4;

    egg5(actor);

    TEST_ASSERT_EQ_INT(ctx, 20, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);

    reset_boss5_state();
    actor = &actwk[9];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actor->xposi.w.h = 3520;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 2;
    actor->colino = 62;
    egg5_get_work(actor)->anim_timer = 1;
    egg5_get_work(actor)->belt_actor_index = 12;
    egg5_get_work(&actwk[12])->belt_actor_index = 1300;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 3537, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(actor)->toggle & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 2;
    actor->colino = 62;
    egg5_get_work(actor)->anim_timer = 1;
    egg5_get_work(actor)->toggle = 1;
    egg5_get_work(actor)->belt_actor_index = 12;
    egg5_get_work(&actwk[12])->belt_actor_index = 1300;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 3535, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->toggle & 1);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 2;
    actor->colino = 62;
    egg5_get_work(actor)->anim_timer = 1;
    egg5_get_work(actor)->toggle = 1;
    egg5_get_work(actor)->belt_actor_index = 12;
    egg5_get_work(&actwk[12])->belt_actor_index = 1100;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 3535, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->toggle & 1);

    reset_boss5_state();
    actor = &actwk[9];
    actor->xposi.w.h = 3536;
    actor->yposi.w.h = 584;
    egg5meca2(actor);
    egg5_get_work(&actwk[23])->belt_actor_index = 1488;
    egg5_get_work(&actwk[23])->progress = 1488;
    for (i = 0; i < 600; ++i) {
        belt_hitpnt(9, &actwk[23]);
    }
    actor->r_no0 = 8;
    actor->patno = 1;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 13, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[24].actno);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 12;
    egg5_get_work(actor)->timer = 120;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, egg5_get_work(actor)->flags & 8);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actor->xposi.w.h = 3500;
    actor->yposi.w.h = 650;
    egg5_get_work(actor)->angle_high = 33;
    egg5_get_work(actor)->y_offset = -3;
    sinset_sin = 512;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 659, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 37, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 6, egg5_get_work(actor)->y_offset);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    egg5_get_work(&actwk[8])->flags = 4;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 8;
    egg5_get_work(actor)->timer = 7;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 57, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss5_state();
    actwkchk_fail_after = 1;
    make_bomb2(&actwk[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 57, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[21].actno);
}

static void test_belt_helper_boundaries(test_context *ctx) {
    sprite_status *actor = &actwk[11];

    reset_boss5_state();
    actwk[0].cddat = 0;
    actwk[0].xspeed.w = 100;
    egg5_get_work(actor)->belt_speed = 300;
    TEST_ASSERT_EQ_INT(ctx, 0, belt_spdset(actor));
    TEST_ASSERT_EQ_INT(ctx, 293, egg5_get_work(actor)->belt_speed);

    reset_boss5_state();
    actwk[0].cddat = 0;
    actwk[0].xspeed.w = 400;
    egg5_get_work(actor)->belt_speed = 300;
    TEST_ASSERT_EQ_INT(ctx, 0, belt_spdset(actor));
    TEST_ASSERT_EQ_INT(ctx, 308, egg5_get_work(actor)->belt_speed);

    reset_boss5_state();
    actwk[0].mstno.b.h = 24;
    egg5_get_work(actor)->belt_speed = 500;
    TEST_ASSERT_EQ_INT(ctx, -1, belt_spdset(actor));
    TEST_ASSERT_EQ_INT(ctx, 192, egg5_get_work(actor)->belt_speed);
    TEST_ASSERT_EQ_INT(ctx, -1, chk_belt_on());

    reset_boss5_state();
    actwk[0].cddat = 0;
    egg5_get_work(actor)->belt_speed = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, belt_spdset(actor));
    TEST_ASSERT_EQ_INT(ctx, 192, egg5_get_work(actor)->belt_speed);
    TEST_ASSERT_EQ_INT(ctx, 0, chk_belt_on());

    reset_boss5_state();
    actwk[0].cddat = 2;
    egg5_get_work(actor)->belt_speed = 1500;
    TEST_ASSERT_EQ_INT(ctx, 1, belt_spdset(actor));
    TEST_ASSERT_EQ_INT(ctx, 1488, egg5_get_work(actor)->belt_speed);

    reset_boss5_state();
    actwk[0].xposi.w.h = 3500;
    egg5_get_work(actor)->belt_speed = 1000;
    belt_sncspd(0, actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 3497, actwk[0].xposi.w.h);

    reset_boss5_state();
    actwk[0].xposi.w.h = 3500;
    actwk[0].xspeed.w = 100;
    egg5_get_work(actor)->belt_speed = 200;
    belt_sncspd(0, actor);
    TEST_ASSERT_EQ_INT(ctx, 3499, actwk[0].xposi.w.h);

    reset_boss5_state();
    actwk[0].xposi.w.h = 3500;
    egg5_get_work(&actwk[8])->flags = 0;
    belt_sncxmax(8);
    TEST_ASSERT_EQ_INT(ctx, 3464, actwk[0].xposi.w.h);

    reset_boss5_state();
    egg5_get_work(actor)->belt_speed = 1200;
    egg5_get_work(actor)->timer = 0;
    belt_anime(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);

    reset_boss5_state();
    egg5_get_work(actor)->belt_speed = 900;
    egg5_get_work(actor)->timer = 0;
    belt_anime(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->timer);

    reset_boss5_state();
    actor = &actwk[11];
    set_belt_col(actor);
    set_belt_col(actor);
    set_belt_col(actor);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[42].peRed);

    reset_boss5_state();
    actor = &actwk[9];
    egg5_get_work(actor)->timer = 1;
    bom_set(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_boss5_state();
    actor = &actwk[9];
    egg5_get_work(actor)->timer = 16;
    actor->xposi.w.h = 3000;
    actor->yposi.w.h = 620;
    bom_set(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, actwk[20].actno);
}

static void test_final_reachable_boss5_edges(test_context *ctx) {
    sprite_status *actor = &actwk[9];

    reset_boss5_state();
    actor->r_no0 = 2;
    scra_vline = 201;
    actwk[0].xposi.w.h = 3200;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 5, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 2, egg5_get_work(actor)->flags & 2);
    TEST_ASSERT_EQ_INT(ctx, 207, scra_vline);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 4;
    generate_flag = 1;
    scralim_right = 3730;
    scralim_n_right = 3730;

    egg5meca1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 25, sub_sync_requests[0]);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 6;
    actor->yposi.w.h = 640;
    egg5_get_work(actor)->target_y_outer = 648;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 641, actor->yposi.w.h);

    reset_boss5_state();
    actor = &actwk[9];
    actor->xposi.w.h = 3536;
    actor->yposi.w.h = 584;
    egg5meca2(actor);
    egg5_get_work(&actwk[23])->belt_actor_index = 1488;
    egg5_get_work(&actwk[23])->progress = 1488;
    belt_hitpnt(9, &actwk[23]);
    actor->r_no0 = 8;
    egg5_get_work(actor)->flags = 2;

    egg5meca2(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, egg5_get_work(actor)->flags & 2);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);

    reset_boss5_state();
    actor = &actwk[9];
    actor->r_no0 = 10;
    actor->xposi.w.h = 3400;
    actwk[0].xposi.w.h = 3300;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3399, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_boss5_state();
    actor = &actwk[10];
    actor->r_no0 = 2;
    actor->colicnt = 3;

    egg5bomb2(actor);

    TEST_ASSERT_EQ_INT(ctx, 24, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    actor->r_no0 = 2;
    actor->xposi.w.h = 3449;

    egg5hibana(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 8;
    actor->colicnt = 3;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 24, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[10];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 8;
    egg5_get_work(&actwk[8])->flags = 4;

    egg5bomb(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss5_state();
    actor = &actwk[11];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 4;
    actwk[0].xposi.w.h = 3464;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 192, egg5_get_work(actor)->belt_speed);

    reset_boss5_state();
    actor = &actwk[11];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 6;
    egg5_get_work(actor)->anim_timer = 0;
    egg5_get_work(actor)->belt_speed = 512;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[11];
    actor->r_no0 = 8;

    egg5belt(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_hibana_initializes_and_moves_left(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_boss5_state();
    actor->xposi.w.h = 3600;
    actor->yposi.w.h = 700;
    scra_h_posit.w.h = 3400;
    scra_v_posit.w.h = 600;

    egg5hibana(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8892, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5hibana_pat);
    TEST_ASSERT_EQ_INT(ctx, 3597, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == egg5hibana_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_hibana_variant_uses_gravity_and_frameout(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_boss5_state();
    actor->xposi.w.h = 3449;
    actor->yposi.w.h = 700;
    egg5_get_work(actor)->phase = 1;

    egg5hibana(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3446, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 698, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_bakuha_initializes_and_frameout_state_deletes(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss5_state();
    actor->yposi.w.h = 700;

    egg5bakuha(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1664, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == bakupat);
    TEST_ASSERT_EQ_INT(ctx, 697, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == egg5bakuha_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss5_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    egg5bakuha(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_meca3_and_pipe_follow_parent(test_context *ctx) {
    sprite_status *parent = &actwk[7];
    sprite_status *actor = &actwk[8];

    reset_boss5_state();
    parent->xposi.w.h = 3500;
    parent->yposi.w.h = 640;
    egg5_get_work(actor)->parent_index = 7;

    egg5meca3(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5meca3_pat);
    TEST_ASSERT_EQ_INT(ctx, 3500, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 640, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[7];
    actor = &actwk[8];
    parent->yposi.w.h = 704;
    actor->yposi.w.h = 640;
    egg5_get_work(actor)->parent_index = 7;

    egg5pipe(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5pipe_pat);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    egg5_get_work(parent)->flags = 8;
    egg5pipe(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_catch_return_and_fire_cycle(test_context *ctx) {
    sprite_status *core = &actwk[7];
    sprite_status *parent = &actwk[8];
    sprite_status *actor = &actwk[9];

    reset_boss5_state();
    egg5_get_work(parent)->parent_index = 7;
    egg5_get_work(actor)->parent_index = 8;
    actor->xposi.w.h = 3400;
    core->r_no0 = 0;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5catch_pat);
    TEST_ASSERT_EQ_INT(ctx, 3401, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == egg5catch_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    parent = &actwk[8];
    actor = &actwk[9];
    egg5_get_work(actor)->parent_index = 8;
    actor->r_no0 = 12;
    egg5_get_work(actor)->timer = 29;

    egg5catch(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 30, egg5_get_work(actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 64, egg5_get_work(actor)->flags & 64);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_make_hibana_helpers_spawn_exact_positions(test_context *ctx) {
    sprite_status *parent = &actwk[5];

    reset_boss5_state();
    make_hibana1();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 3560, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 696, actwk[20].yposi.w.h);

    reset_boss5_state();
    parent->xposi.w.h = 4000;
    parent->yposi.w.h = 600;
    parent->patno = 2;
    make_hibana2(parent);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, egg5_get_work(&actwk[20])->phase);
    TEST_ASSERT_EQ_INT(ctx, 5, egg5_get_work(&actwk[20])->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 3976, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 644, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4024, actwk[23].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 644, actwk[23].yposi.w.h);

    reset_boss5_state();
    actwkchk_fail_after = 2;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    make_hibana2(parent);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[22].actno);
}

static void test_egg5hari_setup_and_motion(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_boss5_state();
    actor->xposi.w.h = 3200;
    actor->yposi.w.h = 700;
    actwk[0].xposi.w.h = 3152;

    egg5hari(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17084, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg5hari_pat);
    TEST_ASSERT_EQ_INT(ctx, 191, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 1, divdevset_count);
    TEST_ASSERT_EQ_INT(ctx, 698, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss5_state();
    actor = &actwk[6];
    actor->r_no0 = 4;
    actor->yposi.w.h = 671;
    egg5hari(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 672, actor->yposi.w.h);

    actor->xposi.w.h = 3279;
    egg5hari(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3280, actor->xposi.w.h);
}

TEST_MAIN_BEGIN;
test_egg5_setup_wait_and_escape_paths(&ctx);
test_meca1_setup_scroll_and_normal_paths(&ctx);
test_meca2_setup_hit_and_down_cycle(&ctx);
test_meca2_motion_kezu_escape_and_end_paths(&ctx);
test_bomb_launch_hit_and_bomb2_motion(&ctx);
test_bomb2_terminal_paths(&ctx);
test_catch_wait_go_and_fire_paths(&ctx);
test_belt_setup_speed_sync_and_palette_paths(&ctx);
test_remaining_boss5_boundary_branches(&ctx);
test_belt_helper_boundaries(&ctx);
test_final_reachable_boss5_edges(&ctx);
test_hibana_initializes_and_moves_left(&ctx);
test_hibana_variant_uses_gravity_and_frameout(&ctx);
test_bakuha_initializes_and_frameout_state_deletes(&ctx);
test_meca3_and_pipe_follow_parent(&ctx);
test_catch_return_and_fire_cycle(&ctx);
test_make_hibana_helpers_spawn_exact_positions(&ctx);
test_egg5hari_setup_and_motion(&ctx);
TEST_MAIN_END

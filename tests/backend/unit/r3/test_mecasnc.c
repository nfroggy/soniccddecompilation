#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 ta_flag;
Uint8 emie3end;
Uint8 plpower_a;
Uint8 plpower_m;
PALETTEENTRY colorwk[64];
PALETTEENTRY *lpcolorwk = colorwk;
PALETTEENTRY zone3colora[16];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[8];
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int hitchk_count;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);
void sub_sync(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Uint8 CCset(Sint32 lSrc, Sint32 lDst);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);

#define R31A
#include "src/r3/msnccg.c"
#include "src/r3/mecasnc.c"

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

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8) {
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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    *sin = (kakudo & 128) ? -64 : 64;
    *cos = 0;
}

Uint8 CCset(Sint32 lSrc, Sint32 lDst) { return lSrc - lDst < 0; }

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pActwk;
    (void)pPlayerwk;
    ++hitchk_count;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pPlayerwk;
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    return 0;
}

Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    ++playdamageset_count;
    playdamageset_player = pActwk;
    playdamageset_actor = pColliAct;
    return -1;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_mecasnc_state(void) {
    memset(actwk, 0, sizeof(actwk));
    ta_flag = 0;
    emie3end = 0;
    plpower_a = 0;
    plpower_m = 0;
    memset(colorwk, 0, sizeof(colorwk));
    memset(zone3colora, 0, sizeof(zone3colora));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    hitchk_count = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
    jmp_flg = 0;
    bCarry = 0;
}

static void reset_callback_logs(void) {
    actionsub_count = 0;
    frameout_count = 0;
    frameout_s_count = 0;
    soundset_count = 0;
    sub_sync_count = 0;
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    patchg_count = 0;
    hitchk_count = 0;
    ride_on_clr_count = 0;
    playdamageset_count = 0;
}

static void assert_palette(test_context *ctx, PALETTEENTRY actual, int red,
                           int green, int blue, int flags) {
    TEST_ASSERT_EQ_INT(ctx, red, actual.peRed);
    TEST_ASSERT_EQ_INT(ctx, green, actual.peGreen);
    TEST_ASSERT_EQ_INT(ctx, blue, actual.peBlue);
    TEST_ASSERT_EQ_INT(ctx, flags, actual.peFlags);
}

static void test_mecasnc_tables_and_small_helpers(test_context *ctx) {
    reset_mecasnc_state();
    TEST_ASSERT_TRUE(ctx, msnc_tbl[0] == &mecasnc3_init);
    TEST_ASSERT_TRUE(ctx, msnc_tbl[5] == &mecasnc3_rght);
    TEST_ASSERT_TRUE(ctx, emie3_tbl[2] == &emie3_help);
    TEST_ASSERT_TRUE(ctx, hari_tbl[1] == &hari3x_move);
    TEST_ASSERT_TRUE(ctx, heart_tbl[1] == &heart3_move);
    TEST_ASSERT_TRUE(ctx, msfire_tbl[1] == &msnc3fire_move);
    TEST_ASSERT_TRUE(ctx, msc_pchg[0] == vatck_pchg);
    TEST_ASSERT_TRUE(ctx, msc_pchg[2] == hover_pchg);
    TEST_ASSERT_TRUE(ctx, fire_pchg[0] == fire_pchg0);
    TEST_ASSERT_EQ_INT(ctx, 0, CSset_b(10, 20));
    TEST_ASSERT_EQ_INT(ctx, 1, CSset_b(200, 55));
    TEST_ASSERT_EQ_INT(ctx, 512, spd_tbl[0]);
    TEST_ASSERT_EQ_INT(ctx, -1024, spd_tbl[7]);
}

static void test_mecasnc_entry_init_wait_and_fout(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_mecasnc_state();
    actor->r_no0 = 0;
    mecasnc3(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_mecasnc_state();
    mecasnc3_work_get(actor)->hover_angle = 1;
    mecasnc3_matu1(actor);

    reset_mecasnc_state();
    actor->actflg = 128;
    mecasnc3_matu1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 202, soundset_requests[0]);

    reset_mecasnc_state();
    actwk[0].xposi.w.h = 1216;
    zone3colora[0] = (PALETTEENTRY){11, 22, 33, 1};
    actor->r_no0 = 2;
    mecasnc3(actor);
    TEST_ASSERT_EQ_INT(ctx, 255, emie3end);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    assert_palette(ctx, colorwk[16], 11, 22, 33, 1);
}

static void test_mecasnc_motion_states(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_mecasnc_state();
    actor->xposi.l = 0x04000000;
    actor->xspeed.w = -400;
    mecasnc3_work_get(actor)->acceleration = -500;
    mecasnc3_come(actor);

    reset_mecasnc_state();
    actor->xposi.w.h = 990;
    actor->yposi.w.h = 77;
    queue_actor(&actwk[10]);
    mecasnc3_come(actor);
    TEST_ASSERT_EQ_INT(ctx, 52, actwk[10].actno);

    reset_mecasnc_state();
    actor->xposi.w.h = 930;
    mecasnc3_work_get(actor)->ride_actor_index = 5;
    actwk[5].xposi.w.h = 900;
    mecasnc3_come(actor);

    reset_mecasnc_state();
    actor->xposi.l = 0x01000000;
    actwk[0].xposi.l = 0x02000000;
    mecasnc3_work_get(actor)->base_y = 100;
    mecasnc3_work_get(actor)->hover_angle = 0;
    mecasnc3_hovr(actor);

    reset_mecasnc_state();
    mecasnc3_work_get(actor)->base_y = 100;
    mecasnc3_work_get(actor)->hover_angle = 128;
    mecasnc3_hovr(actor);

    reset_mecasnc_state();
    actor->xposi.l = 0x02000000;
    actor->yposi.l = 0x01000000;
    actor->yspeed.w = -256;
    mecasnc3_work_get(actor)->acceleration = 11;
    mecasnc3_work_get(actor)->lift_timer = 2;
    mecasnc3_upmv(actor);

    reset_mecasnc_state();
    actor->yposi.l = 0x01000000;
    actor->yspeed.w = -256;
    mecasnc3_work_get(actor)->acceleration = 11;
    mecasnc3_work_get(actor)->lift_timer = 1;
    mecasnc3_work_get(actor)->wait_timer = 4;
    mecasnc3_upmv(actor);

    reset_mecasnc_state();
    actor->xposi.w.h = 400;
    mecasnc3_work_get(actor)->wait_timer = 4;
    actwk[0].xposi.w.h = 200;
    mecasnc3_upmv(actor);

    reset_mecasnc_state();
    actor->xposi.w.h = 300;
    mecasnc3_work_get(actor)->wait_timer = 1;
    actor->cddat = 1;
    actwk[0].xposi.w.h = 200;
    mecasnc3_upmv(actor);

    reset_mecasnc_state();
    actor->xposi.l = 0x05000000;
    actor->xspeed.w = 800;
    mecasnc3_work_get(actor)->acceleration = 400;
    mecasnc3_rght(actor);

    reset_mecasnc_state();
    actor->xposi.w.h = 1320;
    mecasnc3_rght(actor);
    TEST_ASSERT_EQ_INT(ctx, 255, emie3end);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_emie_init_walk_help_and_palette(test_context *ctx) {
    sprite_status *emie = &actwk[1];

    reset_mecasnc_state();
    actwk[0].xposi.l = 0x02000000;
    emie->xposi.l = 0x01000000;
    emie3_init(emie);
    assert_palette(ctx, colorwk[18], 128, 32, 96, 1);

    reset_mecasnc_state();
    emie3_work_get(emie)->initialized_flag = 7;
    emie->xposi.l = 0x03000000;
    actwk[0].xposi.l = 0x02000000;
    emie3_init(emie);

    reset_mecasnc_state();
    ta_flag = 1;
    emie3(emie);
    TEST_ASSERT_EQ_INT(ctx, 255, emie3end);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    actwk[0].xposi.w.h = 100;
    emie3(emie);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == emie);

    reset_mecasnc_state();
    actwk[0].xposi.w.h = 1216;
    emie3(emie);
    TEST_ASSERT_EQ_INT(ctx, 255, emie3end);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    emie3end = 1;
    emie3(emie);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    emie->r_no0 = 2;
    mecasnc3_work_get(&actwk[4])->ready_flag = 1;
    emie3_work_get(emie)->ride_actor_index = 4;
    emie3_walk(emie);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 125, sub_sync_requests[0]);

    reset_mecasnc_state();
    emie->cddat = 1;
    emie->xposi.w.h = 120;
    emie->xspeed.w = 99;
    emie3_walk(emie);

    reset_mecasnc_state();
    emie->xposi.l = 0x03000000;
    emie->xspeed.w = -600;
    emie3_walk(emie);

    reset_mecasnc_state();
    actwk[0].xposi.w.h = 200;
    emie->xposi.w.h = 100;
    emie->xspeed.w = 0;
    emie3_walk(emie);

    reset_mecasnc_state();
    emie->xposi.w.h = 960;
    queue_actor(&actwk[10]);
    emie3_walk(emie);
    TEST_ASSERT_EQ_INT(ctx, 49, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1280, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[10].yposi.w.h);

    reset_mecasnc_state();
    emie3_work_get(emie)->ride_actor_index = 5;
    actwk[5].actno = 49;
    actwk[5].xposi.w.h = 300;
    actwk[5].yposi.w.h = 88;
    emie3_help(emie);

    reset_mecasnc_state();
    emie3_work_get(emie)->ride_actor_index = 5;
    actwk[5].actno = 49;
    actwk[5].cddat = 1;
    actwk[5].xposi.w.h = 300;
    emie3_help(emie);

    reset_mecasnc_state();
    emie3_work_get(emie)->ride_actor_index = 5;
    actwk[5].actno = 0;
    emie3_help(emie);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_animation_and_heart_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_mecasnc_state();
    actor->mstno.b.h = 1;
    actor->pattim = 0;
    actor->cddat = 1;
    empatchg(actor, em_pchg);

    actor->pattim = 3;
    actor->patno = 9;
    empatchg(actor, em_pchg);

    actor->mstno.b.h = 2;
    actor->pattim = 0;
    actor->patcnt = 4;
    empatchg(actor, hover_pchg == 0 ? em_pchg : msc_pchg);

    reset_mecasnc_state();
    actor->pattim = 0;
    actor->patcnt = 4;
    empatchg(actor, msc_pchg);

    reset_mecasnc_state();
    emie3_work_get(actor)->heart_timer = 240;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    heartset(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    emie3_work_get(actor)->heart_timer = 248;
    queue_actor(&actwk[10]);
    heartset(actor);
    TEST_ASSERT_EQ_INT(ctx, 51, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 112, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 38, actwk[10].yposi.w.h);

    reset_mecasnc_state();
    emie3_work_get(actor)->heart_timer = 248;
    heartset(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[10].actno);

    reset_mecasnc_state();
    emie3_work_get(actor)->heart_timer = 248;
    actor->cddat = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    queue_actor(&actwk[10]);
    heartset(actor);
    TEST_ASSERT_EQ_INT(ctx, 86, actwk[10].xposi.w.h);
}

static void test_hari3x_wrappers_init_break_and_ride_damage(test_context *ctx) {
    sprite_status *hari = &actwk[1];
    sprite_status *player = &actwk[0];

    reset_mecasnc_state();
    ta_flag = 1;
    hari3x(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_mecasnc_state();
    hari->r_no0 = 4;
    hari3x_work_get(hari)->timer = 2;
    hari3x(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hari3x_init(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 2;
    actwk[32].actno = 49;
    actwk[32].xposi.w.h = 100;
    actwk[32].yposi.w.h = 100;
    hari->xposi.w.h = 100;
    hari->yposi.w.h = 100;
    hari->actflg = 128;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    queue_actor(&actwk[12]);
    hari3x_init(hari);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[10].patno);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);

    reset_mecasnc_state();
    hari->userflag.b.h = 3;
    hari3x_init(hari);

    reset_mecasnc_state();
    hari->userflag.b.h = 3;
    brknset(hari);

    reset_mecasnc_state();
    hari3x_work_get(hari)->piece_index = 2;
    hari3x_init(hari);

    reset_mecasnc_state();
    hari3x_work_get(hari)->piece_index = 3;
    hari3x_spdset(hari);

    reset_mecasnc_state();
    hari->xposi.l = 0x01000000;
    hari->yposi.l = 0x02000000;
    hari->xspeed.w = 512;
    hari->yspeed.w = -512;
    hari3x_work_get(hari)->gravity = 96;
    hari3x_work_get(hari)->timer = 1;
    hari3x_brkn(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 0;
    hari3x_move(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hitchk_result = 1;
    hari->cddat = 8;
    player->r_no0 = 2;
    player->yposi.l = 0x02000000;
    player->yspeed.w = -256;
    hari3x_ridechk(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == hari);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hitchk_result = 1;
    hari->cddat = 0;
    hari3x_ridechk(hari);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hitchk_result = 1;
    hari->cddat = 8;
    plpower_a = 1;
    hari3x_ridechk(hari);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hitchk_result = 1;
    hari->cddat = 8;
    plpower_m = 1;
    hari3x_ridechk(hari);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_mecasnc_state();
    hari->userflag.b.h = 1;
    hitchk_result = 1;
    hari->cddat = 8;
    player->r_no0 = 2;
    player->yposi.l = 0x02000000;
    player->yspeed.w = 256;
    hari3x_ridechk(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);

    reset_mecasnc_state();
    emie3end = 1;
    hari3x_init(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_hari3x_move_bounds(test_context *ctx) {
    sprite_status *hari = &actwk[1];
    sprite_status *msnc = &actwk[32];

    reset_mecasnc_state();
    msnc->actno = 49;
    hari->sprhsize = 12;
    hari->sprvsize = 8;
    msnc->xposi.w.h = 40;
    hari->xposi.w.h = 100;
    hari3x_move(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_mecasnc_state();
    msnc->actno = 49;
    hari->sprhsize = 12;
    hari->sprvsize = 8;
    msnc->xposi.w.h = 200;
    hari->xposi.w.h = 100;
    hari3x_move(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_mecasnc_state();
    msnc->actno = 49;
    hari->sprhsize = 12;
    hari->sprvsize = 8;
    msnc->xposi.w.h = 100;
    hari->xposi.w.h = 100;
    msnc->yposi.w.h = 20;
    hari->yposi.w.h = 100;
    hari3x_move(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_mecasnc_state();
    msnc->actno = 49;
    hari->sprhsize = 12;
    hari->sprvsize = 8;
    msnc->xposi.w.h = 100;
    hari->xposi.w.h = 100;
    msnc->yposi.w.h = 200;
    hari->yposi.w.h = 100;
    hari3x_move(hari);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_heart_and_fire_actors(test_context *ctx) {
    sprite_status *heart = &actwk[1];
    sprite_status *fire = &actwk[2];

    reset_mecasnc_state();
    heart3(heart);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_mecasnc_state();
    heart3_work_get(heart)->timer = 19;
    heart->yspeed.w = -96;
    heart3_move(heart);

    reset_mecasnc_state();
    heart3_work_get(heart)->timer = 109;
    heart->yspeed.w = -96;
    heart3_move(heart);

    reset_mecasnc_state();
    heart3_work_get(heart)->timer = 119;
    heart3_work_get(heart)->no_wave = 255;
    heart3_move(heart);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    msnc3fire_init(fire);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_mecasnc_state();
    actwk[5].actno = 49;
    actwk[5].xposi.w.h = 400;
    actwk[5].yposi.w.h = 90;
    msnc3fire_work_get(fire)->parent_index = 5;
    msnc3fire_move(fire);
    TEST_ASSERT_TRUE(ctx, patchg_table == fire_pchg);

    reset_mecasnc_state();
    actwk[5].actno = 49;
    actwk[5].cddat = 1;
    actwk[5].xposi.w.h = 400;
    msnc3fire_work_get(fire)->parent_index = 5;
    msnc3fire(fire);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
test_mecasnc_tables_and_small_helpers(&ctx);
test_mecasnc_entry_init_wait_and_fout(&ctx);
test_mecasnc_motion_states(&ctx);
test_emie_init_walk_help_and_palette(&ctx);
test_animation_and_heart_helpers(&ctx);
test_hari3x_wrappers_init_break_and_ride_damage(&ctx);
test_hari3x_move_bounds(&ctx);
test_heart_and_fire_actors(&ctx);
TEST_MAIN_END

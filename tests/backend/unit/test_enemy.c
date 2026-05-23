#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
short_union editmode;

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int emycol_d_count;
static Sint16 emycol_d_result;
static Sint16 emycol_d_queue[8];
static int emycol_d_queue_count;
static int emycol_d_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int soundset_count;
static Sint16 soundset_requests[16];
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s0(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 emycol_d(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void soundset(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/enemy.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    if (emycol_d_queue_index < emycol_d_queue_count) {
        return emycol_d_queue[emycol_d_queue_index++];
    }
    return emycol_d_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
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

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_emycol(Sint16 result) {
    emycol_d_queue[emycol_d_queue_count++] = result;
}

static void reset_enemy_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&editmode, 0, sizeof(editmode));
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    memset(emycol_d_queue, 0, sizeof(emycol_d_queue));
    emycol_d_queue_count = 0;
    emycol_d_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 128;
    sinset_cos = 256;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, ari_pchg[0] == ari_pchg00);
    TEST_ASSERT_TRUE(ctx, chou_pchg[1] == chou_pchg01);
    TEST_ASSERT_TRUE(ctx, pchg_e[0] == pchg_e_0);
    TEST_ASSERT_TRUE(ctx, pchg_b[1] == pchg_b_1);
    TEST_ASSERT_TRUE(ctx, e_kamem_pchg[0] == e_kamem_pchg00);
    TEST_ASSERT_TRUE(ctx, b_tagame_pchg[1] == b_tagame_pchg01);
    TEST_ASSERT_TRUE(ctx, ari_pat[0] == &ari0);
    TEST_ASSERT_TRUE(ctx, e_chou_pat[2] == &chou3e);
    TEST_ASSERT_TRUE(ctx, b_ka_pat[4] == &ka5b);
    TEST_ASSERT_TRUE(ctx, tama_pat[3] == &tama_pat03);
    TEST_ASSERT_EQ_INT(ctx, 370, ari0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 433, tagame_pat06.spra[0].index);
}

static void test_enemy_wrappers_obey_suicide(test_context *ctx) {
    reset_enemy_state();
    enemy_suicide_result = -1;
    ene_ari(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);

    reset_enemy_state();
    enemy_suicide_result = -1;
    ene_chou(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_enemy_state();
    enemy_suicide_result = -1;
    ene_ka(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_enemy_state();
    enemy_suicide_result = -1;
    ene_kamemusi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_enemy_state();
    enemy_suicide_result = -1;
    ene_tagame_a(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_ari_paths(test_context *ctx) {
    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    emycol_d_result = -1;
    ene_ari(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].actflg);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == ari_pat);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);

    reset_enemy_state();
    actwk[1].userflag.b.h = 1;
    emycol_d_result = 0;
    ari_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);

    reset_enemy_state();
    emycol_d_result = -1;
    ari_fall(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    emycol_d_result = -1;
    ari_init(&actwk[1]);
    ari_fall(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    emycol_d_result = 3;
    ari_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[1].yposi.w.h);
    TEST_ASSERT_TRUE(ctx, patchg_table == ari_pchg);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    emycol_d_result = -1;
    ari_init(&actwk[1]);
    ari_fall(&actwk[1]);
    queue_emycol(8);
    queue_emycol(0);
    ari_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].cddat & 1);
    TEST_ASSERT_TRUE(ctx, patchg_table == ari_pchg);
}

static void test_chou_paths(test_context *ctx) {
    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    ene_chou(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].actflg);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == e_chou_pat);
    TEST_ASSERT_EQ_INT(ctx, 42, actwk[1].colino);

    reset_enemy_state();
    actwk[1].userflag.b.h = 1;
    chou_init(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == b_chou_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    chou_init(&actwk[1]);
    chou_move(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == chou_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    chou_init(&actwk[1]);
    actwk[1].xposi.w.h = 300;
    chou_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].cddat & 1);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    chou_init(&actwk[1]);
    ene_chou(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == chou_pchg);
}

static void test_ka_paths(test_context *ctx) {
    reset_enemy_state();
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1000;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    ene_ka(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == e_ka_pat);
    TEST_ASSERT_EQ_INT(ctx, 43, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_enemy_state();
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1000;
    actwk[1].userflag.b.h = 1;
    ka_init(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == b_ka_pat);

    reset_enemy_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    ka_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);

    reset_enemy_state();
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1000;
    ka_init(&actwk[1]);
    ka_turn(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);

    reset_enemy_state();
    emycol_d_result = -8;
    actwk[1].actflg = 128;
    ka_down(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 167, soundset_requests[0]);

    reset_enemy_state();
    emycol_d_result = -7;
    ka_down(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_enemy_state();
    actwk[1].actflg = 128;
    ka_stop(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
    actwk[1].actflg = 0;
    ka_stop(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
}

static void test_kamemusi_and_tama_paths(test_context *ctx) {
    reset_enemy_state();
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1000;
    actwk[1].xposi.w.h = 100;
    emycol_d_result = -1;
    ene_kamemusi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == e_kamem_pat);
    TEST_ASSERT_EQ_INT(ctx, 44, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_enemy_state();
    actwk[1].userflag.b.h = 2;
    emycol_d_result = 0;
    kamemusi_init(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == b_kamem_pat);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_enemy_state();
    emycol_d_result = -1;
    kamemusi_fall(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_enemy_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 1, area(&actwk[1]));
    actwk[0].xposi.w.h = 300;
    TEST_ASSERT_EQ_INT(ctx, 0, area(&actwk[1]));
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 400;
    TEST_ASSERT_EQ_INT(ctx, 0, area(&actwk[1]));

    reset_enemy_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 100;
    emycol_d_result = 0;
    kamemusi_init(&actwk[1]);
    kamemusi_fall(&actwk[1]);
    kamemusi_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);

    reset_enemy_state();
    editmode.w = 1;
    actwk[1].xposi.w.h = 100;
    emycol_d_result = 0;
    kamemusi_init(&actwk[1]);
    kamemusi_fall(&actwk[1]);
    kamemusi_move(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == e_kamem_pchg);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    emycol_d_result = 0;
    kamemusi_init(&actwk[1]);
    kamemusi_tama(&actwk[1]);
    kamemusi_move(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == e_kamem_pchg);

    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    emycol_d_result = 0;
    kamemusi_init(&actwk[1]);
    queue_emycol(8);
    queue_emycol(0);
    kamemusi_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].cddat & 1);

    reset_enemy_state();
    kamemusi_stop(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].mstno.b.h);

    reset_enemy_state();
    actwk[1].actno = 88;
    actwk[1].actflg = 128;
    actwk[1].sprpri = 4;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    kamemusi_tama(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 88, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[40].userflag.b.h);

    reset_enemy_state();
    actwk[1].userflag.b.h = 2;
    kamemusi_tama(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_enemy_state();
    actwk[1].userflag.b.h = 1;
    ene_kamemusi(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == tama_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_enemy_state();
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    kamemusi_tama(&actwk[1]);
    tama_init(&actwk[41]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[41].r_no0);

    reset_enemy_state();
    actwk[1].actflg = 0;
    tama_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_enemy_state();
    actwk[1].actflg = 128;
    emycol_d_result = -1;
    tama_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_enemy_state();
    actwk[1].actflg = 128;
    emycol_d_result = 0;
    tama_init(&actwk[1]);
    tama_move(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == tama_pchg);
}

static void test_tagame_paths(test_context *ctx) {
    reset_enemy_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    ene_tagame_a(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == e_tagame_pat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_enemy_state();
    actwk[1].userflag.b.h = 1;
    tagame_init(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == b_tagame_pat);

    reset_enemy_state();
    tagame_wait(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[1].mstno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].mstno.b.h);

    reset_enemy_state();
    actwk[1].userflag.b.h = 0;
    tagame_init(&actwk[1]);
    tagame_wait1(&actwk[1]);
    TEST_ASSERT_TRUE(ctx, patchg_table == e_tagame_pchg);

    reset_enemy_state();
    actwk[1].actflg = 128;
    actwk[1].userflag.b.h = 0;
    actwk[1].yposi.w.h = 200;
    tagame_init(&actwk[1]);
    tagame_jump(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[1].colino);
    TEST_ASSERT_EQ_INT(ctx, 162, soundset_requests[0]);

    reset_enemy_state();
    actwk[1].userflag.b.h = 0;
    actwk[1].yposi.w.h = 200;
    tagame_init(&actwk[1]);
    actwk[1].yposi.w.h = 300;
    tagame_jump1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);

    reset_enemy_state();
    actwk[1].actflg = 128;
    actwk[1].userflag.b.h = 0;
    actwk[1].yposi.w.h = 200;
    tagame_init(&actwk[1]);
    actwk[1].yposi.w.h = 300;
    tagame_jump1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 162, soundset_requests[0]);

    reset_enemy_state();
    ene_tama(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].actno);
}

TEST_MAIN_BEGIN;
    test_tables_capture_literal_data(&ctx);
    test_enemy_wrappers_obey_suicide(&ctx);
    test_ari_paths(&ctx);
    test_chou_paths(&ctx);
    test_ka_paths(&ctx);
    test_kamemusi_and_tama_paths(&ctx);
    test_tagame_paths(&ctx);
TEST_MAIN_END

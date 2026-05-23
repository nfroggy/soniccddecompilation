#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 prio_flag;
ushort_union swdata;
short_union editmode;
int_union scra_v_posit;

static sprite_pattern dummy_spring0 = {1, {{0, 0, 0, 100}}};
static sprite_pattern dummy_spring1 = {1, {{0, 0, 0, 101}}};
static sprite_pattern dummy_spring2 = {1, {{0, 0, 0, 102}}};
sprite_pattern *springpat2[] = {&dummy_spring0};
sprite_pattern *spring90pat2[] = {&dummy_spring1};
sprite_pattern *spring45pat2[] = {&dummy_spring2};
static Uint8 dummy_springchg0[] = {1, 0, 255};
Uint8 *springchg[] = {dummy_springchg0};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static Sint32 frameout_s00_result;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int speedset2_count;
static sprite_status *speedset2_actor;
static int soundset_count;
static Sint16 soundset_requests[8];
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void speedset2(sprite_status *pActwk);
void soundset(Sint16 ReqNo);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r7/chgban.c"

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

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return frameout_s00_result;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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
    pActwk->xposi.l += (Sint32)pActwk->xspeed.w << 8;
    pActwk->yposi.l += (Sint32)pActwk->yspeed.w << 8;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) / sizeof(soundset_requests[0]))) {
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

static void queue_actwk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_actwk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void link_actor(sprite_status *actor, int actfree_offset, int index) {
    *(Sint32 *)&actor->actfree[actfree_offset] = index;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    ((Sint16 *)actor)[index] = value;
}

static void reset_chgban_state(void) {
    memset(actwk, 0, sizeof(actwk));
    prio_flag = 0;
    memset(&swdata, 0, sizeof(swdata));
    memset(&editmode, 0, sizeof(editmode));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s00_result = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    speedset2_count = 0;
    speedset2_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 16;
    sinset_cos = 32;
}

static void test_chgban_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, amipat[0] == &amipat0);
    TEST_ASSERT_TRUE(ctx, chgbanpat[0] == &chgbanpat0);
    TEST_ASSERT_TRUE(ctx, chgbanpat[3] == &chgbanpat4);
    TEST_ASSERT_TRUE(ctx, spr_rpat[4] == &spr_rpat4);
    TEST_ASSERT_TRUE(ctx, hibanapat[1] == &hibanapat1);
    TEST_ASSERT_EQ_INT(ctx, 442, amipat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 443, chgbanpat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 450, spr_rpat2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 453, hibanapat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 4, chgbanchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 252, chgbanchg0[10]);
    TEST_ASSERT_EQ_INT(ctx, 5, spr_rchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, hibanachg0[0]);
    TEST_ASSERT_EQ_INT(ctx, -24, ppositbl[5]);
    TEST_ASSERT_EQ_INT(ctx, 4, gpositbl[15]);
}

static void test_ami_uses_priority_flag(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_chgban_state();
    ami(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17144, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == amipat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_chgban_state();
    actor->r_no0 = 2;
    prio_flag = 1;
    ami(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->sprpri);
}

static void test_chgban_priority_gate_paths(test_context *ctx) {
    sprite_status *gate = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    gate->xposi.w.h = 100;
    gate->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    gate->userflag.b.h = 1;
    chgban(gate);
    TEST_ASSERT_EQ_INT(ctx, 4, gate->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, gate->actflg & 6);
    TEST_ASSERT_EQ_INT(ctx, 845 | 32768, gate->sproffset);
    TEST_ASSERT_TRUE(ctx, gate->patbase == chgbanpat);
    TEST_ASSERT_EQ_INT(ctx, 24, gate->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 60, gate->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, gate->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, prio_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_chgban_state();
    gate->r_no0 = 2;
    gate->actfree[0] = 3;
    chgban_move0(gate);
    TEST_ASSERT_EQ_INT(ctx, 2, gate->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, gate->r_no0);

    reset_chgban_state();
    gate->r_no0 = 2;
    gate->xposi.w.h = 100;
    gate->yposi.w.h = 100;
    gate->sprhs = 24;
    gate->sprvsize = 24;
    gate->sproffset = 32768;
    player->xposi.w.h = 300;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    prio_flag = 1;
    chgban_move0(gate);
    TEST_ASSERT_EQ_INT(ctx, 60, gate->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, gate->r_no0);

    reset_chgban_state();
    gate->r_no0 = 4;
    chgban_move1(gate);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == chgbanchg);

    reset_chgban_state();
    gate->r_no0 = 6;
    prio_flag = 0;
    chgban_move2(gate);
    TEST_ASSERT_EQ_INT(ctx, 32768, gate->sproffset & 32768);
    TEST_ASSERT_EQ_INT(ctx, 4, gate->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, gate->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 2, gate->r_no0);

    reset_chgban_state();
    gate->r_no0 = 6;
    gate->sproffset = 32768;
    prio_flag = 1;
    chgban_move2(gate);
    TEST_ASSERT_EQ_INT(ctx, 0, gate->sproffset & 32768);
    TEST_ASSERT_EQ_INT(ctx, 0, gate->sprpri);
}

static void test_spring_r_initialization_and_wrapper_paths(test_context *ctx) {
    sprite_status *spring = &actwk[2];

    reset_chgban_state();
    spring->xposi.w.h = 120;
    spring->yposi.w.h = 80;
    spring->userflag.b.h = 3;
    queue_actwk2(&actwk[10]);
    queue_actwk2(&actwk[11]);
    queue_actwk2(&actwk[12]);
    queue_actwk2(&actwk[10]);
    queue_actwk2(&actwk[11]);
    queue_actwk2(&actwk[12]);
    spring_r(spring);
    TEST_ASSERT_EQ_INT(ctx, 2, spring->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, spring->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, spring->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[12].actno);
    TEST_ASSERT_TRUE(ctx, actwk[10].patbase == spring90pat2);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[10].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, *(Sint32 *)&spring->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 11, *(Sint32 *)&spring->actfree[12]);
    TEST_ASSERT_EQ_INT(ctx, 12, *(Sint32 *)&spring->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);

    reset_chgban_state();
    queue_actwk2(&actwk[10]);
    spr_r_init(spring);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == spring);

    reset_chgban_state();
    spring->r_no0 = 4;
    spring->actfree[20] = 1;
    spring_r(spring);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_chgban_state();
    spring->r_no0 = 6;
    actwk[0].r_no0 = 6;
    spring_r(spring);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_chgban_state();
    spring->r_no0 = 2;
    link_actor(spring, 0, 10);
    link_actor(spring, 12, 11);
    link_actor(spring, 16, 12);
    set_actor_word(spring, 28, 444);
    frameout_s00_result = 1;
    spring_r(spring);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[11].actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[12].actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 444, frameout_s00_x);
}

static void test_spring_piece_set_collision_and_release_paths(test_context *ctx) {
    sprite_status *piece = &actwk[5];
    sprite_status *master = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 4;
    spr_r_set(piece);
    TEST_ASSERT_TRUE(ctx, piece->patbase == spring45pat2);
    TEST_ASSERT_EQ_INT(ctx, 1266, piece->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 92, piece->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 92, piece->yposi.w.h);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->r_no0 = 4;
    master->patcnt = 1;
    spr_r_move0(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == spr_rchg);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 0;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->sprhs = 7;
    player->sprvsize = 14;
    player->sproffset = 32768;
    sinset_sin = 16;
    sinset_cos = 32;
    spr_r_move1(piece);
    TEST_ASSERT_EQ_INT(ctx, 2, piece->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, piece->actfree[8]);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, player->cddat & 2);
    TEST_ASSERT_EQ_INT(ctx, 512, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 256, player->yspeed.w);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->patcnt = 1;
    piece->r_no0 = 4;
    piece->actfree[8] = 2;
    link_actor(piece, 4, 0);
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, piece->actfree[8]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->patcnt = 2;
    piece->r_no0 = 4;
    piece->actfree[8] = 1;
    link_actor(piece, 4, 0);
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 0, piece->actfree[8]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 206, soundset_requests[0]);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->patcnt = 3;
    piece->r_no0 = 4;
    piece->actfree[8] = 0;
    link_actor(piece, 4, 0);
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    link_actor(master, 12, 11);
    link_actor(master, 16, 12);
    piece->r_no0 = 6;
    piece->patcnt = 8;
    piece->patno = 9;
    piece->userflag.b.h = 1;
    actwk[11].userflag.b.l = 3;
    actwk[12].userflag.b.l = 4;
    spr_r_move3(piece);
    TEST_ASSERT_EQ_INT(ctx, 2, piece->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, piece->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, piece->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[11].userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[12].userflag.b.l);
}

static void test_spring_gawa_movement_and_gun_launch(test_context *ctx) {
    sprite_status *gawa = &actwk[8];
    sprite_status *piece = &actwk[5];
    sprite_status *master = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    link_actor(master, 4, 0);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    piece->patcnt = 1;
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->userflag.b.l = 2;
    gawa->userflag.b.h = 1;
    gawa->userflag.b.l = 0;
    player->sprhs = 7;
    player->sprvsize = 14;
    spr_r_move4(gawa);
    TEST_ASSERT_EQ_INT(ctx, 2, gawa->patno);
    TEST_ASSERT_EQ_INT(ctx, 124, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, player->yposi.w.h);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    link_actor(piece, 12, 9);
    link_actor(piece, 16, 10);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    piece->patcnt = 0;
    gawa->r_no0 = 10;
    gawa->userflag.b.h = 1;
    player->xposi.w.h = 128;
    player->yposi.w.h = 104;
    player->sprhs = 7;
    player->sprvsize = 14;
    spr_r_move4(gawa);
    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[10].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 26, actwk[10].actfree[8]);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    link_actor(piece, 12, 9);
    link_actor(piece, 16, 10);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    piece->patcnt = 0;
    gawa->r_no0 = 12;
    gawa->userflag.b.h = 1;
    player->xposi.w.h = 72;
    player->yposi.w.h = 96;
    player->sprhs = 7;
    player->sprvsize = 14;
    spr_r_move5(gawa);
    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[9].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[9].yspeed.w);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    gawa->r_no0 = 14;
    gawa->r_no1 = 10;
    gawa->actfree[8] = 1;
    gawa->xspeed.w = 3;
    gawa->yspeed.w = -2;
    player->sproffset = 0;
    prio_sav = 128;
    spr_r_move6(gawa);
    TEST_ASSERT_EQ_INT(ctx, 2, master->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 10, gawa->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 192, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 32768, player->sproffset & 32768);
    TEST_ASSERT_EQ_INT(ctx, 3, gawa->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2, gawa->yposi.w.h);
}

static void test_gun7_paths_and_projectile_spawns(test_context *ctx) {
    sprite_status *gun = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    gun->xposi.w.h = 100;
    gun->yposi.w.h = 100;
    player->xposi.w.h = 120;
    player->yposi.w.h = 100;
    player->sprhs = 7;
    player->sprvsize = 14;
    queue_actwk(&actwk[20]);
    queue_actwk(&actwk[21]);
    gun7(gun);
    TEST_ASSERT_EQ_INT(ctx, 4, gun->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, gun->patno);
    TEST_ASSERT_EQ_INT(ctx, 6, gun->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 2048, gun->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -4096, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[21].actno);
    TEST_ASSERT_EQ_INT(ctx, 116, actwk[21].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 206, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_chgban_state();
    gun->xposi.w.h = 100;
    gun->yposi.w.h = 100;
    player->xposi.w.h = 80;
    player->yposi.w.h = 100;
    player->sprhs = 7;
    player->sprvsize = 14;
    queue_actwk(&actwk[20]);
    queue_actwk(&actwk[21]);
    gun7_init(gun);
    TEST_ASSERT_EQ_INT(ctx, -2048, gun->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4096, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 84, actwk[21].xposi.w.h);

    reset_chgban_state();
    gun->userflag.b.h = 1;
    gun->yposi.w.h = 40;
    scra_v_posit.w.h = 0;
    gun7_init(gun);
    TEST_ASSERT_EQ_INT(ctx, 6, gun->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1984, gun->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset2_count);

    reset_chgban_state();
    gun->r_no0 = 4;
    gun->actfree[0] = 1;
    gun->xspeed.w = 100;
    gun7_move1(gun);
    TEST_ASSERT_EQ_INT(ctx, -100, gun->xspeed.w);

    reset_chgban_state();
    gun->r_no0 = 4;
    gun->xposi.w.h = 50;
    set_actor_word(gun, 24, 50);
    gun7_move1(gun);
    TEST_ASSERT_EQ_INT(ctx, 2, gun->r_no0);

    reset_chgban_state();
    gun->yposi.w.h = 300;
    scra_v_posit.w.h = 0;
    gun7_move2(gun);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_collision_guards_and_player_speed_variants(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->sprhs = 7;
    player->sprvsize = 14;
    player->r_no0 = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, coli0(actor, player, 32, 32));
    TEST_ASSERT_EQ_INT(ctx, 2, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 120, ((Sint16 *)player)[26]);

    player->mstno.b.h = 43;
    TEST_ASSERT_EQ_INT(ctx, 0, coli0(actor, player, 32, 32));
    player->mstno.b.h = 0;
    player->r_no0 = 6;
    TEST_ASSERT_EQ_INT(ctx, 0, coli0(actor, player, 32, 32));

    reset_chgban_state();
    actor->actflg = 1;
    player->cddat = 8;
    plspdset(actor, player, 1);
    TEST_ASSERT_EQ_INT(ctx, -4096, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, player->cddat & 3);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat & 8);

    reset_chgban_state();
    actor->actflg = 2;
    plspdset(actor, player, 3);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4096, player->yspeed.w);

    reset_chgban_state();
    actor->actflg = 3;
    sinset_sin = 16;
    sinset_cos = 32;
    plspdset(actor, player, 0);
    TEST_ASSERT_EQ_INT(ctx, 224, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, -512, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, player->cddat & 1);
}

static void test_hibana_initializes_animates_and_exits(test_context *ctx) {
    sprite_status *spark = &actwk[6];

    reset_chgban_state();
    hibana(spark);
    TEST_ASSERT_EQ_INT(ctx, 2, spark->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, spark->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 33592, spark->sproffset);
    TEST_ASSERT_TRUE(ctx, spark->patbase == hibanapat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_chgban_state();
    spark->r_no0 = 2;
    hibana(spark);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == hibanachg);

    reset_chgban_state();
    spark->r_no0 = 4;
    hibana(spark);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_remaining_spring_gate_guard_paths(test_context *ctx) {
    sprite_status *piece = &actwk[5];
    sprite_status *gawa = &actwk[8];
    sprite_status *master = &actwk[2];
    sprite_status *player = &actwk[0];

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 1;
    piece->userflag.b.h = 1;
    spr_r_set(piece);
    TEST_ASSERT_TRUE(ctx, piece->patbase == spring90pat2);
    TEST_ASSERT_EQ_INT(ctx, 0, piece->actflg & 3);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 2;
    piece->userflag.b.h = 2;
    spr_r_set(piece);
    TEST_ASSERT_TRUE(ctx, piece->patbase == spring45pat2);
    TEST_ASSERT_EQ_INT(ctx, 3, piece->actflg & 3);

    reset_chgban_state();
    editmode.b.h = 1;
    coli_spr(piece);
    TEST_ASSERT_EQ_INT(ctx, 0, piece->r_no0);

    reset_chgban_state();
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    player->xposi.w.h = 1000;
    player->yposi.w.h = 100;
    player->sprhs = 7;
    player->sprvsize = 14;
    TEST_ASSERT_EQ_INT(ctx, 0, coli0(piece, player, 32, 32));
    player->xposi.w.h = 100;
    player->yposi.w.h = 1000;
    TEST_ASSERT_EQ_INT(ctx, 0, coli0(piece, player, 32, 32));

    reset_chgban_state();
    piece->actfree[0] = 2;
    piece->xspeed.w = 4;
    gun7_move1(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, piece->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, piece->xspeed.w);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    link_actor(piece, 4, 0);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 0;
    piece->userflag.b.h = 1;
    piece->userflag.b.l = 4;
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    link_actor(piece, 4, 0);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 1;
    piece->userflag.b.h = 1;
    piece->userflag.b.l = 1;
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_chgban_state();
    link_actor(piece, 0, 2);
    link_actor(piece, 4, 0);
    master->xposi.w.h = 100;
    master->yposi.w.h = 100;
    master->patcnt = 2;
    piece->userflag.b.h = 1;
    piece->userflag.b.l = 2;
    swdata.b.l = 112;
    spr_r_move2(piece);
    TEST_ASSERT_EQ_INT(ctx, 4, piece->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    spr_r_move4(gawa);
    TEST_ASSERT_EQ_INT(ctx, 4, gawa->patno);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    gawa->userflag.b.h = 1;
    gawa->userflag.b.l = 1;
    spr_r_move4(gawa);
    TEST_ASSERT_EQ_INT(ctx, 4, gawa->patno);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    gawa->userflag.b.h = 1;
    gawa->userflag.b.l = 1;
    spr_r_move5(gawa);
    TEST_ASSERT_EQ_INT(ctx, 4, gawa->patno);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    gawa->userflag.b.h = 1;
    gawa->userflag.b.l = 7;
    player->xposi.w.h = 1000;
    player->yposi.w.h = 1000;
    player->sprhs = 7;
    player->sprvsize = 14;
    spr_r_move4(gawa);
    TEST_ASSERT_EQ_INT(ctx, 0, gawa->userflag.b.l);

    reset_chgban_state();
    link_actor(gawa, 0, 5);
    link_actor(piece, 0, 2);
    link_actor(piece, 16, 10);
    link_actor(&actwk[10], 0, 5);
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 100;
    gawa->r_no0 = 10;
    actwk[10].r_no0 = 10;
    player->sprhs = 7;
    player->sprvsize = 14;
    playset_g(gawa, piece, player, (Uint8 *)ppositblg_0);
    TEST_ASSERT_EQ_INT(ctx, 2, master->userflag.b.h);
}

TEST_MAIN_BEGIN;
test_chgban_tables_capture_literal_data(&ctx);
test_ami_uses_priority_flag(&ctx);
test_chgban_priority_gate_paths(&ctx);
test_spring_r_initialization_and_wrapper_paths(&ctx);
test_spring_piece_set_collision_and_release_paths(&ctx);
test_spring_gawa_movement_and_gun_launch(&ctx);
test_gun7_paths_and_projectile_spawns(&ctx);
test_collision_guards_and_player_speed_variants(&ctx);
test_hibana_initializes_animates_and_exits(&ctx);
test_remaining_spring_gate_guard_paths(&ctx);
TEST_MAIN_END

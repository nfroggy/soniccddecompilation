#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 plpower_m;

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[8];
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static Sint16 emycol_d_result;
static Sint16 dircol_l2_result;
static Sint16 dircol_r2_result;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 dircol_l2(sprite_status *pActwk);
Sint16 dircol_r2(sprite_status *pActwk);
Uint8 CCset(Sint32 lSrc, Sint32 lDst);
Uint8 bchg(Uint8 bySrc, Uint8 byDst);

#include "src/r3/kama.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
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
    if (soundset_count < 8) {
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

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}

Sint16 dircol_l2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_l2_result;
}

Sint16 dircol_r2(sprite_status *pActwk) {
    (void)pActwk;
    return dircol_r2_result;
}

Uint8 CCset(Sint32 lSrc, Sint32 lDst) {
    return lSrc - lDst < 0;
}

Uint8 bchg(Uint8 bySrc, Uint8 byDst) {
    return byDst ^ (Uint8)(1u << bySrc);
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    ((Sint16 *)actor)[index] = value;
}

static Sint16 actor_word(sprite_status *actor, int index) {
    return ((Sint16 *)actor)[index];
}

static void reset_kama_state(void) {
    memset(actwk, 0, sizeof(actwk));
    plpower_m = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    emycol_d_result = 0;
    dircol_l2_result = 0;
    dircol_r2_result = 0;
    jmp_flg = 0;
}

static void reset_kama_logs(void) {
    enemy_suicide_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void test_kama_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, kama_tbl[0] == &kama_init);
    TEST_ASSERT_TRUE(ctx, kama_tbl[6] == &kama_reve);
    TEST_ASSERT_TRUE(ctx, wpkama_tbl[0] == &wpkama_init);
    TEST_ASSERT_TRUE(ctx, wpkama_tbl[2] == &wpkama_move);
    TEST_ASSERT_TRUE(ctx, kama_pchg[0] == kama_pchg0);
    TEST_ASSERT_TRUE(ctx, kama_pchg[4] == kama_pchg4);
    TEST_ASSERT_TRUE(ctx, wpkama_pchg[0] == wpkama_pchg0);
    TEST_ASSERT_TRUE(ctx, kamapat[0] == &kama_patA);
    TEST_ASSERT_TRUE(ctx, bkamapat[7] == &bkama_patH);
    TEST_ASSERT_TRUE(ctx, wpkamapat[5] == &bu5);
    TEST_ASSERT_EQ_INT(ctx, 424, kama_patA.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 473, bkama_patH.spra[2].index);
    TEST_ASSERT_EQ_INT(ctx, 423, bu5.spra[0].index);
}

static void test_ene_kama_wrapper_and_initialization(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    enemy_suicide_result = -1;
    ene_kama(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_kama_state();
    actor->xposi.w.h = 4800;
    actor->yposi.w.h = 890;
    emycol_d_result = 0;
    ene_kama(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_TRUE(ctx, actor->patbase == kamapat);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == kama_pchg);

    reset_kama_state();
    actor->userflag.b.h = -1;
    emycol_d_result = 0;
    kama_init(actor);
    TEST_ASSERT_TRUE(ctx, actor->patbase == bkamapat);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
}

static void test_kama_fall_and_movement_preserve_fixed_point_behavior(
    test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    actor->xspeed.w = -256;
    actor->yspeed.w = -128;
    actor->xposi.l = 0x00100000;
    actor->yposi.l = 0x00200000;
    movement(actor);
    TEST_ASSERT_EQ_INT(ctx, 15, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 31, actor->yposi.w.h);

    reset_kama_state();
    actor->yspeed.w = 256;
    emycol_d_result = -1;
    kama_fall(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, patchg_table == kama_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_kama_wait_attack_and_timer_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    kama_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_TRUE(ctx, patchg_table == kama_pchg);

    reset_kama_state();
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 80;
    actor->actflg = 128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actor->actno = 36;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    kama_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 37, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 117, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 109, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 119, actor_word(actor, 24));

    reset_kama_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 120;
    actor->actflg = 128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    kama_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->mstno.b.h);

    reset_kama_state();
    set_actor_word(actor, 24, 1);
    kama_atck(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 59, actor_word(actor, 24));

    reset_kama_state();
    set_actor_word(actor, 24, 1);
    dircol_l2_result = -1;
    kama_pati(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
}

static void test_kama_move_ground_wall_and_reveal_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    actor->cddat = 0;
    actor->userflag.b.h = 0;
    emycol_d_result = 0;
    kama_move(actor);
    TEST_ASSERT_EQ_INT(ctx, -256, actor->xspeed.w);
    TEST_ASSERT_TRUE(ctx, patchg_table == kama_pchg);

    reset_kama_state();
    actor->cddat = 1;
    actor->userflag.b.h = -1;
    emycol_d_result = 6;
    kama_move(actor);
    TEST_ASSERT_EQ_INT(ctx, 128, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->yposi.w.h);

    reset_kama_state();
    dircol_l2_result = -1;
    kama_move(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 71, actor_word(actor, 24));

    reset_kama_state();
    dircol_r2_result = -1;
    kama_move(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);

    reset_kama_state();
    set_actor_word(actor, 24, 1);
    actor->actflg = 0;
    actor->cddat = 0;
    actor->r_no0 = 6;
    kama_reve(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat & 1);
}

static void test_set_wpkama_spawns_or_skips_by_userflag(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    actor->userflag.b.h = -1;
    set_wpkama(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_kama_state();
    actor->actflg = 0;
    actor->userflag.b.h = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    set_wpkama(actor);
    TEST_ASSERT_EQ_INT(ctx, 83, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 91, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[10].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 44, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[10].userflag.b.h);

    reset_kama_state();
    actor->actflg = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    set_wpkama(actor);
    TEST_ASSERT_EQ_INT(ctx, 117, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 109, actwk[11].xposi.w.h);
}

static void test_tama_kama_init_wait_and_display_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_kama_state();
    actor->xposi.w.h = 200;
    tama_kama(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -768, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actflg & 1);
    TEST_ASSERT_TRUE(ctx, actor->patbase == wpkamapat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 200, frameout_s00_xpos);

    reset_kama_state();
    actor->actfree[4] = 1;
    wpkama_init(actor);
    TEST_ASSERT_EQ_INT(ctx, 768, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actflg & 1);

    reset_kama_state();
    actor->r_no0 = 2;
    set_actor_word(actor, 24, 1);
    set_actor_word(actor, 26, 5);
    actwk[5].actno = 0;
    wpkama_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 135, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 60, actor_word(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_kama_state();
    actor->r_no0 = 2;
    set_actor_word(actor, 24, 2);
    set_actor_word(actor, 26, 5);
    actwk[5].actno = 36;
    wpkama_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, patchg_table == wpkama_pchg);

    reset_kama_state();
    actor->colino = 135;
    set_actor_word(actor, 28, 1);
    wpkama_disp(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_word(actor, 28));
}

static void test_wpkama_collision_checks_and_reveal(test_context *ctx) {
    sprite_status *bullet = &actwk[1];
    sprite_status *player = &actwk[0];

    reset_kama_state();
    TEST_ASSERT_EQ_INT(ctx, 0, wpkama_colicheck(bullet, player));

    player->cddat = 4;
    player->xposi.w.h = -40;
    TEST_ASSERT_EQ_INT(ctx, 0, wpkama_colicheck(bullet, player));

    player->xposi.w.h = 40;
    TEST_ASSERT_EQ_INT(ctx, 0, wpkama_colicheck(bullet, player));

    player->xposi.w.h = 0;
    player->yposi.w.h = -40;
    TEST_ASSERT_EQ_INT(ctx, 0, wpkama_colicheck(bullet, player));

    player->yposi.w.h = 40;
    TEST_ASSERT_EQ_INT(ctx, 0, wpkama_colicheck(bullet, player));

    player->yposi.w.h = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, wpkama_colicheck(bullet, player));

    reset_kama_state();
    plpower_m = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, wpkama_colicheck(bullet, player));

    player->xspeed.w = 100;
    bullet->xspeed.w = 20;
    wpkama_reve(bullet, player);
    TEST_ASSERT_EQ_INT(ctx, 0, bullet->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, bullet->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 120, bullet->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -2048, bullet->yspeed.w);
}

static void test_wpkama_move_sound_collision_and_frameout(test_context *ctx) {
    sprite_status *bullet = &actwk[1];
    sprite_status *player = &actwk[0];

    reset_kama_state();
    bullet->actflg = 128;
    bullet->patno = 0;
    bullet->xspeed.w = 256;
    bullet->yspeed.w = 0;
    bullet->colino = 135;
    set_actor_word(bullet, 28, 2);
    player->cddat = 4;
    player->xposi.w.h = 1;
    player->yposi.w.h = 0;
    player->xspeed.w = 50;
    wpkama_move(bullet);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 185, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, bullet->mstno.b.h);
    TEST_ASSERT_TRUE(ctx, patchg_table == wpkama_pchg);

    reset_kama_state();
    bullet->r_no0 = 4;
    set_actor_word(bullet, 27, 123);
    tama_kama(bullet);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 123, frameout_s00_xpos);
}

TEST_MAIN_BEGIN;
test_kama_tables_capture_literal_data(&ctx);
test_ene_kama_wrapper_and_initialization(&ctx);
test_kama_fall_and_movement_preserve_fixed_point_behavior(&ctx);
test_kama_wait_attack_and_timer_paths(&ctx);
test_kama_move_ground_wall_and_reveal_paths(&ctx);
test_set_wpkama_spawns_or_skips_by_userflag(&ctx);
test_tama_kama_init_wait_and_display_paths(&ctx);
test_wpkama_collision_checks_and_reveal(&ctx);
test_wpkama_move_sound_collision_and_frameout(&ctx);
TEST_MAIN_END

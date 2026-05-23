#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 plpower_a;
Uint8 plpower_m;
Uint8 plpower_b;
Uint8 conbine_flag;
Uint8 bossstart;
Sint16 emyscorecnt;
Sint16 plring;
short_union debugflag;
short_union editmode;

static int scoreup_count;
static Uint32 scoreup_values[8];
static int soundset_count;
static Sint16 soundset_requests[8];
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int jumpcolsub_count;

void scoreup(Uint32 updata);
void soundset(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
void jumpcolsub(void);
Uint8 CCset(Sint32 lSrc, Sint32 lDst);
Uint8 CSset(Uint16 wSrc, Uint16 wDst);

#include "src/r3/coli3.c"

void scoreup(Uint32 updata) {
    if (scoreup_count < 8) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
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

void jumpcolsub(void) { ++jumpcolsub_count; }

Uint8 CCset(Sint32 lSrc, Sint32 lDst) { return lSrc - lDst < 0; }

Uint8 CSset(Uint16 wSrc, Uint16 wDst) {
    return (Uint32)wSrc + (Uint32)wDst > 65535;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    ((Sint16 *)actor)[index] = value;
}

static void reset_coli3_state(void) {
    memset(actwk, 0, sizeof(actwk));
    plpower_a = 0;
    plpower_m = 0;
    plpower_b = 0;
    conbine_flag = 0;
    bossstart = 0;
    emyscorecnt = 0;
    plring = 0;
    debugflag.w = 0;
    editmode.w = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    jumpcolsub_count = 0;
}

static void test_coli3_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 10, escoretbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 100, escoretbl[3]);
    TEST_ASSERT_EQ_INT(ctx, 20, colitbl[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 20, colitbl[0][1]);
    TEST_ASSERT_EQ_INT(ctx, 6, colitbl[6][0]);
    TEST_ASSERT_EQ_INT(ctx, 6, colitbl[6][1]);
    TEST_ASSERT_EQ_INT(ctx, 40, colitbl[57][0]);
    TEST_ASSERT_EQ_INT(ctx, 36, colitbl[57][1]);
    TEST_ASSERT_EQ_INT(ctx, 64, colitbl[62][0]);
    TEST_ASSERT_EQ_INT(ctx, 16, colitbl[62][1]);
    TEST_ASSERT_EQ_INT(ctx, 0, colitbl[63][0]);
}

static void test_pcol_scans_active_collision_slots(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *enemy = &actwk[32];

    reset_coli3_state();
    player->actfree[2] = 64;
    TEST_ASSERT_EQ_INT(ctx, 0, pcol(player));

    reset_coli3_state();
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->sprvsize = 16;
    TEST_ASSERT_EQ_INT(ctx, 0, pcol(player));

    plpower_a = 1;
    enemy->actflg = 128;
    enemy->colino = 1;
    enemy->xposi.w.h = 100;
    enemy->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 1, pcol(player));
    TEST_ASSERT_EQ_INT(ctx, 24, enemy->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);

    reset_coli3_state();
    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->sprvsize = 20;
    player->patno = 57;
    plpower_a = 1;
    enemy->actflg = 128;
    enemy->colino = 1;
    enemy->xposi.w.h = 100;
    enemy->yposi.w.h = 110;
    TEST_ASSERT_EQ_INT(ctx, 1, pcol(player));
}

static void test_collision_table_x_and_y_bounds(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *target = &actwk[1];

    reset_coli3_state();
    target->colino = 65;
    target->xposi.w.h = 100;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, -1,
                       CollitblDataXchk(player, target, 92, 92, 16));
    TEST_ASSERT_EQ_INT(ctx, 2, target->r_no0);

    reset_coli3_state();
    target->colino = 65;
    target->xposi.w.h = 100;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataXchk(player, target, 30, 92, 16));

    reset_coli3_state();
    target->colino = 65;
    target->xposi.w.h = 100;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataXchk(player, target, 130, 92, 16));

    reset_coli3_state();
    target->colino = 65;
    target->xposi.w.h = 100;
    target->yposi.w.h = 100;
    (void)CollitblDataXchk(player, target, 70, 92, 16);
    TEST_ASSERT_EQ_INT(ctx, 2, target->r_no0);

    reset_coli3_state();
    target->colino = 65;
    target->xposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataXchk(player, target, 60, 92, 16));

    reset_coli3_state();
    target->colino = 65;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataYchk(player, target, 30, 0, 16));

    reset_coli3_state();
    target->colino = 65;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataYchk(player, target, 130, 0, 16));

    reset_coli3_state();
    target->colino = 65;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, CollitblDataYchk(player, target, 60, 0, 16));

    reset_coli3_state();
    target->colino = 65;
    target->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, -1, CollitblDataYchk(player, target, 70, 0, 16));
    TEST_ASSERT_EQ_INT(ctx, 2, target->r_no0);
}

static void test_hit_dispatch_routes_by_colino_high_bits(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *target = &actwk[1];

    reset_coli3_state();
    plpower_a = 1;
    target->colino = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, ColliHitChk(player, target));
    TEST_ASSERT_EQ_INT(ctx, 24, target->actno);

    reset_coli3_state();
    target->colino = 65;
    TEST_ASSERT_EQ_INT(ctx, -1, ColliHitChk(player, target));
    TEST_ASSERT_EQ_INT(ctx, 2, target->r_no0);

    reset_coli3_state();
    plpower_m = 1;
    target->colino = 129;
    TEST_ASSERT_EQ_INT(ctx, 1, ColliHitChk(player, target));
    TEST_ASSERT_EQ_INT(ctx, 0, jumpcolsub_count);

    reset_coli3_state();
    target->colino = 215;
    TEST_ASSERT_EQ_INT(ctx, 1, ColliHitChk(player, target));
    TEST_ASSERT_EQ_INT(ctx, 1, target->colicnt);
}

static void test_item_collision_preserves_bumper_behaviors(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *item = &actwk[1];

    reset_coli3_state();
    item->colino = 65;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolitem(player, item));
    TEST_ASSERT_EQ_INT(ctx, 2, item->r_no0);

    reset_coli3_state();
    item->colino = 65;
    set_actor_word(player, 26, 90);
    TEST_ASSERT_EQ_INT(ctx, -1, pcolitem(player, item));
    TEST_ASSERT_EQ_INT(ctx, 0, item->r_no0);

    reset_coli3_state();
    item->colino = 70;
    item->yposi.w.h = 100;
    player->yposi.w.h = 120;
    player->yspeed.w = -300;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolitem(player, item));
    TEST_ASSERT_EQ_INT(ctx, 300, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -384, item->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, item->r_no1);

    reset_coli3_state();
    item->colino = 70;
    item->yposi.w.h = 100;
    item->r_no1 = 6;
    player->yposi.w.h = 80;
    player->yspeed.w = -300;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolitem(player, item));
    TEST_ASSERT_EQ_INT(ctx, -300, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, item->r_no1);

    reset_coli3_state();
    item->colino = 70;
    player->yspeed.w = 300;
    player->mstno.b.h = 2;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolitem(player, item));
    TEST_ASSERT_EQ_INT(ctx, -300, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, item->r_no0);
}

static void test_normal_enemy_collision_scores_and_bounces(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *enemy = &actwk[1];

    reset_coli3_state();
    enemy->xposi.w.h = 120;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolnomal(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 6, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 147, soundset_requests[0]);

    reset_coli3_state();
    plpower_a = 1;
    enemy->colicnt = 2;
    player->xspeed.w = 400;
    player->yspeed.w = -200;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolnomal(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, -200, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy->colicnt);

    reset_coli3_state();
    plpower_a = 1;
    enemy->colicnt = 1;
    pcolnomal(player, enemy);
    TEST_ASSERT_EQ_INT(ctx, 0, enemy->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 128, enemy->cddat & 128);

    reset_coli3_state();
    plpower_a = 1;
    player->yposi.w.h = 90;
    player->yspeed.w = 300;
    enemy->yposi.w.h = 100;
    pcolnomal(player, enemy);
    TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, -300, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 24, enemy->actno);

    reset_coli3_state();
    plpower_a = 1;
    emyscorecnt = 6;
    player->yposi.w.h = 120;
    player->yspeed.w = 300;
    enemy->yposi.w.h = 100;
    pcolnomal(player, enemy);
    TEST_ASSERT_EQ_INT(ctx, 100, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 44, player->yspeed.w);

    reset_coli3_state();
    plpower_a = 1;
    emyscorecnt = 31;
    player->yspeed.w = -300;
    pcolnomal(player, enemy);
    TEST_ASSERT_EQ_INT(ctx, 1000, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, -44, player->yspeed.w);
}

static void test_player_damage_and_death_paths(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *enemy = &actwk[1];

    reset_coli3_state();
    plpower_m = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolplay(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 0, jumpcolsub_count);

    reset_coli3_state();
    set_actor_word(player, 26, 1);
    TEST_ASSERT_EQ_INT(ctx, -1, pcole(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 0, jumpcolsub_count);

    reset_coli3_state();
    plpower_b = 3;
    conbine_flag = 7;
    enemy->xposi.w.h = 120;
    player->xposi.w.h = 100;
    player->cddat = 64;
    TEST_ASSERT_EQ_INT(ctx, -1, playdamageset(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 4, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -512, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -256, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, plpower_b);
    TEST_ASSERT_EQ_INT(ctx, 7, conbine_flag);

    reset_coli3_state();
    plpower_b = 2;
    conbine_flag = 7;
    player->xposi.w.h = 100;
    enemy->xposi.w.h = 80;
    TEST_ASSERT_EQ_INT(ctx, -1, playdamageset(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 512, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, plpower_b);
    TEST_ASSERT_EQ_INT(ctx, 0, conbine_flag);

    reset_coli3_state();
    plring = 4;
    player->xposi.w.h = 320;
    player->yposi.w.h = 180;
    queue_actor(&actwk[10]);
    TEST_ASSERT_EQ_INT(ctx, -1, playdamageset(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 17, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 320, actwk[10].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 180, actwk[10].yposi.w.h);

    reset_coli3_state();
    plring = 4;
    TEST_ASSERT_EQ_INT(ctx, -1, playdamageset(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 4, player->r_no0);

    reset_coli3_state();
    debugflag.w = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, playdamageset(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 4, player->r_no0);

    reset_coli3_state();
    editmode.w = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, playdieset(player));
    TEST_ASSERT_EQ_INT(ctx, 0, jumpcolsub_count);

    reset_coli3_state();
    plpower_m = 1;
    player->yposi.w.h = 456;
    player->sproffset = 2;
    TEST_ASSERT_EQ_INT(ctx, -1, playdieset(player));
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 6, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1792, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 32770, player->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 147, soundset_requests[0]);
}

static void test_special_and_boss_collision_paths(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *boss = &actwk[1];

    reset_coli3_state();
    boss->colino = 192 + 31;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolspecial(player, boss));
    TEST_ASSERT_EQ_INT(ctx, 1, boss->colicnt);

    reset_coli3_state();
    boss->colino = 192 + 58;
    player->mstno.b.h = 2;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolspecial(player, boss));
    TEST_ASSERT_EQ_INT(ctx, 1, boss->colicnt);

    reset_coli3_state();
    boss->colino = 192 + 60;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolspecial(player, boss));
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colicnt);

    reset_coli3_state();
    bossstart = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, eggman_chk(player, boss, 60));
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colicnt);

    reset_coli3_state();
    bossstart = 1;
    plpower_a = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, eggman_chk(player, boss, 60));
    TEST_ASSERT_EQ_INT(ctx, 4, boss->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colino);

    reset_coli3_state();
    bossstart = 1;
    boss_1(player, boss, 59);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colicnt);

    reset_coli3_state();
    player->cddat = 20;
    player->xposi.w.h = 120;
    boss->xposi.w.h = 100;
    boss_4(player, boss, 63);
    TEST_ASSERT_EQ_INT(ctx, 21, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1024, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 512, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat & 20);

    reset_coli3_state();
    player->cddat = 0;
    boss_4(player, boss, 63);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);

    reset_coli3_state();
    plpower_m = 1;
    boss_4(player, boss, 62);
    TEST_ASSERT_EQ_INT(ctx, 1, boss->colicnt);

    reset_coli3_state();
    plpower_m = 1;
    boss_5(player, boss, 61);
    TEST_ASSERT_EQ_INT(ctx, 1, boss->colicnt);

    reset_coli3_state();
    player->mstno.b.h = 0;
    main_attack(player, boss);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colicnt);
}

static void test_pcolplay2_sets_enemy_damage_flag(test_context *ctx) {
    sprite_status *player = &actwk[0];
    sprite_status *enemy = &actwk[1];

    reset_coli3_state();
    plpower_m = 1;
    TEST_ASSERT_EQ_INT(ctx, -1, pcolplay2(player, enemy));
    TEST_ASSERT_EQ_INT(ctx, 128, enemy->cddat & 128);
}

TEST_MAIN_BEGIN;
test_coli3_tables_capture_literal_data(&ctx);
test_pcol_scans_active_collision_slots(&ctx);
test_collision_table_x_and_y_bounds(&ctx);
test_hit_dispatch_routes_by_colino_high_bits(&ctx);
test_item_collision_preserves_bumper_behaviors(&ctx);
test_normal_enemy_collision_scores_and_bounces(&ctx);
test_player_damage_and_death_paths(&ctx);
test_special_and_boss_collision_paths(&ctx);
test_pcolplay2_sets_enemy_damage_flag(&ctx);
TEST_MAIN_END

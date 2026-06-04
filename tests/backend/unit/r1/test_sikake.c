#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union editmode;
Uint8 plpower_a;
Uint8 plpower_m;

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);

#include "src/r1/sikake.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    ++playdamageset_count;
    playdamageset_player = pActwk;
    playdamageset_actor = pColliAct;
    return 0;
}

static void reset_sikake_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&editmode, 0, sizeof(editmode));
    plpower_a = 0;
    plpower_m = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
}

static void place_overlapping(sprite_status *actor, sprite_status *player) {
    actor->xposi.l = 100 << 16;
    actor->yposi.l = 100 << 16;
    player->xposi.l = 100 << 16;
    player->yposi.l = 100 << 16;
    player->sprhs = 8;
    player->sprvsize = 8;
    player->yspeed.w = 0;
}

static void test_sikake_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, tobita1chg[0] == tobita1chg0);
    TEST_ASSERT_TRUE(ctx, tobita1chg[4] == tobitastop2);
    TEST_ASSERT_TRUE(ctx, tobita1_pat[0] == &tobita_a);
    TEST_ASSERT_TRUE(ctx, tobita1_pat[5] == &tobita_r_c);
    TEST_ASSERT_TRUE(ctx, hari12pat[0] == &pat0);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SIKAKE_BASE, tobita_a.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SIKAKE_BASE + 5,
                       tobita_r_c.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SIKAKE_BASE + 6, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 255, tobita1chg0[15]);
}

static void test_colli_pl_sikake_sets_and_clears_collision_bit(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    place_overlapping(actor, player);

    TEST_ASSERT_EQ_INT(ctx, -1, colli_pl_sikake(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 8, actor->cddat & 8);

    player->xposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    place_overlapping(actor, player);
    player->yposi.w.h = 200;
    actor->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    place_overlapping(actor, player);
    player->xposi.w.h = 80;
    actor->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    place_overlapping(actor, player);
    player->yposi.w.h = 88;
    actor->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);
}

static void test_colli_pl_sikake_d_applies_player_gates(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->cddat = 8;
    editmode.b.h = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake_d(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    reset_sikake_state();
    place_overlapping(actor, player);
    player->r_no0 = 6;
    actor->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake_d(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    reset_sikake_state();
    place_overlapping(actor, player);
    player->yspeed.w = -1;
    actor->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 0, colli_pl_sikake_d(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 8);

    reset_sikake_state();
    place_overlapping(actor, player);
    TEST_ASSERT_EQ_INT(ctx, -1, colli_pl_sikake_d(actor, player, 5));
    TEST_ASSERT_EQ_INT(ctx, 8, actor->cddat & 8);
}

static void test_tobita_init_sets_default_and_flipped_variants(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sikake_state();
    tobita_init(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == tobita1_pat);

    reset_sikake_state();
    actor->userflag.b.h = 1;
    actor->actflg = 1;
    actor->cddat = 1;
    tobita_init(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
}

static void test_tobita_stop_states_mount_player_on_collision(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 2;
    player->sprvsize = 8;
    tobita_stop_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 92, player->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == tobita1chg);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 4;
    player->sprvsize = 8;
    tobita_stop_l(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 92, player->yposi.w.h);

    reset_sikake_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    tobita_stop_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
}

static void test_tobita_ride_states_reset_or_keep_oscillation(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    actor->r_no0 = 6;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    tobita_ride_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);

    reset_sikake_state();
    actor->r_no0 = 6;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    player->cddat = 2;
    tobita_ride_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 64, tobita1_get_work(actor)->wobble_timer);

    reset_sikake_state();
    actor->r_no0 = 8;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    player->cddat = 2;
    tobita_ride_l(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 64, tobita1_get_work(actor)->wobble_timer);

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 6;
    tobita_ride_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
}

static void test_tobita_yure_states_bounce_and_reset_timer(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 10;
    player->yspeed.w = 3000;
    player->cddat = 32;
    player_work_get(player)->jump_lock = 5;
    tobita_yure_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 63, tobita1_get_work(actor)->wobble_timer);
    TEST_ASSERT_EQ_INT(ctx, -2560, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 14, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 105, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat & 32);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->jump_lock);

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 12;
    player->yspeed.w = 8;
    player->cddat = 6;
    player_work_get(player)->jump_started = 9;
    tobita_yure_l(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 63, tobita1_get_work(actor)->wobble_timer);
    TEST_ASSERT_EQ_INT(ctx, -264, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 19, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 16, player->cddat & 16);

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 10;
    player->yspeed.w = 8;
    player->cddat = 6;
    player_work_get(player)->jump_started = 9;
    tobita_yure_r(actor);
    TEST_ASSERT_EQ_INT(ctx, -264, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 19, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 16, player->cddat & 16);

    reset_sikake_state();
    place_overlapping(actor, player);
    actor->r_no0 = 12;
    player->yspeed.w = 3000;
    tobita_yure_l(actor);
    TEST_ASSERT_EQ_INT(ctx, -2560, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 14, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 105, player->yposi.w.h);

    reset_sikake_state();
    actor->r_no0 = 10;
    actor->mstno.b.h = 1;
    tobita1_get_work(actor)->wobble_timer = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    tobita_yure_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 64, tobita1_get_work(actor)->wobble_timer);

    reset_sikake_state();
    actor->r_no0 = 12;
    actor->mstno.b.h = 2;
    tobita1_get_work(actor)->wobble_timer = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;
    player->sprhs = 8;
    player->sprvsize = 8;
    tobita_yure_l(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 64, tobita1_get_work(actor)->wobble_timer);
}

static void test_tobita1_wrapper_dispatches_and_frames_out(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sikake_state();
    actor->r_no0 = 0;
    tobita1(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_sikake_state();
    actor->r_no0 = 12;
    tobita1_get_work(actor)->wobble_timer = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 100;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 8;
    tobita1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_hari_init_sets_normal_and_inverted_variants(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    player->yposi.w.h = 200;
    actor->yposi.w.h = 100;
    hari_init(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == hari12pat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_sikake_state();
    actor->actflg = 2;
    player->yposi.w.h = 100;
    actor->yposi.w.h = 100;
    hari_init(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 131, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_hari_normal_damage_and_gate_paths(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_sikake_state();
    actor->yposi.w.h = 100;
    actor->cddat = 8;
    player->yposi.w.h = 90;
    player->yspeed.w = 5;
    player->r_no0 = 0;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 85, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == actor);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    player->yposi.w.h = 200;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    player->yposi.w.h = 100;
    hitchk_result = 0;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    player->yposi.w.h = 100;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    actor->cddat = 8;
    player->yposi.w.h = 100;
    player->r_no0 = 4;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    actor->cddat = 8;
    player->yposi.w.h = 100;
    player_work_get(player)->damage_invulnerability_timer = 1;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    actor->cddat = 8;
    player->yposi.w.h = 100;
    plpower_a = 1;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_sikake_state();
    actor->yposi.w.h = 100;
    actor->cddat = 8;
    player->yposi.w.h = 100;
    plpower_m = 1;
    hitchk_result = 1;
    hari_normal(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_hariyama_wrapper_dispatches(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_sikake_state();
    actwk[0].yposi.w.h = 200;
    actor->yposi.w.h = 100;
    hariyama(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);

    reset_sikake_state();
    actor->r_no0 = 2;
    actor->yposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    hariyama(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_sikake_tables_capture_literal_data(&ctx);
    test_colli_pl_sikake_sets_and_clears_collision_bit(&ctx);
    test_colli_pl_sikake_d_applies_player_gates(&ctx);
    test_tobita_init_sets_default_and_flipped_variants(&ctx);
    test_tobita_stop_states_mount_player_on_collision(&ctx);
    test_tobita_ride_states_reset_or_keep_oscillation(&ctx);
    test_tobita_yure_states_bounce_and_reset_timer(&ctx);
    test_tobita1_wrapper_dispatches_and_frames_out(&ctx);
    test_hari_init_sets_normal_and_inverted_variants(&ctx);
    test_hari_normal_damage_and_gate_paths(&ctx);
    test_hariyama_wrapper_dispatches(&ctx);
TEST_MAIN_END

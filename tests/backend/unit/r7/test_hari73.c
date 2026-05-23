#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 plpower_m;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int soundset_count;
static Sint16 soundset_requests[8];
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int hari_spdadd_count;
static sprite_status *hari_spdadd_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
void hari_spdadd(sprite_status *pActwk);

#include "src/r7/hari73.c"

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

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) / sizeof(soundset_requests[0]))) {
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

void hari_spdadd(sprite_status *pActwk) {
    ++hari_spdadd_count;
    hari_spdadd_actor = pActwk;
    pActwk->xposi.w.h += pActwk->xspeed.w;
    pActwk->yposi.w.h += pActwk->yspeed.w;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    ((Sint16 *)actor)[index] = value;
}

static Sint16 get_actor_word(sprite_status *actor, int index) {
    return ((Sint16 *)actor)[index];
}

static void reset_hari73_state(void) {
    memset(actwk, 0, sizeof(actwk));
    plpower_m = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    hari_spdadd_count = 0;
    hari_spdadd_actor = 0;
}

static void test_hari73_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, hari73_pat[0] == &hari73_pat0);
    TEST_ASSERT_TRUE(ctx, hari73_pat[1] == &hari73_pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, hari73_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, hari73_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, hari73_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 419, hari73_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -4, hari73_pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 420, hari73_pat1.spra[0].index);
}

static void test_hari73_init_search_and_frameout_paths(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_hari73_state();
    spike->xposi.w.h = 200;
    spike->yposi.w.h = 80;
    actwk[9].actno = 47;
    actwk[9].xposi.w.h = 100;
    actwk[9].sprhsize = 16;
    hari73(spike);
    TEST_ASSERT_EQ_INT(ctx, 2, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, spike->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, spike->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1276, spike->sproffset);
    TEST_ASSERT_TRUE(ctx, spike->patbase == hari73_pat);
    TEST_ASSERT_EQ_INT(ctx, 16, spike->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, spike->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, spike->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, get_actor_word(spike, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_hari73_state();
    spike->xposi.w.h = 100;
    actwk[9].actno = 47;
    actwk[9].xposi.w.h = 120;
    actwk[9].sprhsize = 16;
    hari73_init(spike);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == spike);

    reset_hari73_state();
    spike->xposi.w.h = 200;
    hari73_init(spike);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_hari73_state();
    actwk[127].actno = 47;
    TEST_ASSERT_EQ_INT(ctx, 0, act_search(&spike, 47));
    TEST_ASSERT_TRUE(ctx, spike == &actwk[127]);
    actwk[127].actno = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, act_search(&spike, 47));
}

static void test_hari73_move_collision_damage_guards(test_context *ctx) {
    sprite_status *spike = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_hari73_state();
    hitchk_result = 1;
    player->colino = 99;
    player->cddat = 0;
    spike->actfree[11] = 7;
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[11]);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_hari73_state();
    hitchk_result = 1;
    player->cddat = 8;
    player->r_no0 = 2;
    player->actfree[6] = 0;
    player->yposi.l = 100 << 16;
    player->yspeed.w = 8;
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 1, spike->actfree[11]);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) - (8 << 8), player->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == spike);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);

    reset_hari73_state();
    hitchk_result = 1;
    player->cddat = 8;
    player->r_no0 = 2;
    plpower_m = 1;
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_hari73_state();
    hitchk_result = 1;
    player->cddat = 8;
    player->r_no0 = 4;
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_hari73_state();
    hitchk_result = 1;
    player->cddat = 8;
    player->r_no0 = 2;
    player->actfree[6] = 1;
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_hari73_msnc_hit_spawns_fragments(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_hari73_state();
    spike->xposi.w.h = 200;
    spike->yposi.w.h = 96;
    spike->sprhsize = 16;
    spike->actfree[11] = 1;
    actwk[9].xposi.w.h = 208;
    set_actor_word(spike, 23, 9);
    queue_actor(&actwk[20]);
    queue_actor(&actwk[21]);
    queue_actor(&actwk[22]);
    queue_actor(&actwk[23]);
    hari73_move(spike);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 163, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[20].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[20].patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[20].actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[20].sprpri);
    TEST_ASSERT_EQ_INT(ctx, 188, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 96, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 576, actwk[20].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -512, actwk[20].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, get_actor_word(&actwk[20], 26));
    TEST_ASSERT_EQ_INT(ctx, 16, get_actor_word(&actwk[20], 27));
    TEST_ASSERT_EQ_INT(ctx, 212, actwk[23].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 480, actwk[23].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 10, get_actor_word(&actwk[23], 26));
    TEST_ASSERT_EQ_INT(ctx, 16, get_actor_word(&actwk[23], 27));
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_hari73_state();
    msnc_hit(spike);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_hari73_tobi_animation_gate(test_context *ctx) {
    sprite_status *piece = &actwk[5];

    reset_hari73_state();
    piece->r_no0 = 4;
    piece->xspeed.w = 3;
    piece->yspeed.w = -2;
    piece->actfree[10] = 0;
    hari73(piece);
    TEST_ASSERT_EQ_INT(ctx, 1, hari_spdadd_count);
    TEST_ASSERT_TRUE(ctx, hari_spdadd_actor == piece);
    TEST_ASSERT_EQ_INT(ctx, 3, piece->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2, piece->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, piece->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_hari73_state();
    piece->r_no0 = 4;
    piece->actfree[10] = 1;
    hari73_tobi(piece);
    TEST_ASSERT_EQ_INT(ctx, 2, piece->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == piece);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
test_hari73_tables_capture_literal_data(&ctx);
test_hari73_init_search_and_frameout_paths(&ctx);
test_hari73_move_collision_damage_guards(&ctx);
test_hari73_msnc_hit_spawns_fragments(&ctx);
test_hari73_tobi_animation_gate(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static Sint16 emycol_d_results[8];
static int emycol_d_result_count;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[8];

Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r5/kumo.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    Sint16 result = 0;

    (void)pActwk;
    ++emycol_d_count;
    if (emycol_d_count <= emycol_d_result_count) {
        result = emycol_d_results[emycol_d_count - 1];
    }
    return result;
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

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
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

static void reset_logs(void) {
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_count = 0;
    memset(emycol_d_results, 0, sizeof(emycol_d_results));
    emycol_d_result_count = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_kumo_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_emycol(Sint16 result) {
    emycol_d_results[emycol_d_result_count++] = result;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void assert_visible_callbacks(test_context *ctx,
                                     sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void init_kumo_for_move(sprite_status *actor) {
    reset_kumo_state();
    actor->actno = 35;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    queue_emycol(0);
    kumo(actor);
    reset_logs();
}

static void test_patterns_and_animation_tables_capture_literals(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg_e[0] == pchg_e_00);
    TEST_ASSERT_TRUE(ctx, pchg_b[0] == pchg_b_00);
    TEST_ASSERT_TRUE(ctx, pchg_tama[0] == pchg_tama_00);
    TEST_ASSERT_EQ_INT(ctx, 7, pchg_e_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 15, pchg_b_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_tama_00[3]);
    TEST_ASSERT_TRUE(ctx, pat_kumo_e[0] == &kumo_e_pat00);
    TEST_ASSERT_TRUE(ctx, pat_kumo_b[2] == &kumo_b_pat02);
    TEST_ASSERT_TRUE(ctx, pat_tama[1] == &tama_pat01);
    TEST_ASSERT_EQ_INT(ctx, -28, kumo_e_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -25, kumo_e_pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 409, kumo_e_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 414, kumo_b_pat02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 416, tama_pat01.spra[0].index);
}

static void test_enemy_suicide_short_circuits_non_projectile(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kumo_state();
    enemy_suicide_result = 1;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
}

static void test_init_normal_and_black_variants(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kumo_state();
    actor->actno = 35;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    queue_emycol(0);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9236, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 53, actor->colino);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kumo_e);
    TEST_ASSERT_EQ_INT(ctx, -524288, get_actfree_long(actor, 6));
    TEST_ASSERT_EQ_INT(ctx, 16384, get_actfree_long(actor, 10));
    TEST_ASSERT_EQ_INT(ctx, 50, actor->yposi.w.h);
    assert_visible_callbacks(ctx, actor);

    reset_kumo_state();
    actor = &actwk[4];
    actor->actno = 35;
    actor->userflag.b.h = 1;
    actor->userflag.b.l = 1;
    actor->yposi.w.h = 50;
    queue_emycol(-2);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kumo_b);
    TEST_ASSERT_EQ_INT(ctx, -327680, get_actfree_long(actor, 6));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 56, actor->yposi.w.h);
}

static void test_jump_uses_stored_speed_and_lands(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kumo_state();
    actor->actno = 35;
    actor->r_no0 = 2;
    actor->patno = 2;
    actor->yposi.w.h = 100;
    set_actfree_long(actor, 6, -524288);
    queue_emycol(0);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 84, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -507904, get_actfree_long(actor, 10));

    reset_kumo_state();
    actor = &actwk[4];
    actor->actno = 35;
    actor->r_no0 = 4;
    actor->yposi.w.h = 100;
    set_actfree_long(actor, 10, -16384);
    queue_emycol(-4);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 104, actor->yposi.w.h);
}

static void test_move_timer_patches_and_cycles_back_to_jump(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_kumo_for_move(actor);
    actor->r_no0 = 6;
    actwk[0].yposi.w.h = 400;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 54, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 179, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);

    reset_logs();
    set_actfree_word(actor, 0, 1);
    actwk[0].yposi.w.h = 400;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_player_check_flips_direction_and_enters_fire_wait(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_kumo_for_move(actor);
    actor->r_no0 = 8;
    set_actfree_word(actor, 0, 180);
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 100;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 60, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    init_kumo_for_move(actor);
    actor->r_no0 = 8;
    actor->actflg |= 1;
    actor->cddat |= 1;
    set_actfree_word(actor, 0, 180);
    actwk[0].xposi.w.h = 110;
    actwk[0].yposi.w.h = 100;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
}

static void test_black_variant_skips_player_check(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kumo_state();
    actor->actno = 35;
    actor->userflag.b.h = 1;
    actor->r_no0 = 8;
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 100;
    set_actfree_word(actor, 0, 2);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
}

static void test_fire_wait_and_projectile_spawn(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *shot = &actwk[40];

    reset_kumo_state();
    actor->actno = 35;
    actor->r_no0 = 10;
    set_actfree_word(actor, 0, 2);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, get_actfree_word(actor, 0));

    reset_logs();
    set_actfree_word(actor, 0, 1);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);

    reset_logs();
    actor->actno = 35;
    actor->r_no0 = 12;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->actflg = 128;
    actor->sproffset = 9236;
    queue_actwkchk(shot);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 35, shot->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, shot->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 124, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, shot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, shot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9236, shot->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 12, shot->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 12, shot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, shot->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 247, shot->colino);
    TEST_ASSERT_TRUE(ctx, shot->patbase == pat_tama);
    TEST_ASSERT_EQ_INT(ctx, 196608, get_actfree_long(shot, 10));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);

    reset_kumo_state();
    actor = &actwk[4];
    shot = &actwk[40];
    actor->actno = 35;
    actor->r_no0 = 12;
    actor->xposi.w.h = 100;
    actor->actflg = 1;
    queue_actwkchk(shot);

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 76, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -196608, get_actfree_long(shot, 10));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_kumo_state();
    actor = &actwk[4];
    actor->r_no0 = 12;

    kumo(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
}

static void test_projectile_moves_or_grabs_player(test_context *ctx) {
    sprite_status *shot = &actwk[40];

    reset_kumo_state();
    shot->userflag.b.h = -1;
    shot->xposi.l = 100 << 16;
    set_actfree_long(shot, 10, 196608);

    kumo(shot);

    TEST_ASSERT_EQ_INT(ctx, 103, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == shot);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_tama);
    assert_visible_callbacks(ctx, shot);

    reset_kumo_state();
    shot = &actwk[40];
    shot->userflag.b.h = -1;
    shot->colicnt = 1;
    actwk[0].r_no0 = 0;
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 160;
    actwk[0].xspeed.w = 123;
    actwk[0].mspeed.w = -456;

    kumo(shot);

    TEST_ASSERT_EQ_INT(ctx, 0, shot->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 119, get_actfree_word(shot, 0));
    TEST_ASSERT_EQ_INT(ctx, 300, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 160, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    assert_visible_callbacks(ctx, shot);

    reset_logs();
    set_actfree_word(shot, 0, 1);

    kumo(shot);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == shot);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_projectile_collision_skips_grab_for_player_states(
    test_context *ctx) {
    sprite_status *shot = &actwk[40];

    reset_kumo_state();
    shot->userflag.b.h = -1;
    shot->colicnt = 1;
    shot->xposi.l = 100 << 16;
    actwk[0].r_no0 = 4;
    set_actfree_long(shot, 10, 196608);

    kumo(shot);

    TEST_ASSERT_EQ_INT(ctx, 0, get_actfree_word(shot, 0));
    TEST_ASSERT_EQ_INT(ctx, 103, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_kumo_state();
    shot = &actwk[40];
    shot->userflag.b.h = -1;
    shot->colicnt = 1;
    shot->xposi.l = 100 << 16;
    actwk[0].r_no0 = 0;
    set_actfree_word(&actwk[0], 6, 1);
    set_actfree_long(shot, 10, 196608);

    kumo(shot);

    TEST_ASSERT_EQ_INT(ctx, 0, get_actfree_word(shot, 0));
    TEST_ASSERT_EQ_INT(ctx, 103, shot->xposi.w.h);
}

TEST_MAIN_BEGIN;
    test_patterns_and_animation_tables_capture_literals(&ctx);
    test_enemy_suicide_short_circuits_non_projectile(&ctx);
    test_init_normal_and_black_variants(&ctx);
    test_jump_uses_stored_speed_and_lands(&ctx);
    test_move_timer_patches_and_cycles_back_to_jump(&ctx);
    test_player_check_flips_direction_and_enters_fire_wait(&ctx);
    test_black_variant_skips_player_check(&ctx);
    test_fire_wait_and_projectile_spawn(&ctx);
    test_projectile_moves_or_grabs_player(&ctx);
    test_projectile_collision_skips_grab_for_player_states(&ctx);
TEST_MAIN_END

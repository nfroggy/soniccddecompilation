#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_pattern;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r6/semi.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_pattern = pat_dat;
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
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_semi_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_pattern = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void assert_common_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void place_player_near(sprite_status *actor, Sint16 x_delta,
                              Sint16 y_delta) {
    actwk[0].xposi.w.h = actor->xposi.w.h + x_delta;
    actwk[0].yposi.w.h = actor->yposi.w.h + y_delta;
}

static void test_semi_main_respects_suicide_and_initializes(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    enemy_suicide_result = -1;
    semi(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_semi_state();
    actor->actno = 60;
    actor->userflag.b.l = 3;
    actor->userflag.b.h = 0;
    semi(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 19, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 19, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 42152, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 54, actor->colino);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_semi_e);
    assert_common_callbacks(ctx, actor);
}

static void test_semi_init_uses_b_pattern_and_wait_countdown(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    actor->userflag.b.h = 1;
    actor->userflag.b.l = 2;

    act_init(actor);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_semi_b);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);

    act_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    act_wait(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
}

static void test_semi_wait1_checks_player_window(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    actor->r_no0 = 4;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 500;

    place_player_near(actor, 0, -97);
    act_wait1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);

    place_player_near(actor, 121, 0);
    act_wait1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);

    place_player_near(actor, 120, 96);
    act_wait1(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
}

static void test_semi_main_dispatches_active_states(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    actor->r_no0 = 4;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    place_player_near(actor, 0, 0);
    semi(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    assert_common_callbacks(ctx, actor);

    reset_semi_state();
    actor->r_no0 = 6;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    semi(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->yposi.w.h);
    assert_common_callbacks(ctx, actor);

    reset_semi_state();
    actor->r_no0 = 10;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    semi(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    assert_common_callbacks(ctx, actor);
}

static void test_semi_movea_tracks_direction_and_variant(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    actor->r_no0 = 4;
    place_player_near(actor, 40, 0);
    act_wait1(actor);
    act_movea(actor);
    act_movea1(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg);

    reset_semi_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    actor->r_no0 = 4;
    actor->userflag.b.h = 1;
    place_player_near(actor, -40, 0);
    act_wait1(actor);
    act_movea(actor);
    for (int i = 0; i < 42; ++i) {
        act_movea1(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 131, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 65, actor->yposi.w.h);
}

static void test_semi_moveb_spawns_bomb_and_handles_misses(test_context *ctx) {
    sprite_status *actor = &actwk[5];
    sprite_status *bomb_actor = &actwk[20];

    reset_semi_state();
    actor->actno = 60;
    actor->actflg = 128;
    actor->sprpri = 3;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 70;
    actor->r_no0 = 4;
    place_player_near(actor, 20, 0);
    act_wait1(actor);
    act_moveb(actor);
    queue_actor(bomb_actor);
    act_moveb1(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 299, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 60, bomb_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 300, bomb_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, bomb_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1, bomb_actor->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 128, bomb_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, bomb_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_semi_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 70;
    actor->r_no0 = 12;
    actor->userflag.b.h = 1;
    act_moveb(actor);
    act_moveb1(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 299, actor->xposi.w.h);

    reset_semi_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 70;
    actor->r_no0 = 10;
    place_player_near(actor, 0, 200);
    act_moveb(actor);
    act_moveb1(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_semi_moveb_handles_allocation_failure(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_semi_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 70;
    actor->r_no0 = 4;
    place_player_near(actor, -20, 0);

    act_wait1(actor);
    act_moveb(actor);
    act_moveb1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 301, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_semi_bomb_falls_waits_blinks_and_dies(test_context *ctx) {
    sprite_status *bomb_actor = &actwk[9];

    reset_semi_state();
    bomb_actor->userflag.b.l = -1;
    bomb_actor->actno = 60;
    bomb_actor->yposi.w.h = 100;
    emycol_d_result = -3;

    semi(bomb_actor);

    TEST_ASSERT_EQ_INT(ctx, 4, bomb_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 183, bomb_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 6, bomb_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 6, bomb_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 6, bomb_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 33992, bomb_actor->sproffset);
    TEST_ASSERT_TRUE(ctx, bomb_actor->patbase == pat_bomb);
    TEST_ASSERT_EQ_INT(ctx, 97, bomb_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    assert_common_callbacks(ctx, bomb_actor);

    for (int i = 0; i < 121; ++i) {
        bomb_wait(bomb_actor);
    }
    TEST_ASSERT_EQ_INT(ctx, 6, bomb_actor->r_no0);

    patchg_count = 0;
    for (int i = 0; i < 121; ++i) {
        bomb_blink(bomb_actor);
    }
    TEST_ASSERT_EQ_INT(ctx, 8, bomb_actor->r_no0);
    TEST_ASSERT_TRUE(ctx, patchg_actor == bomb_actor);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg_bomb);

    bomb_actor->actflg = 128;
    bomb_die(bomb_actor);
    TEST_ASSERT_EQ_INT(ctx, 24, bomb_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 0, bomb_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, bomb_actor->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    reset_semi_state();
    bomb_actor = &actwk[9];
    bomb_actor->actflg = 0;
    bomb_die(bomb_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_semi_main_dispatches_bomb_states(test_context *ctx) {
    sprite_status *bomb_actor = &actwk[9];

    reset_semi_state();
    bomb_actor->userflag.b.l = -1;
    bomb_actor->r_no0 = 4;
    semi(bomb_actor);
    TEST_ASSERT_EQ_INT(ctx, 6, bomb_actor->r_no0);
    assert_common_callbacks(ctx, bomb_actor);

    reset_semi_state();
    bomb_actor->userflag.b.l = -1;
    bomb_actor->r_no0 = 6;
    semi(bomb_actor);
    TEST_ASSERT_EQ_INT(ctx, 8, bomb_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == bomb_actor);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg_bomb);
    assert_common_callbacks(ctx, bomb_actor);

    reset_semi_state();
    bomb_actor->userflag.b.l = -1;
    bomb_actor->r_no0 = 8;
    bomb_actor->actflg = 128;
    semi(bomb_actor);
    TEST_ASSERT_EQ_INT(ctx, 24, bomb_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    assert_common_callbacks(ctx, bomb_actor);
}

static void test_semi_bomb_fall_without_ground_keeps_falling(test_context *ctx) {
    sprite_status *bomb_actor = &actwk[9];

    reset_semi_state();
    bomb_actor->yposi.w.h = 100;
    bomb_init(bomb_actor);
    emycol_d_result = 2;

    bomb_fall(bomb_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, bomb_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, bomb_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
}

TEST_MAIN_BEGIN;
    test_semi_main_respects_suicide_and_initializes(&ctx);
    test_semi_init_uses_b_pattern_and_wait_countdown(&ctx);
    test_semi_wait1_checks_player_window(&ctx);
    test_semi_main_dispatches_active_states(&ctx);
    test_semi_movea_tracks_direction_and_variant(&ctx);
    test_semi_moveb_spawns_bomb_and_handles_misses(&ctx);
    test_semi_moveb_handles_allocation_failure(&ctx);
    test_semi_bomb_falls_waits_blinks_and_dies(&ctx);
    test_semi_main_dispatches_bomb_states(&ctx);
    test_semi_bomb_fall_without_ground_keeps_falling(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
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
static Sint16 emycol_d_queue[8];
static int emycol_d_queue_count;
static int emycol_d_queue_index;
static Sint16 emycol_r_result;
static Sint16 emycol_l_result;
static Sint16 emycol_d2_result;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_r(sprite_status *pActwk, Uint8 iD5);
Sint16 emycol_l(sprite_status *pActwk, Uint8 iD5);
Sint16 emycol_d2(sprite_status *pActwk, Sint16 iD3);
Uint8 CCset(Sint32 lSrc, Sint32 lDst);

#include "src/r3/tentou.c"

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
    if (emycol_d_queue_index < emycol_d_queue_count) {
        return emycol_d_queue[emycol_d_queue_index++];
    }
    return emycol_d_result;
}

Sint16 emycol_r(sprite_status *pActwk, Uint8 iD5) {
    (void)pActwk;
    (void)iD5;
    return emycol_r_result;
}

Sint16 emycol_l(sprite_status *pActwk, Uint8 iD5) {
    (void)pActwk;
    (void)iD5;
    return emycol_l_result;
}

Sint16 emycol_d2(sprite_status *pActwk, Sint16 iD3) {
    (void)pActwk;
    (void)iD3;
    return emycol_d2_result;
}

Uint8 CCset(Sint32 lSrc, Sint32 lDst) {
    return lSrc - lDst < 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_emycol_d(Sint16 result) {
    emycol_d_queue[emycol_d_queue_count++] = result;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    memcpy(&actor->actfree[offset], &value, sizeof(value));
}

static void reset_tentou_state(void) {
    memset(actwk, 0, sizeof(actwk));
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
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
    memset(emycol_d_queue, 0, sizeof(emycol_d_queue));
    emycol_d_queue_count = 0;
    emycol_d_queue_index = 0;
    emycol_r_result = 0;
    emycol_l_result = 0;
    emycol_d2_result = 0;
}

static void reset_tentou_logs(void) {
    enemy_suicide_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    emycol_d_queue_count = 0;
    emycol_d_queue_index = 0;
}

static void test_tentou_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, tentou_tbl[0] == &ten_a_init);
    TEST_ASSERT_TRUE(ctx, tentou_tbl[5] == &ten_a_gake);
    TEST_ASSERT_TRUE(ctx, tentou1_tbl[0] == &ten_b_init);
    TEST_ASSERT_TRUE(ctx, tentou1_tbl[4] == &ten_b_die);
    TEST_ASSERT_TRUE(ctx, pchg0[0] == pchg0_0);
    TEST_ASSERT_TRUE(ctx, pchg1[0] == pchg1_0);
    TEST_ASSERT_TRUE(ctx, pat_ten_b_tentou[0] == &ten_b_tentou0);
    TEST_ASSERT_TRUE(ctx, pat_e_tentou[2] == &e_tentou2);
    TEST_ASSERT_TRUE(ctx, tentou_bomten_b_pat[1] == &bomten_b_1);
    TEST_ASSERT_EQ_INT(ctx, 474, ten_b_tentou0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 479, e_tentou2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 481, bomten_b_1.spra[0].index);
}

static void test_tentou_wrapper_routes_and_obeys_suicide(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    actor->userflag.b.h = 1;
    tentou(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_tentou_state();
    enemy_suicide_result = -1;
    tentou(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_tentou_state();
    emycol_d_result = 9;
    tentou(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_tentou_a_init_and_fall_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    emycol_d_result = 9;
    ten_a_init(actor);

    reset_tentou_state();
    actor->userflag.w = -1;
    emycol_d_result = 8;
    ten_a_init(actor);

    reset_tentou_state();
    actor->yposi.w.h = 20;
    emycol_d_result = -4;
    ten_a_fall(actor);

    reset_tentou_state();
    set_actfree_long(actor, 0, 1);
    emycol_d_result = -8;
    ten_a_fall(actor);
}

static void test_tentou_a_wait_and_direct_bomb_spawn_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 80;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actor->actflg = 129;
    actor->cddat = 1;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);

    ten_a_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_tentou_state();
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 100;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actor->actflg = 0;
    actor->cddat = 0;
    ten_a_wait(actor);

    reset_tentou_state();
    actwk[0].xposi.w.h = 20;
    actwk[0].yposi.w.h = 400;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actor->actflg = 128;
    actor->cddat = 0;

    ten_a_wait(actor);

}

static void test_tentou_a_hover_up_and_abs(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    actor->yposi.l = 0x00200000;
    ten_a_up(actor);

    reset_tentou_state();
    set_actor_word(actor, 29, 15);
    set_actfree_long(actor, 4, 12288);
    ten_a_hover(actor);
    TEST_ASSERT_EQ_INT(ctx, 5, abs(-5));
    TEST_ASSERT_EQ_INT(ctx, 7, abs(7));
}

static void test_tentou_a_lr_ground_wall_and_ledge_paths(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    actor->cddat = 1;
    actor->userflag.b.h = 0;
    actor->actfree[21] = 255;
    set_actor_word(actor, 29, 127);
    actor->actno = 22;
    queue_actor(&actwk[10]);
    emycol_r_result = 0;
    emycol_d_result = 5;
    ten_a_lr(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 22, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].userflag.b.h);

    reset_tentou_state();
    set_actfree_long(actor, 0, -1);
    emycol_l_result = -1;
    ten_a_lr(actor);

    reset_tentou_state();
    actor->cddat = 1;
    emycol_r_result = 0;
    emycol_d_result = 20;
    ten_a_lr(actor);

    reset_tentou_state();
    actor->r_no0 = 10;
    set_actfree_long(actor, 0, 1);
    actor->xposi.w.h = 100;
    actor->sprhsize = 8;
    emycol_d2_result = 16;
    ten_a_gake(actor);

}

static void test_tentou_a_range_covers_bounds_without_state_change(
    test_context *ctx) {
    reset_tentou_state();
    actwk[0].yposi.w.h = 100;
    actwk[1].yposi.w.h = 50;
    ten_a_range(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[1].yposi.w.h);

    actwk[0].yposi.w.h = -300;
    ten_a_range(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[1].yposi.w.h);
}

static void test_tentou_b_init_fall_wait_blink_and_die(test_context *ctx) {
    sprite_status *actor = &actwk[1];

    reset_tentou_state();
    emycol_d_result = 4;
    ten_b_init(actor);

    reset_tentou_state();
    actor->colicnt = 1;
    ten_b_fall(actor);

    reset_tentou_state();
    emycol_d_result = -3;
    ten_b_fall(actor);

    reset_tentou_state();
    set_actor_word(actor, 23, 1);
    ten_b_wait(actor);

    reset_tentou_state();
    actor->colicnt = 1;
    ten_b_wait(actor);

    reset_tentou_state();
    set_actor_word(actor, 23, 2);
    ten_b_blink(actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg1);
    reset_tentou_logs();
    set_actor_word(actor, 23, 1);
    ten_b_blink(actor);

    reset_tentou_state();
    actor->colicnt = 1;
    ten_b_blink(actor);

    reset_tentou_state();
    actor->actflg = 128;
    ten_b_die(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
}

TEST_MAIN_BEGIN;
test_tentou_tables_capture_literal_data(&ctx);
test_tentou_wrapper_routes_and_obeys_suicide(&ctx);
test_tentou_a_init_and_fall_paths(&ctx);
test_tentou_a_wait_and_direct_bomb_spawn_paths(&ctx);
test_tentou_a_hover_up_and_abs(&ctx);
test_tentou_a_lr_ground_wall_and_ledge_paths(&ctx);
test_tentou_a_range_covers_bounds_without_state_change(&ctx);
test_tentou_b_init_fall_wait_blink_and_die(&ctx);
TEST_MAIN_END

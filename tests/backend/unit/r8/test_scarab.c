#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 time_flag;
Sint16 time_stop;

static sprite_pattern item_pattern = {1, {{0, 0, 0, 900}}};
sprite_pattern *itempat[1] = {&item_pattern};
static Uint8 itemchg0[3] = {0, 1, 255};
Uint8 *itemchg[1] = {itemchg0};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[12];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int speedset_count;
static int ride_on_chk_count;
static Uint8 ride_on_chk_actno_seen[8];
static int ride_on_clr_count;
static int soundset_count;
static Sint16 soundset_requests[8];
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static Sint16 emycol_d_result;
static Sint16 emycol_d_queue[8];
static int emycol_d_queue_count;
static int emycol_d_queue_index;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s0(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void speedset(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/scarab.c"

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

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void speedset(sprite_status *pActwk) {
    ++speedset_count;
    pActwk->xposi.l += ((Sint32)pActwk->xspeed.w) << 8;
    pActwk->yposi.l += ((Sint32)pActwk->yspeed.w) << 8;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pPlayerwk;
    if (ride_on_chk_count < 8) {
        ride_on_chk_actno_seen[ride_on_chk_count] = pActwk->actno;
    }
    ++ride_on_chk_count;
    return 0;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pActwk;
    (void)pPlayerwk;
    ++ride_on_clr_count;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    if (emycol_d_queue_index < emycol_d_queue_count) {
        return emycol_d_queue[emycol_d_queue_index++];
    }
    return emycol_d_result;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_actor2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void queue_emycol(Sint16 value) {
    emycol_d_queue[emycol_d_queue_count++] = value;
}

static void set_controller_flag_ptr(sprite_status *actor, Uint8 *flag) {
    scarab_work_get(actor)->flag_work = flag;
}

static void reset_scarab_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    time_flag = 0;
    time_stop = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    speedset_count = 0;
    ride_on_chk_count = 0;
    memset(ride_on_chk_actno_seen, 0, sizeof(ride_on_chk_actno_seen));
    ride_on_clr_count = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_result = 0;
    memset(emycol_d_queue, 0, sizeof(emycol_d_queue));
    emycol_d_queue_count = 0;
    emycol_d_queue_index = 0;
}

static void test_scarab_tables_and_patterns(test_context *ctx) {
    reset_scarab_state();
    TEST_ASSERT_TRUE(ctx, pat_scarab[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_scarab[5] == &pat05);
    TEST_ASSERT_EQ_INT(ctx, 402, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 407, pat05.spra[0].index);
    TEST_ASSERT_TRUE(ctx, c_act_tbl[0] == &c_init);
    TEST_ASSERT_TRUE(ctx, c_act_tbl[2] == &c_move);
    TEST_ASSERT_TRUE(ctx, item_move_tbl[2] == &itemget);
    TEST_ASSERT_TRUE(ctx, e_act_tbl[1] == &e_move);
    TEST_ASSERT_TRUE(ctx, e_move_tbl[2] == &e1_wait);
    TEST_ASSERT_TRUE(ctx, pchg_n[0] == pchg_n00);
    TEST_ASSERT_TRUE(ctx, pchg_o[0] == pchg_o00);
}

static void test_controller_init_wait_and_make_paths(test_context *ctx) {
    sprite_status *ctrl = &actwk[3];

    reset_scarab_state();
    time_flag = 2;
    ctrl->userflag.b.h = 3;
    ctrl->cdsts = 4;
    ctrl->xposi.w.h = 1000;
    ctrl->yposi.w.h = 1000;
    actwk[0].xposi.w.h = 0;
    actwk[0].yposi.w.h = 0;
    scarab(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_scarab_state();
    ctrl->userflag.b.h = 3;
    ctrl->xposi.w.h = 100;
    ctrl->yposi.w.h = 80;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 80;
    queue_actor2(&actwk[10]);
    queue_actor2(&actwk[11]);
    c_init(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 47, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 76, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 76, actwk[11].xposi.w.h);

    reset_scarab_state();
    ctrl->userflag.b.h = 19;
    ctrl->userflag.b.l = 4;
    ctrl->xposi.w.h = 100;
    ctrl->yposi.w.h = 80;
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    queue_actor2(&actwk[10]);
    queue_actor2(&actwk[11]);
    c_make(ctrl);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[10].userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 124, actwk[11].xposi.w.h);

    reset_scarab_state();
    ctrl->userflag.b.h = 3;
    ctrl->userflag.b.l = -1;
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    queue_actor2(&actwk[10]);
    c_make(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 2, flagwork[5] & 2);

    reset_scarab_state();
    ctrl->userflag.b.h = 3;
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    c_make(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_scarab_state();
    flagwork[5] = 1;
    ctrl->userflag.b.h = 3;
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    c_make(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
}

static void test_controller_move_tracks_child_deletion(test_context *ctx) {
    sprite_status *ctrl = &actwk[3];

    reset_scarab_state();
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    scarab_work_get(ctrl)->child_enemy_index = 10;
    scarab_work_get(ctrl)->child_item_index = 11;
    actwk[10].actno = 0;
    actwk[11].actno = 47;
    actwk[11].xspeed.w = 500;
    scarab_work_get(&actwk[11])->carried_actor_index = 12;
    player_work_get(&actwk[12])->status_flags = 1;
    c_move(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[5] & 1);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[11].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(&actwk[12])->status_flags & 1);

    reset_scarab_state();
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    scarab_work_get(ctrl)->child_item_index = 11;
    actwk[11].actno = 0;
    c_move(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 2, flagwork[5] & 2);

    reset_scarab_state();
    flagwork[5] = 3;
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    c_move(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_scarab_state();
    set_controller_flag_ptr(ctrl, &flagwork[5]);
    c_move(ctrl);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
}

static void test_enemy_wrapper_and_initialization(test_context *ctx) {
    sprite_status *enemywk = &actwk[4];
    sprite_status *ctrl = &actwk[3];

    reset_scarab_state();
    scarab_work_get(enemywk)->parent_index = 3;
    scarab(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_scarab_state();
    scarab_work_get(enemywk)->parent_index = 3;
    enemywk->userflag.b.l = 1;
    enemy(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    enemywk->userflag.b.l = 1;
    scarab(enemywk);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    enemywk->userflag.b.l = 1;
    enemywk->r_no0 = 2;
    scarab(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    enemy_suicide_result = -1;
    enemy(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    enemywk->xposi.w.h = 100;
    enemywk->yposi.w.h = 50;
    enemywk->userflag.b.l = 0;
    queue_emycol(5);
    queue_emycol(0);
    queue_actor2(&actwk[10]);
    enemy(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 47, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    e_init(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    enemywk->xposi.w.h = 100;
    enemywk->yposi.w.h = 50;
    enemywk->userflag.b.h = 1;
    enemywk->userflag.b.l = 1;
    queue_emycol(0);
    queue_emycol(0);
    queue_actor2(&actwk[10]);
    e_init(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].cddat & 1);
}

static void test_enemy_move_turn_followers_and_catch(test_context *ctx) {
    sprite_status *enemywk = &actwk[4];
    sprite_status *ctrl = &actwk[3];

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->origin_x = 100;
    scarab_work_get(enemywk)->animation_timer = 10;
    scarab_work_get(enemywk)->shell_actor_index = 10;
    scarab_work_get(ctrl)->child_item_index = 11;
    scarab_work_get(enemywk)->carried_actor_index = 12;
    enemywk->xposi.w.h = 100;
    enemywk->yposi.w.h = 80;
    actwk[11].xposi.w.h = 200;
    actwk[11].yposi.w.h = 90;
    actwk[12].xposi.w.h = 220;
    actwk[12].yposi.w.h = 95;
    scarab_work_get(enemywk)->x_velocity = 65536;
    emycol_d_result = 0;
    e_move(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 256, actwk[11].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 101, actwk[10].xposi.w.h);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->origin_x = 100;
    scarab_work_get(enemywk)->animation_timer = 10;
    scarab_work_get(enemywk)->shell_actor_index = 10;
    enemywk->xposi.w.h = 100;
    scarab_work_get(enemywk)->x_velocity = 24576;
    emycol_d_result = 7;
    e_move(enemywk);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->origin_x = 200;
    scarab_work_get(enemywk)->animation_timer = 10;
    scarab_work_get(enemywk)->shell_actor_index = 10;
    enemywk->xposi.w.h = 100;
    e_move(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[10].xposi.w.h);

    reset_scarab_state();
    ctrl->actno = 47;
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->origin_x = 0;
    scarab_work_get(enemywk)->animation_timer = 10;
    scarab_work_get(enemywk)->shell_actor_index = 10;
    enemywk->xposi.w.h = 100;
    scarab_work_get(enemywk)->x_velocity = 65536;
    e_move(enemywk);

    reset_scarab_state();
    scarab_work_get(enemywk)->animation_timer = -1;
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->shell_actor_index = 10;
    enemywk->xposi.w.h = 123;
    e_move(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[10].xposi.w.h);

    reset_scarab_state();
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->animation_timer = 1;
    enemywk->colicnt = 1;
    e1_check(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(&actwk[0])->status_flags & 1);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);

    reset_scarab_state();
    scarab_work_get(enemywk)->parent_index = 3;
    scarab_work_get(enemywk)->animation_timer = 1;
    enemywk->colicnt = 1;
    actwk[0].r_no0 = 4;
    e1_check(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_scarab_state();
    scarab_work_get(enemywk)->animation_timer = -30;
    e1_chk_patchg(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_scarab_state();
    enemywk->userflag.b.l = 1;
    enemywk->xposi.w.h = 100;
    enemywk->yposi.w.h = 80;
    e1_catch(enemywk);
    TEST_ASSERT_EQ_INT(ctx, 124, actwk[0].xposi.w.h);
}

static void test_enemy_keep_and_wait_release_player(test_context *ctx) {
    sprite_status *enemywk = &actwk[4];
    sprite_status *player = &actwk[0];

    reset_scarab_state();
    scarab_work_get(enemywk)->carried_actor_index = 0;
    scarab_work_get(enemywk)->hold_timer = 2;
    e1_keep(enemywk);

    reset_scarab_state();
    scarab_work_get(enemywk)->carried_actor_index = 0;
    scarab_work_get(enemywk)->hold_timer = 1;
    player_work_get(player)->status_flags = 1;
    enemywk->userflag.b.h = 0;
    enemywk->userflag.b.l = 0;
    e1_keep(enemywk);

    reset_scarab_state();
    scarab_work_get(enemywk)->carried_actor_index = 0;
    scarab_work_get(enemywk)->hold_timer = 1;
    enemywk->userflag.b.h = 1;
    enemywk->userflag.b.l = 1;
    player->cddat = 1;
    e1_keep(enemywk);

    reset_scarab_state();
    scarab_work_get(enemywk)->hold_timer = 2;
    e1_wait(enemywk);

    scarab_work_get(enemywk)->hold_timer = 1;
    e1_wait(enemywk);
}

static void test_item_states_and_pickup(test_context *ctx) {
    sprite_status *item = &actwk[6];
    sprite_status *ctrl = &actwk[3];

    reset_scarab_state();
    item->userflag.b.h = 2;
    scarab_work_get(item)->parent_index = 3;
    scarab(item);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_scarab_state();
    ctrl->actno = 47;
    item->userflag.b.h = 2;
    item->userflag.b.l = 5;
    scarab_work_get(item)->parent_index = 3;
    emycol_d_result = 3;
    scarab(item);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_scarab_state();
    ctrl->actno = 47;
    item->userflag.b.h = 2;
    item->r_no0 = 2;
    item->actflg = 0;
    scarab_work_get(item)->parent_index = 3;
    scarab(item);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_scarab_state();
    item->actflg = 128;
    item->r_no1 = 1;
    item->yspeed.w = 256;
    scarab_work_get(item)->parent_index = 3;
    emycol_d_result = -5;
    itemmove(item);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_scarab_state();
    item->actflg = 128;
    scarab_work_get(item)->parent_index = 3;
    itemmove(item);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 25, ride_on_chk_actno_seen[0]);

    reset_scarab_state();
    time_stop = 1;
    scarab_work_get(item)->parent_index = 3;
    itemmove2(item);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_scarab_state();
    scarab_work_get(item)->parent_index = 3;
    item->actno = 0;
    itemmove3(item);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_scarab_state();
    scarab_work_get(item)->parent_index = 3;
    scarab_work_get(ctrl)->child_enemy_index = 4;
    item->xposi.w.h = 300;
    item->yposi.w.h = 120;
    item->mstno.b.h = 7;
    queue_actor(&actwk[10]);
    queue_actor(&actwk[11]);
    itemget(item);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[4].colicnt);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 150, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 26, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[10].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_scarab_state();
    scarab_work_get(item)->parent_index = 3;
    itemget(item);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

TEST_MAIN_BEGIN;
test_scarab_tables_and_patterns(&ctx);
test_controller_init_wait_and_make_paths(&ctx);
test_controller_move_tracks_child_deletion(&ctx);
test_enemy_wrapper_and_initialization(&ctx);
test_enemy_move_turn_followers_and_catch(&ctx);
test_enemy_keep_and_wait_release_player(&ctx);
test_item_states_and_pickup(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
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
void frameout(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r6/hachi6.c"

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

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
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

static void reset_hachi6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
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

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void place_player_near(sprite_status *actor, Sint16 x_delta,
                              Sint16 y_delta) {
    actwk[0].xposi.w.h = actor->xposi.w.h + x_delta;
    actwk[0].yposi.w.h = actor->yposi.w.h + y_delta;
}

static void assert_hachi_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_hachi6_main_respects_suicide_and_initializes(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_hachi6_state();
    enemy_suicide_result = -1;
    hachi6(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_hachi6_state();
    actor->actno = 70;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    hachi6(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 42071, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->colino);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_hachi6_e);
    TEST_ASSERT_EQ_INT(ctx, 300, actor->xposi.w.h);
    assert_hachi_callbacks(ctx, actor);

    reset_hachi6_state();
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    hachi6(actor);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_hachi6_b);
    TEST_ASSERT_EQ_INT(ctx, 300, actor->xposi.w.h);
}

static void test_hachi6_move1_checks_player_and_timer(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_hachi6_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->r_no0 = 4;
    hachi6_get_work(actor)->x_speed = -65536;
    hachi6_get_work(actor)->timer = 5;
    hachi6_get_work(actor)->shot_cooldown = 1;
    place_player_near(actor, 0, 0);

    act_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 299, actor->xposi.w.h);

    act_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 298, actor->xposi.w.h);

    reset_hachi6_state();
    actor->r_no0 = 4;
    actor->userflag.b.h = 1;
    hachi6_get_work(actor)->x_speed = -32768;
    hachi6_get_work(actor)->timer = 1;
    act_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
}

static void test_hachi6_act_check_flips_when_player_is_behind(
    test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_hachi6_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->actflg = 1;
    actor->cddat = 1;
    actor->r_no0 = 4;
    hachi6_get_work(actor)->x_speed = -65536;
    hachi6_get_work(actor)->timer = 5;
    place_player_near(actor, -20, 0);

    act_move1(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 299, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);

    reset_hachi6_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->r_no0 = 4;
    hachi6_get_work(actor)->x_speed = -65536;
    hachi6_get_work(actor)->timer = 5;
    place_player_near(actor, 0, 97);
    act_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);

    place_player_near(actor, 121, 0);
    act_move1(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
}

static void test_hachi6_reverse_sequence(test_context *ctx) {
    sprite_status *actor = &actwk[6];

    reset_hachi6_state();
    actor->r_no0 = 6;
    hachi6_get_work(actor)->x_speed = -65536;
    act_rev(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    act_rev1(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);

    hachi6_get_work(actor)->timer = 0;
    act_rev1(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);

    act_rev2(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    hachi6_get_work(actor)->timer = 0;
    act_rev2(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
}

static void test_hachi6_shot_sequence_spawns_projectile(test_context *ctx) {
    sprite_status *actor = &actwk[6];
    sprite_status *shot = &actwk[20];

    reset_hachi6_state();
    actor->actno = 70;
    actor->actflg = 129;
    actor->sproffset = 42071;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->r_no0 = 12;
    hachi6_get_work(actor)->shot_x_offset = 8;

    act_shot(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);

    hachi6_get_work(actor)->timer = 0;
    act_shot1(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 50, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 308, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 84, actor->yposi.w.h);

    queue_actor(shot);
    hachi6_get_work(actor)->timer = 0;
    act_shot2(actor);

    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 70, shot->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, shot->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 129, shot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 42071, shot->sproffset);
    TEST_ASSERT_TRUE(ctx, shot->patbase == pat_tama);
    TEST_ASSERT_EQ_INT(ctx, 1, shot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, shot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 179, shot->colino);
    TEST_ASSERT_EQ_INT(ctx, 315, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 107, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);

    hachi6_get_work(actor)->timer = 2;
    act_shot3(actor);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);

    hachi6_get_work(actor)->timer = 0;
    act_shot3(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 80, actor->yposi.w.h);
}

static void test_hachi6_shot2_handles_wait_failure_and_left_facing(
    test_context *ctx) {
    sprite_status *actor = &actwk[6];
    sprite_status *shot = &actwk[20];

    reset_hachi6_state();
    actor->actno = 70;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->r_no0 = 16;
    hachi6_get_work(actor)->timer = 2;
    act_shot2(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    hachi6_get_work(actor)->timer = 0;
    act_shot2(actor);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_hachi6_state();
    actor->actno = 70;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 80;
    actor->r_no0 = 16;
    queue_actor(shot);
    hachi6_get_work(actor)->timer = 0;
    act_shot2(actor);
    TEST_ASSERT_EQ_INT(ctx, 293, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_hachi6_tama_dispatch_and_lifecycle(test_context *ctx) {
    sprite_status *shot = &actwk[20];

    reset_hachi6_state();
    shot->userflag.b.h = -1;
    tama(shot);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    tam_move1(shot);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->r_no0);
    tam_move1(shot);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, shot->patno);

    hachi6_get_work(shot)->x_speed = -0x20000;
    hachi6_get_work(shot)->y_speed = 0x20000;
    tama(shot);
    TEST_ASSERT_EQ_INT(ctx, -2, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, shot->r_no0);

    hachi6_get_work(shot)->timer = 0;
    tam_move2(shot);
    TEST_ASSERT_EQ_INT(ctx, 6, shot->r_no0);

    shot->actflg = 0;
    tam_move3(shot);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == shot);

    reset_hachi6_state();
    shot = &actwk[20];
    shot->r_no0 = 6;
    shot->userflag.b.h = -1;
    shot->actflg = 128;
    hachi6_get_work(shot)->x_speed = 0x20000;
    hachi6_get_work(shot)->y_speed = 0x20000;
    tama(shot);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, shot->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == shot);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg_tama);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_hachi6_main_dispatches_tama_actor(test_context *ctx) {
    sprite_status *shot = &actwk[20];

    reset_hachi6_state();
    shot->userflag.b.h = -1;
    hachi6(shot);

    TEST_ASSERT_EQ_INT(ctx, 2, shot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_hachi6_main_respects_suicide_and_initializes(&ctx);
    test_hachi6_move1_checks_player_and_timer(&ctx);
    test_hachi6_act_check_flips_when_player_is_behind(&ctx);
    test_hachi6_reverse_sequence(&ctx);
    test_hachi6_shot_sequence_spawns_projectile(&ctx);
    test_hachi6_shot2_handles_wait_failure_and_left_facing(&ctx);
    test_hachi6_tama_dispatch_and_lifecycle(&ctx);
    test_hachi6_main_dispatches_tama_actor(&ctx);
TEST_MAIN_END

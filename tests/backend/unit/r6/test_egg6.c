#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 generate_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int actwkchk_count;
static sprite_status *actwkchk_queue[24];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[24];
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_pattern;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r6/egg6.c"

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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) /
                               sizeof(soundset_requests[0]))) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_pattern = pat_dat;
}

static void reset_egg6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    generate_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_pattern = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_bomb_floor(sprite_status *actor, Sint16 value) {
    actor->userflag.w = (Uint16)value;
    set_actfree_word(actor, 4, value);
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void assert_body_callbacks(test_context *ctx, sprite_status *actor,
                                  int expect_frameout_s) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expect_frameout_s, frameout_s_count);
    if (expect_frameout_s != 0)
        TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_eggz6_init_obeys_generate_flag(test_context *ctx) {
    sprite_status *egg = &actwk[8];

    reset_egg6_state();
    generate_flag = 1;

    eggz6(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == egg);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, egg->r_no0);

    reset_egg6_state();
    egg->xposi.w.h = 320;
    egg->yposi.w.h = 160;

    eggz6(egg);

    TEST_ASSERT_EQ_INT(ctx, 2, egg->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, egg->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, egg->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 20, egg->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, egg->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 28, egg->sprvsize);
    TEST_ASSERT_TRUE(ctx, egg->patbase == pat_egg6);
    TEST_ASSERT_EQ_INT(ctx, 248, egg->colino);
    assert_body_callbacks(ctx, egg, 1);
}

static void test_egg_check_collision_and_ride_clear_paths(test_context *ctx) {
    sprite_status *egg = &actwk[8];

    reset_egg6_state();
    egg->r_no0 = 2;

    act_check(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == egg);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, egg->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);

    reset_egg6_state();
    egg->r_no0 = 2;
    egg->colino = 248;
    egg->colicnt = 3;
    hitchk_result = 1;

    act_check(egg);

    TEST_ASSERT_EQ_INT(ctx, 0, egg->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, egg->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 4, egg->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == egg);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
}

static void test_egg_make0_waits_then_spawns_scripted_debris(
    test_context *ctx) {
    sprite_status *egg = &actwk[8];
    sprite_status *debris = &actwk[20];

    reset_egg6_state();
    egg->xposi.w.h = 300;
    egg->yposi.w.h = 120;
    act_init(egg);
    queue_actor(debris);

    act_make0(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 24, debris->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, debris->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 300, debris->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, debris->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    act_make0(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, egg->actfree[21]);
}

static void test_egg_make0_handles_exhaustion_and_end_marker(
    test_context *ctx) {
    sprite_status *egg = &actwk[8];

    reset_egg6_state();
    act_init(egg);

    act_make0(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_egg6_state();
    act_init(egg);
    for (int i = 20; i < 29; ++i)
        queue_actor(&actwk[i]);

    while (egg->r_no0 == 2)
        act_make0(egg);

    TEST_ASSERT_EQ_INT(ctx, 4, egg->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, egg->patno);
    TEST_ASSERT_EQ_INT(ctx, 60, egg->actfree[21]);
}

static void test_egg_wait_and_make1_spawn_bombs_or_frames_out_on_failure(
    test_context *ctx) {
    sprite_status *egg = &actwk[8];
    int i;

    reset_egg6_state();
    egg->r_no0 = 6;
    egg->actfree[21] = 2;

    act_wait(egg);

    TEST_ASSERT_EQ_INT(ctx, 6, egg->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, egg->actfree[21]);

    act_wait(egg);

    TEST_ASSERT_EQ_INT(ctx, 8, egg->r_no0);

    reset_egg6_state();
    egg->actno = 24;
    egg->xposi.w.h = 400;
    egg->yposi.w.h = 200;
    for (i = 20; i < 30; ++i)
        queue_actor(&actwk[i]);

    act_make1(egg);

    TEST_ASSERT_EQ_INT(ctx, 10, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == egg);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[20].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 376, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -24, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 520, actwk[29].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -24, actwk[29].yposi.w.h);

    reset_egg6_state();
    egg->xposi.w.h = 400;
    egg->yposi.w.h = 200;

    act_make1(egg);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == egg);
}

static void test_bomb_init_fall_and_die_paths(test_context *ctx) {
    sprite_status *bombwk = &actwk[12];

    reset_egg6_state();

    bomb_init(bombwk);

    TEST_ASSERT_EQ_INT(ctx, 2, bombwk->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, bombwk->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, bombwk->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 6, bombwk->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 6, bombwk->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 6, bombwk->sprhsize);
    TEST_ASSERT_TRUE(ctx, bombwk->patbase == pat_bomb);
    TEST_ASSERT_EQ_INT(ctx, 183, bombwk->colino);

    reset_egg6_state();
    bombwk->r_no0 = 2;
    bombwk->yposi.l = 100 << 16;
    set_bomb_floor(bombwk, 104);
    set_actfree_long(bombwk, 0, 2048);

    bomb(bombwk);

    TEST_ASSERT_EQ_INT(ctx, 2, bombwk->r_no0);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 2048, bombwk->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == bombwk);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == pchg_bomb);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    bombwk->yposi.w.h = 104;

    bomb(bombwk);

    TEST_ASSERT_EQ_INT(ctx, 4, bombwk->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, patchg_count);

    bomb(bombwk);

    TEST_ASSERT_EQ_INT(ctx, 24, bombwk->actno);
    TEST_ASSERT_EQ_INT(ctx, 0, bombwk->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, bombwk->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
}

static void test_eggz6_dispatches_bomb_without_body_callbacks(
    test_context *ctx) {
    sprite_status *bombwk = &actwk[12];

    reset_egg6_state();
    bombwk->userflag.b.h = -1;

    eggz6(bombwk);

    TEST_ASSERT_EQ_INT(ctx, 2, bombwk->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bombwk);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

TEST_MAIN_BEGIN;
test_eggz6_init_obeys_generate_flag(&ctx);
test_egg_check_collision_and_ride_clear_paths(&ctx);
test_egg_make0_waits_then_spawns_scripted_debris(&ctx);
test_egg_make0_handles_exhaustion_and_end_marker(&ctx);
test_egg_wait_and_make1_spawn_bombs_or_frames_out_on_failure(&ctx);
test_bomb_init_fall_and_die_paths(&ctx);
test_eggz6_dispatches_bomb_without_body_callbacks(&ctx);
TEST_MAIN_END

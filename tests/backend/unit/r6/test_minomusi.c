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
static int actwkchk2_count;
static sprite_status *actwkchk2_parent;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_pattern;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r6/minomusi.c"

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

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_parent = pActwk;
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
    patchg_pattern = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_minomusi_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    actwkchk2_count = 0;
    actwkchk2_parent = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_pattern = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void queue_actor(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actor_long(sprite_status *actor, int index, Sint32 value) {
    int offset = index * 4 - 46;
    memcpy(&actor->actfree[offset], &value, sizeof(value));
}

static void place_player_near(sprite_status *actor, Sint16 x_delta,
                              Sint16 y_delta) {
    actwk[0].xposi.w.h = actor->xposi.w.h + x_delta;
    actwk[0].yposi.w.h = actor->yposi.w.h + y_delta;
}

static void assert_body_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_minomusi_dispatch_body_and_suicide(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];
    sprite_status *string_actor = &actwk[20];

    reset_minomusi_state();
    enemy_suicide_result = -1;
    minomusi(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_minomusi_state();
    body_actor->actno = 51;
    body_actor->xposi.w.h = 320;
    body_actor->yposi.w.h = 80;
    queue_actor(string_actor);
    minomusi(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, body_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, body_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 9352, body_actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 52, body_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 88, body_actor->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, body_actor->patbase == pat_minomusi_e);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_parent == body_actor);

    TEST_ASSERT_EQ_INT(ctx, 51, string_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, string_actor->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 320, string_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 88, string_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, string_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, string_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9352, string_actor->sproffset);
    TEST_ASSERT_TRUE(ctx, string_actor->patbase == pat_minomusi_e);
    TEST_ASSERT_EQ_INT(ctx, 32, string_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, string_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 1, string_actor->sprhsize);
    assert_body_callbacks(ctx, body_actor);
}

static void test_minomusi_body_init_variant_and_allocation_failure(
    test_context *ctx) {
    sprite_status *body_actor = &actwk[8];

    reset_minomusi_state();
    body_actor->userflag.b.h = 1;
    body_init(body_actor);
    TEST_ASSERT_TRUE(ctx, body_actor->patbase == pat_minomusi_b);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body_actor);
}

static void test_minomusi_wait_checks_player_window(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];

    reset_minomusi_state();
    body_actor->xposi.w.h = 200;
    body_actor->yposi.w.h = 100;
    body_wait(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 9, body_actor->patno);

    set_actor_word(body_actor, 23, 1);
    place_player_near(body_actor, 0, 39);
    body_wait1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, body_actor->r_no0);

    body_actor->r_no0 = 4;
    set_actor_word(body_actor, 23, 1);
    place_player_near(body_actor, -168, 40);
    body_wait1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 6, body_actor->r_no0);

    body_actor->r_no0 = 4;
    set_actor_word(body_actor, 23, 1);
    place_player_near(body_actor, 168, 40);
    body_wait1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);
}

static void test_minomusi_down_and_up_motion(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];

    reset_minomusi_state();
    body_actor->yposi.w.h = 100;
    set_actor_word(body_actor, 28, 100);
    set_actor_word(body_actor, 29, 116);
    body_actor->r_no0 = 6;

    body_down(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 8, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 108, body_actor->yposi.w.h);
    body_down1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 14, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 116, body_actor->yposi.w.h);

    body_actor->r_no0 = 10;
    body_actor->userflag.b.h = 0;
    body_actor->yposi.w.h = 116;
    body_up(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 12, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 109, body_actor->yposi.w.h);
    body_up1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 12, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 102, body_actor->yposi.w.h);
    body_up1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, body_actor->yposi.w.h);

    reset_minomusi_state();
    body_actor->r_no0 = 10;
    body_actor->userflag.b.h = 1;
    body_actor->yposi.w.h = 116;
    set_actor_word(body_actor, 28, 100);
    body_up(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 114, body_actor->yposi.w.h);
}

static void test_minomusi_stay_spawns_spike_and_sounds(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];
    sprite_status *spike_actor = &actwk[21];

    reset_minomusi_state();
    body_actor->actno = 51;
    body_actor->actflg = 128;
    body_actor->sprpri = 3;
    body_actor->sproffset = 9352;
    body_actor->patbase = pat_minomusi_e;
    body_actor->sprvsize = 16;
    body_actor->sprhs = 16;
    body_actor->sprhsize = 16;
    body_actor->xposi.w.h = 320;
    body_actor->yposi.w.h = 180;
    body_actor->r_no0 = 14;

    body_stay(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 255, body_actor->mstno.w);

    body_actor->patcnt = 14;
    body_stay1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == body_actor);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);

    body_actor->patcnt = 15;
    queue_actor(spike_actor);
    body_stay1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 51, spike_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, spike_actor->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 320, spike_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 184, spike_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, spike_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, spike_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9352, spike_actor->sproffset);
    TEST_ASSERT_TRUE(ctx, spike_actor->patbase == pat_minomusi_e);
    TEST_ASSERT_EQ_INT(ctx, 181, spike_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 183, soundset_requests[0]);
}

static void test_minomusi_stay_variant_and_countdown_paths(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];

    reset_minomusi_state();
    body_actor->userflag.b.h = 1;
    body_actor->r_no0 = 14;
    body_stay(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 511, body_actor->mstno.w);

    body_stay1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);

    reset_minomusi_state();
    body_actor->r_no0 = 16;
    set_actor_word(body_actor, 23, 1);
    body_stay1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 10, body_actor->r_no0);

    reset_minomusi_state();
    body_actor->patcnt = 15;
    body_stay1(body_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_minomusi_hari_dispatches_by_parent_pattern(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];
    sprite_status *spike_actor = &actwk[21];

    reset_minomusi_state();
    body_actor->patcnt = 0;
    spike_actor->userflag.b.l = 1;
    set_actor_word(spike_actor, 30, 8);
    minomusi(spike_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == spike_actor);

    body_actor->patcnt = 1;
    hari(spike_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == spike_actor);
}

static void test_minomusi_ito_tracks_parent_and_frames_out(test_context *ctx) {
    sprite_status *body_actor = &actwk[8];
    sprite_status *string_actor = &actwk[20];

    reset_minomusi_state();
    body_actor->actno = 51;
    body_actor->yposi.w.h = 140;
    set_actor_word(body_actor, 28, 100);
    string_actor->userflag.b.l = -1;
    set_actor_word(string_actor, 30, 8);

    minomusi(string_actor);
    TEST_ASSERT_EQ_INT(ctx, 2, string_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 124, string_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == string_actor);

    body_actor->yposi.w.h = 110;
    ito(string_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, string_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 116, string_actor->yposi.w.h);

    body_actor->actno = 0;
    ito(string_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == string_actor);
}

TEST_MAIN_BEGIN;
    test_minomusi_dispatch_body_and_suicide(&ctx);
    test_minomusi_body_init_variant_and_allocation_failure(&ctx);
    test_minomusi_wait_checks_player_window(&ctx);
    test_minomusi_down_and_up_motion(&ctx);
    test_minomusi_stay_spawns_spike_and_sounds(&ctx);
    test_minomusi_stay_variant_and_countdown_paths(&ctx);
    test_minomusi_hari_dispatches_by_parent_pattern(&ctx);
    test_minomusi_ito_tracks_parent_and_frames_out(&ctx);
TEST_MAIN_END

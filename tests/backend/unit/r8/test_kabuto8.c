#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_results[8];
static int emycol_d_result_count;
static int soundset_count;
static Sint16 soundset_requests[8];
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint16 emycol_d(sprite_status *pActwk);
void soundset(Sint16 ReqNo);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r8/kabuto8.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    ++enemy_suicide_count;
    (void)pActwk;
    return enemy_suicide_result;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_actor = pActwk;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    if (emycol_d_count <= emycol_d_result_count) {
        return emycol_d_results[emycol_d_count - 1];
    }
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void reset_logs(void) {
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    memset(emycol_d_results, 0, sizeof(emycol_d_results));
    emycol_d_result_count = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_kabuto8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_kabuto8_word(sprite_status *actor, int offset, Sint16 value) {
    kabuto8_work *work = kabuto8_get_work(actor);

    switch (offset) {
    case 0:
        work->timer = value;
        break;
    case 2:
        work->offset_x = value;
        break;
    case 4:
        work->offset_y = value;
        break;
    case 6:
        work->base_x = value;
        break;
    case 8:
        work->sound_timer = value;
        break;
    case 20:
        work->parent_index = value;
        break;
    }
}

static void set_kabuto8_long(sprite_status *actor, int offset, Sint32 value) {
    if (offset == 2)
        kabuto8_get_work(actor)->x_speed = value;
}

static void assert_body_outer_callbacks(test_context *ctx,
                                        sprite_status *actor,
                                        Sint16 expected_xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expected_xpos, frameout_s00_xpos);
}

static void assert_tusk_actionsub(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_kabuto8_patterns_capture_default_literal_data(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kabuto[0] == &cub00);
    TEST_ASSERT_EQ_INT(ctx, 1, cub00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -24, cub00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, cub00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KABUTO8_BASE, cub00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 2, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[2]);
    TEST_ASSERT_EQ_INT(ctx, 255, p00[3]);
    TEST_ASSERT_EQ_INT(ctx, 2, p01[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, p01[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, p01[2]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg[1] == p01);
    TEST_ASSERT_TRUE(ctx, pat[0] == &hea00);
    TEST_ASSERT_TRUE(ctx, pat[3] == &hea03);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KABUTO8_BASE + 2, hea00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KABUTO8_BASE + 1, hea00.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_KABUTO8_BASE + 3, hea02.spra[0].index);
}

static void test_kabuto_body_suicide_return_skips_work(test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    enemy_suicide_result = -1;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
}

static void test_kabuto_body_initializes_and_spawns_tusk(test_context *ctx) {
    sprite_status *body = &actwk[3];
    sprite_status *tusk = &actwk[20];

    reset_kabuto8_state();
    body->actno = 36;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    queue_actwkchk2(tusk);
    emycol_d_results[0] = 0;
    emycol_d_result_count = 1;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == body);
    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_body_initializes_flipped_when_userflag_low_nonzero(
    test_context *ctx) {
    sprite_status *body = &actwk[3];
    sprite_status *tusk = &actwk[20];

    reset_kabuto8_state();
    body->actno = 36;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    body->userflag.b.h = 7;
    body->userflag.b.l = 1;
    queue_actwkchk2(tusk);

    kabuto(body);

    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_body_allocation_failure_frames_out(test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->xposi.w.h = 100;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_body_fall_continues_then_lands(test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->r_no0 = 2;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    set_kabuto8_word(body, 6, 100);
    emycol_d_results[0] = 3;
    emycol_d_result_count = 1;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    assert_body_outer_callbacks(ctx, body, 100);

    reset_logs();
    body->r_no0 = 2;
    body->yposi.w.h = 201;
    set_kabuto8_word(body, 6, 100);
    emycol_d_results[0] = -4;
    emycol_d_result_count = 1;

    kabuto(body);

    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_body_move_updates_position_without_sound(
    test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->r_no0 = 4;
    body->actflg = 0;
    body->xposi.l = 100 << 16;
    body->yposi.w.h = 200;
    set_kabuto8_word(body, 0, 2);
    set_kabuto8_long(body, 2, -20480);
    set_kabuto8_word(body, 6, 100);
    emycol_d_results[0] = 5;
    emycol_d_result_count = 1;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_body_move_triggers_sound_and_reverses_timer(
    test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->r_no0 = 4;
    body->actflg = 128;
    body->xposi.l = 100 << 16;
    body->yposi.w.h = 200;
    set_kabuto8_word(body, 0, 0);
    set_kabuto8_long(body, 2, -20480);
    set_kabuto8_word(body, 6, 100);
    set_kabuto8_word(body, 8, 31);
    emycol_d_results[0] = -2;
    emycol_d_result_count = 1;

    kabuto(body);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 177, soundset_requests[0]);
    assert_body_outer_callbacks(ctx, body, 100);
}

static void test_kabuto_tusk_initializes(test_context *ctx) {
    sprite_status *tusk = &actwk[20];

    reset_kabuto8_state();
    tusk->userflag.b.l = -1;

    kabuto(tusk);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_tusk_actionsub(ctx, tusk);
}

static void test_kabuto_tusk_frames_out_when_parent_missing(test_context *ctx) {
    sprite_status *tusk = &actwk[20];

    reset_kabuto8_state();
    tusk->r_no0 = 2;
    tusk->userflag.b.l = -1;
    set_kabuto8_word(tusk, 20, 3);
    actwk[3].actno = 0;

    kabuto(tusk);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == tusk);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_tusk_actionsub(ctx, tusk);
}

static void test_kabuto_tusk_uses_existing_offsets_while_timer_positive(
    test_context *ctx) {
    sprite_status *tusk = &actwk[20];
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->actno = 36;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    tusk->r_no0 = 2;
    tusk->userflag.b.l = -1;
    set_kabuto8_word(tusk, 20, 3);
    set_kabuto8_word(tusk, 0, 2);
    set_kabuto8_word(tusk, 2, 5);
    set_kabuto8_word(tusk, 4, 6);

    kabuto(tusk);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == tusk);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    assert_tusk_actionsub(ctx, tusk);
}

static void test_kabuto_tusk_timer_expiry_toggles_high_motion(
    test_context *ctx) {
    sprite_status *tusk = &actwk[20];
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->actno = 36;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    tusk->r_no0 = 2;
    tusk->userflag.b.l = -1;
    set_kabuto8_word(tusk, 20, 3);
    set_kabuto8_word(tusk, 0, 0);

    kabuto(tusk);

    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    assert_tusk_actionsub(ctx, tusk);
}

static void test_kabuto_tusk_userflag_high_forces_offsets_and_flip(
    test_context *ctx) {
    sprite_status *tusk = &actwk[20];
    sprite_status *body = &actwk[3];

    reset_kabuto8_state();
    body->actno = 36;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    tusk->r_no0 = 2;
    tusk->actflg = 1;
    tusk->mstno.b.h = 0;
    tusk->userflag.b.h = 1;
    tusk->userflag.b.l = -1;
    set_kabuto8_word(tusk, 20, 3);
    set_kabuto8_word(tusk, 0, 40);

    kabuto(tusk);

    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    assert_tusk_actionsub(ctx, tusk);
}

TEST_MAIN_BEGIN;
    test_kabuto8_patterns_capture_default_literal_data(&ctx);
    test_kabuto_body_suicide_return_skips_work(&ctx);
    test_kabuto_body_initializes_and_spawns_tusk(&ctx);
    test_kabuto_body_initializes_flipped_when_userflag_low_nonzero(&ctx);
    test_kabuto_body_allocation_failure_frames_out(&ctx);
    test_kabuto_body_fall_continues_then_lands(&ctx);
    test_kabuto_body_move_updates_position_without_sound(&ctx);
    test_kabuto_body_move_triggers_sound_and_reverses_timer(&ctx);
    test_kabuto_tusk_initializes(&ctx);
    test_kabuto_tusk_frames_out_when_parent_missing(&ctx);
    test_kabuto_tusk_uses_existing_offsets_while_timer_positive(&ctx);
    test_kabuto_tusk_timer_expiry_toggles_high_motion(&ctx);
    test_kabuto_tusk_userflag_high_forces_offsets_and_flip(&ctx);
TEST_MAIN_END

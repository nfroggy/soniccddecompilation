#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
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
static int soundset_count;
static Sint16 soundset_requests[8];

void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void soundset(Sint16 ReqNo);

#include "src/r8/nokogiri.c"

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_logs(void) {
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
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
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_nokogiri_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void assert_outer_callbacks(test_context *ctx, sprite_status *actor,
                                   Sint16 expected_xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expected_xpos, frameout_s00_xpos);
}

static void test_nokogiri_patterns_capture_default_literal_data(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg[8] == p08);
    TEST_ASSERT_TRUE(ctx, pchg[9] == p08);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[3]);
    TEST_ASSERT_EQ_INT(ctx, 16, p08[1]);
    TEST_ASSERT_EQ_INT(ctx, 17, p08[2]);
    TEST_ASSERT_TRUE(ctx, pat_nokogiri[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_nokogiri[17] == &pat17);
    TEST_ASSERT_TRUE(ctx, pat_nokogiri[18] == 0);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -34, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_NOKOGIRI_BASE, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -112, pat17.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -114, pat17.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_NOKOGIRI_BASE + 17, pat17.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pat_bar[0] == &pat_bar0);
    TEST_ASSERT_EQ_INT(ctx, -41, pat_bar0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -43, pat_bar0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_NOKOGIRI_BASE + 18, pat_bar0.spra[0].index);
}

static void test_nokogiri_initializes_body_spawns_bar_and_falls_through_open(
    test_context *ctx) {
    sprite_status *body = &actwk[3];
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    body->actno = 39;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    queue_actwkchk2(bar_actor);

    nokogiri(body);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == body);
    assert_outer_callbacks(ctx, body, 100);
}

static void test_nokogiri_initializes_flipped_diagonal_bar_offset(
    test_context *ctx) {
    sprite_status *body = &actwk[3];
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    body->actno = 39;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    body->userflag.b.h = 3;
    queue_actwkchk2(bar_actor);

    nokogiri(body);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == body);
    assert_outer_callbacks(ctx, body, 100);
}

static void test_nokogiri_initial_negative_userflag_uses_large_bar_offset(
    test_context *ctx) {
    sprite_status *body = &actwk[3];
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    body->actno = 39;
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;
    body->userflag.b.h = -125;
    queue_actwkchk2(bar_actor);

    nokogiri(body);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == body);
    assert_outer_callbacks(ctx, body, 100);
}

static void test_nokogiri_allocation_failure_frames_out_but_open_still_runs(
    test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_nokogiri_state();
    body->xposi.w.h = 100;
    body->yposi.w.h = 200;

    nokogiri(body);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body);
    assert_outer_callbacks(ctx, body, 100);
}

static void test_nokogiri_open_timer_paths(test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_nokogiri_state();
    body->r_no0 = 2;
    body->xposi.l = 100 << 16;
    body->yposi.l = 200 << 16;
    set_actfree_long(body, 2, 65536);
    set_actfree_long(body, 6, -65536);
    set_actfree_word(body, 0, 2);
    set_actfree_word(body, 10, 5);
    set_actfree_word(body, 12, 77);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 77);

    reset_logs();
    body->r_no0 = 2;
    body->xposi.l = 100 << 16;
    body->yposi.l = 200 << 16;
    body->mstno.b.h = 4;
    set_actfree_word(body, 0, 1);
    set_actfree_word(body, 10, 2);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 77);

    reset_logs();
    body->r_no0 = 2;
    body->xposi.l = 100 << 16;
    body->yposi.l = 200 << 16;
    body->mstno.b.h = 5;
    set_actfree_word(body, 0, 1);
    set_actfree_word(body, 10, 1);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 77);
}

static void test_nokogiri_opend_close_and_closed_timer_paths(
    test_context *ctx) {
    sprite_status *body = &actwk[3];

    reset_nokogiri_state();
    body->r_no0 = 4;
    set_actfree_word(body, 0, 2);
    set_actfree_word(body, 12, 66);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);

    reset_logs();
    body->r_no0 = 4;
    set_actfree_word(body, 0, 1);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);

    reset_logs();
    body->r_no0 = 6;
    body->xposi.l = 100 << 16;
    body->yposi.l = 200 << 16;
    body->mstno.b.h = 7;
    set_actfree_long(body, 2, 65536);
    set_actfree_long(body, 6, -65536);
    set_actfree_word(body, 0, 1);
    set_actfree_word(body, 10, 2);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);

    reset_logs();
    body->r_no0 = 6;
    body->xposi.l = 100 << 16;
    body->yposi.l = 200 << 16;
    body->mstno.b.h = 6;
    set_actfree_word(body, 0, 1);
    set_actfree_word(body, 10, 1);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);

    reset_logs();
    body->r_no0 = 8;
    set_actfree_word(body, 0, 2);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);

    reset_logs();
    body->r_no0 = 8;
    set_actfree_word(body, 0, 1);

    nokogiri(body);

    assert_outer_callbacks(ctx, body, 66);
}

static void test_nokogiri_soundset_is_gated_by_visible_flag_and_counter(
    test_context *ctx) {
    sprite_status actor;

    memset(&actor, 0, sizeof(actor));
    actor.actflg = 0;
    _soundset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    actor.actflg = 128;
    set_actfree_word(&actor, 14, 30);
    _soundset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    _soundset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 177, soundset_requests[0]);
}

static void test_nokogiri_bar_initializes_when_parent_is_alive(
    test_context *ctx) {
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    actwk[3].actno = 39;
    bar_actor->userflag.b.l = -1;
    set_actfree_word(bar_actor, 0, 3);

    nokogiri(bar_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_nokogiri_bar_existing_actor_calls_actionsub(
    test_context *ctx) {
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    actwk[3].actno = 39;
    bar_actor->userflag.b.l = -1;
    bar_actor->r_no0 = 2;
    set_actfree_word(bar_actor, 0, 3);

    nokogiri(bar_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bar_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_nokogiri_bar_frames_out_when_parent_is_missing(
    test_context *ctx) {
    sprite_status *bar_actor = &actwk[20];

    reset_nokogiri_state();
    actwk[3].actno = 0;
    bar_actor->userflag.b.l = -1;
    set_actfree_word(bar_actor, 0, 3);

    nokogiri(bar_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == bar_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

TEST_MAIN_BEGIN;
    test_nokogiri_patterns_capture_default_literal_data(&ctx);
    test_nokogiri_initializes_body_spawns_bar_and_falls_through_open(&ctx);
    test_nokogiri_initializes_flipped_diagonal_bar_offset(&ctx);
    test_nokogiri_initial_negative_userflag_uses_large_bar_offset(&ctx);
    test_nokogiri_allocation_failure_frames_out_but_open_still_runs(&ctx);
    test_nokogiri_open_timer_paths(&ctx);
    test_nokogiri_opend_close_and_closed_timer_paths(&ctx);
    test_nokogiri_soundset_is_gated_by_visible_flag_and_counter(&ctx);
    test_nokogiri_bar_initializes_when_parent_is_alive(&ctx);
    test_nokogiri_bar_existing_actor_calls_actionsub(&ctx);
    test_nokogiri_bar_frames_out_when_parent_is_missing(&ctx);
TEST_MAIN_END

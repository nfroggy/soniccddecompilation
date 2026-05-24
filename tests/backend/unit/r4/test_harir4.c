#include <string.h>
#include <stddef.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 plpower_a;
Uint8 plpower_m;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;
static int soundset_count;
static Sint16 soundset_request;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);
void soundset(Sint16 ReqNo);

#include "src/r4/harir4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
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
    return -1;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_request = ReqNo;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
    soundset_count = 0;
    soundset_request = 0;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    plpower_a = 0;
    plpower_m = 0;
    reset_logs();
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    Uint16 bits = (Uint16)actor->actfree[offset] |
                  ((Uint16)actor->actfree[offset + 1] << 8);
    return (Sint16)bits;
}

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index, Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void assert_rendered_with_frameout_x(test_context *ctx,
                                            sprite_status *actor,
                                            Sint16 xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, xpos, frameout_s00_xpos);
}

static void test_harir4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, harir4pat[0] == &harir4_pat0);
    TEST_ASSERT_TRUE(ctx, harir4pat[1] == &harir4_pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, harir4_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, harir4_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, harir4_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 429, harir4_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 430, harir4_pat1.spra[0].index);
}

static void test_harir4_init_sets_flat_spike_defaults(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->xposi.w.h = 120;
    spike->yposi.w.h = 200;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 2, spike->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, spike->actflg);
    TEST_ASSERT_EQ_INT(ctx, 928, spike->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 4, spike->sprpri);
    TEST_ASSERT_TRUE(ctx, spike->patbase == harir4pat);
    TEST_ASSERT_EQ_INT(ctx, 120, get_legacy_word(spike, 29));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(spike, 27));
    TEST_ASSERT_EQ_INT(ctx, 18, spike->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, spike->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 208, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_rendered_with_frameout_x(ctx, spike, 120);
}

static void test_harir4_init_preserves_existing_frameout_origin(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->xposi.w.h = 120;
    spike->yposi.w.h = 200;
    set_legacy_word(spike, 29, 77);

    harir4_init(spike);

    TEST_ASSERT_EQ_INT(ctx, 77, get_legacy_word(spike, 29));
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_harir4_init_sets_vertical_spike_without_oscillation(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->xposi.w.h = 120;
    spike->yposi.w.h = 200;
    spike->userflag.b.h = 1;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, spike->patno);
    TEST_ASSERT_EQ_INT(ctx, 131, spike->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 200, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_rendered_with_frameout_x(ctx, spike, 120);
}

static void test_harir4_existing_actor_oscillates_down_then_waits(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->r_no0 = 2;
    spike->actflg = 128;
    spike->yposi.w.h = 200;
    set_legacy_word(spike, 27, 200);
    spike->actfree[17] = 24;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 32, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, spike->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 60, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 232, spike->yposi.w.h);
    assert_rendered_with_frameout_x(ctx, spike, 0);

    reset_logs();
    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 59, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 32, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_harir4_wait_expiry_plays_visible_sound(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->r_no0 = 2;
    spike->actflg = 128;
    set_legacy_word(spike, 27, 100);
    spike->actfree[16] = 1;
    spike->actfree[17] = 32;
    spike->actfree[18] = 1;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 183, soundset_request);
    TEST_ASSERT_EQ_INT(ctx, 24, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 124, spike->yposi.w.h);
}

static void test_harir4_oscillation_reaches_top_and_waits(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->r_no0 = 2;
    set_legacy_word(spike, 27, 100);
    spike->actfree[17] = 4;
    spike->actfree[18] = 1;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 60, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 100, spike->yposi.w.h);
}

static void test_harir4_attached_actor_tracks_live_parent(test_context *ctx) {
    sprite_status *spike = &actwk[3];
    sprite_status *parent = &actwk[9];

    reset_state();
    spike->r_no0 = 2;
    spike->userflag.b.h = 1;
    spike->actfree[14] = 5;
    spike->actfree[15] = (Uint8)-7;
    set_legacy_word(spike, 28, 9);
    parent->actno = 77;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 105, spike->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 449, spike->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_rendered_with_frameout_x(ctx, spike, get_legacy_word(spike, 29));
}

static void test_harir4_attached_actor_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->r_no0 = 2;
    spike->userflag.b.h = 1;
    set_legacy_word(spike, 28, 9);

    harir4(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == spike);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_harir4_ridechk_vertical_spike_only_checks_hit(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_state();
    spike->patno = 1;
    hitchk_result = 1;

    harir4_ridechk(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == spike);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir4_ridechk_damages_vulnerable_player(test_context *ctx) {
    sprite_status *spike = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_state();
    hitchk_result = 1;
    spike->cddat = 8;
    player->r_no0 = 2;
    player->yspeed.w = 5;
    player->yposi.l = 200 << 16;

    harir4_ridechk(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == spike);
    TEST_ASSERT_EQ_INT(ctx, 199, player->yposi.w.h);
}

static void test_harir4_ridechk_damage_gates(test_context *ctx) {
    sprite_status *spike = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_state();
    hitchk_result = 0;
    spike->cddat = 8;
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_state();
    hitchk_result = 1;
    spike->cddat = 0;
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_state();
    hitchk_result = 1;
    spike->cddat = 8;
    plpower_a = 1;
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_state();
    hitchk_result = 1;
    spike->cddat = 8;
    plpower_m = 1;
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_state();
    hitchk_result = 1;
    spike->cddat = 8;
    player->r_no0 = 4;
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);

    reset_state();
    hitchk_result = 1;
    spike->cddat = 8;
    player->r_no0 = 2;
    set_legacy_word(player, 26, 1);
    harir4_ridechk(spike);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

TEST_MAIN_BEGIN;
    test_harir4_patterns_capture_literal_data(&ctx);
    test_harir4_init_sets_flat_spike_defaults(&ctx);
    test_harir4_init_preserves_existing_frameout_origin(&ctx);
    test_harir4_init_sets_vertical_spike_without_oscillation(&ctx);
    test_harir4_existing_actor_oscillates_down_then_waits(&ctx);
    test_harir4_wait_expiry_plays_visible_sound(&ctx);
    test_harir4_oscillation_reaches_top_and_waits(&ctx);
    test_harir4_attached_actor_tracks_live_parent(&ctx);
    test_harir4_attached_actor_frames_out_when_parent_is_gone(&ctx);
    test_harir4_ridechk_vertical_spike_only_checks_hit(&ctx);
    test_harir4_ridechk_damages_vulnerable_player(&ctx);
    test_harir4_ridechk_damage_gates(&ctx);
TEST_MAIN_END

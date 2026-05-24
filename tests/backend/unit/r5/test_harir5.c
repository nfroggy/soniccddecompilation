#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;
Uint8 flagwork[766];
Uint8 time_flag;
Uint8 plpower_a;
Uint8 plpower_m;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);

#include "src/r5/harir5.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    ++playdamageset_count;
    playdamageset_player = pActwk;
    playdamageset_actor = pColliAct;
    return 0;
}

static void reset_harir5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(flagwork, 0, sizeof(flagwork));
    time_flag = 0;
    plpower_a = 0;
    plpower_m = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void assert_tail_action_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void prime_moving_origin(sprite_status *spike) {
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->userflag.b.h = 4;
    harir5(spike);
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
}

static void test_init_sets_type_specific_size_collision_and_pattern(
    test_context *ctx) {
    Uint8 expected_h[4] = {18, 18, 10, 10};
    Uint8 expected_v[4] = {12, 12, 14, 14};
    Uint8 expected_colino[4] = {0, 131, 130, 130};

    for (int i = 0; i < 4; ++i) {
        sprite_status *spike = &actwk[3];
        reset_harir5_state();
        scra_h_posit.w.h = 128;
        spike->xposi.w.h = 128;
        spike->yposi.w.h = 240;
        spike->userflag.b.h = (Sint8)i;

        harir5(spike);

        TEST_ASSERT_EQ_INT(ctx, 2, spike->r_no0);
        TEST_ASSERT_EQ_INT(ctx, 4, spike->actflg);
        TEST_ASSERT_EQ_INT(ctx, 1099, spike->sproffset);
        TEST_ASSERT_EQ_INT(ctx, 3, spike->sprpri);
        TEST_ASSERT_TRUE(ctx, spike->patbase == harir5pat);
        TEST_ASSERT_EQ_INT(ctx, i, spike->patno);
        TEST_ASSERT_EQ_INT(ctx, expected_h[i], spike->sprhsize);
        TEST_ASSERT_EQ_INT(ctx, expected_v[i], spike->sprvsize);
        TEST_ASSERT_EQ_INT(ctx, expected_colino[i], spike->colino);
        TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
        TEST_ASSERT_TRUE(ctx, hitchk_actor == spike);
        TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
        assert_tail_action_only(ctx, spike);
    }
}

static void test_motion_kinds_use_origin_and_actfree_amplitude(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->userflag.b.h = 4;
    harir5(spike);
    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 248, spike->yposi.w.h);

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->userflag.b.h = 5;
    harir5(spike);
    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 232, spike->yposi.w.h);

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->userflag.b.h = 6;
    harir5(spike);
    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 120, spike->xposi.w.h);

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->userflag.b.h = 7;
    harir5(spike);
    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 136, spike->xposi.w.h);
}

static void test_motion_timer_growth_pause_and_reverse_edges(test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_harir5_state();
    prime_moving_origin(spike);
    spike->actfree[16] = 1;
    spike->actfree[17] = 24;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 32, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, spike->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 60, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 272, spike->yposi.w.h);

    reset_harir5_state();
    prime_moving_origin(spike);
    spike->actfree[16] = 2;
    spike->actfree[17] = 24;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 24, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 264, spike->yposi.w.h);

    reset_harir5_state();
    prime_moving_origin(spike);
    spike->actfree[17] = 16;
    spike->actfree[18] = 1;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 8, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 1, spike->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 248, spike->yposi.w.h);

    reset_harir5_state();
    prime_moving_origin(spike);
    spike->actfree[17] = 0;
    spike->actfree[18] = 1;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, spike->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 60, spike->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 240, spike->yposi.w.h);
}

static void test_flat_spikes_damage_player_when_all_guards_allow(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->cddat = 8;
    player->r_no0 = 2;
    player->yspeed.w = 256;
    player->yposi.l = 100 << 16;
    hitchk_result = 1;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == spike);
    TEST_ASSERT_EQ_INT(ctx, 99 << 16, player->yposi.l);
    assert_tail_action_only(ctx, spike);

    reset_harir5_state();
    scra_h_posit.w.h = 128;
    spike->xposi.w.h = 128;
    spike->yposi.w.h = 240;
    spike->cddat = 8;
    player->r_no0 = 2;
    hitchk_result = 1;
    plpower_a = 1;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_parent_follow_uses_actfree_offsets(test_context *ctx) {
    sprite_status *parent = &actwk[5];
    sprite_status *child = &actwk[3];

    reset_harir5_state();
    parent->xposi.w.h = 200;
    parent->yposi.w.h = 300;
    child->r_no0 = 2;
    child->userflag.b.h = 1;
    child->patno = 1;
    child->xposi.w.h = 128;
    child->yposi.w.h = 240;
    set_actfree_word(child, 10, 5);
    set_actfree_word(child, 12, 1541);
    child->actfree[14] = 5;
    child->actfree[15] = 6;
    scra_h_posit.w.h = 1664;

    harir5(child);

    TEST_ASSERT_EQ_INT(ctx, 205, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 306, child->yposi.w.h);
    assert_tail_action_only(ctx, child);
}

static void test_offscreen_clears_persistent_flag_and_frames_out(
    test_context *ctx) {
    sprite_status *spike = &actwk[3];

    reset_harir5_state();
    spike->xposi.w.h = 1000;
    spike->yposi.w.h = 240;
    spike->cdsts = 2;
    time_flag = 1;
    flagwork[7] = 255;
    scra_h_posit.w.h = 128;

    harir5(spike);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[7]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == spike);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_init_sets_type_specific_size_collision_and_pattern(&ctx);
    test_motion_kinds_use_origin_and_actfree_amplitude(&ctx);
    test_motion_timer_growth_pause_and_reverse_edges(&ctx);
    test_flat_spikes_damage_player_when_all_guards_allow(&ctx);
    test_parent_follow_uses_actfree_offsets(&ctx);
    test_offscreen_clears_persistent_flag_and_frames_out(&ctx);
TEST_MAIN_END

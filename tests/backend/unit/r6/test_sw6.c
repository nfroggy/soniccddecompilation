#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[4];

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);
Uint8 bchg(Uint8 bySrc, Uint8 byDst);

#include "src/r6/sw6.c"

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

Uint8 bchg(Uint8 bySrc, Uint8 byDst) {
    return (Uint8)(byDst ^ (1 << bySrc));
}

static void reset_sw6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void assert_common_callbacks(test_context *ctx, sprite_status *sw) {
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == sw);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == sw);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == sw);
}

static void test_sw6_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_sw6[0] == &pat0_sw6);
    TEST_ASSERT_TRUE(ctx, pat_sw6[1] == &pat1_sw6);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0_sw6.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0_sw6.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat0_sw6.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 307, pat0_sw6.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -4, pat1_sw6.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 308, pat1_sw6.spra[0].index);
}

static void test_sw6_initializes_and_clears_inactive_switch(test_context *ctx) {
    sprite_status *sw = &actwk[3];

    reset_sw6_state();
    sw->userflag.b.h = 4;
    sw->xposi.w.h = 100;
    sw->yposi.w.h = 50;
    switchflag[4] = 255;
    sw6(sw);

    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[4]);
    assert_common_callbacks(ctx, sw);
}

static void test_sw6_press_transition_toggles_switch_and_sprite(
    test_context *ctx) {
    sprite_status *sw = &actwk[3];

    reset_sw6_state();
    sw->r_no0 = 2;
    sw->actflg = 128;
    sw6_get_work(sw)->switch_index = 5;
    sw6_get_work(sw)->current_state = 0;
    sw->sprvsize = 8;
    sw->patno = 0;
    sw->yposi.w.h = 20;
    actwk[0].yposi.w.h = 100;
    hitchk_result = 1;
    sw6(sw);

    TEST_ASSERT_EQ_INT(ctx, 224, switchflag[5]);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);
    assert_common_callbacks(ctx, sw);
}

static void test_sw6_held_press_keeps_shape_after_initial_transition(
    test_context *ctx) {
    sprite_status *sw = &actwk[3];

    reset_sw6_state();
    sw->r_no0 = 2;
    sw6_get_work(sw)->switch_index = 6;
    sw6_get_work(sw)->current_state = 255;
    sw->sprvsize = 4;
    sw->patno = 1;
    sw->yposi.w.h = 24;
    actwk[0].yposi.w.h = 108;
    hitchk_result = 1;
    sw6(sw);

    TEST_ASSERT_EQ_INT(ctx, 192, switchflag[6]);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, sw);
}

static void test_sw6_release_transition_restores_switch_shape(
    test_context *ctx) {
    sprite_status *sw = &actwk[3];

    reset_sw6_state();
    sw->r_no0 = 2;
    sw6_get_work(sw)->switch_index = 7;
    sw6_get_work(sw)->current_state = 255;
    sw->sprvsize = 4;
    sw->patno = 1;
    sw->yposi.w.h = 24;
    actwk[0].yposi.w.h = 108;
    switchflag[7] = 255;
    hitchk_result = 0;
    sw6(sw);

    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[7]);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, sw);
}

TEST_MAIN_BEGIN;
test_sw6_patterns_capture_literal_data(&ctx);
test_sw6_initializes_and_clears_inactive_switch(&ctx);
test_sw6_press_transition_toggles_switch_and_sprite(&ctx);
test_sw6_held_press_keeps_shape_after_initial_transition(&ctx);
test_sw6_release_transition_restores_switch_shape(&ctx);
TEST_MAIN_END

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

#include "src/r8/sw8.c"

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

static void reset_sw8_state(void) {
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

static void assert_common_callbacks(test_context *ctx, sprite_status *sw_actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == sw_actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == sw_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == sw_actor);
}

static void initialize_switch(sprite_status *sw_actor, Sint16 index) {
    sw_actor->userflag.b.h = (Sint8)index;
    sw(sw_actor);
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_sw8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_sw[0] == &sw00);
    TEST_ASSERT_TRUE(ctx, pat_sw[1] == &sw01);
    TEST_ASSERT_EQ_INT(ctx, 2, sw00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, sw00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, sw00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, sw00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SW8_BASE, sw00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, sw00.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, sw00.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, 8, sw00.spra[1].etc);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SW8_BASE, sw00.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, 2, sw01.cnt);
    TEST_ASSERT_EQ_INT(ctx, -4, sw01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SW8_BASE + 1, sw01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, sw01.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, -4, sw01.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, 8, sw01.spra[1].etc);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_SW8_BASE + 1, sw01.spra[1].index);
}

static void test_sw8_initializes_without_running_move(test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    sw_actor->userflag.b.h = 4;
    sw_actor->actflg = 128;
    switchflag[4] = 255;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 255, switchflag[4]);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_sw8_clears_inactive_switch_flag(test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    initialize_switch(sw_actor, 5);
    switchflag[5] = 255;
    sw8_work_get(sw_actor)->current_pressed = 0;
    sw_actor->yposi.w.h = 20;
    actwk[0].yposi.w.h = 20;
    hitchk_result = 0;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[5]);
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, sw_actor);
}

static void test_sw8_ignores_hit_when_player_is_below_switch(test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    initialize_switch(sw_actor, 6);
    switchflag[6] = 255;
    sw8_work_get(sw_actor)->current_pressed = 0;
    sw_actor->yposi.w.h = 20;
    actwk[0].yposi.w.h = 21;
    hitchk_result = 1;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[6]);
    TEST_ASSERT_EQ_INT(ctx, 21, actwk[0].yposi.w.h);
    assert_common_callbacks(ctx, sw_actor);
}

static void test_sw8_press_transition_toggles_switch_and_sprite(
    test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    initialize_switch(sw_actor, 7);
    sw_actor->actflg = 128;
    sw8_work_get(sw_actor)->current_pressed = 0;
    sw_actor->sprvsize = 8;
    sw_actor->patno = 0;
    sw_actor->yposi.w.h = 20;
    actwk[0].yposi.w.h = 20;
    hitchk_result = 1;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 224, switchflag[7]);
    TEST_ASSERT_EQ_INT(ctx, 28, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);
    assert_common_callbacks(ctx, sw_actor);
}

static void test_sw8_held_press_keeps_pressed_shape(test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    initialize_switch(sw_actor, 8);
    sw8_work_get(sw_actor)->current_pressed = 255;
    sw_actor->sprvsize = 4;
    sw_actor->patno = 1;
    sw_actor->yposi.w.h = 24;
    actwk[0].yposi.w.h = 24;
    hitchk_result = 1;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 192, switchflag[8]);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, sw_actor);
}

static void test_sw8_release_transition_restores_switch_shape(
    test_context *ctx) {
    sprite_status *sw_actor = &actwk[3];

    reset_sw8_state();
    initialize_switch(sw_actor, 9);
    sw8_work_get(sw_actor)->current_pressed = 255;
    sw_actor->sprvsize = 4;
    sw_actor->patno = 1;
    sw_actor->yposi.w.h = 24;
    actwk[0].yposi.w.h = 28;
    switchflag[9] = 255;
    hitchk_result = 0;

    sw(sw_actor);

    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[9]);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, sw_actor);
}

TEST_MAIN_BEGIN;
    test_sw8_patterns_capture_literal_data(&ctx);
    test_sw8_initializes_without_running_move(&ctx);
    test_sw8_clears_inactive_switch_flag(&ctx);
    test_sw8_ignores_hit_when_player_is_below_switch(&ctx);
    test_sw8_press_transition_toggles_switch_and_sprite(&ctx);
    test_sw8_held_press_keeps_pressed_shape(&ctx);
    test_sw8_release_transition_restores_switch_shape(&ctx);
TEST_MAIN_END


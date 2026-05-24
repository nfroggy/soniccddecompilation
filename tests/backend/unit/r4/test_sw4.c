#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s0(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r4/sw4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_callbacks(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
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

static int legacy_byte_actfree_offset(int byte_index) {
    return byte_index - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index, Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void set_link_offset_x(sprite_status *actor, Sint8 value) {
    actor->actfree[legacy_byte_actfree_offset(60)] = (Uint8)value;
}

static void set_link_offset_y(sprite_status *actor, Sint8 value) {
    actor->actfree[legacy_byte_actfree_offset(61)] = (Uint8)value;
}

static Sint8 get_link_offset_y(sprite_status *actor) {
    return (Sint8)actor->actfree[legacy_byte_actfree_offset(61)];
}

static void assert_action_and_hitch(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
}

static void test_sw4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_sw[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_sw[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 472, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -4, pat01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 473, pat01.spra[0].index);
}

static void test_sw4_initializes_and_clears_switch_when_not_pressed(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];

    reset_state();
    sw->userflag.b.h = 3;
    switchflag[3] = 255;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 2, sw->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, sw->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, sw->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, sw->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 10, sw->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1290, sw->sproffset);
    TEST_ASSERT_TRUE(ctx, sw->patbase == pat_sw);
    TEST_ASSERT_EQ_INT(ctx, 3, get_legacy_word(sw, 32));
    TEST_ASSERT_EQ_INT(ctx, 0, sw->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[3]);
    assert_action_and_hitch(ctx, sw);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == sw);
}

static void test_sw4_press_transition_sets_flags_sprite_and_sound(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->r_no0 = 2;
    sw->actflg = 128;
    sw->yposi.w.h = 100;
    sw->sprvsize = 10;
    set_legacy_word(sw, 32, 4);
    player->sprvsize = 5;
    hitchk_result = 1;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 255, sw->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 224, switchflag[4]);
    TEST_ASSERT_EQ_INT(ctx, 94, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 104, sw->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, sw->patno);
    TEST_ASSERT_EQ_INT(ctx, 6, sw->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);
    assert_action_and_hitch(ctx, sw);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_sw4_press_without_visible_flag_is_silent(test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->r_no0 = 2;
    sw->yposi.w.h = 40;
    sw->sprvsize = 6;
    set_legacy_word(sw, 32, 2);
    player->sprvsize = 3;
    hitchk_result = 1;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 255, sw->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 44, sw->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->patno + 1 - 2);
}

static void test_sw4_release_transition_restores_sprite_and_player(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->r_no0 = 2;
    sw->yposi.w.h = 104;
    sw->sprvsize = 6;
    sw->patno = 1;
    sw->actfree[21] = 255;
    set_legacy_word(sw, 32, 4);
    switchflag[4] = 255;
    player->yposi.w.h = 90;
    hitchk_result = 0;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 255, sw->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[4]);
    TEST_ASSERT_EQ_INT(ctx, 82, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, sw->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->patno);
    TEST_ASSERT_EQ_INT(ctx, 10, sw->sprvsize);
    assert_action_and_hitch(ctx, sw);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_sw4_hosei_zero_result_does_not_press_switch(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    sw->r_no0 = 2;
    sw->yposi.w.h = 14;
    sw->sprvsize = 8;
    set_legacy_word(sw, 32, 6);
    switchflag[6] = 255;
    player->sprvsize = 7;
    hitchk_result = 1;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 0, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 127, switchflag[6]);
    TEST_ASSERT_EQ_INT(ctx, 0, sw->patno);
    TEST_ASSERT_EQ_INT(ctx, 8, sw->sprvsize);
}

static void test_sw4_linked_actor_follows_parent_and_stays_onscreen(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *parent = &actwk[10];

    reset_state();
    sw->r_no0 = 2;
    sw->xposi.w.h = 100;
    set_legacy_word(sw, 28, 10);
    set_legacy_word(sw, 32, 1);
    set_link_offset_x(sw, 5);
    set_link_offset_y(sw, -3);
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;
    set_legacy_word(parent, 29, 256);
    scra_h_posit.w.h = 128;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 305, sw->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 397, sw->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -13056, sw->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
}

static void test_sw4_linked_press_and_release_adjust_stored_y_offset(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *parent = &actwk[10];

    reset_state();
    sw->r_no0 = 2;
    sw->sprvsize = 10;
    set_legacy_word(sw, 28, 10);
    set_legacy_word(sw, 32, 1);
    set_link_offset_y(sw, 0);
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 100;
    set_legacy_word(parent, 29, 256);
    actwk[0].sprvsize = 5;
    hitchk_result = 1;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 4, get_link_offset_y(sw));
    TEST_ASSERT_EQ_INT(ctx, 255, sw->actfree[21]);

    reset_callbacks();
    sw->actfree[21] = 255;
    sw->sprvsize = 6;
    hitchk_result = 0;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 0, get_link_offset_y(sw));
    TEST_ASSERT_EQ_INT(ctx, 0, sw->actfree[21]);
}

static void test_sw4_linked_actor_frames_out_when_parent_offscreen(
    test_context *ctx) {
    sprite_status *sw = &actwk[5];
    sprite_status *parent = &actwk[10];

    reset_state();
    sw->r_no0 = 2;
    set_legacy_word(sw, 28, 10);
    set_legacy_word(sw, 32, 1);
    set_legacy_word(parent, 29, 1024);
    scra_h_posit.w.h = 0;

    sw4(sw);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == sw);
}

TEST_MAIN_BEGIN;
    test_sw4_patterns_capture_literal_data(&ctx);
    test_sw4_initializes_and_clears_switch_when_not_pressed(&ctx);
    test_sw4_press_transition_sets_flags_sprite_and_sound(&ctx);
    test_sw4_press_without_visible_flag_is_silent(&ctx);
    test_sw4_release_transition_restores_sprite_and_player(&ctx);
    test_sw4_hosei_zero_result_does_not_press_switch(&ctx);
    test_sw4_linked_actor_follows_parent_and_stays_onscreen(&ctx);
    test_sw4_linked_press_and_release_adjust_stored_y_offset(&ctx);
    test_sw4_linked_actor_frames_out_when_parent_offscreen(&ctx);
TEST_MAIN_END

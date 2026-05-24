#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void soundset(Sint16 ReqNo);

#include "src/r4/daid4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
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

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static int legacy_long_actfree_offset(int long_index) {
    return (long_index * 4) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index, Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void set_legacy_long(sprite_status *actor, int long_index, Sint32 value) {
    set_actfree_long(actor, legacy_long_actfree_offset(long_index), value);
}

static Sint32 get_legacy_long(sprite_status *actor, int long_index) {
    return get_actfree_long(actor, legacy_long_actfree_offset(long_index));
}

static void assert_action_frame(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_daid4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_daid4[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, pat_daid4[1] == &pat1);
    TEST_ASSERT_EQ_INT(ctx, 2, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -32, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 501, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 503, pat0.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, -34, pat1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 502, pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 504, pat1.spra[1].index);
}

static void test_daid4_init_records_origin_and_static_fields(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->yposi.w.h = 240;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, platform->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 32, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 33, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17168, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == pat_daid4);
    TEST_ASSERT_EQ_INT(ctx, 240, get_legacy_word(platform, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_daid4_born_waits_until_player_is_low_enough(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->yposi.w.h = 100;
    set_legacy_word(platform, 23, 100);
    actwk[0].yposi.w.h = 300;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == platform);
}

static void test_daid4_born_moves_to_player_offset_and_enters_off(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    platform->yposi.w.h = 100;
    set_legacy_word(platform, 23, 100);
    actwk[0].yposi.w.h = 400;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 176, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_daid4_off_initializes_fall_and_keeps_rising_speed(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 4;
    platform->patno = 3;
    platform->sprvsize = 20;
    platform->yspeed.w = 99;
    platform->yposi.w.h = 100;
    actwk[0].yposi.w.h = 100;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 6, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 33, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1024, get_legacy_long(platform, 12));
    assert_action_frame(ctx, platform);
}

static void test_daid4_off1_caps_fall_speed(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 6;
    platform->actflg = 128;
    platform->yposi.w.h = 100;
    set_legacy_long(platform, 12, 65536);

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 101, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 65536, get_legacy_long(platform, 12));
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_frame(ctx, platform);
}

static void test_daid4_off1_resets_when_far_from_player(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 6;
    platform->yposi.w.h = 0;
    actwk[0].yposi.w.h = 300;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    assert_action_frame(ctx, platform);
}

static void test_daid4_off1_ride_switches_to_on_state(test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 6;
    platform->yposi.w.h = 100;
    platform->sprvsize = 33;
    player->yposi.w.h = 110;
    ridechk_result = 1;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 8, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 102, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 114, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == player);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);
    assert_action_frame(ctx, platform);
}

static void test_daid4_on_resets_when_it_reaches_origin(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 8;
    platform->patno = 0;
    platform->sprvsize = 33;
    platform->yposi.w.h = 100;
    set_legacy_word(platform, 23, 120);

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 31, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, -512, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 98, platform->yposi.w.h);
    assert_action_frame(ctx, platform);
}

static void test_daid4_on1_without_ride_switches_back_to_off(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 10;
    platform->yposi.w.h = 150;
    set_legacy_word(platform, 23, 100);
    player->yposi.w.h = 80;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 146, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 76, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_frame(ctx, platform);
}

static void test_daid4_on1_with_ride_keeps_on_state(test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 10;
    platform->yposi.w.h = 150;
    set_legacy_word(platform, 23, 100);
    player->yposi.w.h = 80;
    ridechk_result = 1;

    daid4(platform);

    TEST_ASSERT_EQ_INT(ctx, 10, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 148, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_frame(ctx, platform);
}

TEST_MAIN_BEGIN;
    test_daid4_patterns_capture_literal_data(&ctx);
    test_daid4_init_records_origin_and_static_fields(&ctx);
    test_daid4_born_waits_until_player_is_low_enough(&ctx);
    test_daid4_born_moves_to_player_offset_and_enters_off(&ctx);
    test_daid4_off_initializes_fall_and_keeps_rising_speed(&ctx);
    test_daid4_off1_caps_fall_speed(&ctx);
    test_daid4_off1_resets_when_far_from_player(&ctx);
    test_daid4_off1_ride_switches_to_on_state(&ctx);
    test_daid4_on_resets_when_it_reaches_origin(&ctx);
    test_daid4_on1_without_ride_switches_back_to_off(&ctx);
    test_daid4_on1_with_ride_keeps_on_state(&ctx);
TEST_MAIN_END

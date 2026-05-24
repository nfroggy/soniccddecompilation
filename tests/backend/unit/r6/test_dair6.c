#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union gametimer;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r6/dair6.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_dair6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&gametimer, 0, sizeof(gametimer));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    sinset_angle = 0;
    sinset_sin = 64;
    sinset_cos = 0;
}

static void init_platform(sprite_status *platform) {
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    dair6_init(platform);
}

static void test_dair6_init_sets_layout_and_origin(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair6_state();
    init_platform(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17384, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == dair6pat);
    TEST_ASSERT_EQ_INT(ctx, 12, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 24, platform->sprhsize);
}

static void test_dair6_sub2_uses_timer_low_byte_and_scaled_sine(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    Sint16 offset;

    reset_dair6_state();
    gametimer.w = 0x1234;
    sinset_sin = 80;

    offset = dair6_sub2(platform);

    TEST_ASSERT_EQ_INT(ctx, 0x34, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 20, offset);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->actfree[16]);
}

static void test_dair6_moves_right_and_left_with_visible_x_speed(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair6_state();
    init_platform(platform);
    sinset_sin = 64;

    mv_right(platform);

    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);

    reset_dair6_state();
    platform = &actwk[8];
    init_platform(platform);
    sinset_sin = 64;

    mv_left(platform);

    TEST_ASSERT_EQ_INT(ctx, 84, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, platform->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
}

static void test_dair6_moves_up_and_down_from_origin(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair6_state();
    init_platform(platform);
    sinset_sin = 64;

    mv_down(platform);

    TEST_ASSERT_EQ_INT(ctx, 216, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);

    reset_dair6_state();
    platform = &actwk[8];
    init_platform(platform);
    sinset_sin = 64;

    mv_up(platform);

    TEST_ASSERT_EQ_INT(ctx, 184, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
}

static void test_dair6_move_dispatches_all_userflag_directions(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair6_state();
    init_platform(platform);
    sinset_sin = 64;
    platform->userflag.b.h = 0;
    dair6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);

    reset_dair6_state();
    platform = &actwk[8];
    init_platform(platform);
    sinset_sin = 64;
    platform->userflag.b.h = 1;
    dair6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 84, platform->xposi.w.h);

    reset_dair6_state();
    platform = &actwk[8];
    init_platform(platform);
    sinset_sin = 64;
    platform->userflag.b.h = 2;
    dair6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 184, platform->yposi.w.h);

    reset_dair6_state();
    platform = &actwk[8];
    init_platform(platform);
    sinset_sin = 64;
    platform->userflag.b.h = 3;
    dair6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 216, platform->yposi.w.h);
}

static void test_dair6_entry_dispatches_callbacks(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_dair6_state();
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;
    sinset_sin = 64;

    dair6(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

TEST_MAIN_BEGIN;
    test_dair6_init_sets_layout_and_origin(&ctx);
    test_dair6_sub2_uses_timer_low_byte_and_scaled_sine(&ctx);
    test_dair6_moves_right_and_left_with_visible_x_speed(&ctx);
    test_dair6_moves_up_and_down_from_origin(&ctx);
    test_dair6_move_dispatches_all_userflag_directions(&ctx);
    test_dair6_entry_dispatches_callbacks(&ctx);
TEST_MAIN_END

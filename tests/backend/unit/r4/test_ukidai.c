#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Sint16 watermoveposi;
Sint16 waterposi;
Sint16 waterposi_m;
Uint8 waterspeed;
Uint8 water_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int emycol_l3_count;
static sprite_status *emycol_l3_actor;
static Sint16 emycol_l3_x;
static Sint16 emycol_l3_y;
static Sint16 emycol_l3_results[8];
static int emycol_r3_count;
static sprite_status *emycol_r3_actor;
static Sint16 emycol_r3_x;
static Sint16 emycol_r3_y;
static Sint16 emycol_r3_results[8];
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
Sint16 emycol_l3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 emycol_r3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/ukidai.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint16 emycol_l3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    Sint16 result = emycol_l3_results[emycol_l3_count];
    ++emycol_l3_count;
    emycol_l3_actor = pActwk;
    emycol_l3_x = iXposi;
    emycol_l3_y = iYposi;
    return result;
}

Sint16 emycol_r3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    Sint16 result = emycol_r3_results[emycol_r3_count];
    ++emycol_r3_count;
    emycol_r3_actor = pActwk;
    emycol_r3_x = iXposi;
    emycol_r3_y = iYposi;
    return result;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
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
    return ridechk_result;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    watermoveposi = 0;
    waterposi = 0;
    waterposi_m = 0;
    waterspeed = 0;
    water_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    emycol_l3_count = 0;
    emycol_l3_actor = 0;
    emycol_l3_x = 0;
    emycol_l3_y = 0;
    memset(emycol_l3_results, 0, sizeof(emycol_l3_results));
    emycol_r3_count = 0;
    emycol_r3_actor = 0;
    emycol_r3_x = 0;
    emycol_r3_y = 0;
    memset(emycol_r3_results, 0, sizeof(emycol_r3_results));
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
}

static void test_ukidai_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_ukidai[0] == &pat00);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 530, pat00.spra[0].index);
}

static void test_ukidai_init_rejects_unexpected_water_target(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->xposi.w.h = 100;
    watermoveposi = 1234;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 848, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == pat_ukidai);
    TEST_ASSERT_EQ_INT(ctx, 100, ukidai_work_get(platform)->origin_x);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_ukidai_moving_waits_when_water_has_not_arrived(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->xposi.w.h = 100;
    watermoveposi = 1920;
    waterposi_m = 1504;
    waterposi = 300;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 300, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
}

static void test_ukidai_moving_unexpected_arrival_frames_out(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    watermoveposi = 1234;
    waterposi_m = 1234;
    waterposi = 310;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 310, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == platform);
}

static void test_ukidai_moving_arrival_at_1920_enters_wait(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    watermoveposi = 1920;
    waterposi_m = 1920;
    waterposi = 330;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 330, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_ukidai_moving_arrival_at_1504_starts_move(test_context *ctx) {
    sprite_status *platform = &actwk[5];

    reset_state();
    platform->r_no0 = 2;
    watermoveposi = 1504;
    waterposi_m = 1504;
    waterposi = 340;
    waterspeed = 2;
    water_flag = 3;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 8, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -515, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 340, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
}

static void test_ukidai_wait_ride_starts_descent(test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 4;
    platform->xposi.w.h = 200;
    player->xposi.w.h = 222;
    waterposi = 320;
    ridechk_result = 1;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 6, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 222, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 320, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1504, watermoveposi);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == player);
}

static void test_ukidai_move_builds_water_speed_and_skips_zero_x_collision(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 6;
    platform->xposi.w.h = 0;
    player->xposi.w.h = 123;
    waterposi = 340;
    waterspeed = 2;
    water_flag = 3;
    ridechk_result = 1;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 8, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -515, platform->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 340, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_l3_count);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_r3_count);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xposi.w.h);
}

static void test_ukidai_move1_right_short_distance_stops_without_moving(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 8;
    platform->xposi.w.h = 100;
    platform->sprhs = 16;
    player->xposi.w.h = 101;
    waterposi = 350;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r3_count);
    TEST_ASSERT_TRUE(ctx, emycol_r3_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 116, emycol_r3_x);
    TEST_ASSERT_EQ_INT(ctx, 357, emycol_r3_y);
    TEST_ASSERT_EQ_INT(ctx, 100, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, ukidai_work_get(platform)->collision_side);
    TEST_ASSERT_EQ_INT(ctx, 1, ukidai_work_get(platform)->step_direction);
    TEST_ASSERT_EQ_INT(ctx, 16, ukidai_work_get(platform)->probe_x_offset);
    TEST_ASSERT_EQ_INT(ctx, 0, ukidai_work_get(platform)->remaining_delta);
    TEST_ASSERT_EQ_INT(ctx, 357, ukidai_work_get(platform)->probe_y);
}

static void test_ukidai_move1_right_walks_until_collision(test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 8;
    platform->xposi.w.h = 100;
    platform->sprhs = 16;
    player->xposi.w.h = 103;
    waterposi = 360;
    emycol_r3_results[0] = 0;
    emycol_r3_results[1] = -1;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, emycol_r3_count);
    TEST_ASSERT_EQ_INT(ctx, 101, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, ukidai_work_get(platform)->remaining_delta);
}

static void test_ukidai_move1_left_uses_left_probe_and_collision_break(
    test_context *ctx) {
    sprite_status *platform = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    platform->r_no0 = 8;
    platform->xposi.w.h = 100;
    platform->sprhs = 16;
    player->xposi.w.h = 97;
    waterposi = 370;
    emycol_l3_results[0] = -1;

    ukidai(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_l3_count);
    TEST_ASSERT_TRUE(ctx, emycol_l3_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 91, emycol_l3_x);
    TEST_ASSERT_EQ_INT(ctx, 377, emycol_l3_y);
    TEST_ASSERT_EQ_INT(ctx, 100, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ukidai_work_get(platform)->collision_side);
    TEST_ASSERT_EQ_INT(ctx, -1, ukidai_work_get(platform)->step_direction);
    TEST_ASSERT_EQ_INT(ctx, -16, ukidai_work_get(platform)->probe_x_offset);
    TEST_ASSERT_EQ_INT(ctx, 3, ukidai_work_get(platform)->remaining_delta);
}

TEST_MAIN_BEGIN;
    test_ukidai_patterns_capture_literal_data(&ctx);
    test_ukidai_init_rejects_unexpected_water_target(&ctx);
    test_ukidai_moving_waits_when_water_has_not_arrived(&ctx);
    test_ukidai_moving_unexpected_arrival_frames_out(&ctx);
    test_ukidai_moving_arrival_at_1920_enters_wait(&ctx);
    test_ukidai_moving_arrival_at_1504_starts_move(&ctx);
    test_ukidai_wait_ride_starts_descent(&ctx);
    test_ukidai_move_builds_water_speed_and_skips_zero_x_collision(&ctx);
    test_ukidai_move1_right_short_distance_stops_without_moving(&ctx);
    test_ukidai_move1_right_walks_until_collision(&ctx);
    test_ukidai_move1_left_uses_left_probe_and_collision_break(&ctx);
TEST_MAIN_END

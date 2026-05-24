#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union gametimer;
Uint8 time_flag;
Uint8 generate_flag;

static int scramapad_count;
static Sint16 scramapad_x[8];
static Sint16 scramapad_y[8];
static Sint16 scramapad_result;
static int actwkchk_count;
static int actwkchk_fail;
static sprite_status *actwkchk_actor;

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/r7/playsp7.c"

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    (void)pActwk;
    if (scramapad_count < 8) {
        scramapad_x[scramapad_count] = iXposi;
        scramapad_y[scramapad_count] = iYposi;
    }
    ++scramapad_count;
    return scramapad_result;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_actor;
    return 0;
}

static void reset_playsp7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&gametimer, 0, sizeof(gametimer));
    time_flag = 0;
    generate_flag = 0;
    scramapad_count = 0;
    memset(scramapad_x, 0, sizeof(scramapad_x));
    memset(scramapad_y, 0, sizeof(scramapad_y));
    scramapad_result = 0;
    actwkchk_count = 0;
    actwkchk_fail = 0;
    actwkchk_actor = &actwk[9];
}

static void setup_fast_player(void) {
    actwk[0].xposi.w.h = 320;
    actwk[0].yposi.w.h = 640;
    actwk[0].sprvsize = 18;
    actwk[0].xspeed.w = 2560;
}

static void test_playsp_returns_when_timer_is_not_on_four_frame_boundary(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    gametimer.b.l = 1;
    scramapad_result = 190;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_playsp_returns_when_absolute_x_speed_is_too_low(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    actwk[0].xspeed.w = 2559;
    scramapad_result = 190;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_playsp7_state();
    setup_fast_player();
    actwk[0].xspeed.w = -2559;
    scramapad_result = 190;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_playsp_returns_when_block_is_not_in_current_time_table(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    time_flag = 1;
    scramapad_result = 155;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 320, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 648, scramapad_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_playsp_uses_generate_flag_for_time_two_table_selection(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    time_flag = 2;
    generate_flag = 1;
    scramapad_result = 517;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[9].actno);
    TEST_ASSERT_EQ_INT(ctx, 320, actwk[9].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 658, actwk[9].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[9].cddat);
}

static void test_playsp_spawns_particle_from_positive_speed_match(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    time_flag = 0;
    scramapad_result = 165;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[9].actno);
    TEST_ASSERT_EQ_INT(ctx, 320, actwk[9].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 658, actwk[9].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[9].cddat);
}

static void test_playsp_spawns_flipped_particle_from_negative_speed_match(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    actwk[0].xspeed.w = -2560;
    time_flag = 1;
    scramapad_result = 345;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[9].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[9].cddat);
}

static void test_playsp_keeps_state_when_actor_allocation_fails(
    test_context *ctx) {
    reset_playsp7_state();
    setup_fast_player();
    scramapad_result = 190;
    actwkchk_fail = 1;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[9].actno);
}

TEST_MAIN_BEGIN;
test_playsp_returns_when_timer_is_not_on_four_frame_boundary(&ctx);
test_playsp_returns_when_absolute_x_speed_is_too_low(&ctx);
test_playsp_returns_when_block_is_not_in_current_time_table(&ctx);
test_playsp_uses_generate_flag_for_time_two_table_selection(&ctx);
test_playsp_spawns_particle_from_positive_speed_match(&ctx);
test_playsp_spawns_flipped_particle_from_negative_speed_match(&ctx);
test_playsp_keeps_state_when_actor_allocation_fails(&ctx);
TEST_MAIN_END;

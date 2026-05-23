#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 prio_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r7/chgwall7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

static void reset_chgwall7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    prio_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
}

static void test_chgwall7_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_chgwall7[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_chgwall7[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 1, pat01.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, pat01.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 403, pat01.spra[0].index);
}

static void test_chgwall7_initializes_and_draws(test_context *ctx) {
    sprite_status *wall = &actwk[2];

    reset_chgwall7_state();
    wall->actflg = 128;
    wall->patno = 1;
    chgwall7(wall);

    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 132, wall->actflg);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 17, wall->sprvsize);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_chgwall7);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == wall);
}

static void test_chgwall7_priority_on_sets_latch_when_player_hits(
    test_context *ctx) {
    sprite_status *wall = &actwk[2];

    reset_chgwall7_state();
    wall->r_no0 = 2;
    prio_flag = 1;
    hitchk_result = 0;
    chgwall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);

    reset_chgwall7_state();
    wall->r_no0 = 2;
    prio_flag = 1;
    hitchk_result = 1;
    chgwall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, wall->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_chgwall7_priority_off_clears_previous_ride(test_context *ctx) {
    sprite_status *wall = &actwk[2];

    reset_chgwall7_state();
    wall->r_no0 = 2;
    prio_flag = 0;
    wall->actfree[0] = 0;
    chgwall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);

    reset_chgwall7_state();
    wall->r_no0 = 2;
    prio_flag = 0;
    wall->actfree[0] = 1;
    chgwall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == wall);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
}

TEST_MAIN_BEGIN;
test_chgwall7_patterns_capture_literal_data(&ctx);
test_chgwall7_initializes_and_draws(&ctx);
test_chgwall7_priority_on_sets_latch_when_player_hits(&ctx);
test_chgwall7_priority_off_clears_previous_ride(&ctx);
TEST_MAIN_END

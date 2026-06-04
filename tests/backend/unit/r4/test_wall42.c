#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 time_flag;
int_union scra_h_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/wall42.c"

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

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    time_flag = 0;
    scra_h_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
}

static int flag_index_for(sprite_status *actor) {
    return (int)((Uint16)actor->cdsts * 3 + (Uint16)time_flag);
}

static void assert_actionsub_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_wall42_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_wall42[0] == &pat00);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -64, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, pat00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 531, pat00.spra[0].index);
}

static void test_wall42_init_sets_flag_and_wait_state(test_context *ctx) {
    sprite_status *wall = &actwk[3];
    int index;

    reset_state();
    wall->cdsts = 5;
    wall->xposi.w.h = 300;
    wall->yposi.w.h = 100;
    time_flag = 2;
    index = flag_index_for(wall);

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[index] & 1);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 64, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17514, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_wall42);
    TEST_ASSERT_EQ_INT(ctx, 60, wall42_work_get(wall)->timer);
    assert_actionsub_only(ctx, wall);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_wall42_init_reuses_existing_flag_as_stopped_wall(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];
    int index;

    reset_state();
    wall->cdsts = 4;
    wall->xposi.w.h = 300;
    time_flag = 1;
    index = flag_index_for(wall);
    flagwork[index] = 1;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 492, wall->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 6, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[index]);
    assert_actionsub_only(ctx, wall);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_wall42_wait_counts_down_and_calls_hitchk(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_state();
    wall->r_no0 = 2;
    wall42_work_get(wall)->timer = 2;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, wall42_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    assert_actionsub_only(ctx, wall);
}

static void test_wall42_wait_minus_one_starts_move_timer(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_state();
    wall->r_no0 = 2;
    wall42_work_get(wall)->timer = -1;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 4, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 383, wall42_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_wall42_move_pushes_player_and_advances_wall(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_state();
    wall->r_no0 = 4;
    wall->xposi.l = 100 << 16;
    wall42_work_get(wall)->timer = 2;
    hitchk_result = 1;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, wall->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, wall->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 1, wall42_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->r_no0);
    assert_actionsub_only(ctx, wall);
}

static void test_wall42_move_timer_minus_one_enters_stop(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_state();
    wall->r_no0 = 4;
    wall42_work_get(wall)->timer = -1;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 6, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -2, wall42_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    assert_actionsub_only(ctx, wall);
}

static void test_wall42_stop_only_checks_ride(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_state();
    wall->r_no0 = 6;
    hitchk_result = 1;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    assert_actionsub_only(ctx, wall);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_wall42_offscreen_clears_initialized_flagwork_and_frames_out(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];
    int index;

    reset_state();
    wall->cdsts = 6;
    wall->xposi.w.h = 1024;
    time_flag = 2;
    index = flag_index_for(wall);
    scra_h_posit.w.h = 128;

    wall42(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 0, flagwork[index] & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwork[index] & 1);
    assert_actionsub_only(ctx, wall);
}

TEST_MAIN_BEGIN;
    test_wall42_patterns_capture_literal_data(&ctx);
    test_wall42_init_sets_flag_and_wait_state(&ctx);
    test_wall42_init_reuses_existing_flag_as_stopped_wall(&ctx);
    test_wall42_wait_counts_down_and_calls_hitchk(&ctx);
    test_wall42_wait_minus_one_starts_move_timer(&ctx);
    test_wall42_move_pushes_player_and_advances_wall(&ctx);
    test_wall42_move_timer_minus_one_enters_stop(&ctx);
    test_wall42_stop_only_checks_ride(&ctx);
    test_wall42_offscreen_clears_initialized_flagwork_and_frames_out(&ctx);
TEST_MAIN_END

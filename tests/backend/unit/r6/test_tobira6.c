#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 switchflag[32];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r6/tobira6.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return 0;
}

static void reset_tobira6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    memset(switchflag, 0, sizeof(switchflag));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
}

static void test_tobira6_init_captures_normal_stage_layout(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->userflag.b.h = 3;

    tobira6_init(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, door->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, door->sprpri);
    TEST_ASSERT_TRUE(ctx, door->patbase == tobira6pat);
    TEST_ASSERT_EQ_INT(ctx, 928, door->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 32, door->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 0, door->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, tobira6_get_work(door)->switch_index);
    TEST_ASSERT_EQ_INT(ctx, 0, tobira6_get_work(door)->open_amount);
    TEST_ASSERT_EQ_INT(ctx, 200, door->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == door);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
}

static void test_tobira6_init_uses_stage_two_wide_layout(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    stageno.b.l = 2;
    door->yposi.w.h = 300;
    door->userflag.b.h = 31;

    tobira6_init(door);

    TEST_ASSERT_EQ_INT(ctx, 816, door->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 32, door->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 1, door->patno);
    TEST_ASSERT_EQ_INT(ctx, 15, tobira6_get_work(door)->switch_index);
    TEST_ASSERT_EQ_INT(ctx, 300, door->yposi.w.h);
}

static void test_tobira6_move_opens_until_transition(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->yposi.w.h = 100;
    tobira6_init(door);
    tobira6_get_work(door)->open_amount = 60;
    tobira6_get_work(door)->close_flag = 0;
    door->r_no0 = 2;
    actwk[0].xposi.w.h = 140;
    actwk[0].yposi.w.h = 220;

    tobira6_move(door);

    TEST_ASSERT_EQ_INT(ctx, 64, tobira6_get_work(door)->open_amount);
    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 36, door->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
}

static void test_tobira6_move_respects_open_switch_flag(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->yposi.w.h = 100;
    door->userflag.b.h = 4;
    tobira6_init(door);
    tobira6_get_work(door)->close_flag = 255;
    switchflag[4] = 128;

    tobira6_move(door);

    TEST_ASSERT_EQ_INT(ctx, 0, tobira6_get_work(door)->close_flag);
    TEST_ASSERT_EQ_INT(ctx, 4, tobira6_get_work(door)->open_amount);
    TEST_ASSERT_EQ_INT(ctx, 96, door->yposi.w.h);
}

static void test_tobira6_check_waits_while_player_overlaps_edges(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->sprhsize = 8;
    actwk[0].sprhs = 8;
    actwk[0].xposi.w.h = 50;
    tobira6_move(door);
    door->r_no0 = 4;

    actwk[0].xposi.w.h = 80;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    actwk[0].xposi.w.h = 150;
    tobira6_move(door);
    door->r_no0 = 4;
    actwk[0].xposi.w.h = 120;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    actwk[0].xposi.w.h = 150;
    tobira6_move(door);
    door->r_no0 = 4;
    actwk[0].xposi.w.h = 88;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);

    actwk[0].xposi.w.h = 50;
    tobira6_move(door);
    door->r_no0 = 4;
    actwk[0].xposi.w.h = 110;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 4, door->r_no0);
}

static void test_tobira6_check_advances_after_player_clears_edges(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->sprhsize = 8;
    actwk[0].sprhs = 8;
    actwk[0].xposi.w.h = 50;
    tobira6_move(door);
    door->r_no0 = 4;

    actwk[0].xposi.w.h = 140;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);

    reset_tobira6_state();
    door->xposi.w.h = 100;
    door->yposi.w.h = 200;
    door->sprhsize = 8;
    actwk[0].sprhs = 8;
    actwk[0].xposi.w.h = 150;
    tobira6_move(door);
    door->r_no0 = 4;

    actwk[0].xposi.w.h = 80;
    tobira6_chek(door);

    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
}

static void test_tobira6_close_counts_down_and_returns_to_move(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->yposi.w.h = 100;
    tobira6_init(door);
    door->r_no0 = 6;
    tobira6_get_work(door)->open_amount = 8;

    tobira6_clse(door);

    TEST_ASSERT_EQ_INT(ctx, 4, tobira6_get_work(door)->open_amount);
    TEST_ASSERT_EQ_INT(ctx, 6, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 96, door->yposi.w.h);

    tobira6_clse(door);

    TEST_ASSERT_EQ_INT(ctx, 0, tobira6_get_work(door)->open_amount);
    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, door->yposi.w.h);
}

static void test_tobira6_entry_dispatches_and_runs_callbacks(test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_tobira6_state();
    door->yposi.w.h = 100;

    tobira6(door);

    TEST_ASSERT_EQ_INT(ctx, 2, door->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == door);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == door);
}

TEST_MAIN_BEGIN;
    test_tobira6_init_captures_normal_stage_layout(&ctx);
    test_tobira6_init_uses_stage_two_wide_layout(&ctx);
    test_tobira6_move_opens_until_transition(&ctx);
    test_tobira6_move_respects_open_switch_flag(&ctx);
    test_tobira6_check_waits_while_player_overlaps_edges(&ctx);
    test_tobira6_check_advances_after_player_clears_edges(&ctx);
    test_tobira6_close_counts_down_and_returns_to_move(&ctx);
    test_tobira6_entry_dispatches_and_runs_callbacks(&ctx);
TEST_MAIN_END

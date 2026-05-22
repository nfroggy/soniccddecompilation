#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Uint8 time_flag;
int_union scra_h_posit;
int_union scra_v_posit;
sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_platforms[8];
static sprite_status *ridechk_players[8];
static Sint16 ridechk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/block.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    if (ridechk_count < 8) {
        ridechk_platforms[ridechk_count] = pActwk;
        ridechk_players[ridechk_count] = pPlayerwk;
    }
    ++ridechk_count;
    return ridechk_result;
}

static void reset_block_state(sprite_status *actor) {
    memset(&time_flag, 0, sizeof(time_flag));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(actwk, 0, sizeof(actwk));
    memset(actor, 0, sizeof(*actor));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    memset(ridechk_platforms, 0, sizeof(ridechk_platforms));
    memset(ridechk_players, 0, sizeof(ridechk_players));
    ridechk_result = 0;
}

static void expect_ride_calls(test_context *ctx, sprite_status *platform,
                              int count) {
    TEST_ASSERT_EQ_INT(ctx, count, ridechk_count);
    for (int i = 0; i < count && i < 8; ++i) {
        TEST_ASSERT_TRUE(ctx, ridechk_platforms[i] == platform);
        TEST_ASSERT_TRUE(ctx, ridechk_players[i] == actwk);
    }
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, block_tbl[0] == block_init);
    TEST_ASSERT_TRUE(ctx, block_tbl[1] == block_move);
    TEST_ASSERT_TRUE(ctx, block_tbl[2] == block_move2);
    TEST_ASSERT_TRUE(ctx, block_tbl[3] == block_move3);
    TEST_ASSERT_TRUE(ctx, block_tbl[4] == block_move4);
    TEST_ASSERT_TRUE(ctx, block_tbl[5] == block_move5);
    TEST_ASSERT_TRUE(ctx, block_tbl[6] == block_move6);

    TEST_ASSERT_EQ_INT(ctx, 2, blockchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, blockchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 255, blockchg0[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, blockchg1[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, blockchg1[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, blockchg1[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, blockchg1[3]);
    TEST_ASSERT_EQ_INT(ctx, 5, blockchg1[4]);
    TEST_ASSERT_EQ_INT(ctx, 3, blockchg1[5]);
    TEST_ASSERT_EQ_INT(ctx, 5, blockchg1[6]);
    TEST_ASSERT_EQ_INT(ctx, 4, blockchg1[7]);
    TEST_ASSERT_EQ_INT(ctx, 5, blockchg1[8]);
    TEST_ASSERT_EQ_INT(ctx, 252, blockchg1[9]);
    TEST_ASSERT_EQ_INT(ctx, 2, blockchg2[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, blockchg2[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, blockchg2[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, blockchg2[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, blockchg2[4]);
    TEST_ASSERT_EQ_INT(ctx, 3, blockchg2[5]);
    TEST_ASSERT_EQ_INT(ctx, 0, blockchg2[6]);
    TEST_ASSERT_EQ_INT(ctx, 4, blockchg2[7]);
    TEST_ASSERT_EQ_INT(ctx, 0, blockchg2[8]);
    TEST_ASSERT_EQ_INT(ctx, 252, blockchg2[9]);
    TEST_ASSERT_TRUE(ctx, blockchg[0] == blockchg0);
    TEST_ASSERT_TRUE(ctx, blockchg[1] == blockchg1);
    TEST_ASSERT_TRUE(ctx, blockchg[2] == blockchg2);

    TEST_ASSERT_TRUE(ctx, blockpat[0] == &block0);
    TEST_ASSERT_TRUE(ctx, blockpat[5] == &block5);
    TEST_ASSERT_EQ_INT(ctx, 0, block0.cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, block1.cnt);
    TEST_ASSERT_EQ_INT(ctx, -12, block1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, block1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, block1.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, block1.spra[0].index + 4, block5.spra[0].index);
}

static void test_block_init_sets_shape_then_moves(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.actflg = 1;
    actor.patno = 9;
    actor.xposi.w.h = 128;

    block(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor.actflg);
    TEST_ASSERT_TRUE(ctx, actor.patbase == blockpat);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actor.patno);
    expect_ride_calls(ctx, &actor, 2);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_block_frames_out_after_dispatch_when_horizontally_far(
    test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 2;
    actor.xposi.w.h = 1024;
    scra_h_posit.w.h = 0;

    block(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

static void test_block_move_time_zero_calls_actionsub(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.patno = 7;
    actor.r_no0 = 2;

    block_move(&actor);

    TEST_ASSERT_EQ_INT(ctx, 7, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move_time_one_changes_pattern_without_ride(
    test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    time_flag = 1;
    actor.r_no0 = 2;
    actor.patno = 7;
    actor.mstno.b.h = 3;

    block_move(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_block_move_time_one_riding_enters_wait_state(
    test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    time_flag = 1;
    actor.r_no0 = 2;
    actor.patno = 7;
    actor.cddat = 8;

    block_move(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 6, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_block_move_time_two_without_ride_stays_put(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    time_flag = 2;
    actor.r_no0 = 2;
    actor.patno = 7;
    actor.yposi.w.h = 20;

    block_move(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 7, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 20, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move_time_two_riding_falls_by_reusing_move2(
    test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    time_flag = 2;
    actor.r_no0 = 2;
    actor.cddat = 8;
    actor.yposi.w.h = 20;
    scra_v_posit.w.h = 0;

    block_move(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 22, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    expect_ride_calls(ctx, &actor, 4);
}

static void test_block_move2_actions_or_frames_after_falling(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.yposi.w.h = 222;
    scra_v_posit.w.h = 0;

    block_move2(&actor);

    TEST_ASSERT_EQ_INT(ctx, 224, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    expect_ride_calls(ctx, &actor, 2);

    reset_block_state(&actor);
    actor.yposi.w.h = 223;
    scra_v_posit.w.h = 0;

    block_move2(&actor);

    TEST_ASSERT_EQ_INT(ctx, 225, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move3_resets_when_not_ridden(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 6;

    block_move3(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move3_animates_when_ridden(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 6;
    actor.cddat = 8;

    block_move3(&actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == blockchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_block_move4_advances_after_ride_ends(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 8;
    actor.mstno.b.h = 5;

    block_move4(&actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move4_animates_while_ridden(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 8;
    actor.mstno.b.h = 5;
    actor.cddat = 8;

    block_move4(&actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == blockchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_block_move5_always_animates(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);

    block_move5(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == &actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == blockchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    expect_ride_calls(ctx, &actor, 2);
}

static void test_block_move6_returns_to_normal_move(test_context *ctx) {
    sprite_status actor;
    reset_block_state(&actor);
    actor.r_no0 = 12;

    block_move6(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_block_ride_and_wrapper_call_ridechk(test_context *ctx) {
    sprite_status actor;
    sprite_status player;
    reset_block_state(&actor);

    block_ride(&actor);

    expect_ride_calls(ctx, &actor, 2);

    reset_block_state(&actor);
    memset(&player, 0, sizeof(player));

    ride_on_chk_b(&actor, &player);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_platforms[0] == &actor);
    TEST_ASSERT_TRUE(ctx, ridechk_players[0] == &player);
}

TEST_MAIN_BEGIN;
    test_tables_capture_literal_data(&ctx);
    test_block_init_sets_shape_then_moves(&ctx);
    test_block_frames_out_after_dispatch_when_horizontally_far(&ctx);
    test_block_move_time_zero_calls_actionsub(&ctx);
    test_block_move_time_one_changes_pattern_without_ride(&ctx);
    test_block_move_time_one_riding_enters_wait_state(&ctx);
    test_block_move_time_two_without_ride_stays_put(&ctx);
    test_block_move_time_two_riding_falls_by_reusing_move2(&ctx);
    test_block_move2_actions_or_frames_after_falling(&ctx);
    test_block_move3_resets_when_not_ridden(&ctx);
    test_block_move3_animates_when_ridden(&ctx);
    test_block_move4_advances_after_ride_ends(&ctx);
    test_block_move4_animates_while_ridden(&ctx);
    test_block_move5_always_animates(&ctx);
    test_block_move6_returns_to_normal_move(&ctx);
    test_block_ride_and_wrapper_call_ridechk(&ctx);
TEST_MAIN_END

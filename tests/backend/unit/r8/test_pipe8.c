#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 switchflag[32];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r8/pipe8.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_pipe8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    reset_logs();
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void assert_action_and_frameout(test_context *ctx,
                                       sprite_status *actor,
                                       Sint16 expected_xpos) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, expected_xpos, frameout_s00_xpos);
}

static void assert_action_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_pipe8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg[1] == p01);
    TEST_ASSERT_EQ_INT(ctx, 0, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, p00[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, p00[3]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[4]);
    TEST_ASSERT_EQ_INT(ctx, 0, p01[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, p01[1]);
    TEST_ASSERT_EQ_INT(ctx, -1, p01[5]);
    TEST_ASSERT_TRUE(ctx, pat_pipe8[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_pipe8[5] == &pat05);
    TEST_ASSERT_EQ_INT(ctx, 477, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -28, pat04.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 481, pat04.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, pat05.spra[0].index);
}

static void test_pipe8_wait_initializes_and_stays_hidden_when_player_far(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_pipe8_state();
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 2000;

    pipe8(actor);

    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_pipe8_wait_advances_when_player_is_near(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_pipe8_state();
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 1000;

    pipe8(actor);

    assert_action_and_frameout(ctx, actor, 1000);
}

static void test_pipe8_init_frames_out_when_first_allocation_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_pipe8_state();
    actor->r_no0 = 4;
    actor->xposi.w.h = 1000;

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    assert_action_and_frameout(ctx, actor, 0);
}

static void test_pipe8_init_frames_out_when_second_allocation_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *paired = &actwk[20];

    reset_pipe8_state();
    actor->actno = 32;
    actor->r_no0 = 4;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    queue_actwkchk(paired);

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    assert_action_and_frameout(ctx, actor, 0);
}

static void test_pipe8_init_spawns_pair_and_shatter_piece_then_rides(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *paired = &actwk[20];
    sprite_status *shatter_piece = &actwk[21];

    reset_pipe8_state();
    actor->actno = 32;
    actor->r_no0 = 4;
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = 5;
    queue_actwkchk(paired);
    queue_actwkchk(shatter_piece);

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    assert_action_and_frameout(ctx, actor, 0);
}

static void test_pipe8_move_only_checks_ride(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_pipe8_state();
    actor->r_no0 = 6;
    actor->xposi.w.h = 1000;

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    assert_action_and_frameout(ctx, actor, 0);
}

static void test_pipe8_negative_pair_frames_out_when_parent_missing(
    test_context *ctx) {
    sprite_status *actor = &actwk[20];

    reset_pipe8_state();
    actor->userflag.b.h = -1;
    set_actfree_word(actor, 20, 3);

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_pipe8_negative_pair_with_parent_runs_without_frameout_s00(
    test_context *ctx) {
    sprite_status *actor = &actwk[20];

    reset_pipe8_state();
    actwk[3].actno = 32;
    actor->userflag.b.h = -1;
    actor->r_no0 = 6;
    set_actfree_word(actor, 20, 3);

    pipe8(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_only(ctx, actor);
}

static void test_pipe8_shatter_frames_out_when_parent_missing(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    piece->userflag.b.l = -1;
    set_actfree_word(piece, 20, 3);

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == piece);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_pipe8_shatter_initializes_closed_gate(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    set_actfree_word(piece, 20, 3);

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_shatter_closed_clears_switch_flag(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->userflag.b.h = 5;
    piece->r_no0 = 2;
    set_actfree_word(piece, 20, 3);
    switchflag[5] = 255;

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 191, switchflag[5]);
    assert_action_only(ctx, piece);
}

static void test_pipe8_shatter_closed1_opens_when_switch_is_set(
    test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->userflag.b.h = 5;
    piece->r_no0 = 4;
    set_actfree_word(piece, 20, 3);
    switchflag[5] = 128;

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_only(ctx, piece);
}

static void test_pipe8_shatter_closed1_ignores_player_not_moving_up(
    test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->userflag.b.h = 5;
    piece->r_no0 = 4;
    set_actfree_word(piece, 20, 3);
    actwk[0].yspeed.w = 0;

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    assert_action_only(ctx, piece);
}

static void test_pipe8_shatter_closed1_opens_b_when_player_hits_from_below(
    test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 4;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    set_actfree_word(piece, 20, 3);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 205;
    actwk[0].yspeed.w = -1;

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_open_a_animates_until_timer_expires(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 6;
    set_actfree_word(piece, 20, 3);
    set_actfree_word(piece, 0, 1);

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == piece);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    assert_action_only(ctx, piece);

    reset_logs();
    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_action_only(ctx, piece);
}

static void test_pipe8_opend_a_stays_open_while_player_inside(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 8;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    set_actfree_word(piece, 20, 3);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 195;

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_opend_a_closes_when_player_outside(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 8;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    set_actfree_word(piece, 20, 3);
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 195;

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_opend_b_starts_close_when_player_hits_gate(
    test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 10;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    set_actfree_word(piece, 20, 3);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 195;
    actwk[0].yspeed.w = -1;

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_opend_b_ignores_miss(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 10;
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    set_actfree_word(piece, 20, 3);
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;
    actwk[0].yspeed.w = -1;

    pipe8(piece);

    assert_action_only(ctx, piece);
}

static void test_pipe8_close_b_animates_then_returns_closed(test_context *ctx) {
    sprite_status *piece = &actwk[21];

    reset_pipe8_state();
    actwk[3].actno = 32;
    piece->userflag.b.l = -1;
    piece->r_no0 = 12;
    piece->patno = 5;
    piece->mstno.b.h = 1;
    set_actfree_word(piece, 20, 3);
    set_actfree_word(piece, 0, 1);

    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    assert_action_only(ctx, piece);

    reset_logs();
    pipe8(piece);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_action_only(ctx, piece);
}

static void test_pipe8_check_helpers_cover_bounds(test_context *ctx) {
    sprite_status *piece = &actwk[21];
    sprite_status *player = &actwk[0];

    reset_pipe8_state();
    piece->xposi.w.h = 100;
    piece->yposi.w.h = 200;
    player->xposi.w.h = 100;
    player->yposi.w.h = 195;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       act1check1(piece, player, -8, 16, -120, 116));
    player->yposi.w.h = 400;
    TEST_ASSERT_EQ_INT(ctx, 0,
                       act1check1(piece, player, -8, 16, -120, 116));
    player->yposi.w.h = 195;
    player->xposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 0,
                       act1check1(piece, player, -8, 16, -120, 116));
    player->xposi.w.h = 100;
    player->yposi.w.h = 195;
    player->yspeed.w = 0;
    TEST_ASSERT_EQ_INT(ctx, 0,
                       act1check2(piece, player, -8, 16, -120, 116));
    player->yspeed.w = -1;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       act1check2(piece, player, -8, 16, -120, 116));
}

TEST_MAIN_BEGIN;
    test_pipe8_patterns_capture_literal_data(&ctx);
    test_pipe8_wait_initializes_and_stays_hidden_when_player_far(&ctx);
    test_pipe8_wait_advances_when_player_is_near(&ctx);
    test_pipe8_init_frames_out_when_first_allocation_fails(&ctx);
    test_pipe8_init_frames_out_when_second_allocation_fails(&ctx);
    test_pipe8_init_spawns_pair_and_shatter_piece_then_rides(&ctx);
    test_pipe8_move_only_checks_ride(&ctx);
    test_pipe8_negative_pair_frames_out_when_parent_missing(&ctx);
    test_pipe8_negative_pair_with_parent_runs_without_frameout_s00(&ctx);
    test_pipe8_shatter_frames_out_when_parent_missing(&ctx);
    test_pipe8_shatter_initializes_closed_gate(&ctx);
    test_pipe8_shatter_closed_clears_switch_flag(&ctx);
    test_pipe8_shatter_closed1_opens_when_switch_is_set(&ctx);
    test_pipe8_shatter_closed1_ignores_player_not_moving_up(&ctx);
    test_pipe8_shatter_closed1_opens_b_when_player_hits_from_below(&ctx);
    test_pipe8_open_a_animates_until_timer_expires(&ctx);
    test_pipe8_opend_a_stays_open_while_player_inside(&ctx);
    test_pipe8_opend_a_closes_when_player_outside(&ctx);
    test_pipe8_opend_b_starts_close_when_player_hits_gate(&ctx);
    test_pipe8_opend_b_ignores_miss(&ctx);
    test_pipe8_close_b_animates_then_returns_closed(&ctx);
    test_pipe8_check_helpers_cover_bounds(&ctx);
TEST_MAIN_END

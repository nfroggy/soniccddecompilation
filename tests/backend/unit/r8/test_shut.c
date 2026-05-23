#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossstart;
Uint8 prio_flag;
Uint8 shut_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actwkchk_count;
static sprite_status *actwkchk_new_actor;
static Sint32 actwkchk_result;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int block_wrt_count;
static Uint16 block_wrt_block[16];
static Uint16 block_wrt_x[16];
static Uint16 block_wrt_y[16];
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int sinset_count;
static Uint8 sinset_angles[8];
static Sint16 sinset_sin_result;
static Sint16 sinset_cos_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s0(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r8/shut.c"

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

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_result != 0) {
        return actwkchk_result;
    }
    *ppActwk = actwkchk_new_actor;
    return 0;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs) {
    if (block_wrt_count < (int)(sizeof(block_wrt_block) / sizeof(block_wrt_block[0]))) {
        block_wrt_block[block_wrt_count] = BlockNo;
        block_wrt_x[block_wrt_count] = xOffs;
        block_wrt_y[block_wrt_count] = yOffs;
    }
    ++block_wrt_count;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < (int)(sizeof(sinset_angles) / sizeof(sinset_angles[0]))) {
        sinset_angles[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = sinset_sin_result;
    *cos = sinset_cos_result;
}

static void queue_actwk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void reset_shut_state(void) {
    memset(actwk, 0, sizeof(actwk));
    bossstart = 0;
    prio_flag = 0;
    shut_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actwkchk_count = 0;
    actwkchk_new_actor = &actwk[20];
    actwkchk_result = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    block_wrt_count = 0;
    memset(block_wrt_block, 0, sizeof(block_wrt_block));
    memset(block_wrt_x, 0, sizeof(block_wrt_x));
    memset(block_wrt_y, 0, sizeof(block_wrt_y));
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    sinset_count = 0;
    memset(sinset_angles, 0, sizeof(sinset_angles));
    sinset_sin_result = 0;
    sinset_cos_result = 0;
}

static void assert_action_and_simple_frameout(test_context *ctx,
                                             sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_shut_tables_capture_render_patterns(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, shutpat[0] == &shu00);
    TEST_ASSERT_TRUE(ctx, kaitenpat[0] == &kai00);
    TEST_ASSERT_TRUE(ctx, kaitenpat[2] == &kai02);
    TEST_ASSERT_EQ_INT(ctx, 6, shu00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, shu00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -96, shu00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 308, shu00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3, kai00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -40, kai00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 304, kai00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 5, kai02.cnt);
    TEST_ASSERT_EQ_INT(ctx, 307, kai02.spra[4].index);
}

static void test_shut_init_spawns_partner_and_runs_outer_callbacks(
    test_context *ctx) {
    sprite_status *door = &actwk[2];
    sprite_status *partner = &actwk[21];

    reset_shut_state();
    actwkchk_new_actor = partner;
    door->xposi.w.h = 160;
    door->yposi.w.h = 64;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_init_preserves_partner_high_userflag_path(
    test_context *ctx) {
    sprite_status *door = &actwk[2];
    sprite_status *partner = &actwk[21];

    reset_shut_state();
    actwkchk_new_actor = partner;
    partner->userflag.b.h = 1;
    door->xposi.w.h = 160;
    door->yposi.w.h = 64;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_init_frames_out_when_partner_allocation_fails(
    test_context *ctx) {
    sprite_status *door = &actwk[3];

    reset_shut_state();
    actwkchk_result = -1;
    door->xposi.w.h = 160;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == door);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_wait_holds_while_bossstart_is_nonnegative(
    test_context *ctx) {
    sprite_status *door = &actwk[4];

    reset_shut_state();
    door->r_no0 = 2;
    bossstart = 0;
    shut_flag = 9;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 9, shut_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_wait_releases_when_bossstart_is_negative(
    test_context *ctx) {
    sprite_status *door = &actwk[5];

    reset_shut_state();
    door->r_no0 = 2;
    bossstart = 128;
    shut_flag = 9;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 0, shut_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_move_writes_twelve_blocks_on_tile_boundary(
    test_context *ctx) {
    sprite_status *door = &actwk[6];
    int i;

    reset_shut_state();
    door->r_no0 = 4;
    door->xposi.w.h = 104;
    door->yposi.w.h = 200;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 12, block_wrt_count);
    for (i = 0; i < 12; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_block[i]);
        TEST_ASSERT_EQ_INT(ctx, 96, block_wrt_x[i]);
        TEST_ASSERT_EQ_INT(ctx, 104 + i * 16, block_wrt_y[i]);
    }
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_move_frames_out_when_all_blocks_are_cleared(
    test_context *ctx) {
    sprite_status *door = &actwk[7];

    reset_shut_state();
    door->r_no0 = 4;
    door->xposi.w.h = 104;
    door->actfree[2] = 16;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == door);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_shut_move_userflag_branch_runs_between_tile_boundaries(
    test_context *ctx) {
    sprite_status *door = &actwk[7];

    reset_shut_state();
    door->r_no0 = 4;
    door->userflag.b.h = 1;
    door->xposi.w.h = 105;

    shut(door);

    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s0_count);
    assert_action_and_simple_frameout(ctx, door);
}

static void test_kaiten_init_allocation_failure_frames_out_without_outer_draw(
    test_context *ctx) {
    sprite_status *loop = &actwk[8];

    reset_shut_state();

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_kaiten_init_spawns_children_then_draws(test_context *ctx) {
    sprite_status *loop = &actwk[9];

    reset_shut_state();
    queue_actwk2(&actwk[30]);
    queue_actwk2(&actwk[31]);
    queue_actwk2(&actwk[32]);
    queue_actwk2(&actwk[33]);
    queue_actwk2(&actwk[34]);
    queue_actwk2(&actwk[35]);
    queue_actwk2(&actwk[36]);
    queue_actwk2(&actwk[37]);
    loop->xposi.w.h = 256;
    loop->yposi.w.h = 300;
    sinset_cos_result = 256;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 3, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 234, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 149, sinset_angles[1]);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_angles[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_kaiten_move0_observes_ridecheck_miss(test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 96;
    loop->yposi.w.h = 256;
    actwk[0].xposi.w.h = 96;
    actwk[0].yposi.w.h = 512;
    ridechk_result = 0;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == loop);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_priority_block_runs_for_back_layer(test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 96;
    loop->yposi.w.h = 128;
    actwk[0].yposi.w.h = 96;
    actwk[6].actno = 1;
    actwk[8].actno = 1;
    actwk[9].actno = 1;
    actwk[10].actno = 1;
    actwk[11].actno = 1;
    prio_flag = 0;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_priority_block_runs_for_front_layer(test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 96;
    loop->yposi.w.h = 128;
    actwk[0].yposi.w.h = 96;
    actwk[6].actno = 1;
    actwk[8].actno = 1;
    actwk[9].actno = 1;
    actwk[10].actno = 1;
    actwk[11].actno = 1;
    prio_flag = 1;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move0_ride_hit_outside_center_window_returns(
    test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 160;
    loop->yposi.w.h = 256;
    actwk[0].xposi.w.h = 96;
    actwk[0].yposi.w.h = 512;
    ridechk_result = 1;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move0_ride_hit_latched_returns(test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 96;
    loop->yposi.w.h = 256;
    loop->actfree[2] = 1;
    actwk[0].xposi.w.h = 96;
    actwk[0].yposi.w.h = 512;
    ridechk_result = 1;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move0_ride_hit_advances_to_spin_state(
    test_context *ctx) {
    sprite_status *loop = &actwk[10];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->xposi.w.h = 96;
    loop->yposi.w.h = 256;
    actwk[0].xposi.w.h = 96;
    actwk[0].yposi.w.h = 512;
    ridechk_result = 1;
    prio_flag = 1;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_userflag_branch_animates_without_ridecheck(
    test_context *ctx) {
    sprite_status *loop = &actwk[11];

    reset_shut_state();
    loop->r_no0 = 2;
    loop->userflag.b.h = 1;
    actwk[0].mstno.b.h = 51;
    actwk[0].yposi.w.h = 512;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 0, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == loop);
    TEST_ASSERT_TRUE(ctx, patchg_table == kaitenchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move1_keeps_spinning_until_player_finishes(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 4;
    actwk[0].mstno.b.h = 51;
    actwk[0].yposi.w.h = 512;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move1_finishes_spin_and_toggles_priority(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 4;
    actwk[0].mstno.b.h = 5;
    actwk[0].yposi.w.h = 512;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, prio_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_move2_clears_priority_when_already_front(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 6;
    actwk[0].yposi.w.h = 512;
    prio_flag = 1;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, prio_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kaiten_bou0_waits_until_player_is_spinning(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 8;
    actwk[0].mstno.b.h = 50;
    actwk[0].yposi.w.h = 512;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == loop);
}

static void test_kaiten_bou0_advances_when_player_is_spinning(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 8;
    loop->actfree[3] = 20;
    actwk[0].mstno.b.h = 51;
    actwk[0].yposi.w.h = 512;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == loop);
}

static void test_kaiten_bou1_restores_saved_angle_when_player_stops(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 10;
    loop->actfree[4] = 88;
    actwk[0].mstno.b.h = 50;
    actwk[0].yposi.w.h = 512;
    sinset_cos_result = 128;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 88, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == loop);
}

static void test_kaiten_bou1_uses_angle_with_stubbed_sine_table(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 10;
    loop->actfree[3] = 20;
    actwk[0].mstno.b.h = 51;
    actwk[0].cddat = 0;
    prio_flag = 0;
    sinset_cos_result = 128;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 24, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == loop);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == loop);
}

static void test_kaiten_bou1_subtracts_angle_for_front_clockwise(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 10;
    loop->actfree[3] = 20;
    actwk[0].mstno.b.h = 51;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = 512;
    prio_flag = 1;
    sinset_cos_result = 128;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 16, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_kaiten_bou1_subtracts_angle_for_back_counterclockwise(
    test_context *ctx) {
    sprite_status *loop = &actwk[12];

    reset_shut_state();
    loop->r_no0 = 10;
    loop->actfree[3] = 20;
    actwk[0].mstno.b.h = 51;
    actwk[0].cddat = 1;
    actwk[0].yposi.w.h = 512;
    prio_flag = 0;
    sinset_cos_result = 128;

    kaiten(loop);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 16, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

TEST_MAIN_BEGIN;
    test_shut_tables_capture_render_patterns(&ctx);
    test_shut_init_spawns_partner_and_runs_outer_callbacks(&ctx);
    test_shut_init_preserves_partner_high_userflag_path(&ctx);
    test_shut_init_frames_out_when_partner_allocation_fails(&ctx);
    test_shut_wait_holds_while_bossstart_is_nonnegative(&ctx);
    test_shut_wait_releases_when_bossstart_is_negative(&ctx);
    test_shut_move_writes_twelve_blocks_on_tile_boundary(&ctx);
    test_shut_move_frames_out_when_all_blocks_are_cleared(&ctx);
    test_shut_move_userflag_branch_runs_between_tile_boundaries(&ctx);
    test_kaiten_init_allocation_failure_frames_out_without_outer_draw(&ctx);
    test_kaiten_init_spawns_children_then_draws(&ctx);
    test_kaiten_move0_observes_ridecheck_miss(&ctx);
    test_kaiten_priority_block_runs_for_back_layer(&ctx);
    test_kaiten_priority_block_runs_for_front_layer(&ctx);
    test_kaiten_move0_ride_hit_outside_center_window_returns(&ctx);
    test_kaiten_move0_ride_hit_latched_returns(&ctx);
    test_kaiten_move0_ride_hit_advances_to_spin_state(&ctx);
    test_kaiten_userflag_branch_animates_without_ridecheck(&ctx);
    test_kaiten_move1_keeps_spinning_until_player_finishes(&ctx);
    test_kaiten_move1_finishes_spin_and_toggles_priority(&ctx);
    test_kaiten_move2_clears_priority_when_already_front(&ctx);
    test_kaiten_bou0_waits_until_player_is_spinning(&ctx);
    test_kaiten_bou0_advances_when_player_is_spinning(&ctx);
    test_kaiten_bou1_restores_saved_angle_when_player_stops(&ctx);
    test_kaiten_bou1_uses_angle_with_stubbed_sine_table(&ctx);
    test_kaiten_bou1_subtracts_angle_for_front_clockwise(&ctx);
    test_kaiten_bou1_subtracts_angle_for_back_counterclockwise(&ctx);
TEST_MAIN_END

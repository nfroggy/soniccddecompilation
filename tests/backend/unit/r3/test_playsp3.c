#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
Uint8 generate_flag;
short_union col_y;

static int scramapad_count;
static Sint16 scramapad_x[16];
static Sint16 scramapad_y[16];
static Sint16 scramapad_results[16];
static int atan_count;
static Sint16 atan_results[16];
static int sinset_count;
static Uint8 sinset_angles[16];
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int actwkchk_count;
static int actwkchk_fail;
static sprite_status *actwkchk_actor;
static int block_wrt_count;
static Uint16 block_wrt_block[8];
static Uint16 block_wrt_x[8];
static Uint16 block_wrt_y[8];
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int soundset_count;
static Sint16 soundset_requests[8];

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 atan_sonic(Sint16 x, Sint16 y);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint32 actwkchk(sprite_status **ppActwk);
void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs);
void sub_sync(Sint16 ReqNo);
void soundset(Sint16 ReqNo);

#include "src/r3/playsp3.c"

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    (void)pActwk;
    if (scramapad_count < 16) {
        scramapad_x[scramapad_count] = iXposi;
        scramapad_y[scramapad_count] = iYposi;
    }
    return scramapad_results[scramapad_count++];
}

Sint16 atan_sonic(Sint16 x, Sint16 y) {
    (void)x;
    (void)y;
    return atan_results[atan_count++];
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < 16) {
        sinset_angles[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = sinset_sin;
    *cos = sinset_cos;
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

void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs) {
    if (block_wrt_count < 8) {
        block_wrt_block[block_wrt_count] = BlockNo;
        block_wrt_x[block_wrt_count] = xOffs;
        block_wrt_y[block_wrt_count] = yOffs;
    }
    ++block_wrt_count;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_playsp3_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    generate_flag = 0;
    memset(&col_y, 0, sizeof(col_y));
    scramapad_count = 0;
    memset(scramapad_x, 0, sizeof(scramapad_x));
    memset(scramapad_y, 0, sizeof(scramapad_y));
    memset(scramapad_results, 0, sizeof(scramapad_results));
    atan_count = 0;
    memset(atan_results, 0, sizeof(atan_results));
    sinset_count = 0;
    memset(sinset_angles, 0, sizeof(sinset_angles));
    sinset_sin = 0;
    sinset_cos = 256;
    actwkchk_count = 0;
    actwkchk_fail = 0;
    actwkchk_actor = &actwk[7];
    block_wrt_count = 0;
    memset(block_wrt_block, 0, sizeof(block_wrt_block));
    memset(block_wrt_x, 0, sizeof(block_wrt_x));
    memset(block_wrt_y, 0, sizeof(block_wrt_y));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    bump_sound_off = 0;
}

static void setup_player_for_probe(void) {
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 16;
}

static void test_playsp_skips_boyo_for_locked_player_state(test_context *ctx) {
    reset_playsp3_state();
    actwk[0].mstno.b.h = 43;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, atan_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
}

static void test_playsp_invokes_boyo_for_normal_player_state(test_context *ctx) {
    reset_playsp3_state();
    setup_player_for_probe();
    actwk[0].mstno.b.h = 0;
    atan_results[0] = 32;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, atan_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 92, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 216, scramapad_y[0]);
}

static void assert_boyo_probe_pair(test_context *ctx, Sint16 atan_value,
                                   Sint16 first_x, Sint16 first_y,
                                   Sint16 second_x, Sint16 second_y) {
    reset_playsp3_state();
    setup_player_for_probe();
    atan_results[0] = atan_value;

    boyo();

    TEST_ASSERT_EQ_INT(ctx, 1, atan_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, first_x, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, first_y, scramapad_y[0]);
    TEST_ASSERT_EQ_INT(ctx, second_x, scramapad_x[1]);
    TEST_ASSERT_EQ_INT(ctx, second_y, scramapad_y[1]);
}

static void test_boyo_checks_speed_direction_probe_pairs(test_context *ctx) {
    assert_boyo_probe_pair(ctx, 32, 92, 216, 108, 216);
    assert_boyo_probe_pair(ctx, 160, 92, 184, 108, 184);
    assert_boyo_probe_pair(ctx, 224, 108, 190, 108, 216);
    assert_boyo_probe_pair(ctx, 96, 92, 190, 92, 216);
}

static void test_boyo_stops_after_first_successful_probe(test_context *ctx) {
    reset_playsp3_state();
    setup_player_for_probe();
    atan_results[0] = 32;
    scramapad_results[0] = 325;

    boyo();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 92, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 216, scramapad_y[0]);
}

static void test_bycol_sub0_rejects_empty_or_unlisted_blocks(test_context *ctx) {
    reset_playsp3_state();
    scramapad_results[0] = 0;

    TEST_ASSERT_EQ_INT(ctx, 0, bycol_sub0(12, 34));
    TEST_ASSERT_EQ_INT(ctx, 1, bump_sound_off);

    reset_playsp3_state();
    scramapad_results[0] = 500;

    TEST_ASSERT_EQ_INT(ctx, 0, bycol_sub0(12, 34));
    TEST_ASSERT_EQ_INT(ctx, 1, bump_sound_off);
}

static void test_bycol_sub0_dispatches_bounce_and_resets_run_state(
    test_context *ctx) {
    reset_playsp3_state();
    actwk[0].xposi.w.h = 40;
    actwk[0].yposi.w.h = 80;
    actwk[0].r_no0 = 4;
    time_flag = 2;
    generate_flag = 1;
    scramapad_results[0] = 316;
    atan_results[0] = 12;
    sinset_sin = 0;
    sinset_cos = 256;

    TEST_ASSERT_EQ_INT(ctx, 1, bycol_sub0(33, 65));

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[7].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 146, sub_sync_requests[0]);
}

static void test_bycol_sub0_uses_bump_sound_selection(test_context *ctx) {
    reset_playsp3_state();
    time_flag = 1;
    bump_sound_off = 1;
    scramapad_results[0] = 411;
    sinset_sin = 0;
    sinset_cos = 256;

    TEST_ASSERT_EQ_INT(ctx, 1, bycol_sub0(32, 64));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 190, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, bump_sound_off);

    reset_playsp3_state();
    time_flag = 1;
    bump_sound_off = 1;
    scramapad_results[0] = 131;

    TEST_ASSERT_EQ_INT(ctx, 1, bycol_sub0(32, 64));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 181, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, bump_sound_off);
}

static void test_bycol_sub0_returns_zero_when_response_sets_collision_y(
    test_context *ctx) {
    reset_playsp3_state();
    time_flag = 1;
    scramapad_results[0] = 118;

    TEST_ASSERT_EQ_INT(ctx, 0, bycol_sub0(32, 64));
    TEST_ASSERT_EQ_INT(ctx, 1, col_y.b.h);
}

static void test_bob_records_break_blocks_and_optional_actor_spawn(
    test_context *ctx) {
    reset_playsp3_state();
    actwk[0].xposi.w.h = 40;
    actwk[0].yposi.w.h = 80;
    atan_results[0] = 44;
    sinset_sin = 128;
    sinset_cos = -128;

    bob(0, 33, 65, 0);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 44, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -896, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 41, actwk[7].actno);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[7].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, actwk[7].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, block_wrt_count);
    TEST_ASSERT_EQ_INT(ctx, 0, block_wrt_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 48, block_wrt_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 80, block_wrt_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, block_wrt_x[1]);
    TEST_ASSERT_EQ_INT(ctx, 80, block_wrt_y[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, block_wrt_x[2]);
    TEST_ASSERT_EQ_INT(ctx, 64, block_wrt_y[2]);
    TEST_ASSERT_EQ_INT(ctx, 48, block_wrt_x[3]);
    TEST_ASSERT_EQ_INT(ctx, 64, block_wrt_y[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 146, sub_sync_requests[0]);

    reset_playsp3_state();
    actwkchk_fail = 1;
    bob(1, 33, 65, 2048);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[7].actno);
}

static void test_direct_bounce_helpers_set_expected_speed_axes(test_context *ctx) {
    reset_playsp3_state();
    actwk[0].cddat = 48;
    sinset_sin = 256;
    sinset_cos = -256;
    bob_s(2, 35, 67, 0);
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -896, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);

    reset_playsp3_state();
    sinset_sin = -256;
    sinset_cos = 128;
    bob_s0(15, 35, 67, 0);
    TEST_ASSERT_EQ_INT(ctx, -896, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    actwk[0].yspeed.w = 0;
    bob0(5, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);
    actwk[0].yspeed.w = -1;
    bob0(5, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    actwk[0].xspeed.w = 0;
    bob1(6, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);
    actwk[0].xspeed.w = -1;
    bob1(6, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1792, actwk[0].xspeed.w);
}

static void test_bob2_selects_axis_by_x_nibble_and_block_flip(test_context *ctx) {
    reset_playsp3_state();
    bob2(12, 4, 0, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    bob2(12, 4, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob2(12, 12, 0, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob2(12, 12, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);
}

static void test_bob3_collision_mask_and_reflection_paths(test_context *ctx) {
    reset_playsp3_state();
    bob3(9, 0, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, col_y.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);

    reset_playsp3_state();
    atan_results[0] = 10;
    sinset_sin = 0;
    sinset_cos = 256;
    bob3(9, 1, 8, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 86, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    atan_results[0] = 20;
    bob3(8, 1, 3, 2048);
    TEST_ASSERT_EQ_INT(ctx, 140, sinset_angles[0]);
}

static void test_bob4_bob5_and_bob6_cover_selector_combinations(
    test_context *ctx) {
    reset_playsp3_state();
    bob4(8, 4, 3, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    sinset_cos = 256;
    bob4(8, 4, 3, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob4(8, 12, 3, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    bob4(8, 12, 3, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob5(4, 4, 0, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    sinset_cos = 256;
    bob5(4, 4, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    sinset_cos = 256;
    bob5(4, 12, 0, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob5(4, 12, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].yspeed.w);

    reset_playsp3_state();
    bob6(4, 0, 4, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    sinset_cos = 256;
    bob6(4, 0, 4, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    sinset_cos = 256;
    bob6(4, 0, 12, 2048);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);

    reset_playsp3_state();
    bob6(4, 0, 12, 0);
    TEST_ASSERT_EQ_INT(ctx, -1792, actwk[0].xspeed.w);
}

TEST_MAIN_BEGIN;
test_playsp_skips_boyo_for_locked_player_state(&ctx);
test_playsp_invokes_boyo_for_normal_player_state(&ctx);
test_boyo_checks_speed_direction_probe_pairs(&ctx);
test_boyo_stops_after_first_successful_probe(&ctx);
test_bycol_sub0_rejects_empty_or_unlisted_blocks(&ctx);
test_bycol_sub0_dispatches_bounce_and_resets_run_state(&ctx);
test_bycol_sub0_uses_bump_sound_selection(&ctx);
test_bycol_sub0_returns_zero_when_response_sets_collision_y(&ctx);
test_bob_records_break_blocks_and_optional_actor_spawn(&ctx);
test_direct_bounce_helpers_set_expected_speed_axes(&ctx);
test_bob2_selects_axis_by_x_nibble_and_block_flip(&ctx);
test_bob3_collision_mask_and_reflection_paths(&ctx);
test_bob4_bob5_and_bob6_cover_selector_combinations(&ctx);
TEST_MAIN_END;

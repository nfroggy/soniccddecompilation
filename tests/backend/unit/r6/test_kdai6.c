#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union gametimer;
ushort_union swdata;
ushort_union swdata1;
ushort_union swdata2;
Uint8 chibi_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r6/kdai6.c"

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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_kdai6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&gametimer, 0, sizeof(gametimer));
    swdata.w = 0;
    swdata1.w = 0;
    swdata2.w = 0;
    chibi_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 64;
    sinset_cos = 256;
}

static void reset_kdai6_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    sinset_count = 0;
    sinset_angle = 0;
}

static void init_platform(sprite_status *platform, Sint16 x, Sint16 y) {
    platform->xposi.w.h = x;
    platform->yposi.w.h = y;
    kdai6_init(platform);
}

static void assert_entry_callbacks(test_context *ctx, sprite_status *platform,
                                   Sint16 origin_x) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == platform);
    TEST_ASSERT_EQ_INT(ctx, origin_x, frameout_s00_x);
}

static void test_kdai6_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, kdai6_tbl[0] == kdai6_init);
    TEST_ASSERT_TRUE(ctx, kdai6_tbl[1] == kdai6_move);
    TEST_ASSERT_TRUE(ctx, move_tbl[0] == kd_mv_right);
    TEST_ASSERT_TRUE(ctx, move_tbl[1] == kd_mv_left);
    TEST_ASSERT_TRUE(ctx, move_tbl[2] == kd_mv_up);
    TEST_ASSERT_TRUE(ctx, move_tbl[3] == kd_mv_down);
    TEST_ASSERT_TRUE(ctx, kdai6pchg[0] == kdpat0);
    TEST_ASSERT_EQ_INT(ctx, 9, kdpat0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, kdpat0[7]);
    TEST_ASSERT_TRUE(ctx, kdai6pat[0] == &kdai6pat0);
    TEST_ASSERT_TRUE(ctx, kdai6pat[5] == &kdai6pat5);
    TEST_ASSERT_EQ_INT(ctx, -24, kdai6pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, kdai6pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 314, kdai6pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 319, kdai6pat5.spra[0].index);
}

static void test_kdai6_init_and_entry_dispatch_callbacks(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_kdai6_state();
    platform->xposi.w.h = 100;
    platform->yposi.w.h = 200;

    kdai6(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17258, platform->sproffset);
    TEST_ASSERT_TRUE(ctx, platform->patbase == kdai6pat);
    TEST_ASSERT_EQ_INT(ctx, 12, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, platform->sprhsize);
    assert_entry_callbacks(ctx, platform, 100);
}

static void test_kdai6_sub2_uses_timer_low_byte_and_scaled_sine(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    Sint16 offset;

    reset_kdai6_state();
    gametimer.w = 0x12fe;
    sinset_sin = 80;

    offset = kd_dair6_sub2(platform);

    TEST_ASSERT_EQ_INT(ctx, 0xfe, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 20, offset);
    TEST_ASSERT_EQ_INT(ctx, 1, kdai6_get_work(platform)->wobble_counter);
}

static void test_kdai6_moves_each_direction_from_origin(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_kdai6_state();
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    kd_mv_right(platform);
    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, platform->xspeed.w);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    kd_mv_left(platform);
    TEST_ASSERT_EQ_INT(ctx, 84, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, platform->xspeed.w);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    kd_mv_up(platform);
    TEST_ASSERT_EQ_INT(ctx, 184, platform->yposi.w.h);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    kd_mv_down(platform);
    TEST_ASSERT_EQ_INT(ctx, 216, platform->yposi.w.h);
}

static void test_dai6_move_dispatches_all_userflag_directions(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_kdai6_state();
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    platform->userflag.b.h = 0;
    dai6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    platform->userflag.b.h = 1;
    dai6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 84, platform->xposi.w.h);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    platform->userflag.b.h = 2;
    dai6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 184, platform->yposi.w.h);

    reset_kdai6_state();
    platform = &actwk[8];
    init_platform(platform, 100, 200);
    sinset_sin = 64;
    platform->userflag.b.h = 3;
    dai6_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 216, platform->yposi.w.h);
}

static void test_kdai6_move_without_ride_patches_and_returns(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    init_platform(platform, 100, 200);
    reset_kdai6_logs();
    sinset_sin = 64;
    ridechk_result = 0;

    kdai6_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 116, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == platform);
    TEST_ASSERT_TRUE(ctx, patchg_table == kdai6pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == player);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat);
}

static void test_kdai6_move_with_ride_initializes_player_and_rotates(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    init_platform(platform, 100, 200);
    player->xposi.w.h = 80;
    player->actflg = 255;
    player->r_no0 = 2;
    player->actno = 1;
    swdata1.w = 0x1200;
    ridechk_result = 1;
    reset_kdai6_logs();

    kdai6_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 253, player->actflg);
    TEST_ASSERT_EQ_INT(ctx, 45, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 132, player_work_get(player)->special_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(player)->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 36, player_work_get(player)->orbit_radius);
    TEST_ASSERT_EQ_INT(ctx, 152, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 132, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 3, player->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0x1200, swdata.w);
}

static void test_kdai6_move_ride_preserves_existing_spin_and_skips_fast_player(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    init_platform(platform, 100, 200);
    player_work_get(player)->special_angle = 33;
    player_work_get(player)->status_flags = 1;
    player_work_get(player)->orbit_radius = 9;
    player->r_no0 = 6;
    ridechk_result = 1;
    reset_kdai6_logs();

    kdai6_move(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 33, player_work_get(player)->special_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(player)->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 9, player_work_get(player)->orbit_radius);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
}

static void test_kdai6_ridechk_forwards_to_player_slot(test_context *ctx) {
    sprite_status *platform = &actwk[8];

    reset_kdai6_state();

    kdai6_ridechk(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
}

static void test_kdai6_rangecheck_uses_player_bottom(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    platform->yposi.w.h = 100;

    player->yposi.w.h = 70;
    player->sprvsize = 10;
    TEST_ASSERT_EQ_INT(ctx, 0, kdai6_rangecheck(platform));

    player->yposi.w.h = 95;
    player->sprvsize = 10;
    TEST_ASSERT_EQ_INT(ctx, 0, kdai6_rangecheck(platform));

    player->yposi.w.h = 100;
    player->sprvsize = 16;
    TEST_ASSERT_EQ_INT(ctx, -1, kdai6_rangecheck(platform));
}

static void test_kaiten_play_uses_swdata1_and_swdata2(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    platform->xposi.w.h = 100;
    player_work_get(player)->special_angle = 60;
    player_work_get(player)->orbit_radius = 5;
    player->actno = 1;
    swdata1.w = 0x1200;
    sinset_cos = 256;

    kaiten_play(platform);

    TEST_ASSERT_EQ_INT(ctx, 64, player_work_get(player)->special_angle);
    TEST_ASSERT_EQ_INT(ctx, 105, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, player->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 6, player_work_get(player)->orbit_radius);
    TEST_ASSERT_EQ_INT(ctx, 0x1200, swdata.w);

    reset_kdai6_state();
    platform = &actwk[8];
    player = &actwk[0];
    platform->xposi.w.h = 100;
    player_work_get(player)->special_angle = 0;
    player_work_get(player)->orbit_radius = 7;
    player->actno = 2;
    swdata2.w = 0x3400;

    kaiten_play(platform);

    TEST_ASSERT_EQ_INT(ctx, 4, player_work_get(player)->special_angle);
    TEST_ASSERT_EQ_INT(ctx, 107, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0x3400, swdata.w);
}

static void test_k_move_adjusts_distance_for_all_direction_bits(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    platform->xposi.w.h = 100;
    player->xposi.w.h = 90;
    player_work_get(player)->orbit_radius = 3;
    swdata.b.h = 4;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 4, player_work_get(player)->orbit_radius);

    player->xposi.w.h = 90;
    player_work_get(player)->orbit_radius = 3;
    swdata.b.h = 8;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 2, player_work_get(player)->orbit_radius);

    player->xposi.w.h = 90;
    player_work_get(player)->orbit_radius = 0;
    player_work_get(player)->erase_timer = 32768;
    swdata.b.h = 8;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->orbit_radius);

    reset_kdai6_state();
    platform = &actwk[8];
    player = &actwk[0];
    platform->xposi.w.h = 100;
    player->xposi.w.h = 110;
    player_work_get(player)->orbit_radius = 3;
    swdata.b.h = 8;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 4, player_work_get(player)->orbit_radius);

    player_work_get(player)->orbit_radius = 3;
    swdata.b.h = 4;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 2, player_work_get(player)->orbit_radius);

    player_work_get(player)->orbit_radius = 0;
    player_work_get(player)->erase_timer = 32768;
    swdata.b.h = 4;
    k_move(platform);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->orbit_radius);
}

static void test_jumpchk_d_no_input_returns_without_changes(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    player_work_get(player)->status_flags = 7;

    jumpchk_d(platform);

    TEST_ASSERT_EQ_INT(ctx, 7, player_work_get(player)->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat);
}

static void test_jumpchk_d_launches_normal_player(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    swdata.b.l = 112;
    player_work_get(player)->status_flags = 9;
    player->direc.b.h = 64;
    player->yposi.w.h = 200;
    sinset_sin = 128;
    sinset_cos = 256;

    jumpchk_d(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->status_flags);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1664, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 832, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, player_work_get(player)->jump_started);
    TEST_ASSERT_EQ_INT(ctx, 0, player_work_get(player)->jump_lock);
    TEST_ASSERT_EQ_INT(ctx, 14, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 7, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 205, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
}

static void test_jumpchk_d_chibi_existing_ground_sets_stand_flag(
    test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    swdata.b.l = 16;
    chibi_flag = 1;
    player->cddat = 4;
    player->direc.b.h = 65;
    player->yposi.w.h = 200;
    sinset_sin = 0;
    sinset_cos = 0;

    jumpchk_d(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 22, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 10, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 200, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, player->mstno.b.h);
}

static void test_jumpchk_d_chibi_launches_without_y_adjust(test_context *ctx) {
    sprite_status *platform = &actwk[8];
    sprite_status *player = &actwk[0];

    reset_kdai6_state();
    swdata.b.l = 32;
    chibi_flag = 1;
    player->direc.b.h = 64;
    player->yposi.w.h = 200;
    sinset_sin = 0;
    sinset_cos = 0;

    jumpchk_d(platform);

    TEST_ASSERT_EQ_INT(ctx, 6, player->cddat);
    TEST_ASSERT_EQ_INT(ctx, 10, player->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, player->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 200, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
}

TEST_MAIN_BEGIN;
    test_kdai6_tables_capture_literal_data(&ctx);
    test_kdai6_init_and_entry_dispatch_callbacks(&ctx);
    test_kdai6_sub2_uses_timer_low_byte_and_scaled_sine(&ctx);
    test_kdai6_moves_each_direction_from_origin(&ctx);
    test_dai6_move_dispatches_all_userflag_directions(&ctx);
    test_kdai6_move_without_ride_patches_and_returns(&ctx);
    test_kdai6_move_with_ride_initializes_player_and_rotates(&ctx);
    test_kdai6_move_ride_preserves_existing_spin_and_skips_fast_player(&ctx);
    test_kdai6_ridechk_forwards_to_player_slot(&ctx);
    test_kdai6_rangecheck_uses_player_bottom(&ctx);
    test_kaiten_play_uses_swdata1_and_swdata2(&ctx);
    test_k_move_adjusts_distance_for_all_direction_bits(&ctx);
    test_jumpchk_d_no_input_returns_without_changes(&ctx);
    test_jumpchk_d_launches_normal_player(&ctx);
    test_jumpchk_d_chibi_existing_ground_sets_stand_flag(&ctx);
    test_jumpchk_d_chibi_launches_without_y_adjust(&ctx);
TEST_MAIN_END

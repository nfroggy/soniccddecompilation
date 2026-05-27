#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
Uint8 chibi_flag;
short_union editmode;
short_union stageno;

static int sound_count;
static Sint16 sound_requests[8];
static int playdieset_count;
static sprite_status *playdieset_actor;
static int dircol_count;
static int dircolm_count;
static int dircol_d_count;
static int dircol_u2_count;
static Sint16 dircol_result;
static Sint16 dircolm_result;
static Sint16 dircol_d_result;
static Sint16 dircol_u2_result;

void soundset(Sint16 ReqNo);
Sint16 playdieset(sprite_status *pActwk);
Sint16 dircol(sprite_status *pActwk, char *cpDirec);
Sint16 dircolm(sprite_status *pActwk, char *cpDirec);
Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_u2(sprite_status *pActwk);

#include "src/ridechk.c"

void soundset(Sint16 ReqNo) {
    if (sound_count < 8) {
        sound_requests[sound_count] = ReqNo;
    }
    ++sound_count;
}

Sint16 playdieset(sprite_status *pActwk) {
    ++playdieset_count;
    playdieset_actor = pActwk;
    return 0;
}

Sint16 dircol(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    (void)cpDirec;
    ++dircol_count;
    return dircol_result;
}

Sint16 dircolm(sprite_status *pActwk, char *cpDirec) {
    (void)pActwk;
    (void)cpDirec;
    ++dircolm_count;
    return dircolm_result;
}

Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3) {
    (void)pActwk;
    *ipD0 = 11;
    *ipD1 = 12;
    *ipD3 = 13;
    ++dircol_d_count;
    return dircol_d_result;
}

Sint16 dircol_u2(sprite_status *pActwk) {
    (void)pActwk;
    ++dircol_u2_count;
    return dircol_u2_result;
}

static void reset_ridechk_state(void) {
    memset(actwk, 0, sizeof(actwk));
    chibi_flag = 0;
    memset(&editmode, 0, sizeof(editmode));
    memset(&stageno, 0, sizeof(stageno));
    sound_count = 0;
    memset(sound_requests, 0, sizeof(sound_requests));
    playdieset_count = 0;
    playdieset_actor = 0;
    dircol_count = 0;
    dircolm_count = 0;
    dircol_d_count = 0;
    dircol_u2_count = 0;
    dircol_result = 0;
    dircolm_result = 0;
    dircol_d_result = 0;
    dircol_u2_result = 0;
}

static void setup_platform_and_player(sprite_status **platform,
                                      sprite_status **player) {
    *platform = &actwk[5];
    *player = &actwk[0];
    memset(*platform, 0, sizeof(**platform));
    memset(*player, 0, sizeof(**player));
    (*platform)->actflg = 128;
    (*platform)->actno = 20;
    (*platform)->sprhsize = 20;
    (*platform)->sprvsize = 8;
    (*platform)->xposi.w.h = 100;
    (*platform)->yposi.w.h = 100;
    (*player)->actno = 1;
    (*player)->sprvsize = 10;
    (*player)->sprhs = 5;
    (*player)->xposi.w.h = 100;
    (*player)->yposi.w.h = 80;
}

static void test_ride_on_clr_rejects_nonmatching_state(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);

    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_clr(platform, player));
    platform->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_clr(platform, player));
    player->cddat = 0;
    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_clr(platform, player));
    player->cddat = 8;
    player->actfree[19] = 3;
    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_clr(platform, player));
}

static void test_ride_on_clr_clears_matching_ride_state(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->cddat = 8 | 32;
    player->cddat = 8;
    player->actfree[19] = 5;
    player->actfree[0] = 1;
    player->actfree[2] = 3;
    player->actfree[14] = 99;

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr(platform, player));

    TEST_ASSERT_EQ_INT(ctx, 1, sound_count);
    TEST_ASSERT_EQ_INT(ctx, 171, sound_requests[0]);
}

static void test_ride_on_clr_preserves_special_player_flags(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->cddat = 8;
    player->cddat = 8;
    player->actfree[19] = 5;
    player->actfree[2] = 64 | 1;
    ride_on_clr(platform, player);

    platform->cddat = 8;
    player->cddat = 8;
    player->actfree[19] = 5;
    player->actfree[2] = 1;
    player->mstno.b.h = 23;
    ride_on_clr(platform, player);

    platform->cddat = 8;
    player->cddat = 8;
    player->actfree[19] = 5;
    player->mstno.b.h = 43;
    ride_on_clr(platform, player);
}

static void test_ride_on_set_mounts_player_and_adjusts_crouch_size(
    test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->r_no0 = 4;
    player->cddat = 4 | 16;
    player->mstno.b.h = 9;
    player->yposi.w.h = 100;
    player->xspeed.w = 12;
    player->yspeed.w = 77;

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set(platform, player));

}

static void test_ride_on_set_uses_chibi_size_and_clears_prior_platform(
    test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    chibi_flag = 1;
    player->cddat = 4 | 8;
    player->actfree[19] = 7;
    actwk[7].cddat = 8 | 32;
    player->yposi.w.h = 100;

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set(platform, player));

    TEST_ASSERT_EQ_INT(ctx, 32, actwk[7].cddat);
}

static void test_ride_on_set_returns_for_same_platform_and_special_cases(
    test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->cddat = 8;
    player->cddat = 8;
    player->actfree[19] = 5;
    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_set(platform, player));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->mstno.b.h = 43;
    TEST_ASSERT_EQ_INT(ctx, -1, ride_on_set(platform, player));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->r_no0 = 2;
    player->xspeed.w = 44;
    player->mspeed.w = 9;
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set(platform, player));
}

static void test_hitchk_rejects_early_conditions(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);

    player->mstno.b.h = 23;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    player->mstno.b.h = 0;
    player->actfree[2] = 64;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    player->actfree[2] = 0;
    player->r_no0 = 6;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    player->r_no0 = 0;
    player->actno = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    player->actno = 1;
    platform->actflg = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    platform->actflg = 128;
    editmode.b.h = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    editmode.b.h = 0;
    player->xposi.w.h = 40;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    player->xposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));
}

static void test_ridechk_and_hitchk_u_set_routine_and_hit(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->yposi.w.h = 80;
    player->yspeed.w = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk(platform, player));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->yposi.w.h = 80;
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_u(platform, player));
}

static void test_hitchk_special_mstno_and_vertical_speed_branches(
    test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->mstno.b.h = 43;
    platform->cddat = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));
    platform->cddat = 8;
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk(platform, player));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->r_no1 = 1;
    player->yspeed.w = -1;
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk(platform, player));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->r_no1 = 1;
    player->yspeed.w = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk(platform, player));
}

static void test_hit_v_rejects_vertical_misses(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->yposi.w.h = 50;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_v(platform, player, 10, 20));

    player->yposi.w.h = 130;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_v(platform, player, 10, 20));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 25;
    actwk[0].cddat = 4;
    player->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_v(platform, player, 25, 10));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->yposi.w.h = 105;
    player->xspeed.w = -1;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_v(platform, player, 25, 10));
}

static void test_hit_x_pushes_and_clears_by_side_and_speed(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->r_no1 = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, 1, 6));

    platform->r_no1 = 0;
    platform->actno = 10;
    player->cddat = 2;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, 1, 6));

    platform->actno = 20;
    player->cddat = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, 1, 4));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->xspeed.w = 5;
    player->xposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, 3, 6));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->xspeed.w = 0;
    platform->cddat = 32;
    player->cddat = 32;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, 3, 6));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->xspeed.w = -5;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, -3, 6));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->xspeed.w = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_x(platform, player, -3, 6));
}

static void test_hit_y_lands_on_platform_and_collision_failures(
    test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 25;
    player->cddat = 4;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_y(platform, player, 1));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->yspeed.w = -1;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_y(platform, player, 1));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    TEST_ASSERT_EQ_INT(ctx, 1, hit_y(platform, player, -5));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->xspeed.w = 2;
    platform->yspeed.w = 3;
    dircolm_result = -4;
    dircol_d_result = 0;
    player->xposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_y(platform, player, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, dircolm_count);
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_d_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->xspeed.w = -2;
    dircolm_result = -4;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_y(platform, player, 0));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    dircol_d_result = -3;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_y(platform, player, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_d_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->yspeed.w = -1;
    dircol_u2_result = -2;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_y(platform, player, 0));
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->yspeed.w = -1;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_y(platform, player, 0));
    TEST_ASSERT_EQ_INT(ctx, 0, dircol_u2_count);
}

static void test_hit_yu_branches(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->r_no1 = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_yu(platform, player, -5));

    platform->r_no1 = 0;
    platform->actno = 9;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_yu(platform, player, -5));

    platform->actno = 20;
    player->cddat = 2;
    player->yposi.w.h = 80;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_yu(platform, player, -5));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->yspeed.w = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_yu(platform, player, -5));
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->yspeed.w = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_yu(platform, player, -5));
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->r_no1 = 2;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_yu(platform, player, -5));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->actflg = 2;
    TEST_ASSERT_EQ_INT(ctx, 1, hit_yu(platform, player, -5));

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->actflg = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, hit_yu(platform, player, -5));
}

static void test_side_coli_and_push_paths(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    push_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, dircol_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->xspeed.w = 2;
    dircol_result = 0;
    side_coli(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    dircol_result = -1;
    side_coli(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->xspeed.w = -2;
    dircol_result = -1;
    side_coli(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    platform->xspeed.w = 2;
    side_coli(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, dircol_count);
}

static void test_hit_set_and_hit_clr_collision_ownership(test_context *ctx) {
    sprite_status *platform;
    sprite_status *player;

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    platform->actno = 10;
    player->colino = 7;
    hit_set(platform, player);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    hit_set(platform, player);

    hit_set(platform, player);

    reset_ridechk_state();
    setup_platform_and_player(&platform, &player);
    player->colino = 7;
    actwk[7].xspeed.w = 0;
    platform->xspeed.w = 0;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    platform->xspeed.w = 1;
    player->xposi.w.h = 200;
    actwk[7].xposi.w.h = 100;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    player->xposi.w.h = 50;
    platform->actno = 21;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

#if defined(R41A)
    platform->actno = 40;
    actwk[7].actno = 39;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);
#endif

    platform->actno = 39;
    actwk[7].actno = 39;
    stageno.w = 768;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    stageno.w = 0;
    hit_set(platform, player);
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    player->colino = 5;
    hit_clr(platform, player);
    player->colino = 7;
    hit_clr(platform, player);
}

TEST_MAIN_BEGIN;
    test_ride_on_clr_rejects_nonmatching_state(&ctx);
    test_ride_on_clr_clears_matching_ride_state(&ctx);
    test_ride_on_clr_preserves_special_player_flags(&ctx);
    test_ride_on_set_mounts_player_and_adjusts_crouch_size(&ctx);
    test_ride_on_set_uses_chibi_size_and_clears_prior_platform(&ctx);
    test_ride_on_set_returns_for_same_platform_and_special_cases(&ctx);
    test_hitchk_rejects_early_conditions(&ctx);
    test_ridechk_and_hitchk_u_set_routine_and_hit(&ctx);
    test_hitchk_special_mstno_and_vertical_speed_branches(&ctx);
    test_hit_v_rejects_vertical_misses(&ctx);
    test_hit_x_pushes_and_clears_by_side_and_speed(&ctx);
    test_hit_y_lands_on_platform_and_collision_failures(&ctx);
    test_hit_yu_branches(&ctx);
    test_side_coli_and_push_paths(&ctx);
    test_hit_set_and_hit_clr_collision_ownership(&ctx);
TEST_MAIN_END;

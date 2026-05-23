#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static Sint16 emycol_d_result;
static int emycol_d_count;
static Sint16 emycol_r_result;
static int emycol_r_count;
static Uint8 emycol_r_arg;
static Sint16 emycol_l_result;
static int emycol_l_count;
static Uint8 emycol_l_arg;
static Sint16 atan_sonic_result;
static int atan_sonic_count;
static Sint16 atan_sonic_x;
static Sint16 atan_sonic_y;
static Sint16 sinset_sin_result;
static Sint16 sinset_cos_result;
static int sinset_count;
static Uint8 sinset_angle;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_r(sprite_status *pActwk, Uint8 sprhs);
Sint16 emycol_l(sprite_status *pActwk, Uint8 sprhs);
Sint16 atan_sonic(Sint16 x, Sint16 y);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r7/dango7.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

Sint16 emycol_r(sprite_status *pActwk, Uint8 sprhs) {
    (void)pActwk;
    ++emycol_r_count;
    emycol_r_arg = sprhs;
    return emycol_r_result;
}

Sint16 emycol_l(sprite_status *pActwk, Uint8 sprhs) {
    (void)pActwk;
    ++emycol_l_count;
    emycol_l_arg = sprhs;
    return emycol_l_result;
}

Sint16 atan_sonic(Sint16 x, Sint16 y) {
    ++atan_sonic_count;
    atan_sonic_x = x;
    atan_sonic_y = y;
    return atan_sonic_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin_result;
    *cos = sinset_cos_result;
}

static void write_actor_speed(sprite_status *actor, Sint32 speed) {
    memcpy(&actor->actfree[0], &speed, sizeof(speed));
}

static Sint32 read_actor_speed(sprite_status *actor) {
    Sint32 speed;

    memcpy(&speed, &actor->actfree[0], sizeof(speed));
    return speed;
}

static void write_actor_variant(sprite_status *actor, Sint32 variant) {
    memcpy(&actor->actfree[4], &variant, sizeof(variant));
}

static Sint32 read_actor_variant(sprite_status *actor) {
    Sint32 variant;

    memcpy(&variant, &actor->actfree[4], sizeof(variant));
    return variant;
}

static void reset_dango7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    emycol_d_result = 0;
    emycol_d_count = 0;
    emycol_r_result = 0;
    emycol_r_count = 0;
    emycol_r_arg = 0;
    emycol_l_result = 0;
    emycol_l_count = 0;
    emycol_l_arg = 0;
    atan_sonic_result = 0;
    atan_sonic_count = 0;
    atan_sonic_x = 0;
    atan_sonic_y = 0;
    sinset_sin_result = 0;
    sinset_cos_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
}

static void assert_pattern(test_context *ctx, sprite_pattern *pattern,
                           int xoff, int yoff, int etc, int index) {
    TEST_ASSERT_EQ_INT(ctx, 1, pattern->cnt);
    TEST_ASSERT_EQ_INT(ctx, xoff, pattern->spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, yoff, pattern->spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, etc, pattern->spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, index, pattern->spra[0].index);
}

static void assert_bytes(test_context *ctx, Uint8 *actual,
                         const int *expected, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        TEST_ASSERT_EQ_INT(ctx, expected[i], actual[i]);
    }
}

static void test_dango7_tables_capture_literal_data(test_context *ctx) {
    const int pchg_e00_expected[4] = {9, 0, 1, 255};
    const int pchg_e03_expected[8] = {4, 2, 3, 2, 3, 2, 3, 252};
    const int pchg_b00_expected[4] = {29, 0, 1, 255};
    const int pchg_b03_expected[14] = {9, 2, 3, 2, 3, 2, 3,
                                       2, 3, 2, 3, 2, 3, 252};

    TEST_ASSERT_TRUE(ctx, pchg_e[0] == pat_chg_e00);
    TEST_ASSERT_TRUE(ctx, pchg_e[3] == pat_chg_e03);
    TEST_ASSERT_TRUE(ctx, pchg_b[0] == pat_chg_b00);
    TEST_ASSERT_TRUE(ctx, pchg_b[3] == pat_chg_b03);
    assert_bytes(ctx, pat_chg_e00, pchg_e00_expected, 4);
    assert_bytes(ctx, pat_chg_e03, pchg_e03_expected, 8);
    assert_bytes(ctx, pat_chg_b00, pchg_b00_expected, 4);
    assert_bytes(ctx, pat_chg_b03, pchg_b03_expected, 14);

    TEST_ASSERT_TRUE(ctx, pat_dango_e[0] == &spr_dango_e_00);
    TEST_ASSERT_TRUE(ctx, pat_dango_e[5] == &spr_dango_e_05);
    TEST_ASSERT_TRUE(ctx, pat_dango_b[0] == &spr_dango_b_00);
    TEST_ASSERT_TRUE(ctx, pat_dango_b[5] == &spr_dango_b_05);
    assert_pattern(ctx, &spr_dango_e_00, -16, -12, 0, 494);
    assert_pattern(ctx, &spr_dango_e_02, -16, -16, 0, 496);
    assert_pattern(ctx, &spr_dango_e_05, -16, -15, 0, 502);
    assert_pattern(ctx, &spr_dango_b_00, -16, -12, 0, 497);
    assert_pattern(ctx, &spr_dango_b_02, -16, -17, 0, 499);
    assert_pattern(ctx, &spr_dango_b_05, -16, -15, 0, 502);
}

static void test_dango7_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    enemy_suicide_result = -1;
    dango(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_dango7_initializes_green_and_brown_variants(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    dango(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, read_actor_speed(actor));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_dango7_state();
    time_flag = 1;
    actor->userflag.b.h = 1;
    dango(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, read_actor_variant(actor));
}

static void test_dango7_fall_walk_and_roll_setup_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->yposi.w.h = 100;
    emycol_d_result = -5;
    a_fall(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);

    reset_dango7_state();
    actor->yposi.w.h = 100;
    actor->r_no0 = 2;
    emycol_d_result = 4;
    a_fall(actor);

    reset_dango7_state();
    a_walk(actor);
    TEST_ASSERT_EQ_INT(ctx, -24576, read_actor_speed(actor));

    reset_dango7_state();
    actor->actflg = 1;
    actor->userflag.b.h = 1;
    a_roll(actor);
    TEST_ASSERT_EQ_INT(ctx, 65536, read_actor_speed(actor));
}

static void test_dango7_transition_helpers_capture_exact_state(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->r_no0 = 8;
    actor->mstno.b.h = 2;
    a_walk_stop(actor);

    reset_dango7_state();
    actor->r_no0 = 12;
    actor->mstno.b.h = 3;
    actor->yposi.w.h = 50;
    actor->sprvsize = 9;
    a_to_roll(actor);

    reset_dango7_state();
    actor->r_no0 = 16;
    actor->mstno.b.h = 2;
    actor->yposi.w.h = 50;
    actor->sprvsize = 16;
    actor->colino = 44;
    write_actor_speed(actor, -1234);
    a_roll_stop(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, read_actor_speed(actor));

    reset_dango7_state();
    actor->r_no0 = 24;
    actor->mstno.b.h = 5;
    actor->yposi.w.h = 50;
    actor->sprvsize = 13;
    actor->colino = 237;
    a_to_walk(actor);

    reset_dango7_state();
    actor->r_no0 = 28;
    actor->mstno.b.h = 7;
    actor->yposi.w.h = 50;
    actor->sprvsize = 16;
    a_to_walk1(actor);
}

static void test_dango7_move_reverses_when_wall_blocks(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->actflg = 0;
    actor->cddat = 0;
    actor->sprhs = 14;
    write_actor_speed(actor, 65536);
    emycol_r_result = -1;
    a_walk1(actor);

    TEST_ASSERT_EQ_INT(ctx, -65536, read_actor_speed(actor));
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 14, emycol_r_arg);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_dango7_walk1_detects_player_side_and_green_patch(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->actflg = 1;
    actor->sprhs = 14;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 64;
    actor->r_no0 = 6;
    actwk[0].xposi.w.h = 110;
    actwk[0].yposi.w.h = 60;
    write_actor_speed(actor, 65536);
    write_actor_variant(actor, 0);
    a_walk1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);

    reset_dango7_state();
    actor->actflg = 0;
    actor->sprhs = 14;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 64;
    actor->r_no0 = 6;
    actwk[0].xposi.w.h = 90;
    actwk[0].yposi.w.h = 60;
    write_actor_speed(actor, 65536);
    write_actor_variant(actor, 0);
    a_walk1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);
}

static void test_dango7_move_restores_x_when_floor_is_out_of_range(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    Sint16 moved;

    reset_dango7_state();
    actor->sprhs = 14;
    actor->xposi.w.h = 20;
    actor->yposi.w.h = 30;
    write_actor_speed(actor, 65536);
    emycol_r_result = 0;
    emycol_d_result = 7;
    moved = a_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, moved);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
}

static void test_dango7_move_patchg_and_roll_collision_paths(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->sprhs = 14;
    actor->userflag.b.h = 1;
    write_actor_variant(actor, 1);
    write_actor_speed(actor, -4096);
    emycol_l_result = 0;
    emycol_d_result = 3;
    a_walk1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_l_count);
    TEST_ASSERT_EQ_INT(ctx, (Uint8)-14, emycol_l_arg);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);

    reset_dango7_state();
    actor->colicnt = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 40;
    actwk[0].yposi.w.h = 180;
    atan_sonic_result = 33;
    sinset_sin_result = 64;
    sinset_cos_result = 128;
    a_roll1(actor);
    TEST_ASSERT_EQ_INT(ctx, 60, atan_sonic_x);
    TEST_ASSERT_EQ_INT(ctx, 20, atan_sonic_y);
    TEST_ASSERT_EQ_INT(ctx, 33, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, -896, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -448, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat & 2);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 229376, read_actor_speed(actor));
}

static void test_dango7_roll1_no_collision_moves_or_advances_state(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_dango7_state();
    actor->sprhs = 14;
    actor->colicnt = 0;
    actor->xposi.w.h = 12;
    actor->yposi.w.h = 30;
    write_actor_speed(actor, 65536);
    write_actor_variant(actor, 0);
    emycol_r_result = 0;
    emycol_d_result = -6;
    a_roll1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);

    reset_dango7_state();
    actor->sprhs = 14;
    actor->colicnt = 0;
    actor->xposi.w.h = 12;
    actor->yposi.w.h = 30;
    write_actor_speed(actor, 65536);
    write_actor_variant(actor, 1);
    emycol_r_result = 0;
    emycol_d_result = 6;
    a_roll1(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);

    reset_dango7_state();
    actor->sprhs = 14;
    actor->r_no0 = 18;
    actor->colicnt = 0;
    write_actor_speed(actor, 65536);
    emycol_r_result = -1;
    a_roll1(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

TEST_MAIN_BEGIN;
test_dango7_tables_capture_literal_data(&ctx);
test_dango7_suicide_short_circuits(&ctx);
test_dango7_initializes_green_and_brown_variants(&ctx);
test_dango7_fall_walk_and_roll_setup_paths(&ctx);
test_dango7_transition_helpers_capture_exact_state(&ctx);
test_dango7_move_reverses_when_wall_blocks(&ctx);
test_dango7_walk1_detects_player_side_and_green_patch(&ctx);
test_dango7_move_restores_x_when_floor_is_out_of_range(&ctx);
test_dango7_move_patchg_and_roll_collision_paths(&ctx);
test_dango7_roll1_no_collision_moves_or_advances_state(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "types.h"

char dirstk[4];

typedef struct {
    int rotated;
    Sint16 x;
    Sint16 y;
    Sint16 offset;
    Sint16 mask;
    Sint16 rideon;
    char *dir_ptr;
} scd_call;

static scd_call scd_calls[32];
static Sint16 scd_results[32];
static char scd_dir_results[32];
static int scd_call_count;

Sint16 scdchk(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
              char *cpDirStk);
Sint16 scdchk_r(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
                Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
                char *cpDirstk);

#include "src/dircol.c"

Sint16 scdchk(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
              char *cpDirStk) {
    int index = scd_call_count++;
    (void)pActwk;
    scd_calls[index].rotated = 0;
    scd_calls[index].x = iXposi;
    scd_calls[index].y = iYposi;
    scd_calls[index].offset = iOffset;
    scd_calls[index].mask = iBlkMsk;
    scd_calls[index].rideon = iRideon;
    scd_calls[index].dir_ptr = cpDirStk;
    *cpDirStk = scd_dir_results[index];
    return scd_results[index];
}

Sint16 scdchk_r(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
                Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon,
                char *cpDirstk) {
    int index = scd_call_count++;
    (void)pActwk;
    scd_calls[index].rotated = 1;
    scd_calls[index].x = iXposi;
    scd_calls[index].y = iYposi;
    scd_calls[index].offset = iOffset;
    scd_calls[index].mask = iBlkMsk;
    scd_calls[index].rideon = iRideon;
    scd_calls[index].dir_ptr = cpDirstk;
    *cpDirstk = scd_dir_results[index];
    return scd_results[index];
}

static void reset_dircol_state(void) {
    memset(dirstk, 0, sizeof(dirstk));
    memset(scd_calls, 0, sizeof(scd_calls));
    memset(scd_results, 0, sizeof(scd_results));
    memset(scd_dir_results, 0, sizeof(scd_dir_results));
    scd_call_count = 0;
}

static void setup_actor(sprite_status *actor) {
    memset(actor, 0, sizeof(*actor));
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->xspeed.w = 3;
    actor->yspeed.w = -2;
    actor->sprhs = 5;
    actor->sprvsize = 12;
}

static void assert_scd_call(test_context *ctx, int index, int rotated,
                            Sint16 x, Sint16 y, Sint16 offset, Sint16 mask,
                            Sint16 rideon, char *dir_ptr) {
    TEST_ASSERT_EQ_INT(ctx, rotated, scd_calls[index].rotated);
    TEST_ASSERT_EQ_INT(ctx, x, scd_calls[index].x);
    TEST_ASSERT_EQ_INT(ctx, y, scd_calls[index].y);
    TEST_ASSERT_EQ_INT(ctx, offset, scd_calls[index].offset);
    TEST_ASSERT_EQ_INT(ctx, mask, scd_calls[index].mask);
    TEST_ASSERT_EQ_INT(ctx, rideon, scd_calls[index].rideon);
    TEST_ASSERT_TRUE(ctx, scd_calls[index].dir_ptr == dir_ptr);
}

static void test_swap_preserves_bit_pattern(test_context *ctx) {
    reset_dircol_state();

    TEST_ASSERT_EQ_INT(ctx, 0x12345678, (Sint32)swap(0x12345678));
    TEST_ASSERT_EQ_INT(ctx, -2, (Sint32)swap(-2));
}

static void test_dircol_dispatches_down_up_left_and_right(test_context *ctx) {
    sprite_status actor;
    char direction;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 11;
    direction = 0;
    TEST_ASSERT_EQ_INT(ctx, 11, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 0, 100, 209, 16, 0, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = -12;
    direction = (char)96;
    TEST_ASSERT_EQ_INT(ctx, -12, dircolm(&actor, &direction));
    assert_scd_call(ctx, 0, 0, 100, 178, -16, 4096, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 13;
    direction = (char)64;
    TEST_ASSERT_EQ_INT(ctx, 13, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 1, 85, 207, -16, 2048, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 14;
    direction = (char)192;
    TEST_ASSERT_EQ_INT(ctx, 14, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 1, 110, 207, 16, 0, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 15;
    direction = (char)224;
    TEST_ASSERT_EQ_INT(ctx, 15, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 0, 100, 209, 16, 0, 14, &dirstk[0]);
}

static void test_dircol_side_dispatch_adds_y_when_low_bits_clear(
    test_context *ctx) {
    sprite_status actor;
    char direction;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 21;
    direction = (char)40;
    TEST_ASSERT_EQ_INT(ctx, 21, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 1, 85, 199, -16, 2048, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 22;
    direction = (char)192;
    TEST_ASSERT_EQ_INT(ctx, 22, dircol(&actor, &direction));
    assert_scd_call(ctx, 0, 1, 110, 207, 16, 0, 14, &dirstk[0]);
}

static void test_dircol2_rewrites_direction_and_switches_all_quadrants(
    test_context *ctx) {
    sprite_status actor;
    char direction;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 4;
    scd_results[1] = 9;
    scd_dir_results[0] = 8;
    scd_dir_results[1] = 6;
    direction = 0;
    TEST_ASSERT_EQ_INT(ctx, 4, dircol2(&actor, &direction));
    TEST_ASSERT_EQ_INT(ctx, 0, direction);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 7;
    scd_results[1] = 3;
    scd_dir_results[0] = 8;
    scd_dir_results[1] = 6;
    direction = 32;
    TEST_ASSERT_EQ_INT(ctx, 3, dircol2(&actor, &direction));
    TEST_ASSERT_EQ_INT(ctx, 64, direction);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = -2;
    scd_results[1] = -5;
    scd_dir_results[0] = 8;
    scd_dir_results[1] = 6;
    direction = 96;
    TEST_ASSERT_EQ_INT(ctx, -5, dircol2(&actor, &direction));
    TEST_ASSERT_EQ_INT(ctx, -128, direction);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 10;
    scd_results[1] = 1;
    scd_dir_results[0] = 8;
    scd_dir_results[1] = 6;
    direction = (char)160;
    TEST_ASSERT_EQ_INT(ctx, 1, dircol2(&actor, &direction));
    TEST_ASSERT_EQ_INT(ctx, 192, (Uint8)direction);
}

static void test_cardinal_two_probe_functions_use_exact_points(
    test_context *ctx) {
    sprite_status actor;
    Sint16 d0;
    Sint16 d1;
    Sint16 d3;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 2;
    scd_results[1] = 5;
    scd_dir_results[0] = 9;
    scd_dir_results[1] = 4;
    TEST_ASSERT_EQ_INT(ctx, 2, dircol_d(&actor, &d0, &d1, &d3));
    TEST_ASSERT_EQ_INT(ctx, 5, d0);
    TEST_ASSERT_EQ_INT(ctx, 2, d1);
    TEST_ASSERT_EQ_INT(ctx, 0, d3);
    assert_scd_call(ctx, 0, 0, 105, 212, 16, 0, 13, &dirstk[0]);
    assert_scd_call(ctx, 1, 0, 95, 212, 16, 0, 13, &dirstk[2]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 6;
    scd_results[1] = 1;
    scd_dir_results[0] = 2;
    scd_dir_results[1] = 3;
    TEST_ASSERT_EQ_INT(ctx, 1, dircol_r(&actor, &d0, &d1, &d3));
    TEST_ASSERT_EQ_INT(ctx, -64, d3);
    assert_scd_call(ctx, 0, 1, 112, 195, 16, 0, 14, &dirstk[0]);
    assert_scd_call(ctx, 1, 1, 112, 205, 16, 0, 14, &dirstk[2]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 8;
    scd_results[1] = 4;
    scd_dir_results[0] = 2;
    scd_dir_results[1] = 3;
    TEST_ASSERT_EQ_INT(ctx, 4, dircol_l(&actor, &d0, &d1, &d3));
    TEST_ASSERT_EQ_INT(ctx, 64, d3);
    assert_scd_call(ctx, 0, 1, 87, 195, -16, 2048, 14, &dirstk[0]);
    assert_scd_call(ctx, 1, 1, 87, 205, -16, 2048, 14, &dirstk[2]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 11;
    scd_results[1] = 7;
    scd_dir_results[0] = 2;
    scd_dir_results[1] = 3;
    TEST_ASSERT_EQ_INT(ctx, 7, dircol_u(&actor, &d0, &d1, &d3));
    TEST_ASSERT_EQ_INT(ctx, -128, d3);
    assert_scd_call(ctx, 0, 0, 105, 179, -16, 4096, 14, &dirstk[0]);
    assert_scd_call(ctx, 1, 0, 95, 179, -16, 4096, 14, &dirstk[2]);
}

static void test_single_probe_wrappers_use_exact_transforms(test_context *ctx) {
    sprite_status actor;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 31;
    TEST_ASSERT_EQ_INT(ctx, 31, dircol_d3(&actor, 120, 220));
    assert_scd_call(ctx, 0, 0, 120, 230, 16, 0, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 32;
    TEST_ASSERT_EQ_INT(ctx, 32, dircol_r2(&actor));
    assert_scd_call(ctx, 0, 1, 110, 200, 16, 0, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 33;
    TEST_ASSERT_EQ_INT(ctx, 33, dircol_l2(&actor));
    assert_scd_call(ctx, 0, 1, 85, 200, -16, 2048, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 34;
    TEST_ASSERT_EQ_INT(ctx, 34, dircol_u2(&actor));
    assert_scd_call(ctx, 0, 0, 100, 177, -16, 4096, 14, &dirstk[0]);
}

static void test_enemy_collision_helpers_probe_exact_points(test_context *ctx) {
    sprite_status actor;

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 41;
    TEST_ASSERT_EQ_INT(ctx, 41, emycol_u(&actor));
    assert_scd_call(ctx, 0, 0, 100, 179, -16, 4096, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 42;
    TEST_ASSERT_EQ_INT(ctx, 42, emycol_d(&actor));
    assert_scd_call(ctx, 0, 0, 100, 212, 16, 0, 13, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    actor.sprvsize = 250;
    scd_results[0] = 43;
    TEST_ASSERT_EQ_INT(ctx, 43, emycol_d2(&actor, 90));
    assert_scd_call(ctx, 0, 0, 90, 194, 16, 0, 13, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 44;
    TEST_ASSERT_EQ_INT(ctx, 44, emycol_l(&actor, 250));
    assert_scd_call(ctx, 0, 1, 94, 200, -16, 2048, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 45;
    TEST_ASSERT_EQ_INT(ctx, 45, emycol_l3(&actor, 80, 190));
    assert_scd_call(ctx, 0, 1, 80, 190, -16, 2048, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 46;
    TEST_ASSERT_EQ_INT(ctx, 46, emycol_r(&actor, 250));
    assert_scd_call(ctx, 0, 1, 94, 200, 16, 0, 14, &dirstk[0]);

    reset_dircol_state();
    setup_actor(&actor);
    scd_results[0] = 47;
    TEST_ASSERT_EQ_INT(ctx, 47, emycol_r3(&actor, 81, 191));
    assert_scd_call(ctx, 0, 1, 81, 191, 16, 0, 14, &dirstk[0]);
}

static void test_dircolchk_selects_low_probe_and_direction(test_context *ctx) {
    Sint16 d0;
    Sint16 d1;
    Sint16 d3;
    char direct;

    reset_dircol_state();
    dirstk[0] = 8;
    dirstk[2] = 6;
    d0 = 3;
    d1 = 7;
    d3 = 0;
    direct = 64;
    TEST_ASSERT_EQ_INT(ctx, 3, dircolchk(&d0, &d1, &d3, &direct));
    TEST_ASSERT_EQ_INT(ctx, 7, d0);
    TEST_ASSERT_EQ_INT(ctx, 3, d1);
    TEST_ASSERT_EQ_INT(ctx, 8, d3);

    reset_dircol_state();
    dirstk[0] = 8;
    dirstk[2] = 5;
    d0 = 9;
    d1 = 2;
    d3 = 0;
    direct = -128;
    TEST_ASSERT_EQ_INT(ctx, 2, dircolchk(&d0, &d1, &d3, &direct));
    TEST_ASSERT_EQ_INT(ctx, -128, d3);
}

static void test_dircolchk1_replaces_odd_direction(test_context *ctx) {
    reset_dircol_state();
    dirstk[0] = 7;
    TEST_ASSERT_EQ_INT(ctx, -64, dircolchk1(-64));

    dirstk[0] = 8;
    TEST_ASSERT_EQ_INT(ctx, 8, dircolchk1(-64));
}

TEST_MAIN_BEGIN;
    test_swap_preserves_bit_pattern(&ctx);
    test_dircol_dispatches_down_up_left_and_right(&ctx);
    test_dircol_side_dispatch_adds_y_when_low_bits_clear(&ctx);
    test_dircol2_rewrites_direction_and_switches_all_quadrants(&ctx);
    test_cardinal_two_probe_functions_use_exact_points(&ctx);
    test_single_probe_wrappers_use_exact_transforms(&ctx);
    test_enemy_collision_helpers_probe_exact_points(&ctx);
    test_dircolchk_selects_low_probe_and_direction(&ctx);
    test_dircolchk1_replaces_odd_direction(&ctx);
TEST_MAIN_END;

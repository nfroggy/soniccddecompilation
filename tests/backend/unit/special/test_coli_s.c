#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

void (*sOutputDebugString)(char *);
void (*sPrintf)(char *, const char *, ...);
Uint16 sm_adr0[16384];
Uint8 stagenm;

#include "src/special/sps_equ.c"
#include "src/special/coli_s.c"

static int debug_count;
static char debug_text[128];
static int printf_count;
static char printf_text[128];

static void test_output_debug_string(char *text) {
    ++debug_count;
    strncpy(debug_text, text, sizeof(debug_text) - 1);
    debug_text[sizeof(debug_text) - 1] = '\0';
}

static void test_sprintf(char *buffer, const char *format, ...) {
    va_list args;

    ++printf_count;
    va_start(args, format);
    vsnprintf(buffer, 80, format, args);
    va_end(args);
    strncpy(printf_text, buffer, sizeof(printf_text) - 1);
    printf_text[sizeof(printf_text) - 1] = '\0';
}

static void reset_coli_s_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(sm_adr0, 0, sizeof(sm_adr0));
    colliadr = 0;
    stagenm = 0;
    debug_count = 0;
    debug_text[0] = '\0';
    printf_count = 0;
    printf_text[0] = '\0';
    sOutputDebugString = test_output_debug_string;
    sPrintf = test_sprintf;
}

static void test_colli_init_selects_stage_collision_table(test_context *ctx) {
    int i;

    for (i = 0; i < 8; ++i) {
        reset_coli_s_state();
        stagenm = (Uint8)i;

        colli_init();

        TEST_ASSERT_TRUE(ctx, colliadr == colliadr_tbl[i]);
    }
}

static void test_stpnmget_swaps_no_bytes_and_extracts_collision_index(
    test_context *ctx) {
    Sint16 raw;
    Sint16 index;

    reset_coli_s_state();
    sm_adr0[130] = 0x1234;

    stpnmget(64, 32, &raw, &index);

    TEST_ASSERT_EQ_INT(ctx, 0x1234, (Uint16)raw);
    TEST_ASSERT_EQ_INT(ctx, 141, index);
    TEST_ASSERT_EQ_INT(ctx, 0, printf_count);
    TEST_ASSERT_EQ_INT(ctx, 0, debug_count);
}

static void test_stpnmget_clamps_large_index_and_reports_debug(
    test_context *ctx) {
    Sint16 raw;
    Sint16 index;

    reset_coli_s_state();
    sm_adr0[16383] = 0x07fc;

    stpnmget(4095, 4095, &raw, &index);

    TEST_ASSERT_EQ_INT(ctx, 0x07fc, (Uint16)raw);
    TEST_ASSERT_EQ_INT(ctx, 511, index);
    TEST_ASSERT_EQ_INT(ctx, 1, printf_count);
    TEST_ASSERT_EQ_INT(ctx, 1, debug_count);
    TEST_ASSERT_TRUE(ctx, strstr(debug_text, "stpnmget Err wD2 = ") != 0);
}

static void test_blcolli_returns_collision_table_value(test_context *ctx) {
    reset_coli_s_state();
    colliadr = ctbl30;
    sm_adr0[0] = 8;

    TEST_ASSERT_EQ_INT(ctx, ctbl30[2], blcolli(&actwk[4]));
}

static void test_plcolli_samples_center_direction_and_corners(
    test_context *ctx) {
    sprite_status_sp *actor = &actwk[4];

    reset_coli_s_state();
    colliadr = ctbl00;
    actor->x_posi.w.h = 64;
    actor->y_posi.w.h = 64;
    sm_adr0[258] = 0x3008;
    sm_adr0[129] = 0x000c;
    sm_adr0[130] = 0x0010;
    sm_adr0[257] = 0x0018;

    plcolli(actor);

    TEST_ASSERT_EQ_INT(ctx, ctbl00[2], actor->scno_ce);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->scno_sdir);
    TEST_ASSERT_EQ_INT(ctx, ctbl00[3], actor->scno_ul);
    TEST_ASSERT_EQ_INT(ctx, ctbl00[4], actor->scno_ur);
    TEST_ASSERT_EQ_INT(ctx, ctbl00[2], actor->scno_dr);
    TEST_ASSERT_EQ_INT(ctx, ctbl00[6], actor->scno_dl);
}

static void test_ufovspl_ignores_inactive_or_already_colliding_player(
    test_context *ctx) {
    sprite_status_sp *ufo = &actwk[3];
    sprite_status_sp *player = &actwk[0];

    reset_coli_s_state();
    ufo->actno = 7;
    player->actno = 0;

    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, ufo->colliflg);

    player->actno = 1;
    player->colliflg = 9;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, ufo->colliflg);
    TEST_ASSERT_EQ_INT(ctx, 9, player->colliflg);
}

static void test_ufovspl_rejects_each_bounds_check(test_context *ctx) {
    sprite_status_sp *ufo = &actwk[3];
    sprite_status_sp *player = &actwk[0];

    reset_coli_s_state();
    ufo->actno = 7;
    player->actno = 1;
    ufo->x_posi.w.h = 100;
    ufo->y_posi.w.h = 100;
    player->x_posi.w.h = 150;
    player->y_posi.w.h = 100;
    player->z_posi.w.h = 560;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);

    player->x_posi.w.h = 50;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);

    player->x_posi.w.h = 100;
    player->y_posi.w.h = 140;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);

    player->y_posi.w.h = 60;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);

    player->y_posi.w.h = 100;
    player->z_posi.w.h = 527;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);

    player->z_posi.w.h = 624;
    ufovspl(ufo);
    TEST_ASSERT_EQ_INT(ctx, 0, player->colliflg);
}

static void test_ufovspl_sets_mutual_collision_when_overlapping(
    test_context *ctx) {
    sprite_status_sp *ufo = &actwk[3];
    sprite_status_sp *player = &actwk[0];

    reset_coli_s_state();
    ufo->actno = 7;
    ufo->x_posi.w.h = 100;
    ufo->y_posi.w.h = 100;
    player->actno = 1;
    player->x_posi.w.h = 100;
    player->y_posi.w.h = 100;
    player->z_posi.w.h = 560;

    ufovspl(ufo);

    TEST_ASSERT_EQ_INT(ctx, 7, player->colliflg);
    TEST_ASSERT_EQ_INT(ctx, 1, ufo->colliflg);
}

TEST_MAIN_BEGIN;
test_colli_init_selects_stage_collision_table(&ctx);
test_stpnmget_swaps_no_bytes_and_extracts_collision_index(&ctx);
test_stpnmget_clamps_large_index_and_reports_debug(&ctx);
test_blcolli_returns_collision_table_value(&ctx);
test_plcolli_samples_center_direction_and_corners(&ctx);
test_ufovspl_ignores_inactive_or_already_colliding_player(&ctx);
test_ufovspl_rejects_each_bounds_check(&ctx);
test_ufovspl_sets_mutual_collision_when_overlapping(&ctx);
TEST_MAIN_END

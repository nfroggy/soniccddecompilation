#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

static game_info keep_work;
game_info *lpKeepWork;
Sint16 hane_cnt;
Sint16 damage_cnt;
Sint16 chg_timer;

#include "src/special/bact.c"

static void reset_bact_state(void) {
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;
    hane_cnt = 0;
    damage_cnt = 0;
    chg_timer = 0;
}

static void call_block_chg(Uint16 *hane1, Uint16 *hane2, Uint16 *dmg1,
                           Uint16 *dmg2) {
    *hane1 = 0xffff;
    *hane2 = 0xffff;
    *dmg1 = 0xffff;
    *dmg2 = 0xffff;
    Special_block_chg(hane1, hane2, dmg1, dmg2);
}

static void test_special_block_chg_uses_stage_tables(test_context *ctx) {
    static const Uint16 expected_hane[8] = {87, 103, 119, 111,
                                            104, 87,  101, 87};
    static const Uint16 expected_damage[8] = {90, 106, 122, 114,
                                              107, 90,  104, 90};
    Uint16 hane1, hane2, dmg1, dmg2;
    int i;

    for (i = 0; i < 8; ++i) {
        reset_bact_state();
        keep_work.stagenm = (Uint16)i;

        call_block_chg(&hane1, &hane2, &dmg1, &dmg2);

        TEST_ASSERT_EQ_INT(ctx, expected_hane[i], hane1);
        TEST_ASSERT_EQ_INT(ctx, expected_hane[i] + 1, hane2);
        TEST_ASSERT_EQ_INT(ctx, expected_damage[i], dmg1);
        TEST_ASSERT_EQ_INT(ctx, expected_damage[i], dmg2);
        TEST_ASSERT_EQ_INT(ctx, 1, hane_cnt);
        TEST_ASSERT_EQ_INT(ctx, 0, damage_cnt);
        TEST_ASSERT_EQ_INT(ctx, 1, chg_timer);
    }
}

static void test_special_block_chg_wraps_hane_counter(test_context *ctx) {
    Uint16 hane1, hane2, dmg1, dmg2;

    reset_bact_state();
    keep_work.stagenm = 0;
    hane_cnt = 2;

    call_block_chg(&hane1, &hane2, &dmg1, &dmg2);

    TEST_ASSERT_EQ_INT(ctx, 87, hane1);
    TEST_ASSERT_EQ_INT(ctx, 87, hane2);
    TEST_ASSERT_EQ_INT(ctx, 90, dmg1);
    TEST_ASSERT_EQ_INT(ctx, 90, dmg2);
    TEST_ASSERT_EQ_INT(ctx, 0, hane_cnt);
}

static void test_special_block_chg_advances_damage_on_even_ticks(
    test_context *ctx) {
    Uint16 hane1, hane2, dmg1, dmg2;

    reset_bact_state();
    keep_work.stagenm = 1;

    call_block_chg(&hane1, &hane2, &dmg1, &dmg2);
    TEST_ASSERT_EQ_INT(ctx, 106, dmg1);
    TEST_ASSERT_EQ_INT(ctx, 106, dmg2);
    TEST_ASSERT_EQ_INT(ctx, 0, damage_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, chg_timer);

    call_block_chg(&hane1, &hane2, &dmg1, &dmg2);
    TEST_ASSERT_EQ_INT(ctx, 106, dmg1);
    TEST_ASSERT_EQ_INT(ctx, 107, dmg2);
    TEST_ASSERT_EQ_INT(ctx, 1, damage_cnt);
    TEST_ASSERT_EQ_INT(ctx, 2, chg_timer);
}

static void test_special_block_chg_wraps_damage_counter(test_context *ctx) {
    Uint16 hane1, hane2, dmg1, dmg2;

    reset_bact_state();
    keep_work.stagenm = 6;
    chg_timer = 1;
    damage_cnt = 3;

    call_block_chg(&hane1, &hane2, &dmg1, &dmg2);

    TEST_ASSERT_EQ_INT(ctx, 101, hane1);
    TEST_ASSERT_EQ_INT(ctx, 102, hane2);
    TEST_ASSERT_EQ_INT(ctx, 104, dmg1);
    TEST_ASSERT_EQ_INT(ctx, 104, dmg2);
    TEST_ASSERT_EQ_INT(ctx, 0, damage_cnt);
    TEST_ASSERT_EQ_INT(ctx, 2, chg_timer);
}

TEST_MAIN_BEGIN;
test_special_block_chg_uses_stage_tables(&ctx);
test_special_block_chg_wraps_hane_counter(&ctx);
test_special_block_chg_advances_damage_on_even_ticks(&ctx);
test_special_block_chg_wraps_damage_counter(&ctx);
TEST_MAIN_END

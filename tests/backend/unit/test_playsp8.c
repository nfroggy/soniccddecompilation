#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int scramapad_count;
static Sint16 scramapad_x[4];
static Sint16 scramapad_y[4];
static Sint16 scramapad_results[4];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);

#include "src/r8/playsp8.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    (void)pActwk;
    if (scramapad_count < 4) {
        scramapad_x[scramapad_count] = iXposi;
        scramapad_y[scramapad_count] = iYposi;
    }
    return scramapad_results[scramapad_count++];
}

static void reset_playsp8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    scramapad_count = 0;
    memset(scramapad_x, 0, sizeof(scramapad_x));
    memset(scramapad_y, 0, sizeof(scramapad_y));
    memset(scramapad_results, 0, sizeof(scramapad_results));
}

static void setup_player_for_playsp(void) {
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 2000;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 16;
}

static void test_chibi_coli_initializes_then_checks_player_collision(
    test_context *ctx) {
    sprite_status smallwk;

    reset_playsp8_state();
    memset(&smallwk, 0, sizeof(smallwk));

    chibi_coli(&smallwk);

    TEST_ASSERT_EQ_INT(ctx, 2, smallwk.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, smallwk.actflg);
    TEST_ASSERT_TRUE(ctx, smallwk.patbase == chibipat);
    TEST_ASSERT_EQ_INT(ctx, 16, smallwk.sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, smallwk.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, smallwk.sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == &smallwk);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &smallwk);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == &smallwk);
}

static void test_chibi_coli_move_rechecks_player_collision(test_context *ctx) {
    sprite_status smallwk;

    reset_playsp8_state();
    memset(&smallwk, 0, sizeof(smallwk));
    smallwk.r_no0 = 2;
    smallwk.actflg = 128;

    chibi_coli(&smallwk);

    TEST_ASSERT_EQ_INT(ctx, 2, smallwk.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 128, smallwk.actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == &smallwk);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_playsp_skips_stage_two_without_map_queries(test_context *ctx) {
    reset_playsp8_state();
    setup_player_for_playsp();
    stageno.b.l = 2;
    scramapad_results[0] = 270;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 0, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);
}

static void test_playsp_moves_right_for_left_probe_unflipped_belt(
    test_context *ctx) {
    reset_playsp8_state();
    setup_player_for_playsp();
    scramapad_results[0] = 270;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 992, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 2018, scramapad_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 1001, actwk[0].xposi.w.h);
}

static void test_playsp_uses_right_probe_when_left_probe_misses(
    test_context *ctx) {
    reset_playsp8_state();
    setup_player_for_playsp();
    scramapad_results[0] = 10;
    scramapad_results[1] = 2048 | 271;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 992, scramapad_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 1008, scramapad_x[1]);
    TEST_ASSERT_EQ_INT(ctx, 2018, scramapad_y[1]);
    TEST_ASSERT_EQ_INT(ctx, 999, actwk[0].xposi.w.h);
}

static void test_playsp_returns_when_both_probes_miss(test_context *ctx) {
    reset_playsp8_state();
    setup_player_for_playsp();
    scramapad_results[0] = 10;
    scramapad_results[1] = 20;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 2, scramapad_count);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[0].xposi.w.h);
}

static void test_playsp_inverts_direction_for_block_276(test_context *ctx) {
    reset_playsp8_state();
    setup_player_for_playsp();
    scramapad_results[0] = 276;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 999, actwk[0].xposi.w.h);

    reset_playsp8_state();
    setup_player_for_playsp();
    scramapad_results[0] = 2048 | 276;

    playsp();

    TEST_ASSERT_EQ_INT(ctx, 1001, actwk[0].xposi.w.h);
}

static void test_playsp_chk_accepts_only_belt_blocks(test_context *ctx) {
    reset_playsp8_state();

    TEST_ASSERT_EQ_INT(ctx, 0, playsp_chk(269));
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_chk(270));
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_chk(273));
    TEST_ASSERT_EQ_INT(ctx, 1, playsp_chk(276));
    TEST_ASSERT_EQ_INT(ctx, 0, playsp_chk(277));
}

TEST_MAIN_BEGIN;
test_chibi_coli_initializes_then_checks_player_collision(&ctx);
test_chibi_coli_move_rechecks_player_collision(&ctx);
test_playsp_skips_stage_two_without_map_queries(&ctx);
test_playsp_moves_right_for_left_probe_unflipped_belt(&ctx);
test_playsp_uses_right_probe_when_left_probe_misses(&ctx);
test_playsp_returns_when_both_probes_miss(&ctx);
test_playsp_inverts_direction_for_block_276(&ctx);
test_playsp_chk_accepts_only_belt_blocks(&ctx);
TEST_MAIN_END;

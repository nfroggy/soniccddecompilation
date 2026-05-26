#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);

#include "src/r4/wall1.c"

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
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

static void reset_wall1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
}

static void assert_wall1_callbacks(test_context *ctx, sprite_status *wall) {
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == wall);
}

static void test_wall1_patterns_capture_all_sizes_and_tiles(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_wall1[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_wall1[6] == &pat06);
    TEST_ASSERT_EQ_INT(ctx, 1, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL1_BASE, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -32, pat02.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat02.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL1_BASE + 2, pat02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat03.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -32, pat03.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL1_BASE + 3, pat03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -64, pat05.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat05.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL1_BASE + 5, pat05.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, pat06.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -64, pat06.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL1_BASE + 6, pat06.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 16, tbl0[0]);
    TEST_ASSERT_EQ_INT(ctx, 64, tbl0[10]);
    TEST_ASSERT_EQ_INT(ctx, 64, tbl0[13]);
}

static void test_wall1_init_runs_collision_and_draw_callbacks(test_context *ctx) {
    sprite_status *wall = &actwk[2];

    reset_wall1_state();
    wall->userflag.b.h = 0;

    wall1(wall);

    assert_wall1_callbacks(ctx, wall);
}

static void test_wall1_init_accepts_largest_pattern_variant(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_wall1_state();
    wall->userflag.b.h = 6;

    wall1(wall);

    assert_wall1_callbacks(ctx, wall);
}

static void test_wall1_move_path_skips_initialization(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall1_state();
    wall->r_no0 = 2;
    wall->userflag.b.h = 3;

    wall1(wall);

    assert_wall1_callbacks(ctx, wall);
}

TEST_MAIN_BEGIN;
    test_wall1_patterns_capture_all_sizes_and_tiles(&ctx);
    test_wall1_init_runs_collision_and_draw_callbacks(&ctx);
    test_wall1_init_accepts_largest_pattern_variant(&ctx);
    test_wall1_move_path_skips_initialization(&ctx);
TEST_MAIN_END

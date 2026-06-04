#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;

static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#include "src/r7/wall7.c"
#define hscrollwork_get_pair hscrollwork_get_pair_wall73
#define hscrollwork_set_pair hscrollwork_set_pair_wall73
#define hscrollwork_add_pair hscrollwork_add_pair_wall73
#include "src/r7/wall73.c"
#undef hscrollwork_get_pair
#undef hscrollwork_set_pair
#undef hscrollwork_add_pair

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

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static void reset_wall7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    bossflag = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void test_wall7_patterns_and_static_wall_init(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    TEST_ASSERT_TRUE(ctx, pat_wall7[0] == &wall7_pat0);
    TEST_ASSERT_TRUE(ctx, pat_wall7[5] == &wall7_pat5);
    TEST_ASSERT_EQ_INT(ctx, 1, wall7_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, wall7_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL7_BASE, wall7_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, wall7_pat5.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -48, wall7_pat5.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALL7_BASE + 5, wall7_pat5.spra[0].index);

    reset_wall7_state();
    wall->userflag.b.h = 2;
    wall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_wall73_init_and_collision(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall7_state();
    wall->userflag.b.h = 5;
    hitchk_result = 1;
    actwk[0].colino = 99;
    wall73(wall);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_wall73_move_start_and_direction(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall7_state();
    bossflag = 0;
    wall->userflag.b.l = 2;
    wall->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 0, wall73_work_get(wall)->move_target_y);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 2;
    wall->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 132, wall73_work_get(wall)->move_target_y);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 130;
    wall->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 68, wall73_work_get(wall)->move_target_y);
}

static void test_wall73_move_completion_paths(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 1;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    TEST_ASSERT_EQ_INT(ctx, -1, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == wall);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 65;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_wall7_state();
    bossflag = 128;
    wall->r_no0 = 2;
    wall->userflag.b.l = 1;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    wall73(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_wall73_move_updates_remaining_steps_and_size(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 34;
    wall->patno = 2;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 18;
    wall->patno = 0;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 2;
    wall->patno = 4;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    wall73_work_get(wall)->move_target_y = 102;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
}

TEST_MAIN_BEGIN;
test_wall7_patterns_and_static_wall_init(&ctx);
test_wall73_init_and_collision(&ctx);
test_wall73_move_start_and_direction(&ctx);
test_wall73_move_completion_paths(&ctx);
test_wall73_move_updates_remaining_steps_and_size(&ctx);
TEST_MAIN_END

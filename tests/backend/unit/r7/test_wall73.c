#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;
sprite_pattern *pat_wall7[5];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r7/wall73.c"

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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

static void reset_wall73_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(pat_wall7, 0, sizeof(pat_wall7));
    bossflag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
}

static void set_move_target(sprite_status *actor, Sint16 value) {
    wall73_work_get(actor)->move_target_y = value;
}

static Sint16 get_move_target(sprite_status *actor) {
    return wall73_work_get(actor)->move_target_y;
}

static void assert_common_callbacks(test_context *ctx, sprite_status *wall) {
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == wall);
}

static void test_wall73_initializes_shape_and_collision(test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->userflag.b.h = 2;
    hitchk_result = 1;
    actwk[0].colino = 77;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17676, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_wall7);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 48, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 48, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].colino);
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_ignores_movement_without_boss_trigger(
    test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.h = 1;
    wall->userflag.b.l = 3;
    wall->yposi.w.h = 200;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 200, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_move_target(wall));
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_starts_downward_and_upward_moves(test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 3;
    wall->yposi.w.h = 200;
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 512, wall->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 232, get_move_target(wall));
    TEST_ASSERT_EQ_INT(ctx, 202, wall->yposi.w.h);
    assert_common_callbacks(ctx, wall);

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = (Sint8)0x83;
    wall->yposi.w.h = 200;
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, -512, wall->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 168, get_move_target(wall));
    TEST_ASSERT_EQ_INT(ctx, 198, wall->yposi.w.h);
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_move_countdown_updates_downward_shape(
    test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 2;
    wall->patno = 2;
    wall->yposi.w.h = 232;
    set_move_target(wall, 232);
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 0, get_move_target(wall));
    TEST_ASSERT_EQ_INT(ctx, 1, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 1, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 248, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprvsize);
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_move_countdown_updates_upward_shape_and_wraps(
    test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 18;
    wall->patno = 0;
    wall->yposi.w.h = 168;
    set_move_target(wall, 168);
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 17, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 152, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprvsize);
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_move_countdown_can_hold_shape(test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 34;
    wall->patno = 3;
    wall->yposi.w.h = 232;
    set_move_target(wall, 232);
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 33, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 232, wall->yposi.w.h);
    assert_common_callbacks(ctx, wall);
}

static void test_wall73_final_move_clears_bossflag_or_deletes(
    test_context *ctx) {
    sprite_status *wall = &actwk[7];

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 1;
    wall->yposi.w.h = 232;
    set_move_target(wall, 232);
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 0, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_wall73_state();
    wall->r_no0 = 2;
    wall->userflag.b.l = 65;
    wall->yposi.w.h = 232;
    set_move_target(wall, 232);
    bossflag = 128;

    wall73(wall);

    TEST_ASSERT_EQ_INT(ctx, 0, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_common_callbacks(ctx, wall);
}

TEST_MAIN_BEGIN;
test_wall73_initializes_shape_and_collision(&ctx);
test_wall73_ignores_movement_without_boss_trigger(&ctx);
test_wall73_starts_downward_and_upward_moves(&ctx);
test_wall73_move_countdown_updates_downward_shape(&ctx);
test_wall73_move_countdown_updates_upward_shape_and_wraps(&ctx);
test_wall73_move_countdown_can_hold_shape(&ctx);
test_wall73_final_move_clears_bossflag_or_deletes(&ctx);
TEST_MAIN_END

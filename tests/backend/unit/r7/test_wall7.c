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
#include "src/r7/wall73.c"

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

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    ((Sint16 *)actor)[index] = value;
}

static Sint16 get_actor_word(sprite_status *actor, int index) {
    return ((Sint16 *)actor)[index];
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
    TEST_ASSERT_EQ_INT(ctx, 428, wall7_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, wall7_pat5.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -48, wall7_pat5.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 433, wall7_pat5.spra[0].index);

    reset_wall7_state();
    wall->userflag.b.h = 2;
    wall7(wall);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17270, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_wall7);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 44, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 44, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 17, wall->sprvsize);
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
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17676, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_wall7);
    TEST_ASSERT_EQ_INT(ctx, 5, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 48, wall->sprvsize);
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
    TEST_ASSERT_EQ_INT(ctx, 0, get_actor_word(wall, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, wall->yspeed.w);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 2;
    wall->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 512, wall->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 132, get_actor_word(wall, 23));
    TEST_ASSERT_EQ_INT(ctx, 102, wall->yposi.w.h);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 130;
    wall->yposi.w.h = 100;
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, -512, wall->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 68, get_actor_word(wall, 23));
    TEST_ASSERT_EQ_INT(ctx, 98, wall->yposi.w.h);
}

static void test_wall73_move_completion_paths(test_context *ctx) {
    sprite_status *wall = &actwk[4];

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 1;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    set_actor_word(wall, 23, 102);
    TEST_ASSERT_EQ_INT(ctx, -1, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == wall);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 65;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    set_actor_word(wall, 23, 102);
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_wall7_state();
    bossflag = 128;
    wall->r_no0 = 2;
    wall->userflag.b.l = 1;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    set_actor_word(wall, 23, 102);
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
    set_actor_word(wall, 23, 102);
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 33, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 102, wall->yposi.w.h);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 18;
    wall->patno = 0;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    set_actor_word(wall, 23, 102);
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 17, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 86, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, wall->sprvsize);

    reset_wall7_state();
    bossflag = 128;
    wall->userflag.b.l = 2;
    wall->patno = 4;
    wall->yposi.w.h = 100;
    wall->yspeed.w = 512;
    set_actor_word(wall, 23, 102);
    TEST_ASSERT_EQ_INT(ctx, 0, move_blk(wall));
    TEST_ASSERT_EQ_INT(ctx, 1, wall->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 118, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 11, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprvsize);
}

TEST_MAIN_BEGIN;
test_wall7_patterns_and_static_wall_init(&ctx);
test_wall73_init_and_collision(&ctx);
test_wall73_move_start_and_direction(&ctx);
test_wall73_move_completion_paths(&ctx);
test_wall73_move_updates_remaining_steps_and_size(&ctx);
TEST_MAIN_END

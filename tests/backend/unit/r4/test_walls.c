#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk2_count;
static int actwkchk2_queue[16];
static int actwkchk2_queue_count;
static int actwkchk2_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xposi;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/walls.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_pos >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return 1;
    }

    *ppNewActwk = &actwk[actwkchk2_queue[actwkchk2_queue_pos++]];
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xposi = xposi;
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xposi = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
}

static void queue_children(int first_index, int count) {
    int i;

    actwkchk2_queue_count = count;
    for (i = 0; i < count; ++i) {
        actwkchk2_queue[i] = first_index + i;
    }
}

static void test_walls_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_walls[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_walls[1] == &pat01);
    TEST_ASSERT_TRUE(ctx, pat_walls[2] == &pat02);
    TEST_ASSERT_TRUE(ctx, pat_walls[3] == &pat03);
    TEST_ASSERT_TRUE(ctx, pat_walls[4] == &pat04);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALLS_BASE, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALLS_BASE + 1, pat01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALLS_BASE + 2, pat02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_WALLS_BASE + 3, pat03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 2, patno_tbl[0][0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patno_tbl[1][0]);
    TEST_ASSERT_EQ_INT(ctx, 1, patno_tbl[8][7]);
    TEST_ASSERT_EQ_INT(ctx, 60, data_tbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 131072, data_tbl[3]);
    TEST_ASSERT_EQ_INT(ctx, -32768, data_tbl[7]);
}

static void test_walls_init_creates_children_and_moves_twice(test_context *ctx) {
    sprite_status *wall = &actwk[5];
    sprite_status *left = &actwk[20];
    sprite_status *right = &actwk[21];

    reset_state();
    queue_children(20, 8);
    wall->actno = 51;
    wall->xposi.w.h = 100;
    wall->yposi.w.h = 200;
    wall->userflag.b.h = 0;

    walls(wall);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17514, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_walls);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 100, walls_work_get(wall)->origin_x);
    TEST_ASSERT_EQ_INT(ctx, 5, walls_work_get(wall)->parent_actor);
    TEST_ASSERT_EQ_INT(ctx, 190, walls_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 20, walls_work_get(wall)->child_actors[0]);
    TEST_ASSERT_EQ_INT(ctx, 27, walls_work_get(wall)->child_actors[7]);
    TEST_ASSERT_EQ_INT(ctx, 2, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xposi);

    TEST_ASSERT_EQ_INT(ctx, 51, left->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, left->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 248, left->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, left->patno);
    TEST_ASSERT_EQ_INT(ctx, 48, left->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 48, left->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 133, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, left->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 128, left->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, walls_work_get(left)->parent_actor);

    TEST_ASSERT_EQ_INT(ctx, 51, right->actno);
    TEST_ASSERT_EQ_INT(ctx, -2, right->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 248, right->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, right->patno);
    TEST_ASSERT_EQ_INT(ctx, 32, right->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 51, right->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, right->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, -128, right->xspeed.w);
}

static void test_walls_init_failure_frames_out_without_children(
    test_context *ctx) {
    sprite_status *wall = &actwk[5];

    reset_state();
    wall->actno = 51;
    wall->xposi.w.h = 100;

    main_init(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_walls_existing_main_moves_children_once(test_context *ctx) {
    sprite_status *wall = &actwk[5];
    sprite_status *left = &actwk[30];
    sprite_status *right = &actwk[31];
    int i;

    reset_state();
    wall->r_no0 = 2;
    wall->xposi.w.h = 200;
    for (i = 0; i < 8; ++i) {
        walls_work_get(wall)->child_actors[i] = (Uint8)(30 + i);
        actwk[30 + i].xposi.w.h = 400 + i;
    }
    walls_work_get(wall)->origin_x = 200;
    walls_work_get(wall)->timer = 2;

    walls(wall);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 400, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, left->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 0, left->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 401, right->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, right->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 0, right->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, walls_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 200, frameout_s00_xposi);
}

static void test_walls_main_move_rolls_timer_to_stationary_segment(
    test_context *ctx) {
    sprite_status *wall = &actwk[5];
    sprite_status *left = &actwk[40];
    sprite_status *right = &actwk[41];
    int i;

    reset_state();
    queue_children(40, 8);
    wall->xposi.w.h = 120;
    wall->yposi.w.h = 160;

    walls(wall);

    for (i = 0; i < 8; ++i) {
        actwk[40 + i].xposi.w.h = 500 + i;
        actwk[40 + i].xposi.w.l = 0;
    }
    walls_work_get(wall)->origin_x = 120;
    walls_work_get(wall)->timer = 1;
    walls_work_get(wall)->data_index = 0;

    main_move(wall);
    TEST_ASSERT_EQ_INT(ctx, 60, walls_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 8, walls_work_get(wall)->data_index);
    TEST_ASSERT_EQ_INT(ctx, 500, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, left->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 128, left->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 500, right->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, right->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, -128, right->xspeed.w);

    main_move(wall);
    TEST_ASSERT_EQ_INT(ctx, 59, walls_work_get(wall)->timer);
    TEST_ASSERT_EQ_INT(ctx, 500, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, left->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 500, right->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -32768, right->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 0, left->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, right->xspeed.w);
}

static void test_walls_option_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *child = &actwk[30];

    reset_state();
    child->userflag.b.h = (Sint8)128;
    walls_work_get(child)->parent_actor = 5;
    actwk[5].actno = 0;

    walls(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_walls_option_checks_collision_when_parent_is_alive(
    test_context *ctx) {
    sprite_status *child = &actwk[30];

    reset_state();
    child->userflag.b.h = (Sint8)128;
    walls_work_get(child)->parent_actor = 5;
    actwk[5].actno = 51;

    walls(child);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == child);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
}

TEST_MAIN_BEGIN;
    test_walls_tables_capture_literal_data(&ctx);
    test_walls_init_creates_children_and_moves_twice(&ctx);
    test_walls_init_failure_frames_out_without_children(&ctx);
    test_walls_existing_main_moves_children_once(&ctx);
    test_walls_main_move_rolls_timer_to_stationary_segment(&ctx);
    test_walls_option_frames_out_when_parent_is_gone(&ctx);
    test_walls_option_checks_collision_when_parent_is_alive(&ctx);
TEST_MAIN_END

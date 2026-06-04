#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_request;

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r5/kowasi5.c"

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_request = ReqNo;
}

static void reset_kowasi5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    soundset_request = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_init_sets_r5_fields_and_waits(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi5_state();
    wall->userflag.b.h = 2;

    kowasi(wall);

    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, wall->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 243, wall->colino);
    TEST_ASSERT_EQ_INT(ctx, 17575, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_kowasi);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, wall->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == wall);
}

static void test_wait_collision_spawns_three_fragments(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi5_state();
    wall->actno = 47;
    wall->r_no0 = 2;
    wall->actflg = 4;
    wall->sprpri = 1;
    wall->sproffset = 17575;
    wall->patbase = pat_kowasi;
    wall->colino = 243;
    wall->colicnt = 1;
    wall->xposi.l = 100 << 16;
    wall->yposi.l = 200 << 16;
    actwk[0].xspeed.w = 100;
    actwk[0].yspeed.w = -32;
    actwk[0].yposi.w.h = 200;
    hitchk_result = 1;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);

    kowasi(wall);

    TEST_ASSERT_EQ_INT(ctx, 4, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == wall);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 176, soundset_request);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 2, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 97, wall->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 182, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -194180, kowasi5_work_get(wall)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, -67760, kowasi5_work_get(wall)->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 47, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 184, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -252434, kowasi5_work_get(&actwk[40])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 0, kowasi5_work_get(&actwk[40])->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 47, actwk[41].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[41].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[41].patno);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[41].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[41].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -194180, kowasi5_work_get(&actwk[41])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 84144, kowasi5_work_get(&actwk[41])->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_wait_negative_player_speed_flips_fragment_x_speed(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi5_state();
    wall->r_no0 = 2;
    wall->colicnt = 1;
    wall->userflag.b.h = 1;
    wall->xposi.l = 100 << 16;
    wall->yposi.l = 200 << 16;
    actwk[0].xspeed.w = -100;
    actwk[0].yposi.w.h = 200;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);

    kowasi(wall);

    TEST_ASSERT_EQ_INT(ctx, -50, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 194180, kowasi5_work_get(wall)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 252434, kowasi5_work_get(&actwk[40])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[41].patno);
    TEST_ASSERT_EQ_INT(ctx, 194180, kowasi5_work_get(&actwk[41])->x_velocity);
}

static void test_wait_allocation_failure_keeps_later_fragments_unwritten(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi5_state();
    wall->r_no0 = 2;
    wall->colicnt = 1;
    wall->xposi.l = 100 << 16;
    wall->yposi.l = 200 << 16;
    actwk[0].xspeed.w = 100;
    actwk[0].yposi.w.h = 200;

    kowasi(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_down_moves_or_frames_out_by_player_distance(test_context *ctx) {
    sprite_status *fragment = &actwk[3];

    reset_kowasi5_state();
    fragment->r_no0 = 4;
    fragment->xposi.l = 100 << 16;
    fragment->yposi.l = 200 << 16;
    actwk[0].yposi.w.h = 200;
    kowasi5_work_get(fragment)->x_velocity = 2 << 16;
    kowasi5_work_get(fragment)->y_velocity = -1 << 16;

    kowasi(fragment);

    TEST_ASSERT_EQ_INT(ctx, 102, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)((-1 << 16) + 16384),
                       kowasi5_work_get(fragment)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_kowasi5_state();
    fragment->r_no0 = 4;
    fragment->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;

    kowasi(fragment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_init_sets_r5_fields_and_waits(&ctx);
    test_wait_collision_spawns_three_fragments(&ctx);
    test_wait_negative_player_speed_flips_fragment_x_speed(&ctx);
    test_wait_allocation_failure_keeps_later_fragments_unwritten(&ctx);
    test_down_moves_or_frames_out_by_player_distance(&ctx);
TEST_MAIN_END

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
static sprite_status *actwkchk_queue[8];
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

#include "src/r5/hasira5.c"

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

static void reset_hasira5_state(void) {
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

static void test_init_sets_fields_and_waits_without_collision(
    test_context *ctx) {
    sprite_status *pillar = &actwk[3];

    reset_hasira5_state();

    hasira5(pillar);

    TEST_ASSERT_EQ_INT(ctx, 2, pillar->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, pillar->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, pillar->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, pillar->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, pillar->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 48, pillar->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 238, pillar->colino);
    TEST_ASSERT_EQ_INT(ctx, 17515, pillar->sproffset);
    TEST_ASSERT_TRUE(ctx, pillar->patbase == pat_hasira5);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == pillar);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == pillar);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == pillar);
}

static void test_collision_spawns_all_eight_fragments_and_drops_main(
    test_context *ctx) {
    sprite_status *pillar = &actwk[3];

    reset_hasira5_state();
    pillar->actno = 44;
    pillar->r_no0 = 2;
    pillar->actflg = 4;
    pillar->sprpri = 4;
    pillar->sproffset = 17515;
    pillar->patbase = pat_hasira5;
    pillar->colino = 238;
    pillar->colicnt = 1;
    pillar->xposi.l = 100 << 16;
    pillar->yposi.l = 200 << 16;
    actwk[0].xspeed.w = 120;
    actwk[0].mspeed.w = -80;
    actwk[0].yposi.w.h = 200;
    hitchk_result = 1;
    for (int i = 0; i < 7; ++i) {
        queue_actor(&actwk[40 + i]);
    }

    hasira5(pillar);

    TEST_ASSERT_EQ_INT(ctx, 4, pillar->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, pillar->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, pillar->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 60, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -40, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == pillar);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 176, soundset_request);
    TEST_ASSERT_EQ_INT(ctx, 7, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 2, pillar->patno);
    TEST_ASSERT_EQ_INT(ctx, 16, pillar->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, pillar->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, pillar->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, (92 << 16) - 77101, pillar->xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (176 << 16) - 131072 + 16384, pillar->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, -77101, hasira5_work_get(pillar)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, -114688, hasira5_work_get(pillar)->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 44, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 176, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 77101, hasira5_work_get(&actwk[40])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, -131072, hasira5_work_get(&actwk[40])->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[41].patno);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[41].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, actwk[41].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -163840, hasira5_work_get(&actwk[41])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, -98304, hasira5_work_get(&actwk[41])->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 7, actwk[45].patno);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[45].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 224, actwk[45].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -163840, hasira5_work_get(&actwk[45])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 65536, hasira5_work_get(&actwk[45])->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 5, actwk[46].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[46].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 224, actwk[46].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 163840, hasira5_work_get(&actwk[46])->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 65536, hasira5_work_get(&actwk[46])->y_velocity);

    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == pillar);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_collision_allocation_failure_keeps_later_fragments_clear(
    test_context *ctx) {
    sprite_status *pillar = &actwk[3];

    reset_hasira5_state();
    pillar->r_no0 = 2;
    pillar->colicnt = 1;
    pillar->xposi.l = 100 << 16;
    pillar->yposi.l = 200 << 16;
    actwk[0].xspeed.w = 100;
    actwk[0].mspeed.w = 100;
    actwk[0].yposi.w.h = 200;

    hasira5(pillar);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, pillar->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_down_moves_or_frames_out_by_player_distance(test_context *ctx) {
    sprite_status *fragment = &actwk[3];

    reset_hasira5_state();
    fragment->r_no0 = 4;
    fragment->xposi.l = 100 << 16;
    fragment->yposi.l = 200 << 16;
    actwk[0].yposi.w.h = 200;
    hasira5_work_get(fragment)->x_velocity = 2 << 16;
    hasira5_work_get(fragment)->y_velocity = -1 << 16;

    hasira5(fragment);

    TEST_ASSERT_EQ_INT(ctx, 102, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)((-1 << 16) + 16384),
                       hasira5_work_get(fragment)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_hasira5_state();
    fragment->r_no0 = 4;
    fragment->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;

    hasira5(fragment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_init_sets_fields_and_waits_without_collision(&ctx);
    test_collision_spawns_all_eight_fragments_and_drops_main(&ctx);
    test_collision_allocation_failure_keeps_later_fragments_clear(&ctx);
    test_down_moves_or_frames_out_by_player_distance(&ctx);
TEST_MAIN_END

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
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[4];

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r1/kowasi1.c"

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
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    return (Sint32)((Uint32)actor->actfree[offset] |
                    ((Uint32)actor->actfree[offset + 1] << 8) |
                    ((Uint32)actor->actfree[offset + 2] << 16) |
                    ((Uint32)actor->actfree[offset + 3] << 24));
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void reset_kowasi1_state(void) {
    memset(actwk, 0, sizeof(actwk));
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
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void test_kowasi1_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kowasi[0] == &spr_kowasi00);
    TEST_ASSERT_TRUE(ctx, pat_kowasi[13] == &spr_kowasi0d);
    TEST_ASSERT_EQ_INT(ctx, -16, spr_kowasi00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, spr_kowasi00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 434, spr_kowasi00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, spr_kowasi08.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 447, spr_kowasi0d.spra[0].index);
}

static void test_m_init_sets_fields_and_waits_without_collision(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi1_state();
    wall->userflag.b.h = 5;

    m_init(wall);

    TEST_ASSERT_EQ_INT(ctx, 2, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, wall->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, wall->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 239, wall->colino);
    TEST_ASSERT_EQ_INT(ctx, 50366, wall->sproffset);
    TEST_ASSERT_TRUE(ctx, wall->patbase == pat_kowasi);
    TEST_ASSERT_EQ_INT(ctx, 5, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == wall);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == wall);
}

static void test_m_wait_collision_spawns_all_fragments(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi1_state();
    wall->actno = 53;
    wall->r_no0 = 2;
    wall->actflg = 4;
    wall->sprpri = 1;
    wall->sproffset = 50366;
    wall->patbase = pat_kowasi;
    wall->userflag.b.h = 0;
    wall->colino = 239;
    wall->colicnt = 1;
    wall->xposi.w.h = 100;
    wall->yposi.w.h = 200;
    actwk[0].xspeed.w = 100;
    actwk[0].yspeed.w = -32;
    actwk[0].yposi.w.h = 200;
    for (int i = 0; i < 5; ++i) {
        queue_actor(&actwk[40 + i]);
    }

    m_wait(wall);

    TEST_ASSERT_EQ_INT(ctx, 4, wall->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, wall->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 176, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 89, wall->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 182, wall->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -194180, actfree_long(wall, 0));

    TEST_ASSERT_EQ_INT(ctx, 53, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 9, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -252434, actfree_long(&actwk[40], 0));

    TEST_ASSERT_EQ_INT(ctx, 12, actwk[42].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[42].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 184, actwk[42].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 13, actwk[43].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[43].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[43].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[44].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[44].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actwk[44].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == wall);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_m_wait_negative_player_speed_flips_fragments(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi1_state();
    wall->r_no0 = 2;
    wall->colicnt = 1;
    wall->userflag.b.h = 1;
    wall->xposi.w.h = 100;
    wall->yposi.w.h = 200;
    actwk[0].xspeed.w = -100;
    for (int i = 0; i < 5; ++i) {
        queue_actor(&actwk[40 + i]);
    }

    m_wait(wall);

    TEST_ASSERT_EQ_INT(ctx, -50, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 194180, actfree_long(wall, 0));
    TEST_ASSERT_EQ_INT(ctx, 8, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[42].patno);
    TEST_ASSERT_EQ_INT(ctx, 11, actwk[43].patno);
}

static void test_m_wait_allocation_failure_keeps_existing_fragments(
    test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi1_state();
    wall->r_no0 = 2;
    wall->colicnt = 1;
    wall->userflag.b.h = 0;
    wall->xposi.w.h = 100;
    wall->yposi.w.h = 200;
    actwk[0].xspeed.w = 100;

    m_wait(wall);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 8, wall->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
}

static void test_m_down_actions_or_frameouts_by_player_distance(
    test_context *ctx) {
    sprite_status *fragment = &actwk[3];

    reset_kowasi1_state();
    fragment->xposi.l = 100 << 16;
    fragment->yposi.l = 200 << 16;
    actwk[0].yposi.w.h = 200;
    set_actfree_long(fragment, 0, 2 << 16);
    set_actfree_long(fragment, 4, -1 << 16);

    m_down(fragment);

    TEST_ASSERT_EQ_INT(ctx, 102, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)((-1 << 16) + 16384),
                       actfree_long(fragment, 4));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_kowasi1_state();
    fragment->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;

    m_down(fragment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_kowasi_dispatches_wait_and_down(test_context *ctx) {
    sprite_status *wall = &actwk[3];

    reset_kowasi1_state();
    wall->r_no0 = 2;
    kowasi(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_kowasi1_state();
    wall->r_no0 = 4;
    wall->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;
    kowasi(wall);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

TEST_MAIN_BEGIN;
    test_kowasi1_patterns_capture_literal_data(&ctx);
    test_m_init_sets_fields_and_waits_without_collision(&ctx);
    test_m_wait_collision_spawns_all_fragments(&ctx);
    test_m_wait_negative_player_speed_flips_fragments(&ctx);
    test_m_wait_allocation_failure_keeps_existing_fragments(&ctx);
    test_m_down_actions_or_frameouts_by_player_distance(&ctx);
    test_kowasi_dispatches_wait_and_down(&ctx);
TEST_MAIN_END

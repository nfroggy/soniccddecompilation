#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actors[8];
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static Sint16 ridechk_result;
static int ridechk_count;
static sprite_status *ridechk_actors[8];
static Sint16 emycol_d2_result;
static int emycol_d2_count;
static sprite_status *emycol_d2_actor;
static Sint16 emycol_d2_xpos;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 emycol_d2(sprite_status *pActwk, Sint16 iXposi);

#include "src/r6/seesaw6.c"

void actionsub(sprite_status *pActwk) {
    if (actionsub_count < (int)(sizeof(actionsub_actors) /
                                sizeof(actionsub_actors[0]))) {
        actionsub_actors[actionsub_count] = pActwk;
    }
    ++actionsub_count;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pPlayerwk;
    if (ridechk_count < (int)(sizeof(ridechk_actors) /
                              sizeof(ridechk_actors[0]))) {
        ridechk_actors[ridechk_count] = pActwk;
    }
    ++ridechk_count;
    return ridechk_result;
}

Sint16 emycol_d2(sprite_status *pActwk, Sint16 iXposi) {
    ++emycol_d2_count;
    emycol_d2_actor = pActwk;
    emycol_d2_xpos = iXposi;
    return emycol_d2_result;
}

static void reset_seesaw6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    memset(actionsub_actors, 0, sizeof(actionsub_actors));
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_result = 0;
    ridechk_count = 0;
    memset(ridechk_actors, 0, sizeof(ridechk_actors));
    emycol_d2_result = 1;
    emycol_d2_count = 0;
    emycol_d2_actor = 0;
    emycol_d2_xpos = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_m_ini_s_copies_visible_parent_state(test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *slave_actor = &actwk[9];

    reset_seesaw6_state();
    parent->actno = 44;
    parent->actflg = 7;
    parent->sprpri = 3;
    parent->sproffset = 952;
    parent->patbase = pat_seesaw6;
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;

    m_ini_s(parent, slave_actor);

    TEST_ASSERT_EQ_INT(ctx, 44, slave_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 7, slave_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, slave_actor->sprpri);
    TEST_ASSERT_TRUE(ctx, slave_actor->patbase == pat_seesaw6);
    TEST_ASSERT_EQ_INT(ctx, 300, slave_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, slave_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1, slave_actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 16, slave_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, slave_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, slave_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9, slave_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, seesaw6_get_work(slave_actor)->parent_index);
    TEST_ASSERT_EQ_INT(ctx, 120, seesaw6_get_work(parent)->timer);
}

static void test_m_init_allocates_two_slaves_and_positions_them(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *left = &actwk[10];
    sprite_status *right = &actwk[11];

    reset_seesaw6_state();
    parent->actno = 44;
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;
    queue_actor(left);
    queue_actor(right);

    m_init(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, parent->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, parent->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, parent->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, parent->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, parent->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 952, parent->sproffset);
    TEST_ASSERT_TRUE(ctx, parent->patbase == pat_seesaw6);
    TEST_ASSERT_EQ_INT(ctx, 10, seesaw6_get_work(parent)->left_slave_index);
    TEST_ASSERT_EQ_INT(ctx, 11, seesaw6_get_work(parent)->right_slave_index);
    TEST_ASSERT_EQ_INT(ctx, 260, left->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 376, left->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 340, right->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 424, right->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, right->actflg & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, right->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_m_init_frames_out_when_slave_allocation_fails(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *left = &actwk[10];

    reset_seesaw6_state();
    m_init(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);

    reset_seesaw6_state();
    queue_actor(left);

    m_init(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
}

static void test_m_stay_counts_down_and_starts_fall(test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_seesaw6_state();
    seesaw6_get_work(parent)->timer = 61;

    m_stay(parent);

    TEST_ASSERT_EQ_INT(ctx, 60, seesaw6_get_work(parent)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->yspeed.w);

    seesaw6_get_work(parent)->timer = 0;

    m_stay(parent);

    TEST_ASSERT_EQ_INT(ctx, -1, seesaw6_get_work(parent)->timer);
    TEST_ASSERT_EQ_INT(ctx, 256, parent->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, parent->mstno.b.h);
}

static void setup_parent_with_slaves(sprite_status *parent) {
    seesaw6_get_work(parent)->left_slave_index = 10;
    seesaw6_get_work(parent)->right_slave_index = 11;
    actwk[10].xposi.w.h = 260;
    actwk[10].yposi.w.h = 376;
    actwk[10].sprhsize = 16;
    actwk[11].xposi.w.h = 340;
    actwk[11].yposi.w.h = 424;
    actwk[11].sprhsize = 16;
}

static void test_m_fall_moves_all_pieces_and_stops_on_collision(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_seesaw6_state();
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;
    parent->yspeed.w = 512;
    setup_parent_with_slaves(parent);
    emycol_d2_result = 5;

    m_fall(parent);

    TEST_ASSERT_EQ_INT(ctx, 402, parent->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 378, actwk[10].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 426, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, parent->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d2_count);
    TEST_ASSERT_TRUE(ctx, emycol_d2_actor == &actwk[11]);
    TEST_ASSERT_EQ_INT(ctx, 284, emycol_d2_xpos);

    emycol_d2_result = -3;

    m_fall(parent);

    TEST_ASSERT_EQ_INT(ctx, 401, parent->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 377, actwk[10].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 425, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->yspeed.w);
}

static void test_m_up_raises_and_then_swaps_slaves(test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_seesaw6_state();
    parent->r_no0 = 4;
    parent->yposi.w.h = 400;
    parent->yspeed.w = 256;
    parent->mstno.b.h = 2;
    setup_parent_with_slaves(parent);
    seesaw6_get_work(parent)->timer = 1;

    m_up(parent);

    TEST_ASSERT_EQ_INT(ctx, 388, parent->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, seesaw6_get_work(parent)->timer);
    TEST_ASSERT_EQ_INT(ctx, 4, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actors[0] == &actwk[11]);
    TEST_ASSERT_TRUE(ctx, actionsub_actors[1] == &actwk[10]);

    m_up(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 120, seesaw6_get_work(parent)->timer);
    TEST_ASSERT_EQ_INT(ctx, 11, seesaw6_get_work(parent)->left_slave_index);
    TEST_ASSERT_EQ_INT(ctx, 10, seesaw6_get_work(parent)->right_slave_index);
    TEST_ASSERT_EQ_INT(ctx, 3, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, -1, parent->mstno.b.l);

    reset_seesaw6_state();
    parent->r_no0 = 4;
    parent->mstno.b.h = 4;
    setup_parent_with_slaves(parent);
    seesaw6_get_work(parent)->timer = 0;

    m_up(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, -1, parent->mstno.b.l);
}

static void test_m_move_patches_or_enters_up_state_from_ride(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_seesaw6_state();
    parent->r_no0 = 2;
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;
    setup_parent_with_slaves(parent);
    seesaw6_get_work(parent)->timer = 120;
    ridechk_result = 0;

    m_move(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actors[0] == &actwk[11]);
    TEST_ASSERT_TRUE(ctx, ridechk_actors[1] == &actwk[10]);
    TEST_ASSERT_EQ_INT(ctx, 0, seesaw6_get_work(&actwk[10])->pressed);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == parent);
    TEST_ASSERT_TRUE(ctx, patchg_table == seesaw_pchg);

    reset_seesaw6_state();
    parent->r_no0 = 2;
    parent->xposi.w.h = 300;
    parent->yposi.w.h = 400;
    parent->yspeed.w = 512;
    setup_parent_with_slaves(parent);
    emycol_d2_result = 1;

    m_move(parent);

    TEST_ASSERT_EQ_INT(ctx, 402, parent->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d2_count);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_seesaw6_state();
    parent->r_no0 = 2;
    setup_parent_with_slaves(parent);
    ridechk_result = 1;

    m_move(parent);

    TEST_ASSERT_EQ_INT(ctx, 4, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, seesaw6_get_work(parent)->timer);
    TEST_ASSERT_EQ_INT(ctx, 8, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 255, seesaw6_get_work(&actwk[10])->pressed);
}

static void test_slave_frames_out_when_parent_is_not_seesaw(test_context *ctx) {
    sprite_status *slave_actor = &actwk[10];

    reset_seesaw6_state();
    seesaw6_get_work(slave_actor)->parent_index = 4;
    actwk[4].actno = 0;

    slave(slave_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == slave_actor);

    frameout_count = 0;
    actwk[4].actno = 44;

    slave(slave_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_seesaw6_wrapper_dispatches_and_frames(test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *left = &actwk[10];
    sprite_status *right = &actwk[11];

    reset_seesaw6_state();
    parent->actno = 44;
    queue_actor(left);
    queue_actor(right);

    seesaw6(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actors[0] == parent);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == parent);

    reset_seesaw6_state();
    parent->userflag.b.h = -1;
    seesaw6_get_work(parent)->parent_index = 4;
    actwk[4].actno = 0;

    seesaw6(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

TEST_MAIN_BEGIN;
test_m_ini_s_copies_visible_parent_state(&ctx);
test_m_init_allocates_two_slaves_and_positions_them(&ctx);
test_m_init_frames_out_when_slave_allocation_fails(&ctx);
test_m_stay_counts_down_and_starts_fall(&ctx);
test_m_fall_moves_all_pieces_and_stops_on_collision(&ctx);
test_m_up_raises_and_then_swaps_slaves(&ctx);
test_m_move_patches_or_enters_up_state_from_ride(&ctx);
test_slave_frames_out_when_parent_is_not_seesaw(&ctx);
test_seesaw6_wrapper_dispatches_and_frames(&ctx);
TEST_MAIN_END

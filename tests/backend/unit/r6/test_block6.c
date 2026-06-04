#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

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
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r6/block6.c"

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

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_logs(void) {
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
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
}

static void reset_block6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
    stack_pointer = 0;
}

static void seed_parent(sprite_status *parent, Sint8 table, Sint8 group) {
    parent->actno = 42;
    parent->userflag.b.h = table;
    parent->userflag.b.l = group;
    parent->xposi.l = 0x00100000;
    parent->yposi.l = 0x00200000;
    parent->xspeed.w = 3;
    parent->yspeed.w = -4;
    parent->mspeed.w = 5;
    parent->sproffset = 1234;
    parent->sprpri = 2;
    parent->patno = 3;
    parent->mstno.w = 4;
    parent->patcnt = 5;
    parent->pattim = 6;
    parent->pattimm = 7;
    parent->colino = 8;
    parent->colicnt = 9;
    parent->cddat = 10;
    parent->cdsts = 11;
    parent->r_no1 = 12;
    parent->direc.w = 13;
    main_init(parent);
}

static sprite_status *spawn_from_parent(sprite_status *parent,
                                        sprite_status *slot) {
    queue_actwkchk(slot);
    block6_get_work(parent)->trigger = 255;
    main_move(parent);
    return slot;
}

static void drive_until_rno(test_context *ctx,
                            sprite_status *actor,
                            Uint8 expected_rno) {
    int guard = 256;

    while (actor->r_no0 != expected_rno && guard-- > 0)
        block6(actor);

    TEST_ASSERT_EQ_INT(ctx, expected_rno, actor->r_no0);
}

static void test_block6_parent_init_sets_visible_state_and_callbacks(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_block6_state();
    parent->actno = 42;

    block6(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, parent->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, parent->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, parent->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, parent->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, parent->sprvsize);
    TEST_ASSERT_TRUE(ctx, parent->patbase == pat_block6);
    TEST_ASSERT_EQ_INT(ctx, 255, block6_get_work(parent)->trigger);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == parent);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == parent);
}

static void test_block6_parent_move_waits_when_trigger_is_clear(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_block6_state();
    seed_parent(parent, 0, 2);
    reset_logs();
    block6_get_work(parent)->trigger = 0;

    main_move(parent);

    TEST_ASSERT_EQ_INT(ctx, 0, block6_get_work(parent)->trigger);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_block6_parent_move_spawns_child_with_copied_visible_state(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[7];

    reset_block6_state();
    seed_parent(parent, 0, 6);

    spawn_from_parent(parent, child);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, block6_get_work(parent)->trigger);
    TEST_ASSERT_EQ_INT(ctx, 42, child->actno);
    TEST_ASSERT_EQ_INT(ctx, parent->actflg, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, parent->sproffset, child->sproffset);
    TEST_ASSERT_TRUE(ctx, child->patbase == parent->patbase);
    TEST_ASSERT_EQ_INT(ctx, parent->xposi.l, child->xposi.l);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.l, child->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, parent->xspeed.w, child->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->yspeed.w, child->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->mspeed.w, child->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->sprhsize, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, parent->sprvsize, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, parent->sprhs, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, parent->sprpri + 1, child->sprpri);
    TEST_ASSERT_EQ_INT(ctx, parent->patno, child->patno);
    TEST_ASSERT_EQ_INT(ctx, parent->mstno.w, child->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, parent->patcnt, child->patcnt);
    TEST_ASSERT_EQ_INT(ctx, parent->pattim, child->pattim);
    TEST_ASSERT_EQ_INT(ctx, parent->pattimm, child->pattimm);
    TEST_ASSERT_EQ_INT(ctx, parent->colino, child->colino);
    TEST_ASSERT_EQ_INT(ctx, parent->colicnt, child->colicnt);
    TEST_ASSERT_EQ_INT(ctx, parent->cddat, child->cddat);
    TEST_ASSERT_EQ_INT(ctx, parent->cdsts, child->cdsts);
    TEST_ASSERT_EQ_INT(ctx, 0, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, parent->r_no1, child->r_no1);
    TEST_ASSERT_EQ_INT(ctx, parent->direc.w, child->direc.w);
    TEST_ASSERT_EQ_INT(ctx, -1, child->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, parent->userflag.b.l, child->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_block6_parent_move_frameouts_when_no_child_slot(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];

    reset_block6_state();
    seed_parent(parent, 0, 2);

    main_move(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
}

static void test_block6_parent_sequence_wraps_after_four_entries(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *children[5] = {
        &actwk[10], &actwk[11], &actwk[12], &actwk[13], &actwk[14]};
    Sint32 expected_x[5] = {0, 32768, 0, -32768, 0};
    Sint32 expected_y[5] = {-32768, 0, 32768, 0, -32768};
    int i;

    reset_block6_state();
    seed_parent(parent, 0, 2);
    for (i = 0; i < 5; ++i)
        spawn_from_parent(parent, children[i]);

    reset_logs();
    for (i = 0; i < 5; ++i) {
        Sint32 start_x = children[i]->xposi.l;
        Sint32 start_y = children[i]->yposi.l;

        block6(children[i]);

        TEST_ASSERT_EQ_INT(ctx, 2, children[i]->r_no0);
        TEST_ASSERT_EQ_INT(ctx, start_x + expected_x[i], children[i]->xposi.l);
        TEST_ASSERT_EQ_INT(ctx, start_y + expected_y[i], children[i]->yposi.l);
    }
}

static void test_block6_sub_frameouts_for_invalid_or_mismatched_parent(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[7];

    reset_block6_state();
    seed_parent(parent, 0, 3);
    spawn_from_parent(parent, child);
    reset_logs();

    parent->actno = 0;
    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_block6_state();
    parent = &actwk[4];
    child = &actwk[7];
    seed_parent(parent, 0, 3);
    spawn_from_parent(parent, child);
    reset_logs();

    parent->userflag.b.l = 4;
    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_block6_sub_go_sets_vertical_speed_for_table_two(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child0 = &actwk[7];
    sprite_status *child1 = &actwk[8];
    Sint32 start_y;

    reset_block6_state();
    seed_parent(parent, 2, 1);
    spawn_from_parent(parent, child0);
    spawn_from_parent(parent, child1);

    start_y = child0->yposi.l;
    block6(child0);
    TEST_ASSERT_EQ_INT(ctx, 2, child0->r_no0);
    TEST_ASSERT_EQ_INT(ctx, start_y - 32768, child0->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, -1, child0->yspeed.w);

    start_y = child1->yposi.l;
    block6(child1);
    TEST_ASSERT_EQ_INT(ctx, 2, child1->r_no0);
    TEST_ASSERT_EQ_INT(ctx, start_y + 32768, child1->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 1, child1->yspeed.w);
}

static void test_block6_table_two_stop_uses_immediate_child_spawn(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child0 = &actwk[7];
    sprite_status *child1 = &actwk[8];

    reset_block6_state();
    seed_parent(parent, 2, 1);
    spawn_from_parent(parent, child0);

    drive_until_rno(ctx, child0, 4);
    block6(child0);
    TEST_ASSERT_EQ_INT(ctx, 6, child0->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child0->yspeed.w);

    queue_actwkchk(child1);
    block6(child0);

    TEST_ASSERT_EQ_INT(ctx, 8, child0->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 42, child1->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, child1->userflag.b.h);
}

static void test_block6_child_lifecycle_reaches_back_stop_and_die(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child0 = &actwk[7];
    sprite_status *child1 = &actwk[8];
    sprite_status *child2 = &actwk[9];
    Uint8 child1_rno_before_stop11;
    Sint32 child2_y_after_forward;
    Sint32 child2_y_after_back;

    reset_block6_state();
    seed_parent(parent, 0, 5);
    spawn_from_parent(parent, child0);

    drive_until_rno(ctx, child0, 4);
    block6(child0);
    TEST_ASSERT_EQ_INT(ctx, 6, child0->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child0->yspeed.w);

    queue_actwkchk(child1);
    drive_until_rno(ctx, child0, 8);
    block6(child0);
    TEST_ASSERT_EQ_INT(ctx, 8, child0->r_no0);

    drive_until_rno(ctx, child1, 4);
    block6(child1);
    queue_actwkchk(child2);
    drive_until_rno(ctx, child1, 8);

    drive_until_rno(ctx, child2, 4);
    child2_y_after_forward = child2->yposi.l;
    TEST_ASSERT_EQ_INT(ctx, -4194304, child2_y_after_forward);
    block6(child2);
    drive_until_rno(ctx, child2, 10);
    TEST_ASSERT_EQ_INT(ctx, 255, block6_get_work(parent)->trigger);

    block6(child2);
    TEST_ASSERT_EQ_INT(ctx, 12, child2->r_no0);
    drive_until_rno(ctx, child2, 14);
    child2_y_after_back = child2->yposi.l;
    TEST_ASSERT_EQ_INT(ctx, -2097152, child2_y_after_back);
    TEST_ASSERT_TRUE(ctx, child2_y_after_forward != child2_y_after_back);

    block6(child2);
    TEST_ASSERT_EQ_INT(ctx, 16, child2->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child2->yspeed.w);
    child1_rno_before_stop11 = child1->r_no0;

    drive_until_rno(ctx, child2, 18);
    TEST_ASSERT_EQ_INT(ctx, child1_rno_before_stop11 + 2, child1->r_no0);

    reset_logs();
    hitchk_result = 1;
    block6(child2);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == child2);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == child2);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child2);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_block6_stop11_leaves_non_slave_parent_rno_alone(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[7];
    Uint8 parent_rno_before;

    reset_block6_state();
    seed_parent(parent, 0, 2);
    spawn_from_parent(parent, child);
    parent_rno_before = parent->r_no0;
    child->r_no0 = 16;

    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 18, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, parent_rno_before, parent->r_no0);
}

static void test_block6_table_two_stop1_uses_immediate_timer(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[7];

    reset_block6_state();
    seed_parent(parent, 2, 1);
    spawn_from_parent(parent, child);
    child->yspeed.w = 99;
    child->r_no0 = 14;

    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 16, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child->yspeed.w);

    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 18, child->r_no0);
}

static void test_block6_die_without_hitchk_skips_ride_clear(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[7];

    reset_block6_state();
    seed_parent(parent, 0, 2);
    spawn_from_parent(parent, child);
    child->r_no0 = 18;
    reset_logs();
    hitchk_result = 0;

    block6(child);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_block6_parent_init_sets_visible_state_and_callbacks(&ctx);
    test_block6_parent_move_waits_when_trigger_is_clear(&ctx);
    test_block6_parent_move_spawns_child_with_copied_visible_state(&ctx);
    test_block6_parent_move_frameouts_when_no_child_slot(&ctx);
    test_block6_parent_sequence_wraps_after_four_entries(&ctx);
    test_block6_sub_frameouts_for_invalid_or_mismatched_parent(&ctx);
    test_block6_sub_go_sets_vertical_speed_for_table_two(&ctx);
    test_block6_table_two_stop_uses_immediate_child_spawn(&ctx);
    test_block6_child_lifecycle_reaches_back_stop_and_die(&ctx);
    test_block6_stop11_leaves_non_slave_parent_rno_alone(&ctx);
    test_block6_table_two_stop1_uses_immediate_timer(&ctx);
    test_block6_die_without_hitchk_skips_ride_clear(&ctx);
TEST_MAIN_END

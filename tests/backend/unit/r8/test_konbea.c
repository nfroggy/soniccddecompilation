#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r8/konbea.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    pActwk->actno = 0;
    return 0;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_konbea_word(sprite_status *actor, int offset, Sint16 value) {
    konbea_work *work = konbea_get_work(actor);

    if (actor->userflag.b.h) {
        switch (offset) {
        case 2:
            work->moving.origin_x = value;
            break;
        case 4:
            work->moving.origin_y = value;
            break;
        }
    } else {
        switch (offset) {
        case 4:
            work->straight.origin_x = value;
            break;
        case 6:
            work->straight.origin_y = value;
            break;
        }
    }
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_konbea_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void assert_common_platform_fields(test_context *ctx,
                                          sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 844, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_konbea);
}

static void test_konbea_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 0, konbea8tbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 26, konbea8tbl[1]);
    TEST_ASSERT_EQ_INT(ctx, -89, konbea8tbl[178]);
    TEST_ASSERT_EQ_INT(ctx, -19, konbea8tbl[179]);
    TEST_ASSERT_EQ_INT(ctx, 60, a_tbl_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, a_tbl_00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, a_tbl_01[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, a_tbl_01[3]);
    TEST_ASSERT_TRUE(ctx, a_tbl0[0] == a_tbl_00);
    TEST_ASSERT_TRUE(ctx, a_tbl0[1] == a_tbl_01);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_00[13]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg_00);
    TEST_ASSERT_TRUE(ctx, pat_konbea[0] == &pat_00);
    TEST_ASSERT_TRUE(ctx, pat_konbea[6] == &pat_00);
    TEST_ASSERT_EQ_INT(ctx, 408, pat_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 413, pat_05.spra[0].index);
}

static void test_konbea_a_init_single_platform(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea_state();
    actor->actno = 42;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    assert_common_platform_fields(ctx, actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_konbea_a_init_spawns_child_and_child_can_run(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    assert_common_platform_fields(ctx, actor);
    assert_common_platform_fields(ctx, child);
    TEST_ASSERT_EQ_INT(ctx, 42, child->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, child->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 132, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, child->yposi.w.h);

    reset_logs();
    ridechk_result = 1;

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == child);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, konbea_get_work(child)->straight.riding_flag);
    TEST_ASSERT_EQ_INT(ctx, 4, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_konbea_a_child_frames_out_when_source_parent_is_gone(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);
    konbea(actor);

    actor->actno = 0;
    reset_logs();

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_konbea_a_child_frames_out_when_origin_words_differ(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);
    konbea(actor);

    set_konbea_word(child, 4, 101);
    reset_logs();

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_konbea_state();
    actwk[0].actno = 42;
    actor = &actwk[3];
    child = &actwk[20];
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);
    konbea(actor);

    set_konbea_word(child, 6, 201);
    reset_logs();

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_konbea_a_init_frames_out_on_child_allocation_failure(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_konbea_a_stop_and_move_paths(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    ridechk_result = 1;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 255, konbea_get_work(actor)->straight.riding_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    actor->r_no0 = 6;
    actor->patno = 0;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);

    reset_logs();
    actor->r_no0 = 6;
    actor->patno = 3;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
}

static void test_konbea_a_stop_clears_standing_flag_when_not_riding(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    konbea_get_work(actor)->straight.riding_flag = 255;
    ridechk_result = 0;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, konbea_get_work(actor)->straight.riding_flag);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_konbea_a_stop_countdown_clears_ride_and_enters_move(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    reset_konbea_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    ridechk_result = 1;

    for (i = 0; i < 120; ++i) {
        konbea(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 120, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 120, actionsub_count);
}

static void test_konbea_b_init_spawns_children_and_moves_child(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *children[4] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23]};
    Sint16 old_x;
    Sint16 expected_x;
    Sint16 expected_y;

    reset_konbea_state();
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (int i = 0; i < 4; ++i) {
        queue_actor(children[i]);
    }

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    assert_common_platform_fields(ctx, actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 42, children[i]->actno);
        TEST_ASSERT_EQ_INT(ctx, 1, children[i]->userflag.b.h);
        TEST_ASSERT_EQ_INT(ctx, -1, children[i]->userflag.b.l);
        TEST_ASSERT_EQ_INT(ctx, 100, children[i]->xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 200, children[i]->yposi.w.h);
        assert_common_platform_fields(ctx, children[i]);
    }

    reset_logs();
    old_x = children[0]->xposi.w.h;
    expected_x = 100 + konbea8tbl[89 * 2];
    expected_y = 200 + konbea8tbl[89 * 2 + 1];

    konbea(children[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, expected_x, children[0]->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, expected_y, children[0]->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (expected_x - old_x) << 8, children[0]->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4, children[0]->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_konbea_b_child_frames_out_when_source_parent_is_gone(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *children[4] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23]};

    reset_konbea_state();
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (int i = 0; i < 4; ++i) {
        queue_actor(children[i]);
    }
    konbea(actor);

    actor->actno = 0;
    reset_logs();

    konbea(children[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == children[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_konbea_b_child_frames_out_when_origin_words_differ(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *children[4] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23]};
    sprite_status *child = children[0];

    reset_konbea_state();
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (int i = 0; i < 4; ++i) {
        queue_actor(children[i]);
    }
    konbea(actor);

    set_konbea_word(child, 2, 101);
    reset_logs();

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_konbea_state();
    actor = &actwk[3];
    child = &actwk[20];
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (int i = 0; i < 4; ++i) {
        queue_actor(children[i]);
    }
    konbea(actor);

    set_konbea_word(child, 4, 201);
    reset_logs();

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_konbea_b_init_frames_out_on_child_allocation_failure(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea_state();
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_konbea_b_stop_roll_and_table_counter_wrap_paths(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *children[4] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23]};
    sprite_status *child = children[0];
    int i;

    reset_konbea_state();
    actor->actno = 42;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (i = 0; i < 4; ++i) {
        queue_actor(children[i]);
    }
    konbea(actor);
    reset_logs();
    ridechk_result = 1;

    for (i = 0; i < 256 && child->r_no0 != 6; ++i) {
        konbea(child);
    }

    TEST_ASSERT_EQ_INT(ctx, 6, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == child);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);

    reset_logs();
    child->patno = 0;
    child->patcnt = 0;

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 6, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child->patno);
    TEST_ASSERT_EQ_INT(ctx, 128, child->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    for (i = 0; i < 216; ++i) {
        konbea(child);
    }

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, child->patno);
    TEST_ASSERT_EQ_INT(ctx, 216, actionsub_count);
}

TEST_MAIN_BEGIN;
test_konbea_tables_capture_literal_data(&ctx);
test_konbea_a_init_single_platform(&ctx);
test_konbea_a_init_spawns_child_and_child_can_run(&ctx);
test_konbea_a_child_frames_out_when_source_parent_is_gone(&ctx);
test_konbea_a_child_frames_out_when_origin_words_differ(&ctx);
test_konbea_a_init_frames_out_on_child_allocation_failure(&ctx);
test_konbea_a_stop_and_move_paths(&ctx);
test_konbea_a_stop_clears_standing_flag_when_not_riding(&ctx);
test_konbea_a_stop_countdown_clears_ride_and_enters_move(&ctx);
test_konbea_b_init_spawns_children_and_moves_child(&ctx);
test_konbea_b_child_frames_out_when_source_parent_is_gone(&ctx);
test_konbea_b_child_frames_out_when_origin_words_differ(&ctx);
test_konbea_b_init_frames_out_on_child_allocation_failure(&ctx);
test_konbea_b_stop_roll_and_table_counter_wrap_paths(&ctx);
TEST_MAIN_END

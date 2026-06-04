#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actors[16];
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 emycol_d_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/legmeca8.c"

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
    pActwk->actno = 0;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
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

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = 1024;
    *cos = -2048;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_logs(void) {
    actionsub_count = 0;
    memset(actionsub_actors, 0, sizeof(actionsub_actors));
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    emycol_d_result = 0;
}

static void reset_legmeca8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_leg_chain(sprite_status *children[8]) {
    for (int i = 0; i < 8; ++i) {
        queue_actor(children[i]);
    }
}

static void initialize_legmeca(sprite_status *master,
                               sprite_status *children[8]) {
    master->actno = 38;
    master->xposi.w.h = 100;
    master->yposi.w.h = 200;
    queue_leg_chain(children);
    legmeca(master);
}

static void assert_link_sprite_fields(test_context *ctx,
                                      sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 38, actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 792, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_legmeca);
}

static void assert_leg_sprite_fields(test_context *ctx,
                                     sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 41, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 792, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_legmeca);
}

static void test_legmeca8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_legmeca[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_legmeca[1] == &pat01);
    TEST_ASSERT_EQ_INT(ctx, 2, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -4, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -40, pat00.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, 463, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 464, pat00.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, 465, pat01.spra[0].index);
}

static void test_legmeca8_master_init_spawns_chain_and_frames_master(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, legmeca8_get_work(master)->routine);
    TEST_ASSERT_EQ_INT(ctx, 12, legmeca8_get_work(master)->roll_start_timer);
    assert_leg_sprite_fields(ctx, master);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 9, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);

    for (int i = 0; i < 7; ++i) {
        assert_link_sprite_fields(ctx, children[i]);
        TEST_ASSERT_EQ_INT(ctx, 108 + i * 8, children[i]->xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 200, children[i]->yposi.w.h);
    }
    assert_leg_sprite_fields(ctx, children[7]);
    TEST_ASSERT_EQ_INT(ctx, 164, children[7]->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, children[7]->yposi.w.h);
}

static void test_legmeca8_allocation_failure_frames_out_master(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *first_child = &actwk[20];

    reset_legmeca8_state();
    master->actno = 38;
    master->xposi.w.h = 100;
    master->yposi.w.h = 200;
    queue_actor(first_child);

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 0, master->actno);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_legmeca8_slave_frameout_depends_on_source_parent(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);

    reset_logs();
    legmeca(children[0]);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 38, children[0]->actno);

    master->actno = 0;

    legmeca(children[0]);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == children[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, children[0]->actno);
}

static void test_legmeca8_ride_starts_roll_on_master_and_outer_leg(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    reset_logs();
    ridechk_result = 1;

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 11, legmeca8_get_work(master)->roll_start_timer);
    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, children[7]->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == master);
}

static void test_legmeca8_outer_leg_roll_entry_sets_subpixel_origin(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    children[7]->r_no0 = 2;
    reset_logs();

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 4, children[7]->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -32768, children[7]->xposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, -32768, children[7]->yposi.w.l);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == master);
}

static void test_legmeca8_center_roll_updates_chain_with_sine_step(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    children[7]->r_no0 = 2;
    legmeca(master);
    reset_logs();
    legmeca8_get_work(master)->rotation = 0x4000;

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 66, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 9, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 100, children[0]->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 232, children[0]->yposi.w.h);
}

static void test_legmeca8_master_reset_modes_switch_motion_sets(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    reset_logs();
    legmeca8_get_work(master)->routine = 4;

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 6, legmeca8_get_work(master)->routine);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actionsub_count);

    reset_logs();
    legmeca8_get_work(master)->routine = 8;

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 2, legmeca8_get_work(master)->routine);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 9, actionsub_count);
}

static void test_legmeca8_radius_roll_lands_and_resets_legs(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};
    int i;

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    children[7]->r_no0 = 2;
    legmeca(master);
    reset_logs();
    master->r_no0 = 2;
    children[7]->r_no0 = 4;
    ridechk_result = 1;
    emycol_d_result = -3;

    for (i = 0; i < 32; ++i) {
        legmeca(master);
    }

    TEST_ASSERT_EQ_INT(ctx, 4, legmeca8_get_work(master)->routine);
    TEST_ASSERT_EQ_INT(ctx, 0, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, children[7]->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, master->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, children[7]->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 455, master->yposi.w.h);
}

static void test_legmeca8_radius_roll_keeps_single_ride_check_when_clear(
    test_context *ctx) {
    sprite_status *master = &actwk[3];
    sprite_status *children[8] = {&actwk[20], &actwk[21], &actwk[22],
                                  &actwk[23], &actwk[24], &actwk[25],
                                  &actwk[26], &actwk[27]};

    reset_legmeca8_state();
    initialize_legmeca(master, children);
    reset_logs();
    master->r_no0 = 2;
    ridechk_result = 0;

    legmeca(master);

    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 41, master->sprvsize);
}

TEST_MAIN_BEGIN;
test_legmeca8_patterns_capture_literal_data(&ctx);
test_legmeca8_master_init_spawns_chain_and_frames_master(&ctx);
test_legmeca8_allocation_failure_frames_out_master(&ctx);
test_legmeca8_slave_frameout_depends_on_source_parent(&ctx);
test_legmeca8_ride_starts_roll_on_master_and_outer_leg(&ctx);
test_legmeca8_outer_leg_roll_entry_sets_subpixel_origin(&ctx);
test_legmeca8_center_roll_updates_chain_with_sine_step(&ctx);
test_legmeca8_master_reset_modes_switch_motion_sets(&ctx);
test_legmeca8_radius_roll_lands_and_resets_legs(&ctx);
test_legmeca8_radius_roll_keeps_single_ride_check_when_clear(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
Uint8 generate_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int emycol_d3_count;
static sprite_status *emycol_d3_actor;
static Sint16 emycol_d3_x;
static Sint16 emycol_d3_y;
static Sint16 emycol_d3_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 emycol_d3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);

#include "src/r5/hari5f.c"

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

Sint16 emycol_d3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi) {
    ++emycol_d3_count;
    emycol_d3_actor = pActwk;
    emycol_d3_x = iXposi;
    emycol_d3_y = iYposi;
    return emycol_d3_result;
}

static void reset_hari5f_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    generate_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    emycol_d3_count = 0;
    emycol_d3_actor = 0;
    emycol_d3_x = 0;
    emycol_d3_y = 0;
    emycol_d3_result = 0;
}

static void reset_hari5f_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    emycol_d3_count = 0;
    emycol_d3_actor = 0;
    emycol_d3_x = 0;
    emycol_d3_y = 0;
}

static void assert_tail_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void run_init_case(test_context *ctx, Uint8 time_value,
                          Uint8 generate_value, Uint16 expected_offset) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.xposi.w.h = 200;
    actor.yposi.w.h = 200;
    time_flag = time_value;
    generate_flag = generate_value;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor.sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor.sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor.sprvsize);
    TEST_ASSERT_TRUE(ctx, actor.patbase == pat_hari5f);
    TEST_ASSERT_EQ_INT(ctx, expected_offset, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.colino);
    assert_tail_callbacks(ctx, &actor);
}

static void test_init_sproffset_rules(test_context *ctx) {
    run_init_case(ctx, 0, 0, 17218);
    run_init_case(ctx, 1, 0, 834);
    run_init_case(ctx, 2, 1, 834);
    run_init_case(ctx, 2, 0, 17218);
}

static void test_wait_geometry_controls_drop_arming(test_context *ctx) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 100;
    actwk[0].xposi.w.h = 92;
    actwk[0].yposi.w.h = 124;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 185, actor.colino);
    assert_tail_callbacks(ctx, &actor);

    reset_hari5f_logs();
    actor.r_no0 = 2;
    actor.colino = 0;
    actwk[0].xposi.w.h = 140;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.colino);
    assert_tail_callbacks(ctx, &actor);
}

static void test_fall_without_active_flag_frames_out_below_player(test_context *ctx) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.yposi.w.h = 200;
    actwk[0].yposi.w.h = 100;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d3_count);
    assert_tail_callbacks(ctx, &actor);
}

static void test_fall_on_solid_floor_clears_air_flag_and_accelerates(test_context *ctx) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.actflg = 128;
    actor.xposi.w.h = 33;
    actor.yposi.w.h = 100;
    actor.sprvsize = 24;
    actor.yposi.l = 100 << 16;
    hari5f_work_get(&actor)->y_velocity = 65536;
    emycol_d3_result = 5;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d3_count);
    TEST_ASSERT_TRUE(ctx, emycol_d3_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 33, emycol_d3_x);
    TEST_ASSERT_EQ_INT(ctx, 76, emycol_d3_y);
    TEST_ASSERT_EQ_INT(ctx, 0, hari5f_work_get(&actor)->previous_collision);
    TEST_ASSERT_EQ_INT(ctx, (101 << 16), actor.yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 81920, hari5f_work_get(&actor)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_tail_callbacks(ctx, &actor);
}

static void test_fall_in_open_air_sets_air_flag_and_accelerates(test_context *ctx) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.actflg = 128;
    actor.yposi.w.h = 90;
    actor.sprvsize = 16;
    actor.yposi.l = 90 << 16;
    hari5f_work_get(&actor)->y_velocity = 16384;
    emycol_d3_result = -1;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d3_count);
    TEST_ASSERT_EQ_INT(ctx, 255, hari5f_work_get(&actor)->previous_collision);
    TEST_ASSERT_EQ_INT(ctx, (90 << 16) + 16384, actor.yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 32768, hari5f_work_get(&actor)->y_velocity);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_tail_callbacks(ctx, &actor);
}

static void test_fall_lands_after_air_flag_frames_out_before_motion(test_context *ctx) {
    sprite_status actor;
    reset_hari5f_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.actflg = 128;
    actor.yposi.l = 75 << 16;
    hari5f_work_get(&actor)->previous_collision = 128;
    hari5f_work_get(&actor)->y_velocity = 49152;
    emycol_d3_result = 0;

    hari5f(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d3_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 128, hari5f_work_get(&actor)->previous_collision);
    TEST_ASSERT_EQ_INT(ctx, 75 << 16, actor.yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 49152, hari5f_work_get(&actor)->y_velocity);
    assert_tail_callbacks(ctx, &actor);
}

TEST_MAIN_BEGIN;
    test_init_sproffset_rules(&ctx);
    test_wait_geometry_controls_drop_arming(&ctx);
    test_fall_without_active_flag_frames_out_below_player(&ctx);
    test_fall_on_solid_floor_clears_air_flag_and_accelerates(&ctx);
    test_fall_in_open_air_sets_air_flag_and_accelerates(&ctx);
    test_fall_lands_after_air_flag_frames_out_before_motion(&ctx);
TEST_MAIN_END

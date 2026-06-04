#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_v_posit;
short_union gametimer;
Uint8 time_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);

#include "src/r7/slight7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

static void reset_slight7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(&gametimer, 0, sizeof(gametimer));
    time_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
}

static void test_slight7_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, slight7pat[0] == &slight_pat0);
    TEST_ASSERT_TRUE(ctx, slight7pat[2] == &slight_pat2);
    TEST_ASSERT_TRUE(ctx, slight7patb[0] == &slight_pat4);
    TEST_ASSERT_TRUE(ctx, slight7patb[3] == &slight_pat3);
    TEST_ASSERT_EQ_INT(ctx, 1, slight_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -12, slight_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -112, slight_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 434, slight_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 2, slight_pat2.cnt);
    TEST_ASSERT_EQ_INT(ctx, -64, slight_pat2.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, 437, slight_pat2.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, 440, slight_pat6.spra[0].index);
}

static void test_slight7_initializes_default_and_draws(test_context *ctx) {
    sprite_status *light = &actwk[2];

    reset_slight7_state();
    light->xposi.w.h = 320;
    light->yposi.w.h = 999;
    light->userflag.b.h = 0;
    light->userflag.b.l = 0;
    scra_v_posit.w.h = 20;
    gametimer.w = 0;
    slight7(light);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == light);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == light);
    TEST_ASSERT_EQ_INT(ctx, 320, frameout_s00_x);
}

static void test_slight7_initializes_priority_variants(test_context *ctx) {
    sprite_status *light = &actwk[2];

    reset_slight7_state();
    light->xposi.w.h = 100;
    light->userflag.b.h = 4;
    light->userflag.b.l = 8;
    time_flag = 1;
    slight7_init(light);

    reset_slight7_state();
    light->xposi.w.h = 100;
    light->userflag.b.h = 4;
    light->userflag.b.l = 10;
    time_flag = 2;
    slight7_init(light);
}

static void test_slight7_actionsub_gating(test_context *ctx) {
    sprite_status *light = &actwk[2];

    reset_slight7_state();
    light->r_no0 = 2;
    light->userflag.b.h = 4;
    light->userflag.b.l = 0;
    slight7_work_get(light)->display_phase = 0;
    gametimer.w = 2;
    slight7(light);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_slight7_state();
    light->r_no0 = 2;
    light->userflag.b.h = 4;
    light->userflag.b.l = 0;
    slight7_work_get(light)->display_phase = 2;
    gametimer.w = 6;
    slight7(light);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_slight7_state();
    light->r_no0 = 2;
    light->userflag.b.h = 4;
    light->userflag.b.l = 4;
    slight7_work_get(light)->display_phase = 2;
    gametimer.w = 6;
    slight7(light);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_slight7_state();
    light->r_no0 = 2;
    light->userflag.b.h = 4;
    light->userflag.b.l = 4;
    slight7_work_get(light)->display_phase = 6;
    gametimer.w = 6;
    slight7(light);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_slight7_animation_tables(test_context *ctx) {
    sprite_status *light = &actwk[2];

    reset_slight7_state();
    light->xposi.w.h = 200;
    light->userflag.b.h = 1;
    slight7_init(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    light->userflag.b.h = 2;
    slight7_init(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    light->userflag.b.h = 3;
    slight7_init(light);

    reset_slight7_state();
    light->r_no0 = 2;
    light->xposi.w.h = 200;
    light->userflag.b.h = 0;
    slight7_work_get(light)->anim_index = 2;
    type0(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    slight7_work_get(light)->anim_index = 5;
    type0(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    slight7_work_get(light)->frame_timer = 2;
    slight7_work_get(light)->anim_index = 1;
    type1(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    slight7_work_get(light)->frame_timer = 1;
    slight7_work_get(light)->anim_index = 3;
    type2(light);

    reset_slight7_state();
    light->xposi.w.h = 200;
    slight7_work_get(light)->frame_timer = 1;
    slight7_work_get(light)->anim_index = 2;
    type3(light);
}

static void test_slight7_fixed_types(test_context *ctx) {
    sprite_status *light = &actwk[2];

    reset_slight7_state();
    light->userflag.b.h = 3;
    light->patno = 7;
    type4(light);

    reset_slight7_state();
    light->userflag.b.h = 6;
    type4(light);

    reset_slight7_state();
    light->userflag.b.h = 5;
    type7(light);

    reset_slight7_state();
    light->userflag.b.h = 8;
    slight7_move(light);
}

TEST_MAIN_BEGIN;
test_slight7_patterns_capture_literal_data(&ctx);
test_slight7_initializes_default_and_draws(&ctx);
test_slight7_initializes_priority_variants(&ctx);
test_slight7_actionsub_gating(&ctx);
test_slight7_animation_tables(&ctx);
test_slight7_fixed_types(&ctx);
TEST_MAIN_END

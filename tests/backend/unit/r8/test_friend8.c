#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 time_flag;

static Sint16 friend_suicide_result;
static int friend_suicide_count;
static int sinset_count;
static Uint8 sinset_angle;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static Sint16 emycol_d_result;
static int emycol_d_count;
static sprite_status *emycol_d_actor;

Sint16 friend_suicide(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/friend8.c"

Sint16 friend_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++friend_suicide_count;
    return friend_suicide_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = 32;
    *cos = 64;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

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

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

static void reset_friend8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    stageno.w = 0;
    time_flag = 0;
    friend_suicide_result = 0;
    friend_suicide_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    emycol_d_result = 1;
    emycol_d_count = 0;
    emycol_d_actor = 0;
}

static void reset_friend8_logs(void) {
    friend_suicide_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
}

static void init_hato(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_friend8_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    friend(actor);
    reset_friend8_logs();
}

static void init_sheep(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_friend8_state();
    actor->userflag.b.h = 1;
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    friend(actor);
    reset_friend8_logs();
}

static void test_friend8_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_friend0[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_friend0[1] == &pat01);
    TEST_ASSERT_TRUE(ctx, pat_friend1[0] == &pat02);
    TEST_ASSERT_TRUE(ctx, pat_friend1[1] == &pat03);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 497, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -12, pat03.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat03.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 500, pat03.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg0[0] == p00);
    TEST_ASSERT_TRUE(ctx, pchg0[1] == (char *)p01);
    TEST_ASSERT_EQ_INT(ctx, 3, p00[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, p00[3]);
    TEST_ASSERT_EQ_INT(ctx, 255, p01[29]);
    TEST_ASSERT_EQ_INT(ctx, 1943, tbl0sproffset[0]);
    TEST_ASSERT_EQ_INT(ctx, 986, tbl0sproffset[10]);
}

static void test_friend_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend8_state();
    actor->userflag.b.h = 1;
    friend_suicide_result = -1;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_hato_initializes_visible_fields(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend8_state();
    stageno.b.l = 0;
    time_flag = 0;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend0);
    TEST_ASSERT_EQ_INT(ctx, 1943, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_hato_moves_with_roll_animation_and_origin_callback(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_hato(actor, 100, 200);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 132, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_hato_wraps_direction_during_long_motion(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    int i;

    init_hato(actor, 100, 200);

    for (i = 0; i < 130; ++i) {
        friend(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 130, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 130, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 130, patchg_count);
}

static void test_sheep_initializes_visible_fields(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_friend8_state();
    stageno.b.l = 1;
    time_flag = 2;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 7, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend1);
    TEST_ASSERT_EQ_INT(ctx, 986, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_sheep_moves_in_arc_without_ground_collision(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_sheep(actor, 300, 400);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_sheep_switches_animation_after_gravity_turns_positive(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    int i;

    init_sheep(actor, 300, 400);

    for (i = 0; i < 32; ++i) {
        friend(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 32, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 32, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 32, frameout_s_count);
}

static void test_sheep_lands_and_reverses_direction(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    Uint8 before_actflg;
    Uint8 before_cddat;

    init_sheep(actor, 300, 400);
    before_actflg = actor->actflg;
    before_cddat = actor->cddat;
    emycol_d_result = -3;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, (before_actflg ^ 1), actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, (before_cddat ^ 1), actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_p_end_dispatches_public_callbacks(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_friend8_state();

    p_end(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

TEST_MAIN_BEGIN;
    test_friend8_tables_capture_literal_data(&ctx);
    test_friend_suicide_short_circuits(&ctx);
    test_hato_initializes_visible_fields(&ctx);
    test_hato_moves_with_roll_animation_and_origin_callback(&ctx);
    test_hato_wraps_direction_during_long_motion(&ctx);
    test_sheep_initializes_visible_fields(&ctx);
    test_sheep_moves_in_arc_without_ground_collision(&ctx);
    test_sheep_switches_animation_after_gravity_turns_positive(&ctx);
    test_sheep_lands_and_reverses_direction(&ctx);
    test_p_end_dispatches_public_callbacks(&ctx);
TEST_MAIN_END

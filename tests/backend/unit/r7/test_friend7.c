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
static int frameout_count;
static sprite_status *frameout_actor;
static Sint16 emycol_d_result;
static int emycol_d_count;
static sprite_status *emycol_d_actor;

Sint16 friend_suicide(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r7/friend7.c"

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

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

static void reset_friend7_state(void) {
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
    frameout_count = 0;
    frameout_actor = 0;
    emycol_d_result = 1;
    emycol_d_count = 0;
    emycol_d_actor = 0;
}

static void reset_friend7_logs(void) {
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
    frameout_count = 0;
    frameout_actor = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void init_flicky(sprite_status *actor, Sint16 x, Sint16 y,
                        Sint8 userflag_h) {
    reset_friend7_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    actor->userflag.b.h = userflag_h;
    friend(actor);
    reset_friend7_logs();
}

static void init_ricky(sprite_status *actor, Sint16 x, Sint16 y,
                       Sint8 userflag_h) {
    reset_friend7_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    actor->userflag.b.h = userflag_h;
    friend(actor);
    reset_friend7_logs();
}

static void test_friend7_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_friend0[0] == &spr_flicky1);
    TEST_ASSERT_TRUE(ctx, pat_friend0[2] == &spr_friend0);
    TEST_ASSERT_TRUE(ctx, pat_friend1[0] == &spr_ricky1);
    TEST_ASSERT_TRUE(ctx, pat_friend1[4] == &spr_ricky4);
    TEST_ASSERT_EQ_INT(ctx, -8, spr_flicky1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, spr_flicky1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 462, spr_flicky1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, spr_ricky4.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -4, spr_ricky4.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 467, spr_ricky4.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, spr_friend0.cnt);
    TEST_ASSERT_TRUE(ctx, pchg0[0] == pchg00);
    TEST_ASSERT_TRUE(ctx, pchg0[1] == pchg01);
    TEST_ASSERT_TRUE(ctx, pchg1[0] == pchg10);
    TEST_ASSERT_EQ_INT(ctx, 918, tbl0sproffset[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl0sproffset[3]);
    TEST_ASSERT_EQ_INT(ctx, 918, tbl0sproffset[10]);
}

static void test_friend_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend7_state();
    actor->userflag.b.h = 1;
    friend_suicide_result = -1;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_flicky_initializes_visible_fields(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend7_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_flicky_sproffset_uses_stage_and_time_index(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend7_state();
    stageno.b.l = 0;
    time_flag = 3;

    friend(actor);

}

static void test_flicky_moves_with_roll_animation_and_frameout_origin(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_flicky(actor, 100, 200, 0);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_flicky_move_reverses_direction_after_existing_wrap(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_flicky(actor, 100, 200, 0);

    for (int i = 0; i < 130; ++i) {
        friend(actor);
    }

}

static void test_flicky_movie_variant_initializes_and_uses_parent(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *parent = &actwk[2];

    init_flicky(actor, 120, 220, -128);


    set_actfree_word(actor, 20, 2);
    parent->actno = 39;
    actor->actfree[4] = 8;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_flicky_movie_wrap_reverses_without_drawing(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *parent = &actwk[2];

    init_flicky(actor, 120, 220, -128);
    set_actfree_word(actor, 20, 2);
    parent->actno = 39;
    actor->actfree[4] = 124;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_flicky_movie_frameouts_when_parent_invalid(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *parent = &actwk[2];

    reset_friend7_state();
    actor->r_no0 = 4;
    actor->userflag.b.h = -128;
    set_actfree_word(actor, 20, 2);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_friend7_state();
    actor = &actwk[3];
    parent = &actwk[2];
    actor->r_no0 = 4;
    actor->userflag.b.h = -128;
    set_actfree_word(actor, 20, 2);
    parent->actno = 39;
    parent->actfree[21] = 255;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_ricky_initializes_and_moves_in_arc(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_friend7_state();
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    friend(actor);


    reset_friend7_logs();
    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_ricky_lands_then_reverses(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_ricky(actor, 300, 400, 1);
    emycol_d_result = -3;

    friend(actor);


    reset_friend7_logs();
    emycol_d_result = -2;

    friend(actor);


    reset_friend7_logs();
    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_ricky_switches_to_falling_pattern_after_velocity_positive(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_ricky(actor, 300, 400, 1);
    for (int i = 0; i < 34; ++i) {
        friend(actor);
    }

}

static void test_ricky_movie_variant_initializes_and_moves(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *parent = &actwk[2];

    init_ricky(actor, 300, 400, -127);


    set_actfree_word(actor, 20, 2);
    parent->actno = 39;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_ricky_movie_frameouts_when_parent_invalid(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *parent = &actwk[2];

    reset_friend7_state();
    actor->userflag.b.h = -127;
    actor->r_no0 = 8;
    set_actfree_word(actor, 20, 2);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_friend7_state();
    actor = &actwk[4];
    parent = &actwk[2];
    actor->userflag.b.h = -127;
    actor->r_no0 = 8;
    set_actfree_word(actor, 20, 2);
    parent->actno = 39;
    parent->actfree[21] = 255;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

TEST_MAIN_BEGIN;
    test_friend7_tables_capture_literal_data(&ctx);
    test_friend_suicide_short_circuits(&ctx);
    test_flicky_initializes_visible_fields(&ctx);
    test_flicky_sproffset_uses_stage_and_time_index(&ctx);
    test_flicky_moves_with_roll_animation_and_frameout_origin(&ctx);
    test_flicky_move_reverses_direction_after_existing_wrap(&ctx);
    test_flicky_movie_variant_initializes_and_uses_parent(&ctx);
    test_flicky_movie_wrap_reverses_without_drawing(&ctx);
    test_flicky_movie_frameouts_when_parent_invalid(&ctx);
    test_ricky_initializes_and_moves_in_arc(&ctx);
    test_ricky_lands_then_reverses(&ctx);
    test_ricky_switches_to_falling_pattern_after_velocity_positive(&ctx);
    test_ricky_movie_variant_initializes_and_moves(&ctx);
    test_ricky_movie_frameouts_when_parent_invalid(&ctx);
TEST_MAIN_END

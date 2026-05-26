#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 time_flag;

static int friend_suicide_count;
static Sint16 friend_suicide_result;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
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
static int emycol_d_count;
static Sint16 emycol_d_result;

Sint16 friend_suicide(sprite_status *pActwk);
void sinset(Uint8 angle, Sint16 *sin, Sint16 *cos);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r1/friend1.c"

Sint16 friend_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++friend_suicide_count;
    return friend_suicide_result;
}

void sinset(Uint8 angle, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = angle;
    *sin = sinset_sin;
    *cos = sinset_cos;
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
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

static size_t short_alias_offset(int short_index) {
    return (size_t)short_index * sizeof(Sint16) - offsetof(sprite_status, actfree);
}

static size_t long_alias_offset(int long_index) {
    return (size_t)long_index * sizeof(Sint32) - offsetof(sprite_status, actfree);
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = short_alias_offset(short_index);
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actor_long_alias(sprite_status *actor, int long_index,
                                 Sint32 value) {
    size_t offset = long_alias_offset(long_index);
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    return (Sint32)((Uint32)actor->actfree[offset] |
                    ((Uint32)actor->actfree[offset + 1] << 8) |
                    ((Uint32)actor->actfree[offset + 2] << 16) |
                    ((Uint32)actor->actfree[offset + 3] << 24));
}

static void reset_friend1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    time_flag = 0;
    friend_suicide_count = 0;
    friend_suicide_result = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 0;
    sinset_cos = 0;
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
    emycol_d_result = 0;
}

static void test_friend1_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_friend0[0] == &spr_flicky1);
    TEST_ASSERT_TRUE(ctx, pat_friend0[2] == &spr_friend0);
    TEST_ASSERT_TRUE(ctx, pat_friend1[5] == &spr_ricky5);
    TEST_ASSERT_TRUE(ctx, pchg0[0] == pchg00);
    TEST_ASSERT_TRUE(ctx, pchg1[0] == pchg10);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_FRIEND_BASE, spr_flicky1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_FRIEND_BASE + 6,
                       spr_ricky5.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1271, tbl0sproffset[0]);
    TEST_ASSERT_EQ_INT(ctx, 975, tbl0sproffset[10]);
}

static void test_t_init_sets_flicky_fields_and_orbit_origin(test_context *ctx) {
    sprite_status *bird = &actwk[3];

    reset_friend1_state();
    bird->xposi.w.h = 100;
    bird->yposi.w.h = 200;
    bird->userflag.b.h = 0;
    stageno.b.l = 1;
    time_flag = 2;

    t_init(bird);

    TEST_ASSERT_EQ_INT(ctx, 2, bird->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, bird->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->cddat);
    TEST_ASSERT_EQ_INT(ctx, 8, bird->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, bird->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, bird->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, bird->sprhsize);
    TEST_ASSERT_TRUE(ctx, bird->patbase == pat_friend0);
    TEST_ASSERT_EQ_INT(ctx, 100, actor_short_alias(bird, 23));
    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(bird, 24));
    TEST_ASSERT_EQ_INT(ctx, (1121 | 32768), bird->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->actfree[5]);

    reset_friend1_state();
    bird->xposi.w.h = 120;
    bird->yposi.w.h = 240;
    bird->userflag.b.h = -128;

    t_init(bird);

    TEST_ASSERT_EQ_INT(ctx, 4, bird->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 3, bird->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 5, bird->actflg);
}

static void test_t_move_rolls_animates_and_reverses_at_limits(
    test_context *ctx) {
    sprite_status *bird = &actwk[3];

    reset_friend1_state();
    set_actor_short_alias(bird, 23, 100);
    set_actor_short_alias(bird, 24, 200);
    bird->actfree[4] = 10;
    bird->actfree[5] = 2;
    sinset_sin = 40;
    sinset_cos = 20;

    t_move(bird);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 10, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 110, bird->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 220, bird->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 12, bird->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 2, bird->actfree[5]);
    TEST_ASSERT_TRUE(ctx, patchg_actor == bird);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg0);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bird);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == bird);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);

    reset_friend1_state();
    set_actor_short_alias(bird, 23, 100);
    set_actor_short_alias(bird, 24, 200);
    bird->actfree[4] = 127;
    bird->actfree[5] = 1;

    t_move(bird);

    TEST_ASSERT_EQ_INT(ctx, 127, bird->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, -1, (Sint8)bird->actfree[5]);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->cddat);
}

static void test_t_movie_parent_gates_and_orbit_paths(test_context *ctx) {
    sprite_status *bird = &actwk[3];
    sprite_status *parent = &actwk[46];

    reset_friend1_state();
    set_actor_short_alias(bird, 33, 46);
    parent->actno = 0;
    t_movie(bird);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == bird);

    reset_friend1_state();
    set_actor_short_alias(bird, 33, 46);
    parent->actno = 46;
    parent->actfree[21] = 1;
    t_movie(bird);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_friend1_state();
    set_actor_short_alias(bird, 23, 160);
    set_actor_short_alias(bird, 24, 96);
    set_actor_short_alias(bird, 33, 46);
    parent->actno = 46;
    bird->actfree[4] = 124;
    sinset_sin = 64;
    sinset_cos = 32;

    t_movie(bird);

    TEST_ASSERT_EQ_INT(ctx, 164, bird->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, bird->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, bird->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, bird->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_friend1_state();
    set_actor_short_alias(bird, 23, 160);
    set_actor_short_alias(bird, 24, 96);
    set_actor_short_alias(bird, 33, 46);
    parent->actno = 46;
    bird->actfree[4] = 8;
    sinset_sin = 64;
    sinset_cos = 32;

    t_movie(bird);

    TEST_ASSERT_TRUE(ctx, patchg_actor == bird);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == bird);
}

static void test_p_init_sets_ricky_fields_and_movie_variant(test_context *ctx) {
    sprite_status *ricky = &actwk[3];

    reset_friend1_state();
    ricky->xposi.w.h = 320;
    ricky->userflag.b.h = 1;
    stageno.b.l = 2;
    time_flag = 2;

    p_init(ricky);

    TEST_ASSERT_EQ_INT(ctx, 2, ricky->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, ricky->actflg);
    TEST_ASSERT_EQ_INT(ctx, 8, ricky->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, ricky->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 4, ricky->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, ricky->sprhsize);
    TEST_ASSERT_TRUE(ctx, ricky->patbase == pat_friend1);
    TEST_ASSERT_EQ_INT(ctx, 320, actor_short_alias(ricky, 23));
    TEST_ASSERT_EQ_INT(ctx, 975, ricky->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 65536, actor_long_alias(ricky, 12));
    TEST_ASSERT_EQ_INT(ctx, -0x40000, actor_long_alias(ricky, 13));

    reset_friend1_state();
    ricky->userflag.b.h = -127;
    p_init(ricky);
    TEST_ASSERT_EQ_INT(ctx, 4, ricky->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, ricky->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, ricky->cddat);
}

static void test_p_move_bounces_on_collision_and_falls_without_collision(
    test_context *ctx) {
    sprite_status *ricky = &actwk[3];

    reset_friend1_state();
    ricky->xposi.l = 100 << 16;
    ricky->yposi.l = 200 << 16;
    set_actor_long_alias(ricky, 12, 2 << 16);
    set_actor_long_alias(ricky, 13, -1 << 16);
    emycol_d_result = 3;

    p_move(ricky);

    TEST_ASSERT_EQ_INT(ctx, 102, ricky->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, ricky->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, ricky->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == ricky);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == ricky);

    reset_friend1_state();
    ricky->xposi.l = 100 << 16;
    ricky->yposi.l = 200 << 16;
    set_actor_long_alias(ricky, 12, 2 << 16);
    set_actor_long_alias(ricky, 13, 1 << 16);
    emycol_d_result = -5;

    p_move(ricky);

    TEST_ASSERT_EQ_INT(ctx, 102, ricky->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 196, ricky->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, ricky->patno);
    TEST_ASSERT_EQ_INT(ctx, -0x40000, actor_long_alias(ricky, 13));
    TEST_ASSERT_EQ_INT(ctx, -(2 << 16), actor_long_alias(ricky, 12));
    TEST_ASSERT_EQ_INT(ctx, 1, ricky->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, ricky->cddat);
}

static void test_p_movie_parent_gates_and_animates(test_context *ctx) {
    sprite_status *ricky = &actwk[3];
    sprite_status *parent = &actwk[46];

    reset_friend1_state();
    set_actor_short_alias(ricky, 33, 46);
    parent->actno = 0;
    p_movie(ricky);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == ricky);

    reset_friend1_state();
    set_actor_short_alias(ricky, 33, 46);
    parent->actno = 46;
    parent->actfree[21] = 1;
    p_movie(ricky);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_friend1_state();
    set_actor_short_alias(ricky, 33, 46);
    parent->actno = 46;
    p_movie(ricky);
    TEST_ASSERT_TRUE(ctx, patchg_actor == ricky);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg1);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == ricky);
}

static void test_friend_dispatches_or_exits_on_suicide(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend1_state();
    friend_suicide_result = -1;
    friend(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);

    reset_friend1_state();
    actor->userflag.b.h = 0;
    friend(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend0);

    reset_friend1_state();
    actor->userflag.b.h = 1;
    friend(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend1);
}

TEST_MAIN_BEGIN;
    test_friend1_tables_capture_literal_data(&ctx);
    test_t_init_sets_flicky_fields_and_orbit_origin(&ctx);
    test_t_move_rolls_animates_and_reverses_at_limits(&ctx);
    test_t_movie_parent_gates_and_orbit_paths(&ctx);
    test_p_init_sets_ricky_fields_and_movie_variant(&ctx);
    test_p_move_bounces_on_collision_and_falls_without_collision(&ctx);
    test_p_movie_parent_gates_and_animates(&ctx);
    test_friend_dispatches_or_exits_on_suicide(&ctx);
TEST_MAIN_END

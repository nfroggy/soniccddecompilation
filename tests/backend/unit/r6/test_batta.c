#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int emycol_u_count;
static Sint16 emycol_u_result;
static int emycol_r_count;
static Uint8 emycol_r_arg;
static Sint16 emycol_r_result;
static int emycol_l_count;
static Uint8 emycol_l_arg;
static Sint16 emycol_l_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_u(sprite_status *pActwk);
Sint16 emycol_r(sprite_status *pActwk, Uint8 byD3);
Sint16 emycol_l(sprite_status *pActwk, Uint8 byD3);

#include "src/r6/batta.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

Sint16 emycol_u(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_u_count;
    return emycol_u_result;
}

Sint16 emycol_r(sprite_status *pActwk, Uint8 byD3) {
    (void)pActwk;
    ++emycol_r_count;
    emycol_r_arg = byD3;
    return emycol_r_result;
}

Sint16 emycol_l(sprite_status *pActwk, Uint8 byD3) {
    (void)pActwk;
    ++emycol_l_count;
    emycol_l_arg = byD3;
    return emycol_l_result;
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actor_long(sprite_status *actor, int byte_offset,
                           Sint32 value) {
    memcpy(&actor->actfree[byte_offset], &value, sizeof(value));
}

static void reset_batta_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    emycol_u_count = 0;
    emycol_u_result = 0;
    emycol_r_count = 0;
    emycol_r_arg = 0;
    emycol_r_result = 0;
    emycol_l_count = 0;
    emycol_l_arg = 0;
    emycol_l_result = 0;
}

static void test_batta_tables_and_patterns(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, batta_tbl[0] == &batta_init);
    TEST_ASSERT_TRUE(ctx, batta_tbl[1] == &batta_down);
    TEST_ASSERT_TRUE(ctx, batta_tbl[2] == &batta_wait);
    TEST_ASSERT_TRUE(ctx, batta_tbl[3] == &batta_up);
    TEST_ASSERT_TRUE(ctx, pat_batta_e[0] == &ep_batta0);
    TEST_ASSERT_TRUE(ctx, pat_batta_b[1] == &bp_batta1);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BATTA_BASE, ep_batta0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_BATTA_BASE + 3, bp_batta1.spra[0].index);
}

static void test_batta_main_dispatch_and_initialization(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_batta_state();
    enemy_suicide_result = -1;
    batta(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_batta_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;
    batta(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_batta_state();
    actor->userflag.b.h = 1;
    batta_init(actor);
}

static void test_batta_lateral_collision_helper(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_batta_state();
    actor->actfree[12] = 255;
    emycol_r_result = -7;
    emylr_jump(actor, 240);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 240, emycol_r_arg);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_l_count);

    reset_batta_state();
    actor->actfree[12] = 1;
    emycol_l_result = 9;
    emylr_jump(actor, 8);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_l_count);
    TEST_ASSERT_EQ_INT(ctx, 8, emycol_l_arg);
}

static void test_batta_down_floor_wall_and_gravity_paths(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_batta_state();
    actor->actfree[12] = 255;
    emycol_d_result = 3;
    emycol_r_result = -1;
    set_actor_word(actor, 28, -16);
    batta_down(actor);

    reset_batta_state();
    actor->actfree[12] = 1;
    emycol_d_result = -2;
    emycol_l_result = -4;
    set_actor_word(actor, 28, 16);
    batta_down(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    actor->yposi.w.h = 100;
    actor->userflag.b.h = 0;
    emycol_d_result = -5;
    emycol_r_result = -1;
    set_actor_word(actor, 28, -16);
    batta_down(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    actor->yposi.w.h = 100;
    actor->userflag.b.h = 1;
    emycol_d_result = -2;
    emycol_r_result = 0;
    batta_down(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    actor->xposi.w.h = 10;
    actor->yposi.w.h = 20;
    emycol_d_result = 0;
    emycol_r_result = 0;
    set_actor_long(actor, 0, 65536);
    set_actor_long(actor, 4, 0);
    batta_down(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    emycol_d_result = 0;
    emycol_r_result = 0;
    set_actor_long(actor, 4, 458752);
    batta_down(actor);
    batta_down(actor);
}

static void test_batta_wait_release_paths(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_batta_state();
    actor->userflag.b.h = 1;
    actor->yposi.w.h = 100;
    actor->patno = 0;
    set_actor_word(actor, 31, 11);
    batta_wait(actor);

    reset_batta_state();
    actor->userflag.b.h = 1;
    actor->yposi.w.h = 100;
    actor->patno = 1;
    set_actor_word(actor, 31, 6);
    batta_wait(actor);

    reset_batta_state();
    actor->userflag.b.h = 0;
    actor->yposi.w.h = 100;
    actor->patno = 0;
    ((char *)actor)[54] = 1;
    set_actor_word(actor, 31, 1);
    batta_wait(actor);

    reset_batta_state();
    actor->userflag.b.h = 1;
    actor->actflg = 1;
    actor->cddat = 1;
    actor->patno = 1;
    actor->actfree[12] = 1;
    ((char *)actor)[54] = 0;
    set_actor_word(actor, 31, 1);
    batta_wait(actor);
}

static void test_batta_up_wall_ceiling_and_fall_paths(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_batta_state();
    actor->actfree[12] = 255;
    emycol_u_result = 1;
    emycol_r_result = -1;
    set_actor_word(actor, 28, -16);
    batta_up(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    emycol_u_result = -5;
    emycol_r_result = -7;
    set_actor_word(actor, 28, -16);
    batta_up(actor);

    reset_batta_state();
    actor->actfree[12] = 255;
    actor->yposi.w.h = 100;
    actor->patno = 0;
    emycol_u_result = -5;
    emycol_r_result = -1;
    batta_up(actor);

    reset_batta_state();
    actor->yposi.w.h = 100;
    actor->patno = 1;
    emycol_u_result = -1;
    emycol_r_result = 0;
    batta_up(actor);

    reset_batta_state();
    actor->yposi.w.h = 100;
    emycol_u_result = 0;
    emycol_r_result = 0;
    set_actor_long(actor, 4, -16384);
    batta_up(actor);

    reset_batta_state();
    actor->yposi.w.h = 100;
    actor->patno = 0;
    emycol_u_result = 0;
    emycol_r_result = 0;
    set_actor_long(actor, 4, -4096);
    batta_up(actor);
}

TEST_MAIN_BEGIN;
test_batta_tables_and_patterns(&ctx);
test_batta_main_dispatch_and_initialization(&ctx);
test_batta_lateral_collision_helper(&ctx);
test_batta_down_floor_wall_and_gravity_paths(&ctx);
test_batta_wait_release_paths(&ctx);
test_batta_up_wall_ceiling_and_fall_paths(&ctx);
TEST_MAIN_END

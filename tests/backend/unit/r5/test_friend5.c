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

#include "src/r5/friend5.c"

Sint16 friend_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++friend_suicide_count;
    return friend_suicide_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
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
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

static void reset_logs(void) {
    friend_suicide_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 32;
    sinset_cos = 64;
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

static void reset_friend5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    time_flag = 0;
    friend_suicide_result = 0;
    emycol_d_result = 1;
    reset_logs();
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 get_actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void init_flicky(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_friend5_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    friend(actor);
    reset_logs();
}

static void init_ricky(sprite_status *actor, Sint16 x, Sint16 y) {
    reset_friend5_state();
    actor->userflag.b.h = 1;
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    friend(actor);
    reset_logs();
}

static void assert_action_frameout_s(test_context *ctx,
                                     sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_friend0[0] == &friend0_pat00);
    TEST_ASSERT_TRUE(ctx, pat_friend0[1] == &friend0_pat01);
    TEST_ASSERT_TRUE(ctx, pat_friend1[0] == &friend1_pat00);
    TEST_ASSERT_TRUE(ctx, pat_friend1[4] == &friend1_pat04);
    TEST_ASSERT_TRUE(ctx, pat_friend2[0] == &friend2_pat00);
    TEST_ASSERT_TRUE(ctx, pat_friend2[4] == &friend2_pat04);
    TEST_ASSERT_EQ_INT(ctx, -8, friend0_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 435, friend0_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -12, friend1_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 437, friend1_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 444, friend2_pat04.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg0[0] == pchg0_00);
    TEST_ASSERT_TRUE(ctx, pchg0[1] == pchg0_01);
    TEST_ASSERT_TRUE(ctx, pchg1[0] == pchg1_00);
    TEST_ASSERT_EQ_INT(ctx, 19, pchg0_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg0_01[29]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg1_00[57]);
}

static void test_friend_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend5_state();
    actor->userflag.b.h = 1;
    friend_suicide_result = -1;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_flicky_init_normal_and_movie_variants(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend5_state();
    stageno.b.l = 1;
    time_flag = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend0);
    TEST_ASSERT_EQ_INT(ctx, (1210 | 32768), actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 100, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 200, get_actfree_word(actor, 2));
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[5]);

    reset_friend5_state();
    actor->userflag.b.h = -128;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1210, actor->sproffset);
}

static void test_flicky_move_rolls_patches_and_wraps_direction(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_flicky(actor, 100, 200);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 132, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);

    reset_logs();
    actor->actfree[4] = 127;
    actor->actfree[5] = 1;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 127, actor->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->actfree[5]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_flicky_movie_liveness_and_spin_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *movie_actor = &actwk[7];

    reset_friend5_state();
    actor->userflag.b.h = -128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    set_actfree_word(actor, 20, 7);
    movie_actor->actno = 44;
    friend(actor);
    reset_logs();

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_logs();
    actor->actfree[4] = 124;
    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 128, actor->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, (5 ^ 1), actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, (1 ^ 1), actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_logs();
    movie_actor->actfree[21] = 1;
    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_logs();
    movie_actor->actfree[21] = 0;
    movie_actor->actno = 0;
    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_ricky_init_normal_and_movie_variants(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_friend5_state();
    stageno.b.l = 2;
    time_flag = 2;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend1);
    TEST_ASSERT_EQ_INT(ctx, 1210, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 300, get_actfree_word(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 65536, get_actfree_long(actor, 2));
    TEST_ASSERT_EQ_INT(ctx, -262144, get_actfree_long(actor, 6));

    reset_friend5_state();
    actor->userflag.b.h = -127;
    actor->xposi.w.h = 30;
    actor->yposi.w.h = 40;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend2);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1210, actor->sproffset);
}

static void test_ricky_move_arc_animation_and_landing(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    init_ricky(actor, 300, 400);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    assert_action_frameout_s(ctx, actor);

    reset_logs();
    set_actfree_long(actor, 6, 0);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    assert_action_frameout_s(ctx, actor);

    reset_logs();
    emycol_d_result = -3;
    actor->actflg = 4;
    actor->cddat = 0;
    set_actfree_long(actor, 2, 65536);

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, -65536, get_actfree_long(actor, 2));
    TEST_ASSERT_EQ_INT(ctx, -262144, get_actfree_long(actor, 6));
    assert_action_frameout_s(ctx, actor);
}

static void test_ricky_movie_liveness_and_patch_path(test_context *ctx) {
    sprite_status *actor = &actwk[5];
    sprite_status *movie_actor = &actwk[7];

    reset_friend5_state();
    actor->userflag.b.h = -127;
    set_actfree_word(actor, 20, 7);
    movie_actor->actno = 44;

    friend(actor);
    reset_logs();

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);

    reset_logs();
    movie_actor->actfree[21] = 1;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_logs();
    movie_actor->actfree[21] = 0;
    movie_actor->actno = 0;

    friend(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

TEST_MAIN_BEGIN;
    test_tables_capture_literal_data(&ctx);
    test_friend_suicide_short_circuits(&ctx);
    test_flicky_init_normal_and_movie_variants(&ctx);
    test_flicky_move_rolls_patches_and_wraps_direction(&ctx);
    test_flicky_movie_liveness_and_spin_paths(&ctx);
    test_ricky_init_normal_and_movie_variants(&ctx);
    test_ricky_move_arc_animation_and_landing(&ctx);
    test_ricky_movie_liveness_and_patch_path(&ctx);
TEST_MAIN_END

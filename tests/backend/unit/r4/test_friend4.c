#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 time_flag;

static Sint16 friend_suicide_result;
static int friend_suicide_count;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int sinset_count;
static Uint8 sinset_angle;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;

Sint16 friend_suicide(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#include "src/r4/friend4.c"

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

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

static size_t short_alias_offset(int short_index) {
    return (size_t)short_index * sizeof(Sint16) -
           offsetof(sprite_status, actfree);
}

static size_t long_alias_offset(int long_index) {
    return (size_t)long_index * sizeof(Sint32) -
           offsetof(sprite_status, actfree);
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = short_alias_offset(short_index);
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actor_long_alias(sprite_status *actor, int long_index,
                                 Sint32 value) {
    size_t offset = long_alias_offset(long_index);
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
    actor->actfree[offset + 2] = (Uint8)(bits >> 16);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    Uint32 bits = actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
    actor->actfree[offset + 2] = (Uint8)(bits >> 16);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void reset_logs(void) {
    friend_suicide_count = 0;
    sinset_sin = 32;
    sinset_cos = 64;
    sinset_count = 0;
    sinset_angle = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
}

static void reset_friend4_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    time_flag = 0;
    friend_suicide_result = 0;
    reset_logs();
}

static void init_normal(sprite_status *actor, Sint16 x, Sint16 y,
                        Sint8 userflag_h) {
    reset_friend4_state();
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
    actor->userflag.b.h = userflag_h;
    friend4(actor);
    reset_logs();
}

static void assert_patch_and_action(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_TRUE(ctx, pchg[1] == pchg1);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg0[29]);
    TEST_ASSERT_EQ_INT(ctx, 19, pchg1[0]);
    TEST_ASSERT_TRUE(ctx, pat_friend0[0] == &spr_friend4_00);
    TEST_ASSERT_TRUE(ctx, pat_friend0[2] == &spr_friend4_99);
    TEST_ASSERT_TRUE(ctx, pat_friend1[1] == &spr_friend4_11);
    TEST_ASSERT_EQ_INT(ctx, -8, spr_friend4_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, spr_friend4_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 435, spr_friend4_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 438, spr_friend4_11.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1104, tbl0sproffset[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl0sproffset[3]);
    TEST_ASSERT_EQ_INT(ctx, 1104, tbl0sproffset[10]);
}

static void test_friend_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend4_state();
    friend_suicide_result = 1;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, friend_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_normal_init_variants(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_friend4_state();
    stageno.b.l = 1;
    time_flag = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1104, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend0);
    TEST_ASSERT_EQ_INT(ctx, 200, actor_short_alias(actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 65536, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 256, actor_long_alias(actor, 14));

    reset_friend4_state();
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 9296, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend1);
}

static void test_normal_move_and_direction_limits(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    init_normal(actor, 100, 200, 0);

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 101, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 204, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, actor_long_alias(actor, 13));
    assert_patch_and_action(ctx, actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);

    reset_logs();
    set_actor_long_alias(actor, 13, -512);
    set_actor_long_alias(actor, 14, 256);
    set_actfree_long(actor, 0, 65536);

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, -256, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, -65536, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    assert_patch_and_action(ctx, actor);

    reset_logs();
    actor->actflg = 4;
    actor->cddat = 0;
    set_actor_long_alias(actor, 13, 65535);
    set_actor_long_alias(actor, 14, 1);
    set_actfree_long(actor, 0, 65536);

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, -1, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, -65536, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
}

static void test_movie_init_and_parent_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_friend4_state();
    actor->userflag.b.h = -128;
    actor->xposi.w.h = 500;
    actor->yposi.w.h = 600;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 254, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 500, actor_short_alias(actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 512, actor_long_alias(actor, 14));

    reset_logs();
    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_logs();
    actwk[0].actno = 82;
    actor->actfree[9] = 7;
    set_actor_long_alias(actor, 13, -512);
    actor->sprpri = 5;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 254, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 508, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 602, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    assert_patch_and_action(ctx, actor);

    reset_logs();
    set_actor_long_alias(actor, 13, -512);
    actor->sprpri = 3;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    assert_patch_and_action(ctx, actor);

    reset_friend4_state();
    actor = &actwk[4];
    actor->userflag.b.h = -127;
    actor->xposi.w.h = 700;
    actor->yposi.w.h = 800;

    friend4(actor);

    TEST_ASSERT_EQ_INT(ctx, 254, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, -512, actor_long_alias(actor, 14));
    TEST_ASSERT_EQ_INT(ctx, 12288, actor_long_alias(actor, 13));
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_friend1);
    TEST_ASSERT_EQ_INT(ctx, 9296, actor->sproffset);
}

TEST_MAIN_BEGIN;
test_tables_capture_literal_data(&ctx);
test_friend_suicide_short_circuits(&ctx);
test_normal_init_variants(&ctx);
test_normal_move_and_direction_limits(&ctx);
test_movie_init_and_parent_paths(&ctx);
TEST_MAIN_END

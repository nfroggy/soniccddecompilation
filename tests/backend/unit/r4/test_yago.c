#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xposi;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static Sint16 emycol_d_result;
static int emycol_d_count;
static Sint16 emycol_r_result;
static int emycol_r_count;
static Uint8 emycol_r_sprhs;
static Sint16 emycol_l_result;
static int emycol_l_count;
static Uint8 emycol_l_sprhs;

Sint16 enemy_suicide(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_r(sprite_status *pActwk, Uint8 sprhs);
Sint16 emycol_l(sprite_status *pActwk, Uint8 sprhs);

#include "src/r4/yago.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
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
    frameout_s00_xposi = xposi;
    return 0;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
}

Sint16 emycol_r(sprite_status *pActwk, Uint8 sprhs) {
    (void)pActwk;
    ++emycol_r_count;
    emycol_r_sprhs = sprhs;
    return emycol_r_result;
}

Sint16 emycol_l(sprite_status *pActwk, Uint8 sprhs) {
    (void)pActwk;
    ++emycol_l_count;
    emycol_l_sprhs = sprhs;
    return emycol_l_result;
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
    Uint32 raw = (Uint32)value;
    actor->actfree[offset] = (Uint8)raw;
    actor->actfree[offset + 1] = (Uint8)(raw >> 8);
    actor->actfree[offset + 2] = (Uint8)(raw >> 16);
    actor->actfree[offset + 3] = (Uint8)(raw >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    Uint32 raw = actor->actfree[offset] |
                 ((Uint32)actor->actfree[offset + 1] << 8) |
                 ((Uint32)actor->actfree[offset + 2] << 16) |
                 ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)raw;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 raw = (Uint32)value;
    actor->actfree[offset] = (Uint8)raw;
    actor->actfree[offset + 1] = (Uint8)(raw >> 8);
    actor->actfree[offset + 2] = (Uint8)(raw >> 16);
    actor->actfree[offset + 3] = (Uint8)(raw >> 24);
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    Uint32 raw = actor->actfree[offset] |
                 ((Uint32)actor->actfree[offset + 1] << 8) |
                 ((Uint32)actor->actfree[offset + 2] << 16) |
                 ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)raw;
}

static void reset_yago_state(void) {
    memset(actwk, 0, sizeof(actwk));
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xposi = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    emycol_d_result = 5;
    emycol_d_count = 0;
    emycol_r_result = 5;
    emycol_r_count = 0;
    emycol_r_sprhs = 0;
    emycol_l_result = 5;
    emycol_l_count = 0;
    emycol_l_sprhs = 0;
}

static void assert_displayed(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pat_chg);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
}

static void test_yago_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_chg[0] == pat_chg0);
    TEST_ASSERT_EQ_INT(ctx, 9, pat_chg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pat_chg0[3]);
    TEST_ASSERT_TRUE(ctx, pat_yago_e[0] == &spr_yago_e_00);
    TEST_ASSERT_TRUE(ctx, pat_yago_b[1] == &spr_yago_b_01);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_YAGO_BASE + 2,
                       spr_yago_e_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_YAGO_BASE + 1,
                       spr_yago_b_01.spra[0].index);
}

static void test_suicide_gates_skip_both_variants(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    enemy_suicide_result = 1;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_yago_state();
    enemy_suicide_result = 1;
    actor->userflag.b.h = 1;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_init_and_fall_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    actor->xposi.w.h = 120;
    actor->yposi.w.h = 80;
    emycol_d_result = 5;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 47, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9200, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_yago_e);
    TEST_ASSERT_EQ_INT(ctx, 120, actor_short_alias(actor, 33));
    TEST_ASSERT_EQ_INT(ctx, 81, actor->yposi.w.h);
    assert_displayed(ctx, actor);

    reset_yago_state();
    actor = &actwk[4];
    actor->xposi.w.h = 130;
    actor->userflag.b.h = 1;
    emycol_d_result = 4;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_yago_b);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5242880, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, -16384, actfree_long(actor, 4));

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 2;
    emycol_d_result = 5;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->yposi.w.h);
    assert_displayed(ctx, actor);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 2;
    emycol_d_result = 4;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5242880, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, -16384, actfree_long(actor, 4));
}

static void test_lr_branches_and_collision_sides(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    actor->r_no0 = 4;
    actor->sprhs = 24;
    actor->xposi.w.h = 200;
    actwk[0].xposi.w.h = 0;
    set_actfree_long(actor, 0, 5242880);
    set_actfree_long(actor, 4, -16384);
    emycol_d_result = 5;
    emycol_l_result = 5;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_l_count);
    TEST_ASSERT_EQ_INT(ctx, (Uint8)-24, emycol_l_sprhs);
    TEST_ASSERT_EQ_INT(ctx, 0, emycol_r_count);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    actor->sprhs = 24;
    actor->actflg = 1;
    actor->xposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    set_actfree_long(actor, 0, 5242880);
    set_actfree_long(actor, 4, -16384);
    emycol_d_result = 5;
    emycol_r_result = 5;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 7, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_r_count);
    TEST_ASSERT_EQ_INT(ctx, 24, emycol_r_sprhs);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    set_actfree_long(actor, 0, 0);
    emycol_d_result = 5;
    emycol_l_result = 4;

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    set_actfree_long(actor, 0, -1);

    yago(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));
}

static void test_rev_and_jet_countdowns(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    actor->r_no0 = 6;
    set_actor_short_alias(actor, 27, 2);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(actor, 27));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 6;
    actor->actflg = 4;
    actor->cddat = 0;
    set_actor_short_alias(actor, 27, 1);
    set_actfree_long(actor, 4, -16384);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 5242880, actfree_long(actor, 0));
    TEST_ASSERT_EQ_INT(ctx, 16384, actfree_long(actor, 4));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 8;
    set_actor_short_alias(actor, 27, 2);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 8;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 10;
    set_actor_short_alias(actor, 27, 2);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 10;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));
}

static void test_e_jet_and_fall_recovery(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    actor->r_no0 = 12;
    set_actor_short_alias(actor, 27, 2);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(actor, 27));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 12;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -294912, actor_long_alias(actor, 15));
    TEST_ASSERT_EQ_INT(ctx, -98304, actor_long_alias(actor, 14));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 12;
    actor->actflg = 1;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 294912, actor_long_alias(actor, 15));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 14;
    actor->sprhs = 24;
    set_actor_long_alias(actor, 15, 0x10000);
    set_actor_long_alias(actor, 14, -98304);
    emycol_l_result = 5;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, -90112, actor_long_alias(actor, 14));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 14;
    actor->actflg = 1;
    actor->sprhs = 24;
    emycol_r_result = 4;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 16;
    emycol_d_result = 5;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->r_no0 = 16;
    actor->yposi.w.h = 10;
    emycol_d_result = 4;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));
}

static void test_b_variant_jet_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_yago_state();
    actor->userflag.b.h = 1;
    actor->r_no0 = 12;
    set_actor_short_alias(actor, 27, 2);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 12;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, -65536, actor_long_alias(actor, 15));
    TEST_ASSERT_EQ_INT(ctx, -98304, actor_long_alias(actor, 14));

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 12;
    actor->actflg = 1;
    set_actor_short_alias(actor, 27, 1);
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 65536, actor_long_alias(actor, 15));

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 14;
    set_actor_long_alias(actor, 14, -8192);
    emycol_l_result = 0;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, -4096, actor_long_alias(actor, 14));

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 14;
    set_actor_long_alias(actor, 14, -4096);
    emycol_l_result = 0;
    emycol_d_result = 4;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 14;
    set_actor_long_alias(actor, 14, -4096);
    emycol_l_result = 0;
    emycol_d_result = 5;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);

    reset_yago_state();
    actor = &actwk[4];
    actor->userflag.b.h = 1;
    actor->r_no0 = 14;
    actor->actflg = 1;
    emycol_r_result = -1;
    yago(actor);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 31, actor_short_alias(actor, 27));
}

TEST_MAIN_BEGIN;
test_yago_tables_capture_literal_data(&ctx);
test_suicide_gates_skip_both_variants(&ctx);
test_init_and_fall_paths(&ctx);
test_lr_branches_and_collision_sides(&ctx);
test_rev_and_jet_countdowns(&ctx);
test_e_jet_and_fall_recovery(&ctx);
test_b_variant_jet_paths(&ctx);
TEST_MAIN_END

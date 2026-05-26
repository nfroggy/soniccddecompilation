#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_x;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint16 enemy_suicide(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/tonbo.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_x = xposi;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_x = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 64;
    sinset_cos = 123;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    Uint16 bits = (Uint16)actor->actfree[offset] |
                  ((Uint16)actor->actfree[offset + 1] << 8);
    return (Sint16)bits;
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

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static int legacy_long_actfree_offset(int long_index) {
    return (long_index * 4) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void set_legacy_long(sprite_status *actor, int long_index,
                            Sint32 value) {
    set_actfree_long(actor, legacy_long_actfree_offset(long_index), value);
}

static Sint32 get_legacy_long(sprite_status *actor, int long_index) {
    return get_actfree_long(actor, legacy_long_actfree_offset(long_index));
}

static void test_tonbo_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tonbo_e[0] == &spr_tonbo_e_00);
    TEST_ASSERT_TRUE(ctx, pat_tonbo_e[1] == &spr_tonbo_e_01);
    TEST_ASSERT_TRUE(ctx, pat_tonbo_e[2] == &spr_tonbo_e_02);
    TEST_ASSERT_TRUE(ctx, pat_tonbo_b[0] == &spr_tonbo_b_00);
    TEST_ASSERT_TRUE(ctx, pat_tonbo_b[1] == &spr_tonbo_b_01);
    TEST_ASSERT_TRUE(ctx, pat_tonbo_b[2] == &spr_tonbo_b_02);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE + 3,
                       spr_tonbo_e_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE + 4,
                       spr_tonbo_e_01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE + 5,
                       spr_tonbo_e_02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE,
                       spr_tonbo_b_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE + 1,
                       spr_tonbo_b_01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TONBO_BASE + 2,
                       spr_tonbo_b_02.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_TRUE(ctx, pchg[1] == pchg1);
}

static void test_tonbo_entry_returns_when_enemy_suicide_handles_actor(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    enemy_suicide_result = 1;

    tonbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_tonbo_init_enemy_variant_sets_fast_motion(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->xposi.w.h = 120;
    actor->yposi.w.h = 80;

    tonbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 44, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 28, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 28, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9238, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 120, get_legacy_word(actor, 33));
    TEST_ASSERT_EQ_INT(ctx, 80 << 16, get_actfree_long(actor, 0));
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_tonbo_e);
    TEST_ASSERT_EQ_INT(ctx, -65536, get_legacy_long(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 4, get_legacy_word(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 128, get_legacy_word(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 120, frameout_s00_x);
}

static void test_tonbo_init_b_variant_sets_slower_motion(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 90;
    actor->userflag.b.h = 1;

    tonbo(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_tonbo_b);
    TEST_ASSERT_EQ_INT(ctx, -32768, get_legacy_long(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 1, get_legacy_word(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 512, get_legacy_word(actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(actor, 29));
}

static void test_tonbo_move_applies_sine_motion_without_turning(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->r_no0 = 2;
    actor->xposi.l = 120 << 16;
    actor->yposi.l = 80 << 16;
    actor->actfree[4] = 12;
    set_actfree_long(actor, 0, 80 << 16);
    set_legacy_long(actor, 13, -65536);
    set_legacy_word(actor, 28, 4);
    set_legacy_word(actor, 29, 2);
    set_legacy_word(actor, 30, 256);
    set_legacy_word(actor, 33, 120);

    tonbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 119, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 84, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, get_legacy_word(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, -65536, get_legacy_long(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 16, get_legacy_word(actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 12, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_tonbo_move_turns_when_countdown_expires(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->r_no0 = 2;
    actor->xposi.l = 120 << 16;
    actor->yposi.l = 80 << 16;
    actor->actflg = 4;
    actor->cddat = 2;
    set_actfree_long(actor, 0, 80 << 16);
    set_legacy_long(actor, 13, -32768);
    set_legacy_word(actor, 28, 1);
    set_legacy_word(actor, 29, 1);
    set_legacy_word(actor, 30, 512);
    set_legacy_word(actor, 33, 120);

    tonbo(actor);

    TEST_ASSERT_EQ_INT(ctx, 119, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, get_legacy_word(actor, 29));
    TEST_ASSERT_EQ_INT(ctx, 32768, get_legacy_long(actor, 13));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, get_legacy_word(actor, 25));
}

TEST_MAIN_BEGIN;
    test_tonbo_patterns_capture_literal_data(&ctx);
    test_tonbo_entry_returns_when_enemy_suicide_handles_actor(&ctx);
    test_tonbo_init_enemy_variant_sets_fast_motion(&ctx);
    test_tonbo_init_b_variant_sets_slower_motion(&ctx);
    test_tonbo_move_applies_sine_motion_without_turning(&ctx);
    test_tonbo_move_turns_when_countdown_expires(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
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

Sint16 enemy_suicide(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);

#include "src/r7/kanabun.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = 64;
    *cos = 128;
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

static void reset_kanabun_state(void) {
    memset(actwk, 0, sizeof(actwk));
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
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
}

static void reset_kanabun_logs(void) {
    enemy_suicide_count = 0;
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
}

static void init_actor(sprite_status *actor, Sint8 userflag_h) {
    reset_kanabun_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = userflag_h;
    kanabun(actor);
    reset_kanabun_logs();
}

static void test_kanabun_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_kanabun_e[0] == &pat_e00);
    TEST_ASSERT_TRUE(ctx, pat_kanabun_e[7] == &pat_e07);
    TEST_ASSERT_TRUE(ctx, pat_kanabun_b[0] == &pat_b00);
    TEST_ASSERT_TRUE(ctx, pat_kanabun_b[7] == &pat_b07);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_e00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_e00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 128, pat_e00.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 415, pat_e00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -12, pat_e07.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 422, pat_e07.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 407, pat_b00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 414, pat_b07.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pchg_e[0] == pchg_e00);
    TEST_ASSERT_TRUE(ctx, pchg_b[0] == pchg_b00);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg_e00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_e00[65]);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg_b00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_b00[65]);
    TEST_ASSERT_EQ_INT(ctx, 47, tbl0[0]);
    TEST_ASSERT_EQ_INT(ctx, 48, tbl0[1]);
    TEST_ASSERT_EQ_INT(ctx, 48, tbl0[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, tbl0[3]);
}

static void test_kanabun_suicide_short_circuits(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kanabun_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    enemy_suicide_result = -1;

    kanabun(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 100, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_kanabun_initializes_green_variant_and_moves_left(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kanabun_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    kanabun(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9193, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kanabun_e);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 254, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);
    TEST_ASSERT_EQ_INT(ctx, 47, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_kanabun_initializes_brown_variant_with_slower_angle_step(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_kanabun_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->userflag.b.h = 1;

    kanabun(actor);

    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kanabun_b);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 255, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_b);
    TEST_ASSERT_EQ_INT(ctx, 47, actor->colino);
}

static void test_kanabun_reverses_at_horizontal_extent(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_actor(actor, 0);
    actor->xposi.w.h = 35;

    kanabun(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 254, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg_e);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_kanabun_logs();
    kanabun(actor);

    TEST_ASSERT_EQ_INT(ctx, 36, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_x);
}

static void test_kanabun_uses_pattern_number_for_collision_code(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    init_actor(actor, 0);
    actor->patno = 2;
    kanabun(actor);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->colino);

    actor->patno = 4;
    kanabun(actor);
    TEST_ASSERT_EQ_INT(ctx, 48, actor->colino);

    actor->patno = 6;
    kanabun(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
}

TEST_MAIN_BEGIN;
    test_kanabun_tables_capture_literal_data(&ctx);
    test_kanabun_suicide_short_circuits(&ctx);
    test_kanabun_initializes_green_variant_and_moves_left(&ctx);
    test_kanabun_initializes_brown_variant_with_slower_angle_step(&ctx);
    test_kanabun_reverses_at_horizontal_extent(&ctx);
    test_kanabun_uses_pattern_number_for_collision_code(&ctx);
TEST_MAIN_END

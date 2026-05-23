#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r3/togebl3a.c"

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

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void queue_actor(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void reset_togebl3a_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 8;
    sinset_cos = 7;
}

static void reset_togebl3a_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void test_togebl3a_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_chg[0] == &spat_chg0);
    TEST_ASSERT_TRUE(ctx, togeball_pat[0] == &spat_chg1);
    TEST_ASSERT_EQ_INT(ctx, -8, spat_chg0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, spat_chg0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 556, spat_chg0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, spat_chg1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, spat_chg1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 557, spat_chg1.spra[0].index);
}

static void test_togebl3a_main_initializes_and_spawns_options(
    test_context *ctx) {
    sprite_status *parent = &actwk[2];
    sprite_status *opt1 = &actwk[3];
    sprite_status *opt2 = &actwk[4];
    sprite_status *opt3 = &actwk[5];

    reset_togebl3a_state();
    parent->actno = 44;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 50;
    queue_actor(opt1);
    queue_actor(opt2);
    queue_actor(opt3);

    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_togebl3a_main_reverses_rotation_from_userflag(
    test_context *ctx) {
    sprite_status *parent = &actwk[2];

    reset_togebl3a_state();
    parent->actno = 44;
    parent->userflag.b.h = 1;
    queue_actor(&actwk[3]);
    queue_actor(&actwk[4]);
    queue_actor(&actwk[5]);

    togeball(parent);

    reset_togebl3a_logs();
    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 255, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_togebl3a_main_frameouts_when_option_spawn_fails(
    test_context *ctx) {
    sprite_status *parent = &actwk[2];

    reset_togebl3a_state();
    parent->actno = 44;

    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_togebl3a_state();
    parent = &actwk[2];
    parent->actno = 44;
    queue_actor(&actwk[3]);

    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);

    reset_togebl3a_state();
    parent = &actwk[2];
    parent->actno = 44;
    queue_actor(&actwk[3]);
    queue_actor(&actwk[4]);

    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
}

static void test_togebl3a_parent_move_positions_options_through_option_path(
    test_context *ctx) {
    sprite_status *parent = &actwk[2];
    sprite_status *opt1 = &actwk[3];
    sprite_status *opt2 = &actwk[4];
    sprite_status *opt3 = &actwk[5];

    reset_togebl3a_state();
    parent->actno = 44;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 50;
    queue_actor(opt1);
    queue_actor(opt2);
    queue_actor(opt3);
    togeball(parent);

    reset_togebl3a_logs();
    togeball(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_togebl3a_logs();
    togeball(opt1);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_togebl3a_logs();
    togeball(opt2);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_togebl3a_logs();
    togeball(opt3);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_togebl3a_option_initializes_and_moves_existing_position(
    test_context *ctx) {
    sprite_status *parent = &actwk[2];
    sprite_status *opt = &actwk[3];

    reset_togebl3a_state();
    parent->actno = 44;
    opt->userflag.b.h = -1;
    set_actfree_word(opt, 20, 2);
    set_actfree_word(opt, 2, 12);
    set_actfree_word(opt, 6, 34);

    togeball(opt);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_togebl3a_option_frameouts_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *opt = &actwk[3];

    reset_togebl3a_state();
    opt->userflag.b.h = -1;
    opt->r_no0 = 2;
    set_actfree_word(opt, 20, 2);

    togeball(opt);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == opt);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_togebl3a_tables_capture_literal_data(&ctx);
    test_togebl3a_main_initializes_and_spawns_options(&ctx);
    test_togebl3a_main_reverses_rotation_from_userflag(&ctx);
    test_togebl3a_main_frameouts_when_option_spawn_fails(&ctx);
    test_togebl3a_parent_move_positions_options_through_option_path(&ctx);
    test_togebl3a_option_initializes_and_moves_existing_position(&ctx);
    test_togebl3a_option_frameouts_when_parent_is_gone(&ctx);
TEST_MAIN_END

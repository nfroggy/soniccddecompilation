#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;
int_union scra_v_posit;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_new_actor;
static Sint32 actwkchk2_result;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);

#include "src/r7/gaitou73.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_actor = pActwk;
    if (actwkchk2_result != 0) {
        *ppNewActwk = 0;
        return actwkchk2_result;
    }

    *ppNewActwk = actwkchk2_new_actor;
    return 0;
}

static void reset_gaitou73_state(void) {
    memset(actwk, 0, sizeof(actwk));
    scra_h_posit.l = 0;
    scra_v_posit.l = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    actwkchk2_new_actor = &actwk[9];
    actwkchk2_result = 0;
}

static Sint16 actor_word(sprite_status *actor, int index) {
    int offset = (index - 23) * 2;
    return (Sint16)(actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void set_actor_word(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void test_gaitou73_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, gaitou73_pat[0] == &pat0);
    TEST_ASSERT_TRUE(ctx, gaitou73_pat[1] == &pat1);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -24, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 128, pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 417, pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -112, pat1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 128, pat1.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 418, pat1.spra[0].index);
}

static void test_gaitou73_base_initializes_and_allocates_child(test_context *ctx) {
    sprite_status *base = &actwk[4];
    sprite_status *child = &actwk[9];

    reset_gaitou73_state();

    gaitou73(base);

    TEST_ASSERT_EQ_INT(ctx, 9, actor_word(base, 25));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == base);
    TEST_ASSERT_EQ_INT(ctx, 4, actor_word(child, 25));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == base);
}

static void test_gaitou73_base_handles_child_allocation_failure(
    test_context *ctx) {
    sprite_status *base = &actwk[4];

    reset_gaitou73_state();
    actwkchk2_result = -1;

    gaitou73(base);

    TEST_ASSERT_EQ_INT(ctx, 0, actor_word(base, 25));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[9].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_gaitou73_child_initializes_without_allocation(test_context *ctx) {
    sprite_status *child = &actwk[9];

    reset_gaitou73_state();
    child->userflag.b.h = 1;

    gaitou73(child);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
}

static void test_gaitou73_base_move_selects_short_pattern_near_scroll(
    test_context *ctx) {
    sprite_status *base = &actwk[4];

    reset_gaitou73_state();
    base->r_no0 = 2;
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 360;

    gaitou73(base);

    TEST_ASSERT_EQ_INT(ctx, 496, actor_word(base, 26));
    TEST_ASSERT_EQ_INT(ctx, 350, actor_word(base, 27));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_gaitou73_base_move_selects_tall_pattern_when_above_scroll(
    test_context *ctx) {
    sprite_status *base = &actwk[4];

    reset_gaitou73_state();
    base->r_no0 = 2;
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 700;

    gaitou73(base);

    TEST_ASSERT_EQ_INT(ctx, 496, actor_word(base, 26));
    TEST_ASSERT_EQ_INT(ctx, 236, actor_word(base, 27));
}

static void test_gaitou73_horizontal_scroll_wraps_after_segment_48(
    test_context *ctx) {
    sprite_status *base = &actwk[4];

    reset_gaitou73_state();
    base->r_no0 = 2;
    scra_h_posit.w.h = 16000;
    scra_v_posit.w.h = 360;

    gaitou73(base);

    TEST_ASSERT_EQ_INT(ctx, 576, actor_word(base, 26));
}

static void test_gaitou73_child_follows_base_cached_position(test_context *ctx) {
    sprite_status *child = &actwk[9];
    sprite_status *base = &actwk[4];

    reset_gaitou73_state();
    child->r_no0 = 4;
    set_actor_word(child, 25, 4);
    set_actor_word(base, 26, 444);
    set_actor_word(base, 27, 555);

    gaitou73(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
}

TEST_MAIN_BEGIN;
    test_gaitou73_tables_capture_literal_data(&ctx);
    test_gaitou73_base_initializes_and_allocates_child(&ctx);
    test_gaitou73_base_handles_child_allocation_failure(&ctx);
    test_gaitou73_child_initializes_without_allocation(&ctx);
    test_gaitou73_base_move_selects_short_pattern_near_scroll(&ctx);
    test_gaitou73_base_move_selects_tall_pattern_when_above_scroll(&ctx);
    test_gaitou73_horizontal_scroll_wraps_after_segment_48(&ctx);
    test_gaitou73_child_follows_base_cached_position(&ctx);
TEST_MAIN_END

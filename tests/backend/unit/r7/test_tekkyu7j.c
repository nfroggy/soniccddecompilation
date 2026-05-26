#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static sprite_pattern base_ball_pattern = {1, {{-1, -2, 3, 444}}};
sprite_pattern *pat_tekkyu7[1] = {&base_ball_pattern};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int actwkchk_result;
static sprite_status *actwkchk_actor;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/r7/tekkyu7j.c"

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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_result != 0) {
        *ppActwk = 0;
        return actwkchk_result;
    }

    *ppActwk = actwkchk_actor;
    return 0;
}

static void reset_tekkyu7j_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    actwkchk_result = 0;
    actwkchk_actor = &actwk[20];
}

static void reset_tekkyu7j_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
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

static void set_actor_long(sprite_status *actor, int index, Sint32 value) {
    int offset = index * 4 - 46;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void test_tekkyu7j_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tekkyu7j[0] == &tekkyu7j_pat0);
    TEST_ASSERT_TRUE(ctx, pat_tekkyu7j[1] == &tekkyu7j_pat1);
    TEST_ASSERT_TRUE(ctx, pat_tekkyu7j[2] == &tekkyu7j_pat2);
    TEST_ASSERT_TRUE(ctx, pat_tekkyu7j[3] == &tekkyu7j_pat3);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7j_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7j_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU7J_BASE, tekkyu7j_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7j_pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 0, tekkyu7j_pat1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU7J_BASE + 1, tekkyu7j_pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7j_pat2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU7J_BASE + 2, tekkyu7j_pat2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, tekkyu7j_pat3.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu7j_pat3.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU7J_BASE + 3, tekkyu7j_pat3.spra[0].index);
}

static void test_launcher_initializes_and_counts_down(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_tekkyu7j_state();
    actor->actno = 61;
    actor->userflag.b.h = 2;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 120;

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 149, actor_word(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_launcher_spawns_child_when_timer_reaches_zero(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *child = &actwk[20];

    reset_tekkyu7j_state();
    actor->actno = 61;
    actor->r_no0 = 2;
    actor->userflag.b.h = 3;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 120;
    set_actor_word(actor, 23, 1);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 150, actor_word(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_launcher_allocation_failure_only_resets_timer(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *child = &actwk[20];

    reset_tekkyu7j_state();
    actor->r_no0 = 2;
    set_actor_word(actor, 23, 1);
    actwkchk_result = -1;

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 150, actor_word(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_child_init_uses_vertical_even_direction(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->userflag.b.h = 0;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 31, actor_word(actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 200, actor_word(actor, 24));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_child_init_covers_positive_and_horizontal_directions(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->userflag.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    tekkyu7j(actor);


    reset_tekkyu7j_state();
    actor = &actwk[5];
    actor->userflag.b.l = -1;
    actor->userflag.b.h = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 100, actor_word(actor, 24));
}

static void test_child_move_waits_then_enters_stop(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->r_no0 = 2;
    actor->actfree[0] = 1;
    actor->yposi.w.h = 200;
    set_actor_word(actor, 25, 2);
    set_actor_long(actor, 13, 1 << 16);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor_word(actor, 25));

    reset_tekkyu7j_logs();
    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 30, actor_word(actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_child_stop_waits_then_enters_jump(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->userflag.b.h = 0;
    actor->r_no0 = 4;
    set_actor_word(actor, 25, 2);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor_word(actor, 25));

    reset_tekkyu7j_logs();
    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_child_jump_moves_vertically_without_frameout_when_side_matches(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->r_no0 = 6;
    actor->actfree[0] = 1;
    actor->actfree[21] = 0;
    actor->yposi.w.h = 200;
    set_actor_word(actor, 24, 200);
    set_actor_long(actor, 13, 1 << 16);
    set_actor_long(actor, 14, 0);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_child_jump_frameouts_when_vertical_side_changes(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->r_no0 = 6;
    actor->actfree[0] = 1;
    actor->actfree[21] = 0;
    actor->yposi.w.h = 200;
    set_actor_word(actor, 24, 200);
    set_actor_long(actor, 13, -1 << 16);
    set_actor_long(actor, 14, 0);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_child_jump_moves_horizontally_and_frameouts_on_mismatch(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_tekkyu7j_state();
    actor->userflag.b.l = -1;
    actor->r_no0 = 6;
    actor->actfree[0] = 0;
    actor->actfree[21] = 255;
    actor->xposi.w.h = 200;
    set_actor_word(actor, 24, 200);
    set_actor_long(actor, 13, 1 << 16);
    set_actor_long(actor, 14, 0);

    tekkyu7j(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_tekkyu7j_tables_capture_literal_data(&ctx);
    test_launcher_initializes_and_counts_down(&ctx);
    test_launcher_spawns_child_when_timer_reaches_zero(&ctx);
    test_launcher_allocation_failure_only_resets_timer(&ctx);
    test_child_init_uses_vertical_even_direction(&ctx);
    test_child_init_covers_positive_and_horizontal_directions(&ctx);
    test_child_move_waits_then_enters_stop(&ctx);
    test_child_stop_waits_then_enters_jump(&ctx);
    test_child_jump_moves_vertically_without_frameout_when_side_matches(&ctx);
    test_child_jump_frameouts_when_vertical_side_changes(&ctx);
    test_child_jump_moves_horizontally_and_frameouts_on_mismatch(&ctx);
TEST_MAIN_END

#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/tekkyu4.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 1024;
    sinset_cos = 2048;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
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

static int legacy_word_actfree_offset(int word_index) {
    return (word_index * 2) - (int)offsetof(sprite_status, actfree);
}

static void set_legacy_word(sprite_status *actor, int word_index, Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void assert_init_common(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 169, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 880, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_tekkyu4);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
}

static void spawn_tekkyu4_chain(sprite_status *main_actor,
                                sprite_status *child1,
                                sprite_status *child2,
                                sprite_status *child3) {
    queue_actwkchk(child3);
    queue_actwkchk(child2);
    queue_actwkchk(child1);
    main_actor->actno = 56;
    main_actor->xposi.w.h = 100;
    main_actor->yposi.w.h = 200;
    tekkyu4(main_actor);
    reset_logs();
}

static void test_tekkyu4_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tekkyu4[0] == &tekkyu4_pat0);
    TEST_ASSERT_EQ_INT(ctx, 1, tekkyu4_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu4_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, tekkyu4_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, tekkyu4_pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 474, tekkyu4_pat0.spra[0].index);
}

static void test_tekkyu4_init_builds_four_link_chain(test_context *ctx) {
    sprite_status *main_actor = &actwk[4];
    sprite_status *child1 = &actwk[20];
    sprite_status *child2 = &actwk[21];
    sprite_status *child3 = &actwk[22];

    reset_state();
    spawn_tekkyu4_chain(main_actor, child1, child2, child3);

    TEST_ASSERT_EQ_INT(ctx, 1, main_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 2, child1->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 3, child2->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 4, child3->sprpri);
    assert_init_common(ctx, main_actor);
    assert_init_common(ctx, child1);
    assert_init_common(ctx, child2);
    assert_init_common(ctx, child3);
    TEST_ASSERT_EQ_INT(ctx, 100, get_legacy_word(main_actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 200, get_legacy_word(main_actor, 24));
    TEST_ASSERT_EQ_INT(ctx, -2048, get_legacy_word(child1, 25));
    TEST_ASSERT_EQ_INT(ctx, -4096, get_legacy_word(child2, 25));
    TEST_ASSERT_EQ_INT(ctx, -6144, get_legacy_word(child3, 25));
}

static void test_tekkyu4_init_reverses_direction_from_userflag(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[4];
    sprite_status *child1 = &actwk[20];

    reset_state();
    main_actor->actno = 56;
    main_actor->userflag.b.h = (Sint8)130;
    queue_actwkchk(&actwk[22]);
    queue_actwkchk(&actwk[21]);
    queue_actwkchk(child1);

    tekkyu4(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 2048, get_legacy_word(child1, 25));
    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(main_actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 126, get_legacy_word(main_actor, 27));
}

static void test_tekkyu4_init_frames_out_on_allocation_failures(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[4];

    reset_state();
    main_actor->actno = 56;
    tekkyu4(main_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);

    reset_state();
    main_actor = &actwk[4];
    main_actor->actno = 56;
    queue_actwkchk(&actwk[22]);
    tekkyu4(main_actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);

    reset_state();
    main_actor = &actwk[4];
    main_actor->actno = 56;
    queue_actwkchk(&actwk[22]);
    queue_actwkchk(&actwk[21]);
    tekkyu4(main_actor);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);
}

static void test_tekkyu4_main_move_uses_sine_and_frames_out(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[4];

    reset_state();
    main_actor->r_no0 = 2;
    set_legacy_word(main_actor, 23, 100);
    set_legacy_word(main_actor, 24, 200);
    set_legacy_word(main_actor, 25, 0);
    set_legacy_word(main_actor, 26, 256);
    set_legacy_word(main_actor, 27, 2);
    sinset_sin = 1024;
    sinset_cos = 2048;

    tekkyu4(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 612, main_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, main_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == main_actor);
}

static void test_tekkyu4_child_moves_when_parent_is_alive(test_context *ctx) {
    sprite_status *main_actor = &actwk[4];
    sprite_status *child1 = &actwk[20];

    reset_state();
    spawn_tekkyu4_chain(main_actor, child1, &actwk[21], &actwk[22]);
    sinset_sin = 256;
    sinset_cos = 512;

    tekkyu4(child1);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 249, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 612, child1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, child1->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child1);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_tekkyu4_child_frames_out_when_parent_is_not_active(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[4];
    sprite_status *child1 = &actwk[20];

    reset_state();
    spawn_tekkyu4_chain(main_actor, child1, &actwk[21], &actwk[22]);
    main_actor->actno = 55;

    tekkyu4(child1);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child1);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
}

static void test_tekkyu4_act_init_sub_copies_parent_motion_fields(
    test_context *ctx) {
    sprite_status parent;
    sprite_status child;

    reset_state();
    memset(&parent, 0, sizeof(parent));
    memset(&child, 0, sizeof(child));
    parent.xposi.w.h = 123;
    parent.yposi.w.h = 234;
    set_legacy_word(&parent, 26, -256);
    set_legacy_word(&parent, 27, 5);

    act_init_sub(&parent, &child);

    assert_init_common(ctx, &child);
    TEST_ASSERT_EQ_INT(ctx, 123, get_legacy_word(&child, 23));
    TEST_ASSERT_EQ_INT(ctx, 234, get_legacy_word(&child, 24));
    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(&child, 26));
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(&child, 27));
}

TEST_MAIN_BEGIN;
    test_tekkyu4_patterns_capture_literal_data(&ctx);
    test_tekkyu4_init_builds_four_link_chain(&ctx);
    test_tekkyu4_init_reverses_direction_from_userflag(&ctx);
    test_tekkyu4_init_frames_out_on_allocation_failures(&ctx);
    test_tekkyu4_main_move_uses_sine_and_frames_out(&ctx);
    test_tekkyu4_child_moves_when_parent_is_alive(&ctx);
    test_tekkyu4_child_frames_out_when_parent_is_not_active(&ctx);
    test_tekkyu4_act_init_sub_copies_parent_motion_fields(&ctx);
TEST_MAIN_END

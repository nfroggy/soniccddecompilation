#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static int actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r4/tekkyu.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_pos >= actwkchk_queue_count) {
        *ppActwk = 0;
        return 1;
    }

    *ppActwk = &actwk[actwkchk_queue[actwkchk_queue_pos++]];
    return 0;
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
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 1024;
    sinset_cos = 2048;
}

static void queue_children(int first_index, int count) {
    int i;

    actwkchk_queue_count = count;
    for (i = 0; i < count; ++i) {
        actwkchk_queue[i] = first_index + i;
    }
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

static void spawn_tekkyu(sprite_status *main_actor) {
    queue_children(20, 6);
    main_actor->actno = 54;
    main_actor->xposi.w.h = 100;
    main_actor->yposi.w.h = 200;
    tekkyu(main_actor);
}

static void test_tekkyu_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_tekkyu[0] == &tekkyu_pat00);
    TEST_ASSERT_TRUE(ctx, pat_tyuusin == pat_tekkyu);
    TEST_ASSERT_TRUE(ctx, pat_kusari[0] == &kusari_pat00);
    TEST_ASSERT_TRUE(ctx, pat_tekkyu1[0] == &tekkyu1_pat00);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU_BASE, tekkyu_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU_BASE + 1, kusari_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_TEKKYU_BASE + 2, tekkyu1_pat00.spra[0].index);
}

static void test_tekkyu_main_init_spawns_chain_and_moves(test_context *ctx) {
    sprite_status *main_actor = &actwk[5];
    sprite_status *first_child = &actwk[20];
    sprite_status *last_child = &actwk[25];

    reset_state();
    spawn_tekkyu(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, main_actor->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, main_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, main_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, main_actor->sprvsize);
    TEST_ASSERT_TRUE(ctx, main_actor->patbase == pat_tyuusin);
    TEST_ASSERT_EQ_INT(ctx, 872, main_actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(main_actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 256, get_legacy_word(main_actor, 24));
    TEST_ASSERT_EQ_INT(ctx, 20, get_legacy_word(main_actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 25, get_legacy_word(main_actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == main_actor);

    TEST_ASSERT_EQ_INT(ctx, 54, first_child->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, first_child->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 8, first_child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, first_child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(first_child, 33));
    TEST_ASSERT_EQ_INT(ctx, 54, last_child->actno);
    TEST_ASSERT_EQ_INT(ctx, -2, last_child->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(last_child, 33));
}

static void test_tekkyu_main_init_reverses_speed_for_nonzero_userflag(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[5];

    reset_state();
    queue_children(20, 6);
    main_actor->actno = 54;
    main_actor->userflag.b.h = 1;

    tekkyu(main_actor);

    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(main_actor, 23));
    TEST_ASSERT_EQ_INT(ctx, -256, get_legacy_word(main_actor, 24));
    TEST_ASSERT_EQ_INT(ctx, 255, sinset_angle);
}

static void test_tekkyu_main_init_frames_out_on_allocation_failure(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[5];

    reset_state();
    main_actor->actno = 54;

    tekkyu(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_tekkyu_existing_main_moves_without_allocation(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[5];

    reset_state();
    main_actor->r_no0 = 2;
    main_actor->xposi.w.h = 300;
    main_actor->yposi.w.h = 400;
    set_legacy_word(main_actor, 23, 512);
    set_legacy_word(main_actor, 24, 256);
    set_legacy_word(main_actor, 25, 20);
    set_legacy_word(main_actor, 26, 21);
    set_legacy_word(main_actor, 27, 22);
    set_legacy_word(main_actor, 28, 23);
    set_legacy_word(main_actor, 29, 24);
    set_legacy_word(main_actor, 30, 25);
    sinset_sin = 512;
    sinset_cos = 1024;

    tekkyu(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 768, get_legacy_word(main_actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 3, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_tekkyu_chain_option_initializes_and_exposes_motion(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[5];
    sprite_status *chain = &actwk[20];

    reset_state();
    spawn_tekkyu(main_actor);
    actionsub_count = 0;
    frameout_s_count = 0;
    frameout_count = 0;
    sinset_count = 0;

    tekkyu(chain);

    TEST_ASSERT_EQ_INT(ctx, 2, chain->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, chain->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, chain->sprpri);
    TEST_ASSERT_TRUE(ctx, chain->patbase == pat_kusari);
    TEST_ASSERT_EQ_INT(ctx, 872, chain->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 228, chain->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 264, chain->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == chain);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_tekkyu_ball_option_initializes_collision_sprite(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[5];
    sprite_status *ball = &actwk[25];

    reset_state();
    spawn_tekkyu(main_actor);
    actionsub_count = 0;
    frameout_count = 0;

    tekkyu(ball);

    TEST_ASSERT_EQ_INT(ctx, 2, ball->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, ball->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 2, ball->sprpri);
    TEST_ASSERT_TRUE(ctx, ball->patbase == pat_tekkyu1);
    TEST_ASSERT_EQ_INT(ctx, 880, ball->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 169, ball->colino);
    TEST_ASSERT_EQ_INT(ctx, 868, ball->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 584, ball->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_tekkyu_option_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *chain = &actwk[20];

    reset_state();
    chain->r_no0 = 2;
    chain->userflag.b.h = -1;
    set_legacy_word(chain, 24, 123);
    set_legacy_word(chain, 26, 456);
    set_legacy_word(chain, 33, 5);
    actwk[5].actno = 0;

    tekkyu(chain);

    TEST_ASSERT_EQ_INT(ctx, 123, chain->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, chain->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == chain);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_tekkyu_patterns_capture_literal_data(&ctx);
    test_tekkyu_main_init_spawns_chain_and_moves(&ctx);
    test_tekkyu_main_init_reverses_speed_for_nonzero_userflag(&ctx);
    test_tekkyu_main_init_frames_out_on_allocation_failure(&ctx);
    test_tekkyu_existing_main_moves_without_allocation(&ctx);
    test_tekkyu_chain_option_initializes_and_exposes_motion(&ctx);
    test_tekkyu_ball_option_initializes_collision_sprite(&ctx);
    test_tekkyu_option_frames_out_when_parent_is_gone(&ctx);
TEST_MAIN_END

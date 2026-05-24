#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[2];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r4/baneiwa.c"

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

    *ppActwk = actwkchk_queue[actwkchk_queue_pos++];
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

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return 0;
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
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
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

static void set_legacy_word(sprite_status *actor, int word_index,
                            Sint16 value) {
    set_actfree_word(actor, legacy_word_actfree_offset(word_index), value);
}

static Sint16 get_legacy_word(sprite_status *actor, int word_index) {
    return get_actfree_word(actor, legacy_word_actfree_offset(word_index));
}

static void test_baneiwa_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, baneiwapat[0] == &pat0);
    TEST_ASSERT_EQ_INT(ctx, 1, pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -32, pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 500, pat0.spra[0].index);
}

static void test_baneiwa_init_spawns_child_and_draws_master(test_context *ctx) {
    sprite_status *master = &actwk[5];
    sprite_status *child = &actwk[20];

    reset_state();
    master->xposi.w.h = 100;
    master->yposi.w.h = 200;
    queue_actwkchk(child);

    baneiwa(master);

    TEST_ASSERT_EQ_INT(ctx, 2, master->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, master->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 17514, master->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, master->sprpri);
    TEST_ASSERT_TRUE(ctx, master->patbase == baneiwapat);
    TEST_ASSERT_EQ_INT(ctx, 16, master->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, master->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 72, child->actno);
    TEST_ASSERT_EQ_INT(ctx, 132, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, child->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, child->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 5, get_legacy_word(child, 28));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_baneiwa_init_allocation_failure_still_draws_master(
    test_context *ctx) {
    sprite_status *master = &actwk[5];

    reset_state();
    master->xposi.w.h = 100;

    baneiwa(master);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_baneiwa_child_entry_validates_parent(test_context *ctx) {
    sprite_status *child = &actwk[20];
    sprite_status *master = &actwk[5];

    reset_state();
    child->actfree[18] = 1;
    set_legacy_word(child, 28, 5);
    master->actno = 72;

    baneiwa(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_state();
    child->actfree[18] = 1;
    set_legacy_word(child, 28, 5);
    master->actno = 0;

    baneiwa(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_baneiwa_move_starts_main_motion_segment(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();

    baneiwa_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 128, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, -4, get_legacy_word(actor, 33));
    TEST_ASSERT_EQ_INT(ctx, 512, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
}

static void test_baneiwa_move_inverts_table_for_child(test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->actfree[18] = 1;
    actor->actfree[17] = 2;

    baneiwa_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 128, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, -4, get_legacy_word(actor, 33));
    TEST_ASSERT_EQ_INT(ctx, 512, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

static void test_baneiwa_move_active_timer_negative_speed_checks_before(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->actfree[16] = 2;
    actor->yposi.l = 100 << 16;
    actor->yspeed.w = -16;
    set_legacy_word(actor, 33, 4);

    baneiwa_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -12, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == actor);
}

static void test_baneiwa_move_active_timer_advances_or_wraps_segment(
    test_context *ctx) {
    sprite_status *actor = &actwk[5];

    reset_state();
    actor->actfree[16] = 1;
    actor->actfree[17] = 1;
    actor->yspeed.w = 0;
    set_legacy_word(actor, 33, 4);

    baneiwa_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);

    reset_state();
    actor->actfree[16] = 1;
    actor->actfree[17] = 3;
    actor->yspeed.w = 0;
    set_legacy_word(actor, 33, 4);

    baneiwa_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
}

TEST_MAIN_BEGIN;
    test_baneiwa_patterns_capture_literal_data(&ctx);
    test_baneiwa_init_spawns_child_and_draws_master(&ctx);
    test_baneiwa_init_allocation_failure_still_draws_master(&ctx);
    test_baneiwa_child_entry_validates_parent(&ctx);
    test_baneiwa_move_starts_main_motion_segment(&ctx);
    test_baneiwa_move_inverts_table_for_child(&ctx);
    test_baneiwa_move_active_timer_negative_speed_checks_before(&ctx);
    test_baneiwa_move_active_timer_advances_or_wraps_segment(&ctx);
TEST_MAIN_END

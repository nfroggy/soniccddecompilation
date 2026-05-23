#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/r3/rollplat.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_rollplat_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void reset_rollplat_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
}

static void test_rollplat_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg00);
    TEST_ASSERT_TRUE(ctx, pchg[1] == pchg01);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg00[5]);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg01[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg01[5]);
    TEST_ASSERT_TRUE(ctx, kaitenban_pat[0] == &kpat00);
    TEST_ASSERT_TRUE(ctx, kaitenban_pat[3] == &kpat03);
    TEST_ASSERT_EQ_INT(ctx, -16, kpat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, kpat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 316, kpat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, kpat03.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, kpat03.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 319, kpat03.spra[0].index);
}

static void test_rollplat_initializes_without_available_child(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_rollplat_state();
    actor->actno = 55;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 400;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_rollplat_initializes_special_actor_and_first_child(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *child = &actwk[7];

    reset_rollplat_state();
    actor->actno = 55;
    actor->userflag.b.h = -1;
    actor->xposi.w.h = 200;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 100;
    queue_actor(child);

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_rollplat_move_misses_player_by_y_and_x(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_rollplat_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_rollplat_state();
    actor = &actwk[2];
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 100;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_rollplat_move_ignores_player_when_contact_latch_is_set(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_rollplat_state();
    actor->r_no0 = 2;
    actor->actfree[20] = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_rollplat_contact_sets_rotation_and_spawns_next_plate(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *child = &actwk[8];

    reset_rollplat_state();
    actor->actno = 55;
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    set_actfree_word(actor, 4, -2);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = 0;
    queue_actor(child);

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
}

static void test_rollplat_upward_player_uses_reverse_rotation_without_spawn(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_rollplat_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    set_actfree_word(actor, 0, 5);
    set_actfree_word(actor, 4, -2);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].yspeed.w = -1;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_rollplat_third_spawn_releases_anchor_plate(test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *child = &actwk[9];
    sprite_status *anchor = &actwk[6];

    reset_rollplat_state();
    actor->actno = 55;
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    set_actfree_word(actor, 4, 2);
    set_actfree_word(actor, 2, 6);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    queue_actor(child);

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_rollplat_completed_cycle_releases_all_linked_plates(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_rollplat_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 100;
    set_actfree_word(actor, 4, 4);
    set_actfree_word(actor, 2, 6);
    set_actfree_word(actor, 6, 7);
    set_actfree_word(actor, 8, 8);
    set_actfree_word(actor, 10, 9);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;

    kaitenban(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[6].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[7].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[8].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

TEST_MAIN_BEGIN;
    test_rollplat_tables_capture_literal_data(&ctx);
    test_rollplat_initializes_without_available_child(&ctx);
    test_rollplat_initializes_special_actor_and_first_child(&ctx);
    test_rollplat_move_misses_player_by_y_and_x(&ctx);
    test_rollplat_move_ignores_player_when_contact_latch_is_set(&ctx);
    test_rollplat_contact_sets_rotation_and_spawns_next_plate(&ctx);
    test_rollplat_upward_player_uses_reverse_rotation_without_spawn(&ctx);
    test_rollplat_third_spawn_releases_anchor_plate(&ctx);
    test_rollplat_completed_cycle_releases_all_linked_plates(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int ridechk_count;
static sprite_status *ridechk_actor;
static sprite_status *ridechk_player;
static Sint16 ridechk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r8/konbea83.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
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

Sint16 ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ridechk_count;
    ridechk_actor = pActwk;
    ridechk_player = pPlayerwk;
    return ridechk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    ridechk_count = 0;
    ridechk_actor = 0;
    ridechk_player = 0;
    ridechk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_konbea83_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void test_konbea83_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_EQ_INT(ctx, 60, a_tbl_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, a_tbl_00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, a_tbl_01[1]);
    TEST_ASSERT_EQ_INT(ctx, 32, a_tbl_01[3]);
    TEST_ASSERT_TRUE(ctx, a_tbl0[0] == a_tbl_00);
    TEST_ASSERT_TRUE(ctx, a_tbl0[1] == a_tbl_01);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_00[13]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg_00);
    TEST_ASSERT_TRUE(ctx, pat_konbea[0] == &pat_00);
    TEST_ASSERT_TRUE(ctx, pat_konbea[6] == &pat_00);
    TEST_ASSERT_EQ_INT(ctx, -16, pat_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 507, pat_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 512, pat_05.spra[0].index);
}

static void test_konbea83_initializes_single_platform(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea83_state();
    actor->actno = 42;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 844, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_konbea);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_konbea83_initializes_child_platform(test_context *ctx) {
    sprite_status *actor = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea83_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    queue_actor(child);

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 42, child->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, child->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 132, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, child->yposi.w.h);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_konbea);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_konbea83_child_frames_out_when_parent_missing(
    test_context *ctx) {
    sprite_status *child = &actwk[20];

    reset_konbea83_state();
    child->userflag.b.l = -1;
    set_actfree_word(child, 0, 3);
    actwk[3].actno = 0;

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

static void test_konbea83_child_frames_out_when_origin_mismatches(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *child = &actwk[20];

    reset_konbea83_state();
    parent->actno = 42;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    set_actfree_word(parent, 4, 100);
    set_actfree_word(parent, 6, 200);
    child->userflag.b.l = -1;
    set_actfree_word(child, 0, 3);
    set_actfree_word(child, 4, 101);
    set_actfree_word(child, 6, 200);

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_konbea83_state();
    parent->actno = 42;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    set_actfree_word(parent, 4, 100);
    set_actfree_word(parent, 6, 200);
    child->userflag.b.l = -1;
    set_actfree_word(child, 0, 3);
    set_actfree_word(child, 4, 100);
    set_actfree_word(child, 6, 201);

    konbea(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_konbea83_parent_frames_out_when_child_allocation_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea83_state();
    actwk[0].actno = 42;
    actor->actno = 42;
    actor->userflag.b.l = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_konbea83_stop_waits_and_clears_ride_on_expiry(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea83_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    ridechk_result = 1;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_TRUE(ctx, ridechk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ridechk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    actor->r_no0 = 4;
    set_actfree_word(actor, 2, 1);
    actor->actfree[20] = 255;
    ridechk_result = 1;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->mstno.w);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);

    reset_logs();
    actor->r_no0 = 4;
    actor->sprvsize = 5;
    set_actfree_word(actor, 2, 2);
    actor->actfree[20] = 255;
    ridechk_result = 0;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ridechk_count);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_konbea83_move_animates_and_resets_on_zero_pattern(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_konbea83_state();
    actor->r_no0 = 6;
    actor->patno = 0;
    actor->xposi.w.h = 100;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == pchg);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);

    reset_logs();
    actor->r_no0 = 6;
    actor->patno = 3;

    konbea(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

TEST_MAIN_BEGIN;
    test_konbea83_tables_capture_literal_data(&ctx);
    test_konbea83_initializes_single_platform(&ctx);
    test_konbea83_initializes_child_platform(&ctx);
    test_konbea83_child_frames_out_when_parent_missing(&ctx);
    test_konbea83_child_frames_out_when_origin_mismatches(&ctx);
    test_konbea83_parent_frames_out_when_child_allocation_fails(&ctx);
    test_konbea83_stop_waits_and_clears_ride_on_expiry(&ctx);
    test_konbea83_move_animates_and_resets_on_zero_pattern(&ctx);
TEST_MAIN_END

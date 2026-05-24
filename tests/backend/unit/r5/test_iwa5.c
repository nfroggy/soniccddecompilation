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
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_result;
static int actwkchk_count;
static sprite_status *actwkchk_queue[4];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_request;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r5/iwa5.c"

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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
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

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_request = ReqNo;
}

static void reset_iwa5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    emycol_d_result = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    soundset_request = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)((bits >> 8) & 255);
    actor->actfree[offset + 2] = (Uint8)((bits >> 16) & 255);
    actor->actfree[offset + 3] = (Uint8)((bits >> 24) & 255);
}

static Sint32 actfree_long(sprite_status *actor, int offset) {
    Uint32 bits = (Uint32)actor->actfree[offset] |
                  ((Uint32)actor->actfree[offset + 1] << 8) |
                  ((Uint32)actor->actfree[offset + 2] << 16) |
                  ((Uint32)actor->actfree[offset + 3] << 24);
    return (Sint32)bits;
}

static void assert_normal_tail_callbacks(test_context *ctx,
                                         sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_init_sets_fields_and_waits(test_context *ctx) {
    sprite_status *rock = &actwk[3];

    reset_iwa5_state();
    rock->xposi.w.h = 100;
    rock->yposi.w.h = 200;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 2, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, rock->actflg);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->patno);
    TEST_ASSERT_EQ_INT(ctx, 3, rock->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, rock->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 16, rock->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, rock->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17202, rock->sproffset);
    TEST_ASSERT_TRUE(ctx, rock->patbase == pat_iwa5);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == rock);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    assert_normal_tail_callbacks(ctx, rock);
}

static void test_wait_arms_only_when_player_is_inside_trigger_box(
    test_context *ctx) {
    sprite_status *rock = &actwk[3];

    reset_iwa5_state();
    rock->r_no0 = 2;
    rock->userflag.b.h = 1;
    rock->xposi.w.h = 100;
    rock->yposi.w.h = 200;
    actwk[0].xposi.w.h = 92;
    actwk[0].yposi.w.h = 208;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 4, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 248, rock->colino);
    assert_normal_tail_callbacks(ctx, rock);

    reset_iwa5_state();
    rock->r_no0 = 2;
    rock->userflag.b.h = 1;
    rock->xposi.w.h = 100;
    rock->yposi.w.h = 200;
    actwk[0].xposi.w.h = 92;
    actwk[0].yposi.w.h = 400;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 2, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->colino);

    reset_iwa5_state();
    rock->r_no0 = 2;
    rock->userflag.b.h = 1;
    rock->xposi.w.h = 100;
    rock->yposi.w.h = 200;
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 208;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 2, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->colino);
}

static void test_fall_solid_floor_returns_or_frames_out(test_context *ctx) {
    sprite_status *rock = &actwk[3];

    reset_iwa5_state();
    rock->r_no0 = 4;
    rock->xposi.l = 100 << 16;
    rock->yposi.l = 100 << 16;
    set_actfree_long(rock, 4, 65536);
    actwk[0].yposi.w.h = 150;
    emycol_d_result = 0;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == rock);
    TEST_ASSERT_EQ_INT(ctx, 101 << 16, rock->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 81920, actfree_long(rock, 4));
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_normal_tail_callbacks(ctx, rock);

    reset_iwa5_state();
    rock->r_no0 = 4;
    rock->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;
    emycol_d_result = 0;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == rock);
    assert_normal_tail_callbacks(ctx, rock);
}

static void test_fall_breaks_into_four_fragments(test_context *ctx) {
    sprite_status *rock = &actwk[3];

    reset_iwa5_state();
    rock->actno = 40;
    rock->r_no0 = 4;
    rock->actflg = 132;
    rock->sproffset = 17202;
    rock->patbase = pat_iwa5;
    rock->colino = 248;
    rock->xposi.l = 100 << 16;
    rock->yposi.l = 200 << 16;
    set_actfree_long(rock, 4, 0);
    emycol_d_result = -1;
    for (int i = 0; i < 4; ++i) {
        queue_actor(&actwk[40 + i]);
    }

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 24, rock->actno);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, rock->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_request);

    TEST_ASSERT_EQ_INT(ctx, 40, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[40].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[40].patno);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -65536, actfree_long(&actwk[40], 0));
    TEST_ASSERT_EQ_INT(ctx, -262144, actfree_long(&actwk[40], 4));

    TEST_ASSERT_EQ_INT(ctx, 3, actwk[41].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[41].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, actwk[41].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 65536, actfree_long(&actwk[41], 0));

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[42].patno);
    TEST_ASSERT_EQ_INT(ctx, 92, actwk[42].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, actwk[42].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -131072, actfree_long(&actwk[42], 0));

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[43].patno);
    TEST_ASSERT_EQ_INT(ctx, 108, actwk[43].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, actwk[43].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 131072, actfree_long(&actwk[43], 0));
    TEST_ASSERT_EQ_INT(ctx, -196608, actfree_long(&actwk[43], 4));
    assert_normal_tail_callbacks(ctx, rock);
}

static void test_fall_allocation_failure_still_resets_parent(test_context *ctx) {
    sprite_status *rock = &actwk[3];

    reset_iwa5_state();
    rock->actno = 40;
    rock->r_no0 = 4;
    rock->actflg = 4;
    rock->colino = 248;
    rock->xposi.l = 100 << 16;
    rock->yposi.l = 200 << 16;
    emycol_d_result = -1;

    iwa5(rock);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 24, rock->actno);
    TEST_ASSERT_EQ_INT(ctx, 0, rock->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, rock->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_normal_tail_callbacks(ctx, rock);
}

static void test_fragment_motion_uses_high_userflag_path(test_context *ctx) {
    sprite_status *fragment = &actwk[3];

    reset_iwa5_state();
    fragment->userflag.b.h = -1;
    fragment->xposi.l = 100 << 16;
    fragment->yposi.l = 200 << 16;
    set_actfree_long(fragment, 0, 2 << 16);
    set_actfree_long(fragment, 4, -1 << 16);
    actwk[0].yposi.w.h = 200;

    iwa5(fragment);

    TEST_ASSERT_EQ_INT(ctx, 102, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 199, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, (Sint32)((-1 << 16) + 16384),
                       actfree_long(fragment, 4));
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_iwa5_state();
    fragment->userflag.b.h = -1;
    fragment->yposi.l = 500 << 16;
    actwk[0].yposi.w.h = 200;

    iwa5(fragment);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_init_sets_fields_and_waits(&ctx);
    test_wait_arms_only_when_player_is_inside_trigger_box(&ctx);
    test_fall_solid_floor_returns_or_frames_out(&ctx);
    test_fall_breaks_into_four_fragments(&ctx);
    test_fall_allocation_failure_still_resets_parent(&ctx);
    test_fragment_motion_uses_high_userflag_path(&ctx);
TEST_MAIN_END

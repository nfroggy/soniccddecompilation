#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union editmode;
Uint8 colrevflag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[4];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/r5/beltsw5.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 4) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_beltsw5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&editmode, 0, sizeof(editmode));
    colrevflag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_callbacks(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void assert_common_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_beltsw5_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, beltsw5pat[0] == &beltsw5_pat0);
    TEST_ASSERT_TRUE(ctx, beltsw5pat[1] == &beltsw5_pat1);
    TEST_ASSERT_TRUE(ctx, beltsw5pat[2] == &beltsw5_pat2);
    TEST_ASSERT_EQ_INT(ctx, 1, beltsw5_pat0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -4, beltsw5_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, beltsw5_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 0, beltsw5_pat0.spra[0].etc);
    TEST_ASSERT_EQ_INT(ctx, 406, beltsw5_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, beltsw5_pat1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -36, beltsw5_pat1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 407, beltsw5_pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -28, beltsw5_pat2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, beltsw5_pat2.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 408, beltsw5_pat2.spra[0].index);
}

static void test_beltsw5_initializes_and_records_current_direction(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_beltsw5_state();
    editmode.b.h = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 18327, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == beltsw5pat);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[18]);
    assert_common_callbacks(ctx, actor);

    reset_beltsw5_state();
    editmode.b.h = 1;
    colrevflag = 1;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 50;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[18]);
    assert_common_callbacks(ctx, actor);
}

static void test_beltsw5_move1_toggles_when_player_overlaps(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_beltsw5_state();
    actor->r_no0 = 2;
    actor->actflg = 128;
    actor->xposi.w.h = 80;
    actor->yposi.w.h = 40;
    actor->actfree[18] = 0;
    actwk[0].xposi.w.h = 87;
    actwk[0].yposi.w.h = 55;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, colrevflag);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 191, soundset_requests[0]);
    assert_common_callbacks(ctx, actor);

    reset_callbacks();
    colrevflag = 0;
    actor->r_no0 = 2;
    actor->actflg = 0;
    actor->xposi.w.h = 80;
    actor->yposi.w.h = 40;
    actor->actfree[18] = 0;
    actwk[0].xposi.w.h = 73;
    actwk[0].yposi.w.h = 25;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, colrevflag);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, actor);
}

static void test_beltsw5_move1_tracks_external_direction_change(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_beltsw5_state();
    actor->r_no0 = 2;
    actor->xposi.w.h = 80;
    actor->yposi.w.h = 40;
    actor->actfree[18] = 0;
    colrevflag = 1;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 100;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, colrevflag);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_common_callbacks(ctx, actor);
}

static void test_beltsw5_move2_animates_and_finishes_cycle(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_beltsw5_state();
    actor->r_no0 = 4;
    actor->actfree[16] = 6;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 7, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    assert_common_callbacks(ctx, actor);

    reset_callbacks();
    colrevflag = 1;
    actor->r_no0 = 4;
    actor->actfree[16] = 13;
    actor->actfree[19] = 255;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[19]);
    assert_common_callbacks(ctx, actor);
}

static void test_beltsw5_move3_waits_until_player_leaves(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_beltsw5_state();
    actor->r_no0 = 6;
    actor->xposi.w.h = 80;
    actor->yposi.w.h = 40;
    actor->actfree[18] = 0;
    actwk[0].xposi.w.h = 95;
    actwk[0].yposi.w.h = 71;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[18]);
    assert_common_callbacks(ctx, actor);

    reset_callbacks();
    actor->r_no0 = 6;
    actor->xposi.w.h = 80;
    actor->yposi.w.h = 40;
    actwk[0].xposi.w.h = 65;
    actwk[0].yposi.w.h = 9;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[18]);
    assert_common_callbacks(ctx, actor);

    reset_callbacks();
    colrevflag = 1;
    actwk[0].xposi.w.h = 96;
    actwk[0].yposi.w.h = 72;

    beltsw5(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[18]);
    assert_common_callbacks(ctx, actor);
}

TEST_MAIN_BEGIN;
    test_beltsw5_patterns_capture_literal_data(&ctx);
    test_beltsw5_initializes_and_records_current_direction(&ctx);
    test_beltsw5_move1_toggles_when_player_overlaps(&ctx);
    test_beltsw5_move1_tracks_external_direction_change(&ctx);
    test_beltsw5_move2_animates_and_finishes_cycle(&ctx);
    test_beltsw5_move3_waits_until_player_leaves(&ctx);
TEST_MAIN_END

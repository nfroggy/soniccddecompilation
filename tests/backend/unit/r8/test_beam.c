#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 chibi_flag;
short_union editmode;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_new_actor;
static Sint32 actwkchk2_result;
static int actwkchk_count;
static sprite_status *actwkchk_new_actor;
static Sint32 actwkchk_result;
static int soundset_count;
static Sint16 soundset_requests[8];
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/beam.c"

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
    ++actwkchk2_count;
    actwkchk2_actor = pActwk;
    if (actwkchk2_result != 0) {
        return actwkchk2_result;
    }
    *ppNewActwk = actwkchk2_new_actor;
    return 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_result != 0) {
        return actwkchk_result;
    }
    *ppActwk = actwkchk_new_actor;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    actwkchk2_new_actor = &actwk[10];
    actwkchk2_result = 0;
    actwkchk_count = 0;
    actwkchk_new_actor = &actwk[11];
    actwkchk_result = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    emycol_d_result = 0;
}

static void reset_beam_state(void) {
    memset(actwk, 0, sizeof(actwk));
    chibi_flag = 0;
    memset(&editmode, 0, sizeof(editmode));
    reset_logs();
}

static void assert_outer_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_beam_tables_capture_animation_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, beampat[0] == &bea00);
    TEST_ASSERT_TRUE(ctx, beampat[4] == &bea04);
    TEST_ASSERT_TRUE(ctx, beampat0[4] == &bea1d);
    TEST_ASSERT_EQ_INT(ctx, 2, bea00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, bea00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, bea00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 294, bea00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 16, bea14.cnt);
    TEST_ASSERT_EQ_INT(ctx, -124, bea14.spra[15].yoff);
    TEST_ASSERT_EQ_INT(ctx, 298, bea14.spra[15].index);
    TEST_ASSERT_EQ_INT(ctx, 16, bea1c.cnt);
    TEST_ASSERT_EQ_INT(ctx, -124, bea1c.spra[15].yoff);
    TEST_ASSERT_EQ_INT(ctx, 299, bea1c.spra[15].index);
    TEST_ASSERT_EQ_INT(ctx, 2, beamchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, beamchg0[1]);
    TEST_ASSERT_EQ_INT(ctx, 13, beamchg0[2]);
    TEST_ASSERT_EQ_INT(ctx, 255, beamchg0[3]);
    TEST_ASSERT_TRUE(ctx, beamchg[8] == beamchg8);
}

static void test_beam_initializes_master_and_spawns_normal_child(
    test_context *ctx) {
    sprite_status *master = &actwk[2];
    sprite_status *child = &actwk[12];

    reset_beam_state();
    actwkchk2_new_actor = child;
    master->actflg = 128;
    master->xposi.w.h = 33;
    master->yposi.w.h = 47;

    beam(master);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == master);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 197, soundset_requests[0]);
    assert_outer_callbacks(ctx, master);
}

static void test_beam_userflag_initializes_alternate_master_sound(
    test_context *ctx) {
    sprite_status *master = &actwk[2];
    sprite_status *child = &actwk[12];

    reset_beam_state();
    actwkchk2_new_actor = child;
    master->actflg = 128;
    master->userflag.b.h = 1;

    beam(master);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 188, soundset_requests[0]);
}

static void test_beam_master_waits_when_timer_remains(test_context *ctx) {
    sprite_status *master = &actwk[2];

    reset_beam_state();
    master->r_no0 = 2;
    master->actfree[0] = 2;

    beam(master);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_outer_callbacks(ctx, master);
}

static void test_beam_master_falls_back_to_actwkchk_after_actwkchk2_failure(
    test_context *ctx) {
    sprite_status *master = &actwk[2];
    sprite_status *child = &actwk[13];

    reset_beam_state();
    master->r_no0 = 2;
    master->actfree[0] = 1;
    master->xposi.w.h = 77;
    master->yposi.w.h = 31;
    actwkchk2_result = -1;
    actwkchk_new_actor = child;

    beam(master);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_beam_master_stops_when_both_allocators_fail(test_context *ctx) {
    sprite_status *master = &actwk[2];

    reset_beam_state();
    master->r_no0 = 2;
    master->actfree[0] = 1;
    actwkchk2_result = -1;
    actwkchk_result = -1;

    beam(master);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_beam_move0_extends_until_floor_collision(test_context *ctx) {
    sprite_status *segment = &actwk[3];

    reset_beam_state();
    segment->r_no0 = 4;
    segment->mstno.b.h = 6;
    segment->yposi.w.h = 12;
    emycol_d_result = 0;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == segment);
    TEST_ASSERT_TRUE(ctx, patchg_table == beamchg);
    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == segment);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    assert_outer_callbacks(ctx, segment);
}

static void test_beam_move0_creates_endcap_when_collision_missing(
    test_context *ctx) {
    sprite_status *segment = &actwk[3];
    sprite_status *endcap = &actwk[14];

    reset_beam_state();
    actwkchk2_new_actor = endcap;
    segment->r_no0 = 4;
    segment->patbase = beampat0;
    segment->actflg = 1;
    segment->xposi.w.h = 120;
    segment->yposi.w.h = 28;
    segment->userflag.b.h = 2;
    emycol_d_result = -1;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
}

static void test_beam_move0_marks_missing_endcap_when_allocator_fails(
    test_context *ctx) {
    sprite_status *segment = &actwk[3];

    reset_beam_state();
    segment->r_no0 = 4;
    emycol_d_result = -1;
    actwkchk2_result = -1;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
}

static void test_beam_move1_retracts_without_finishing(test_context *ctx) {
    sprite_status *segment = &actwk[3];

    reset_beam_state();
    segment->r_no0 = 6;
    segment->mstno.b.h = 2;
    segment->yposi.w.h = 28;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_outer_callbacks(ctx, segment);
}

static void test_beam_move1_finishes_and_releases_endcap(test_context *ctx) {
    sprite_status *segment = &actwk[3];
    sprite_status *endcap = &actwk[14];

    reset_beam_state();
    actwkchk2_new_actor = endcap;
    segment->r_no0 = 4;
    segment->patbase = beampat;
    segment->xposi.w.h = 120;
    segment->yposi.w.h = 28;
    emycol_d_result = -1;
    beam(segment);
    reset_logs();

    segment->r_no0 = 6;
    segment->mstno.b.h = 0;
    segment->yposi.w.h = 12;
    segment->actfree[21] = 0;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == segment);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_outer_callbacks(ctx, segment);
}

static void test_beam_move1_finishes_without_endcap_when_marked_missing(
    test_context *ctx) {
    sprite_status *segment = &actwk[3];

    reset_beam_state();
    segment->r_no0 = 6;
    segment->mstno.b.h = 0;
    segment->yposi.w.h = 12;
    segment->actfree[21] = 255;

    beam(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == segment);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_beam_move2_frames_out_when_released(test_context *ctx) {
    sprite_status *endcap = &actwk[14];

    reset_beam_state();
    endcap->r_no0 = 8;
    endcap->actfree[0] = 1;

    beam(endcap);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == endcap);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_outer_callbacks(ctx, endcap);
}

static void test_beam_move2_animates_while_attached(test_context *ctx) {
    sprite_status *endcap = &actwk[14];

    reset_beam_state();
    endcap->r_no0 = 8;
    endcap->actfree[0] = 0;

    beam(endcap);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == endcap);
    TEST_ASSERT_TRUE(ctx, patchg_table == beamchg);
    assert_outer_callbacks(ctx, endcap);
}

static void set_player_for_beam_overlap(Sint16 player_x, Sint16 beam_x,
                                        Sint16 player_y, Sint16 beam_y) {
    actwk[0].xposi.w.h = player_x;
    actwk[0].yposi.w.h = player_y;
    actwk[0].sprhs = 9;
    actwk[0].sprvsize = 19;
    actwk[0].mstno.b.h = 0;
    actwk[0].xposi.w.h = player_x;
    actwk[1].xposi.w.h = beam_x;
    actwk[1].yposi.w.h = beam_y;
    actwk[1].mstno.b.h = 0;
}

static void assert_player_was_beamed(test_context *ctx, Sint16 expected_xspeed,
                                     Uint8 expected_chibi,
                                     Uint8 expected_sprvsize,
                                     Uint8 expected_sprhs) {
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, expected_xspeed, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 56, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, expected_chibi, chibi_flag);
    TEST_ASSERT_EQ_INT(ctx, expected_sprvsize, actwk[0].sprvsize);
    TEST_ASSERT_EQ_INT(ctx, expected_sprhs, actwk[0].sprhs);
}

static void test_beam_collision_turns_normal_player_into_chibi(test_context *ctx) {
    sprite_status *beam_actor = &actwk[1];

    reset_beam_state();
    set_player_for_beam_overlap(95, 100, 46, 48);

    beam_coli0(beam_actor);

    assert_player_was_beamed(ctx, -512, 1, 10, 5);
}

static void test_beam_collision_turns_chibi_player_back_to_normal(
    test_context *ctx) {
    sprite_status *beam_actor = &actwk[1];

    reset_beam_state();
    chibi_flag = 1;
    set_player_for_beam_overlap(105, 100, 46, 48);
    beam_actor->userflag.b.h = 1;

    beam_coli0(beam_actor);

    assert_player_was_beamed(ctx, 512, 0, 19, 9);
}

static void test_beam_collision_respects_player_and_mode_gates(
    test_context *ctx) {
    sprite_status *beam_actor = &actwk[1];

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, 46, 48);
    actwk[0].mstno.b.h = 43;
    beam_coli0(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, 46, 48);
    chibi_flag = 1;
    beam_coli0(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, 46, 48);
    beam_actor->userflag.b.h = 1;
    chibi_flag = 0;
    beam_coli0(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, 46, 48);
    editmode.b.h = 1;
    beam_coli0(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);
}

static void test_beam_collision_rejects_non_overlaps(test_context *ctx) {
    sprite_status *beam_actor = &actwk[1];

    reset_beam_state();
    set_player_for_beam_overlap(200, 100, 46, 48);
    beam_coli(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(71, 100, 46, 48);
    beam_coli(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, -20, 48);
    beam_coli(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);

    reset_beam_state();
    set_player_for_beam_overlap(100, 100, 80, 48);
    beam_coli(beam_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].r_no0);
}

TEST_MAIN_BEGIN;
    test_beam_tables_capture_animation_data(&ctx);
    test_beam_initializes_master_and_spawns_normal_child(&ctx);
    test_beam_userflag_initializes_alternate_master_sound(&ctx);
    test_beam_master_waits_when_timer_remains(&ctx);
    test_beam_master_falls_back_to_actwkchk_after_actwkchk2_failure(&ctx);
    test_beam_master_stops_when_both_allocators_fail(&ctx);
    test_beam_move0_extends_until_floor_collision(&ctx);
    test_beam_move0_creates_endcap_when_collision_missing(&ctx);
    test_beam_move0_marks_missing_endcap_when_allocator_fails(&ctx);
    test_beam_move1_retracts_without_finishing(&ctx);
    test_beam_move1_finishes_and_releases_endcap(&ctx);
    test_beam_move1_finishes_without_endcap_when_marked_missing(&ctx);
    test_beam_move2_frames_out_when_released(&ctx);
    test_beam_move2_animates_while_attached(&ctx);
    test_beam_collision_turns_normal_player_into_chibi(&ctx);
    test_beam_collision_turns_chibi_player_back_to_normal(&ctx);
    test_beam_collision_respects_player_and_mode_gates(&ctx);
    test_beam_collision_rejects_non_overlaps(&ctx);
TEST_MAIN_END

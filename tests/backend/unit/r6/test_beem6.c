#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
Uint8 generate_flag;
Uint8 st6clrchg;
short_union stageno;
int_union scra_v_posit;
PALETTEENTRY colorwk[256];
PALETTEENTRY colorwk2[256];
PALETTEENTRY *lpcolorwk = colorwk;
PALETTEENTRY *lpcolorwk2 = colorwk2;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r6/beem6.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = (size_t)short_index * sizeof(Sint16) -
                    offsetof(sprite_status, actfree);

    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = (size_t)short_index * sizeof(Sint16) -
                    offsetof(sprite_status, actfree);

    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void reset_beem6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    generate_flag = 0;
    st6clrchg = 0;
    memset(&stageno, 0, sizeof(stageno));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(colorwk, 0, sizeof(colorwk));
    memset(colorwk2, 0, sizeof(colorwk2));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void assert_palette(test_context *ctx, PALETTEENTRY actual, int red,
                           int green, int blue, int flags,
                           const char *label) {
    if (actual.peRed != red || actual.peGreen != green ||
        actual.peBlue != blue || actual.peFlags != flags) {
        ++ctx->failures;
        fprintf(stderr,
                "%s: expected {%d,%d,%d,%d}, got {%u,%u,%u,%u}\n", label,
                red, green, blue, flags, actual.peRed, actual.peGreen,
                actual.peBlue, actual.peFlags);
    }
}

static void test_beem6_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, beem6pat[0] == &beem6pat0);
    TEST_ASSERT_TRUE(ctx, beem6pat[1] == &nullpat);
    TEST_ASSERT_EQ_INT(ctx, -24, beem6pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, beem6pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 417, beem6pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3808, clrtblA[2]);
    TEST_ASSERT_EQ_INT(ctx, 2720, clrtblA2[3]);
    TEST_ASSERT_EQ_INT(ctx, 2122, clrtblB2[3]);
    TEST_ASSERT_EQ_INT(ctx, 584, clrtblD2[3]);
}

static void test_beem6_init_sets_fields_palette_and_schedules_partner(
    test_context *ctx) {
    sprite_status *beam = &actwk[3];
    sprite_status *partner = &actwk[40];

    reset_beem6_state();
    actwk[0].xposi.w.h = 123;
    actwk[0].yposi.w.h = 456;
    beam->userflag.b.h = 1;
    queue_actor(partner);

    beem6_init(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, beam->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, beam->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 25432, beam->sproffset);
    TEST_ASSERT_TRUE(ctx, beam->patbase == beem6pat);
    TEST_ASSERT_EQ_INT(ctx, 16, beam->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 16, beam->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 1, beam->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 33, partner->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, partner->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 123, partner->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, partner->yposi.w.h);
    assert_palette(ctx, colorwk2[50], 96, 32, 96, 1, "init wk2 50");
    assert_palette(ctx, colorwk2[32], 96, 64, 96, 1, "init wk2 32");
}

static void test_beem6_actsch_existing_and_allocation_failure_paths(
    test_context *ctx) {
    reset_beem6_state();
    actwk[32].actno = 33;
    actwk[32].userflag.b.h = 1;

    beem6_actsch();

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_beem6_state();
    beem6_actsch();

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].actno);
}

static void test_beem6_wait_gates_and_countdown_paths(test_context *ctx) {
    sprite_status *beam = &actwk[3];

    reset_beem6_state();
    beam->userflag.b.h = 0;
    beam->xposi.w.h = 10;
    actwk[0].xposi.w.h = 300;

    beem6_wait(beam);

    TEST_ASSERT_EQ_INT(ctx, 10, beam->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 31));

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beam->r_no0 = 2;
    set_actor_short_alias(beam, 31, 2);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 400;

    beem6_wait(beam);

    TEST_ASSERT_EQ_INT(ctx, 300, beam->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, beam->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(beam, 31));
    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beam->r_no0 = 2;
    beam->actflg = 128;
    set_actor_short_alias(beam, 31, 1);

    beem6_wait(beam);

    TEST_ASSERT_EQ_INT(ctx, 4, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 89, actor_short_alias(beam, 26));
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 178, soundset_requests[0]);

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beam->r_no0 = 2;
    set_actor_short_alias(beam, 31, 1);

    beem6_wait(beam);

    TEST_ASSERT_EQ_INT(ctx, 4, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 89, actor_short_alias(beam, 26));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_beem6_wait_sets_delay_for_time_modes(test_context *ctx) {
    sprite_status *beam = &actwk[3];

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beem6_wait(beam);
    TEST_ASSERT_EQ_INT(ctx, 360, actor_short_alias(beam, 31));

    reset_beem6_state();
    beam->userflag.b.h = 1;
    time_flag = 1;
    beem6_wait(beam);
    TEST_ASSERT_EQ_INT(ctx, 480, actor_short_alias(beam, 31));

    reset_beem6_state();
    beam->userflag.b.h = 1;
    time_flag = 2;
    beem6_wait(beam);
    TEST_ASSERT_EQ_INT(ctx, 240, actor_short_alias(beam, 31));

    reset_beem6_state();
    beam->userflag.b.h = 1;
    time_flag = 2;
    generate_flag = 1;
    beem6_wait(beam);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 31));

    reset_beem6_state();
    beam->userflag.b.h = 1;
    stageno.b.h = -1;
    scra_v_posit.w.h = 1024;
    beem6_wait(beam);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 31));
}

static void test_beem6_move_warning_active_then_beam_active_paths(
    test_context *ctx) {
    sprite_status *beam = &actwk[3];

    reset_beem6_state();
    beam->actfree[18] = 0;
    beam->r_no0 = 4;
    set_actor_short_alias(beam, 26, 2);

    beem6_move(beam);

    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(beam, 26));
    TEST_ASSERT_EQ_INT(ctx, 0, st6clrchg);
    assert_palette(ctx, colorwk[50], 0, 128, 0, 1, "warning color");

    reset_beem6_state();
    beam->r_no0 = 4;
    set_actor_short_alias(beam, 26, 1);
    set_actor_short_alias(beam, 31, 2);

    beem6_move(beam);

    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 26));
    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(beam, 31));
    TEST_ASSERT_EQ_INT(ctx, 1, st6clrchg);
    assert_palette(ctx, colorwk[50], 0, 0, 0, 1, "beam channel color");

    reset_beem6_state();
    beam->r_no0 = 4;
    set_actor_short_alias(beam, 31, 1);

    beem6_move(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, st6clrchg);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 31));
    TEST_ASSERT_EQ_INT(ctx, 1, beam->actfree[18]);

    reset_beem6_state();
    beam->r_no0 = 4;
    beam->actfree[18] = 2;
    set_actor_short_alias(beam, 31, 1);
    set_actor_short_alias(beam, 33, 77);

    beem6_move(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, st6clrchg);
    TEST_ASSERT_EQ_INT(ctx, 0, beam->actfree[18]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(beam, 33));
}

static void test_palette_helpers_cover_time_modes_and_table_wraps(
    test_context *ctx) {
    sprite_status *beam = &actwk[3];

    reset_beem6_state();
    clrset0();
    assert_palette(ctx, colorwk[50], 96, 32, 96, 1, "clrset0 normal 50");
    assert_palette(ctx, colorwk[32], 96, 64, 96, 1, "clrset0 normal 32");

    reset_beem6_state();
    time_flag = 1;
    clrset0();
    assert_palette(ctx, colorwk[50], 32, 32, 32, 1, "clrset0 past 50");
    assert_palette(ctx, colorwk[61], 0, 128, 96, 1, "clrset0 past 61");

    reset_beem6_state();
    time_flag = 2;
    clrset0_2();
    assert_palette(ctx, colorwk2[50], 32, 0, 64, 1, "clrset0_2 future 50");
    assert_palette(ctx, colorwk2[61], 96, 64, 32, 1, "clrset0_2 future 61");

    reset_beem6_state();
    time_flag = 1;
    clrset0_2();
    assert_palette(ctx, colorwk2[50], 32, 32, 32, 1, "clrset0_2 past 50");
    assert_palette(ctx, colorwk2[61], 0, 128, 96, 1, "clrset0_2 past 61");

    reset_beem6_state();
    time_flag = 2;
    generate_flag = 1;
    clrset0_2();
    assert_palette(ctx, colorwk2[50], 0, 0, 0, 0, "clrset0_2 generate skip");

    reset_beem6_state();
    time_flag = 2;
    generate_flag = 1;
    clrset0();
    assert_palette(ctx, colorwk[50], 0, 0, 0, 0, "clrset0 generate skip");

    reset_beem6_state();
    beam->actfree[20] = 13;
    beam->actfree[21] = 7;
    clrset1(beam, 0);
    TEST_ASSERT_EQ_INT(ctx, 0, beam->actfree[20]);
    TEST_ASSERT_EQ_INT(ctx, 0, beam->actfree[21]);
    assert_palette(ctx, colorwk[50], 0, 0, 0, 1, "clrset1 wrap main");
    assert_palette(ctx, colorwk[32], 160, 64, 128, 1, "clrset1 wrap sub");

    reset_beem6_state();
    time_flag = 1;
    clrset1(beam, 1);
    assert_palette(ctx, colorwk[51], 0, 0, 0, 1, "clrset1 time1 main");
    assert_palette(ctx, colorwk[61], 0, 96, 160, 1, "clrset1 time1 sub");

    reset_beem6_state();
    time_flag = 2;
    clrset1(beam, 2);
    assert_palette(ctx, colorwk[52], 0, 0, 0, 1, "clrset1 time2 main");
    assert_palette(ctx, colorwk[61], 64, 64, 32, 1, "clrset1 time2 sub");

    reset_beem6_state();
    beam->actfree[4] = 2;
    maeclrset(beam, 1);
    assert_palette(ctx, colorwk[51], 0, 0, 0, 1, "maeclrset dark half");
}

static void test_beem6_wrapper_dispatches_and_runs_callbacks(test_context *ctx) {
    sprite_status *beam = &actwk[3];

    reset_beem6_state();
    beam->userflag.b.h = 0;

    beem6(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == beam);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == beam);

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beam->r_no0 = 4;
    set_actor_short_alias(beam, 31, 1);

    beem6(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_beem6_state();
    beam->userflag.b.h = 1;
    beam->r_no0 = 2;

    beem6(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_beem6_state();
    beam->r_no0 = 6;

    beem6(beam);

    TEST_ASSERT_EQ_INT(ctx, 6, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_beem6_tables_capture_literal_data(&ctx);
    test_beem6_init_sets_fields_palette_and_schedules_partner(&ctx);
    test_beem6_actsch_existing_and_allocation_failure_paths(&ctx);
    test_beem6_wait_gates_and_countdown_paths(&ctx);
    test_beem6_wait_sets_delay_for_time_modes(&ctx);
    test_beem6_move_warning_active_then_beam_active_paths(&ctx);
    test_palette_helpers_cover_time_modes_and_table_wraps(&ctx);
    test_beem6_wrapper_dispatches_and_runs_callbacks(&ctx);
TEST_MAIN_END

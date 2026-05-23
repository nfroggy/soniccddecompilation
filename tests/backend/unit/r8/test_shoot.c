#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
ushort_union swdata;
ushort_union swdata1;
ushort_union swdata2;
Uint8 time_flag;
char time_item;
Uint8 flagwork[766];
sprite_pattern *bariapat[13];

static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[8];

Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/r8/shoot.c"

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_shoot_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&swdata, 0, sizeof(swdata));
    memset(&swdata1, 0, sizeof(swdata1));
    memset(&swdata2, 0, sizeof(swdata2));
    time_flag = 0;
    time_item = 0;
    memset(flagwork, 0, sizeof(flagwork));
    memset(bariapat, 0, sizeof(bariapat));
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void test_shoot_tables_capture_route_lengths_and_points(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, shooterpositbl[0] == shooterposi_d);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[14] == shooterposi_e);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[60] == shooterposi_3c);
    TEST_ASSERT_EQ_INT(ctx, 20, shooterposi_d[0]);
    TEST_ASSERT_EQ_INT(ctx, 896, shooterposi_d[1]);
    TEST_ASSERT_EQ_INT(ctx, 1744, shooterposi_d[2]);
    TEST_ASSERT_EQ_INT(ctx, 68, shooterposi_2b[0]);
    TEST_ASSERT_EQ_INT(ctx, 4216, shooterposi_2b[33]);
    TEST_ASSERT_EQ_INT(ctx, 384, shooterposi_2b[34]);
    TEST_ASSERT_EQ_INT(ctx, 64, shooterposi_3c[0]);
    TEST_ASSERT_EQ_INT(ctx, 2176, shooterposi_3c[31]);
    TEST_ASSERT_EQ_INT(ctx, 696, shooterposi_3c[32]);
}

static void test_shoot_s_initializes_editor_shooter_and_frames_out(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = -128;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;

    shoot_s(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == shoot);

    reset_shoot_state();
    actwk[0].r_no0 = 6;
    shoot->userflag.b.h = -128;

    shoot_s(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_shoot_s_direction_input_sets_player_motion(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = -128;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 4096;

    shoot_s(shoot);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    reset_shoot_state();

    shoot->userflag.b.h = -128;
    shoot->r_no0 = 4;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    swdata1.b.l = 1;

    shoot_s(shoot);

    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_shoot_s_direction_variants_and_idle(test_context *ctx) {
    static const struct {
        Uint8 input;
        Sint16 expected_xspeed;
        Sint16 expected_yspeed;
    } cases[] = {
        {2, 0, 4096},
        {4, -4096, 0},
        {8, 4096, 0},
    };
    sprite_status *shoot;
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        shoot = &actwk[3];

        reset_shoot_state();
        shoot->userflag.b.h = -128;
        shoot->xposi.w.h = 100;
        shoot->yposi.w.h = 200;
        actwk[0].xposi.w.h = 100;
        actwk[0].yposi.w.h = 200;

        shoot_s(shoot);
        shoot->r_no0 = 4;
        swdata1.b.l = cases[i].input;

        shoot_s(shoot);

        TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
        TEST_ASSERT_EQ_INT(ctx, cases[i].expected_xspeed, actwk[0].xspeed.w);
        TEST_ASSERT_EQ_INT(ctx, cases[i].expected_yspeed, actwk[0].yspeed.w);
        TEST_ASSERT_EQ_INT(ctx, 2, frameout_s_count);
    }

    reset_shoot_state();
    shoot = &actwk[3];
    shoot->userflag.b.h = -128;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    shoot_s(shoot);
    shoot->r_no0 = 4;
    swdata1.b.l = 0;

    shoot_s(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, frameout_s_count);
}

static void test_shooterinit_captures_player_and_starts_route(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    bariapat[0] = (sprite_pattern *)0x1234;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].r_no0 = 4;
    actwk[0].sproffset = 32768 | 7;
    actwk[0].cddat = 32;

    shooterinit(shoot);

    TEST_ASSERT_TRUE(ctx, shoot->patbase == bariapat);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, shoot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, shoot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 7, actwk[0].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->actfree[8]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);
}

static void test_shootermove_rejects_outside_and_busy_player(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 200;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].actfree[2] = 1;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_shootermove_rejects_negative_offsets(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 83;
    actwk[0].yposi.w.h = 200;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 183;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_shootermove_captures_player_without_jump_state(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    shoot->cddat = 32;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].r_no0 = 2;
    actwk[0].sproffset = 32768 | 3;
    actwk[0].cddat = 32;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
}

static void test_shootermove3_advances_fixed_point_and_resets_at_end(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    shooterinit(shoot);

    shoot->actfree[4] = 2;
    actwk[0].xspeed.w = 3;
    actwk[0].yspeed.w = -4;
    actwk[0].xposi.l = 100 << 16;
    actwk[0].yposi.l = 200 << 16;

    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 1, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 768, actwk[0].xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) - 1024, actwk[0].yposi.l);

    shoot->actfree[4] = 0;
    shoot->actfree[16] = 20;
    shoot->actfree[17] = 20;
    actwk[0].actfree[2] = 129;
    actwk[0].yposi.w.h = 4096;

    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 1744, actwk[0].yposi.w.h);
}

static void test_shootermove2_and_route_advance_set_next_segment(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    shooterinit(shoot);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    soundset_count = 0;

    shootermove2(shoot);

    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2111, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);

    shoot->actfree[4] = 0;
    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 896, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1744, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].yspeed.w);
}

static void test_shooter_wrapper_clears_time_flagwork_and_obeys_gates(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    time_flag = 128 | 2;
    time_item = 1;
    shoot->cdsts = 4;
    flagwork[13] = 255;
    actwk[0].mstno.b.h = 43;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[13]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot_state();
    actwk[0].r_no0 = 6;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot_state();
    shoot->userflag.b.h = -128;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 4096;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_shooter_wrapper_time_and_dispatch_edges(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    time_flag = 128 | 1;
    time_item = 3;
    shoot->cdsts = 2;
    flagwork[6] = 255;
    flagwork[7] = 255;
    flagwork[8] = 255;
    actwk[0].mstno.b.h = 43;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[6]);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[7]);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[8]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot_state();
    time_flag = 128 | 9;
    time_item = 1;
    shoot->cdsts = 2;
    flagwork[6] = 255;
    flagwork[7] = 255;
    flagwork[8] = 255;
    actwk[0].r_no0 = 6;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[6]);
    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[7]);
    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[8]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot_state();
    time_flag = 128;
    time_item = 0;
    shoot->cdsts = 0;
    flagwork[0] = 255;
    shoot->r_no0 = 4;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 4096;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);

    reset_shoot_state();
    shoot->r_no0 = 6;
    shoot->actfree[4] = 2;
    actwk[0].xposi.l = 100 << 16;
    actwk[0].yposi.l = 200 << 16;
    actwk[0].xspeed.w = 1;
    actwk[0].yspeed.w = 2;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 1, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 256, actwk[0].xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) + 512, actwk[0].yposi.l);
}

static void test_shooter_wrapper_dispatches_init_and_wait_cases(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    bariapat[0] = (sprite_pattern *)0x1234;
    shoot->r_no0 = 0;
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 4096;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_TRUE(ctx, shoot->patbase == bariapat);
    TEST_ASSERT_EQ_INT(ctx, 129, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);

    reset_shoot_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].mspeed.w = 4096;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
}

static void test_mspd_set_keeps_or_reduces_scripted_speed(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = -128;

    mspd_set(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].mspeed.w);

    reset_shoot_state();
    shoot->userflag.b.h = 3;

    mspd_set(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].mspeed.w);
}

static void test_shooterspdset_x_dominant_and_negative_delta_paths(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 700;
    shoot->yposi.w.h = 1744;
    actwk[0].xposi.w.h = 700;
    actwk[0].yposi.w.h = 1744;

    shooterinit(shoot);
    shootermove2(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 12, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 64, shoot->actfree[5]);

    reset_shoot_state();
    shoot = &actwk[3];
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 1000;
    shoot->yposi.w.h = 1900;
    actwk[0].xposi.w.h = 1000;
    actwk[0].yposi.w.h = 1900;

    shooterinit(shoot);
    shootermove2(shoot);

    TEST_ASSERT_EQ_INT(ctx, -2730, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 9, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 192, shoot->actfree[5]);
}

static void test_shooterspdset_direct_sign_and_division_edges(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = -4096;
    set_actfree_word(shoot, 12, 101);
    set_actfree_word(shoot, 14, 150);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, -81, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 3, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 32, shoot->actfree[5]);

    reset_shoot_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 4096;
    set_actfree_word(shoot, 12, 200);
    set_actfree_word(shoot, 14, 110);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 409, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 64, shoot->actfree[5]);

    reset_shoot_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = -4096;
    set_actfree_word(shoot, 12, 200);
    set_actfree_word(shoot, 14, 100);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 6, shoot->actfree[4]);
    TEST_ASSERT_EQ_INT(ctx, 64, shoot->actfree[5]);
}

TEST_MAIN_BEGIN;
test_shoot_tables_capture_route_lengths_and_points(&ctx);
test_shoot_s_initializes_editor_shooter_and_frames_out(&ctx);
test_shoot_s_direction_input_sets_player_motion(&ctx);
test_shoot_s_direction_variants_and_idle(&ctx);
test_shooterinit_captures_player_and_starts_route(&ctx);
test_shootermove_rejects_outside_and_busy_player(&ctx);
test_shootermove_rejects_negative_offsets(&ctx);
test_shootermove_captures_player_without_jump_state(&ctx);
test_shootermove3_advances_fixed_point_and_resets_at_end(&ctx);
test_shootermove2_and_route_advance_set_next_segment(&ctx);
test_shooter_wrapper_clears_time_flagwork_and_obeys_gates(&ctx);
test_shooter_wrapper_time_and_dispatch_edges(&ctx);
test_shooter_wrapper_dispatches_init_and_wait_cases(&ctx);
test_mspd_set_keeps_or_reduces_scripted_speed(&ctx);
test_shooterspdset_x_dominant_and_negative_delta_paths(&ctx);
test_shooterspdset_direct_sign_and_division_edges(&ctx);
TEST_MAIN_END

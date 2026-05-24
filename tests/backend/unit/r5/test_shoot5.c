#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 time_flag;
char time_item;
Uint8 flagwork[766];
Uint8 colrevflag;

static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[8];

Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);

#include "src/r5/shoot5.c"

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

static void reset_shoot5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    time_flag = 0;
    time_item = 0;
    memset(flagwork, 0, sizeof(flagwork));
    colrevflag = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)(bits & 255);
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 get_actfree_word(sprite_status *actor, int offset) {
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void test_tables_capture_route_lengths_and_points(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, shooterpositbl[0] == shooterposi_0);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[15] == shooterposi_0f);
    TEST_ASSERT_TRUE(ctx, shooterpositbl[21] == shooterposi_15);
    TEST_ASSERT_EQ_INT(ctx, 40, shooterposi_0[0]);
    TEST_ASSERT_EQ_INT(ctx, 9808, shooterposi_0[1]);
    TEST_ASSERT_EQ_INT(ctx, 208, shooterposi_0[2]);
    TEST_ASSERT_EQ_INT(ctx, 84, shooterposi_10[0]);
    TEST_ASSERT_EQ_INT(ctx, 6832, shooterposi_11[1]);
    TEST_ASSERT_EQ_INT(ctx, 856, shooterposi_11[2]);
    TEST_ASSERT_EQ_INT(ctx, 36, shooterposi_15[0]);
    TEST_ASSERT_EQ_INT(ctx, 6576, shooterposi_15[13]);
    TEST_ASSERT_EQ_INT(ctx, 672, shooterposi_15[14]);
}

static void test_wrapper_clears_time_flagwork_and_obeys_player_gates(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    time_flag = 128 | 2;
    time_item = 1;
    shoot->cdsts = 4;
    flagwork[13] = 255;
    actwk[0].mstno.b.h = 43;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[13]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_shoot5_state();
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

    reset_shoot5_state();
    time_flag = 128;
    time_item = 0;
    shoot->cdsts = 0;
    flagwork[0] = 255;
    actwk[0].r_no0 = 6;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 255, flagwork[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);

    reset_shoot5_state();
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
}

static void test_wrapper_collision_reverse_gates_to_frameout(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    shoot->userflag.b.h = -128;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 500;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == shoot);

    reset_shoot5_state();
    shoot->userflag.b.h = -127;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 500;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_shoot5_state();
    colrevflag = 1;
    shoot->userflag.b.h = -128;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_shoot5_state();
    colrevflag = 1;
    shoot->userflag.b.h = -127;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 500;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_shooterinit_captures_first_route_target(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 10;
    shoot->yposi.w.h = 20;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 500;

    shooterinit(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, shoot->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 16, shoot->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 40, get_actfree_word(shoot, 16));
    TEST_ASSERT_EQ_INT(ctx, 9808, get_actfree_word(shoot, 12));
    TEST_ASSERT_EQ_INT(ctx, 208, get_actfree_word(shoot, 14));
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_shootermove_captures_player_and_rejects_misses(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];
    sprite_status *player = &actwk[0];

    reset_shoot5_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 120;
    player->xposi.w.h = 100;
    player->yposi.w.h = 120;
    player->r_no0 = 4;
    player->cddat = 32;
    player->mspeed.w = 3000;
    shoot->cddat = 32;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, player->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 120, get_actfree_word(player, 6));
    TEST_ASSERT_EQ_INT(ctx, 129, player->actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, player->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2560, player->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, player->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->cddat & 32);
    TEST_ASSERT_EQ_INT(ctx, 0, player->cddat & 32);
    TEST_ASSERT_EQ_INT(ctx, 2, player->cddat & 2);
    TEST_ASSERT_EQ_INT(ctx, 100, player->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, player->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, shoot->actfree[8]);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);

    reset_shoot5_state();
    shoot->r_no0 = 2;
    shoot->xposi.w.h = 100;
    shoot->yposi.w.h = 100;
    player->xposi.w.h = 200;
    player->yposi.w.h = 100;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    player->xposi.w.h = 100;
    player->yposi.w.h = 200;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);

    player->xposi.w.h = 100;
    player->yposi.w.h = 100;
    player->actfree[2] = 1;

    shootermove(shoot);

    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no0);
}

static void test_shootermove_rejects_negative_offsets(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
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
}

static void test_shootermove2_and_route_advance(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 9808;
    shoot->yposi.w.h = 208;
    actwk[0].xposi.w.h = 9808;
    actwk[0].yposi.w.h = 208;
    actwk[0].mspeed.w = 4096;

    shooterinit(shoot);
    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    soundset_count = 0;

    shootermove2(shoot);

    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2560, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 145, soundset_requests[0]);

    shoot->actfree[5] = 0;
    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->actfree[17]);
    TEST_ASSERT_EQ_INT(ctx, 9808, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 208, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2560, actwk[0].yspeed.w);
}

static void test_shootermove3_advances_and_resets_at_route_end(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    actwk[0].xposi.l = 100 << 16;
    actwk[0].yposi.l = 200 << 16;
    actwk[0].xspeed.w = 2;
    actwk[0].yspeed.w = -3;
    shoot->actfree[5] = 2;

    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 1, shoot->actfree[5]);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 512, actwk[0].xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) - 768, actwk[0].yposi.l);

    reset_shoot5_state();
    shoot->r_no0 = 6;
    shoot->actfree[4] = 0;
    shoot->actfree[16] = 8;
    shoot->actfree[17] = 8;
    set_actfree_word(shoot, 12, 12);
    set_actfree_word(shoot, 14, 4097);
    actwk[0].actfree[2] = 129;

    shootermove3(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, shoot->r_no1);
}

static void test_shooter_wrapper_dispatches_active_states(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    shoot->r_no0 = 0;
    shoot->userflag.b.h = 0;
    shoot->xposi.w.h = 9808;
    shoot->yposi.w.h = 208;
    actwk[0].xposi.w.h = 9808;
    actwk[0].yposi.w.h = 208;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 129, actwk[0].actfree[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_shoot5_state();
    shoot->r_no0 = 4;
    actwk[0].mspeed.w = 1024;
    set_actfree_word(shoot, 12, 0);
    set_actfree_word(shoot, 14, 1024);

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 6, shoot->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].yspeed.w);

    reset_shoot5_state();
    shoot->r_no0 = 6;
    shoot->actfree[5] = 2;
    actwk[0].xposi.l = 100 << 16;
    actwk[0].yposi.l = 200 << 16;
    actwk[0].xspeed.w = 1;
    actwk[0].yspeed.w = 1;

    shooter(shoot);

    TEST_ASSERT_EQ_INT(ctx, 1, shoot->actfree[5]);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 256, actwk[0].xposi.l);
    TEST_ASSERT_EQ_INT(ctx, (200 << 16) + 256, actwk[0].yposi.l);
}

static void test_shooterspdset_axis_dominance_and_direction(
    test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 0;
    set_actfree_word(shoot, 12, 100);
    set_actfree_word(shoot, 14, 4196);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2560, actwk[0].yspeed.w);

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 1024;
    set_actfree_word(shoot, 12, -400);
    set_actfree_word(shoot, 14, 200);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 204, actwk[0].yspeed.w);

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = -1024;
    set_actfree_word(shoot, 12, 100);
    set_actfree_word(shoot, 14, -1948);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].yspeed.w);

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = -1024;
    set_actfree_word(shoot, 12, -400);
    set_actfree_word(shoot, 14, 200);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -204, actwk[0].yspeed.w);
}

static void test_shooterspdset_division_edges(test_context *ctx) {
    sprite_status *shoot = &actwk[3];

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 4096;
    set_actfree_word(shoot, 12, 101);
    set_actfree_word(shoot, 14, 150);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 81, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].yspeed.w);

    reset_shoot5_state();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 100;
    actwk[0].mspeed.w = 4096;
    set_actfree_word(shoot, 12, 200);
    set_actfree_word(shoot, 14, 100);

    shooterspdset(shoot);

    TEST_ASSERT_EQ_INT(ctx, 4096, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
}

TEST_MAIN_BEGIN;
    test_tables_capture_route_lengths_and_points(&ctx);
    test_wrapper_clears_time_flagwork_and_obeys_player_gates(&ctx);
    test_wrapper_collision_reverse_gates_to_frameout(&ctx);
    test_shooterinit_captures_first_route_target(&ctx);
    test_shootermove_captures_player_and_rejects_misses(&ctx);
    test_shootermove_rejects_negative_offsets(&ctx);
    test_shootermove2_and_route_advance(&ctx);
    test_shootermove3_advances_and_resets_at_route_end(&ctx);
    test_shooter_wrapper_dispatches_active_states(&ctx);
    test_shooterspdset_axis_dominance_and_direction(&ctx);
    test_shooterspdset_division_edges(&ctx);
TEST_MAIN_END

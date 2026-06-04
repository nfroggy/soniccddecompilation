#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int sinset_count;
static Uint8 sinset_requests[16];
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;
static Sint16 ride_on_chk_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r8/piston.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < 16) {
        sinset_requests[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = (Sint16)kakudo;
    *cos = (Sint16)(1000 + kakudo);
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return ride_on_chk_result;
}

static void reset_piston_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    sinset_count = 0;
    memset(sinset_requests, 0, sizeof(sinset_requests));
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
    ride_on_chk_result = 1;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    sinset_count = 0;
    memset(sinset_requests, 0, sizeof(sinset_requests));
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
}

static void assert_outer_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void initialize_piston(sprite_status *actor, Sint8 userflag,
                              Sint16 ypos) {
    actor->userflag.b.h = userflag;
    actor->yposi.w.h = ypos;
    piston(actor);
    reset_logs();
}

static void test_piston_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pistonpat[0] == &pis00);
    TEST_ASSERT_TRUE(ctx, pistonpat[1] == &pis01);
    TEST_ASSERT_TRUE(ctx, pistonpat[2] == &pis02);
    TEST_ASSERT_TRUE(ctx, pistonpat[3] == &pis03);
    TEST_ASSERT_EQ_INT(ctx, 1, pis00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -32, pis00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -31, pis00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_PISTON_BASE, pis00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3, pis01.cnt);
    TEST_ASSERT_EQ_INT(ctx, -95, pis01.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 33, pis01.spra[2].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_PISTON_BASE + 1, pis02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, pis03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 4, spd_tbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, spd_tbl[3]);
}

static void test_piston_initializes_type0_fast_radius32(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    actor->userflag.b.h = 12;
    actor->actflg = 128;
    actor->xspeed.w = 123;
    actor->yspeed.w = -456;
    actor->yposi.w.h = 200;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_requests[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_initializes_type2_radius48(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    actor->userflag.b.h = 66;
    actor->yposi.w.h = 300;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_requests[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_initializes_type1_short_delay(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    actor->userflag.b.h = 1;
    actor->yposi.w.h = 200;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_requests[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_initializes_type1_long_delay_and_offset_phase(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    actor->userflag.b.h = 25;
    actor->yposi.w.h = 200;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_requests[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type0_moves_and_checks_ride_downward(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 12, 200);

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 8, sinset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type0_checks_ride_before_moving_when_speed_is_negative(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 12, 200);
    actor->yspeed.w = -1;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_enters_pause_at_quarter_phase(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 17, 200);
    actor->yposi.w.h = 210;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_holds_pause_while_delay_positive(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 17, 200);
    actor->yposi.w.h = 210;
    piston(actor);
    reset_logs();
    piston_get_work(actor)->pause_timer = 1;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_exits_pause_and_advances(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 17, 200);
    actor->yposi.w.h = 210;
    piston(actor);
    reset_logs();
    piston_get_work(actor)->pause_timer = 0;

    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 65, sinset_requests[0]);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_jump_return_when_ride_check_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 37, 200);
    ride_on_chk_result = 0;

    piston(actor);
    reset_logs();
    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].mstno.b.h);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_launches_player_with_high_jump(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, 37, 200);
    actwk[0].cddat = 8;
    actor->cddat = 8;

    piston(actor);
    reset_logs();
    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, -4096, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[0].mstno.b.h);
    assert_outer_callbacks(ctx, actor);
}

static void test_piston_type1_launches_player_with_low_jump(test_context *ctx) {
    sprite_status *actor = &actwk[3];

    reset_piston_state();
    initialize_piston(actor, -91, 200);
    actwk[0].cddat = 8;
    actor->cddat = 8;

    piston(actor);
    reset_logs();
    piston(actor);

    TEST_ASSERT_EQ_INT(ctx, -2560, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[0].mstno.b.h);
    assert_outer_callbacks(ctx, actor);
}

TEST_MAIN_BEGIN;
    test_piston_patterns_capture_literal_data(&ctx);
    test_piston_initializes_type0_fast_radius32(&ctx);
    test_piston_initializes_type2_radius48(&ctx);
    test_piston_initializes_type1_short_delay(&ctx);
    test_piston_initializes_type1_long_delay_and_offset_phase(&ctx);
    test_piston_type0_moves_and_checks_ride_downward(&ctx);
    test_piston_type0_checks_ride_before_moving_when_speed_is_negative(&ctx);
    test_piston_type1_enters_pause_at_quarter_phase(&ctx);
    test_piston_type1_holds_pause_while_delay_positive(&ctx);
    test_piston_type1_exits_pause_and_advances(&ctx);
    test_piston_type1_jump_return_when_ride_check_fails(&ctx);
    test_piston_type1_launches_player_with_high_jump(&ctx);
    test_piston_type1_launches_player_with_low_jump(&ctx);
TEST_MAIN_END

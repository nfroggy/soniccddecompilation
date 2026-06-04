#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
int_union scra_h_posit;
Sint16 time_stop;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static sprite_status *ride_on_chk_player;
static Sint16 ride_on_chk_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r8/okusieso.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    ++actwkchk2_count;
    actwkchk2_actor = pActwk;
    if (actwkchk2_queue_index >= actwkchk2_queue_count) {
        *ppNewActwk = 0;
        return -1;
    }
    *ppNewActwk = actwkchk2_queue[actwkchk2_queue_index++];
    return 0;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    ride_on_chk_player = pPlayerwk;
    return ride_on_chk_result;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    ride_on_chk_player = 0;
    ride_on_chk_result = 0;
}

static void reset_okusieso_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    time_stop = 0;
    reset_logs();
}

static void spawn_okusieso(sprite_status *parent,
                           sprite_status *ball,
                           sprite_status *platform) {
    queue_actwkchk2(ball);
    queue_actwkchk2(platform);
    parent->actno = 58;
    parent->xposi.w.h = 512;
    parent->yposi.w.h = 384;
    scra_h_posit.w.h = 512;

    okusieso(parent);
    reset_logs();
}

static void assert_action_only(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_okusieso_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, okusiesopat[0] == &spr_sieso_00);
    TEST_ASSERT_TRUE(ctx, okusiesopat[4] == &spr_sieso_04);
    TEST_ASSERT_TRUE(ctx, okusiesopat[5] == &spr_ball_00);
    TEST_ASSERT_TRUE(ctx, okusiesopat[8] == &spr_dai_00);
    TEST_ASSERT_EQ_INT(ctx, 2, spr_sieso_00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -40, spr_sieso_00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -56, spr_sieso_00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE, spr_sieso_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE + 10, spr_sieso_01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE + 4, spr_sieso_04.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE + 5, spr_ball_00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE + 7, spr_ball_02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_OKUSIESO_BASE + 8, spr_dai_00.spra[0].index);
}

static void test_okusieso_initializes_parent_and_children(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    queue_actwkchk2(ball);
    queue_actwkchk2(platform);
    parent->actno = 58;
    parent->xposi.w.h = 512;
    parent->yposi.w.h = 384;
    scra_h_posit.w.h = 512;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, parent->actflg);
    TEST_ASSERT_TRUE(ctx, parent->patbase == okusiesopat);
    TEST_ASSERT_EQ_INT(ctx, 928, parent->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 5, parent->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 40, parent->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 56, parent->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(parent)->anim_phase);

    TEST_ASSERT_EQ_INT(ctx, 2, ball->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, ball->actflg);
    TEST_ASSERT_TRUE(ctx, ball->patbase == okusiesopat);
    TEST_ASSERT_EQ_INT(ctx, 17312, ball->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 6, ball->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 5, ball->patno);
    TEST_ASSERT_EQ_INT(ctx, 16, ball->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, ball->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(ball)->flags);
    TEST_ASSERT_EQ_INT(ctx, parent->actno, ball->actno);
    TEST_ASSERT_EQ_INT(ctx, parent->xposi.w.h, ball->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h, ball->yposi.w.h);

    TEST_ASSERT_EQ_INT(ctx, 2, platform->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, platform->actflg);
    TEST_ASSERT_TRUE(ctx, platform->patbase == okusiesopat);
    TEST_ASSERT_EQ_INT(ctx, 928, platform->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 3, platform->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, platform->patno);
    TEST_ASSERT_EQ_INT(ctx, 40, platform->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, platform->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 2, okusieso_get_work(platform)->flags);
    TEST_ASSERT_EQ_INT(ctx, parent->actno, platform->actno);
    TEST_ASSERT_EQ_INT(ctx, parent->xposi.w.h, platform->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 44, platform->yposi.w.h);
}

static void test_okusieso_stops_spawning_after_first_allocation_failure(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];

    reset_okusieso_state();
    parent->actno = 58;
    parent->xposi.w.h = 512;
    parent->yposi.w.h = 384;
    scra_h_posit.w.h = 512;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    assert_action_only(ctx, parent);
}

static void test_okusieso_keeps_ball_when_platform_allocation_fails(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];

    reset_okusieso_state();
    queue_actwkchk2(ball);
    parent->actno = 58;
    parent->xposi.w.h = 512;
    parent->yposi.w.h = 384;
    scra_h_posit.w.h = 512;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 2, parent->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, ball->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(ball)->flags);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[21].r_no0);
    assert_action_only(ctx, parent);
}

static void test_okusieso_frames_out_when_offscreen(test_context *ctx) {
    sprite_status *parent = &actwk[3];

    reset_okusieso_state();
    parent->actno = 58;
    parent->xposi.w.h = 1024;
    parent->yposi.w.h = 384;
    scra_h_posit.w.h = 0;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == parent);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == parent);
}

static void test_okusieso_ball_waits_until_parent_triggered(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, 0, ball->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h, ball->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(ball)->flags);
    assert_action_only(ctx, ball);
}

static void test_okusieso_ball_pause_and_zero_speed_animation_paths(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags |= 16;
    time_stop = 1;

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, 0, ball->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h, ball->yposi.w.h);
    assert_action_only(ctx, ball);

    reset_logs();
    time_stop = 0;
    okusieso_get_work(ball)->flags |= 64;
    ball->yposi.w.h = parent->yposi.w.h - 40;
    ball->yspeed.w = -256;
    ball->patno = 5;

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, 6, ball->patno);
    TEST_ASSERT_EQ_INT(ctx, 164, ball->colino);
    TEST_ASSERT_EQ_INT(ctx, 4, ball->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 0, ball->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 40, ball->yposi.w.h);
    assert_action_only(ctx, ball);
}

static void test_okusieso_ball_launches_then_lands(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags |= 16;

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, -12288, ball->yspeed.w);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(ball)->flags & 64) != 0);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 12, ball->yposi.w.h);
    assert_action_only(ctx, ball);

    reset_logs();
    ball->yposi.w.h = parent->yposi.w.h;
    ball->yspeed.w = -256;

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h + 4, ball->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, ball->yspeed.w);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(ball)->flags & 8) != 0);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 8) != 0);
    TEST_ASSERT_EQ_INT(ctx, 4, parent->patno);
    assert_action_only(ctx, ball);
}

static void test_okusieso_platform_ride_triggers_parent_and_tracks_patno(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    parent->patno = 2;
    ride_on_chk_result = 1;

    okusieso(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == platform);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_player == &actwk[0]);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 16) != 0);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 4) != 0);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 12, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, platform->yspeed.w);
    assert_action_only(ctx, platform);
}

static void test_okusieso_platform_no_ride_and_parent_already_triggered_paths(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    parent->patno = 1;
    ride_on_chk_result = 0;

    okusieso(platform);

    TEST_ASSERT_EQ_INT(ctx, 2, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 16) == 0);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 4) == 0);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 28, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2048, platform->yspeed.w);
    assert_action_only(ctx, platform);

    reset_logs();
    okusieso_get_work(parent)->flags |= 16 | 4;
    parent->patno = 3;

    okusieso(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 16) != 0);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 4) == 0);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h + 12, platform->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2048, platform->yspeed.w);
    assert_action_only(ctx, platform);
}

static void test_okusieso_platform_pause_returns_without_ride_check(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    time_stop = 1;

    okusieso(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, parent->yposi.w.h - 44, platform->yposi.w.h);
    assert_action_only(ctx, platform);
}

static void test_okusieso_parent_advances_when_ride_hold_bit_is_set(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags |= 16 | 32 | 64 | 4;
    parent->patno = 2;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 3, parent->patno);
    assert_action_only(ctx, parent);
}

static void test_okusieso_parent_remaining_early_return_paths(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags = 8;
    parent->patno = 2;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->patno);
    assert_action_only(ctx, parent);

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags = 0;
    parent->patno = 1;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, parent->patno);
    assert_action_only(ctx, parent);

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags = 16 | 64;
    okusieso_get_work(parent)->anim_timer = 2;
    parent->patno = 1;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(parent)->anim_timer);
    assert_action_only(ctx, parent);
}

static void test_okusieso_ball_terminal_negative_speed_literal_path(
    test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags |= 16;
    okusieso_get_work(ball)->flags |= 64;
    ball->yposi.w.h = parent->yposi.w.h + 100;
    ball->yspeed.w = (Sint16)57088;
    ball->patno = 5;

    okusieso(ball);

    TEST_ASSERT_EQ_INT(ctx, (Sint16)57344, ball->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, ball->patno);
    TEST_ASSERT_TRUE(ctx, ball->yposi.w.h < parent->yposi.w.h + 100);
    assert_action_only(ctx, ball);
}

static void test_okusieso_parent_animation_timer_phases(test_context *ctx) {
    sprite_status *parent = &actwk[3];
    sprite_status *ball = &actwk[20];
    sprite_status *platform = &actwk[21];

    reset_okusieso_state();
    spawn_okusieso(parent, ball, platform);
    okusieso_get_work(parent)->flags |= 16;
    time_stop = 1;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 0, parent->patno);
    assert_action_only(ctx, parent);

    reset_logs();
    time_stop = 0;
    okusieso_get_work(parent)->anim_phase = 0;

    okusieso(parent);

    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 64) != 0);
    TEST_ASSERT_EQ_INT(ctx, 1, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 5, okusieso_get_work(parent)->anim_timer);
    assert_action_only(ctx, parent);

    reset_logs();
    okusieso_get_work(parent)->anim_timer = 0;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 5, okusieso_get_work(parent)->anim_timer);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 32) == 0);
    assert_action_only(ctx, parent);

    reset_logs();
    parent->patno = 3;
    okusieso_get_work(parent)->anim_timer = 0;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 4, parent->patno);
    TEST_ASSERT_TRUE(ctx, (okusieso_get_work(parent)->flags & 32) != 0);
    TEST_ASSERT_EQ_INT(ctx, 0, okusieso_get_work(parent)->anim_phase);
    TEST_ASSERT_EQ_INT(ctx, 10, okusieso_get_work(parent)->anim_timer);
    assert_action_only(ctx, parent);

    reset_logs();
    okusieso_get_work(parent)->flags &= (Uint8)~4;
    okusieso_get_work(parent)->anim_timer = 0;
    okusieso_get_work(parent)->anim_phase = 0;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 2, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, okusieso_get_work(parent)->anim_phase);
    TEST_ASSERT_EQ_INT(ctx, 10, okusieso_get_work(parent)->anim_timer);
    assert_action_only(ctx, parent);

    reset_logs();
    okusieso_get_work(parent)->anim_timer = 0;
    okusieso_get_work(parent)->anim_phase = 3;

    okusieso(parent);

    TEST_ASSERT_EQ_INT(ctx, 3, parent->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, okusieso_get_work(parent)->anim_phase);
    TEST_ASSERT_EQ_INT(ctx, 10, okusieso_get_work(parent)->anim_timer);
    assert_action_only(ctx, parent);
}

TEST_MAIN_BEGIN;
    test_okusieso_patterns_capture_literal_data(&ctx);
    test_okusieso_initializes_parent_and_children(&ctx);
    test_okusieso_stops_spawning_after_first_allocation_failure(&ctx);
    test_okusieso_keeps_ball_when_platform_allocation_fails(&ctx);
    test_okusieso_frames_out_when_offscreen(&ctx);
    test_okusieso_ball_waits_until_parent_triggered(&ctx);
    test_okusieso_ball_pause_and_zero_speed_animation_paths(&ctx);
    test_okusieso_ball_launches_then_lands(&ctx);
    test_okusieso_platform_ride_triggers_parent_and_tracks_patno(&ctx);
    test_okusieso_platform_no_ride_and_parent_already_triggered_paths(&ctx);
    test_okusieso_platform_pause_returns_without_ride_check(&ctx);
    test_okusieso_parent_advances_when_ride_hold_bit_is_set(&ctx);
    test_okusieso_parent_remaining_early_return_paths(&ctx);
    test_okusieso_ball_terminal_negative_speed_literal_path(&ctx);
    test_okusieso_parent_animation_timer_phases(&ctx);
TEST_MAIN_END

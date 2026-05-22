#include <string.h>

#include "support/callback_log.h"
#include "support/test_runner.h"
#include "types.h"

bmp_info SprBmp[700];
void (*EAsprset)(Sint16, Sint16, Uint16, Uint16, Uint16) =
    callback_log_easprset;
static int action_callback_count;
static sprite_status *action_callback_actor;
static int memset_callback_count;
static void *memset_callback_ptr;
static Uint8 memset_callback_value;
static Sint32 memset_callback_size;
static void action_callback(sprite_status *actor);
static void memset_callback(void *ptr, Uint8 value, Sint32 size);
void (*sMemSet)(void *, Uint8, Sint32) = memset_callback;
void (*act_tbl[1])(sprite_status *) = {action_callback};
sprite_status actwk[128];
sprite_statuses pbuffer[8];
game_info *lpKeepWork;
int_union scra_h_posit;
int_union scra_v_posit;
int_union scrb_h_posit;
int_union scrb_v_posit;
int_union scrc_h_posit;
int_union scrc_v_posit;
Uint8 linkdata;
static game_info keep_work;

#include "src/action.c"

static void action_callback(sprite_status *actor) {
    ++action_callback_count;
    action_callback_actor = actor;
}

static void memset_callback(void *ptr, Uint8 value, Sint32 size) {
    ++memset_callback_count;
    memset_callback_ptr = ptr;
    memset_callback_value = value;
    memset_callback_size = size;
    memset(ptr, value, (size_t)size);
}

static void reset_action_state(void) {
    callback_log_reset();
    action_callback_count = 0;
    action_callback_actor = 0;
    memset_callback_count = 0;
    memset_callback_ptr = 0;
    memset_callback_value = 0;
    memset_callback_size = 0;
    memset(SprBmp, 0, sizeof(SprBmp));
    memset(actwk, 0, sizeof(actwk));
    memset(pbuffer, 0, sizeof(pbuffer));
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;
    scra_h_posit.l = 0;
    scra_v_posit.l = 0;
    scrb_h_posit.l = 0;
    scrb_v_posit.l = 0;
    scrc_h_posit.l = 0;
    scrc_v_posit.l = 0;
    linkdata = 0;
}

static void set_actor_position(sprite_status *actor, Sint16 x, Sint16 y) {
    actor->xposi.l = 0;
    actor->yposi.l = 0;
    actor->xposi.w.h = x;
    actor->yposi.w.h = y;
}

static void assert_easprset(test_context *ctx, int call_index, Sint16 x,
                            Sint16 y, Uint16 index, Uint16 link,
                            Uint16 reverse) {
    const easprset_call *call = callback_log_easprset_at(call_index);
    TEST_ASSERT_NOT_NULL(ctx, call);
    if (!call) {
        return;
    }

    TEST_ASSERT_EQ_INT(ctx, x, call->x);
    TEST_ASSERT_EQ_INT(ctx, y, call->y);
    TEST_ASSERT_EQ_INT(ctx, index, call->index);
    TEST_ASSERT_EQ_INT(ctx, link, call->link);
    TEST_ASSERT_EQ_INT(ctx, reverse, call->reverse);
}

static void test_action_dispatches_active_actor(test_context *ctx) {
    reset_action_state();

    actwk[0].actno = 0;
    actwk[1].actno = 1;

    action();

    TEST_ASSERT_EQ_INT(ctx, 1, action_callback_count);
    TEST_ASSERT_TRUE(ctx, action_callback_actor == &actwk[1]);
}

static void test_speedset_adds_gravity_and_moves_by_original_speed(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xposi.l = 0x00010000;
    actor.yposi.l = 0x00020000;
    actor.xspeed.w = 3;
    actor.yspeed.w = 100;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0x00010300, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 0x00026400, actor.yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 156, actor.yspeed.w);
}

static void test_speedset_skips_gravity_for_actfree_8(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xposi.l = 0x00010000;
    actor.yposi.l = 0x00020000;
    actor.xspeed.w = -2;
    actor.yspeed.w = 100;
    actor.actfree[2] = 8;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0x0000FE00, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 0x00026400, actor.yposi.l);
    TEST_ASSERT_EQ_INT(ctx, 100, actor.yspeed.w);
}

static void test_speedset_skips_gravity_for_actfree_4(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.yspeed.w = -100;
    actor.actfree[2] = 4;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, -100, actor.yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -25600, actor.yposi.l);
}

static void test_speedset_keeps_fast_upward_speed_with_actfree_2(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.yspeed.w = -2049;
    actor.actfree[2] = 2;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, -2049, actor.yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -524544, actor.yposi.l);
}

static void test_speedset_adds_gravity_at_upward_threshold(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.yspeed.w = -2048;
    actor.actfree[2] = 2;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, -1992, actor.yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -524288, actor.yposi.l);
}

static void test_speedset_clamps_downward_speed_after_movement(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.yspeed.w = 4090;

    speedset(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4096, actor.yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1047040, actor.yposi.l);
}

static void test_speedset2_moves_without_attached_platform(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xposi.l = 0x00010000;
    actor.yposi.l = 0x00020000;
    actor.xspeed.w = 4;
    actor.yspeed.w = -3;

    speedset2(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0x00010400, actor.xposi.l);
    TEST_ASSERT_EQ_INT(ctx, 0x0001FD00, actor.yposi.l);
}

static void test_speedset2_subtracts_attached_platform_speed(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xspeed.w = 4;
    actor.cddat = 8;
    actor.actfree[19] = 3;
    actwk[3].actno = 30;

    speedset2(&actor);

    TEST_ASSERT_EQ_INT(ctx, -252 << 8, actor.xposi.l);
}

static void test_speedset2_adds_attached_platform_speed(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xspeed.w = 4;
    actor.cddat = 8;
    actor.actfree[19] = 3;
    actwk[3].actno = 30;
    actwk[3].cddat = 1;

    speedset2(&actor);

    TEST_ASSERT_EQ_INT(ctx, 260 << 8, actor.xposi.l);
}

static void test_speedset2_ignores_non_platform_attachment(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.xspeed.w = 4;
    actor.cddat = 8;
    actor.actfree[19] = 3;
    actwk[3].actno = 29;

    speedset2(&actor);

    TEST_ASSERT_EQ_INT(ctx, 4 << 8, actor.xposi.l);
}

static void test_frameout_uses_memset_callback(test_context *ctx) {
    reset_action_state();

    sprite_status actor;
    memset(&actor, 0x55, sizeof(actor));

    frameout(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, memset_callback_count);
    TEST_ASSERT_TRUE(ctx, memset_callback_ptr == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, memset_callback_value);
    TEST_ASSERT_TRUE(ctx, memset_callback_size > 0);
}

static void test_spatset_emits_unflipped_sprite(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprite = {0};
    sprite.xoff = 5;
    sprite.yoff = -3;
    sprite.index = 7;

    spatset(100, 50, &actor, &sprite, 1);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 105, 47, 7, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_spatset_emits_horizontal_flip(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprite = {0};
    actor.actflg = 1;
    sprite.xoff = 5;
    sprite.yoff = -3;
    sprite.index = 7;
    SprBmp[7].xs = 12;

    spatset(100, 50, &actor, &sprite, 1);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 83, 47, 7, 0, 1);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_spatset_emits_vertical_flip(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprite = {0};
    actor.actflg = 2;
    sprite.xoff = 5;
    sprite.yoff = -3;
    sprite.index = 7;
    SprBmp[7].ys = 9;

    spatset(100, 50, &actor, &sprite, 1);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 105, 44, 7, 0, 2);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_spatset_combines_pattern_and_actor_flip_flags(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprite = {0};
    actor.actflg = 3;
    actor.sproffset = 32768;
    sprite.xoff = 5;
    sprite.yoff = -3;
    sprite.etc = 128 | 8;
    sprite.index = 7;
    SprBmp[7].xs = 12;
    SprBmp[7].ys = 9;

    spatset(100, 50, &actor, &sprite, 1);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 83, 44, 7, 0, 32768 | 2);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_spatset_skips_zero_index_entries(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprites[2] = {0};
    sprites[0].xoff = 1;
    sprites[0].yoff = 2;
    sprites[0].index = 0;
    sprites[1].xoff = 3;
    sprites[1].yoff = 4;
    sprites[1].index = 5;

    spatset(10, 20, &actor, sprites, 2);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 13, 24, 5, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_spatset_stops_at_link_capacity(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    sprite_data sprites[2] = {0};
    sprites[0].index = 5;
    sprites[1].index = 6;
    linkdata = 79;

    spatset(10, 20, &actor, sprites, 2);

    TEST_ASSERT_EQ_INT(ctx, 1, callback_log_easprset_count());
    assert_easprset(ctx, 0, 10, 20, 5, 79, 0);
    TEST_ASSERT_EQ_INT(ctx, 80, linkdata);
}

static void test_scronchk_screen_bounds(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 50;

    set_actor_position(&actor, 100, 50);
    TEST_ASSERT_EQ_INT(ctx, 0, scronchk(&actor));

    set_actor_position(&actor, 419, 273);
    TEST_ASSERT_EQ_INT(ctx, 0, scronchk(&actor));

    set_actor_position(&actor, 99, 50);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk(&actor));

    set_actor_position(&actor, 420, 50);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk(&actor));

    set_actor_position(&actor, 100, 49);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk(&actor));

    set_actor_position(&actor, 100, 274);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk(&actor));
}

static void test_scronchk2_uses_horizontal_size(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.sprhsize = 5;
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 50;

    set_actor_position(&actor, 95, 50);
    TEST_ASSERT_EQ_INT(ctx, 0, scronchk2(&actor));

    set_actor_position(&actor, 94, 50);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk2(&actor));

    set_actor_position(&actor, 424, 273);
    TEST_ASSERT_EQ_INT(ctx, 0, scronchk2(&actor));

    set_actor_position(&actor, 425, 273);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk2(&actor));

    set_actor_position(&actor, 100, 49);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk2(&actor));

    set_actor_position(&actor, 100, 274);
    TEST_ASSERT_EQ_INT(ctx, -1, scronchk2(&actor));
}

static void test_actionsub_returns_when_gamepass_is_set(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    keep_work.GamePass = 1;

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, pbuffer[0].cnt);
}

static void test_actionsub_queues_actor_without_screen_check(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 128;
    actor.sprpri = 3;

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, pbuffer[3].cnt);
    TEST_ASSERT_TRUE(ctx, pbuffer[3].pActwk[0] == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.actflg);
}

static void test_actionsub_culls_left_and_right(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 4;
    actor.sprhsize = 5;
    scra_h_posit.w.h = 100;

    set_actor_position(&actor, 94, 50);
    actionsub(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, pbuffer[0].cnt);

    set_actor_position(&actor, 425, 50);
    actionsub(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, pbuffer[0].cnt);
}

static void test_actionsub_culls_top_and_bottom(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 4;
    actor.sprvsize = 5;
    scra_v_posit.w.h = 50;

    set_actor_position(&actor, 100, 44);
    actionsub(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, pbuffer[0].cnt);

    set_actor_position(&actor, 100, 279);
    actionsub(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, pbuffer[0].cnt);
}

static void test_actionsub_queues_visible_screen_relative_actor(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 4;
    actor.sprhsize = 5;
    actor.sprvsize = 25;
    actor.sprpri = 2;
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 50;
    set_actor_position(&actor, 100, 50);

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, pbuffer[2].cnt);
    TEST_ASSERT_TRUE(ctx, pbuffer[2].pActwk[0] == &actor);
}

static void test_actionsub_wraps_vertical_position_near_top(test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 4;
    actor.sprvsize = 5;
    scra_v_posit.w.h = 10;
    set_actor_position(&actor, 100, 2058);

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, pbuffer[0].cnt);
}

static void test_actionsub_wraps_vertical_position_near_bottom(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.actflg = 4;
    actor.sprvsize = 25;
    scra_v_posit.w.h = 1800;
    set_actor_position(&actor, 100, 0);

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, pbuffer[0].cnt);
}

static void test_actionsub_does_not_queue_when_priority_full(
    test_context *ctx) {
    reset_action_state();

    sprite_status actor = {0};
    actor.sprpri = 1;
    pbuffer[1].cnt = 63;

    actionsub(&actor);

    TEST_ASSERT_EQ_INT(ctx, 63, pbuffer[1].cnt);
}

static void test_patset_clears_all_sprite_slots_when_empty(test_context *ctx) {
    reset_action_state();

    patset();

    TEST_ASSERT_EQ_INT(ctx, 80, callback_log_easprset_count());
    assert_easprset(ctx, 0, 0, 0, 0, 0, 0);
    assert_easprset(ctx, 79, 0, 0, 0, 79, 0);
    TEST_ASSERT_EQ_INT(ctx, 0, linkdata);
}

static void test_patset_returns_without_output_on_gamepass(test_context *ctx) {
    reset_action_state();
    keep_work.GamePass = 1;

    patset();

    TEST_ASSERT_EQ_INT(ctx, 0, callback_log_easprset_count());
}

static void test_patset_emits_actor_pattern_then_clears_rest(test_context *ctx) {
    reset_action_state();

    sprite_pattern pattern = {0};
    sprite_pattern *patterns[1] = {&pattern};
    sprite_status actor = {0};
    actor.actno = 1;
    actor.patbase = patterns;
    set_actor_position(&actor, 20, 30);
    pattern.cnt = 2;
    pattern.spra[0].xoff = 3;
    pattern.spra[0].yoff = 4;
    pattern.spra[0].index = 5;
    pattern.spra[1].xoff = -2;
    pattern.spra[1].yoff = -6;
    pattern.spra[1].index = 6;
    pbuffer[2].cnt = 1;
    pbuffer[2].pActwk[0] = &actor;

    patset();

    TEST_ASSERT_EQ_INT(ctx, 80, callback_log_easprset_count());
    assert_easprset(ctx, 0, 23, 34, 5, 0, 0);
    assert_easprset(ctx, 1, 18, 24, 6, 1, 0);
    assert_easprset(ctx, 2, 0, 0, 0, 2, 0);
    assert_easprset(ctx, 79, 0, 0, 0, 79, 0);
    TEST_ASSERT_EQ_INT(ctx, 2, linkdata);
}

static void test_patset_limits_pattern_when_single_sprite_flag_is_set(
    test_context *ctx) {
    reset_action_state();

    sprite_pattern pattern = {0};
    sprite_pattern *patterns[1] = {&pattern};
    sprite_status actor = {0};
    actor.actno = 1;
    actor.actflg = 32;
    actor.patbase = patterns;
    set_actor_position(&actor, 20, 30);
    pattern.cnt = 2;
    pattern.spra[0].xoff = 3;
    pattern.spra[0].yoff = 4;
    pattern.spra[0].index = 5;
    pattern.spra[1].xoff = -2;
    pattern.spra[1].yoff = -6;
    pattern.spra[1].index = 6;
    pbuffer[0].cnt = 1;
    pbuffer[0].pActwk[0] = &actor;

    patset();

    TEST_ASSERT_EQ_INT(ctx, 80, callback_log_easprset_count());
    assert_easprset(ctx, 0, 23, 34, 5, 0, 0);
    assert_easprset(ctx, 1, 0, 0, 0, 1, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_patset_applies_screen_relative_offsets(test_context *ctx) {
    reset_action_state();

    sprite_pattern pattern = {0};
    sprite_pattern *patterns[1] = {&pattern};
    sprite_status actor = {0};
    actor.actno = 1;
    actor.actflg = 4;
    actor.patbase = patterns;
    set_actor_position(&actor, 120, 60);
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 50;
    pattern.cnt = 1;
    pattern.spra[0].xoff = 3;
    pattern.spra[0].yoff = 4;
    pattern.spra[0].index = 5;
    pbuffer[0].cnt = 1;
    pbuffer[0].pActwk[0] = &actor;

    patset();

    TEST_ASSERT_EQ_INT(ctx, 80, callback_log_easprset_count());
    assert_easprset(ctx, 0, 151, 142, 5, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_patset_wraps_vertical_position_near_top(test_context *ctx) {
    reset_action_state();

    sprite_pattern pattern = {0};
    sprite_pattern *patterns[1] = {&pattern};
    sprite_status actor = {0};
    actor.actno = 1;
    actor.actflg = 4;
    actor.patbase = patterns;
    set_actor_position(&actor, 120, 2050);
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 10;
    pattern.cnt = 1;
    pattern.spra[0].xoff = 3;
    pattern.spra[0].yoff = 4;
    pattern.spra[0].index = 5;
    pbuffer[0].cnt = 1;
    pbuffer[0].pActwk[0] = &actor;

    patset();

    assert_easprset(ctx, 0, 151, 124, 5, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

static void test_patset_wraps_vertical_position_near_bottom(test_context *ctx) {
    reset_action_state();

    sprite_pattern pattern = {0};
    sprite_pattern *patterns[1] = {&pattern};
    sprite_status actor = {0};
    actor.actno = 1;
    actor.actflg = 4;
    actor.patbase = patterns;
    set_actor_position(&actor, 120, 20);
    scra_h_posit.w.h = 100;
    scra_v_posit.w.h = 1800;
    pattern.cnt = 1;
    pattern.spra[0].xoff = 3;
    pattern.spra[0].yoff = 4;
    pattern.spra[0].index = 5;
    pbuffer[0].cnt = 1;
    pbuffer[0].pActwk[0] = &actor;

    patset();

    assert_easprset(ctx, 0, 151, 400, 5, 0, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, linkdata);
}

TEST_MAIN_BEGIN;

test_action_dispatches_active_actor(&ctx);
test_speedset_adds_gravity_and_moves_by_original_speed(&ctx);
test_speedset_skips_gravity_for_actfree_8(&ctx);
test_speedset_skips_gravity_for_actfree_4(&ctx);
test_speedset_keeps_fast_upward_speed_with_actfree_2(&ctx);
test_speedset_adds_gravity_at_upward_threshold(&ctx);
test_speedset_clamps_downward_speed_after_movement(&ctx);
test_speedset2_moves_without_attached_platform(&ctx);
test_speedset2_subtracts_attached_platform_speed(&ctx);
test_speedset2_adds_attached_platform_speed(&ctx);
test_speedset2_ignores_non_platform_attachment(&ctx);
test_frameout_uses_memset_callback(&ctx);
test_spatset_emits_unflipped_sprite(&ctx);
test_spatset_emits_horizontal_flip(&ctx);
test_spatset_emits_vertical_flip(&ctx);
test_spatset_combines_pattern_and_actor_flip_flags(&ctx);
test_spatset_skips_zero_index_entries(&ctx);
test_spatset_stops_at_link_capacity(&ctx);
test_scronchk_screen_bounds(&ctx);
test_scronchk2_uses_horizontal_size(&ctx);
test_actionsub_returns_when_gamepass_is_set(&ctx);
test_actionsub_queues_actor_without_screen_check(&ctx);
test_actionsub_culls_left_and_right(&ctx);
test_actionsub_culls_top_and_bottom(&ctx);
test_actionsub_queues_visible_screen_relative_actor(&ctx);
test_actionsub_wraps_vertical_position_near_top(&ctx);
test_actionsub_wraps_vertical_position_near_bottom(&ctx);
test_actionsub_does_not_queue_when_priority_full(&ctx);
test_patset_clears_all_sprite_slots_when_empty(&ctx);
test_patset_returns_without_output_on_gamepass(&ctx);
test_patset_emits_actor_pattern_then_clears_rest(&ctx);
test_patset_limits_pattern_when_single_sprite_flag_is_set(&ctx);
test_patset_applies_screen_relative_offsets(&ctx);
test_patset_wraps_vertical_position_near_top(&ctx);
test_patset_wraps_vertical_position_near_bottom(&ctx);

TEST_MAIN_END

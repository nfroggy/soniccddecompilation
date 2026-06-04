#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 flagwork[766];
Uint8 time_flag;
Uint8 plpower_m;
Uint8 plpower_a;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int actwkchk_count;
static sprite_status *actwkchk_queue[12];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int playdamageset_count;
static sprite_status *playdamageset_player;
static sprite_status *playdamageset_actor;
static int ride_on_chk_count;
static sprite_status *ride_on_chk_actor;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_result;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct);
Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r8/trap_r83.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 playdamageset(sprite_status *pActwk, sprite_status *pColliAct) {
    ++playdamageset_count;
    playdamageset_player = pActwk;
    playdamageset_actor = pColliAct;
    return -1;
}

Sint16 ride_on_chk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    (void)pPlayerwk;
    ++ride_on_chk_count;
    ride_on_chk_actor = pActwk;
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

static void queue_actwk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_trap_r83_word(sprite_status *actor, int offset, Sint16 value) {
    trap_r83_work *work = trap_r83_work_get(actor);

    switch (offset) {
    case 6:
        work->trigger_timer = value;
        break;
    case 8:
        work->origin_y = value;
        break;
    case 12:
        work->origin_x = value;
        break;
    }
}

static void reset_trap_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(flagwork, 0, sizeof(flagwork));
    time_flag = 0;
    plpower_m = 0;
    plpower_a = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    playdamageset_count = 0;
    playdamageset_player = 0;
    playdamageset_actor = 0;
    ride_on_chk_count = 0;
    ride_on_chk_actor = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    emycol_d_result = 0;
}

static void test_togeita_initializes_main_piece_and_children(test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *middle = &actwk[6];
    sprite_status *right = &actwk[7];
    sprite_status *spike = &actwk[8];

    reset_trap_state();
    main->actno = 51;
    main->actflg = 128;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    actwk[0].yposi.w.h = 112;
    queue_actwk(middle);
    queue_actwk(right);
    queue_actwk(spike);

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_chk_count);
    TEST_ASSERT_TRUE(ctx, ride_on_chk_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 300, frameout_s00_xpos);
}

static void test_togeita_userflag_one_uses_second_pattern(test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *middle = &actwk[6];
    sprite_status *right = &actwk[7];

    reset_trap_state();
    main->actno = 51;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 1;
    queue_actwk(middle);
    queue_actwk(right);

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_togeita_init_failure_clears_flag_and_frames_out(
    test_context *ctx) {
    sprite_status *main = &actwk[2];
    Uint16 flag_index;

    reset_trap_state();
    main->actno = 51;
    main->cdsts = 5;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    time_flag = 2;
    flag_index = (Uint16)(time_flag + main->cdsts * 3);
    flagwork[flag_index] = 255;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[flag_index]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
}

static void test_togeita_init_failure_with_zero_cdsts_only_frames_out(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
}

static void test_togeita_second_child_failure_clears_flag_and_frames_out(
    test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *middle = &actwk[6];
    Uint16 flag_index;

    reset_trap_state();
    main->actno = 51;
    main->cdsts = 7;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    time_flag = 1;
    flag_index = (Uint16)(time_flag + main->cdsts * 3);
    flagwork[flag_index] = 255;
    queue_actwk(middle);

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 127, flagwork[flag_index]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
}

static void test_togeita_patno_three_skips_ride_check(test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->actflg = 128;
    main->patno = 3;
    main->yposi.w.h = 120;
    actwk[0].yposi.w.h = 120;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_togeita_existing_piece_patno_three_init_returns(
    test_context *ctx) {
    sprite_status *piece = &actwk[6];

    reset_trap_state();
    piece->actno = 51;
    trap_r83_work_get(piece)->group_index = 1;
    piece->patno = 3;
    piece->userflag.b.h = 0;

    togeita(piece);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_togeita_spike_child_allocation_failure_is_ignored(
    test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *middle = &actwk[6];
    sprite_status *right = &actwk[7];

    reset_trap_state();
    main->actno = 51;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    queue_actwk(middle);
    queue_actwk(right);

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_togeita_move1_advances_when_player_is_in_trigger_box(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 130;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_chk_count);
}

static void test_togeita_move1_does_not_advance_for_player_above(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 1;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 100;

    togeita(main);

}

static void test_togeita_move1_does_not_advance_for_far_below_player(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 376;

    togeita(main);

}

static void test_togeita_move1_userflag_one_uses_wide_trigger(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->yposi.w.h = 120;
    main->userflag.b.h = 1;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 360;
    actwk[0].yposi.w.h = 130;

    togeita(main);

}

static void test_togeita_move1_rejects_player_left_of_trigger(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 240;
    actwk[0].yposi.w.h = 130;

    togeita(main);

}

static void test_togeita_move1_rejects_player_right_of_trigger(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 2;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 348;
    actwk[0].yposi.w.h = 130;

    togeita(main);

}

static void test_togeita_move2_falls_while_collision_is_below(
    test_context *ctx) {
    sprite_status *main = &actwk[2];
    Sint32 original_y;

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 1;
    original_y = main->yposi.l;
    emycol_d_result = 0;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == main);
}

static void test_togeita_move2_stops_when_collision_is_missing(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 2;
    emycol_d_result = -1;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
}

static void test_togeita_move2_existing_stop_skips_collision(test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->userflag.b.h = 0;
    trap_r83_work_get(main)->stopped = 255;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 0, emycol_d_count);
}

static void test_togeita_move2_periodically_spawns_extra_piece(
    test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *extra = &actwk[9];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    trap_r83_work_get(main)->segment_count = 3;
    set_trap_r83_word(main, 6, 299);
    set_trap_r83_word(main, 8, 120);
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 120;
    queue_actwk(extra);
    emycol_d_result = 0;

    togeita(main);

}

static void test_togeita_move2_extra_piece_later_tick_cases(test_context *ctx) {
    Sint16 tick_values[3] = {599, 839, 1079};
    int i;

    for (i = 0; i < 3; ++i) {
        sprite_status *main = &actwk[2];
        sprite_status *extra = &actwk[9];

        reset_trap_state();
        main->actno = 51;
        main->r_no0 = 4;
        main->xposi.w.h = 244;
        main->yposi.w.h = 120;
        main->userflag.b.h = 0;
        trap_r83_work_get(main)->segment_count = 2;
        set_trap_r83_word(main, 6, tick_values[i]);
        set_trap_r83_word(main, 8, 120);
        set_trap_r83_word(main, 12, 300);
        actwk[0].xposi.w.h = 300;
        actwk[0].yposi.w.h = 120;
        queue_actwk(extra);
        emycol_d_result = 0;

        togeita(main);

    }
}

static void test_togeita_move2_skips_extra_piece_for_far_above_player(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    trap_r83_work_get(main)->segment_count = 3;
    set_trap_r83_word(main, 6, 299);
    set_trap_r83_word(main, 8, 120);
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = -8;
    emycol_d_result = 0;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_togeita_move2_skips_extra_piece_for_outside_x(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    trap_r83_work_get(main)->segment_count = 3;
    set_trap_r83_word(main, 6, 299);
    set_trap_r83_word(main, 8, 120);
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 120;
    emycol_d_result = 0;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_togeita_move2_extra_piece_allocation_failure_is_ignored(
    test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 4;
    main->xposi.w.h = 244;
    main->yposi.w.h = 120;
    main->userflag.b.h = 0;
    trap_r83_work_get(main)->segment_count = 3;
    set_trap_r83_word(main, 6, 299);
    set_trap_r83_word(main, 8, 120);
    set_trap_r83_word(main, 12, 300);
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 120;
    emycol_d_result = 0;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
}

static void test_togeita_move3_keeps_actor_state(test_context *ctx) {
    sprite_status *main = &actwk[2];

    reset_trap_state();
    main->actno = 51;
    main->r_no0 = 6;

    togeita(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_togeitax_follows_live_parent(test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *child = &actwk[8];

    reset_trap_state();
    main->actno = 51;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    queue_actwk(&actwk[6]);
    queue_actwk(&actwk[7]);
    queue_actwk(child);
    togeita(main);
    actionsub_count = 0;
    main->yspeed.w = 128;
    main->yposi.w.h = 180;

    togeita(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_togeitax_frames_out_when_parent_is_gone(test_context *ctx) {
    sprite_status *main = &actwk[2];
    sprite_status *child = &actwk[8];

    reset_trap_state();
    main->actno = 51;
    main->xposi.w.h = 300;
    main->yposi.w.h = 120;
    queue_actwk(&actwk[6]);
    queue_actwk(&actwk[7]);
    queue_actwk(child);
    togeita(main);
    main->actno = 0;

    togeita(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_togedair8_delegates_negative_userflag_to_harir8(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->userflag.b.h = -1;
    spike->xposi.w.h = 200;
    spike->yposi.w.h = 100;

    togedair8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_togedair8_initializes_platform_child(test_context *ctx) {
    sprite_status *platform = &actwk[3];
    sprite_status *child = &actwk[7];

    reset_trap_state();
    platform->actno = 54;
    platform->actflg = 128;
    platform->xposi.w.h = 144;
    platform->yposi.w.h = 208;
    actwk[0].yposi.w.h = 220;
    queue_actwk(child);

    togedair8(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_togedair8_allocation_failure_keeps_init_state(
    test_context *ctx) {
    sprite_status *platform = &actwk[3];

    reset_trap_state();
    platform->actno = 54;
    platform->actflg = 128;
    platform->xposi.w.h = 144;
    platform->yposi.w.h = 208;
    actwk[0].yposi.w.h = 220;

    togedair8(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_togedair8_userflag_one_spawns_harir8_child(test_context *ctx) {
    sprite_status *platform = &actwk[3];
    sprite_status *child = &actwk[7];

    reset_trap_state();
    platform->actno = 54;
    platform->userflag.b.h = 1;
    platform->xposi.w.h = 144;
    platform->yposi.w.h = 208;
    queue_actwk(child);

    togedair8(platform);

}

static void test_togedair8_move_absorbs_negative_player_delta(
    test_context *ctx) {
    sprite_status *platform = &actwk[3];

    reset_trap_state();
    platform->actno = 54;
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->yposi.w.h = 208;
    actwk[0].yposi.w.h = 200;

    togedair8(platform);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == platform);
}

static void test_togedair8_move_skips_far_player(test_context *ctx) {
    sprite_status *platform = &actwk[3];

    reset_trap_state();
    platform->actno = 54;
    platform->r_no0 = 2;
    platform->actflg = 128;
    platform->yposi.w.h = 208;
    actwk[0].yposi.w.h = 260;

    togedair8(platform);

    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_harir8_patno_zero_damages_player(test_context *ctx) {
    sprite_status *spike = &actwk[5];
    sprite_status *player = &actwk[0];
    Sint32 original_y;

    reset_trap_state();
    spike->actno = 54;
    spike->userflag.b.h = -1;
    spike->xposi.w.h = 200;
    spike->yposi.w.h = 100;
    spike->cddat = 8;
    player->yposi.w.h = 112;
    player->yspeed.w = 3;
    original_y = player->yposi.l;
    hitchk_result = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, playdamageset_count);
    TEST_ASSERT_TRUE(ctx, playdamageset_player == player);
    TEST_ASSERT_TRUE(ctx, playdamageset_actor == spike);
}

static void test_harir8_patno_zero_skips_when_far(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_harir8_patno_zero_skips_when_hitchk_misses(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;
    hitchk_result = 0;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir8_patno_zero_skips_when_collision_flag_clear(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->cddat = 0;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;
    hitchk_result = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir8_patno_zero_skips_when_player_powered(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->cddat = 8;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;
    hitchk_result = 1;
    plpower_m = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir8_patno_zero_skips_when_player_action_blocks(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->cddat = 8;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;
    actwk[0].r_no0 = 4;
    hitchk_result = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir8_patno_zero_skips_when_hidden_player_flag_blocks(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = -1;
    spike->cddat = 8;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;
    player_work_get(&actwk[0])->damage_invulnerability_timer = 1;
    hitchk_result = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, playdamageset_count);
}

static void test_harir8_solid_spike_checks_when_active(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->userflag.b.h = 0;
    spike->actflg = 128;
    spike->xposi.w.h = 200;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 112;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
}

static void test_harir8_solid_spike_absorbs_negative_player_delta(
    test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = 0;
    spike->actflg = 128;
    spike->patno = 1;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 90;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == spike);
}

static void test_harir8_solid_spike_ignores_when_inactive(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = 0;
    spike->patno = 1;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_harir8_solid_spike_ignores_far_player(test_context *ctx) {
    sprite_status *spike = &actwk[5];

    reset_trap_state();
    spike->actno = 54;
    spike->r_no0 = 2;
    spike->userflag.b.h = 0;
    spike->actflg = 128;
    spike->patno = 1;
    spike->yposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    harir8(spike);

    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
}

static void test_harir8_master_child_follows_parent(test_context *ctx) {
    sprite_status *platform = &actwk[3];
    sprite_status *child = &actwk[7];

    reset_trap_state();
    platform->actno = 54;
    platform->userflag.b.h = 1;
    platform->xposi.w.h = 144;
    platform->yposi.w.h = 208;
    queue_actwk(child);
    togedair8(platform);
    actionsub_count = 0;
    frameout_s00_count = 0;
    platform->xposi.w.h = 180;
    platform->yposi.w.h = 90;
    trap_r83_work_get(child)->follow_x_offset = (Uint8)-8;

    harir8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
}

static void test_harir8_master_child_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *platform = &actwk[3];
    sprite_status *child = &actwk[7];

    reset_trap_state();
    platform->actno = 54;
    platform->userflag.b.h = 1;
    platform->xposi.w.h = 144;
    platform->yposi.w.h = 208;
    queue_actwk(child);
    togedair8(platform);
    actionsub_count = 0;
    frameout_s00_count = 0;
    platform->actno = 0;

    harir8(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s00_count);
}

TEST_MAIN_BEGIN;
    test_togeita_initializes_main_piece_and_children(&ctx);
    test_togeita_userflag_one_uses_second_pattern(&ctx);
    test_togeita_init_failure_clears_flag_and_frames_out(&ctx);
    test_togeita_init_failure_with_zero_cdsts_only_frames_out(&ctx);
    test_togeita_second_child_failure_clears_flag_and_frames_out(&ctx);
    test_togeita_patno_three_skips_ride_check(&ctx);
    test_togeita_existing_piece_patno_three_init_returns(&ctx);
    test_togeita_spike_child_allocation_failure_is_ignored(&ctx);
    test_togeita_move1_advances_when_player_is_in_trigger_box(&ctx);
    test_togeita_move1_does_not_advance_for_player_above(&ctx);
    test_togeita_move1_does_not_advance_for_far_below_player(&ctx);
    test_togeita_move1_userflag_one_uses_wide_trigger(&ctx);
    test_togeita_move1_rejects_player_left_of_trigger(&ctx);
    test_togeita_move1_rejects_player_right_of_trigger(&ctx);
    test_togeita_move2_falls_while_collision_is_below(&ctx);
    test_togeita_move2_stops_when_collision_is_missing(&ctx);
    test_togeita_move2_existing_stop_skips_collision(&ctx);
    test_togeita_move2_periodically_spawns_extra_piece(&ctx);
    test_togeita_move2_extra_piece_later_tick_cases(&ctx);
    test_togeita_move2_skips_extra_piece_for_far_above_player(&ctx);
    test_togeita_move2_skips_extra_piece_for_outside_x(&ctx);
    test_togeita_move2_extra_piece_allocation_failure_is_ignored(&ctx);
    test_togeita_move3_keeps_actor_state(&ctx);
    test_togeitax_follows_live_parent(&ctx);
    test_togeitax_frames_out_when_parent_is_gone(&ctx);
    test_togedair8_delegates_negative_userflag_to_harir8(&ctx);
    test_togedair8_initializes_platform_child(&ctx);
    test_togedair8_allocation_failure_keeps_init_state(&ctx);
    test_togedair8_userflag_one_spawns_harir8_child(&ctx);
    test_togedair8_move_absorbs_negative_player_delta(&ctx);
    test_togedair8_move_skips_far_player(&ctx);
    test_harir8_patno_zero_damages_player(&ctx);
    test_harir8_patno_zero_skips_when_far(&ctx);
    test_harir8_patno_zero_skips_when_hitchk_misses(&ctx);
    test_harir8_patno_zero_skips_when_collision_flag_clear(&ctx);
    test_harir8_patno_zero_skips_when_player_powered(&ctx);
    test_harir8_patno_zero_skips_when_player_action_blocks(&ctx);
    test_harir8_patno_zero_skips_when_hidden_player_flag_blocks(&ctx);
    test_harir8_solid_spike_checks_when_active(&ctx);
    test_harir8_solid_spike_absorbs_negative_player_delta(&ctx);
    test_harir8_solid_spike_ignores_when_inactive(&ctx);
    test_harir8_solid_spike_ignores_far_player(&ctx);
    test_harir8_master_child_follows_parent(&ctx);
    test_harir8_master_child_frames_out_when_parent_is_gone(&ctx);
TEST_MAIN_END


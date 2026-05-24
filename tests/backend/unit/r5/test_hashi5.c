#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union editmode;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static int ride_on_set_count;
static sprite_status *ride_on_set_actor;
static sprite_status *ride_on_set_player;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 ride_on_set(sprite_status *pActwk, sprite_status *pPlayerwk);

#include "src/r5/hashi5.c"

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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 ride_on_set(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_set_count;
    ride_on_set_actor = pActwk;
    ride_on_set_player = pPlayerwk;
    return 0;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 256;
    sinset_cos = 0;
    ride_on_set_count = 0;
    ride_on_set_actor = 0;
    ride_on_set_player = 0;
}

static void reset_hashi5_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&editmode, 0, sizeof(editmode));
    reset_logs();
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_bridge_children(int first_index) {
    int i;

    for (i = 0; i < 8; ++i) {
        queue_actor(&actwk[first_index + i]);
    }
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

static void setup_initialized_bridge(sprite_status *bridge) {
    reset_hashi5_state();
    bridge->actno = 47;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 120;
    queue_bridge_children(20);

    hashi5(bridge);
    reset_logs();
}

static void assert_tail_action(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
}

static void test_pattern_and_sag_tables_capture_literal_data(
    test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, hashi5pat[0] == &hashi5_pat0);
    TEST_ASSERT_TRUE(ctx, hashi5pat[1] == &hashi5_pat1);
    TEST_ASSERT_TRUE(ctx, hashi5pat[2] == &hashi5_pat2);
    TEST_ASSERT_EQ_INT(ctx, -8, hashi5_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, hashi5_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 429, hashi5_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -72, hashi5_pat2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 430, hashi5_pat2.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, hashitbl[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, hashitbl[271]);
    TEST_ASSERT_EQ_INT(ctx, 255, hashitbl2[0]);
    TEST_ASSERT_EQ_INT(ctx, 181, hashitbl2[16]);
}

static void test_init_builds_controller_and_eight_segments(test_context *ctx) {
    sprite_status *bridge = &actwk[3];
    int i;

    reset_hashi5_state();
    bridge->actno = 47;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 120;
    queue_bridge_children(20);

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 2, bridge->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actflg);
    TEST_ASSERT_EQ_INT(ctx, 3, bridge->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 17152, bridge->sproffset);
    TEST_ASSERT_TRUE(ctx, bridge->patbase == hashi5pat);
    TEST_ASSERT_EQ_INT(ctx, 64, bridge->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, bridge->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, bridge->patno);
    TEST_ASSERT_EQ_INT(ctx, 100, get_actfree_word(bridge, 12));
    TEST_ASSERT_EQ_INT(ctx, 120, get_actfree_word(bridge, 8));
    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    assert_tail_action(ctx, bridge);

    for (i = 0; i < 8; ++i) {
        sprite_status *segment = &actwk[20 + i];
        TEST_ASSERT_EQ_INT(ctx, 20 + i, bridge->actfree[i]);
        TEST_ASSERT_EQ_INT(ctx, 47, segment->actno);
        TEST_ASSERT_EQ_INT(ctx, 4, segment->r_no0);
        TEST_ASSERT_EQ_INT(ctx, 3, segment->sprpri);
        TEST_ASSERT_EQ_INT(ctx, 17152, segment->sproffset);
        TEST_ASSERT_TRUE(ctx, segment->patbase == hashi5pat);
        TEST_ASSERT_EQ_INT(ctx, 3, get_actfree_word(segment, 10));
        TEST_ASSERT_EQ_INT(ctx, 255, segment->actfree[21]);
        TEST_ASSERT_EQ_INT(ctx, 7 - i, segment->actfree[18]);
        TEST_ASSERT_EQ_INT(ctx, 36 + i * 16, segment->xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 120, segment->yposi.w.h);
    }
}

static void test_init_allocation_failure_leaves_missing_slots_zero(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    reset_hashi5_state();
    bridge->actno = 47;
    bridge->xposi.w.h = 100;
    bridge->yposi.w.h = 120;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[7]);
    assert_tail_action(ctx, bridge);
}

static void test_move_ride_sags_bridge_and_places_player(test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    setup_initialized_bridge(bridge);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 115;
    actwk[0].sprvsize = 8;
    actwk[0].yspeed.w = 0;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_set_count);
    TEST_ASSERT_TRUE(ctx, ride_on_set_actor == bridge);
    TEST_ASSERT_TRUE(ctx, ride_on_set_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 112, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 4, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == bridge);
    assert_tail_action(ctx, bridge);
}

static void test_move_at_full_sag_keeps_angle_and_updates_player(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    setup_initialized_bridge(bridge);
    bridge->actfree[16] = 64;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 115;
    actwk[0].sprvsize = 8;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 64, bridge->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 64, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    assert_tail_action(ctx, bridge);
}

static void test_move_without_ride_recovers_and_clears_stale_player_link(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    setup_initialized_bridge(bridge);
    bridge->actfree[16] = 8;
    actwk[0].r_no0 = 6;
    actwk[0].actfree[19] = 3;
    actwk[0].actfree[14] = 77;
    actwk[0].cddat = 8;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
    TEST_ASSERT_EQ_INT(ctx, 4, bridge->actfree[16]);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[19]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    assert_tail_action(ctx, bridge);

    reset_logs();
    bridge->actfree[16] = 0;
    actwk[0].actfree[19] = 0;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    assert_tail_action(ctx, bridge);
}

static void test_ride_check_rejects_editmode_speed_and_position_cases(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    setup_initialized_bridge(bridge);
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 115;
    actwk[0].sprvsize = 8;
    editmode.b.h = 1;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[19]);

    reset_logs();
    editmode.b.h = 0;
    actwk[0].yspeed.w = -1;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);

    reset_logs();
    actwk[0].yspeed.w = 0;
    actwk[0].xposi.w.h = 300;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);

    reset_logs();
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 40;

    hashi5(bridge);

    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_set_count);
}

static void test_posiget_rejects_flagged_negative_and_far_positions(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];

    setup_initialized_bridge(bridge);
    actwk[0].cddat = 2;
    hashi5_posiget1p(bridge);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[19]);

    actwk[0].cddat = 0;
    actwk[0].xposi.w.h = 20;
    hashi5_posiget1p(bridge);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[19]);

    actwk[0].xposi.w.h = 160;
    hashi5_posiget1p(bridge);
    TEST_ASSERT_EQ_INT(ctx, 0, bridge->actfree[19]);
}

static void test_posiset_covers_return_and_no_second_side_paths(
    test_context *ctx) {
    sprite_status *bridge = &actwk[3];
    int i;

    setup_initialized_bridge(bridge);
    for (i = 0; i < 9; ++i) {
        bridge->actfree[i] = (Uint8)(20 + (i % 8));
    }
    bridge->actfree[16] = 12;

    bridge->actfree[19] = 7;
    hashi5_posiset(bridge);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 12, sinset_angle);

    reset_logs();
    bridge->actfree[19] = 8;
    hashi5_posiset(bridge);
    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 12, sinset_angle);
}

static void test_segment_move2_liveness_paths(test_context *ctx) {
    sprite_status *bridge = &actwk[3];
    sprite_status *segment = &actwk[20];

    setup_initialized_bridge(bridge);
    reset_logs();
    hashi5(segment);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    assert_tail_action(ctx, segment);

    reset_logs();
    bridge->actfree[21] = 1;
    hashi5(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == segment);
    assert_tail_action(ctx, segment);

    reset_logs();
    bridge->actfree[21] = 0;
    bridge->actno = 0;
    hashi5(segment);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == segment);
    assert_tail_action(ctx, segment);
}

TEST_MAIN_BEGIN;
    test_pattern_and_sag_tables_capture_literal_data(&ctx);
    test_init_builds_controller_and_eight_segments(&ctx);
    test_init_allocation_failure_leaves_missing_slots_zero(&ctx);
    test_move_ride_sags_bridge_and_places_player(&ctx);
    test_move_at_full_sag_keeps_angle_and_updates_player(&ctx);
    test_move_without_ride_recovers_and_clears_stale_player_link(&ctx);
    test_ride_check_rejects_editmode_speed_and_position_cases(&ctx);
    test_posiget_rejects_flagged_negative_and_far_positions(&ctx);
    test_posiset_covers_return_and_no_second_side_paths(&ctx);
    test_segment_move2_liveness_paths(&ctx);
TEST_MAIN_END

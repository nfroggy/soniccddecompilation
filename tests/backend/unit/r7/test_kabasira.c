#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;
static int soundset_count;
static Sint16 soundset_requests[8];
static int sinset_count;
static Uint8 sinset_angles[8];
static Sint16 sinset_sin_result;
static Sint16 sinset_cos_result;
static int atan_sonic_count;
static Sint16 atan_sonic_x;
static Sint16 atan_sonic_y;
static Sint16 atan_sonic_result;
static int enemy_suicide_count;
static Sint16 enemy_suicide_result;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
void soundset(Sint16 ReqNo);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 atan_sonic(Sint16 x, Sint16 y);
Sint16 enemy_suicide(sprite_status *pActwk);

#include "src/r7/kabasira.c"

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

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
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

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) /
                               sizeof(soundset_requests[0]))) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    if (sinset_count < (int)(sizeof(sinset_angles) / sizeof(sinset_angles[0]))) {
        sinset_angles[sinset_count] = kakudo;
    }
    ++sinset_count;
    *sin = sinset_sin_result;
    *cos = sinset_cos_result;
}

Sint16 atan_sonic(Sint16 x, Sint16 y) {
    ++atan_sonic_count;
    atan_sonic_x = x;
    atan_sonic_y = y;
    return atan_sonic_result;
}

Sint16 enemy_suicide(sprite_status *pActwk) {
    ++enemy_suicide_count;
    (void)pActwk;
    return enemy_suicide_result;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sinset_count = 0;
    memset(sinset_angles, 0, sizeof(sinset_angles));
    sinset_sin_result = 0;
    sinset_cos_result = 0;
    atan_sonic_count = 0;
    atan_sonic_x = 0;
    atan_sonic_y = 0;
    atan_sonic_result = 0;
    enemy_suicide_count = 0;
    enemy_suicide_result = 0;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void assert_render_callbacks(test_context *ctx, sprite_status *actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void assert_initialized_actor(test_context *ctx, sprite_status *actor,
                                     Uint8 expected_mstno) {
    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 9158, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_kabasira);
    TEST_ASSERT_EQ_INT(ctx, expected_mstno, actor->mstno.b.h);
}

static void test_kabasira_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg_00);
    TEST_ASSERT_TRUE(ctx, pchg[8] == pchg_08);
    TEST_ASSERT_EQ_INT(ctx, 15, pchg_00[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, pchg_00[17]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg_08[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, pchg_08[1]);
    TEST_ASSERT_TRUE(ctx, pat_kabasira[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_kabasira[13] == &pat0d);
    TEST_ASSERT_EQ_INT(ctx, 2, pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -8, pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 468, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 493, pat0d.spra[0].index);
}

static void test_kabasira_suicide_short_circuits_normal_actor(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_state();
    actor->actno = 34;
    enemy_suicide_result = 1;

    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_kabasira_init_builds_four_segment_chain(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *child0 = &actwk[20];
    sprite_status *child1 = &actwk[21];
    sprite_status *child2 = &actwk[22];

    reset_state();
    queue_actwkchk2(child0);
    queue_actwkchk2(child1);
    queue_actwkchk2(child2);
    actor->actno = 34;
    actor->actflg = 128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 49, actor->colino);
    assert_initialized_actor(ctx, actor, 4);
    assert_initialized_actor(ctx, child0, 5);
    assert_initialized_actor(ctx, child1, 6);
    assert_initialized_actor(ctx, child2, 7);
    TEST_ASSERT_EQ_INT(ctx, -1, child0->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 34, child0->actno);
    TEST_ASSERT_EQ_INT(ctx, 100, child0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, child0->yposi.w.h);
    assert_render_callbacks(ctx, actor);
}

static void test_kabasira_init_uses_compact_chain_when_userflag_high(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_state();
    queue_actwkchk2(&actwk[20]);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    actor->actno = 34;
    actor->userflag.b.h = 1;

    kabasira(actor);

    assert_initialized_actor(ctx, actor, 0);
    assert_initialized_actor(ctx, &actwk[20], 1);
    assert_initialized_actor(ctx, &actwk[21], 2);
    assert_initialized_actor(ctx, &actwk[22], 3);
}

static void test_kabasira_init_frames_parent_when_child_allocation_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_state();
    actor->actno = 34;

    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_kabasira_child_moves_when_parent_is_alive(test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[20];

    reset_state();
    queue_actwkchk2(child);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    parent->actno = 34;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    kabasira(parent);

    reset_logs();
    sinset_sin_result = 256;
    sinset_cos_result = 512;

    kabasira(child);

    TEST_ASSERT_EQ_INT(ctx, 0, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 2, sinset_count);
    assert_render_callbacks(ctx, child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_kabasira_child_frames_out_when_parent_is_gone(
    test_context *ctx) {
    sprite_status *parent = &actwk[4];
    sprite_status *child = &actwk[20];

    reset_state();
    queue_actwkchk2(child);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    parent->actno = 34;
    kabasira(parent);

    reset_logs();
    parent->actno = 33;

    kabasira(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_kabasira_projectile_collision_becomes_explosion(
    test_context *ctx) {
    sprite_status *projectile = &actwk[10];

    reset_state();
    projectile->actno = 34;
    projectile->userflag.b.l = -2;
    projectile->xposi.l = 100 << 16;
    projectile->yposi.l = 200 << 16;
    projectile->colicnt = 1;
    kabasira_get_work(projectile)->x_speed = 3 << 16;
    kabasira_get_work(projectile)->y_speed = -2 << 16;

    kabasira(projectile);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == projectile);
    TEST_ASSERT_EQ_INT(ctx, 24, projectile->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, projectile->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 103, projectile->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 198, projectile->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_kabasira_projectile_frames_out_when_vertically_far(
    test_context *ctx) {
    sprite_status *projectile = &actwk[10];

    reset_state();
    actwk[0].yposi.w.h = 500;
    projectile->userflag.b.l = -2;
    projectile->yposi.w.h = 0;

    kabasira(projectile);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == projectile);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_kabasira_projectile_near_player_renders(test_context *ctx) {
    sprite_status *projectile = &actwk[10];

    reset_state();
    actwk[0].yposi.w.h = 180;
    projectile->userflag.b.l = -2;
    projectile->yposi.w.h = 200;

    kabasira(projectile);

    assert_render_callbacks(ctx, projectile);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_kabasira_move_spawns_projectile_inside_area(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *projectile = &actwk[30];

    reset_state();
    queue_actwkchk2(&actwk[20]);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    actor->actno = 34;
    actor->actflg = 128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 150;
    actwk[0].yposi.w.h = 230;
    kabasira(actor);

    reset_logs();
    queue_actwkchk(projectile);
    atan_sonic_result = 37;
    sinset_sin_result = 256;
    sinset_cos_result = -512;

    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, atan_sonic_count);
    TEST_ASSERT_EQ_INT(ctx, 50, atan_sonic_x);
    TEST_ASSERT_EQ_INT(ctx, 30, atan_sonic_y);
    TEST_ASSERT_EQ_INT(ctx, 3, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 37, sinset_angles[0]);
    TEST_ASSERT_EQ_INT(ctx, -2, projectile->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 241, projectile->colino);
    TEST_ASSERT_EQ_INT(ctx, 34, projectile->actno);
    TEST_ASSERT_EQ_INT(ctx, 5, projectile->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, projectile->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 160, soundset_requests[0]);
    assert_render_callbacks(ctx, actor);
}

static void test_kabasira_move_skips_projectile_while_timer_runs(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_state();
    queue_actwkchk2(&actwk[20]);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    actor->actno = 34;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 150;
    actwk[0].yposi.w.h = 230;
    kabasira(actor);

    reset_logs();
    queue_actwkchk(&actwk[30]);
    kabasira(actor);

    reset_logs();
    queue_actwkchk(&actwk[31]);
    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, atan_sonic_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_render_callbacks(ctx, actor);
}

static void test_kabasira_move_does_not_spawn_outside_area(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_state();
    queue_actwkchk2(&actwk[20]);
    queue_actwkchk2(&actwk[21]);
    queue_actwkchk2(&actwk[22]);
    actor->actno = 34;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 400;
    actwk[0].yposi.w.h = 230;
    kabasira(actor);

    reset_logs();
    queue_actwkchk(&actwk[30]);
    kabasira(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, atan_sonic_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    assert_render_callbacks(ctx, actor);
}

static void test_kabasira_area_bounds_are_inclusive_on_inner_edges(
    test_context *ctx) {
    sprite_status actor;
    sprite_status player;
    Sint16 x = 99;
    Sint16 y = 99;

    memset(&actor, 0, sizeof(actor));
    memset(&player, 0, sizeof(player));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    player.xposi.w.h = 219;
    player.yposi.w.h = 295;

    TEST_ASSERT_EQ_INT(ctx, 1, kabasira_area(&actor, &player, &x, &y));
    TEST_ASSERT_EQ_INT(ctx, 119, x);
    TEST_ASSERT_EQ_INT(ctx, 95, y);

    player.xposi.w.h = 220;
    TEST_ASSERT_EQ_INT(ctx, 0, kabasira_area(&actor, &player, &x, &y));

    player.xposi.w.h = 100;
    player.yposi.w.h = 296;
    TEST_ASSERT_EQ_INT(ctx, 0, kabasira_area(&actor, &player, &x, &y));
}

TEST_MAIN_BEGIN;
    test_kabasira_tables_capture_literal_data(&ctx);
    test_kabasira_suicide_short_circuits_normal_actor(&ctx);
    test_kabasira_init_builds_four_segment_chain(&ctx);
    test_kabasira_init_uses_compact_chain_when_userflag_high(&ctx);
    test_kabasira_init_frames_parent_when_child_allocation_fails(&ctx);
    test_kabasira_child_moves_when_parent_is_alive(&ctx);
    test_kabasira_child_frames_out_when_parent_is_gone(&ctx);
    test_kabasira_projectile_collision_becomes_explosion(&ctx);
    test_kabasira_projectile_frames_out_when_vertically_far(&ctx);
    test_kabasira_projectile_near_player_renders(&ctx);
    test_kabasira_move_spawns_projectile_inside_area(&ctx);
    test_kabasira_move_skips_projectile_while_timer_runs(&ctx);
    test_kabasira_move_does_not_spawn_outside_area(&ctx);
    test_kabasira_area_bounds_are_inclusive_on_inner_edges(&ctx);
TEST_MAIN_END

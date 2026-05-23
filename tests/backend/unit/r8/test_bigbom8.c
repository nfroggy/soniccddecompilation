#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 prio_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s00_count;
static sprite_status *frameout_s00_actor;
static Sint16 frameout_s00_xpos;
static int frameout_s0_count;
static sprite_status *frameout_s0_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int emycol_d_count;
static sprite_status *emycol_d_actor;
static Sint16 emycol_d_result;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi);
Sint32 frameout_s0(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 emycol_d(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r8/bigbom8.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s00(sprite_status *pActwk, Sint16 xposi) {
    ++frameout_s00_count;
    frameout_s00_actor = pActwk;
    frameout_s00_xpos = xposi;
    return 0;
}

Sint32 frameout_s0(sprite_status *pActwk) {
    ++frameout_s0_count;
    frameout_s0_actor = pActwk;
    return 0;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
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

Sint16 emycol_d(sprite_status *pActwk) {
    ++emycol_d_count;
    emycol_d_actor = pActwk;
    return emycol_d_result;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void set_actfree_long(sprite_status *actor, int offset, Sint32 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint32)value >> 8);
    actor->actfree[offset + 2] = (Uint8)((Uint32)value >> 16);
    actor->actfree[offset + 3] = (Uint8)((Uint32)value >> 24);
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s00_count = 0;
    frameout_s00_actor = 0;
    frameout_s00_xpos = 0;
    frameout_s0_count = 0;
    frameout_s0_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    emycol_d_count = 0;
    emycol_d_actor = 0;
    emycol_d_result = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void reset_bigbom8_state(void) {
    memset(actwk, 0, sizeof(actwk));
    prio_flag = 0;
    reset_logs();
}

static void init_main_with_body(sprite_status *main_actor,
                                sprite_status *body_actor) {
    reset_bigbom8_state();
    main_actor->actno = 35;
    main_actor->xposi.w.h = 100;
    main_actor->yposi.w.h = 200;
    queue_actor(body_actor);
    bigbom(main_actor);
    reset_logs();
}

static void test_bigbom8_patterns_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_bigbom[0] == &pat000);
    TEST_ASSERT_TRUE(ctx, pat_bigbom[4] == &pat004);
    TEST_ASSERT_EQ_INT(ctx, 3, pat000.cnt);
    TEST_ASSERT_EQ_INT(ctx, -4, pat000.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -44, pat000.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 482, pat000.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -20, pat004.spra[1].xoff);
    TEST_ASSERT_EQ_INT(ctx, -20, pat004.spra[1].yoff);
    TEST_ASSERT_EQ_INT(ctx, 495, pat004.spra[1].index);
    TEST_ASSERT_TRUE(ctx, pat1[0] == &pat100);
    TEST_ASSERT_EQ_INT(ctx, 496, pat100.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pat2[4] == &pat204);
    TEST_ASSERT_EQ_INT(ctx, 502, pat204.spra[0].index);
    TEST_ASSERT_TRUE(ctx, pat3[1] == &pat301);
    TEST_ASSERT_EQ_INT(ctx, 504, pat301.spra[0].index);
}

static void test_bigbom_main_initializes_body_and_frame_origin(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    reset_bigbom8_state();
    main_actor->actno = 35;
    main_actor->xposi.w.h = 100;
    main_actor->yposi.w.h = 200;
    main_actor->userflag.b.l = 1;
    queue_actor(body_actor);

    bigbom(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 35, body_actor->actno);
    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, body_actor->userflag.b.l);
    TEST_ASSERT_EQ_INT(ctx, 100, body_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, body_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_TRUE(ctx, frameout_s00_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_bigbom_main_allocation_failure_uses_frameout_s0(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];

    reset_bigbom8_state();
    main_actor->actno = 35;
    main_actor->xposi.w.h = 140;

    bigbom(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == main_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 140, frameout_s00_xpos);
}

static void test_bigbom_main_countdown_spawns_head_actor(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    sprite_status *head_actor = &actwk[21];
    int i;

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 4;
    body_actor->actflg = 1;
    queue_actor(head_actor);

    bigbom(main_actor);
    for (i = 0; i < 29; ++i) {
        bigbom(main_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 4, head_actor->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 100, head_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 169, head_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, head_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, head_actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 30, frameout_s00_count);
}

static void test_bigbom_main_head_spawn_allocation_failure_frames_out(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    int i;

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 4;

    bigbom(main_actor);
    for (i = 0; i < 29; ++i) {
        bigbom(main_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);
}

static void test_bigbom_main_late_sequence_spawns_explosion_and_fire(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    sprite_status *explosion = &actwk[21];
    sprite_status *fires[7] = {&actwk[22], &actwk[23], &actwk[24], &actwk[25],
                               &actwk[26], &actwk[27], &actwk[28]};
    int i;

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 10;
    queue_actor(explosion);

    for (i = 0; i < 30; ++i) {
        bigbom(main_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 12, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 35, explosion->actno);
    TEST_ASSERT_EQ_INT(ctx, 6, explosion->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 100, explosion->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, explosion->yposi.w.h);

    for (i = 0; i < 7; ++i) {
        queue_actor(fires[i]);
    }

    bigbom(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    for (i = 0; i < 7; ++i) {
        TEST_ASSERT_EQ_INT(ctx, 35, fires[i]->actno);
        TEST_ASSERT_EQ_INT(ctx, 8, fires[i]->userflag.b.h);
        TEST_ASSERT_EQ_INT(ctx, 100, fires[i]->xposi.w.h);
        TEST_ASSERT_EQ_INT(ctx, 200, fires[i]->yposi.w.h);
    }

    bigbom(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, body_actor->r_no0);

    for (i = 0; i < 8; ++i) {
        bigbom(main_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);
}

static void test_bigbom_main_empty_move4_dispatch_keeps_state(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 8;
    main_actor->xposi.w.h = 123;
    main_actor->yposi.w.h = 456;

    bigbom(main_actor);

    TEST_ASSERT_EQ_INT(ctx, 8, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 123, main_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, main_actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s00_count);
    TEST_ASSERT_EQ_INT(ctx, 100, frameout_s00_xpos);
}

static void test_bigbom_main_late_sequence_frames_out_when_explosion_fails(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    int i;

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 10;

    for (i = 0; i < 30; ++i) {
        bigbom(main_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main_actor);
}

static void test_bigbom_make_fire_allocation_failure_uses_frameout_s0(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *new_actor = 0;

    reset_bigbom8_state();
    main_actor->actno = 35;

    make_fire(main_actor, &new_actor);

    TEST_ASSERT_TRUE(ctx, new_actor == 0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s0_count);
    TEST_ASSERT_TRUE(ctx, frameout_s0_actor == main_actor);
}

static void test_bigbom_body_falls_then_updates_parent(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    init_main_with_body(main_actor, body_actor);
    emycol_d_result = -3;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, emycol_d_count);
    TEST_ASSERT_TRUE(ctx, emycol_d_actor == body_actor);
    TEST_ASSERT_EQ_INT(ctx, 12, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 166, body_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == body_actor);
    TEST_ASSERT_EQ_INT(ctx, body_actor->xposi.w.h, main_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, body_actor->yposi.w.h, main_actor->yposi.w.h);
}

static void test_bigbom_body_fall_for_userflag_two_toggles_direction(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    reset_bigbom8_state();
    main_actor->actno = 35;
    main_actor->userflag.b.l = 2;
    main_actor->xposi.w.h = 100;
    main_actor->yposi.w.h = 200;
    queue_actor(body_actor);
    bigbom(main_actor);
    reset_logs();
    emycol_d_result = -2;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 4, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, body_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, body_actor->cddat);
    TEST_ASSERT_EQ_INT(ctx, 0, body_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_bigbom_body_frames_out_when_parent_is_missing(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    init_main_with_body(main_actor, body_actor);
    main_actor->actno = 0;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_bigbom_body_wait_and_constant_move_paths(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    int i;

    init_main_with_body(main_actor, body_actor);
    body_actor->r_no0 = 4;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;
    body_actor->actfree[19] = 166;
    actwk[0].xposi.w.h = 120;
    actwk[0].yposi.w.h = 200;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 8, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, body_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 102, body_actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 166, body_actor->colino);

    prio_flag = 1;
    for (i = 0; i < 230; ++i) {
        bigbom(body_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 10, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, body_actor->colino);
}

static void test_bigbom_body_prio_move_stop_and_die_paths(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];

    init_main_with_body(main_actor, body_actor);
    body_actor->r_no0 = 10;
    body_actor->xposi.w.h = 100;
    body_actor->yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 16, body_actor->r_no0);

    body_actor->r_no0 = 12;
    body_actor->patno = 3;
    body_actor->userflag.b.l = 1;
    main_actor->r_no0 = 8;
    reset_logs();

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 18, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, body_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, body_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, body_actor->cddat);

    body_actor->r_no0 = 20;
    body_actor->userflag.b.l = 2;
    body_actor->actfree[19] = 166;
    prio_flag = 0;

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 22, body_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, body_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 166, body_actor->colino);

    body_actor->r_no0 = 24;
    reset_logs();

    bigbom(body_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == body_actor);
}

static void test_bigbom_body_check_uses_unsigned_bounds(test_context *ctx) {
    sprite_status actor;
    sprite_status player;

    memset(&actor, 0, sizeof(actor));
    memset(&player, 0, sizeof(player));
    actor.xposi.w.h = 100;
    actor.yposi.w.h = 200;
    player.xposi.w.h = 100;
    player.yposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 1, s0_check(&actor, &player));

    player.xposi.w.h = 400;
    TEST_ASSERT_EQ_INT(ctx, 0, s0_check(&actor, &player));

    player.xposi.w.h = 100;
    player.yposi.w.h = -100;
    TEST_ASSERT_EQ_INT(ctx, 0, s0_check(&actor, &player));
}

static void test_bigbom_projectile_wait_zero_and_die_updates_parent(
    test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *body_actor = &actwk[20];
    sprite_status *head_actor = &actwk[21];
    int i;

    init_main_with_body(main_actor, body_actor);
    main_actor->r_no0 = 4;
    queue_actor(head_actor);

    bigbom(main_actor);
    for (i = 0; i < 29; ++i) {
        bigbom(main_actor);
    }

    reset_logs();
    bigbom(head_actor);
    for (i = 0; i < 59; ++i) {
        bigbom(head_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 4, head_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, patchg_count);

    reset_logs();
    main_actor->r_no0 = 8;
    head_actor->r_no0 = 6;

    bigbom(head_actor);

    TEST_ASSERT_EQ_INT(ctx, 10, main_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == head_actor);
}

static void test_bigbom_projectile_wait_move_and_die_paths(test_context *ctx) {
    sprite_status *main_actor = &actwk[3];
    sprite_status *projectile = &actwk[22];

    init_main_with_body(main_actor, &actwk[20]);
    projectile->actno = 35;
    projectile->userflag.b.h = 4;
    projectile->xposi.w.h = 100;
    projectile->yposi.w.h = 169;

    bigbom(projectile);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == projectile);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    projectile->r_no0 = 4;
    set_actfree_word(projectile, 0, -1);
    bigbom(projectile);

    TEST_ASSERT_EQ_INT(ctx, 6, projectile->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    main_actor->r_no0 = 8;
    projectile->r_no0 = 6;
    bigbom(projectile);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == projectile);
}

static void test_bigbom_explosion_animates_then_frames_out(test_context *ctx) {
    sprite_status *explosion = &actwk[23];

    reset_bigbom8_state();
    explosion->actno = 35;
    explosion->userflag.b.h = 6;

    bigbom(explosion);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    explosion->r_no0 = 2;
    set_actfree_word(explosion, 0, 1);
    bigbom(explosion);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == explosion);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_bigbom_fire_moves_and_frames_out_below_player(test_context *ctx) {
    sprite_status *fire = &actwk[24];

    reset_bigbom8_state();
    fire->actno = 35;
    fire->userflag.b.h = 8;
    fire->xposi.l = 100 << 16;
    fire->yposi.l = 100 << 16;
    set_actfree_long(fire, 0, 65536);
    set_actfree_long(fire, 4, -32768);
    set_actfree_long(fire, 8, 8192);
    set_actfree_long(fire, 12, 4096);
    actwk[0].yposi.w.h = 90;

    bigbom(fire);

    TEST_ASSERT_EQ_INT(ctx, 101, fire->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 99, fire->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_logs();
    fire->r_no0 = 2;
    fire->yposi.w.h = 400;
    actwk[0].yposi.w.h = 100;
    bigbom(fire);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == fire);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_bigbom_fire_userflag_two_refreshes_collision(
    test_context *ctx) {
    sprite_status *fire = &actwk[24];

    reset_bigbom8_state();
    fire->actno = 35;
    fire->userflag.b.h = 8;
    fire->userflag.b.l = 2;
    fire->xposi.l = 100 << 16;
    fire->yposi.l = 100 << 16;
    set_actfree_long(fire, 0, 65536);
    set_actfree_long(fire, 4, -32768);
    set_actfree_long(fire, 8, 8192);
    set_actfree_long(fire, 12, 4096);
    fire->actfree[19] = 167;
    actwk[0].yposi.w.h = 90;

    bigbom(fire);

    TEST_ASSERT_EQ_INT(ctx, 167, fire->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_logs();
    prio_flag = 1;
    bigbom(fire);

    TEST_ASSERT_EQ_INT(ctx, 0, fire->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

TEST_MAIN_BEGIN;
    test_bigbom8_patterns_capture_literal_data(&ctx);
    test_bigbom_main_initializes_body_and_frame_origin(&ctx);
    test_bigbom_main_allocation_failure_uses_frameout_s0(&ctx);
    test_bigbom_main_countdown_spawns_head_actor(&ctx);
    test_bigbom_main_head_spawn_allocation_failure_frames_out(&ctx);
    test_bigbom_main_late_sequence_spawns_explosion_and_fire(&ctx);
    test_bigbom_main_empty_move4_dispatch_keeps_state(&ctx);
    test_bigbom_main_late_sequence_frames_out_when_explosion_fails(&ctx);
    test_bigbom_make_fire_allocation_failure_uses_frameout_s0(&ctx);
    test_bigbom_body_falls_then_updates_parent(&ctx);
    test_bigbom_body_fall_for_userflag_two_toggles_direction(&ctx);
    test_bigbom_body_frames_out_when_parent_is_missing(&ctx);
    test_bigbom_body_wait_and_constant_move_paths(&ctx);
    test_bigbom_body_prio_move_stop_and_die_paths(&ctx);
    test_bigbom_body_check_uses_unsigned_bounds(&ctx);
    test_bigbom_projectile_wait_zero_and_die_updates_parent(&ctx);
    test_bigbom_projectile_wait_move_and_die_paths(&ctx);
    test_bigbom_explosion_animates_then_frames_out(&ctx);
    test_bigbom_fire_moves_and_frames_out_below_player(&ctx);
    test_bigbom_fire_userflag_two_refreshes_collision(&ctx);
TEST_MAIN_END

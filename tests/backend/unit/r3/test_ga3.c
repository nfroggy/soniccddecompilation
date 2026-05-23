#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static sprite_pattern ring_pattern0 = {1, {{0, 0, 0, 700}}};
sprite_pattern *ringpat[] = {&ring_pattern0};

static Sint16 enemy_suicide_result;
static int enemy_suicide_count;
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
static int actwkchk_fail;
static int next_free_actor;

Sint16 enemy_suicide(sprite_status *pActwk);
void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint32 actwkchk(sprite_status **ppActwk);

#include "src/r3/ga3.c"

Sint16 enemy_suicide(sprite_status *pActwk) {
    (void)pActwk;
    ++enemy_suicide_count;
    return enemy_suicide_result;
}

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
    if (actwkchk_fail) {
        return -1;
    }

    *ppActwk = &actwk[next_free_actor++];
    return 0;
}

static void reset_ga3_state(void) {
    memset(actwk, 0, sizeof(actwk));
    enemy_suicide_result = 0;
    enemy_suicide_count = 0;
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
    actwkchk_fail = 0;
    next_free_actor = 32;
}

static void run_ga_until_r_no0(test_context *ctx, sprite_status *actor,
                               Uint8 r_no0, int max_frames) {
    int i;

    for (i = 0; i < max_frames && actor->r_no0 != r_no0; ++i) {
        ga(actor);
    }

}

static void run_ga_until_actwkchk(test_context *ctx, sprite_status *actor,
                                  int max_frames) {
    int i;

    for (i = 0; i < max_frames && actwkchk_count == 0; ++i) {
        ga(actor);
    }

    TEST_ASSERT_TRUE(ctx, actwkchk_count > 0);
}

static void test_ga3_tables_capture_literal_sprite_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_b_ga[0] == &spat_0);
    TEST_ASSERT_TRUE(ctx, pat_b_ga[3] == &spat_b3);
    TEST_ASSERT_TRUE(ctx, pat_e_ga[3] == &spat_e3);
    TEST_ASSERT_EQ_INT(ctx, 1, spat_b3.cnt);
    TEST_ASSERT_EQ_INT(ctx, -16, spat_b3.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -10, spat_b3.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 482, spat_b3.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -16, spat_e3.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, spat_e3.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 485, spat_e3.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -12, spat_2.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, spat_2.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 490, spat_2.spra[0].index);
}

static void test_ga_enemy_suicide_short_circuits_main_enemy(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_ga3_state();
    enemy_suicide_result = -1;

    ga(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_ga_main_enemy_initializes_visible_fields(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_ga3_state();
    actor->actflg = 128;

    ga(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, enemy_suicide_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
}

static void test_ga_main_enemy_uses_b_pattern_for_negative_userflag(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_ga3_state();
    actor->userflag.b.h = -128;

    ga(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_ga_main_enemy_progresses_to_attack_and_spawns_ring(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *spawned;

    reset_ga3_state();
    actor->actno = 44;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 80;
    actwk[0].xposi.w.h = 200;

    run_ga_until_r_no0(ctx, actor, 4, 70);

    run_ga_until_r_no0(ctx, actor, 6, 70);
    TEST_ASSERT_TRUE(ctx, patchg_count > 0);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);

    run_ga_until_r_no0(ctx, actor, 8, 70);

    run_ga_until_actwkchk(ctx, actor, 40);
    spawned = &actwk[32];

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
}

static void test_ga_main_enemy_does_not_spawn_when_allocator_fails(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_ga3_state();
    actor->actno = 44;
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 90;
    actwk[0].xposi.w.h = 100;
    actwkchk_fail = 1;

    run_ga_until_r_no0(ctx, actor, 8, 210);
    run_ga_until_actwkchk(ctx, actor, 40);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[32].actno);
}

static void test_ga_negative_main_enemy_runs_without_spawn(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_ga3_state();
    actor->userflag.b.h = -128;
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 70;
    actwk[0].xposi.w.h = 200;

    run_ga_until_r_no0(ctx, actor, 8, 210);
    for (int i = 0; i < 60; ++i) {
        ga(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_ga_spawned_ring_initializes_and_falls(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_ga3_state();
    actor->userflag.b.h = 1;
    actor->yposi.w.h = 100;

    ga(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)ringchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_ga_spawned_ring_times_out_and_dies(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_ga3_state();
    actor->userflag.b.h = 1;

    for (int i = 0; i < 180; ++i) {
        ga(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 180, actionsub_count);
}

static void test_ga_spawned_ring_die_state_frameouts_immediately(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_ga3_state();
    actor->userflag.b.h = 1;
    actor->r_no0 = 4;

    ga(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_ga3_tables_capture_literal_sprite_data(&ctx);
    test_ga_enemy_suicide_short_circuits_main_enemy(&ctx);
    test_ga_main_enemy_initializes_visible_fields(&ctx);
    test_ga_main_enemy_uses_b_pattern_for_negative_userflag(&ctx);
    test_ga_main_enemy_progresses_to_attack_and_spawns_ring(&ctx);
    test_ga_main_enemy_does_not_spawn_when_allocator_fails(&ctx);
    test_ga_negative_main_enemy_runs_without_spawn(&ctx);
    test_ga_spawned_ring_initializes_and_falls(&ctx);
    test_ga_spawned_ring_times_out_and_dies(&ctx);
    test_ga_spawned_ring_die_state_frameouts_immediately(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[2];
static int actwkchk_queue_count;
static int actwkchk_queue_pos;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;

void actionsub(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#include "src/r4/osumizu.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_pos >= actwkchk_queue_count) {
        *ppActwk = 0;
        return 1;
    }

    *ppActwk = actwkchk_queue[actwkchk_queue_pos++];
    return 0;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    actionsub_count = 0;
    actionsub_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_pos = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void test_osumizu_patterns_and_animation_table(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_osumizu[0] == &osumizu_pat0);
    TEST_ASSERT_TRUE(ctx, pat_osumizu1[0] == &osumizu_pat1);
    TEST_ASSERT_TRUE(ctx, pat_osumizu1[3] == &osumizu_pat4);
    TEST_ASSERT_EQ_INT(ctx, 471, osumizu_pat0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -8, osumizu_pat0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -16, osumizu_pat0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 472, osumizu_pat1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 475, osumizu_pat4.spra[0].index);
    TEST_ASSERT_TRUE(ctx, osumizu_chg[0] == osumizu_chg0);
    TEST_ASSERT_EQ_INT(ctx, 9, osumizu_chg0[0]);
    TEST_ASSERT_EQ_INT(ctx, 255, osumizu_chg0[5]);
}

static void test_osumizu_left_generator_initializes_and_spawns_drop(
    test_context *ctx) {
    sprite_status *generator = &actwk[5];
    sprite_status *drop = &actwk[20];

    reset_state();
    generator->actno = 32;
    generator->xposi.w.h = 100;
    generator->yposi.w.h = 200;
    queue_actwkchk(drop);

    osumizu(generator);

    TEST_ASSERT_EQ_INT(ctx, 2, generator->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, generator->actflg & 4);
    TEST_ASSERT_EQ_INT(ctx, 3, generator->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, generator->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 16, generator->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 17176, generator->sproffset);
    TEST_ASSERT_TRUE(ctx, generator->patbase == pat_osumizu);
    TEST_ASSERT_EQ_INT(ctx, -196608, osumizu_work_get(generator)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 132, osumizu_work_get(generator)->spawn_x);
    TEST_ASSERT_EQ_INT(ctx, 30, osumizu_work_get(generator)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 32, drop->actno);
    TEST_ASSERT_EQ_INT(ctx, -1, drop->userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 132, drop->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 204, drop->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -196608, osumizu_work_get(drop)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 4, drop->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, drop->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 32, drop->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 6, drop->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 792, drop->sproffset);
    TEST_ASSERT_TRUE(ctx, drop->patbase == pat_osumizu1);
    TEST_ASSERT_EQ_INT(ctx, 0, drop->cddat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == generator);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_osumizu_right_generator_sets_flip_and_waits(
    test_context *ctx) {
    sprite_status *generator = &actwk[5];

    reset_state();
    generator->xposi.w.h = 100;
    generator->yposi.w.h = 200;
    generator->userflag.b.h = 1;
    osumizu_work_get(generator)->timer = 5;

    osumizu(generator);

    TEST_ASSERT_EQ_INT(ctx, 2, generator->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, generator->actflg & 5);
    TEST_ASSERT_EQ_INT(ctx, 1, generator->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 196608, osumizu_work_get(generator)->x_velocity);
    TEST_ASSERT_EQ_INT(ctx, 68, osumizu_work_get(generator)->spawn_x);
    TEST_ASSERT_EQ_INT(ctx, 4, osumizu_work_get(generator)->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_osumizu_generator_allocation_failure_keeps_timer(
    test_context *ctx) {
    sprite_status *generator = &actwk[5];

    reset_state();
    generator->r_no0 = 2;
    osumizu_work_get(generator)->timer = 0;
    osumizu_work_get(generator)->spawn_x = 100;
    osumizu_work_get(generator)->x_velocity = -196608;

    osumizu(generator);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 30, osumizu_work_get(generator)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_osumizu_drop_pushes_player_when_overlapping(
    test_context *ctx) {
    sprite_status *drop = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    drop->userflag.b.h = -1;
    drop->xposi.l = 100 << 16;
    drop->yposi.w.h = 200;
    osumizu_work_get(drop)->x_velocity = -196608;
    player->xposi.w.h = 100;
    player->yposi.w.h = 200;
    player->mspeed.w = 1234;

    osumizu(drop);

    TEST_ASSERT_EQ_INT(ctx, -768, player->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 97, drop->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == drop);
    TEST_ASSERT_TRUE(ctx, patchg_table == osumizu_chg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_osumizu_drop_ignores_player_outside_collision_box(
    test_context *ctx) {
    sprite_status *drop = &actwk[5];
    sprite_status *player = &actwk[0];

    reset_state();
    drop->userflag.b.h = -1;
    drop->xposi.l = 100 << 16;
    drop->yposi.w.h = 200;
    osumizu_work_get(drop)->x_velocity = 196608;
    player->xposi.w.h = 100;
    player->yposi.w.h = 230;
    player->mspeed.w = 1234;

    osumizu(drop);

    TEST_ASSERT_EQ_INT(ctx, 1234, player->mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 103, drop->xposi.w.h);

    reset_state();
    drop->userflag.b.h = -1;
    drop->xposi.l = 100 << 16;
    drop->yposi.w.h = 200;
    osumizu_work_get(drop)->x_velocity = 196608;
    player->xposi.w.h = 150;
    player->yposi.w.h = 200;
    player->mspeed.w = 4321;

    osumizu(drop);

    TEST_ASSERT_EQ_INT(ctx, 4321, player->mspeed.w);
}

TEST_MAIN_BEGIN;
    test_osumizu_patterns_and_animation_table(&ctx);
    test_osumizu_left_generator_initializes_and_spawns_drop(&ctx);
    test_osumizu_right_generator_sets_flip_and_waits(&ctx);
    test_osumizu_generator_allocation_failure_keeps_timer(&ctx);
    test_osumizu_drop_pushes_player_when_overlapping(&ctx);
    test_osumizu_drop_ignores_player_outside_collision_box(&ctx);
TEST_MAIN_END

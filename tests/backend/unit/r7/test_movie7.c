#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 projector_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int actwkchk_fail_at;
static int next_free_actor;
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[16];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r7/movie7.c"

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
    if (actwkchk_fail_at == actwkchk_count) {
        return -1;
    }

    *ppActwk = &actwk[next_free_actor++];
    return 0;
}

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++hitchk_count;
    hitchk_actor = pActwk;
    hitchk_player = pPlayerwk;
    return hitchk_result;
}

Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk) {
    ++ride_on_clr_count;
    ride_on_clr_actor = pActwk;
    ride_on_clr_player = pPlayerwk;
    return 0;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void reset_movie7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    projector_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    actwkchk_fail_at = 0;
    next_free_actor = 32;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_movie7_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void init_projector(sprite_status *actor) {
    reset_movie7_state();
    actor->actno = 39;
    actor->xposi.w.h = 400;
    actor->yposi.w.h = 200;
    movie(actor);
    reset_movie7_logs();
}

static void test_movie7_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, movie1_pchg[0] == movie1_pchg00);
    TEST_ASSERT_TRUE(ctx, movie1_pchg[1] == movie1_pchg01);
    TEST_ASSERT_EQ_INT(ctx, 0, movie1_pchg00[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, movie1_pchg00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, movie1_pchg00[2]);
    TEST_ASSERT_EQ_INT(ctx, 3, movie1_pchg00[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, movie1_pchg00[4]);
    TEST_ASSERT_EQ_INT(ctx, 255, movie1_pchg00[5]);

    TEST_ASSERT_TRUE(ctx, pat_movie[0] == &spr_movie1);
    TEST_ASSERT_TRUE(ctx, pat_movie[1] == &spr_movie0);
    TEST_ASSERT_TRUE(ctx, pat_movie[5] == &spr_movie5);
    TEST_ASSERT_EQ_INT(ctx, -12, spr_movie1.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -12, spr_movie1.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 462, spr_movie1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 0, spr_movie0.cnt);
    TEST_ASSERT_EQ_INT(ctx, -20, spr_movie0.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, spr_movie0.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 466, spr_movie0.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, -20, spr_movie5.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, -24, spr_movie5.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 466, spr_movie5.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1, tbl0[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, tbl0[36]);
}

static void test_movie_projector_flag_frameouts_main_actor(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_movie7_state();
    projector_flag = 1;

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_movie_initializes_and_spawns_four_children(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_movie7_state();
    actor->actno = 39;
    actor->xposi.w.h = 400;
    actor->yposi.w.h = 200;

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 251, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1199, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_movie);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    TEST_ASSERT_EQ_INT(ctx, 39, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 379, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 193, actwk[32].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[32].userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 39, actwk[33].actno);
    TEST_ASSERT_EQ_INT(ctx, 312, actwk[33].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 196, actwk[33].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[33].userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 40, actwk[34].actno);
    TEST_ASSERT_EQ_INT(ctx, 312, actwk[34].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 176, actwk[34].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[34].userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 40, actwk[35].actno);
    TEST_ASSERT_EQ_INT(ctx, 302, actwk[35].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[35].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -127, actwk[35].userflag.b.h);
}

static void test_movie_allocation_failures_frameout_at_each_child(
    test_context *ctx) {
    for (int fail_at = 1; fail_at <= 4; ++fail_at) {
        sprite_status *actor = &actwk[2];

        reset_movie7_state();
        actor->actno = 39;
        actor->xposi.w.h = 400;
        actor->yposi.w.h = 200;
        actwkchk_fail_at = fail_at;

        movie(actor);

        TEST_ASSERT_EQ_INT(ctx, fail_at, actwkchk_count);
        TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
        TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
        TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
        TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    }
}

static void test_movie_wait_without_collision_calls_hitchk(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_projector(actor);

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 251, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_movie_collision_enters_destruction_and_clears_ride(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_projector(actor);
    actor->colicnt = 3;
    hitchk_result = 1;

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);

    reset_movie7_logs();
    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 255, actor->actfree[21]);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_movie7_logs();
    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[36].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[36].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[36].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[36].yposi.w.h);
}

static void test_movie_destruction_skips_ride_clear_when_not_hitched(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_projector(actor);
    actor->colicnt = 1;
    movie(actor);
    reset_movie7_logs();
    hitchk_result = 0;

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_movie_explosion_allocator_failure_skips_spawn(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_projector(actor);
    actor->r_no0 = 6;
    actwkchk_count = 0;
    actwkchk_fail_at = 1;

    movie(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[36].actno);
}

static void test_movie_explosion_sequence_finishes_and_sets_projector_flag(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    init_projector(actor);
    actor->r_no0 = 4;
    movie(actor);

    for (int i = 0; i < 280 && actor->r_no0 != 8; ++i) {
        movie(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 12, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[47].actno);
    TEST_ASSERT_EQ_INT(ctx, 390, actwk[47].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 210, actwk[47].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    for (int i = 0; i < 60; ++i) {
        movie(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 255, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
}

static void test_movie_child_frameouts_when_parent_missing_or_destroying(
    test_context *ctx) {
    sprite_status *child = &actwk[5];
    sprite_status *parent = &actwk[2];

    reset_movie7_state();
    child->userflag.b.h = -1;
    ((Uint16 *)child)[33] = 2;
    parent->actno = 0;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_movie7_state();
    child = &actwk[5];
    parent = &actwk[2];
    child->userflag.b.h = 1;
    ((Uint16 *)child)[33] = 2;
    parent->actno = 39;
    parent->actfree[21] = 255;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_movie_child_initializes_negative_and_positive_shapes(
    test_context *ctx) {
    sprite_status *child = &actwk[5];
    sprite_status *parent = &actwk[2];

    reset_movie7_state();
    child->userflag.b.h = -1;
    ((Uint16 *)child)[33] = 2;
    parent->actno = 39;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 1199, child->sproffset);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_movie);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, child->mstno.b.h);

    reset_movie7_state();
    child = &actwk[5];
    parent = &actwk[2];
    child->userflag.b.h = 1;
    ((Uint16 *)child)[33] = 2;
    parent->actno = 39;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, child->mstno.b.h);
}

static void test_movie_child_moves_with_patch_animation(test_context *ctx) {
    sprite_status *child = &actwk[5];
    sprite_status *parent = &actwk[2];

    reset_movie7_state();
    child->userflag.b.h = 1;
    child->r_no0 = 2;
    ((Uint16 *)child)[33] = 2;
    parent->actno = 39;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_TRUE(ctx, patchg_table == movie1_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

TEST_MAIN_BEGIN;
    test_movie7_tables_capture_literal_data(&ctx);
    test_movie_projector_flag_frameouts_main_actor(&ctx);
    test_movie_initializes_and_spawns_four_children(&ctx);
    test_movie_allocation_failures_frameout_at_each_child(&ctx);
    test_movie_wait_without_collision_calls_hitchk(&ctx);
    test_movie_collision_enters_destruction_and_clears_ride(&ctx);
    test_movie_destruction_skips_ride_clear_when_not_hitched(&ctx);
    test_movie_explosion_allocator_failure_skips_spawn(&ctx);
    test_movie_explosion_sequence_finishes_and_sets_projector_flag(&ctx);
    test_movie_child_frameouts_when_parent_missing_or_destroying(&ctx);
    test_movie_child_initializes_negative_and_positive_shapes(&ctx);
    test_movie_child_moves_with_patch_animation(&ctx);
TEST_MAIN_END

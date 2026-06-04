#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 projector_flag;
short_union stageno;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static Sint32 frameout_s_result;
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
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
static Sint16 soundset_requests[8];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);

#include "src/r1/movie1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return frameout_s_result;
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
    if (soundset_count < 8) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_movie1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    projector_flag = 0;
    memset(&stageno, 0, sizeof(stageno));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_s_result = 0;
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
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

static void test_movie1_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, movie1_pchg[0] == movie1_pchg00);
    TEST_ASSERT_TRUE(ctx, movie1_pchg[1] == movie1_pchg01);
    TEST_ASSERT_TRUE(ctx, pat_movie[0] == &spr_movie1);
    TEST_ASSERT_TRUE(ctx, pat_movie[5] == &spr_movie5);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_MOVIE_BASE, spr_movie1.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_MOVIE_BASE + 4, spr_movie5.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 1, tbl0[0]);
    TEST_ASSERT_EQ_INT(ctx, -18, tbl0[4]);
    TEST_ASSERT_EQ_INT(ctx, -1, tbl0[36]);
}

static void test_mm_init_projector_and_allocation_paths(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    projector_flag = 1;
    mm_init(main);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 0, main->r_no0);

    reset_movie1_state();
    main->actno = 45;
    main->xposi.w.h = 400;
    main->yposi.w.h = 200;
    stageno.b.l = 1;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);

    mm_init(main);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 2, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, main->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, main->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, main->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 12, main->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, main->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 251, main->colino);
    TEST_ASSERT_EQ_INT(ctx, 9135, main->sproffset);
    TEST_ASSERT_TRUE(ctx, main->patbase == pat_movie);
    TEST_ASSERT_TRUE(ctx, movie1_work_get(main)->explosion_script == tbl0);
}

static void test_mm_init_spawns_four_children(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    main->actno = 45;
    main->xposi.w.h = 400;
    main->yposi.w.h = 200;
    for (int i = 0; i < 4; ++i) {
        queue_actor(&actwk[40 + i]);
    }

    mm_init(main);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1027, main->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 379, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 193, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1, actwk[40].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 3, movie1_work_get(&actwk[40])->parent_actor);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[41].actno);
    TEST_ASSERT_EQ_INT(ctx, 312, actwk[41].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 196, actwk[41].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[41].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[42].actno);
    TEST_ASSERT_EQ_INT(ctx, 312, actwk[42].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 176, actwk[42].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[42].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[43].actno);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[43].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 204, actwk[43].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -127, actwk[43].userflag.b.h);
}

static void test_mm_init_late_allocation_failures_frameout(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    main->actno = 45;
    main->xposi.w.h = 400;
    main->yposi.w.h = 200;
    queue_actor(&actwk[40]);

    mm_init(main);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[40].actno);

    reset_movie1_state();
    main->actno = 45;
    main->xposi.w.h = 400;
    main->yposi.w.h = 200;
    queue_actor(&actwk[40]);
    queue_actor(&actwk[41]);
    queue_actor(&actwk[42]);

    mm_init(main);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
    TEST_ASSERT_EQ_INT(ctx, 36, actwk[42].actno);
}

static void test_mm_wait_and_die_paths(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    main->r_no0 = 2;
    main->colino = 251;
    main->colicnt = 1;
    mm_wait(main);
    TEST_ASSERT_EQ_INT(ctx, 4, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, main->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, main->colicnt);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == main);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);

    reset_movie1_state();
    main->r_no0 = 4;
    hitchk_result = 1;
    mm_die(main);
    TEST_ASSERT_EQ_INT(ctx, 6, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, main->patno);
    TEST_ASSERT_EQ_INT(ctx, 255, movie1_work_get(main)->parent_destroyed);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == main);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);

    reset_movie1_state();
    main->r_no0 = 4;
    mm_die(main);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_m_baku_waits_spawns_and_finishes_script(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    main->xposi.w.h = 400;
    main->yposi.w.h = 200;
    movie1_work_get(main)->explosion_script = tbl0;
    queue_actor(&actwk[40]);

    m_baku(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[40].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_TRUE(ctx, movie1_work_get(main)->explosion_script == &tbl0[3]);

    m_baku(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_TRUE(ctx, movie1_work_get(main)->explosion_script == &tbl0[3]);

    reset_movie1_state();
    main->r_no0 = 6;
    movie1_work_get(main)->explosion_script = &tbl0[36];
    m_baku(main);
    TEST_ASSERT_EQ_INT(ctx, 8, main->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, movie1_work_get(main)->explosion_timer);
}

static void test_m_baku_allocation_failure_after_script_time(
    test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    movie1_work_get(main)->explosion_script = tbl0;
    m_baku(main);
    m_baku(main);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_mm1wait_counts_down_and_sets_projector(test_context *ctx) {
    sprite_status *main = &actwk[3];

    reset_movie1_state();
    movie1_work_get(main)->explosion_timer = 2;
    mm1wait(main);
    TEST_ASSERT_EQ_INT(ctx, 1, movie1_work_get(main)->explosion_timer);
    TEST_ASSERT_EQ_INT(ctx, 0, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    mm1wait(main);
    TEST_ASSERT_EQ_INT(ctx, 255, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == main);
}

static void test_sub_parent_gates_and_initializes_variants(test_context *ctx) {
    sprite_status *child = &actwk[40];
    sprite_status *parent = &actwk[3];

    reset_movie1_state();
    movie1_work_get(child)->parent_actor = 3;
    parent->actno = 0;
    sub(child);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_movie1_state();
    movie1_work_get(child)->parent_actor = 3;
    parent->actno = 46;
    movie1_work_get(parent)->parent_destroyed = 1;
    sub(child);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_movie1_state();
    movie1_work_get(child)->parent_actor = 3;
    parent->actno = 46;
    child->userflag.b.h = -1;
    stageno.b.l = 1;
    sub(child);
    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 9135, child->sproffset);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_movie);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, child->mstno.b.h);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);

    reset_movie1_state();
    movie1_work_get(child)->parent_actor = 3;
    parent->actno = 46;
    child->userflag.b.h = 1;
    sub(child);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, child->mstno.b.h);
}

static void test_movie1_wrapper_paths(test_context *ctx) {
    sprite_status *main = &actwk[3];
    sprite_status *child = &actwk[40];

    reset_movie1_state();
    projector_flag = 1;
    main->userflag.b.h = 0;
    movie1(main);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_movie1_state();
    frameout_s_result = 7;
    movie1(main);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_movie1_state();
    child->userflag.b.h = 1;
    movie1_work_get(child)->parent_actor = 3;
    actwk[3].actno = 46;
    child->r_no0 = 2;
    movie1(child);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

TEST_MAIN_BEGIN;
    test_movie1_tables_capture_literal_data(&ctx);
    test_mm_init_projector_and_allocation_paths(&ctx);
    test_mm_init_spawns_four_children(&ctx);
    test_mm_init_late_allocation_failures_frameout(&ctx);
    test_mm_wait_and_die_paths(&ctx);
    test_m_baku_waits_spawns_and_finishes_script(&ctx);
    test_m_baku_allocation_failure_after_script_time(&ctx);
    test_mm1wait_counts_down_and_sets_projector(&ctx);
    test_sub_parent_gates_and_initializes_variants(&ctx);
    test_movie1_wrapper_paths(&ctx);
TEST_MAIN_END

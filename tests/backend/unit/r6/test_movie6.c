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
static sprite_status *actwkchk_queue[128];
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
static Uint8 **patchg_pattern;
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

#include "src/r6/movie6.c"

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
    patchg_pattern = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_movie6_state(void) {
    memset(actwk, 0, sizeof(actwk));
    projector_flag = 0;
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
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_pattern = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void init_movie_with_children(sprite_status *movie, sprite_status *a,
                                     sprite_status *b, sprite_status *c,
                                     sprite_status *d) {
    queue_actor(a);
    queue_actor(b);
    queue_actor(c);
    queue_actor(d);
    m_init(movie);
}

static void test_movie6_init_creates_projector_children(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child0 = &actwk[20];
    sprite_status *child1 = &actwk[21];
    sprite_status *spark0 = &actwk[22];
    sprite_status *spark1 = &actwk[23];

    reset_movie6_state();
    movie_actor->actno = 47;
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;

    init_movie_with_children(movie_actor, child0, child1, spark0, spark1);

    TEST_ASSERT_EQ_INT(ctx, 2, movie_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, movie_actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, movie_actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 12, movie_actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 12, movie_actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 12, movie_actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 251, movie_actor->colino);
    TEST_ASSERT_TRUE(ctx, movie_actor->patbase == pat_movie);
    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);

    TEST_ASSERT_EQ_INT(ctx, 47, child0->actno);
    TEST_ASSERT_EQ_INT(ctx, 279, child0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 393, child0->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1, child0->userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 47, child1->actno);
    TEST_ASSERT_EQ_INT(ctx, 228, child1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 396, child1->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, child1->userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 41, spark0->actno);
    TEST_ASSERT_EQ_INT(ctx, 228, spark0->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 376, spark0->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -128, spark0->userflag.b.h);

    TEST_ASSERT_EQ_INT(ctx, 41, spark1->actno);
    TEST_ASSERT_EQ_INT(ctx, 216, spark1->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 407, spark1->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -127, spark1->userflag.b.h);
}

static void test_movie6_init_frames_out_when_projector_already_done(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    projector_flag = 1;

    m_init(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_actor->r_no0);
}

static void test_movie6_init_frames_out_on_first_child_allocation_failure(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();

    m_init(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);
}

static void test_movie6_init_frames_out_on_child_allocation_failure(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child0 = &actwk[20];

    reset_movie6_state();
    queue_actor(child0);

    m_init(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);
}

static void test_movie6_init_frames_out_on_later_child_allocation_failures(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    queue_actor(&actwk[20]);
    queue_actor(&actwk[21]);

    m_init(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);

    reset_movie6_state();
    movie_actor = &actwk[10];
    queue_actor(&actwk[20]);
    queue_actor(&actwk[21]);
    queue_actor(&actwk[22]);

    m_init(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);
}

static void test_movie6_wait_arms_explosion_on_collision(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    movie_actor->r_no0 = 2;
    movie_actor->colino = 251;
    movie_actor->colicnt = 7;
    hitchk_result = 1;

    m_wait(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, movie_actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, movie_actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 4, movie_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == movie_actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
}

static void test_movie6_wait_uses_hitchk_while_idle(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();

    m_wait(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == movie_actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
}

static void test_movie6_die_clears_ride_and_starts_bursts(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *burst = &actwk[20];

    reset_movie6_state();
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    init_movie_with_children(movie_actor, &actwk[21], &actwk[22], &actwk[23],
                             &actwk[24]);
    movie_actor->r_no0 = 4;
    hitchk_result = 1;
    queue_actor(burst);

    m_die(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 6, movie_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, movie_actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 255, movie6_get_work(movie_actor)->destroyed_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == movie_actor);
    TEST_ASSERT_EQ_INT(ctx, 24, burst->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, burst->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 300, burst->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, burst->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
}

static void test_movie6_baku_waits_allocates_and_reaches_final_wait(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *burst = &actwk[20];

    reset_movie6_state();
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    init_movie_with_children(movie_actor, &actwk[21], &actwk[22], &actwk[23],
                             &actwk[24]);
    movie_actor->r_no0 = 6;

    queue_actor(burst);
    m_baku(movie_actor);
    TEST_ASSERT_EQ_INT(ctx, 24, burst->actno);
    TEST_ASSERT_EQ_INT(ctx, 300, burst->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, burst->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);

    for (int i = 0; i < 80 && movie_actor->r_no0 == 6; ++i) {
        queue_actor(&actwk[30 + (i % 20)]);
        m_baku(movie_actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 8, movie_actor->r_no0);
}

static void test_movie6_baku_returns_when_burst_allocation_fails(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    init_movie_with_children(movie_actor, &actwk[21], &actwk[22], &actwk[23],
                             &actwk[24]);
    movie_actor->r_no0 = 6;
    actwkchk_count = 0;
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;

    m_baku(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 6, movie_actor->r_no0);
}

static void test_movie6_final_wait_sets_projector_flag_and_dies(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    movie_actor->r_no0 = 8;
    movie6_get_work(movie_actor)->wait_timer = 1;

    m1wait(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 255, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == movie_actor);
}

static void test_movie6_sub_initializes_negative_child_from_live_parent(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child = &actwk[20];

    reset_movie6_state();
    movie_actor->actno = 47;
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    init_movie_with_children(movie_actor, child, &actwk[21], &actwk[22],
                             &actwk[23]);

    sub(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, child->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprpri);
    TEST_ASSERT_TRUE(ctx, child->patbase == pat_movie);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 8, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 4, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 0, child->mstno.b.h);
}

static void test_movie6_sub_initializes_positive_child(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child = &actwk[21];

    reset_movie6_state();
    movie_actor->actno = 47;
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    init_movie_with_children(movie_actor, &actwk[20], child, &actwk[22],
                             &actwk[23]);

    sub(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 20, child->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, child->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, child->mstno.b.h);
}

static void test_movie6_sub_moves_initialized_child(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child = &actwk[20];

    reset_movie6_state();
    movie_actor->actno = 47;
    init_movie_with_children(movie_actor, child, &actwk[21], &actwk[22],
                             &actwk[23]);
    sub(child);

    patchg_count = actionsub_count = 0;
    sub(child);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == child);
    TEST_ASSERT_TRUE(ctx, patchg_pattern == pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == child);
}

static void test_movie6_sub_frames_out_when_parent_missing_or_dying(
    test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child = &actwk[20];

    reset_movie6_state();
    movie_actor->actno = 47;
    init_movie_with_children(movie_actor, child, &actwk[21], &actwk[22],
                             &actwk[23]);
    movie_actor->actno = 0;

    sub(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);

    reset_movie6_state();
    movie_actor = &actwk[10];
    child = &actwk[20];
    movie_actor->actno = 47;
    init_movie_with_children(movie_actor, child, &actwk[21], &actwk[22],
                             &actwk[23]);
    movie6_get_work(movie_actor)->destroyed_flag = 1;

    sub(child);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == child);
}

static void test_movie6_entry_dispatches_parent_callbacks(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];

    reset_movie6_state();
    movie_actor->actno = 47;
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    queue_actor(&actwk[20]);
    queue_actor(&actwk[21]);
    queue_actor(&actwk[22]);
    queue_actor(&actwk[23]);

    movie(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, movie_actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == movie_actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == movie_actor);
}

static void test_movie6_entry_skips_frameout_after_die_state(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *burst = &actwk[20];

    reset_movie6_state();
    movie_actor->xposi.w.h = 300;
    movie_actor->yposi.w.h = 400;
    init_movie_with_children(movie_actor, &actwk[21], &actwk[22], &actwk[23],
                             &actwk[24]);
    movie_actor->r_no0 = 6;
    queue_actor(burst);

    movie(movie_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_movie6_entry_dispatches_child_actor(test_context *ctx) {
    sprite_status *movie_actor = &actwk[10];
    sprite_status *child = &actwk[20];

    reset_movie6_state();
    movie_actor->actno = 47;
    init_movie_with_children(movie_actor, child, &actwk[21], &actwk[22],
                             &actwk[23]);
    actionsub_count = 0;
    frameout_s_count = 0;

    movie(child);

    TEST_ASSERT_EQ_INT(ctx, 2, child->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

TEST_MAIN_BEGIN;
    test_movie6_init_creates_projector_children(&ctx);
    test_movie6_init_frames_out_when_projector_already_done(&ctx);
    test_movie6_init_frames_out_on_first_child_allocation_failure(&ctx);
    test_movie6_init_frames_out_on_child_allocation_failure(&ctx);
    test_movie6_init_frames_out_on_later_child_allocation_failures(&ctx);
    test_movie6_wait_arms_explosion_on_collision(&ctx);
    test_movie6_wait_uses_hitchk_while_idle(&ctx);
    test_movie6_die_clears_ride_and_starts_bursts(&ctx);
    test_movie6_baku_waits_allocates_and_reaches_final_wait(&ctx);
    test_movie6_baku_returns_when_burst_allocation_fails(&ctx);
    test_movie6_final_wait_sets_projector_flag_and_dies(&ctx);
    test_movie6_sub_initializes_negative_child_from_live_parent(&ctx);
    test_movie6_sub_initializes_positive_child(&ctx);
    test_movie6_sub_moves_initialized_child(&ctx);
    test_movie6_sub_frames_out_when_parent_missing_or_dying(&ctx);
    test_movie6_entry_dispatches_parent_callbacks(&ctx);
    test_movie6_entry_skips_frameout_after_die_state(&ctx);
    test_movie6_entry_dispatches_child_actor(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 generate_flag;
Uint8 time_flag;
short_union stageno;
sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int scoreup_count;
static Uint32 scoreup_values[8];
static int hitchk_count;
static sprite_status *hitchk_actor;
static sprite_status *hitchk_player;
static Sint16 hitchk_result;
static int ride_on_clr_count;
static sprite_status *ride_on_clr_actor;
static sprite_status *ride_on_clr_player;
static int actwkchk_count;
static sprite_status *actwkchk_queue[64];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int soundset_count;
static Sint16 soundset_requests[16];

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void scoreup(Uint32 updata);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk(sprite_status **ppActwk);
void soundset(Sint16 ReqNo);

#include "src/r7/et7.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
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

void scoreup(Uint32 updata) {
    if (scoreup_count < 8) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
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

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_queue_index >= actwkchk_queue_count) {
        *ppActwk = 0;
        return -1;
    }
    *ppActwk = actwkchk_queue[actwkchk_queue_index++];
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

static void queue_actwkchk(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_logs(void) {
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    hitchk_count = 0;
    hitchk_actor = 0;
    hitchk_player = 0;
    hitchk_result = 0;
    ride_on_clr_count = 0;
    ride_on_clr_actor = 0;
    ride_on_clr_player = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
}

static void reset_et7_state(void) {
    generate_flag = 0;
    time_flag = 0;
    memset(&stageno, 0, sizeof(stageno));
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void enter_die_state(sprite_status *actor) {
    reset_et7_state();
    actor->xposi.w.h = 1000;
    actor->yposi.w.h = 400;
    actor->colicnt = 1;
    et(actor);
    reset_logs();
}

static void test_et7_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_et[0] == &et_pat00);
    TEST_ASSERT_TRUE(ctx, pat_et[7] == &et_pat07);
    TEST_ASSERT_EQ_INT(ctx, 1, et_pat00.cnt);
    TEST_ASSERT_EQ_INT(ctx, -40, et_pat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, 16, et_pat00.spra[0].yoff);
    TEST_ASSERT_EQ_INT(ctx, 455, et_pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 459, et_pat04.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 461, et_pat06.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 3, p_a[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, p_a[5]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == p_a);
    TEST_ASSERT_EQ_INT(ctx, 1, tbl0[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, tbl0[63]);
}

static void test_et7_initializes_present_generator_and_waits(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_et7_initializes_inactive_generator_states(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    generate_flag = 1;
    actor->yposi.w.h = 200;
    et(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_et7_state();
    time_flag = 1;
    actor->yposi.w.h = 200;
    et(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_et7_state();
    stageno.b.l = 2;
    time_flag = 3;
    et(actor);
}

static void test_et7_hover_moves_and_reverses_on_exact_ticks(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->yposi.w.h = 200;
    et(actor);
    reset_logs();

    for (int i = 0; i < 27; ++i) {
        a_hover(actor);
    }

    for (int i = 0; i < 8; ++i) {
        a_hover(actor);
    }
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_et7_collision_scores_and_clears_player_ride(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->colicnt = 1;
    hitchk_result = 1;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 150, scoreup_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);
}

static void test_et7_collision_skips_ride_clear_when_not_hitched(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->yposi.w.h = 200;
    actor->colicnt = 1;
    hitchk_result = 0;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, ride_on_clr_count);
}

static void test_et7_die_allocates_explosion_at_scripted_offset(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];
    sprite_status *explosion = &actwk[20];

    enter_die_state(actor);
    queue_actwkchk(explosion);

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_et7_die_advances_even_when_actor_pool_is_full(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    enter_die_state(actor);

    et(actor);
    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
}

static void test_et7_die_waits_when_script_time_has_not_arrived(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->yposi.w.h = 200;
    et(actor);
    actor->r_no0 = 4;
    reset_logs();

    m_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_et7_die_enters_recovery_after_script_ends(test_context *ctx) {
    sprite_status *actor = &actwk[2];

    enter_die_state(actor);

    for (int i = 0; i < 41; ++i) {
        m_die(actor);
    }

    TEST_ASSERT_EQ_INT(ctx, 21, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
}

static void test_et7_recovery_waits_then_restores_original_height(
    test_context *ctx) {
    sprite_status *actor = &actwk[2];

    reset_et7_state();
    actor->yposi.w.h = 200;
    time_flag = 1;
    et(actor);
    actor->r_no0 = 6;
    et7_work_get(actor)->delay_timer = 2;
    actor->yposi.w.h = 184;
    reset_logs();

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_s_count);

    reset_logs();
    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 217, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

TEST_MAIN_BEGIN;
test_et7_tables_capture_literal_data(&ctx);
test_et7_initializes_present_generator_and_waits(&ctx);
test_et7_initializes_inactive_generator_states(&ctx);
test_et7_hover_moves_and_reverses_on_exact_ticks(&ctx);
test_et7_collision_scores_and_clears_player_ride(&ctx);
test_et7_collision_skips_ride_clear_when_not_hitched(&ctx);
test_et7_die_allocates_explosion_at_scripted_offset(&ctx);
test_et7_die_advances_even_when_actor_pool_is_full(&ctx);
test_et7_die_waits_when_script_time_has_not_arrived(&ctx);
test_et7_die_enters_recovery_after_script_ends(&ctx);
test_et7_recovery_waits_then_restores_original_height(&ctx);
TEST_MAIN_END

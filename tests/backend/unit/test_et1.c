#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
short_union stageno;
Uint8 time_flag;
Uint8 generate_flag;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
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
static int scoreup_count;
static Uint32 scoreup_value;
static int actwkchk_count;
static sprite_status *actwkchk_queue[8];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int baku_init_count;
static sprite_status *baku_init_actor;
static int soundset_count;
static Sint16 soundset_last;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint16 ride_on_clr(sprite_status *pActwk, sprite_status *pPlayerwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void scoreup(Uint32 updata);
Sint32 actwkchk(sprite_status **ppActwk);
void baku_init(sprite_status *bakuhawk);
void soundset(Sint16 ReqNo);

#include "src/et1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
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

void scoreup(Uint32 updata) {
    ++scoreup_count;
    scoreup_value = updata;
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

void baku_init(sprite_status *bakuhawk) {
    ++baku_init_count;
    baku_init_actor = bakuhawk;
}

void soundset(Sint16 ReqNo) {
    ++soundset_count;
    soundset_last = ReqNo;
}

static void reset_et1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(&stageno, 0, sizeof(stageno));
    time_flag = 0;
    generate_flag = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
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
    scoreup_count = 0;
    scoreup_value = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    baku_init_count = 0;
    baku_init_actor = 0;
    soundset_count = 0;
    soundset_last = 0;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void assert_public_callbacks(test_context *ctx, sprite_status *actor,
                                    int frameout_expected) {
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, frameout_expected, frameout_s_count);
    if (frameout_expected != 0)
        TEST_ASSERT_TRUE(ctx, frameout_s_actor == actor);
}

static void test_et1_tables_capture_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pat_et[0] == &tpat00);
    TEST_ASSERT_TRUE(ctx, pat_et[7] == &tpat07);
    TEST_ASSERT_EQ_INT(ctx, -40, tpat00.spra[0].xoff);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_ET1_BASE, tpat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 4, tpat06.cnt);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_ET1_BASE + 15, tpat06.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_ET1_BASE + 8, tpat06.spra[3].index);
    TEST_ASSERT_EQ_INT(ctx, 1087, tbl0sproffset[0]);
    TEST_ASSERT_EQ_INT(ctx, 1108, tbl0sproffset[4]);
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg0);
    TEST_ASSERT_EQ_INT(ctx, 3, pchg0[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, pchg0[5]);
}

static void test_et1_entry_initializes_default_and_waits(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 34, actor->sprhs);
    TEST_ASSERT_EQ_INT(ctx, 34, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1108, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == pat_et);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 250, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 184, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5, et1_work_get(actor)->hover_counter);
    TEST_ASSERT_EQ_INT(ctx, 0, et1_work_get(actor)->explosion_table_index);
    TEST_ASSERT_EQ_INT(ctx, 200, et1_work_get(actor)->base_y);
    TEST_ASSERT_EQ_INT(ctx, 1, et1_work_get(actor)->hover_direction);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actor == actor);
    TEST_ASSERT_TRUE(ctx, hitchk_player == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == actor);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    assert_public_callbacks(ctx, actor, 1);
}

static void test_et1_init_variants_capture_pattern_and_wait_gates(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    generate_flag = 1;
    time_flag = 1;
    stageno.w = 0;
    actor->yposi.w.h = 300;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 300, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1024, actor->sproffset);
    TEST_ASSERT_EQ_INT(ctx, 0, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    assert_public_callbacks(ctx, actor, 1);

    reset_et1_state();
    actor = &actwk[4];
    generate_flag = 0;
    time_flag = 1;
    actor->yposi.w.h = 320;
    et(actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 320, actor->yposi.w.h);

    reset_et1_state();
    actor = &actwk[4];
    generate_flag = 1;
    time_flag = 0;
    actor->yposi.w.h = 330;
    et(actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 330, actor->yposi.w.h);
}

static void test_m_wait_collision_scores_and_clears_ride(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    actor->r_no0 = 2;
    actor->colino = 250;
    actor->colicnt = 5;
    actor->patno = 3;
    et1_work_get(actor)->hover_counter = 9;
    hitchk_result = 1;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 0, et1_work_get(actor)->hover_counter);
    TEST_ASSERT_EQ_INT(ctx, 7, actor->patno);
    TEST_ASSERT_EQ_INT(ctx, 1, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 150, scoreup_value);
    TEST_ASSERT_EQ_INT(ctx, 1, hitchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ride_on_clr_count);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_actor == actor);
    TEST_ASSERT_TRUE(ctx, ride_on_clr_player == &actwk[0]);
    assert_public_callbacks(ctx, actor, 0);
}

static void test_m_die_waits_for_script_tick_then_spawns_fragment(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    sprite_status *fragment = &actwk[5];

    reset_et1_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    et1_work_get(actor)->delay_timer = 1;

    m_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, et1_work_get(actor)->delay_timer);
    TEST_ASSERT_EQ_INT(ctx, 0, et1_work_get(actor)->explosion_table_index);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    et1_work_get(actor)->delay_timer = 0;
    queue_actor(fragment);

    m_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 3, et1_work_get(actor)->explosion_table_index);
    TEST_ASSERT_EQ_INT(ctx, 24, fragment->actno);
    TEST_ASSERT_EQ_INT(ctx, 1, fragment->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 100, fragment->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, fragment->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, baku_init_count);
    TEST_ASSERT_TRUE(ctx, baku_init_actor == fragment);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_last);
}

static void test_m_die_handles_allocation_failure_and_script_end(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    et1_work_get(actor)->delay_timer = 0;

    m_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 3, et1_work_get(actor)->explosion_table_index);
    TEST_ASSERT_EQ_INT(ctx, 0, baku_init_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_et1_state();
    actor = &actwk[4];
    actor->r_no0 = 4;
    et1_work_get(actor)->explosion_table_index = 63;

    m_die(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, et1_work_get(actor)->delay_timer);
}

static void test_m1wait_counts_down_then_restores_origin(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    actor->r_no0 = 6;
    et1_work_get(actor)->delay_timer = 2;
    actor->yposi.w.h = 77;
    et1_work_get(actor)->base_y = 123;

    m1wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, et1_work_get(actor)->delay_timer);
    TEST_ASSERT_EQ_INT(ctx, 77, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    m1wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, et1_work_get(actor)->delay_timer);
    TEST_ASSERT_EQ_INT(ctx, 123, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 217, soundset_last);
}

static void test_et1_entry_dispatches_die_and_reset_states(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    actor->r_no0 = 4;
    et1_work_get(actor)->explosion_table_index = 63;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, et1_work_get(actor)->delay_timer);
    assert_public_callbacks(ctx, actor, 0);

    reset_et1_state();
    actor = &actwk[4];
    actor->r_no0 = 6;
    et1_work_get(actor)->delay_timer = 1;
    actor->yposi.w.h = 77;
    et1_work_get(actor)->base_y = 123;

    et(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 123, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 217, soundset_last);
    assert_public_callbacks(ctx, actor, 1);
}

static void test_a_hover_moves_every_eight_ticks_and_flips_every_thirty_two(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_et1_state();
    actor->yposi.w.h = 100;
    et1_work_get(actor)->hover_counter = 7;
    et1_work_get(actor)->hover_direction = 2;

    a_hover(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, et1_work_get(actor)->hover_counter);
    TEST_ASSERT_EQ_INT(ctx, 102, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, et1_work_get(actor)->hover_direction);

    et1_work_get(actor)->hover_counter = 31;
    et1_work_get(actor)->hover_direction = 3;

    a_hover(actor);

    TEST_ASSERT_EQ_INT(ctx, 32, et1_work_get(actor)->hover_counter);
    TEST_ASSERT_EQ_INT(ctx, 105, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -3, et1_work_get(actor)->hover_direction);
}

TEST_MAIN_BEGIN;
    test_et1_tables_capture_literal_data(&ctx);
    test_et1_entry_initializes_default_and_waits(&ctx);
    test_et1_init_variants_capture_pattern_and_wait_gates(&ctx);
    test_m_wait_collision_scores_and_clears_ride(&ctx);
    test_m_die_waits_for_script_tick_then_spawns_fragment(&ctx);
    test_m_die_handles_allocation_failure_and_script_end(&ctx);
    test_m1wait_counts_down_then_restores_origin(&ctx);
    test_et1_entry_dispatches_die_and_reset_states(&ctx);
    test_a_hover_moves_every_eight_ticks_and_flips_every_thirty_two(&ctx);
TEST_MAIN_END

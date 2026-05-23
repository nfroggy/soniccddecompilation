#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];

static int actionsub_count;
static sprite_status *actionsub_actors[8];
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int hitchk_count;
static sprite_status *hitchk_actors[8];
static sprite_status *hitchk_players[8];
static int actwkchk2_count;
static sprite_status *actwkchk2_actor;
static sprite_status *actwkchk2_queue[8];
static int actwkchk2_queue_count;
static int actwkchk2_queue_index;

void actionsub(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);

#include "src/r8/tobira.c"

void actionsub(sprite_status *pActwk) {
    if (actionsub_count < 8) {
        actionsub_actors[actionsub_count] = pActwk;
    }
    ++actionsub_count;
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

Sint16 hitchk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    if (hitchk_count < 8) {
        hitchk_actors[hitchk_count] = pActwk;
        hitchk_players[hitchk_count] = pPlayerwk;
    }
    ++hitchk_count;
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

static void reset_logs(void) {
    actionsub_count = 0;
    memset(actionsub_actors, 0, sizeof(actionsub_actors));
    frameout_s_count = 0;
    frameout_s_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    hitchk_count = 0;
    memset(hitchk_actors, 0, sizeof(hitchk_actors));
    memset(hitchk_players, 0, sizeof(hitchk_players));
    actwkchk2_count = 0;
    actwkchk2_actor = 0;
    memset(actwkchk2_queue, 0, sizeof(actwkchk2_queue));
    actwkchk2_queue_count = 0;
    actwkchk2_queue_index = 0;
}

static void reset_tobira_state(void) {
    memset(actwk, 0, sizeof(actwk));
    reset_logs();
}

static void queue_actwkchk2(sprite_status *actor) {
    actwkchk2_queue[actwkchk2_queue_count++] = actor;
}

static void set_actfree_word(sprite_status *actor, int offset, Sint16 value) {
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static void assert_ms_end_callbacks(test_context *ctx, sprite_status *master,
                                    sprite_status *slave_actor) {
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == master);
    TEST_ASSERT_TRUE(ctx, patchg_table == (Uint8 **)pchg);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actors[0] == slave_actor);
    TEST_ASSERT_TRUE(ctx, actionsub_actors[1] == master);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
    TEST_ASSERT_TRUE(ctx, frameout_s_actor == master);
}

static void setup_initialized_door(sprite_status *master_actor,
                                   sprite_status *slave_actor,
                                   Sint8 userflag_h) {
    master_actor->actno = 41;
    master_actor->xposi.w.h = 100;
    master_actor->yposi.w.h = 200;
    master_actor->userflag.b.h = userflag_h;
    queue_actwkchk2(slave_actor);
    m_init(master_actor);
    reset_logs();
}

static void test_tobira_patterns_capture_default_literal_data(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, pchg[0] == pchg00);
    TEST_ASSERT_EQ_INT(ctx, 2, pchg00[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, pchg00[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, pchg00[2]);
    TEST_ASSERT_EQ_INT(ctx, -1, pchg00[3]);
    TEST_ASSERT_TRUE(ctx, pat_tobira0[0] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_tobira0[1] == &pat00);
    TEST_ASSERT_TRUE(ctx, pat_tobira1[0] == &pat01);
    TEST_ASSERT_TRUE(ctx, pat_tobira1[1] == &pat02);
    TEST_ASSERT_TRUE(ctx, pat_tobira2[0] == &pat03);
    TEST_ASSERT_TRUE(ctx, pat_tobira2[1] == &pat04);
    TEST_ASSERT_EQ_INT(ctx, 464, pat00.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 465, pat01.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 466, pat02.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 467, pat03.spra[0].index);
    TEST_ASSERT_EQ_INT(ctx, 468, pat04.spra[0].index);
}

static void test_tobira_init_sets_type0_master_and_slave(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    master_actor->actno = 41;
    master_actor->xposi.w.h = 100;
    master_actor->yposi.w.h = 200;
    queue_actwkchk2(slave_actor);

    tobira(master_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_TRUE(ctx, actwkchk2_actor == master_actor);
}

static void test_tobira_init_selects_type1_and_type2_patterns(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 2);
}

static void test_tobira_init_frames_out_when_slave_allocation_fails(
    test_context *ctx) {
    sprite_status *master_actor = &actwk[3];

    reset_tobira_state();
    master_actor->actno = 41;
    master_actor->yposi.w.h = 200;

    m_init(master_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == master_actor);
}

static void test_tobira_ini_com_returns_offsets_and_sets_common_fields(
    test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *part = &actwk[4];
    Sint16 d0 = 0;
    Sint16 d1 = 0;

    reset_tobira_state();
    set_actfree_word(master_actor, 4, 184);

    ini_com(master_actor, part, &d0, &d1);

    TEST_ASSERT_EQ_INT(ctx, 16, d0);
    TEST_ASSERT_EQ_INT(ctx, 49, d1);
}

static void test_tobira_closed_stays_closed_for_type0(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 0);

    m_closed(master_actor);

    TEST_ASSERT_EQ_INT(ctx, 2, hitchk_count);
    TEST_ASSERT_TRUE(ctx, hitchk_actors[0] == slave_actor);
    TEST_ASSERT_TRUE(ctx, hitchk_actors[1] == master_actor);
    TEST_ASSERT_TRUE(ctx, hitchk_players[0] == &actwk[0]);
    TEST_ASSERT_TRUE(ctx, hitchk_players[1] == &actwk[0]);
    assert_ms_end_callbacks(ctx, master_actor, slave_actor);
}

static void test_tobira_master_dispatches_each_noninitial_routine(
    test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 0);

    master_actor->r_no0 = 2;
    master(master_actor);
    assert_ms_end_callbacks(ctx, master_actor, slave_actor);

    reset_logs();
    master_actor->r_no0 = 4;
    master_actor->yposi.w.h = 200;
    slave_actor->yposi.w.h = 168;
    master(master_actor);

    reset_logs();
    master_actor->r_no0 = 6;
    master(master_actor);

    reset_logs();
    master_actor->r_no0 = 8;
    master_actor->yposi.w.h = 220;
    slave_actor->yposi.w.h = 148;
    master(master_actor);
}

static void test_tobira_closed_opens_when_forced_by_actfree(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 0);
    master_actor->actfree[21] = 1;

    m_closed(master_actor);

    assert_ms_end_callbacks(ctx, master_actor, slave_actor);
}

static void test_tobira_closed_type1_uses_player_area(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);
    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 190;

    m_closed(master_actor);


    reset_logs();
    master_actor->r_no0 = 2;
    actwk[0].xposi.w.h = 200;

    m_closed(master_actor);

}

static void test_tobira_closed_type2_uses_shifted_player_area(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 2);
    actwk[0].xposi.w.h = 29;
    actwk[0].yposi.w.h = 190;

    m_closed(master_actor);

}

static void test_tobira_open_moves_halves_until_open_limit(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);
    master_actor->r_no0 = 4;
    master_actor->yposi.w.h = 200;
    slave_actor->yposi.w.h = 168;

    m_open(master_actor);

    assert_ms_end_callbacks(ctx, master_actor, slave_actor);

    reset_logs();
    master_actor->yposi.w.h = 232;
    slave_actor->yposi.w.h = 136;

    m_open(master_actor);

}

static void test_tobira_opened_type0_does_not_begin_closing(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 0);
    master_actor->r_no0 = 6;

    m_opend(master_actor);

    assert_ms_end_callbacks(ctx, master_actor, slave_actor);
}

static void test_tobira_opened_type1_closes_when_player_leaves_area(
    test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);
    master_actor->r_no0 = 6;
    actwk[0].xposi.w.h = 60;
    actwk[0].yposi.w.h = 190;

    m_opend(master_actor);


    reset_logs();
    actwk[0].xposi.w.h = 200;

    m_opend(master_actor);

}

static void test_tobira_opened_type2_uses_shifted_player_area(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 2);
    master_actor->r_no0 = 6;
    actwk[0].xposi.w.h = 29;
    actwk[0].yposi.w.h = 190;

    m_opend(master_actor);


    reset_logs();
    actwk[0].xposi.w.h = 200;

    m_opend(master_actor);

}

static void test_tobira_close_moves_halves_until_closed_limit(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);
    master_actor->r_no0 = 8;
    master_actor->yposi.w.h = 220;
    slave_actor->yposi.w.h = 148;

    m_close(master_actor);

    assert_ms_end_callbacks(ctx, master_actor, slave_actor);

    reset_logs();
    master_actor->yposi.w.h = 201;
    slave_actor->yposi.w.h = 167;

    m_close(master_actor);

}

static void test_tobira_ms_end_copies_patno_to_slave(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    setup_initialized_door(master_actor, slave_actor, 1);
    master_actor->patno = 1;
    slave_actor->patno = 0;

    ms_end(master_actor);

    assert_ms_end_callbacks(ctx, master_actor, slave_actor);
}

static void test_tobira_m_area_captures_unsigned_boundaries(test_context *ctx) {
    sprite_status *master_actor = &actwk[3];

    reset_tobira_state();
    master_actor->xposi.w.h = 100;
    set_actfree_word(master_actor, 4, 184);
    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 136;
    TEST_ASSERT_EQ_INT(ctx, 1, m_area(master_actor, 8, 64));

    actwk[0].xposi.w.h = 172;
    TEST_ASSERT_EQ_INT(ctx, 0, m_area(master_actor, 8, 64));

    actwk[0].xposi.w.h = 7;
    TEST_ASSERT_EQ_INT(ctx, 0, m_area(master_actor, 8, 64));

    actwk[0].xposi.w.h = 108;
    actwk[0].yposi.w.h = 232;
    TEST_ASSERT_EQ_INT(ctx, 0, m_area(master_actor, 8, 64));
}

static void test_tobira_slave_frames_out_when_master_disappears(
    test_context *ctx) {
    sprite_status *slave_actor = &actwk[20];

    reset_tobira_state();
    actwk[3].actno = 41;
    set_actfree_word(slave_actor, 0, 3);

    tobira(slave_actor);

    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    actwk[3].actno = 0;
    tobira(slave_actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == slave_actor);
}

TEST_MAIN_BEGIN;
    test_tobira_patterns_capture_default_literal_data(&ctx);
    test_tobira_init_sets_type0_master_and_slave(&ctx);
    test_tobira_init_selects_type1_and_type2_patterns(&ctx);
    test_tobira_init_frames_out_when_slave_allocation_fails(&ctx);
    test_tobira_ini_com_returns_offsets_and_sets_common_fields(&ctx);
    test_tobira_closed_stays_closed_for_type0(&ctx);
    test_tobira_master_dispatches_each_noninitial_routine(&ctx);
    test_tobira_closed_opens_when_forced_by_actfree(&ctx);
    test_tobira_closed_type1_uses_player_area(&ctx);
    test_tobira_closed_type2_uses_shifted_player_area(&ctx);
    test_tobira_open_moves_halves_until_open_limit(&ctx);
    test_tobira_opened_type0_does_not_begin_closing(&ctx);
    test_tobira_opened_type1_closes_when_player_leaves_area(&ctx);
    test_tobira_opened_type2_uses_shifted_player_area(&ctx);
    test_tobira_close_moves_halves_until_closed_limit(&ctx);
    test_tobira_ms_end_copies_patno_to_slave(&ctx);
    test_tobira_m_area_captures_unsigned_boundaries(&ctx);
    test_tobira_slave_frames_out_when_master_disappears(&ctx);
TEST_MAIN_END

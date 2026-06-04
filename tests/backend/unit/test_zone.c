#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Uint8 ta_flag;
Uint8 clrspflg_save;
Uint8 pl_suu;
Uint8 pltimeover_f;
Uint8 play_start;
Uint8 plflag;
Uint8 time_flag;
Uint8 generate_flag;
Uint8 plsubchg_flag;
Uint8 enkeino;
Uint8 projector_flag;
Uint8 special_flag;
Uint8 enecgflg;
Uint8 markerno;
Uint8 gf_flag;
Uint8 tv_flag;
Uint8 plautoflag;
Sint16 timebonus;
Sint16 ringbonus;
Uint8 bonus_f;
short_union scroll_start;
int_union scra_h_posit;
sprite_status actwk[128];
int_union systemtimer;
Uint8 flowercnt[3];
short_union stageno;
short_union gameflag;

static sprite_pattern title_pattern0 = {1, {{0, 0, 0, 900}}};
static sprite_pattern title_pattern1 = {1, {{0, 0, 0, 901}}};
sprite_pattern *title_pat[2] = {&title_pattern0, &title_pattern1};
Uint16 title_tbl[32] = {
    10, 100, 200, 0x0105, 20, 110, 210, 0x0206,
    30, 120, 220, 0x0307, 40, 130, 230, 0x0408,
    50, 140, 240, 0x0509, 60, 150, 250, 0x060A,
    70, 160, 260, 0x070B, 80, 170, 270, 0x080C,
};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int sound_count;
static Sint16 sound_requests[8];
static int wave_stop_count;
static int scoreup_count;
static Uint32 scoreup_values[8];
static int flagwkclr_count;
static int fadeout_count;
static int actwkchk_count;
static int actwkchk_index;
static int actwkchk_fail;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
void sub_sync(Sint16 ReqNo);
void soundset(Sint16 ReqNo);
void scoreup(Uint32 updata);
void flagwkclr(void);
void fadeout_s(void);
Sint32 actwkchk(sprite_status **ppActwk);
static void wave_all_stop_callback(void);
void (*WaveAllStop)(void) = wave_all_stop_callback;

#include "src/zone.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    memset(pActwk, 0, sizeof(*pActwk));
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void soundset(Sint16 ReqNo) {
    if (sound_count < 8) {
        sound_requests[sound_count] = ReqNo;
    }
    ++sound_count;
}

void scoreup(Uint32 updata) {
    if (scoreup_count < 8) {
        scoreup_values[scoreup_count] = updata;
    }
    ++scoreup_count;
}

void flagwkclr(void) { ++flagwkclr_count; }

void fadeout_s(void) { ++fadeout_count; }

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail) {
        return 1;
    }

    *ppActwk = &actwk[actwkchk_index++];
    return 0;
}

static void wave_all_stop_callback(void) { ++wave_stop_count; }

static void reset_zone_state(void) {
    ta_flag = 0;
    clrspflg_save = 0;
    pl_suu = 0;
    pltimeover_f = 0;
    play_start = 0;
    plflag = 0;
    time_flag = 0;
    generate_flag = 0;
    plsubchg_flag = 0;
    enkeino = 0;
    projector_flag = 0;
    special_flag = 0;
    enecgflg = 0;
    markerno = 0;
    gf_flag = 0;
    tv_flag = 0;
    plautoflag = 0;
    timebonus = 0;
    ringbonus = 0;
    bonus_f = 0;
    memset(&scroll_start, 0, sizeof(scroll_start));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(actwk, 0, sizeof(actwk));
    memset(&systemtimer, 0, sizeof(systemtimer));
    memset(flowercnt, 0, sizeof(flowercnt));
    memset(&stageno, 0, sizeof(stageno));
    memset(&gameflag, 0, sizeof(gameflag));
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    sound_count = 0;
    memset(sound_requests, 0, sizeof(sound_requests));
    wave_stop_count = 0;
    scoreup_count = 0;
    memset(scoreup_values, 0, sizeof(scoreup_values));
    flagwkclr_count = 0;
    fadeout_count = 0;
    actwkchk_count = 0;
    actwkchk_index = 1;
    actwkchk_fail = 0;
    WaitCount = 0;
    ClearSountWait = 0;
}

static void test_over_initializes_game_over_pair(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));

    over(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 224, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_TRUE(ctx, actor.patbase == gamepat);
    TEST_ASSERT_EQ_INT(ctx, 128 | 8, plsubchg_flag);
    TEST_ASSERT_EQ_INT(ctx, 2, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 110, sub_sync_requests[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 59, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == gamepat);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 448, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 224, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_over_time_over_uses_time_pattern_and_moves(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    pltimeover_f = 1;
    pl_suu = 3;

    over(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, pltimeover_f);
    TEST_ASSERT_TRUE(ctx, actor.patbase == timepat);
    TEST_ASSERT_EQ_INT(ctx, 128 | 2, plsubchg_flag);
    TEST_ASSERT_EQ_INT(ctx, 136, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == timepat);
}

static void test_over_frames_out_when_life_bonus_without_timeout(
    test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    pl_suu = 1;

    over(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 8, plsubchg_flag);
}

static void test_over_frames_out_when_second_actor_cannot_allocate(
    test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actwkchk_fail = 1;

    over(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, 128 | 8, plsubchg_flag);
}

static void test_over_move_converges_to_target(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    zone_get_work(&actor)->over.target_x = 288;

    actor.xposi.w.h = 280;
    over(&actor);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);

    actor.xposi.w.h = 296;
    over(&actor);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);

    over(&actor);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actionsub_count);
}

static void test_title_initializes_title_actor_group(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    stageno.b.l = 4;

    title(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 280, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 48, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 90, actor.pattim);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_TRUE(ctx, actor.patbase == title_pat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 60, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 5, actwk[1].pattim);
    TEST_ASSERT_EQ_INT(ctx, 60, actwk[6].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[6].patno);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[6].pattim);
}

static void title_setup_actor(sprite_status *actor) {
    reset_zone_state();
    memset(actor, 0, sizeof(*actor));
    title(actor);
    actionsub_count = 0;
    frameout_count = 0;
}

static void test_title_move0_vertical_paths(test_context *ctx) {
    sprite_status actor;

    title_setup_actor(&actor);
    actor.r_no0 = 2;
    actor.yposi.w.h = 232;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 240, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);

    title_setup_actor(&actor);
    actor.r_no0 = 2;
    actor.yposi.w.h = 248;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 240, actor.yposi.w.h);

    title_setup_actor(&actor);
    actor.r_no0 = 2;
    actor.yposi.w.h = 240;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_title_move1_horizontal_paths(test_context *ctx) {
    sprite_status actor;

    title_setup_actor(&actor);
    actor = actwk[1];
    actor.r_no0 = 4;
    actor.xposi.w.h = 192;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.xposi.w.h);

    actor = actwk[1];
    actor.r_no0 = 4;
    actor.xposi.w.h = 208;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 200, actor.xposi.w.h);

    actor = actwk[1];
    actor.r_no0 = 4;
    actor.xposi.w.h = 200;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor.r_no0);
}

static void test_title_back0_waits_then_returns_vertically(test_context *ctx) {
    sprite_status actor;

    title_setup_actor(&actor);
    actor.r_no0 = 6;
    actor.pattim = 2;
    actor.yposi.w.h = 64;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.pattim);
    TEST_ASSERT_EQ_INT(ctx, 64, actor.yposi.w.h);

    actor.pattim = 0;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 48, actor.yposi.w.h);

    actor.yposi.w.h = 32;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 48, actor.yposi.w.h);

    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, scroll_start.b.h);
}

static void test_title_back1_waits_moves_and_frames_out(test_context *ctx) {
    sprite_status actor;

    title_setup_actor(&actor);
    actor = actwk[1];
    actor.r_no0 = 8;
    actor.pattim = 1;
    actor.xposi.w.h = 116;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.pattim);
    TEST_ASSERT_EQ_INT(ctx, 116, actor.xposi.w.h);

    actor.pattim = 0;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);

    actor.xposi.w.h = 84;
    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 100, actor.xposi.w.h);

    title(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
}

static void test_title_wait_stops_scroll_and_frames_out(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    scroll_start.b.h = 1;
    plautoflag = 1;

    title(&actor);

    TEST_ASSERT_EQ_INT(ctx, 0, scroll_start.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_clear_init_wait_and_actor_setup_paths(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    zone_get_work(&actor)->clear.start_delay = 2;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, zone_get_work(&actor)->clear.start_delay);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    zone_get_work(&actor)->clear.start_delay = 1;
    stageno.w = 1282;
    generate_flag = 1;
    systemtimer.w.l = 321;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 321, ClearSountWait);
    TEST_ASSERT_TRUE(ctx, actor.patbase == madepat_0);
    TEST_ASSERT_EQ_INT(ctx, 204, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 3, actwkchk_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actwk[0].xposi.w.h = 500;
    scra_h_posit.w.h = 100;
    generate_flag = 1;
    stageno.b.l = 2;
    clear(&actor);
    TEST_ASSERT_TRUE(ctx, actor.patbase == madepat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 204, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[2].patno);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actwk[0].xposi.w.h = 436;
    scra_h_posit.w.h = 100;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.actno);
}

static void test_clear_move0_position_and_action_paths(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.xposi.w.h = 280;
    actor.patno = 0;
    zone_get_work(&actor)->clear.target_x = 288;
    zone_get_work(&actor)->clear.slide_timer = 351;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.xposi.w.h = 296;
    zone_get_work(&actor)->clear.target_x = 288;
    zone_get_work(&actor)->clear.slide_timer = 360;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.xposi.w.h = 288;
    actor.patno = 0;
    zone_get_work(&actor)->clear.target_x = 288;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 6, actor.r_no0);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 4;
    actor.xposi.w.h = 288;
    actor.patno = 2;
    zone_get_work(&actor)->clear.target_x = 288;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
}

static void test_clear_move1_counts_down_without_bonus(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 6;
    special_flag = 1;
    zone_get_work(&actor)->clear.slide_timer = 31;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, bonus_f);
    TEST_ASSERT_EQ_INT(ctx, 1, sound_count);
    TEST_ASSERT_EQ_INT(ctx, 200, sound_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, scoreup_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 6;
    systemtimer.w.l = 600;
    ClearSountWait = 0;
    zone_get_work(&actor)->clear.slide_timer = 0;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, ClearSountWait);
}

static void test_clear_move1_awards_bonus_and_sounds(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 6;
    timebonus = 100;
    zone_get_work(&actor)->clear.slide_timer = 50;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, timebonus);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_stop_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sound_count);
    TEST_ASSERT_EQ_INT(ctx, 154, sound_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[0]);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 6;
    zone_get_work(&actor)->clear.start_delay = 2;
    timebonus = 200;
    ringbonus = 100;
    zone_get_work(&actor)->clear.slide_timer = 5;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 100, timebonus);
    TEST_ASSERT_EQ_INT(ctx, 0, ringbonus);
    TEST_ASSERT_EQ_INT(ctx, 1, sound_count);
    TEST_ASSERT_EQ_INT(ctx, 189, sound_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, scoreup_values[0]);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 6;
    timebonus = 200;
    zone_get_work(&actor)->clear.slide_timer = 0x0200;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, sound_count);
    TEST_ASSERT_EQ_INT(ctx, 10, scoreup_values[0]);
}

static void test_clear_wait_state_advances_after_timer(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 8;
    ClearSountWait = 1000;
    systemtimer.w.l = 1719;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 8, actor.r_no0);

    systemtimer.w.l = 1720;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 10, actor.r_no0);

    actor.r_no0 = 12;
    WaitCount = 2;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 12, actor.r_no0);
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
}

static void seed_clear_move2_dirty_globals(void) {
    gameflag.w = 99;
    plflag = 7;
    enecgflg = 8;
    flowercnt[0] = 1;
    flowercnt[1] = 2;
    flowercnt[2] = 3;
    tv_flag = 4;
    enkeino = 5;
    projector_flag = 6;
    markerno = 7;
    play_start = 255;
}

static void test_clear_move2_resets_level_state_and_time_zone(
    test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    ta_flag = 1;
    stageno.b.l = 2;
    gf_flag = 77;
    seed_clear_move2_dirty_globals();
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, gameflag.w);
    TEST_ASSERT_EQ_INT(ctx, 0, plflag);
    TEST_ASSERT_EQ_INT(ctx, 0, enecgflg);
    TEST_ASSERT_EQ_INT(ctx, 0, flowercnt[0] + flowercnt[1] + flowercnt[2]);
    TEST_ASSERT_EQ_INT(ctx, 0, tv_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, enkeino);
    TEST_ASSERT_EQ_INT(ctx, 0, projector_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, markerno);
    TEST_ASSERT_EQ_INT(ctx, 252, play_start);
    TEST_ASSERT_EQ_INT(ctx, 1, time_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, stageno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, stageno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 0, gf_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwkclr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, fadeout_count);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    ta_flag = 1;
    stageno.b.l = 0;
    seed_clear_move2_dirty_globals();
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, stageno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 252, play_start);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    stageno.b.l = 1;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, stageno.b.l);
    TEST_ASSERT_EQ_INT(ctx, 2, time_flag);
}

static void test_clear_move2_generate_flag_paths(test_context *ctx) {
    sprite_status actor;

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    stageno.b.l = 0;
    clrspflg_save = 0;
    generate_flag = 0;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 0, gf_flag);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    stageno.b.l = 0;
    clrspflg_save = 0;
    generate_flag = 1;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, gf_flag);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    stageno.b.l = 1;
    clrspflg_save = 0;
    generate_flag = 1;
    gf_flag = 1;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 3, gf_flag);

    reset_zone_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 10;
    stageno.b.l = 0;
    clrspflg_save = 127;
    clear(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, generate_flag);
}

TEST_MAIN_BEGIN;
    test_over_initializes_game_over_pair(&ctx);
    test_over_time_over_uses_time_pattern_and_moves(&ctx);
    test_over_frames_out_when_life_bonus_without_timeout(&ctx);
    test_over_frames_out_when_second_actor_cannot_allocate(&ctx);
    test_over_move_converges_to_target(&ctx);
    test_title_initializes_title_actor_group(&ctx);
    test_title_move0_vertical_paths(&ctx);
    test_title_move1_horizontal_paths(&ctx);
    test_title_back0_waits_then_returns_vertically(&ctx);
    test_title_back1_waits_moves_and_frames_out(&ctx);
    test_title_wait_stops_scroll_and_frames_out(&ctx);
    test_clear_init_wait_and_actor_setup_paths(&ctx);
    test_clear_move0_position_and_action_paths(&ctx);
    test_clear_move1_counts_down_without_bonus(&ctx);
    test_clear_move1_awards_bonus_and_sounds(&ctx);
    test_clear_wait_state_advances_after_timer(&ctx);
    test_clear_move2_resets_level_state_and_time_zone(&ctx);
    test_clear_move2_generate_flag_paths(&ctx);
TEST_MAIN_END;

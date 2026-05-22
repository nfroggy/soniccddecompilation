#include <string.h>

#include "support/test_runner.h"
#include "types.h"

Uint8 plscore_f;
Sint32 plscore;
Sint32 extrascore;
Uint8 pl_suu;
Uint8 pl_suu_f;
short_union debugflag;
Uint8 time_flag;
Sint16 plring;
int_union systemtimer;
Uint8 plring_f;
ushort_union pauseflag;
Uint8 pltime_f;
int_union pltime;
Uint8 pltimeover_f;
Uint8 plautoflag;
sprite_status actwk[128];
Uint8 bonus_f;
short_union stageno;
Sint16 ringbonus;
Sint16 timebonus;
Sint16 blkno;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int frameout_count;
static sprite_status *frameout_actor;
static int playdieset_count;
static sprite_status *playdieset_actor;
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int wave_stop_count;

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);
Sint16 playdieset(sprite_status *pActwk);
void sub_sync(Sint16 ReqNo);
static void wave_all_stop_callback(void);
void (*WaveAllStop)(void) = wave_all_stop_callback;

sprite_pattern bonuspat = {18, {{0}}};
sprite_pattern bonuspat0 = {18, {{0}}};

#include "src/score.c"

#define SPRITE_DIGIT_1 (SPRITE_DIGIT_0 + 1)
#define SPRITE_DIGIT_2 (SPRITE_DIGIT_0 + 2)
#define SPRITE_DIGIT_3 (SPRITE_DIGIT_0 + 3)
#define SPRITE_DIGIT_4 (SPRITE_DIGIT_0 + 4)
#define SPRITE_DIGIT_5 (SPRITE_DIGIT_0 + 5)
#define SPRITE_DIGIT_6 (SPRITE_DIGIT_0 + 6)
#define SPRITE_DIGIT_7 (SPRITE_DIGIT_0 + 7)
#define SPRITE_DIGIT_8 (SPRITE_DIGIT_0 + 8)
#define SPRITE_DIGIT_9 (SPRITE_DIGIT_0 + 9)
#define SPRITE_DIGIT_A (SPRITE_DIGIT_0 + 10)
#define SPRITE_DIGIT_B (SPRITE_DIGIT_0 + 11)

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    memset(pActwk, 0, sizeof(*pActwk));
}

Sint16 playdieset(sprite_status *pActwk) {
    ++playdieset_count;
    playdieset_actor = pActwk;
    return 0;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

static void wave_all_stop_callback(void) { ++wave_stop_count; }

static void reset_score_patterns(void) {
    int i;

    for (i = 0; i < 20; ++i) {
        bonuspat.spra[i].index = 0;
        bonuspat0.spra[i].index = 0;
    }

    scorepat0.spra[1].index = 0;
    scorepat0.spra[2].index = 0;
    scorepat0.spra[3].index = 0;
    scorepat0.spra[4].index = 0;
    scorepat0.spra[5].index = 0;
    scorepat0.spra[6].index = 0;
    scorepat0.spra[7].index = SPRITE_DIGIT_0;
    scorepat0.spra[9].index = SPRITE_DIGIT_0;
    scorepat0.spra[11].index = SPRITE_DIGIT_0;
    scorepat0.spra[12].index = SPRITE_DIGIT_0;
    scorepat0.spra[14].index = SPRITE_DIGIT_0;
    scorepat0.spra[15].index = SPRITE_DIGIT_0;
    scorepat0.spra[16].index = 0;
    scorepat0.spra[17].index = 0;
    scorepat0.spra[18].index = SPRITE_DIGIT_0;

    scorepat1.spra[0].index = SPRITE_SCORE_BASE + 8;
    scorepat1.spra[2].index = SPRITE_DIGIT_0;

    scorepat2.spra[3].index = 0;
    scorepat2.spra[4].index = 0;
    scorepat2.spra[5].index = 0;
    scorepat2.spra[6].index = 0;
    scorepat2.spra[8].index = 0;
    scorepat2.spra[9].index = 0;
    scorepat2.spra[10].index = 0;
    scorepat2.spra[11].index = SPRITE_DIGIT_0;
    scorepat2.spra[12].index = 0;
    scorepat2.spra[13].index = 0;
    scorepat2.spra[14].index = SPRITE_DIGIT_0;

    scorepat3.spra[0].index = SPRITE_SCORE_BASE + 5;
}

static void reset_score_state(void) {
    plscore_f = 0;
    plscore = 0;
    extrascore = 0;
    pl_suu = 0;
    pl_suu_f = 0;
    memset(&debugflag, 0, sizeof(debugflag));
    time_flag = 0;
    plring = 0;
    memset(&systemtimer, 0, sizeof(systemtimer));
    plring_f = 0;
    memset(&pauseflag, 0, sizeof(pauseflag));
    pltime_f = 0;
    memset(&pltime, 0, sizeof(pltime));
    pltimeover_f = 0;
    plautoflag = 0;
    memset(actwk, 0, sizeof(actwk));
    bonus_f = 0;
    memset(&stageno, 0, sizeof(stageno));
    ringbonus = 0;
    timebonus = 0;
    blkno = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    frameout_count = 0;
    frameout_actor = 0;
    playdieset_count = 0;
    playdieset_actor = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    wave_stop_count = 0;
    reset_score_patterns();
}

static void test_score_initializes_main_hud_actor(test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));

    score(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_TRUE(ctx, actor.patbase == scorepat);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 144, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 136, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == &actor);
}

static void test_score_initializes_debug_and_ring_icon_actors(
    test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    debugflag.w = 1;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.patno);

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.l = 1;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.patno);
}

static void test_score_initializes_life_icon_by_time_zone(test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.h = 1;
    time_flag = 0;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 328, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_LIFEICON_PAST, scorepat1.spra[0].index);

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.h = 1;
    time_flag = 1;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_LIFEICON_PRESENT, scorepat1.spra[0].index);

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.h = 1;
    time_flag = 2;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_LIFEICON_FUTURE, scorepat1.spra[0].index);
}

static void test_score_moves_existing_actors(test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actor.userflag.b.h = 0;
    actor.patno = 7;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actor.userflag.b.h = 0;
    actor.userflag.b.l = 1;
    plring = 1;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RINGS_NORMAL, scorepat3.spra[0].index);

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.r_no0 = 2;
    actor.userflag.b.h = 0;
    actor.userflag.b.l = 1;
    plring = 0;
    scorepat3.spra[0].index = SPRITE_RINGS_NORMAL;
    systemtimer.b.b4 = 0;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RINGS_DANGER, scorepat3.spra[0].index);

    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RINGS_NORMAL, scorepat3.spra[0].index);

    systemtimer.b.b4 = 1;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_RINGS_NORMAL, scorepat3.spra[0].index);
}

static void test_score_ten_point_actor_initializes_moves_and_frames_out(
    test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.h = 128 | 3;
    actor.yposi.w.h = 100;

    score(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.actflg);
    TEST_ASSERT_TRUE(ctx, actor.patbase == tenpat);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 23, actor.actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 98, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    actor.actfree[0] = 1;
    actor.yposi.w.h = 50;
    score(&actor);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actor);
    TEST_ASSERT_EQ_INT(ctx, -2, actor.yposi.w.h);
}

static void test_tensuu_static_entry_initializes_actor(test_context *ctx) {
    sprite_status actor;

    reset_score_state();
    memset(&actor, 0, sizeof(actor));
    actor.userflag.b.h = 128 | 4;
    actor.yposi.w.h = 80;

    tensuu(&actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_TRUE(ctx, actor.patbase == tenpat);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 23, actor.actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 78, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_scoreup_clamps_and_awards_life(test_context *ctx) {
    reset_score_state();
    plscore = 999990;
    extrascore = 1000000;

    scoreup(20);

    TEST_ASSERT_EQ_INT(ctx, 1, plscore_f);
    TEST_ASSERT_EQ_INT(ctx, 999999, plscore);
    TEST_ASSERT_EQ_INT(ctx, 1000000, extrascore);
    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_stop_count);

    reset_score_state();
    plscore = 4000;
    extrascore = 5000;
    scoreup(1000);
    TEST_ASSERT_EQ_INT(ctx, 5000, plscore);
    TEST_ASSERT_EQ_INT(ctx, 10000, extrascore);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_stop_count);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 122, sub_sync_requests[0]);
}

static void test_scoreset_writes_score_and_ring_digits(test_context *ctx) {
    reset_score_state();
    plscore_f = 128 | 1;
    plscore = 12345;
    plring_f = 128 | 1;
    plring = 1234;

    scoreset();

    TEST_ASSERT_EQ_INT(ctx, 0, plscore_f);
    TEST_ASSERT_EQ_INT(ctx, 0, plring_f);
    TEST_ASSERT_EQ_INT(ctx, 999, plring);
    TEST_ASSERT_EQ_INT(ctx, 0, scorepat0.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat0.spra[2].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, scorepat0.spra[3].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_3, scorepat0.spra[4].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_4, scorepat0.spra[5].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_5, scorepat0.spra[6].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat0.spra[16].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat0.spra[17].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat0.spra[18].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, scorepat1.spra[2].index);
}

static void test_scoreset_writes_time_and_timeout_death(test_context *ctx) {
    reset_score_state();
    pltime_f = 1;
    pltime.b.b2 = 1;
    pltime.b.b3 = 2;
    pltime.b.b4 = 30;

    scoreset();

    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat0.spra[9].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, scorepat0.spra[11].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, scorepat0.spra[12].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_5, scorepat0.spra[14].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat0.spra[15].index);
    TEST_ASSERT_EQ_INT(ctx, 31, pltime.b.b4);

    reset_score_state();
    pltime_f = 1;
    pltime.b.b2 = 8;
    pltime.b.b3 = 59;
    pltime.b.b4 = 59;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 9, pltime.b.b2);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime.b.b3);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime.b.b4);

    reset_score_state();
    pltime_f = 1;
    pltime.l = 604987;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 0, pltime_f);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime.l);
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);
    TEST_ASSERT_TRUE(ctx, playdieset_actor == &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, pltimeover_f);

    reset_score_state();
    pltime_f = 1;
    pltime.l = 604987;
    time_flag = 128;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 1, pltime_f);
    TEST_ASSERT_EQ_INT(ctx, 604987, pltime.l);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    reset_score_state();
    pltime_f = 1;
    pltime.l = 604986;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 604987, pltime.l);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat0.spra[14].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat0.spra[15].index);
}

static void test_scoreset_respects_pause_and_autoplay_time(test_context *ctx) {
    reset_score_state();
    pltime_f = 1;
    pauseflag.b.h = 1;
    pltime.b.b4 = 10;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 10, pltime.b.b4);

    reset_score_state();
    pltime_f = 1;
    plautoflag = 1;
    pltime.b.b4 = 10;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 10, pltime.b.b4);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat0.spra[14].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_6, scorepat0.spra[15].index);
}

static void test_scoreset_writes_lives_debug_and_bonus(test_context *ctx) {
    reset_score_state();
    pl_suu_f = 1;
    pl_suu = 12;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu_f);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, scorepat1.spra[2].index);

    reset_score_state();
    debugflag.w = 1;
    actwk[0].xposi.w.h = 0x1234;
    actwk[0].yposi.w.h = 0x00AB;
    blkno = 0x0456;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat2.spra[3].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, scorepat2.spra[4].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_3, scorepat2.spra[5].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_4, scorepat2.spra[6].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, scorepat2.spra[8].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, scorepat2.spra[9].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_A, scorepat2.spra[10].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_B, scorepat2.spra[11].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_B, scorepat2.spra[12].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, scorepat2.spra[13].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, scorepat2.spra[14].index);

    reset_score_state();
    bonus_f = 1;
    stageno.w = 0;
    ringbonus = 1234;
    timebonus = 5678;
    plscore = 9012;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, 0, bonus_f);
    TEST_ASSERT_EQ_INT(ctx, 0, bonuspat.spra[8].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, bonuspat.spra[9].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, bonuspat.spra[10].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_3, bonuspat.spra[11].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_4, bonuspat.spra[12].index);
    TEST_ASSERT_EQ_INT(ctx, 0, bonuspat.spra[13].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_5, bonuspat.spra[14].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_6, bonuspat.spra[15].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_7, bonuspat.spra[16].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_8, bonuspat.spra[17].index);
    TEST_ASSERT_EQ_INT(ctx, 0, bonuspat.spra[1].index);
    TEST_ASSERT_EQ_INT(ctx, 0, bonuspat.spra[2].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_9, bonuspat.spra[3].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, bonuspat.spra[4].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, bonuspat.spra[5].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, bonuspat.spra[6].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_0, bonuspat.spra[7].index);

    reset_score_state();
    bonus_f = 1;
    stageno.w = 1282;
    ringbonus = 1;
    timebonus = 2;
    plscore = 3;
    scoreset();
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_1, bonuspat0.spra[12].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_2, bonuspat0.spra[17].index);
    TEST_ASSERT_EQ_INT(ctx, SPRITE_DIGIT_3, bonuspat0.spra[6].index);
}

TEST_MAIN_BEGIN;
    test_score_initializes_main_hud_actor(&ctx);
    test_score_initializes_debug_and_ring_icon_actors(&ctx);
    test_score_initializes_life_icon_by_time_zone(&ctx);
    test_score_moves_existing_actors(&ctx);
    test_score_ten_point_actor_initializes_moves_and_frames_out(&ctx);
    test_tensuu_static_entry_initializes_actor(&ctx);
    test_scoreup_clamps_and_awards_life(&ctx);
    test_scoreset_writes_score_and_ring_digits(&ctx);
    test_scoreset_writes_time_and_timeout_death(&ctx);
    test_scoreset_respects_pause_and_autoplay_time(&ctx);
    test_scoreset_writes_lives_debug_and_bonus(&ctx);
TEST_MAIN_END;

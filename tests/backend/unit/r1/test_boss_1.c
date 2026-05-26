#include <stddef.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
Uint8 bossflag;
Uint8 bossstart;
Uint8 generate_flag;
Sint16 scra_vline;
Sint16 scralim_left;
Sint16 scralim_n_left;
Sint16 scralim_right;
Sint16 scralim_n_right;
Sint16 scralim_down;
Sint16 scralim_n_down;
Sint16 scr_dir_tbl[6] = {4, 0, 3735, 0, 800, 96};

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_requests[8];
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int next_alloc_index;
static int actwkchk_fail_after;
static int colorset2_count;
static Sint32 colorset2_value;
static int scoreup_count;
static Uint32 scoreup_value;
static int sub_sync_count;
static Sint16 sub_sync_requests[8];
static int genecolor_count;
static int sinset_count;
static Uint8 sinset_angle;
static Sint16 sinset_sin;
static Sint16 sinset_cos;
static Sint16 emycol_d_result;

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void colorset2(Sint32 ColorNo);
void scoreup(Uint32 score);
void sub_sync(Sint16 ReqNo);
void genecolor(void);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);
Sint16 emycol_d(sprite_status *pActwk);

#include "src/r1/boss_1.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < (int)(sizeof(soundset_requests) /
                               sizeof(soundset_requests[0]))) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (actwkchk_fail_after >= 0 && actwkchk_count > actwkchk_fail_after) {
        *ppActwk = 0;
        return 1;
    }
    *ppActwk = &actwk[next_alloc_index++];
    return 0;
}

void colorset2(Sint32 ColorNo) {
    ++colorset2_count;
    colorset2_value = ColorNo;
}

void scoreup(Uint32 score) {
    ++scoreup_count;
    scoreup_value = score;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < (int)(sizeof(sub_sync_requests) /
                               sizeof(sub_sync_requests[0]))) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void genecolor(void) { ++genecolor_count; }

void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    ++sinset_count;
    sinset_angle = kakudo;
    *sin = sinset_sin;
    *cos = sinset_cos;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}

static size_t short_alias_offset(int short_index) {
    return (size_t)(short_index - 23) * sizeof(Sint16);
}

static void set_actor_short_alias(sprite_status *actor, int short_index,
                                  Sint16 value) {
    size_t offset = short_alias_offset(short_index);
    Uint16 bits = (Uint16)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
}

static Sint16 actor_short_alias(sprite_status *actor, int short_index) {
    size_t offset = short_alias_offset(short_index);
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static size_t long_alias_offset(int long_index) {
    return (size_t)long_index * sizeof(Sint32) -
           offsetof(sprite_status, actfree);
}

static void set_actor_long_alias(sprite_status *actor, int long_index,
                                 Sint32 value) {
    size_t offset = long_alias_offset(long_index);
    Uint32 bits = (Uint32)value;
    actor->actfree[offset] = (Uint8)bits;
    actor->actfree[offset + 1] = (Uint8)(bits >> 8);
    actor->actfree[offset + 2] = (Uint8)(bits >> 16);
    actor->actfree[offset + 3] = (Uint8)(bits >> 24);
}

static Sint32 actor_long_alias(sprite_status *actor, int long_index) {
    size_t offset = long_alias_offset(long_index);
    return (Sint32)((Uint32)actor->actfree[offset] |
                    ((Uint32)actor->actfree[offset + 1] << 8) |
                    ((Uint32)actor->actfree[offset + 2] << 16) |
                    ((Uint32)actor->actfree[offset + 3] << 24));
}

static void reset_boss1_state(void) {
    memset(actwk, 0, sizeof(actwk));
    bossflag = 0;
    bossstart = 0;
    generate_flag = 0;
    scra_vline = 0;
    scralim_left = scralim_n_left = 0;
    scralim_right = scralim_n_right = 0;
    scralim_down = scralim_n_down = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    next_alloc_index = 1;
    actwkchk_fail_after = -1;
    colorset2_count = 0;
    colorset2_value = 0;
    scoreup_count = 0;
    scoreup_value = 0;
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    genecolor_count = 0;
    sinset_count = 0;
    sinset_angle = 0;
    sinset_sin = 0;
    sinset_cos = 0;
    emycol_d_result = 0;
}

static void make_default_boss_parts(sprite_status *boss) {
    next_alloc_index = 1;
    actwkchk_fail_after = -1;
    boss->xposi.w.h = 3026;
    boss->yposi.w.h = 120;
    egg1_make_act(boss);
}

static void set_all_boss1_collision_slots(Uint8 colino) {
    int i;

    for (i = 0; i <= 14; ++i)
        actwk[i].colino = colino;
}

static void test_egg1_ini_sets_startup_state(test_context *ctx) {
    sprite_status *boss = &actwk[10];

    reset_boss1_state();

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_ini(boss));
    TEST_ASSERT_EQ_INT(ctx, 1, bossstart);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, boss->actflg);
    TEST_ASSERT_EQ_INT(ctx, 6, boss->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 20, boss->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 8, boss->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1021, boss->sproffset);
    TEST_ASSERT_TRUE(ctx, boss->patbase == egg1_pat);
    TEST_ASSERT_EQ_INT(ctx, 3154, boss->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, boss->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 5, colorset2_value);
}

static void test_egg1_anime_counts_down_and_resets_pattern(test_context *ctx) {
    sprite_status *boss = &actwk[1];

    reset_boss1_state();
    boss->actfree[0] = 2;
    boss->patno = 7;
    boss->patcnt = 8;
    boss->pattim = 9;
    boss->pattimm = 10;
    boss->mstno.b.h = 2;

    egg1_anime(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, boss->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 7, boss->patno);

    egg1_anime(boss);

    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->patno);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->patcnt);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->pattim);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->pattimm);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->mstno.b.h);
}

static void test_egg1_jisin_sets_limits_and_shake(test_context *ctx) {
    sprite_status *boss = &actwk[2];

    reset_boss1_state();

    egg1_jisin(boss);

    TEST_ASSERT_EQ_INT(ctx, 784, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_n_down);

    bossflag = 1;
    boss->actfree[11] = 2;

    egg1_jisin(boss);

    TEST_ASSERT_EQ_INT(ctx, 255, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 255, scralim_n_down);
    TEST_ASSERT_EQ_INT(ctx, 1, boss->actfree[11]);

    egg1_jisin(boss);

    TEST_ASSERT_EQ_INT(ctx, 257, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[11]);
}

static void test_sonic_hajiku_uses_collision_side_and_air_state(
    test_context *ctx) {
    reset_boss1_state();
    actwk[0].xposi.w.h = 100;
    actwk[5].xposi.w.h = 200;

    sonic_hajiku(5);

    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].mspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[0].direc.b.h);

    actwk[0].xposi.w.h = 300;
    actwk[0].cddat = 2;

    sonic_hajiku(5);

    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -1024, actwk[0].yspeed.w);
}

static void test_egg1_warai_chk_starts_laugh_animation(test_context *ctx) {
    sprite_status *boss = &actwk[3];

    reset_boss1_state();
    actwk[1].patno = 1;

    egg1_warai_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, boss->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 60, boss->actfree[0]);

    boss->mstno.b.h = 0;
    boss->actfree[0] = 0;
    actwk[1].patno = 0;
    actwk[0].r_no0 = 6;

    egg1_warai_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, boss->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 60, boss->actfree[0]);
}

static void test_make_act_success_and_failure(test_context *ctx) {
    sprite_status *boss = &actwk[0];
    sprite_status *child = 0;

    reset_boss1_state();
    boss->xposi.w.h = 1234;
    boss->yposi.w.h = 567;

    TEST_ASSERT_EQ_INT(ctx, 0, make_act(boss, &child));
    TEST_ASSERT_TRUE(ctx, child == &actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1234, child->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 567, child->yposi.w.h);

    actwkchk_fail_after = actwkchk_count;
    child = &actwk[2];

    TEST_ASSERT_EQ_INT(ctx, 1, make_act(boss, &child));
    TEST_ASSERT_TRUE(ctx, child == &actwk[2]);
}

static void test_egg1_make_act_builds_part_chain(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->xposi.w.h = 3026;
    boss->yposi.w.h = 120;

    egg1_make_act(boss);

    TEST_ASSERT_EQ_INT(ctx, 14, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 3, boss->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor_short_alias(boss, 26));
    TEST_ASSERT_EQ_INT(ctx, 8, actor_short_alias(boss, 27));
    TEST_ASSERT_EQ_INT(ctx, 43, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 44, actwk[2].actno);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[3].actno);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[4].actno);
    TEST_ASSERT_EQ_INT(ctx, 47, actwk[8].actno);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[9].actno);
    TEST_ASSERT_EQ_INT(ctx, 49, actwk[10].actno);
    TEST_ASSERT_TRUE(ctx, actwk[10].patbase == egg1arm3_pat1);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 49, actwk[13].actno);
    TEST_ASSERT_TRUE(ctx, actwk[13].patbase == egg1arm3_pat2);
    TEST_ASSERT_EQ_INT(ctx, 50, actwk[14].actno);
    TEST_ASSERT_EQ_INT(ctx, 252, boss->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->colicnt);
}

static void test_egg1_make_act_failure_cutoffs(test_context *ctx) {
    int allowed_successes;

    for (allowed_successes = 0; allowed_successes < 14; ++allowed_successes) {
        sprite_status *boss = &actwk[0];

        reset_boss1_state();
        boss->xposi.w.h = 3026;
        boss->yposi.w.h = 120;
        actwkchk_fail_after = allowed_successes;

        egg1_make_act(boss);

        TEST_ASSERT_EQ_INT(ctx, allowed_successes + 1, actwkchk_count);
    }
}

static void test_egg1_wait_spawns_after_sixty_ticks(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->actfree[1] = 59;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_wait(boss));

    TEST_ASSERT_EQ_INT(ctx, 4, boss->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3026, boss->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 120, boss->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 14, actwkchk_count);
}

static void test_egg1_scroll_gate_and_boss_start(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    actwk[0].xposi.w.h = 2700;
    scralim_left = 2500;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_01(boss));
    TEST_ASSERT_EQ_INT(ctx, 2540, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2540, scralim_n_left);
    TEST_ASSERT_EQ_INT(ctx, 2752, scralim_right);

    actwk[0].xposi.w.h = 2912;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_01(boss));
    TEST_ASSERT_EQ_INT(ctx, 12, boss->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2752, scralim_left);

    scra_vline = 196;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_06(boss));
    TEST_ASSERT_EQ_INT(ctx, 202, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 103, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 14, boss->r_no0);
}

static void test_boss1_main_gate_edges(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->xposi.w.h = 2600;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_01(boss));
    TEST_ASSERT_EQ_INT(ctx, 2752, scralim_right);

    reset_boss1_state();
    boss->xposi.w.h = 2700;
    scralim_left = 2600;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_01(boss));
    TEST_ASSERT_EQ_INT(ctx, 2600, scralim_left);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[1].actfree[2] = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_02(boss));
    TEST_ASSERT_EQ_INT(ctx, 0, boss->r_no1);

    actwk[1].actfree[2] = 1;
    boss->r_no1 = 16;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_02(boss));
    TEST_ASSERT_EQ_INT(ctx, 6, boss->r_no1);

    reset_boss1_state();
    boss->r_no1 = 1;
    boss->xposi.w.h = 3200;
    generate_flag = 1;

    TEST_ASSERT_EQ_INT(ctx, 0, egg1_04(boss));
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 16, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, genecolor_count);
}

static void test_egg1_coliude_deflects_player_and_rearms_collision(
    test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[0].xposi.w.h = 300;
    actwk[10].xposi.w.h = 200;

    egg1_coliude(10, boss);

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[10].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 1024, actwk[0].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 152, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 252, boss->colino);
}

static void test_egg1_03_transitions_to_escape_setup(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[1] = 93;
    boss->xposi.w.h = 3100;
    boss->yposi.w.h = 400;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_03(boss));

    TEST_ASSERT_EQ_INT(ctx, 94, boss->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 3, boss->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].actfree[2] & 1);

    boss->actfree[1] = 119;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_03(boss));

    TEST_ASSERT_EQ_INT(ctx, 8, boss->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 32, boss->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 32, boss->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 100, scoreup_value);
}

static void test_egg1_05_opens_scroll_and_frames_out_at_limit(
    test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    scr_dir_tbl[2] = 3735;
    scralim_right = 3700;
    scralim_n_right = 3700;

    TEST_ASSERT_EQ_INT(ctx, 0, egg1_05(boss));

    TEST_ASSERT_EQ_INT(ctx, 3706, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    scralim_right = 3734;
    scralim_n_right = 3734;

    TEST_ASSERT_EQ_INT(ctx, 0, egg1_05(boss));

    TEST_ASSERT_EQ_INT(ctx, 3735, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == boss);
}

static void test_egg1_04_rises_then_flies_away(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->xposi.w.h = 3000;
    boss->yposi.w.h = 346;
    boss->yspeed.w = 500;
    set_actor_short_alias(boss, 33, 3100);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_04(boss));

    TEST_ASSERT_EQ_INT(ctx, 1, boss->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 4, boss->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 64, boss->actfree[1]);

    generate_flag = 0;
    boss->xposi.w.h = 3199;
    boss->r_no1 = 1;
    boss->actfree[1] = 64;
    set_actor_short_alias(boss, 31, 8);
    sinset_sin = 256;
    sinset_cos = 0;
    scralim_right = 3735;
    scralim_n_right = 3735;

    TEST_ASSERT_EQ_INT(ctx, 0, egg1_04(boss));

    TEST_ASSERT_EQ_INT(ctx, 1, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, 67, sinset_angle);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 17, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, genecolor_count);
    TEST_ASSERT_EQ_INT(ctx, 0, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 0, bossstart);
}

static void test_boss1_entry_calls_patch_and_action(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->r_no0 = 0;

    egg1(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_TRUE(ctx, patchg_actor == boss);
    TEST_ASSERT_TRUE(ctx, patchg_table == egg1_pchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_TRUE(ctx, actionsub_actor == boss);
}

static void test_egg1_02_advances_body_action_table(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[1].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_02(boss));

    TEST_ASSERT_EQ_INT(ctx, 2, boss->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].actfree[2] & 1);

    actwk[1].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1_02(boss));

    TEST_ASSERT_EQ_INT(ctx, 4, boss->r_no1);
    TEST_ASSERT_EQ_INT(ctx, 3, boss->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 8, boss->actfree[2] & 8);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
}

static void test_boss1_part_wrappers_initialize_and_draw(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;

    egg1body(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == egg1body_pat);

    egg1leg1(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 2, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[2].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[2].patbase == egg1leg1_pat);

    egg1leg2(&actwk[3]);
    TEST_ASSERT_EQ_INT(ctx, 3, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[3].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[3].patbase == egg1leg2_pat);

    egg1leg3(&actwk[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[4].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[4].patbase == egg1leg3_pat);

    egg1arm1(&actwk[8]);
    TEST_ASSERT_EQ_INT(ctx, 5, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[8].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[8].patbase == egg1arm1_pat);

    sinset_sin = 32;
    sinset_cos = 64;
    egg1arm2(&actwk[9]);
    TEST_ASSERT_EQ_INT(ctx, 6, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[9].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[9].patbase == egg1arm2_pat);

    egg1arm3(&actwk[10]);
    TEST_ASSERT_EQ_INT(ctx, 7, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[10].r_no0);

    egg1arm4(&actwk[11]);
    TEST_ASSERT_EQ_INT(ctx, 8, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].r_no0);
    TEST_ASSERT_TRUE(ctx, actwk[11].patbase == egg1arm4_pat);
}

static void test_boss1_chain_flag_helpers_toggle_linked_parts(
    test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);

    bup_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 32, actwk[2].actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[3].actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[4].actfree[2] & 32);

    bup_clr(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[2].actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[3].actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[4].actfree[2] & 32);

    grd_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[2].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[3].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[4].actfree[2] & 16);

    grd_clr(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[2].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[3].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[4].actfree[2] & 16);
}

static void test_boss1_arm_and_bomb_motion_paths(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);

    actwk[8].actfree[2] = 64;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm1_01(&actwk[8]));

    TEST_ASSERT_EQ_INT(ctx, 10, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(&actwk[8], 26));

    actwk[8].actfree[2] = 2;
    actwk[8].yposi.w.h = 500;
    set_actor_long_alias(&actwk[8], 16, 0);
    set_actor_long_alias(&actwk[8], 4, 65536);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm1_02(&actwk[8]));
    TEST_ASSERT_TRUE(ctx, actor_long_alias(&actwk[8], 4) > 65536);

    actwk[8].yposi.w.h = 600;

    TEST_ASSERT_EQ_INT(ctx, 0, egg1arm1_02(&actwk[8]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[8]);

    reset_boss1_state();
    boss->xposi.w.h = 300;
    boss->yposi.w.h = 200;
    boss->actfree[1] = 4;

    bom_set(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 348, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 216, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);
}

static void test_boss1_leg2_and_leg3_position_helpers(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[2].xposi.w.h = 100;
    actwk[2].yposi.w.h = 50;
    set_actor_short_alias(&actwk[3], 25, 2);
    set_actor_short_alias(&actwk[3], 29, 3);
    set_actor_short_alias(&actwk[3], 31, -2);

    leg2_set2(&actwk[3]);

    TEST_ASSERT_EQ_INT(ctx, 107, actwk[3].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[3].yposi.w.h);

    set_actor_short_alias(&actwk[4], 25, 3);
    set_actor_short_alias(&actwk[4], 29, -8);
    set_actor_short_alias(&actwk[4], 31, 16);

    leg3_set_not_grd(&actwk[4]);

    TEST_ASSERT_EQ_INT(ctx, 88, actwk[4].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 94, actwk[4].yposi.w.h);

    emycol_d_result = 0;
    actwk[4].actfree[2] = 0;
    sub_sync_count = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_02(&actwk[4]));

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[4].actfree[2] & 16);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 126, sub_sync_requests[0]);
}

static void test_boss1_armset_variants_update_arm_modes(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);

    armset_1(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[12].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[13].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[10].r_no0);

    armset_2(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 6, actwk[12].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[13].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[10].r_no0);

    armset_3(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[12].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[13].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[13].patno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[10].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].patno);
}

static void test_boss1_body_early_states_cover_leg_handoffs(test_context *ctx) {
    sprite_status *boss = &actwk[0];
    sprite_status *body = &actwk[1];

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->yposi.l = 100 << 16;
    boss->yposi.l = 90 << 16;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_01(body));

    TEST_ASSERT_EQ_INT(ctx, 16, actwk[5].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[6].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[7].r_no0);
    TEST_ASSERT_EQ_INT(ctx, (100 << 16) + 98304, body->yposi.l);
    TEST_ASSERT_EQ_INT(ctx, (90 << 16) + 98304, boss->yposi.l);

    actwk[2].actfree[2] = 16;
    body->actfree[2] = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_01(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);

    reset_boss1_state();
    make_default_boss_parts(boss);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_02(body));
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 1);

    actwk[2].actfree[2] = 1;
    actwk[2].r_no0 = 12;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_02(body));
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[2].r_no0);

    actwk[2].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_02(body));
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[2].r_no0);

    actwk[2].actfree[2] = 1;
    actwk[2].r_no0 = 8;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_02(body));
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[2].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[5].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[6].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[7].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);
}

static void test_boss1_body_late_states_cover_walk_cycles(test_context *ctx) {
    sprite_status *boss = &actwk[0];
    sprite_status *body = &actwk[1];

    reset_boss1_state();
    make_default_boss_parts(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_03(body));
    TEST_ASSERT_EQ_INT(ctx, 2, body->actfree[2] & 2);

    actwk[2].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_03(body));
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 1);

    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_03(body));
    TEST_ASSERT_EQ_INT(ctx, 64, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);

    body->actfree[2] = 0;
    body->actfree[3] = 3;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));
    TEST_ASSERT_EQ_INT(ctx, 64, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[5].r_no0);

    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    body->actfree[3] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);

    body->actfree[2] = 64;
    body->actfree[3] = 3;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_05(body));
    TEST_ASSERT_EQ_INT(ctx, 2, body->actfree[2] & 2);
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 22, actwk[2].r_no0);

    body->actfree[2] = 2;
    body->actfree[3] = 1;
    body->xposi.w.h = 2976;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_05(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);

    body->actfree[2] = 0;
    body->actfree[3] = 3;
    actwk[2].r_no0 = 4;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_06(body));
    TEST_ASSERT_EQ_INT(ctx, 2, body->actfree[2] & 2);
    TEST_ASSERT_EQ_INT(ctx, 64, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[6].r_no0);

    body->actfree[2] = 66;
    body->actfree[3] = 1;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_06(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);
}

static void test_boss1_body_walk_alternate_branches(test_context *ctx) {
    sprite_status *boss = &actwk[0];
    sprite_status *body = &actwk[1];

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->actfree[2] = 0;
    actwk[2].actfree[2] = 0;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));

    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->actfree[2] = 64;
    body->actfree[3] = 3;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[2].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[5].r_no0);

    body->actfree[2] = 0;
    body->actfree[3] = 3;
    body->xposi.w.h = 2904;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_04(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->actfree[2] = 0;
    actwk[2].actfree[2] = 0;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_05(body));

    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_05(body));

    body->actfree[2] = 2;
    body->actfree[3] = 3;
    body->xposi.w.h = 2900;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_05(body));
    TEST_ASSERT_EQ_INT(ctx, 64, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 18, actwk[2].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 22, actwk[5].r_no0);

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->actfree[2] = 2;
    body->actfree[3] = 3;
    actwk[2].actfree[2] = 0;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_06(body));

    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_06(body));

    body->actfree[2] = 64 | 2;
    body->actfree[3] = 3;
    actwk[2].actfree[2] = 1;
    actwk[5].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_06(body));
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[5].r_no0);
}

static void test_boss1_body_finish_and_speed_helpers(test_context *ctx) {
    sprite_status *boss = &actwk[0];
    sprite_status *body = &actwk[1];

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->yposi.w.h = 100;
    boss->yposi.w.h = 90;
    emycol_d_result = 3;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_07(body));
    TEST_ASSERT_EQ_INT(ctx, 102, body->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 92, boss->yposi.w.h);

    body->actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1body_07(body));
    TEST_ASSERT_TRUE(ctx, frameout_actor == body);

    reset_boss1_state();
    make_default_boss_parts(boss);
    body->actfree[3] = 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_08(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1body_08(body));
    TEST_ASSERT_EQ_INT(ctx, 1, body->actfree[2] & 1);
    TEST_ASSERT_EQ_INT(ctx, 0, body->actfree[2] & 2);
}

static void test_boss1_remaining_arm_paths(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[9].actfree[0] = 20;
    sinset_sin = 128;
    sinset_cos = 64;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_02(&actwk[9]));
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_04(&actwk[9]));

    actwk[9].actfree[0] = 46;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_03(&actwk[9]));
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[9].actfree[0]);

    actwk[9].actfree[2] = 0;
    actwk[9].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_05(&actwk[9]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[10].r_no0);
    actwk[9].yposi.l = 600 << 16;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1arm2_05(&actwk[9]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[9]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[10].xposi.w.h = 320;
    actwk[10].yposi.w.h = 120;
    set_actor_short_alias(&actwk[10], 29, 4);
    set_actor_short_alias(&actwk[10], 31, 0);
    TEST_ASSERT_EQ_INT(ctx, 262144, actor_long_alias(&actwk[10], 14));
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_03(&actwk[10]));
    TEST_ASSERT_TRUE(ctx, actor_long_alias(&actwk[10], 14) < 262144);

    set_actor_short_alias(&actwk[10], 29, 4);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_04(&actwk[10]));
    TEST_ASSERT_EQ_INT(ctx, 327680, actor_long_alias(&actwk[10], 14));

    set_actor_short_alias(&actwk[10], 29, 8);
    set_actor_short_alias(&actwk[10], 31, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_05(&actwk[10]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].patno);

    actwk[10].actfree[2] = 0;
    actwk[10].xposi.w.h = 300;
    actwk[10].yposi.w.h = 100;
    next_alloc_index = 20;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_02(&actwk[10]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[11].r_no0);
    set_actor_short_alias(&actwk[10], 23, 6);
    actwk[10].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_02(&actwk[10]));
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[20].actno);

    actwk[10].yposi.w.h = 576;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1arm3_02(&actwk[10]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[10]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[10].patno = 1;
    actwk[11].actfree[2] = 16;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm4_01(&actwk[11]));
    TEST_ASSERT_EQ_INT(ctx, actwk[10].xposi.w.h, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[11].actfree[2] & 16);

    actwk[11].actfree[2] = 0;
    actwk[11].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm4_02(&actwk[11]));
    actwk[11].yposi.l = 600 << 16;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1arm4_02(&actwk[11]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[11]);
}

static void test_boss1_arm_branch_edges(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[8].actfree[2] = 32;
    actwk[12].r_no0 = 2;
    actwk[12].patno = 3;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm1_01(&actwk[8]));
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[12].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor_short_alias(&actwk[8], 27));
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[12].patno);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[9].actfree[0] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_01(&actwk[9]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[9].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[9].actfree[2] & 1);

    actwk[9].actfree[0] = 20;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm2_03(&actwk[9]));
    TEST_ASSERT_EQ_INT(ctx, 22, actwk[9].actfree[0]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    set_actor_short_alias(&actwk[10], 29, 4);
    set_actor_short_alias(&actwk[10], 31, 4);
    set_actor_long_alias(&actwk[10], 14, 0);
    set_actor_long_alias(&actwk[10], 15, 0);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm3_05(&actwk[10]));
    TEST_ASSERT_EQ_INT(ctx, 32768, actor_long_alias(&actwk[10], 14));
    TEST_ASSERT_EQ_INT(ctx, 65536, actor_long_alias(&actwk[10], 15));

    actwk[9].actfree[2] = 4;
    actwk[8].xposi.w.h = 500;
    actwk[8].yposi.w.h = 100;
    actwk[9].actfree[0] = 0;
    sinset_sin = 160;
    sinset_cos = 320;
    egg1arm2_set(&actwk[9]);
    TEST_ASSERT_EQ_INT(ctx, 510, actwk[9].xposi.w.h);

    actwk[10].patno = 2;
    actwk[11].actfree[2] = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm4_01(&actwk[11]));
    TEST_ASSERT_EQ_INT(ctx, actwk[10].xposi.w.h - 4, actwk[11].xposi.w.h);

    actwk[8].actfree[2] = 0;
    actwk[8].xposi.l = 0;
    actwk[8].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1arm1_02(&actwk[8]));
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[8].actfree[2] & 2);
    TEST_ASSERT_EQ_INT(ctx, -65536, actor_long_alias(&actwk[8], 16));
    TEST_ASSERT_EQ_INT(ctx, -0x20000, actor_long_alias(&actwk[8], 4));
}

static void test_boss1_remaining_leg_position_paths(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[4].r_no0 = 2;
    egg1leg1_01(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[4].r_no0);

    actwk[2].actfree[0] = 80;
    actwk[2].actfree[21] = 8;
    egg1leg1_02(&actwk[2]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[2].r_no0);

    actwk[4].actfree[2] = 16;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_03(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].actfree[0] = 32;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_04(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[2].actfree[0]);

    {
        Sint16 old_x = actwk[2].xposi.w.h;
        TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_08(&actwk[2]));
        TEST_ASSERT_EQ_INT(ctx, old_x - 10, actwk[2].xposi.w.h);
    }

    actwk[2].r_no0 = 18;
    actwk[2].actfree[0] = 8;
    actwk[4].r_no0 = 8;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_09(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[2].r_no0);

    actwk[2].actfree[0] = 40;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_10(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[2].r_no0);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_01(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, actwk[2].xposi.w.h + 4, actwk[3].xposi.w.h);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_02(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 3032, actwk[3].xposi.w.h);

    set_actor_short_alias(&actwk[3], 29, 0);
    actwk[3].actfree[2] = 16 | 32;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_03(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_04(&actwk[3]));

    set_actor_short_alias(&actwk[3], 29, 0);
    set_actor_short_alias(&actwk[3], 31, 0);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_05(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_06(&actwk[3]));
}

static void test_boss1_leg_motion_branch_edges(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[2].actfree[21] = 8;
    actwk[2].actfree[0] = 25;
    actwk[4].r_no0 = 6;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_04(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[2].actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].actfree[0] = 0;
    actwk[4].r_no0 = 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_05(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[4].r_no0);

    actwk[4].r_no0 = 8;
    actwk[4].actfree[2] = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_05(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[4].actfree[2] & 1);

    actwk[4].r_no0 = 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_06(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[4].r_no0);

    actwk[4].r_no0 = 4;
    actwk[2].actfree[0] = 88;
    actwk[4].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_06(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[0] = 16;
    actwk[4].r_no0 = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_09(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[4].r_no0);

    actwk[4].r_no0 = 8;
    actwk[2].actfree[0] = 32;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_09(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[2].r_no0);

    actwk[2].actfree[0] = 16;
    actwk[2].actfree[21] = 8;
    actwk[4].r_no0 = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_01(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[2].actfree[0]);

    actwk[2].actfree[0] = 0;
    actwk[4].r_no0 = 6;
    actwk[4].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_01(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[2].r_no0);

    actwk[2].actfree[6] = 3;
    actwk[3].actfree[6] = 4;
    actwk[4].r_no0 = 6;
    actwk[2].actfree[2] = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_11(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[4].r_no0);

    actwk[3].actfree[2] = 0;
    set_actor_short_alias(&actwk[3], 29, 12);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_03(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 3054, actwk[3].xposi.w.h);

    set_actor_short_alias(&actwk[3], 29, -12);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_04(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 3030, actwk[3].xposi.w.h);

    set_actor_short_alias(&actwk[3], 29, -12);
    set_actor_short_alias(&actwk[3], 31, -8);
    actwk[3].actfree[2] = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_05(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 3030, actwk[3].xposi.w.h);

    set_actor_short_alias(&actwk[3], 29, -12);
    set_actor_short_alias(&actwk[3], 31, 8);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_06(&actwk[3]));
    TEST_ASSERT_EQ_INT(ctx, 3030, actwk[3].xposi.w.h);

    actwk[2].actfree[2] = 16;
    actwk[2].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_12(&actwk[2]));
    actwk[2].actfree[2] = 16 | 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_12(&actwk[2]));

    actwk[3].actfree[2] = 16;
    actwk[3].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_07(&actwk[3]));
    actwk[3].actfree[2] = 16 | 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_07(&actwk[3]));
}

static void test_boss1_leg1_set_positioning_modes(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 50;
    actwk[1].xposi.w.h = 1000;
    actwk[1].yposi.w.h = 200;
    actwk[2].actfree[0] = 12;
    actwk[2].actfree[2] = 16;
    sinset_sin = 160;
    sinset_cos = 320;

    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 1032, actwk[2].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 230, actwk[2].yposi.w.h);

    actwk[2].actfree[2] = 16 | 4;
    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 1022, actwk[2].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 230, actwk[2].yposi.w.h);

    actwk[2].actfree[2] = 16 | 32;
    actwk[1].xposi.w.h = 1000;
    actwk[1].yposi.w.h = 200;
    boss->xposi.w.h = 500;
    boss->yposi.w.h = 50;
    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 2, actwk[2].actfree[2] & 2);
    TEST_ASSERT_EQ_INT(ctx, 1000, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[1].yposi.w.h);

    sinset_sin = 320;
    sinset_cos = 160;
    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 1010, actwk[1].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 190, actwk[1].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 510, boss->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 40, boss->yposi.w.h);

    actwk[2].actfree[2] = 0;
    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 1032, actwk[2].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 230, actwk[2].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[2].actfree[2] & 2);

    actwk[2].actfree[2] = 4;
    egg1leg1_set(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 1022, actwk[2].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 230, actwk[2].yposi.w.h);
}

static void test_boss1_remaining_leg3_paths(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    emycol_d_result = 0;
    actwk[4].actfree[2] = 16 | 128;
    set_actor_short_alias(&actwk[4], 31, 10);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_01(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[4].actfree[2] & 16);

    actwk[4].actfree[2] = 16 | 128;
    set_actor_short_alias(&actwk[4], 31, 20);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_01(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[4].r_no0);

    actwk[4].actfree[2] = 0;
    set_actor_short_alias(&actwk[4], 31, 10);
    emycol_d_result = 0;
    sub_sync_count = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_01(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 126, sub_sync_requests[0]);

    actwk[4].actfree[2] = 0;
    set_actor_short_alias(&actwk[4], 31, 20);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_01(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[4].r_no0);

    set_actor_long_alias(&actwk[4], 15, -1);
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_03(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[4].r_no0);

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_04(&actwk[4]));

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_05(&actwk[4]));
    TEST_ASSERT_EQ_INT(ctx, 3005, actwk[4].xposi.w.h);

    actwk[4].actfree[2] = 16 | 32;
    set_actor_long_alias(&actwk[4], 16, 65536);
    set_actor_long_alias(&actwk[4], 4, 65536);
    actwk[4].r_no0 = 6;
    boss->xposi.l = 100 << 16;
    leg3_set(&actwk[4]);
    TEST_ASSERT_TRUE(ctx, boss->xposi.l < (100 << 16));

    actwk[4].r_no0 = 8;
    boss->xposi.l = 100 << 16;
    leg3_set(&actwk[4]);
    TEST_ASSERT_TRUE(ctx, boss->xposi.l > (100 << 16));

    actwk[4].actfree[2] = 0;
    actwk[4].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_06(&actwk[4]));
    actwk[4].yposi.l = 600 << 16;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1leg3_06(&actwk[4]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[4]);

    actwk[4].actfree[2] = 16;
    actwk[4].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_06(&actwk[4]));
    actwk[4].actfree[2] = 16 | 2;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg3_06(&actwk[4]));
}

static void test_boss1_bomb_set_skips_between_spawn_ticks(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    boss->actfree[1] = 1;

    bom_set(boss);

    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);
}

static void test_boss1_collision_damage_paths(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[1] = 3;
    boss->actfree[2] = 8;
    boss->xposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[4].xposi.w.h = 300;

    egg1_coli(4, boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 120, boss->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 32, actwk[8].actfree[2] & 32);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[1] = 2;
    egg1_coli(4, boss);
    TEST_ASSERT_EQ_INT(ctx, 1, boss->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[8].actfree[2] & 64);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[1] = 1;
    egg1_coli(4, boss);
    TEST_ASSERT_EQ_INT(ctx, 6, boss->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->colino);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[2].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 14, actwk[3].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 12, actwk[7].r_no0);
}

static void test_boss1_hit_check_inactive_phase_returns(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 1;
    set_all_boss1_collision_slots(1);

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 1, boss->actfree[1]);
}

static void test_boss1_hit_check_searches_collision_slots(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);

    egg1_hit_chk(boss);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[10]);

    boss->actfree[2] = 8;
    boss->colino = 0;
    boss->actfree[1] = 3;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[10] = 1;
    boss->actfree[1] = 1;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 252, boss->colino);
}

static void test_boss1_hit_check_finds_each_part_slot(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[4].colino = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[3].colino = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[6].colino = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[7].colino = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 20, boss->actfree[10]);
    TEST_ASSERT_EQ_INT(ctx, 2, boss->actfree[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[14].colino = 0;
    soundset_count = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[14].actfree[2] & 16);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 3;
    set_all_boss1_collision_slots(1);
    actwk[11].colino = 0;
    soundset_count = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[11].actfree[2] & 16);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 2;
    set_all_boss1_collision_slots(1);
    actwk[11].colino = 0;
    soundset_count = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 16, actwk[11].actfree[2] & 16);

    reset_boss1_state();
    make_default_boss_parts(boss);
    boss->actfree[2] = 8;
    boss->actfree[1] = 2;
    set_all_boss1_collision_slots(1);
    soundset_count = 0;

    egg1_hit_chk(boss);

    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 0, boss->actfree[10]);
}

static void test_boss1_remaining_leg1_action_variants(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[4].r_no0 = 8;
    actwk[4].actfree[2] = 1;
    actwk[2].actfree[0] = 0;
    actwk[2].actfree[21] = 8;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_05(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].actfree[0] = 88;
    actwk[4].r_no0 = 4;
    actwk[4].actfree[2] = 1;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_06(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].actfree[0] = 8;
    actwk[4].r_no0 = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_07(&actwk[2]));

    actwk[2].actfree[0] = 40;
    actwk[4].r_no0 = 8;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_07(&actwk[2]));

    actwk[2].actfree[0] = 24;
    actwk[4].actfree[2] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_07(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].actfree[6] = 30;
    actwk[2].actfree[0] = 72;
    actwk[2].actfree[21] = 8;
    actwk[30].actfree[6] = 31;
    actwk[31].r_no0 = 4;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_11(&actwk[2]));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[2].actfree[2] & 1);

    actwk[2].actfree[2] = 0;
    actwk[2].yposi.l = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg1_12(&actwk[2]));
    actwk[2].yposi.l = 600 << 16;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1leg1_12(&actwk[2]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[2]);
}

static void test_boss1_remaining_leg2_flight_variant(test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actwk[3].actfree[2] = 0;
    actwk[3].yposi.l = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, egg1leg2_07(&actwk[3]));
    actwk[3].yposi.l = 600 << 16;
    TEST_ASSERT_EQ_INT(ctx, 0, egg1leg2_07(&actwk[3]));
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[3]);
}

static void test_boss1_wrappers_skip_action_when_state_returns_zero(
    test_context *ctx) {
    sprite_status *boss = &actwk[0];

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[1].r_no0 = 14;
    actwk[1].actfree[2] = 1;

    egg1body(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[1]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[8].r_no0 = 4;
    actwk[8].actfree[2] = 2;
    actwk[8].yposi.l = 600 << 16;

    egg1arm1(&actwk[8]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[8]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[9].r_no0 = 10;
    actwk[9].actfree[2] = 2;
    actwk[9].yposi.l = 600 << 16;

    egg1arm2(&actwk[9]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[9]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[10].r_no0 = 4;
    actwk[10].actfree[2] = 2;
    actwk[10].yposi.l = 600 << 16;

    egg1arm3(&actwk[10]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[10]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[11].r_no0 = 4;
    actwk[11].actfree[2] = 2;
    actwk[11].yposi.l = 600 << 16;

    egg1arm4(&actwk[11]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[11]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[2].r_no0 = 24;
    actwk[2].actfree[2] = 2;
    actwk[2].yposi.l = 600 << 16;

    egg1leg1(&actwk[2]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[2]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[3].r_no0 = 14;
    actwk[3].actfree[2] = 2;
    actwk[3].yposi.l = 600 << 16;

    egg1leg2(&actwk[3]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[3]);

    reset_boss1_state();
    make_default_boss_parts(boss);
    actionsub_count = 0;
    actwk[4].r_no0 = 12;
    actwk[4].actfree[2] = 2;
    actwk[4].yposi.l = 600 << 16;

    egg1leg3(&actwk[4]);

    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[4]);
}

TEST_MAIN_BEGIN;
    test_egg1_ini_sets_startup_state(&ctx);
    test_egg1_anime_counts_down_and_resets_pattern(&ctx);
    test_egg1_jisin_sets_limits_and_shake(&ctx);
    test_sonic_hajiku_uses_collision_side_and_air_state(&ctx);
    test_egg1_warai_chk_starts_laugh_animation(&ctx);
    test_make_act_success_and_failure(&ctx);
    test_egg1_make_act_builds_part_chain(&ctx);
    test_egg1_make_act_failure_cutoffs(&ctx);
    test_egg1_wait_spawns_after_sixty_ticks(&ctx);
    test_egg1_scroll_gate_and_boss_start(&ctx);
    test_boss1_main_gate_edges(&ctx);
    test_egg1_coliude_deflects_player_and_rearms_collision(&ctx);
    test_egg1_03_transitions_to_escape_setup(&ctx);
    test_egg1_05_opens_scroll_and_frames_out_at_limit(&ctx);
    test_egg1_04_rises_then_flies_away(&ctx);
    test_boss1_entry_calls_patch_and_action(&ctx);
    test_egg1_02_advances_body_action_table(&ctx);
    test_boss1_part_wrappers_initialize_and_draw(&ctx);
    test_boss1_chain_flag_helpers_toggle_linked_parts(&ctx);
    test_boss1_arm_and_bomb_motion_paths(&ctx);
    test_boss1_leg2_and_leg3_position_helpers(&ctx);
    test_boss1_armset_variants_update_arm_modes(&ctx);
    test_boss1_body_early_states_cover_leg_handoffs(&ctx);
    test_boss1_body_late_states_cover_walk_cycles(&ctx);
    test_boss1_body_walk_alternate_branches(&ctx);
    test_boss1_body_finish_and_speed_helpers(&ctx);
    test_boss1_remaining_arm_paths(&ctx);
    test_boss1_arm_branch_edges(&ctx);
    test_boss1_remaining_leg_position_paths(&ctx);
    test_boss1_leg_motion_branch_edges(&ctx);
    test_boss1_leg1_set_positioning_modes(&ctx);
    test_boss1_remaining_leg3_paths(&ctx);
    test_boss1_bomb_set_skips_between_spawn_ticks(&ctx);
    test_boss1_collision_damage_paths(&ctx);
    test_boss1_hit_check_inactive_phase_returns(&ctx);
    test_boss1_hit_check_searches_collision_slots(&ctx);
    test_boss1_hit_check_finds_each_part_slot(&ctx);
    test_boss1_remaining_leg1_action_variants(&ctx);
    test_boss1_remaining_leg2_flight_variant(&ctx);
    test_boss1_wrappers_skip_action_when_state_returns_zero(&ctx);
TEST_MAIN_END

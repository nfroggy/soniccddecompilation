#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

sprite_status actwk[128];
sprite_pattern *bakupat[7];
Uint16 scr_dir_tbl[6];
Uint8 bossflag;
Uint8 generate_flag;
Uint8 time_flag;
Uint8 plpower_m;
Uint8 plpower_s;
Sint16 plring;
Sint16 scra_hline;
Sint16 scralim_down;
Sint16 scralim_n_down;
Sint16 scralim_left;
Sint16 scralim_right;
Sint16 scralim_n_left;
Sint16 scralim_n_right;
Uint8 bossstart;
short_union stageno;
int_union scra_h_posit;
int_union scra_v_posit;
int_union scrb_h_posit;
int_union scrb_v_posit;
int_union scrc_h_posit;
int_union scrc_v_posit;
int_union scrz_h_posit;
int_union scrz_v_posit;
static PALETTEENTRY colorwk[64];
static PALETTEENTRY colorwk3[64];
PALETTEENTRY *lpcolorwk = colorwk;
PALETTEENTRY *lpcolorwk3 = colorwk3;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int soundset_count;
static Sint16 soundset_values[8];
static int frameout_count;
static sprite_status *frameout_actor;
static int actwkchk_count;
static int actwkchk2_count;
static sprite_status *spawn_queue[16];
static int spawn_queue_count;
static int spawn_queue_index;
static Sint32 spawn_result_after;
static Sint32 random_values[16];
static int random_count;
static int random_index;
static Sint16 emycol_d_result;
static int sub_sync_count;
static Sint16 sub_sync_values[8];
static int goal_count;
static int clchgctr_count;
static int scoreup_count;
static int genecolor_count;
static int playdieset_count;

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void soundset(Sint16 ReqNo);
void frameout(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk);
Sint32 random(void);
Sint16 emycol_d(sprite_status *pActwk);
void sub_sync(Sint16 ReqNo);
void goal(sprite_status *pActwk);
void genecolor(void);
Sint16 playdieset(sprite_status *pActwk);
void clchgctr(void);
void scoreup(Uint32 lScore);
void colorset2(Sint32 ColorNo);
void flower_set(void);
void fadeout_s(void);
void DLL_meminit(char ***pBufTbl, void **pFuncTbl);
void int_union_add(int_union *p, Sint32 add);

#include "src/r7/boss_7.c"

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
    if (soundset_count < 8)
        soundset_values[soundset_count] = ReqNo;
    ++soundset_count;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
    pActwk->actno = 0;
}

Sint32 actwkchk(sprite_status **ppActwk) {
    ++actwkchk_count;
    if (spawn_queue_index >= spawn_queue_count || spawn_result_after == 0) {
        *ppActwk = 0;
        return 1;
    }
    if (spawn_result_after > 0)
        --spawn_result_after;
    *ppActwk = spawn_queue[spawn_queue_index++];
    return 0;
}

Sint32 actwkchk2(sprite_status *pActwk, sprite_status **ppNewActwk) {
    (void)pActwk;
    ++actwkchk2_count;
    if (spawn_queue_index >= spawn_queue_count || spawn_result_after == 0) {
        *ppNewActwk = 0;
        return 1;
    }
    if (spawn_result_after > 0)
        --spawn_result_after;
    *ppNewActwk = spawn_queue[spawn_queue_index++];
    return 0;
}

Sint32 random(void) {
    if (random_index < random_count)
        return random_values[random_index++];
    return 0;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    return emycol_d_result;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 8)
        sub_sync_values[sub_sync_count] = ReqNo;
    ++sub_sync_count;
}

void goal(sprite_status *pActwk) {
    (void)pActwk;
    ++goal_count;
}

void genecolor(void) { ++genecolor_count; }
Sint16 playdieset(sprite_status *pActwk) {
    (void)pActwk;
    ++playdieset_count;
    return -1;
}
void clchgctr(void) { ++clchgctr_count; }
void scoreup(Uint32 lScore) {
    (void)lScore;
    ++scoreup_count;
}
void colorset2(Sint32 ColorNo) { (void)ColorNo; }
void flower_set(void) {}
void fadeout_s(void) {}
void DLL_meminit(char ***pBufTbl, void **pFuncTbl) {
    (void)pBufTbl;
    (void)pFuncTbl;
}
void int_union_add(int_union *p, Sint32 add) { p->l += add; }

static void set_word_index(sprite_status *actor, int index, Sint16 value) {
    int offset = (index - 23) * 2;
    actor->actfree[offset] = (Uint8)value;
    actor->actfree[offset + 1] = (Uint8)((Uint16)value >> 8);
}

static Sint16 get_word_index(sprite_status *actor, int index) {
    int offset = (index - 23) * 2;
    return (Sint16)((Uint16)actor->actfree[offset] |
                    ((Uint16)actor->actfree[offset + 1] << 8));
}

static void queue_spawn(sprite_status *actor) {
    spawn_queue[spawn_queue_count++] = actor;
}

static void queue_random(Sint32 value) {
    random_values[random_count++] = value;
}

static void reset_boss7_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(bakupat, 0, sizeof(bakupat));
    memset(scr_dir_tbl, 0, sizeof(scr_dir_tbl));
    bossflag = 0;
    generate_flag = 0;
    time_flag = 0;
    plpower_m = 0;
    plpower_s = 0;
    plring = 0;
    scra_hline = 0;
    scralim_down = scralim_n_down = 0;
    scralim_left = scralim_right = 0;
    scralim_n_left = scralim_n_right = 0;
    bossstart = 0;
    stageno.w = 0;
    scra_h_posit.l = scra_v_posit.l = 0;
    scrb_h_posit.l = scrb_v_posit.l = 0;
    scrc_h_posit.l = scrc_v_posit.l = 0;
    scrz_h_posit.l = scrz_v_posit.l = 0;
    memset(colorwk, 0, sizeof(colorwk));
    memset(colorwk3, 0, sizeof(colorwk3));
    lpcolorwk = colorwk;
    lpcolorwk3 = colorwk3;
    pPlayerwk = 0;
    pEggman = 0;
    pMsnc = 0;
    GL_d5 = 0;
    QuickReturn = 0;
    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    soundset_count = 0;
    memset(soundset_values, 0, sizeof(soundset_values));
    frameout_count = 0;
    frameout_actor = 0;
    actwkchk_count = 0;
    actwkchk2_count = 0;
    memset(spawn_queue, 0, sizeof(spawn_queue));
    spawn_queue_count = 0;
    spawn_queue_index = 0;
    spawn_result_after = -1;
    memset(random_values, 0, sizeof(random_values));
    random_count = 0;
    random_index = 0;
    emycol_d_result = 0;
    sub_sync_count = 0;
    memset(sub_sync_values, 0, sizeof(sub_sync_values));
    goal_count = 0;
    clchgctr_count = 0;
    scoreup_count = 0;
    genecolor_count = 0;
    playdieset_count = 0;
}

static void test_boss7_speed_helpers_use_indexed_actfree_words(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    actor->xposi.l = 0;
    actor->yposi.l = 0;
    actor->xspeed.w = 1;
    actor->yspeed.w = 2;
    set_word_index(actor, 26, 10);
    set_word_index(actor, 27, -5);

    hari_spdadd(actor);

    TEST_ASSERT_EQ_INT(ctx, 11, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, -3, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 2816, actor->xposi.l);
    TEST_ASSERT_EQ_INT(ctx, -768, actor->yposi.l);

    reset_boss7_state();
    actor->xspeed.w = 100;
    set_word_index(actor, 26, 10);
    set_word_index(actor, 27, 5);
    set_word_index(actor, 28, 105);

    add_spd3(actor);

    TEST_ASSERT_EQ_INT(ctx, 105, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->yspeed.w);

    reset_boss7_state();
    actor->xspeed.w = -100;
    set_word_index(actor, 26, -10);
    set_word_index(actor, 27, 5);
    set_word_index(actor, 28, -105);

    add_spd3(actor);

    TEST_ASSERT_EQ_INT(ctx, -105, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->yspeed.w);
}

static void timer_hit(sprite_status *actor) { actor->actfree[12] = 99; }

static void test_boss7_timer_helpers_advance_event_state(test_context *ctx) {
    sprite_status *actor = &actwk[4];
    Sint16 timers[2] = {2, 4};
    void (*events[2])(sprite_status *) = {timer_hit, timer_hit};

    reset_boss7_state();
    set_word_index(actor, 23, 1);

    w_timer(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);

    reset_boss7_state();
    actor->actfree[1] = 1;

    b_timer(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);

    reset_boss7_state();
    set_word_index(actor, 23, 0);

    event_timer(actor, timers, events);

    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[12]);

    event_timer(actor, timers, events);

    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[12]);

    event_timer(actor, timers, events);

    TEST_ASSERT_EQ_INT(ctx, 3, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 99, actor->actfree[12]);
}

static void test_boss7_spawn_helpers_create_child_actors(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    queue_spawn(&actwk[10]);

    make_ele(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, get_word_index(&actwk[10], 25));

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    queue_spawn(&actwk[11]);

    make_msnc(actor);

    TEST_ASSERT_EQ_INT(ctx, 47, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 11, get_word_index(actor, 25));
    TEST_ASSERT_EQ_INT(ctx, 3120, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 461, actwk[11].yposi.w.h);

    reset_boss7_state();
    queue_spawn(&actwk[12]);
    queue_spawn(&actwk[13]);

    make_jet(actor);

    TEST_ASSERT_EQ_INT(ctx, 45, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[12].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[13].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[13].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[13].mstno.b.h);
}

static void test_boss7_effect_spawn_helpers_capture_positions(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    queue_spawn(&actwk[10]);

    make_kemuri(actor);

    TEST_ASSERT_EQ_INT(ctx, 46, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[10].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(&actwk[10], 29));
    TEST_ASSERT_EQ_INT(ctx, 16, get_word_index(&actwk[10], 30));

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    queue_spawn(&actwk[11]);

    make_bakuha(actor);

    TEST_ASSERT_EQ_INT(ctx, 46, actwk[11].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[11].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 300, actwk[11].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[11].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_values[0]);

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    set_word_index(actor, 31, 20);
    actwk[20].xspeed.w = -700;
    queue_spawn(&actwk[12]);

    make_hahen(actor);

    TEST_ASSERT_EQ_INT(ctx, 55, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 302, actwk[12].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 416, actwk[12].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -700, actwk[12].xspeed.w);
}

static void test_boss7_beam_and_bara_spawn_sequences(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    queue_spawn(&actwk[10]);
    queue_spawn(&actwk[11]);
    queue_spawn(&actwk[12]);
    queue_spawn(&actwk[13]);

    make_beam(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(&actwk[10], 29));
    TEST_ASSERT_EQ_INT(ctx, 52, get_word_index(&actwk[10], 30));
    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(&actwk[11], 29));
    TEST_ASSERT_EQ_INT(ctx, 32, get_word_index(&actwk[11], 30));
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[13].userflag.b.l);

    reset_boss7_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    for (int i = 0; i < 10; ++i) {
        queue_spawn(&actwk[20 + i]);
        queue_random(1000 + i);
    }

    make_bara(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 49, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[20].patno);
    TEST_ASSERT_EQ_INT(ctx, 100, actwk[20].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, actwk[20].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, actwk[20].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 49, actwk[29].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[29].patno);
}

static void test_boss7_public_initializers_set_render_state(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    set_word_index(actor, 25, 5);
    actwk[5].actfree[15] = 128;

    msnc(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 20, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 892, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == msnc_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss7_state();
    actwk[3].mstno.b.h = 10;
    actwk[3].xposi.w.h = 1234;
    actwk[3].yposi.w.h = 456;
    set_word_index(actor, 25, 3);

    msnc_ele(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 24, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 1234, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 456, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
}

static void test_boss7_egg_init_spawns_race_actors(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    for (int i = 0; i < 7; ++i)
        queue_spawn(&actwk[10 + i]);

    egg7(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 36, actor->sprhsize);
    TEST_ASSERT_EQ_INT(ctx, 36, actor->sprvsize);
    TEST_ASSERT_EQ_INT(ctx, 768, actor->sproffset);
    TEST_ASSERT_TRUE(ctx, actor->patbase == egg7_pat);
    TEST_ASSERT_EQ_INT(ctx, 7, bossflag);
    TEST_ASSERT_EQ_INT(ctx, 7, bossstart);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 45, actwk[10].actno);
    TEST_ASSERT_EQ_INT(ctx, 47, actwk[12].actno);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[13].actno);
}

static void test_boss7_beam_public_paths_follow_parent_state(
    test_context *ctx) {
    sprite_status *beam = &actwk[8];
    sprite_status *parent = &actwk[4];

    reset_boss7_state();
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == beam);

    reset_boss7_state();
    parent->actno = 1;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 2, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, beam->actflg);
    TEST_ASSERT_EQ_INT(ctx, 2, beam->sprpri);
    TEST_ASSERT_EQ_INT(ctx, 8, beam->sprhsize);
    TEST_ASSERT_TRUE(ctx, beam->patbase == egg7beam_pat);

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 32 | 64;
    parent->cddat = 1;
    parent->xposi.w.h = 500;
    parent->yposi.w.h = 600;
    beam->r_no0 = 2;
    beam->userflag.b.l = 1;
    set_word_index(beam, 25, 4);
    set_word_index(beam, 29, 12);
    set_word_index(beam, 30, -3);
    queue_spawn(&actwk[20]);
    queue_spawn(&actwk[21]);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 190, beam->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, beam->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 488, beam->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 597, beam->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32 | 64, beam->actfree[2] & (32 | 64));
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 55, actwk[21].actno);
}

static void test_boss7_jet_and_fragment_public_paths(test_context *ctx) {
    sprite_status *actor = &actwk[8];
    sprite_status *parent = &actwk[4];

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 8;
    parent->cddat = 1;
    parent->xposi.w.h = 500;
    parent->yposi.w.h = 600;
    set_word_index(actor, 25, 4);

    egg7jet(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 551, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 626, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_boss7_state();
    actor->xposi.w.h = 100;
    actor->yposi.w.h = 200;
    actor->xspeed.w = 50;
    queue_random(0x1234);

    hahen73(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->r_no0 & 1);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->sprpri);
    TEST_ASSERT_EQ_INT(ctx, -384, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0x1234 % 448 + 50, actor->xspeed.w);

    scra_v_posit.w.h = 0;
    actor->yposi.w.h = 210;

    hahen73(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);
}

static void test_boss7_bara_public_path_actions_or_frames_out(
    test_context *ctx) {
    sprite_status *actor = &actwk[8];

    reset_boss7_state();
    actor->yposi.w.h = 100;
    actor->yspeed.w = 1;

    msnc_bara(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->actflg);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->sprpri);
    TEST_ASSERT_TRUE(ctx, actor->patbase == msnc_bara_pat);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss7_state();
    actor->r_no0 = 2;
    actor->yposi.w.h = 624;

    msnc_bara(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_boss7_egg_demo_transition_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 3040;

    snc_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, -1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 2880, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2880, scralim_n_left);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 103, sub_sync_values[0]);

    reset_boss7_state();
    actor->xposi.w.h = 2800;
    actor->cddat = 1;

    r_l(actor);

    TEST_ASSERT_EQ_INT(ctx, 2840, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 120, get_word_index(actor, 23));

    reset_boss7_state();
    actor->xposi.w.h = 3300;

    l_r(actor);

    TEST_ASSERT_EQ_INT(ctx, 3240, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat & 1);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 120, get_word_index(actor, 23));

    reset_boss7_state();
    actor->xposi.w.h = 3100;
    actor->actfree[2] = 8;

    r_l2(actor);

    TEST_ASSERT_EQ_INT(ctx, 3104, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 8);
}

static void test_boss7_beam_flag_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();

    beam_on(actor);

    TEST_ASSERT_EQ_INT(ctx, 32 | 64, actor->actfree[2] & (32 | 64));

    beam_on2(actor);

    TEST_ASSERT_EQ_INT(ctx, 64, actor->actfree[2] & (32 | 64));

    beam_off(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & (32 | 64));

    move_start(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->actfree[2] & 8);
    TEST_ASSERT_EQ_INT(ctx, -512, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 26));

    actor->cddat = 1;
    beam_next(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 16 | 64, actor->actfree[2] & (16 | 32 | 64));
    TEST_ASSERT_EQ_INT(ctx, 512, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->cddat & 1);
}

static void test_boss7_goal_transition_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pEggman = &actwk[5];
    pPlayerwk = &actwk[0];
    actor->actno = 47;
    actor->xposi.w.h = 16000;
    queue_spawn(&actwk[20]);
    queue_spawn(&actwk[21]);
    queue_spawn(&actwk[22]);
    queue_spawn(&actwk[23]);
    queue_spawn(&actwk[24]);
    queue_spawn(&actwk[25]);
    queue_spawn(&actwk[26]);
    queue_spawn(&actwk[27]);
    queue_spawn(&actwk[28]);
    queue_spawn(&actwk[29]);

    goal_chk_snc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 129, pEggman->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 128, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 187, soundset_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 15944, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 10, actwkchk2_count);

    reset_boss7_state();
    pEggman = &actwk[5];
    pPlayerwk = &actwk[0];
    pPlayerwk->actno = 1;
    pPlayerwk->xposi.w.h = 17000;
    actor->actfree[2] = 32;

    goal_chk_msnc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 16016, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 130, pEggman->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 15944, pPlayerwk->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
}

static void test_boss7_egg_goal_sequence_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    actor->actfree[2] = 32 | 16 | 64;
    actor->actfree[14] = 128 | 7;

    egg7_goal_snc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 64, actor->actfree[2] & (16 | 32 | 64));
    TEST_ASSERT_EQ_INT(ctx, 7, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 30, get_word_index(actor, 23));

    actor->actfree[2] = 64;
    set_word_index(actor, 23, 119);

    egg7_goal0(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 30, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 512, actor->yspeed.w);

    actor->actfree[3] = 5;
    actor->yposi.w.h = 400;
    actor->yspeed.w = 0;

    egg7_goal1(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 60, get_word_index(actor, 23));

    actor->actfree[3] = 7;
    actor->xposi.w.h = 16224;

    egg7_goal2(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 60, get_word_index(actor, 23));

    reset_boss7_state();
    actor->xposi.w.h = 15800;
    pPlayerwk = &actwk[0];

    egg7_goal3(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 32, sub_sync_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 1, genecolor_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 1, QuickReturn);
    TEST_ASSERT_EQ_INT(ctx, 128, colorwk[18].peRed);
    TEST_ASSERT_EQ_INT(ctx, 128, colorwk3[18].peRed);
}

static void test_boss7_msnc_demo1_player_pose_branches(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->cddat = 6;

    msnc_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->cddat = 2;
    actor->actfree[3] = 9;

    msnc_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->cddat = 6;
    actor->mstno.b.h = 1;
    actor->patcnt = 6;

    msnc_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);

    actor->patcnt = 7;
    pPlayerwk->cddat = 0;

    msnc_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
}

static void test_boss7_msnc_demo2_transition_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();

    msnc_demo2_ini(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);

    actor->patno = 6;

    kamae_anime(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 120, get_word_index(actor, 23));

    fire_start(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 4, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 150, get_word_index(actor, 23));

    reset_boss7_state();
    pEggman = &actwk[5];
    generate_flag = 1;

    door_open(actor);

    TEST_ASSERT_EQ_INT(ctx, 130, pEggman->actfree[14]);
    TEST_ASSERT_EQ_INT(ctx, 128, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 60, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 31, sub_sync_values[0]);

    reset_boss7_state();
    actor->actfree[3] = 7;

    msnc_demo2(actor);

    TEST_ASSERT_EQ_INT(ctx, 32, actor->actfree[2] & 32);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 640, get_word_index(actor, 28));
}

static void test_boss7_msnc_egg_check_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pEggman = &actwk[5];
    actor->actfree[2] = 32;
    actor->xposi.w.h = 13600;
    actor->xspeed.w = 100;
    pEggman->xposi.w.h = 13550;
    pEggman->xspeed.w = 900;

    msnc_egg_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 900, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 1536, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);

    reset_boss7_state();
    pEggman = &actwk[5];
    actor->actfree[2] = 32;
    actor->xposi.w.h = 13000;
    actor->xspeed.w = 100;
    pEggman->xposi.w.h = 12950;
    pEggman->xspeed.w = 900;
    actor->actfree[3] = 7;
    set_word_index(actor, 32, 12);

    msnc_egg_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 900, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 8, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 1536, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 32));
}

static void test_boss7_goal_check_dispatch_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pEggman = &actwk[5];
    pPlayerwk = &actwk[0];
    actor->xposi.w.h = 16016;

    goal_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 16016, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 130, pEggman->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 16, actor->r_no0);

    reset_boss7_state();
    pEggman = &actwk[5];
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 16016;
    actor->xposi.w.h = 1000;

    goal_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, pEggman->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 128, bossflag & 128);
    TEST_ASSERT_EQ_INT(ctx, 187, soundset_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 2048, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 16, get_word_index(actor, 26));
}

static void test_boss7_msnc_quickreturn_skips_render_work(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    set_word_index(actor, 25, 5);
    actwk[5].actfree[15] = 128;
    actor->r_no0 = 18;
    actor->actfree[3] = 1;
    set_word_index(actor, 23, 240);

    msnc(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 0, QuickReturn);
}

static void test_boss7_msnc_low_move_selects_high_low_and_boost_modes(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 1100;
    emycol_d_result = 0;

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 6, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 30));
    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 32));
    TEST_ASSERT_EQ_INT(ctx, -1024, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 1024, get_word_index(actor, 28));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 1500;
    set_word_index(actor, 30, 29);

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 5, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, -1, get_word_index(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, -640, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 640, get_word_index(actor, 28));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 14000;
    pPlayerwk->xposi.w.h = 13900;
    actor->actfree[12] = 2;
    set_word_index(actor, 33, 1);

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[12]);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 33));
}

static void test_boss7_msnc_attack_move_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 13000;

    msnc_atc_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 14000;
    pPlayerwk->xposi.w.h = 13000;
    set_word_index(actor, 30, 29);

    msnc_atc_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[12]);
    TEST_ASSERT_EQ_INT(ctx, 240, get_word_index(actor, 33));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 14000;
    actor->actfree[2] = 64;
    pPlayerwk->xposi.w.h = 14100;
    set_word_index(actor, 30, 29);

    msnc_atc_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[12]);
    TEST_ASSERT_EQ_INT(ctx, 240, get_word_index(actor, 33));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 1500;
    actor->xposi.w.h = 1000;
    actor->actfree[3] = 1;
    set_word_index(actor, 23, 60);

    msnc_act_next(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 64, actor->actfree[2] & 64);

    pPlayerwk->xposi.w.h = 500;
    msnc_act_next(actor);

    TEST_ASSERT_EQ_INT(ctx, 14, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 64);
}

static void test_boss7_msnc_attack_event_helpers(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pEggman = &actwk[5];
    actor->xposi.w.h = 100;
    scra_h_posit.w.h = 0;

    msnc_f_atc_tobi(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 1536, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 8, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 202, soundset_values[0]);

    msnc_f_atc_atack(actor);
    TEST_ASSERT_EQ_INT(ctx, 9, actor->mstno.b.h);

    msnc_f_atc_coli(actor);
    TEST_ASSERT_EQ_INT(ctx, 188, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);

    pEggman->xposi.w.h = 0;
    actor->xposi.w.h = 100;
    msnc_f_atc_end(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colicnt);

    reset_boss7_state();
    queue_spawn(&actwk[20]);

    msnc_b_atc_atack(actor);

    TEST_ASSERT_EQ_INT(ctx, -8, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, 512, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 10, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 208, soundset_values[0]);

    msnc_b_atc_coli(actor);
    TEST_ASSERT_EQ_INT(ctx, 189, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 2, actor->colicnt);

    msnc_b_atc_kill(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colino);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->colicnt);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
}

static void test_boss7_msnc_win_and_lose_sequences(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->actno = 1;
    pPlayerwk->xposi.w.h = 17000;

    msnc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 11, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 16016, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 460, actor->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 15944, pPlayerwk->xposi.w.h);

    msnc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 30, actor->actfree[1]);

    actor->actfree[1] = 1;

    msnc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 3, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[1]);

    msnc_win(actor);

    TEST_ASSERT_EQ_INT(ctx, 12, actor->mstno.b.h);

    reset_boss7_state();
    actor->xposi.w.h = 300;
    actor->yposi.w.h = 400;
    queue_spawn(&actwk[20]);

    msnc_lose(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, scoreup_count);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 13, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 26));
    TEST_ASSERT_EQ_INT(ctx, -1240, actor->yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 40, get_word_index(actor, 27));
    TEST_ASSERT_EQ_INT(ctx, 46, actwk[20].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 23));
}

static void test_boss7_egg_demo_wrappers_and_move_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 3040;

    egg7_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, -1536, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->cddat & 1);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    set_word_index(actor, 25, 6);
    actor->xposi.w.h = 2700;
    set_word_index(actor, 23, 120);

    egg7_demo2(actor);

    TEST_ASSERT_EQ_INT(ctx, 2816, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 121, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[6].r_no0);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->sprhsize = 36;
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 4000;
    pPlayerwk->xspeed.w = 100;

    egg7_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->actfree[2] & 4);
    TEST_ASSERT_EQ_INT(ctx, 240, get_word_index(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, 256, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 768, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(actor, 26));

    reset_boss7_state();
    actor->xposi.w.h = 16000;
    actor->xspeed.w = 100;
    actor->yspeed.w = 200;

    egg7_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 15952, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->yspeed.w);
}

static void test_boss7_egg_goal_dispatcher_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    actor->actfree[15] = 1;
    actor->actfree[2] = 32 | 16;
    actor->actfree[14] = 128 | 5;

    egg7_goal(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 30, get_word_index(actor, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & (32 | 16));
    TEST_ASSERT_EQ_INT(ctx, 5, actor->actfree[14]);

    reset_boss7_state();
    actor->actfree[15] = 2;

    egg7_goal(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
}

static void test_boss7_remaining_small_branch_paths(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->cddat = 6;
    actor->actfree[3] = 1;

    msnc_demo1(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor->actfree[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);

    reset_boss7_state();
    pEggman = &actwk[5];
    actor->actfree[2] = 32;
    actor->xposi.w.h = 1000;
    pEggman->xposi.w.h = 800;

    msnc_egg_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 26));

    reset_boss7_state();
    pEggman = &actwk[5];
    pPlayerwk = &actwk[0];
    pEggman->actfree[15] = 1;
    actor->xposi.w.h = 15944;

    goal_chk(actor);

    TEST_ASSERT_EQ_INT(ctx, 129, pEggman->actfree[15]);
    TEST_ASSERT_EQ_INT(ctx, 18, actor->r_no0);
}

static void test_boss7_more_msnc_movement_branches(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    pEggman->actfree[15] = 1;

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->mstno.b.h);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 900;
    set_word_index(actor, 31, -3);

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 31));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 1000;
    set_word_index(actor, 31, -5);

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 32));

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 1050;
    set_word_index(actor, 32, 49);

    msnc_low_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    GL_d5 = 10;
    set_word_index(actor, 31, -121);

    msnc_normal_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->r_no0);

    reset_boss7_state();
    actor->actfree[12] = 1;
    msnc_low_set(actor);
    TEST_ASSERT_EQ_INT(ctx, 1280, get_word_index(actor, 28));

    reset_boss7_state();
    actor->actfree[12] = 2;
    msnc_low_set(actor);
    TEST_ASSERT_EQ_INT(ctx, 426, get_word_index(actor, 28));
}

static void test_boss7_wrapper_attack_and_child_branches(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->xposi.w.h = 100;
    scra_h_posit.w.h = 0;

    msnc_atc(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 201, soundset_values[0]);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    actor->xposi.w.h = 100;
    scra_h_posit.w.h = 0;

    msnc_f_atc(actor);

    TEST_ASSERT_EQ_INT(ctx, 8, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 202, soundset_values[0]);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pEggman = &actwk[5];
    queue_spawn(&actwk[20]);

    msnc_b_atc(actor);

    TEST_ASSERT_EQ_INT(ctx, 10, actor->mstno.b.h);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[20].actno);

    reset_boss7_state();
    pMsnc = &actwk[5];
    pMsnc->mstno.b.h = 9;

    msnc_ele_01(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == actor);

    reset_boss7_state();
    pMsnc = &actwk[5];
    pMsnc->mstno.b.h = 10;
    actor->actfree[1] = 4;
    actor->patno = 1;

    msnc_ele_01(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->patno);
}

static void test_boss7_egg_scroll_beam_and_height_branches(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->actfree[2] = 16;
    actor->xposi.w.h = 1000;
    pPlayerwk->actno = 1;
    pPlayerwk->xposi.w.h = 1010;

    egg_beamchk(actor);

    TEST_ASSERT_EQ_INT(ctx, 128, pPlayerwk->cddat & 128);
    TEST_ASSERT_EQ_INT(ctx, 1, playdieset_count);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->actfree[14] = 128 | 1;
    actor->xposi.w.h = 16000;
    pPlayerwk->xposi.w.h = 15800;

    egg7_hscr(actor);

    TEST_ASSERT_EQ_INT(ctx, 15776, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 15776, scralim_n_left);
    TEST_ASSERT_EQ_INT(ctx, 24, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 2880, scralim_right);

    actor->actfree[14] = 2;
    egg7_hscr(actor);
    TEST_ASSERT_EQ_INT(ctx, 15776, scralim_right);

    actor->actfree[14] = 3;
    scr_dir_tbl[2] = 1234;
    egg7_hscr(actor);
    TEST_ASSERT_EQ_INT(ctx, 1234, scralim_right);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->xposi.w.h = 3120;
    actor->yposi.w.h = 500;
    emycol_d_result = 12;

    hight_set(actor);

    TEST_ASSERT_EQ_INT(ctx, 500, actor->yposi.w.h);

    reset_boss7_state();
    actor->xposi.w.h = 20000;
    actor->yposi.l = 500 << 16;
    actor->userflag.w = 16;
    emycol_d_result = 12;

    hight_set2(actor);

    TEST_ASSERT_EQ_INT(ctx, 64, actor->userflag.w);
    TEST_ASSERT_EQ_INT(ctx, (500 << 16) + (64 << 8), actor->yposi.l);
}

static void test_boss7_beam_variant_and_failure_paths(test_context *ctx) {
    sprite_status *beam = &actwk[8];
    sprite_status *parent = &actwk[4];

    reset_boss7_state();
    parent->actno = 1;
    beam->userflag.b.h = 1;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 4, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 15, beam->actfree[1]);
    TEST_ASSERT_TRUE(ctx, beam->patbase == bakupat);

    reset_boss7_state();
    parent->actno = 1;
    beam->userflag.b.h = 2;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 6, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 30, beam->actfree[1]);

    reset_boss7_state();
    parent->actno = 1;
    parent->xposi.w.h = 100;
    parent->yposi.w.h = 200;
    beam->r_no0 = 2;
    beam->actfree[1] = 3;
    beam->actfree[0] = 4;
    set_word_index(beam, 25, 4);
    set_word_index(beam, 29, 5);
    set_word_index(beam, 30, 6);
    set_word_index(beam, 23, 99);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 105, beam->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 206, beam->yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(beam, 23));
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 64;
    beam->r_no0 = 4;
    beam->actfree[1] = 1;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 6, beam->r_no0);
    TEST_ASSERT_EQ_INT(ctx, 15, beam->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);

    reset_boss7_state();
    parent->actno = 1;
    beam->r_no0 = 6;
    beam->actfree[1] = 1;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss7_state();
    spawn_result_after = 0;
    queue_spawn(&actwk[20]);

    make_bara(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);
}

static void test_boss7_more_jet_and_fragment_paths(test_context *ctx) {
    sprite_status *actor = &actwk[8];
    sprite_status *parent = &actwk[4];

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 8;
    parent->xposi.w.h = 500;
    parent->yposi.w.h = 600;
    actor->userflag.b.h = 1;
    set_word_index(actor, 25, 4);

    egg7jet(actor);

    TEST_ASSERT_EQ_INT(ctx, 452, actor->xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 586, actor->yposi.w.h);

    reset_boss7_state();
    actor->r_no0 = 2;
    set_word_index(actor, 25, 4);

    egg7jet(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_boss7_state();
    actor->r_no0 = 1;
    scra_v_posit.w.h = 0;
    actor->yposi.w.h = 300;

    hahen73(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
}

static void test_boss7_final_edge_branches(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    set_word_index(actor, 33, 3);
    set_word_index(actor, 30, 30);

    baisoku_mode(actor);

    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(actor, 33));
    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);

    reset_boss7_state();
    GL_d5 = -1;
    set_word_index(actor, 31, -121);

    msnc_normal_move(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);

    reset_boss7_state();
    pEggman = &actwk[5];
    pEggman->xposi.w.h = 200;
    actor->xposi.w.h = 100;

    msnc_f_atc_end(actor);

    TEST_ASSERT_EQ_INT(ctx, 6, actor->r_no0);

    reset_boss7_state();
    actor->r_no0 = 8;
    actor->actfree[3] = 9;
    pPlayerwk = &actwk[0];

    egg7(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 3000;
    scralim_left = 2700;

    snc_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 2840, scralim_n_left);

    actor->xposi.w.h = 3000;
    r_l(actor);
    TEST_ASSERT_EQ_INT(ctx, 3000, actor->xposi.w.h);

    actor->xposi.w.h = 3000;
    l_r(actor);
    TEST_ASSERT_EQ_INT(ctx, 3000, actor->xposi.w.h);

    actor->xposi.w.h = 3200;
    r_l2(actor);
    TEST_ASSERT_EQ_INT(ctx, 3200, actor->xposi.w.h);

    reset_boss7_state();
    actor->actfree[2] = 64;
    set_word_index(actor, 23, 19);

    egg7_goal0(actor);

    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 64);
    TEST_ASSERT_EQ_INT(ctx, 20, get_word_index(actor, 23));

    reset_boss7_state();
    generate_flag = 1;
    pPlayerwk = &actwk[0];

    egg7_goal3(actor);

    TEST_ASSERT_EQ_INT(ctx, 31, sub_sync_values[0]);
}

static void test_boss7_last_two_branch_lines(test_context *ctx) {
    sprite_status *actor = &actwk[4];

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    pPlayerwk->xposi.w.h = 3000;
    scralim_left = 2900;

    snc_wait(actor);

    TEST_ASSERT_EQ_INT(ctx, 2900, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->xspeed.w);

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->sprhsize = 36;
    actor->actfree[2] = 4;
    actor->xposi.w.h = 1000;
    pPlayerwk->xposi.w.h = 4000;

    egg7_spdset(actor);

    TEST_ASSERT_EQ_INT(ctx, 4, actor->actfree[2] & 4);
    TEST_ASSERT_EQ_INT(ctx, 0, get_word_index(actor, 31));
}

static void test_boss7_speed_height_and_collision_edge_branches(
    test_context *ctx) {
    sprite_status *actor = &actwk[4];
    Sint16 d1;

    reset_boss7_state();
    pPlayerwk = &actwk[0];
    actor->sprhsize = 1;
    actor->xposi.w.h = 100;
    scra_h_posit.w.h = 1000;
    actor->actfree[2] = 4;
    set_word_index(actor, 31, 2);

    egg7_spdset(actor);

    TEST_ASSERT_EQ_INT(ctx, 1, get_word_index(actor, 31));
    TEST_ASSERT_EQ_INT(ctx, 1536, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 4, get_word_index(actor, 26));

    set_word_index(actor, 31, 0);
    egg7_spdset(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor->actfree[2] & 4);

    pPlayerwk->xposi.w.h = 13000;
    egg7_maxspdset(actor, -1);
    TEST_ASSERT_EQ_INT(ctx, 1088, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 8, get_word_index(actor, 26));

    pPlayerwk->xposi.w.h = 1000;
    egg7_maxspdset(actor, -1);
    TEST_ASSERT_EQ_INT(ctx, 512, get_word_index(actor, 28));
    TEST_ASSERT_EQ_INT(ctx, 2, get_word_index(actor, 26));

    reset_boss7_state();
    actor->actfree[2] = 16;
    pPlayerwk = &actwk[0];
    pPlayerwk->actno = 0;
    egg_beamchk(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    pPlayerwk->actno = 1;
    pPlayerwk->r_no0 = 6;
    egg_beamchk(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    pPlayerwk->r_no0 = 0;
    pPlayerwk->xposi.w.h = 100;
    actor->xposi.w.h = 0;
    egg_beamchk(actor);
    TEST_ASSERT_EQ_INT(ctx, 0, playdieset_count);

    reset_boss7_state();
    actor->xposi.w.h = 3120;
    actor->yposi.w.h = 500;
    emycol_d_result = -12;

    hight_set(actor);

    TEST_ASSERT_EQ_INT(ctx, 500, actor->yposi.w.h);

    d1 = 7;
    actor->xposi.w.h = 3100;
    TEST_ASSERT_EQ_INT(ctx, 0, hight_set_retry(actor, &d1));
    TEST_ASSERT_EQ_INT(ctx, 0, d1);
}

static void test_boss7_more_beam_and_spawn_edges(test_context *ctx) {
    sprite_status *beam = &actwk[8];
    sprite_status *parent = &actwk[4];

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 64;
    beam->r_no0 = 2;
    beam->userflag.b.l = 1;
    beam->actfree[1] = 2;
    beam->actfree[0] = 3;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 1, beam->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, beam->actfree[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk2_count);

    reset_boss7_state();
    parent->actno = 1;
    parent->actfree[2] = 64;
    beam->r_no0 = 6;
    beam->actfree[1] = 2;
    set_word_index(beam, 25, 4);

    egg7beam(beam);

    TEST_ASSERT_EQ_INT(ctx, 1, beam->actfree[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss7_state();
    parent->actno = 1;

    /* Use the first actor as a jet whose parent exists but is not emitting. */
    actwk[8].r_no0 = 2;
    set_word_index(&actwk[8], 25, 4);
    parent->actfree[2] = 0;
    egg7jet(&actwk[8]);

    TEST_ASSERT_EQ_INT(ctx, 0, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, frameout_count);

    reset_boss7_state();
    spawn_result_after = 0;
    queue_spawn(&actwk[20]);

    make_beam(parent);

    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk2_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[20].actno);
}

TEST_MAIN_BEGIN;
    test_boss7_speed_helpers_use_indexed_actfree_words(&ctx);
    test_boss7_timer_helpers_advance_event_state(&ctx);
    test_boss7_spawn_helpers_create_child_actors(&ctx);
    test_boss7_effect_spawn_helpers_capture_positions(&ctx);
    test_boss7_beam_and_bara_spawn_sequences(&ctx);
    test_boss7_public_initializers_set_render_state(&ctx);
    test_boss7_egg_init_spawns_race_actors(&ctx);
    test_boss7_beam_public_paths_follow_parent_state(&ctx);
    test_boss7_jet_and_fragment_public_paths(&ctx);
    test_boss7_bara_public_path_actions_or_frames_out(&ctx);
    test_boss7_egg_demo_transition_helpers(&ctx);
    test_boss7_beam_flag_helpers(&ctx);
    test_boss7_goal_transition_helpers(&ctx);
    test_boss7_egg_goal_sequence_helpers(&ctx);
    test_boss7_msnc_demo1_player_pose_branches(&ctx);
    test_boss7_msnc_demo2_transition_helpers(&ctx);
    test_boss7_msnc_egg_check_paths(&ctx);
    test_boss7_goal_check_dispatch_paths(&ctx);
    test_boss7_msnc_quickreturn_skips_render_work(&ctx);
    test_boss7_msnc_low_move_selects_high_low_and_boost_modes(&ctx);
    test_boss7_msnc_attack_move_helpers(&ctx);
    test_boss7_msnc_attack_event_helpers(&ctx);
    test_boss7_msnc_win_and_lose_sequences(&ctx);
    test_boss7_egg_demo_wrappers_and_move_paths(&ctx);
    test_boss7_egg_goal_dispatcher_paths(&ctx);
    test_boss7_remaining_small_branch_paths(&ctx);
    test_boss7_more_msnc_movement_branches(&ctx);
    test_boss7_wrapper_attack_and_child_branches(&ctx);
    test_boss7_egg_scroll_beam_and_height_branches(&ctx);
    test_boss7_beam_variant_and_failure_paths(&ctx);
    test_boss7_more_jet_and_fragment_paths(&ctx);
    test_boss7_final_edge_branches(&ctx);
    test_boss7_last_two_branch_lines(&ctx);
    test_boss7_speed_height_and_collision_edge_branches(&ctx);
    test_boss7_more_beam_and_spawn_edges(&ctx);
TEST_MAIN_END;

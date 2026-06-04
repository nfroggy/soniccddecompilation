#include <string.h>

#include "support/test_runner.h"
#include "types.h"

sprite_status actwk[128];
Uint8 pltime_f;
Sint16 plring;
Uint8 plautoflag;
Uint8 special_flag;
Uint8 clrspflg_save;
Uint8 ta_flag;
Uint8 time_flag;
Uint8 generate_flag;
Uint8 plflag;
Uint8 plpower_s;
Uint8 plpower_m;
Uint8 plpower_a;
Uint8 bonus_f;
Sint16 timebonus;
Sint16 ringbonus;
Sint16 scralim_left;
Sint16 scralim_n_left;
Sint16 backto_cnt;
char time_item;
short_union stageno;
short_union scroll_start;
ushort_union swdata;
int_union scra_h_posit;
int_union pltime;
Uint32 divdevwk[24];
PALETTEENTRY colorwk[64];
PALETTEENTRY *lpcolorwk = colorwk;

static sprite_pattern dummy_pattern = {1, {{0, 0, 0, 1}}};
sprite_pattern *genepat[] = {&dummy_pattern};
sprite_pattern *bring2pat[] = {&dummy_pattern};
sprite_pattern *bringpat[] = {&dummy_pattern};
sprite_pattern *goalpat[] = {&dummy_pattern};
Uint8 genechg0[] = {1, 0, 255};
Uint8 bring2chg0[] = {1, 1, 255};
Uint8 bringchg0[] = {1, 2, 255};
Uint8 goalchg0[] = {1, 3, 255};
Uint8 *genechg[] = {genechg0};
Uint8 *bring2chg[] = {bring2chg0};
Uint8 *bringchg[] = {bringchg0};
Uint8 *goalchg[] = {goalchg0};
map_init_data mapinittbl;

static int actionsub_count;
static sprite_status *actionsub_actor;
static int patchg_count;
static sprite_status *patchg_actor;
static Uint8 **patchg_table;
static int frameout_count;
static sprite_status *frameout_actor;
static int frameout_s_count;
static sprite_status *frameout_s_actor;
static int soundset_count;
static Sint16 soundset_requests[16];
static int sub_sync_count;
static Sint16 sub_sync_requests[16];
static int speedset_count;
static sprite_status *speedset_actor;
static int emycol_d_count;
static Sint16 emycol_d_result;
static int actwkchk_count;
static sprite_status *actwkchk_queue[16];
static int actwkchk_queue_count;
static int actwkchk_queue_index;
static int colorset2_count;
static Sint32 colorset2_values[8];

void actionsub(sprite_status *pActwk);
void patchg(sprite_status *patchgwk, Uint8 **pat_dat);
void frameout(sprite_status *pActwk);
Sint32 frameout_s(sprite_status *pActwk);
void soundset(Sint16 ReqNo);
void sub_sync(Sint16 ReqNo);
void speedset(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint32 actwkchk(sprite_status **ppActwk);
void colorset2(Sint32 ColorNo);

#include "src/goal.c"

void actionsub(sprite_status *pActwk) {
    ++actionsub_count;
    actionsub_actor = pActwk;
}

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    ++patchg_count;
    patchg_actor = patchgwk;
    patchg_table = pat_dat;
}

void frameout(sprite_status *pActwk) {
    ++frameout_count;
    frameout_actor = pActwk;
}

Sint32 frameout_s(sprite_status *pActwk) {
    ++frameout_s_count;
    frameout_s_actor = pActwk;
    return 0;
}

void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}

void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 16) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}

void speedset(sprite_status *pActwk) {
    ++speedset_count;
    speedset_actor = pActwk;
}

Sint16 emycol_d(sprite_status *pActwk) {
    (void)pActwk;
    ++emycol_d_count;
    return emycol_d_result;
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

void colorset2(Sint32 ColorNo) {
    if (colorset2_count < 8) {
        colorset2_values[colorset2_count] = ColorNo;
    }
    ++colorset2_count;
}

static void queue_actor(sprite_status *actor) {
    actwkchk_queue[actwkchk_queue_count++] = actor;
}

static void reset_goal_state(void) {
    memset(actwk, 0, sizeof(actwk));
    pltime_f = 1;
    plring = 0;
    plautoflag = 0;
    special_flag = 0;
    clrspflg_save = 0;
    ta_flag = 0;
    time_flag = 0;
    generate_flag = 0;
    plflag = 0;
    plpower_s = 1;
    plpower_m = 1;
    plpower_a = 1;
    bonus_f = 0;
    timebonus = 0;
    ringbonus = 0;
    scralim_left = 0;
    scralim_n_left = 0;
    backto_cnt = 99;
    time_item = 5;
    memset(&stageno, 0, sizeof(stageno));
    memset(&scroll_start, 0, sizeof(scroll_start));
    memset(&swdata, 0, sizeof(swdata));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&pltime, 0, sizeof(pltime));
    memset(divdevwk, 0, sizeof(divdevwk));
    memset(colorwk, 0, sizeof(colorwk));
    memset(&mapinittbl, 0, sizeof(mapinittbl));

    actionsub_count = 0;
    actionsub_actor = 0;
    patchg_count = 0;
    patchg_actor = 0;
    patchg_table = 0;
    frameout_count = 0;
    frameout_actor = 0;
    frameout_s_count = 0;
    frameout_s_actor = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    speedset_count = 0;
    speedset_actor = 0;
    emycol_d_count = 0;
    emycol_d_result = 0;
    actwkchk_count = 0;
    memset(actwkchk_queue, 0, sizeof(actwkchk_queue));
    actwkchk_queue_count = 0;
    actwkchk_queue_index = 0;
    colorset2_count = 0;
    memset(colorset2_values, 0, sizeof(colorset2_values));
}

static void test_tables_capture_dispatch_order(test_context *ctx) {
    TEST_ASSERT_TRUE(ctx, gene_tbl[0] == gene_init);
    TEST_ASSERT_TRUE(ctx, gene_tbl[1] == gene_move0);
    TEST_ASSERT_TRUE(ctx, gene_tbl[2] == gene_move1);
    TEST_ASSERT_TRUE(ctx, gene_tbl[3] == goal_move2);
    TEST_ASSERT_TRUE(ctx, gene_tbl[4] == goal_move3);
    TEST_ASSERT_TRUE(ctx, gene_tbl[5] == kira_move);
    TEST_ASSERT_TRUE(ctx, bring2_tbl[0] == bring2init);
    TEST_ASSERT_TRUE(ctx, bring2_tbl[2] == bring2move1);
    TEST_ASSERT_TRUE(ctx, bigring_tbl[0] == bring_init);
    TEST_ASSERT_TRUE(ctx, mosugu_tbl[0] == mosug_init);
    TEST_ASSERT_TRUE(ctx, goal_tbl[4] == goal_move3);
}

static void setup_gene_collision(sprite_status *gene_actor, Sint16 dx) {
    gene_actor->xposi.w.h = 100;
    gene_actor->yposi.w.h = 200;
    actwk[0].cddat = 4;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 12;
    actwk[0].xposi.w.h = gene_actor->xposi.w.h + dx;
    actwk[0].yposi.w.h = 200;
}

static void test_gene_collision_and_init_paths(test_context *ctx) {
    reset_goal_state();
    setup_gene_collision(&actwk[1], 0);
    actwk[0].yspeed.w = 400;

    gene_init(&actwk[1]);

    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].actflg);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 120, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, -100, actwk[0].yspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime_f);
    TEST_ASSERT_TRUE(ctx, patchg_table == genechg);

    reset_goal_state();
    setup_gene_collision(&actwk[1], -80);
    actwk[0].xspeed.w = 400;
    gene_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 400, actwk[0].xspeed.w);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    gene_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
}

static void test_gene_coli_boundaries(test_context *ctx) {
    sprite_status gene_actor;
    sprite_status player;
    reset_goal_state();
    memset(&gene_actor, 0, sizeof(gene_actor));
    memset(&player, 0, sizeof(player));
    gene_actor.xposi.w.h = 100;
    gene_actor.yposi.w.h = 200;
    player.cddat = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
    player.cddat = 4;
    player.sprhs = 8;
    player.sprvsize = 12;
    player.xposi.w.h = 60;
    player.yposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 1, gene_coli(&gene_actor, &player));
    player.xposi.w.h = 40;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
    player.xposi.w.h = 180;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
    player.xposi.w.h = 100;
    player.yposi.w.h = 150;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
    player.yposi.w.h = 160;
    TEST_ASSERT_EQ_INT(ctx, 1, gene_coli(&gene_actor, &player));
    player.yposi.w.h = 120;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
    player.yposi.w.h = 260;
    TEST_ASSERT_EQ_INT(ctx, 0, gene_coli(&gene_actor, &player));
}

static void test_gene_move1_spawns_particles_and_kira_set(test_context *ctx) {
    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    goal_work_get(&actwk[1])->timer = 5;
    queue_actor(&actwk[40]);
    gene_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 24, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[40].r_no1);
    TEST_ASSERT_EQ_INT(ctx, 132, actwk[40].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, actwk[40].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, soundset_count);
    TEST_ASSERT_EQ_INT(ctx, 158, soundset_requests[0]);

    reset_goal_state();
    goal_work_get(&actwk[1])->timer = 6;
    gene_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 5, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 0, actwkchk_count);

    reset_goal_state();
    goal_work_get(&actwk[1])->timer = 5;
    gene_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, soundset_count);

    reset_goal_state();
    mapinittbl.colorno1 = 9;
    actwk[1].xposi.w.h = 300;
    actwk[1].yposi.w.h = 400;
    for (int i = 0; i < 7; ++i) {
        queue_actor(&actwk[40 + i]);
    }
    actwk[1].r_no0 = 4;
    goal_work_get(&actwk[1])->timer = 0;
    gene_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 60, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 9, colorset2_values[0]);
    TEST_ASSERT_EQ_INT(ctx, 21, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, -128, actwk[41].xspeed.w);
    TEST_ASSERT_EQ_INT(ctx, 384, actwk[46].xspeed.w);

    reset_goal_state();
    queue_actor(&actwk[40]);
    kira_set(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwkchk_count);
    TEST_ASSERT_EQ_INT(ctx, 21, actwk[40].actno);
}

static void test_kira_move_and_gene_dispatch(test_context *ctx) {
    reset_goal_state();
    emycol_d_result = 0;
    kira_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, speedset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patchg_count);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].actno);

    reset_goal_state();
    emycol_d_result = -1;
    kira_move(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 31, actwk[1].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].r_no0 = 10;
    emycol_d_result = -1;
    gene(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);

    reset_goal_state();
    actwk[1].r_no0 = 10;
    gene(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_goal_state();
    actwk[1].r_no0 = 8;
    gene(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
}

static void test_bring2_and_bigring_gate_paths(test_context *ctx) {
    reset_goal_state();
    actwk[1].userflag.b.h = 1;
    bigring(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1007, actwk[1].sproffset);
    TEST_ASSERT_TRUE(ctx, actwk[1].patbase == bring2pat);
    TEST_ASSERT_TRUE(ctx, patchg_table == bring2chg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_goal_state();
    actwk[1].userflag.b.h = 1;
    actwk[1].r_no0 = 4;
    bigring(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_goal_state();
    plring = 49;
    bigring(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_bring_init_and_collision_paths(test_context *ctx) {
    reset_goal_state();
    clrspflg_save = 127;
    bring_init(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);

    reset_goal_state();
    plring = 50;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 8;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 200;
    bigring(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_TRUE(ctx, patchg_table == bringchg);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_goal_state();
    plring = 50;
    scra_h_posit.w.h = 1000;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 8;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    queue_actor(&actwk[40]);
    bigring(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1, special_flag);
    TEST_ASSERT_EQ_INT(ctx, 1336, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 2056, swdata.w);
    TEST_ASSERT_EQ_INT(ctx, 1, scroll_start.b.h);
    TEST_ASSERT_EQ_INT(ctx, 175, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[40].actno);

    actwk[0].sprhs = 8;
    actwk[0].sprvsize = 8;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    TEST_ASSERT_EQ_INT(ctx, 1, bring_coli(&actwk[1], &actwk[0]));
    actwk[0].xposi.w.h = 0;
    TEST_ASSERT_EQ_INT(ctx, 0, bring_coli(&actwk[1], &actwk[0]));
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 150;
    TEST_ASSERT_EQ_INT(ctx, 0, bring_coli(&actwk[1], &actwk[0]));
    actwk[0].yposi.w.h = 240;
    TEST_ASSERT_EQ_INT(ctx, 0, bring_coli(&actwk[1], &actwk[0]));
}

static void test_mosugu_gates_and_offset_set(test_context *ctx) {
    reset_goal_state();
    stageno.w = 513;
    time_flag = 1;
    mosug_init(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[1].userflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    stageno.w = 513;
    time_flag = 1;
    actwk[1].userflag.b.h = 1;
    divdevwk[0] = 1;
    mosug_init(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    stageno.b.h = 3;
    time_flag = 2;
    generate_flag = 1;
    offset_set(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 689, actwk[1].sproffset);

    reset_goal_state();
    stageno.w = 0;
    offset_set(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 33626, actwk[1].sproffset);
}

static void test_mosugu_move0_and_move1_boundaries(test_context *ctx) {
    reset_goal_state();
    actwk[1].xposi.w.h = 500;
    mosugu(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].sprhsize = 16;
    actwk[0].sprhsize = 8;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    actwk[0].xspeed.w = 1;
    scra_h_posit.w.h = 444;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 444, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 0, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 0, time_item);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_a);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].sprhsize = 16;
    actwk[0].sprhsize = 8;
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 200;
    actwk[0].xspeed.w = -1;
    actwk[0].sproffset = 32768;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sproffset);

    reset_goal_state();
    stageno.w = 1280;
    scralim_left = 11352;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].sprhsize = 16;
    actwk[0].sprhsize = 8;
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 620;
    actwk[0].sproffset = 32768;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sproffset);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 0;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 400;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    plflag = 2;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[1].sprhsize = 16;
    actwk[0].sprhsize = 8;
    actwk[0].xposi.w.h = 80;
    actwk[0].yposi.w.h = 200;
    mosug_move0(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].sprhsize = 16;
    actwk[0].xposi.w.h = 200;
    actwk[0].sprhsize = 8;
    actwk[0].sproffset = 32768;
    mosug_move1(&actwk[1], &actwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[0].sproffset);

    reset_goal_state();
    stageno.b.l = 2;
    mosugu(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actionsub_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_s_count);
}

static void test_goal_state_machine_and_bonus(test_context *ctx) {
    reset_goal_state();
    stageno.b.h = 1;
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 101;
    actwk[0].yposi.w.h = 200;
    scra_h_posit.w.h = 333;
    goal(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 1212, actwk[1].sproffset);
    TEST_ASSERT_EQ_INT(ctx, 333, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 0, pltime_f);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_s);
    TEST_ASSERT_EQ_INT(ctx, 0, plpower_m);
    TEST_ASSERT_EQ_INT(ctx, 157, soundset_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_count);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 101;
    actwk[0].yposi.w.h = 0;
    goal_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 101;
    actwk[0].yposi.w.h = 400;
    goal_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].xposi.w.h = 100;
    actwk[1].yposi.w.h = 200;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = 200;
    goal_move0(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].r_no0 = 4;
    goal_work_get(&actwk[1])->timer = 2;
    goal_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 4, actwk[1].r_no0);
    goal_move1(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[1].patno);
    TEST_ASSERT_EQ_INT(ctx, 60, goal_work_get(&actwk[1])->timer);

    reset_goal_state();
    actwk[1].r_no0 = 6;
    goal_work_get(&actwk[1])->timer = 2;
    goal_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 6, actwk[1].r_no0);

    reset_goal_state();
    actwk[1].r_no0 = 6;
    goal_work_get(&actwk[1])->timer = 1;
    pltime.b.b3 = 1;
    pltime.b.b2 = 4;
    plring = 3;
    queue_actor(&actwk[40]);
    goal_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 180, goal_work_get(&actwk[1])->timer);
    TEST_ASSERT_EQ_INT(ctx, 8, actwk[1].r_no0);
    TEST_ASSERT_EQ_INT(ctx, 2, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 130, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 107, sub_sync_requests[1]);
    TEST_ASSERT_EQ_INT(ctx, 1, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 2056, swdata.w);
    TEST_ASSERT_EQ_INT(ctx, 58, actwk[40].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, bonus_f);
    TEST_ASSERT_EQ_INT(ctx, 500, timebonus);
    TEST_ASSERT_EQ_INT(ctx, 300, ringbonus);

    reset_goal_state();
    stageno.w = 1282;
    time_flag = 1;
    goal_work_get(&actwk[1])->timer = 1;
    pltime.b.b3 = 59;
    pltime.b.b2 = 10;
    goal_move2(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 0, swdata.w);
    TEST_ASSERT_EQ_INT(ctx, 1, sub_sync_count);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_count);
    TEST_ASSERT_TRUE(ctx, frameout_actor == &actwk[127]);
    TEST_ASSERT_EQ_INT(ctx, 0, timebonus);
}

static void test_goal_move3_and_genecolor(test_context *ctx) {
    reset_goal_state();
    actwk[1].xposi.w.h = 123;
    goal_move3(&actwk[1]);
    TEST_ASSERT_EQ_INT(ctx, 123, actwk[1].xposi.w.h);

    genecolor();
    TEST_ASSERT_EQ_INT(ctx, 32, colorwk[16].peRed);
    TEST_ASSERT_EQ_INT(ctx, 32, colorwk[16].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 160, colorwk[16].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 1, colorwk[16].peFlags);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[31].peRed);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[31].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[31].peBlue);
}

TEST_MAIN_BEGIN;
    test_tables_capture_dispatch_order(&ctx);
    test_gene_collision_and_init_paths(&ctx);
    test_gene_coli_boundaries(&ctx);
    test_gene_move1_spawns_particles_and_kira_set(&ctx);
    test_kira_move_and_gene_dispatch(&ctx);
    test_bring2_and_bigring_gate_paths(&ctx);
    test_bring_init_and_collision_paths(&ctx);
    test_mosugu_gates_and_offset_set(&ctx);
    test_mosugu_move0_and_move1_boundaries(&ctx);
    test_goal_state_machine_and_bonus(&ctx);
    test_goal_move3_and_genecolor(&ctx);
TEST_MAIN_END

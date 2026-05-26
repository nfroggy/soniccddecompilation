#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

#ifndef TEST_SCR31_MAP_ROWS
#define TEST_SCR31_MAP_ROWS 8
#endif
#ifndef TEST_SCR31_MAP_COLS
#define TEST_SCR31_MAP_COLS 64
#endif
#ifndef TEST_SCR31_HIGH_MAP_ROW
#define TEST_SCR31_HIGH_MAP_ROW 7
#endif
#ifndef TEST_SCR31_HIGH_MAP_COL
#define TEST_SCR31_HIGH_MAP_COL 63
#endif
#ifndef TEST_SCR31_SCRALIM_RIGHT
#define TEST_SCR31_SCRALIM_RIGHT 11927
#endif
#ifndef TEST_SCR31_DEFAULT_X
#define TEST_SCR31_DEFAULT_X 96
#endif
#ifndef TEST_SCR31_DEFAULT_Y
#define TEST_SCR31_DEFAULT_Y 1004
#endif
#ifndef TEST_SCR31_DEFAULT_SCRA_V
#define TEST_SCR31_DEFAULT_SCRA_V 908
#endif
#ifndef TEST_SCR31_DEFAULT_SCRB_V
#define TEST_SCR31_DEFAULT_SCRB_V 334
#endif
#ifndef TEST_SCR31_SAVED_SCRZ_H
#define TEST_SCR31_SAVED_SCRZ_H 745
#endif
#ifndef TEST_SCR31_SAVED_SCRC_H
#define TEST_SCR31_SAVED_SCRC_H 1116
#endif
#ifndef TEST_SCR31_SAVED_SCRB_H
#define TEST_SCR31_SAVED_SCRB_H 558
#endif
#ifndef TEST_SCR31_SCROLL_ACT_Y
#define TEST_SCR31_SCROLL_ACT_Y 1004
#endif
#ifndef TEST_SCR31_SCROLL_VSCROLL_L
#define TEST_SCR31_SCROLL_VSCROLL_L 334
#endif
#ifndef TEST_SCR31_ENDDEMO_SCRA_H
#define TEST_SCR31_ENDDEMO_SCRA_H 3584
#endif
#ifndef TEST_SCR31_ENDDEMO_SCRZ_H
#define TEST_SCR31_ENDDEMO_SCRZ_H 224
#endif
#ifndef TEST_SCR31_ENDDEMO_SCRC_H
#define TEST_SCR31_ENDDEMO_SCRC_H 336
#endif
#ifndef TEST_SCR31_ENDDEMO_SCRB_H
#define TEST_SCR31_ENDDEMO_SCRB_H 168
#endif

Uint8 main_play;
game_info *lpKeepWork;
static game_info keep_work;
Uint8 plflag;
Uint8 play_start;
Sint16 enddemono;
short_union demoflag;
sprite_status actwk[128];
Uint8 loopmapno;
Uint8 loopmapno2;
Uint8 ballmapno;
Uint8 ballmapno2;
Uint8 scrh_flag;
Uint8 scrv_flag;
ushort_union scr_die;
ushort_union zone_flag;
short_union scr_timer;
Sint16 scrar_no;
Sint16 scralim_left;
Sint16 scralim_n_left;
Sint16 scralim_right;
Sint16 scralim_n_right;
Sint16 scralim_up;
Sint16 scralim_n_up;
Sint16 scralim_down;
Sint16 scralim_n_down;
Sint16 scra_h_keep;
Uint8 scra_h_count;
Uint8 scra_v_count;
Uint8 scrb_h_count;
Uint8 scrb_v_count;
Uint8 scrc_h_count;
Uint8 scrc_v_count;
Uint8 scrz_h_count;
Uint8 scrz_v_count;
Sint16 scra_vline;
Sint16 scra_hline;
Sint16 scra_hz;
Sint16 scra_vz;
Uint8 limmoveflag;
short_union scroll_start;
ushort_union scrflaga;
ushort_union scrflagb;
ushort_union scrflagc;
ushort_union scrflagz;
ushort_union scrflagaw;
ushort_union scrflagbw;
int_union scra_h_posit;
int_union scra_v_posit;
int_union scrb_h_posit;
int_union scrb_v_posit;
int_union scrc_h_posit;
int_union scrc_v_posit;
int_union scrz_h_posit;
int_union scrz_v_posit;
int_union scra_h_posiw;
int_union scra_v_posiw;
int_union scrb_h_posiw;
int_union scrb_v_posiw;
int_union scrc_h_posiw;
int_union scrc_v_posiw;
int_union scrz_h_posiw;
int_union scrz_v_posiw;
int_union vscroll;
short_union waterdirec;
Sint16 waterposi;
Sint16 hscrollwork[256];
static int_union hscroll_buffer[512];
int_union *lphscrollbuff = hscroll_buffer;
Uint8 mapwkb[TEST_SCR31_MAP_ROWS][TEST_SCR31_MAP_COLS];
Uint8 mapwka[TEST_SCR31_MAP_ROWS][TEST_SCR31_MAP_COLS];
Uint8 awasintbl[256];
static Uint16 map_work[512];
Uint16 *pmapwk = map_work;
Uint16 blockwk[1024][4];
map_init_data mapinittbl;
Sint32 (*SetGrid)(Sint32, Sint32, Sint32, Sint32, Sint32);

static int playload_count;
static int scrchk_count;
static int colorset_count;
static int colorset2_count;
static Sint32 colorset_arg;
static Sint32 colorset2_arg;
static int setgrid_count;
static Sint32 setgrid_base[160];
static Sint32 setgrid_x[160];
static Sint32 setgrid_y[160];
static Sint32 setgrid_block[160];
static Sint32 setgrid_frip[160];

void playload(void);
void scrchk(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);

#ifndef TEST_SCR31_SOURCE
#define TEST_SCR31_SOURCE "src/r3/scr31a.c"
#endif

#include TEST_SCR31_SOURCE

void playload(void) { ++playload_count; }
void scrchk(void) { ++scrchk_count; }
void colorset(Sint32 ColorNo) {
    ++colorset_count;
    colorset_arg = ColorNo;
}
void colorset2(Sint32 ColorNo) {
    ++colorset2_count;
    colorset2_arg = ColorNo;
}

static Sint32 test_setgrid(Sint32 base, Sint32 x, Sint32 y, Sint32 block,
                           Sint32 frip) {
    if (setgrid_count < (int)(sizeof(setgrid_base) / sizeof(setgrid_base[0]))) {
        setgrid_base[setgrid_count] = base;
        setgrid_x[setgrid_count] = x;
        setgrid_y[setgrid_count] = y;
        setgrid_block[setgrid_count] = block;
        setgrid_frip[setgrid_count] = frip;
    }
    ++setgrid_count;
    return 0;
}

static void reset_scr31a_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(mapwkb, 0, sizeof(mapwkb));
    memset(awasintbl, 0, sizeof(awasintbl));
    memset(map_work, 0, sizeof(map_work));
    memset(blockwk, 0, sizeof(blockwk));
    memset(hscrollwork, 0, sizeof(hscrollwork));
    memset(hscroll_buffer, 0, sizeof(hscroll_buffer));
    memset(&mapinittbl, 0, sizeof(mapinittbl));
    memset(&keep_work, 0, sizeof(keep_work));
    lpKeepWork = &keep_work;
    main_play = 0;
    plflag = 0;
    play_start = 0;
    enddemono = 0;
    memset(&demoflag, 0, sizeof(demoflag));
    loopmapno = loopmapno2 = ballmapno = ballmapno2 = 0;
    scrh_flag = scrv_flag = 0;
    memset(&scr_die, 0, sizeof(scr_die));
    memset(&zone_flag, 0, sizeof(zone_flag));
    memset(&scr_timer, 0, sizeof(scr_timer));
    scrar_no = 0;
    scralim_left = scralim_n_left = 0;
    scralim_right = scralim_n_right = 0;
    scralim_up = scralim_n_up = 0;
    scralim_down = scralim_n_down = 0;
    scra_h_keep = 0;
    scra_h_count = scra_v_count = 0;
    scrb_h_count = scrb_v_count = 0;
    scrc_h_count = scrc_v_count = 0;
    scrz_h_count = scrz_v_count = 0;
    scra_vline = scra_hline = 0;
    scra_hz = scra_vz = 0;
    limmoveflag = 0;
    memset(&scroll_start, 0, sizeof(scroll_start));
    memset(&scrflaga, 0, sizeof(scrflaga));
    memset(&scrflagb, 0, sizeof(scrflagb));
    memset(&scrflagc, 0, sizeof(scrflagc));
    memset(&scrflagz, 0, sizeof(scrflagz));
    memset(&scrflagaw, 0, sizeof(scrflagaw));
    memset(&scrflagbw, 0, sizeof(scrflagbw));
    scra_h_posit.l = scra_v_posit.l = 0;
    scrb_h_posit.l = scrb_v_posit.l = 0;
    scrc_h_posit.l = scrc_v_posit.l = 0;
    scrz_h_posit.l = scrz_v_posit.l = 0;
    scra_h_posiw.l = scra_v_posiw.l = 0;
    scrb_h_posiw.l = scrb_v_posiw.l = 0;
    scrc_h_posiw.l = scrc_v_posiw.l = 0;
    scrz_h_posiw.l = scrz_v_posiw.l = 0;
    vscroll.l = 0;
    memset(&waterdirec, 0, sizeof(waterdirec));
    waterposi = 0;
    lphscrollbuff = hscroll_buffer;
    pmapwk = map_work;
    SetGrid = test_setgrid;
    playload_count = 0;
    scrchk_count = 0;
    colorset_count = colorset2_count = 0;
    colorset_arg = colorset2_arg = 0;
    setgrid_count = 0;
    memset(setgrid_base, 0, sizeof(setgrid_base));
    memset(setgrid_x, 0, sizeof(setgrid_x));
    memset(setgrid_y, 0, sizeof(setgrid_y));
    memset(setgrid_block, 0, sizeof(setgrid_block));
    memset(setgrid_frip, 0, sizeof(setgrid_frip));
}

static void seed_block(Uint16 block, Uint16 base) {
    blockwk[block][0] = base;
    blockwk[block][1] = base + 1;
    blockwk[block][2] = base + 2;
    blockwk[block][3] = base + 3;
}

static void test_scr31a_scr_set_default_and_saved_positions(test_context *ctx) {
    reset_scr31a_state();

    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[0]);
    main_play = 1;
    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[1]);

    reset_scr31a_state();
    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SCRALIM_RIGHT, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 576, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_X, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_Y, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_SCRA_V, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_SCRB_V, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_SCRB_V, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_DEFAULT_SCRB_V, scrz_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno2);

    reset_scr31a_state();
    plflag = 1;
    actwk[0].xposi.w.h = 13000;
    actwk[0].yposi.w.h = 2000;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SCRALIM_RIGHT, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1296, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SAVED_SCRZ_H, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SAVED_SCRC_H, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SAVED_SCRB_H, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 528, scrb_v_posit.w.h);
}

static void test_scr31a_scr_set_uses_end_demo_position(test_context *ctx) {
    reset_scr31a_state();
    demoflag.w = -32768;
    enddemono = 2;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 3744, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1132, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_ENDDEMO_SCRA_H, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1036, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_ENDDEMO_SCRZ_H, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_ENDDEMO_SCRC_H, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_ENDDEMO_SCRB_H, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 398, scrb_v_posit.w.h);
}

static void test_scr31a_position_edge_cases(test_context *ctx) {
    reset_scr31a_state();
    plflag = 1;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = -50;

    playposiset();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_posit.w.h);

    reset_scr31a_state();
    scra_v_posit.w.h = 2000;
    scrbinit(2048, 0);

    TEST_ASSERT_EQ_INT(ctx, 536, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 192, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 96, scrb_h_posit.w.h);
}

static void test_scr31a_scroll_horizontal_paths(test_context *ctx) {
    reset_scr31a_state();
    scralim_left = 0;
    scralim_right = TEST_SCR31_SCRALIM_RIGHT;
    scra_hline = 160;
    scra_h_count = 0;
    scra_h_posit.w.h = 100;
    actwk[0].xposi.w.h = 260;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflaga.b.h);

    actwk[0].xposi.w.h = 400;
    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflaga.b.h);

    scrflaga.b.h = 0;
    actwk[0].xposi.w.h = 0;
    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);

    scra_h_posit.w.h = TEST_SCR31_SCRALIM_RIGHT - 7;
    actwk[0].xposi.w.h = 20000;
    right_check(100);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SCRALIM_RIGHT, scra_h_posit.w.h);

    scra_h_posit.w.h = 4;
    left_check((Uint16)-100);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);

    scra_h_posit.w.h = 100;
    scralim_right = 50;
    right_check(10);
    TEST_ASSERT_EQ_INT(ctx, 50, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -12800, scra_hz);

    scra_h_posit.w.h = 100;
    scralim_left = 150;
    left_check((Uint16)-10);
    TEST_ASSERT_EQ_INT(ctx, 150, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 12800, scra_hz);
}

static void test_scr31a_vertical_move_paths_and_limits(test_context *ctx) {
    reset_scr31a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    scra_v_count = 0;
    actwk[0].yposi.w.h = 230;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 106, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1536, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_v_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflaga.b.h);

    actwk[0].yposi.w.h = 0;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1536, scra_vz);

    scra_v_posit.w.h = 10;
    scra_vline = 80;
    actwk[0].yposi.w.h = 95;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 12, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, scra_vz);

    scra_v_posit.w.h = 1290;
    scra_vline = 96;
    actwk[0].mspeed.w = 3000;
    actwk[0].yposi.w.h = 1600;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 1296, scra_v_posit.w.h);

    scra_v_posit.w.h = 0;
    actwk[0].yposi.w.h = -400;
    sv_move_minus(4096);

    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -400, actwk[0].yposi.w.h);

    sv_move_sub(2);
    TEST_ASSERT_EQ_INT(ctx, 2, scra_v_posit.w.h);
    sv_move_sub((Uint16)-2);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
}

static void test_scr31a_vertical_cddat_and_limmove_paths(test_context *ctx) {
    reset_scr31a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].yposi.w.h = 150;
    actwk[0].cddat = 4;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 94, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1536, scra_vz);

    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 130;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 78, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_vz);

    limmoveflag = 1;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = scra_v_posit.w.h + scra_vline;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);

    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 245;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    scra_v_posit.w.h = 100;
    limmoveflag = 0;
    actwk[0].yposi.w.h = 180;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    limmoveflag = 1;
    actwk[0].yposi.w.h = 180;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
}

static void test_scr31a_direct_vertical_limit_helpers(test_context *ctx) {
    int_union pos;

    reset_scr31a_state();
    scralim_up = -1;
    actwk[0].yposi.w.h = -300;
    scra_v_posit.w.h = -300;
    scrb_v_posit.w.h = -300;
    pos.l = 0;
    pos.w.l = (Uint16)-300;

    scrv_up_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 32468, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1748, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 724, scrb_v_posit.w.h);

    reset_scr31a_state();
    scralim_up = 10;
    pos.l = 0;
    pos.w.l = 5;
    scrv_up_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 10, scra_v_posit.w.h);

    reset_scr31a_state();
    scralim_down = 2048;
    pos.l = 0;
    pos.w.l = 2048;
    scrv_down_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 2048, scra_v_posit.w.h);

    reset_scr31a_state();
    scralim_down = 2050;
    actwk[0].yposi.w.h = 3000;
    scra_v_posit.w.h = 3000;
    scrb_v_posit.w.h = 3000;
    pos.l = 0;
    pos.w.l = 2050;
    scrv_down_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 952, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 952, scrb_v_posit.w.h);
}

static void test_scr31a_vertical_speed_branches(test_context *ctx) {
    reset_scr31a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].mspeed.w = -3000;
    actwk[0].yposi.w.h = 210;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 114, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3584, scra_vz);

    scra_vline = 80;
    actwk[0].yposi.w.h = 190;
    sv_move_main1(110);
    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);

    sv_move_main1((Uint16)-5);
    TEST_ASSERT_EQ_INT(ctx, 114, scra_v_posit.w.h);

    sv_move_main1(1);
    TEST_ASSERT_EQ_INT(ctx, 115, scra_v_posit.w.h);

    sv_move_main2(8);
    TEST_ASSERT_EQ_INT(ctx, 123, scra_v_posit.w.h);

    sv_move_main(3);
    TEST_ASSERT_EQ_INT(ctx, 125, scra_v_posit.w.h);

    scra_vline = 96;
    actwk[0].mspeed.w = 0;
    sv_move_main(3);
    TEST_ASSERT_EQ_INT(ctx, 128, scra_v_posit.w.h);
}

static void test_scr31a_parallax_helpers_flag_edges(test_context *ctx) {
    int_union move;

    reset_scr31a_state();
    scrb_h_count = scrc_h_count = scrz_h_count = 16;

    scrollb_h(0x00100000, 4);
    scrollc_h(0x00100000, 16);
    scrollz_h(0x00100000, 64);

    TEST_ASSERT_EQ_INT(ctx, 0, scrb_h_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scrc_h_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scrz_h_count);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);
    TEST_ASSERT_EQ_INT(ctx, 32, scrflagc.b.h);
    TEST_ASSERT_EQ_INT(ctx, 128, scrflagz.b.h);

    scrflagb.b.h = 0;
    scrb_h_count = scrb_v_count = 16;
    scrb_h_posit.l = 0x00100000;
    scrb_v_posit.l = 0x00100000;
    move.l = -0x00100000;
    scrollb_hv(move, move);

    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    reset_scr31a_state();
    scrb_v_posit.w.h = 32;
    scrb_v_count = 16;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    scrflagb.b.h = 0;
    scrb_v_posit.w.h = 0;
    scrb_v_count = 16;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);

    scrflagb.b.h = 0;
    scrb_h_count = scrc_h_count = scrz_h_count = 0;
    scrb_h_posit.l = scrc_h_posit.l = scrz_h_posit.l = 0x00100000;
    scrollb_h(-0x00100000, 4);
    scrollc_h(-0x00100000, 16);
    scrollz_h(-0x00100000, 64);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scrflagc.b.h);
    TEST_ASSERT_EQ_INT(ctx, 64, scrflagz.b.h);

    reset_scr31a_state();
    scrb_h_count = scrb_v_count = 0;
    scrb_h_posit.l = scrb_v_posit.l = 0x00100000;
    move.l = -0x00100000;
    scrollb_hv(move, move);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);

    reset_scr31a_state();
    scrb_h_count = scrb_v_count = 0;
    move.l = 0x00100000;
    scrollb_hv(move, move);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);

    reset_scr31a_state();
    scrb_h_count = 16;
    move.l = 0x00100000;
    scrollb_hv(move, move);
    TEST_ASSERT_EQ_INT(ctx, 10, scrflagb.b.h);
}

static void test_scr31a_block_and_map_addressing(test_context *ctx) {
    POINT tile = {8, 10};
    Sint32 index = 0;
    Uint16 *block = 0;

    reset_scr31a_state();
    seed_block(7, 20);

    blockwrt(16384, &tile, 7 | 6144);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 6144, setgrid_frip[0]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 7 | 4096);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[1]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 7 | 2048);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[1]);

    setgrid_count = 0;
    blockwrt(24576, &tile, 7);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[0]);

    mapwka[0][0] = 2;
    map_work[256 + 2 + 3 * 16] = 0x3456;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset(0, 0, 32, 48, (Uint8 *)mapwka, &index));
    TEST_ASSERT_EQ_INT(ctx, 0x3456, index);

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset1(999, 0, 32, 48, (Uint8 *)mapwka, &index));
    TEST_ASSERT_EQ_INT(ctx, 0x3456, index);

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0x3456, index);
    TEST_ASSERT_TRUE(ctx, block == &map_work[256 + 2 + 3 * 16]);

    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset(0, 0, 256, 0, (Uint8 *)mapwka, &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[TEST_SCR31_HIGH_MAP_ROW][TEST_SCR31_HIGH_MAP_COL] = 3;
    map_work[(3 - 1) * 256 + 15 + 15 * 16] = 0x2222;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset(0, 0, 20000, 3000, (Uint8 *)mapwka, &index));
    TEST_ASSERT_EQ_INT(ctx, 0x2222, index);
}

static void test_scr31a_block_write_visible_and_hidden(test_context *ctx) {
    reset_scr31a_state();
    seed_block(9, 90);
    mapwka[0][0] = 1;
    scra_h_posit.w.h = 0;
    scra_v_posit.w.h = 0;

    block_wrt(9, 32, 48);

    TEST_ASSERT_EQ_INT(ctx, 9, map_work[2 + 3 * 16]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 90, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, block_chk(32, 48));

    setgrid_count = 0;
    block_wrt(10, 600, 600);

    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 1, block_chk(600, 600));
}

static void test_scr31a_scroll_builds_hscroll_and_water_band(test_context *ctx) {
    reset_scr31a_state();
    scr_set();
    actwk[0].xposi.w.h = 400;
    actwk[0].yposi.w.h = TEST_SCR31_SCROLL_ACT_Y;
    awasintbl[0] = 5;
    awasintbl[2] = 7;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, TEST_SCR31_SCROLL_VSCROLL_L, vscroll.w.l);
    TEST_ASSERT_EQ_INT(ctx, 64, waterdirec.w);
    TEST_ASSERT_TRUE(ctx, hscroll_buffer[0].l != 0);

    reset_scr31a_state();
    scroll_start.b.h = 1;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);

    reset_scr31a_state();
    scr_set();
    scra_v_posit.w.h = 1500;
    scrb_v_posit.w.h = 704;
    actwk[0].yposi.w.h = 1596;
    actwk[0].xposi.w.h = 260;
    limmoveflag = 1;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);

    reset_scr31a_state();
    scr_set();
    scra_v_posit.w.h = 1472;
    actwk[0].xposi.w.h = 260;
    actwk[0].yposi.w.h = 1568;
    limmoveflag = 1;
    awasintbl[0] = 9;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);

    reset_scr31a_state();
    scr_set();
    scra_v_posit.w.h = 0;
    actwk[0].xposi.w.h = 260;
    actwk[0].yposi.w.h = 96;
    limmoveflag = 1;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_posit.w.h);
}

static void test_scr31a_scrollwrt_and_mapwrt_emit_grid_writes(
    test_context *ctx) {
    reset_scr31a_state();
    seed_block(5, 50);
    mapwka[0][0] = 1;
    mapwkb[0][0] = 1;
    map_work[0] = 5;
    scrb_v_posit.w.h = 16;
    scrflagaw.b.h = 15;
    scrflagbw.b.h = 3;

    scrollwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagbw.b.h);

    setgrid_count = 0;
    scrb_v_posit.w.h = 16;
    mapwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    setgrid_count = 0;
    scrflagb.b.h = 2;
    scrb_v_posit.w.h = 16;
    scrollwrtadva();
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    setgrid_count = 0;
    scrflagb.b.h = 168;
    scrb_v_posit.w.h = 512;
    scrollwrtb(&scrflagb.b.h, (Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    setgrid_count = 0;
    scrflagb.b.h = 1;
    scrb_v_posit.w.h = 96;
    scrollwrtb(&scrflagb.b.h, (Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    setgrid_count = 0;
    scrflagb.b.h = 1;
    scrb_v_posit.w.h = 112;
    scrollwrtb(&scrflagb.b.h, (Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    scrollwrtc();
    scrollwrtz();

    setgrid_count = 0;
    actwk[0].xposi.w.h = 96;
    scrb_v_posit.w.h = 16;
    mapwrt_z11a((Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);
}

static void test_scr31a_direct_vram_and_zone_water_writer(test_context *ctx) {
    POINT tile = {0, 0};
    int_union *out;
    Uint16 *src;
    Uint16 bands[3] = {0, 16, 32767};
    Uint16 source[8] = {100, 101, 102, 103, 104, 105, 106, 107};
    Uint16 d2 = 0;
    Uint16 d4 = 0;

    reset_scr31a_state();
    vramadrset2(16, 32, 48, 64, &tile);

    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    awasintbl[0] = 3;
    awasintbl[1] = 5;
    out = hscroll_buffer;
    src = source;
    waterdirec.b.h = 0;
    zonescrsetsub0(&out, &src, bands, awasintbl, 2, &d2, &d4);

    TEST_ASSERT_TRUE(ctx, out > hscroll_buffer);
    TEST_ASSERT_TRUE(ctx, src > source);

    out = hscroll_buffer;
    src = source;
    bands[0] = 0;
    bands[1] = 50;
    bands[2] = 32767;
    d2 = 0;
    d4 = 100;
    zonescrsetsub0(&out, &src, bands, awasintbl, 1, &d2, &d4);

    TEST_ASSERT_TRUE(ctx, out >= hscroll_buffer);

    out = hscroll_buffer;
    src = source;
    bands[0] = 0;
    bands[1] = 90;
    bands[2] = 32767;
    d2 = 0;
    d4 = 100;
    zonescrsetsub0(&out, &src, bands, awasintbl, 1, &d2, &d4);

    TEST_ASSERT_TRUE(ctx, out > hscroll_buffer);

    out = hscroll_buffer;
    src = source;
    bands[0] = 0;
    bands[1] = 20;
    bands[2] = 32767;
    d2 = 0;
    d4 = 10;
    zonescrsetsub0(&out, &src, bands, awasintbl, 2, &d2, &d4);

    TEST_ASSERT_TRUE(ctx, out > hscroll_buffer);

    out = hscroll_buffer;
    src = source;
    bands[0] = 0;
    bands[1] = 0;
    bands[2] = 32767;
    d2 = 0;
    d4 = 10;
    zonescrsetsub0(&out, &src, bands, awasintbl, 2, &d2, &d4);

    TEST_ASSERT_TRUE(ctx, out > hscroll_buffer);
}

static void test_scr31a_mapinit_sets_colors_and_optional_device(test_context *ctx) {
    reset_scr31a_state();
    mapinittbl.colorno2 = 12;
    plflag = 1;
    play_start = 2;

    mapinit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 12, colorset_arg);
    TEST_ASSERT_EQ_INT(ctx, 12, colorset2_arg);
}

TEST_MAIN_BEGIN;
test_scr31a_scr_set_default_and_saved_positions(&ctx);
test_scr31a_scr_set_uses_end_demo_position(&ctx);
test_scr31a_position_edge_cases(&ctx);
test_scr31a_scroll_horizontal_paths(&ctx);
test_scr31a_vertical_move_paths_and_limits(&ctx);
test_scr31a_vertical_cddat_and_limmove_paths(&ctx);
test_scr31a_direct_vertical_limit_helpers(&ctx);
test_scr31a_vertical_speed_branches(&ctx);
test_scr31a_parallax_helpers_flag_edges(&ctx);
test_scr31a_block_and_map_addressing(&ctx);
test_scr31a_block_write_visible_and_hidden(&ctx);
test_scr31a_scroll_builds_hscroll_and_water_band(&ctx);
test_scr31a_scrollwrt_and_mapwrt_emit_grid_writes(&ctx);
test_scr31a_direct_vram_and_zone_water_writer(&ctx);
test_scr31a_mapinit_sets_colors_and_optional_device(&ctx);
TEST_MAIN_END

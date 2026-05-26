#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

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
Uint8 scrz_h_count;
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
Sint16 hscrollwork[256];
static int_union hscroll_buffer[512];
int_union *lphscrollbuff = hscroll_buffer;
Uint8 mapwka[8][64];
Uint8 mapwkb[8][64];
static Uint16 map_work[512];
Uint16 *pmapwk = map_work;
Uint16 blockwk[1024][4];
map_init_data mapinittbl;
Sint32 (*SetGrid)(Sint32, Sint32, Sint32, Sint32, Sint32);

static int playload_count;
static int scrchk_count;
static int colorset_count;
static int colorset2_count;
static int sinset_count;
static Sint32 colorset_arg;
static Sint32 colorset2_arg;
static int setgrid_count;
static Sint32 setgrid_base[16];
static Sint32 setgrid_x[16];
static Sint32 setgrid_y[16];
static Sint32 setgrid_block[16];
static Sint32 setgrid_frip[16];

void playload(void);
void scrchk(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos);

#include "src/r6/scr62b.c"

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
void sinset(Uint8 kakudo, Sint16 *sin, Sint16 *cos) {
    (void)kakudo;
    ++sinset_count;
    *sin = 0;
    *cos = 0;
}

static Sint32 test_setgrid(Sint32 base, Sint32 x, Sint32 y, Sint32 block,
                           Sint32 frip) {
    if (setgrid_count < 16) {
        setgrid_base[setgrid_count] = base;
        setgrid_x[setgrid_count] = x;
        setgrid_y[setgrid_count] = y;
        setgrid_block[setgrid_count] = block;
        setgrid_frip[setgrid_count] = frip;
    }
    ++setgrid_count;
    return 0;
}

static void reset_scr62b_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(mapwkb, 0, sizeof(mapwkb));
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
    scrc_h_count = scrz_h_count = 0;
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
    lphscrollbuff = hscroll_buffer;
    pmapwk = map_work;
    SetGrid = test_setgrid;
    playload_count = 0;
    scrchk_count = 0;
    colorset_count = colorset2_count = 0;
    sinset_count = 0;
    colorset_arg = colorset2_arg = 0;
    setgrid_count = 0;
    memset(setgrid_base, 0, sizeof(setgrid_base));
    memset(setgrid_x, 0, sizeof(setgrid_x));
    memset(setgrid_y, 0, sizeof(setgrid_y));
    memset(setgrid_block, 0, sizeof(setgrid_block));
    memset(setgrid_frip, 0, sizeof(setgrid_frip));
}

static void test_scr62b_scr_set_and_saved_position(test_context *ctx) {
    reset_scr62b_state();

    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[0]);
    main_play = 1;
    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[1]);

    reset_scr62b_state();
    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 9111, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 2384, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 412, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 316, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 79, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 79, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 79, scrz_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno2);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno2);

    reset_scr62b_state();
    plflag = 1;
    scralim_right = 500;
    scralim_down = 200;
    actwk[0].xposi.w.h = 900;
    actwk[0].yposi.w.h = 500;

    playposiset();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 500, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 200, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 93, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 250, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 125, scrz_h_posit.w.h);

    reset_scr62b_state();
    demoflag.w = (Sint16)0x8000;
    enddemono = 2;
    scralim_right = 8855;
    scralim_down = 1808;

    playposiset();

    TEST_ASSERT_EQ_INT(ctx, 3744, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1132, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3584, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1036, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 672, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1792, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 896, scrz_h_posit.w.h);
}

static void test_scr62b_horizontal_and_vertical_scroll(test_context *ctx) {
    int_union pos;

    reset_scr62b_state();
    scralim_right = 1000;
    scralim_left = 100;
    scra_hline = 160;
    scra_h_posit.w.h = 200;
    actwk[0].xposi.w.h = 400;

    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 216, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);

    actwk[0].xposi.w.h = 100;
    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 200, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].yposi.w.h = 220;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 106, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1536, scra_vz);

    actwk[0].yposi.w.h = 80;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1536, scra_vz);

    pos.l = 0;
    pos.w.l = -300;
    scrv_up_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 1748, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 80, actwk[0].yposi.w.h);
}

static void test_scr62b_parallax_and_hscroll_lines(test_context *ctx) {
    Sint16 *line_ptr;

    reset_scr62b_state();
    scrb_v_count = 16;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);

    reset_scr62b_state();
    scrb_h_count = 16;
    scrollb_h(16 << 16, 2);
    TEST_ASSERT_EQ_INT(ctx, 16, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);

    scrc_h_count = 16;
    scrollc_h(16 << 16, 4);
    TEST_ASSERT_EQ_INT(ctx, 16, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32, scrflagc.b.h);

    scrz_h_count = 16;
    scrollz_h(16 << 16, 6);
    TEST_ASSERT_EQ_INT(ctx, 16, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, scrflagz.b.h);

    reset_scr62b_state();
    scrb_v_posit.w.h = 16;
    scrb_v_count = 0;
    scrollb_v(0);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    reset_scr62b_state();
    scrb_h_posit.w.h = 16;
    scrb_h_count = 0;
    scrollb_h(-(16 << 16), 2);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);

    reset_scr62b_state();
    scra_h_posit.w.h = 100;
    scrb_h_posit.w.h = 40;
    line_ptr = hscrollwork;

    z81aline(&line_ptr);

    TEST_ASSERT_TRUE(ctx, line_ptr == hscrollwork + 28);
    TEST_ASSERT_EQ_INT(ctx, 0, sinset_count);
    TEST_ASSERT_EQ_INT(ctx, -97, hscrollwork[0]);
    TEST_ASSERT_EQ_INT(ctx, -40, hscrollwork[27]);

    hscrollwork[0] = -11;
    hscrollwork[1] = -22;
    hscrollwork[2] = -33;
    line_ptr = hscrollwork;
    {
        int_union *buf = hscroll_buffer;
        zonescrsetsub0(6, 1, &buf, line_ptr);
    }

    TEST_ASSERT_EQ_INT(ctx, -11, hscroll_buffer[0].w.l);
    TEST_ASSERT_EQ_INT(ctx, -11, hscroll_buffer[1].w.l);
    TEST_ASSERT_EQ_INT(ctx, -22, hscroll_buffer[2].w.l);
    TEST_ASSERT_EQ_INT(ctx, -22, hscroll_buffer[9].w.l);
}

static void test_scr62b_map_and_block_helpers(test_context *ctx) {
    Sint32 index;
    Uint16 *block_ptr;
    POINT tile;

    reset_scr62b_state();
    mapwka[0][0] = 1;
    pmapwk[50] = 777;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset(0, 0, 32, 48, &mapwka[0][0], &index));
    TEST_ASSERT_EQ_INT(ctx, 777, index);

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset1(999, 0, 32, 48, &mapwka[0][0], &index));
    TEST_ASSERT_EQ_INT(ctx, 777, index);

    memset(mapwka, 0, sizeof(mapwka));
    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset(0, 0, 32, 48, &mapwka[0][0], &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 1;
    pmapwk[17] = 333;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(16, 16, &mapwka[0][0], &index, &block_ptr));
    TEST_ASSERT_EQ_INT(ctx, 333, index);
    TEST_ASSERT_TRUE(ctx, block_ptr == &pmapwk[17]);

    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;
    tile.x = 0;
    tile.y = 0;

    blockwrt(16384, &tile, 5 | 6144);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[3]);

    vramadrset(16, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);
}

static void test_scr62b_scroll_write_and_mapinit(test_context *ctx) {
    Uint8 flag;

    reset_scr62b_state();
    blockwk[0][0] = 21;
    blockwk[0][1] = 22;
    blockwk[0][2] = 23;
    blockwk[0][3] = 24;

    mapwrt2(0, 0, &mapwka[0][0], 16384);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[0]);

    reset_scr62b_state();
    blockwk[0][0] = 31;
    blockwk[0][1] = 32;
    blockwk[0][2] = 33;
    blockwk[0][3] = 34;
    scrb_v_posit.w.h = 23 * 16;
    flag = 64;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr62b_state();
    blockwk[0][0] = 41;
    blockwk[0][1] = 42;
    blockwk[0][2] = 43;
    blockwk[0][3] = 44;
    scrflagaw.b.h = 15;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    mapinittbl.colorno2 = 7;
    play_start = 2;

    mapinit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset_arg);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset2_arg);
}

static void test_scr62b_scroll_wrapper_and_extra_edges(test_context *ctx) {
    POINT tile;

    reset_scr62b_state();
    scroll_start.b.h = 1;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);

    reset_scr62b_state();
    scralim_right = 1000;
    scralim_down = 1808;
    scra_hline = 160;
    scra_vline = 96;
    scra_h_count = 16;
    scra_v_count = 16;
    actwk[0].xposi.w.h = 400;
    actwk[0].yposi.w.h = 300;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, scra_v_posit.w.h, vscroll.w.h);
    TEST_ASSERT_EQ_INT(ctx, scrb_v_posit.w.h, vscroll.w.l);

    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 120;
    limmoveflag = 1;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_vz);

    reset_scr62b_state();
    blockwk[0][0] = 51;
    blockwk[0][1] = 52;
    blockwk[0][2] = 53;
    blockwk[0][3] = 54;
    tile.x = 0;
    tile.y = 30;

    vblockwrt(&tile, 16384, 0, 0, 0, 0, &mapwka[0][0], 0);

    TEST_ASSERT_EQ_INT(ctx, 0, tile.y);
}

static void test_scr62b_additional_position_and_scroll_edges(test_context *ctx) {
    int_union pos;

    reset_scr62b_state();
    plflag = 1;
    scralim_right = 1000;
    scralim_down = 1000;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = -5;

    playposiset();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr62b_state();
    scra_h_posit.w.h = 100;
    scra_hline = 160;
    actwk[0].xposi.w.h = 260;
    scra_hz = 1234;

    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);

    reset_scr62b_state();
    scralim_right = 1000;
    scralim_left = 0;
    scra_hline = 160;
    scra_h_count = 16;
    scra_h_posit.w.h = 15;
    actwk[0].xposi.w.h = 200;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 31, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflaga.b.h);

    scrflaga.w = 0;
    actwk[0].xposi.w.h = 0;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 15, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);

    reset_scr62b_state();
    scralim_right = 1000;
    scralim_left = 100;
    scra_h_posit.w.h = 105;
    scra_hline = 160;
    actwk[0].xposi.w.h = 0;

    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);

    scra_h_posit.w.h = 995;
    actwk[0].xposi.w.h = 2000;

    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 1000, scra_h_posit.w.h);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].mspeed.w = -2048;

    sv_move_main(20);

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 96;
    scra_v_posit.w.h = 100;

    sv_move_main(3);

    TEST_ASSERT_EQ_INT(ctx, 103, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 972, scra_vz);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main(-3);

    TEST_ASSERT_EQ_INT(ctx, 98, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, scra_vz);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main(3);

    TEST_ASSERT_EQ_INT(ctx, 102, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, scra_vz);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main(1);

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_v_posit.w.h = 100;

    sv_move_main2(4);

    TEST_ASSERT_EQ_INT(ctx, 104, scra_v_posit.w.h);

    reset_scr62b_state();
    scralim_down = 1296;
    scra_v_posit.w.h = 1200;
    scrb_v_posit.w.h = 900;
    actwk[0].yposi.w.h = 2200;
    pos.l = 0;
    pos.w.l = 2300;

    scrv_down_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 252, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 152, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 900, scrb_v_posit.w.h);

    reset_scr62b_state();
    scralim_up = 10;
    scra_v_posit.w.h = 100;
    pos.l = 0;
    pos.w.l = 5;

    scrv_up_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 10, scra_v_posit.w.h);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_v_posit.w.h = 1700;
    pos.l = 0;
    pos.w.l = 1900;

    scrv_down_ch(pos);

    TEST_ASSERT_EQ_INT(ctx, 1808, scra_v_posit.w.h);

    reset_scr62b_state();
    scralim_down = 1808;
    scra_v_posit.w.h = 100;

    sv_move_sub(-1);

    TEST_ASSERT_EQ_INT(ctx, 99, scra_v_posit.w.h);
}

static void test_scr62b_vertical_pose_branches(test_context *ctx) {
    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 300;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 220;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].yposi.w.h = 196;
    limmoveflag = 1;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);

    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 220;
    limmoveflag = 1;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);

    reset_scr62b_state();
    scra_vline = 96;
    scralim_down = 1808;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 201;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
}

static void test_scr62b_blockwrt_flip_orders(test_context *ctx) {
    POINT tile;

    reset_scr62b_state();
    blockwk[6][0] = 20;
    blockwk[6][1] = 21;
    blockwk[6][2] = 22;
    blockwk[6][3] = 23;
    tile.x = 2;
    tile.y = 4;

    blockwrt(24576, &tile, 6 | 4096);

    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[3]);

    reset_scr62b_state();
    blockwk[7][0] = 30;
    blockwk[7][1] = 31;
    blockwk[7][2] = 32;
    blockwk[7][3] = 33;

    blockwrt(16384, &tile, 7 | 2048);

    TEST_ASSERT_EQ_INT(ctx, 31, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 30, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 33, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 32, setgrid_block[3]);

    reset_scr62b_state();
    blockwk[8][0] = 40;
    blockwk[8][1] = 41;
    blockwk[8][2] = 42;
    blockwk[8][3] = 43;

    blockwrt(16384, &tile, 8);

    TEST_ASSERT_EQ_INT(ctx, 40, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 41, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 42, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 43, setgrid_block[3]);
}

static void test_scr62b_map_block_edges_and_noops(test_context *ctx) {
    Sint32 index;
    Uint16 *block_ptr;
    POINT tile;

    reset_scr62b_state();
    mapwka[7][63] = 1;
    pmapwk[255] = 444;

    TEST_ASSERT_EQ_INT(
        ctx, 1,
        mapadrset(16383, 2047, 1000, 1000, &mapwka[0][0], &index));
    TEST_ASSERT_EQ_INT(ctx, 444, index);

    memset(mapwka, 0, sizeof(mapwka));
    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset2(16, 16, &mapwka[0][0], &index, &block_ptr));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    vramadrset1(999, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    vramadrset0(48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.y);

    vramadrset2(16, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    scrollwrtc();
    scrollwrtz();
    mapset();
    enecginit();
}

static void test_scr62b_scroll_write_helper_paths(test_context *ctx) {
    Uint8 flag;
    POINT tile;

    reset_scr62b_state();
    blockwk[0][0] = 61;
    blockwk[0][1] = 62;
    blockwk[0][2] = 63;
    blockwk[0][3] = 64;
    scrb_v_posit.w.h = 16;
    flag = 1;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 65;
    blockwk[0][1] = 66;
    blockwk[0][2] = 67;
    blockwk[0][3] = 68;
    flag = 2;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 69;
    blockwk[0][1] = 70;
    blockwk[0][2] = 71;
    blockwk[0][3] = 72;
    scrb_v_posit.w.h = 23 * 16;
    flag = 168;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr62b_state();
    blockwk[0][0] = 73;
    blockwk[0][1] = 74;
    blockwk[0][2] = 75;
    blockwk[0][3] = 76;

    mapwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 77;
    blockwk[0][1] = 78;
    blockwk[0][2] = 79;
    blockwk[0][3] = 80;
    mapwrt_z81a(&mapwkb[0][0], 24576);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 81;
    blockwk[0][1] = 82;
    blockwk[0][2] = 83;
    blockwk[0][3] = 84;
    mapwrt_sub(&z81awrttbl[1], 23 * 16, 0, &mapwkb[0][0], 24576);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr62b_state();
    blockwk[0][0] = 85;
    blockwk[0][1] = 86;
    blockwk[0][2] = 87;
    blockwk[0][3] = 88;
    tile.x = 62;
    tile.y = 0;

    hblockwrt1(&tile, 16384, 0, 0, 0, 0, &mapwka[0][0], 0);

    TEST_ASSERT_EQ_INT(ctx, 0, tile.x);
}

static void test_scr62b_negative_parallax_and_writer_edges(test_context *ctx) {
    Uint8 flag;

    reset_scr62b_state();
    scrc_h_posit.w.h = 16;
    scrc_h_count = 0;

    scrollc_h(-(16 << 16), 4);

    TEST_ASSERT_EQ_INT(ctx, 16, scrflagc.b.h);

    reset_scr62b_state();
    scrz_h_posit.w.h = 16;
    scrz_h_count = 0;

    scrollz_h(-(16 << 16), 6);

    TEST_ASSERT_EQ_INT(ctx, 64, scrflagz.b.h);

    reset_scr62b_state();
    blockwk[0][0] = 121;
    blockwk[0][1] = 122;
    blockwk[0][2] = 123;
    blockwk[0][3] = 124;
    scrb_v_posit.w.h = 256;
    flag = 1;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr62b_state();
    blockwk[0][0] = 125;
    blockwk[0][1] = 126;
    blockwk[0][2] = 127;
    blockwk[0][3] = 128;
    scrb_v_posit.w.h = 2000;
    flag = 64;

    scrollwrtb(&flag, &mapwkb[0][0], 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);

    reset_scr62b_state();
    TEST_ASSERT_EQ_INT(ctx, 1, block_chk(0, 0));
}

static void test_scr62b_scrollwrt_foreground_and_block_write(
    test_context *ctx) {
    reset_scr62b_state();
    blockwk[0][0] = 91;
    blockwk[0][1] = 92;
    blockwk[0][2] = 93;
    blockwk[0][3] = 94;
    scrflagaw.b.h = 1;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 95;
    blockwk[0][1] = 96;
    blockwk[0][2] = 97;
    blockwk[0][3] = 98;
    scrflagaw.b.h = 2;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 99;
    blockwk[0][1] = 100;
    blockwk[0][2] = 101;
    blockwk[0][3] = 102;
    scrflagaw.b.h = 4;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    blockwk[0][0] = 103;
    blockwk[0][1] = 104;
    blockwk[0][2] = 105;
    blockwk[0][3] = 106;
    scrflagaw.b.h = 8;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr62b_state();
    mapwka[1][1] = 1;
    pmapwk[41] = 7;
    blockwk[9][0] = 111;
    blockwk[9][1] = 112;
    blockwk[9][2] = 113;
    blockwk[9][3] = 114;
    scra_v_posit.w.h = -16;
    scra_h_posit.w.h = -16;

    block_wrt(9, 400, 300);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);
    TEST_ASSERT_EQ_INT(ctx, 111, setgrid_block[0]);
}

TEST_MAIN_BEGIN;
    test_scr62b_scr_set_and_saved_position(&ctx);
    test_scr62b_horizontal_and_vertical_scroll(&ctx);
    test_scr62b_parallax_and_hscroll_lines(&ctx);
    test_scr62b_map_and_block_helpers(&ctx);
    test_scr62b_scroll_write_and_mapinit(&ctx);
    test_scr62b_scroll_wrapper_and_extra_edges(&ctx);
    test_scr62b_additional_position_and_scroll_edges(&ctx);
    test_scr62b_vertical_pose_branches(&ctx);
    test_scr62b_blockwrt_flip_orders(&ctx);
    test_scr62b_map_block_edges_and_noops(&ctx);
    test_scr62b_scroll_write_helper_paths(&ctx);
    test_scr62b_negative_parallax_and_writer_edges(&ctx);
    test_scr62b_scrollwrt_foreground_and_block_write(&ctx);
TEST_MAIN_END;

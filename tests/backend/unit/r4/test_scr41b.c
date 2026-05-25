#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

Uint8 main_play;
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
Uint8 mapwka[8][64];
Uint8 mapwkb[8][64];
Uint8 awasintbl[256];
static Uint16 map_work[512];
Uint16 *pmapwk = map_work;
Uint16 blockwk[1024][4];
map_init_data mapinittbl;
Sint32 (*SetGrid)(Sint32, Sint32, Sint32, Sint32, Sint32);

static int playload_count;
static int scrchk_count;
static int colorset_count;
static Sint32 colorset_arg;
static int colorset2_count;
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

#include "src/r4/scr41b.c"

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

static void reset_scr41b_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(mapwkb, 0, sizeof(mapwkb));
    memset(awasintbl, 0, sizeof(awasintbl));
    memset(map_work, 0, sizeof(map_work));
    memset(blockwk, 0, sizeof(blockwk));
    memset(hscrollwork, 0, sizeof(hscrollwork));
    memset(hscroll_buffer, 0, sizeof(hscroll_buffer));
    memset(&mapinittbl, 0, sizeof(mapinittbl));
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
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(&scrb_h_posit, 0, sizeof(scrb_h_posit));
    memset(&scrb_v_posit, 0, sizeof(scrb_v_posit));
    memset(&scrc_h_posit, 0, sizeof(scrc_h_posit));
    memset(&scrc_v_posit, 0, sizeof(scrc_v_posit));
    memset(&scrz_h_posit, 0, sizeof(scrz_h_posit));
    memset(&scrz_v_posit, 0, sizeof(scrz_v_posit));
    memset(&scra_h_posiw, 0, sizeof(scra_h_posiw));
    memset(&scra_v_posiw, 0, sizeof(scra_v_posiw));
    memset(&scrb_h_posiw, 0, sizeof(scrb_h_posiw));
    memset(&scrb_v_posiw, 0, sizeof(scrb_v_posiw));
    memset(&scrc_h_posiw, 0, sizeof(scrc_h_posiw));
    memset(&scrc_v_posiw, 0, sizeof(scrc_v_posiw));
    memset(&scrz_h_posiw, 0, sizeof(scrz_h_posiw));
    memset(&scrz_v_posiw, 0, sizeof(scrz_v_posiw));
    memset(&vscroll, 0, sizeof(vscroll));
    memset(&waterdirec, 0, sizeof(waterdirec));
    waterposi = 0;
    lphscrollbuff = hscroll_buffer;
    pmapwk = map_work;
    SetGrid = test_setgrid;
    playload_count = 0;
    scrchk_count = 0;
    colorset_count = 0;
    colorset_arg = 0;
    colorset2_count = 0;
    colorset2_arg = 0;
    setgrid_count = 0;
    memset(setgrid_base, 0, sizeof(setgrid_base));
    memset(setgrid_x, 0, sizeof(setgrid_x));
    memset(setgrid_y, 0, sizeof(setgrid_y));
    memset(setgrid_block, 0, sizeof(setgrid_block));
    memset(setgrid_frip, 0, sizeof(setgrid_frip));
}

static void seed_screen_zero(Uint8 *map) {
    Sint32 i;

    map[0] = 1;
    for (i = 0; i < 16 * 16; ++i) {
        map_work[i] = (Uint16)(i & 1023);
        blockwk[i & 1023][0] = (Uint16)(100 + i);
        blockwk[i & 1023][1] = (Uint16)(200 + i);
        blockwk[i & 1023][2] = (Uint16)(300 + i);
        blockwk[i & 1023][3] = (Uint16)(400 + i);
    }
}

static void test_scr41b_main_chk_and_start_positions(test_context *ctx) {
    reset_scr41b_state();
    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[0]);
    main_play = 1;
    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[1]);

    reset_scr41b_state();
    scr_set();
    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 7319, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 80, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr41b_state();
    demoflag.w = -32768;
    enddemono = 2;
    scr_set();
    TEST_ASSERT_EQ_INT(ctx, 5968, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 189, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5808, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 93, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 46, scrb_v_posit.w.h);

    reset_scr41b_state();
    plflag = 1;
    actwk[0].xposi.w.h = 9000;
    actwk[0].yposi.w.h = -12;
    scr_set();
    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 7319, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
}

static void test_scr41b_addressing_and_grid_writes(test_context *ctx) {
    POINT tile = {4, 6};
    Sint32 index = 123;
    Uint16 *block = (Uint16 *)1;

    reset_scr41b_state();
    TEST_ASSERT_EQ_INT(ctx, 0, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 2;
    map_work[256 + 2 + 3 * 16] = 0x1234;
    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0x1234, index);

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_TRUE(ctx, block == &map_work[256 + 2 + 3 * 16]);

    vramadrset(32, 48, 16, 32, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);
    vramadrset1(999, 48, 16, 32, &tile);
    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);

    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;
    blockwrt(16384, &tile, 5);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[3]);

    setgrid_count = 0;
    blockwrt(24576, &tile, 5 | 6144);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[3]);
}

static void test_scr41b_horizontal_and_vertical_scroll_helpers(
    test_context *ctx) {
    int_union dx;
    int_union dy;

    reset_scr41b_state();
    scralim_left = 0;
    scralim_right = 7319;
    scra_hline = 160;
    scra_h_posit.w.h = 100;
    scra_h_count = 0;
    actwk[0].xposi.w.h = 120;
    scroll_h();
    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);

    scra_h_posit.w.h = 0;
    scra_h_count = 0;
    actwk[0].xposi.w.h = 400;
    scroll_h();
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflaga.b.h & 8);

    reset_scr41b_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].yposi.w.h = 230;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 106, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1536, scra_vz);

    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 130;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 90, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_vz);

    reset_scr41b_state();
    memset(&dx, 0, sizeof(dx));
    memset(&dy, 0, sizeof(dy));
    dx.l = 0x00100000;
    dy.l = 0x00100000;
    scrb_h_count = 16;
    scrb_v_count = 16;
    scrollb_hv(dx.l, dy.l);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h & 8);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h & 2);

    reset_scr41b_state();
    scrb_h_count = 16;
    scrb_v_count = 16;
    scrb_v_posit.l = 0x00200000;
    dx.l = -0x00100000;
    dy.l = -0x00100000;
    scrollb_hv(dx.l, dy.l);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h & 4);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h & 1);
}

static void test_scr41b_extra_scroll_helper_branches(test_context *ctx) {
    int_union move;

    reset_scr41b_state();
    plflag = 1;
    scralim_down = 100;
    actwk[0].xposi.w.h = 200;
    actwk[0].yposi.w.h = 5000;
    playposiset();
    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);

    reset_scr41b_state();
    scra_h_posit.w.h = 40;
    scra_hline = 160;
    actwk[0].xposi.w.h = 200;
    scroll_h();
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);

    reset_scr41b_state();
    scralim_right = 100;
    scra_h_posit.w.h = 90;
    right_check(100);
    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2560, scra_hz);

    scra_h_posit.w.h = 100;
    right_check((Uint16)-1);
    TEST_ASSERT_EQ_INT(ctx, 99, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -256, scra_hz);

    reset_scr41b_state();
    scralim_left = 0;
    scra_h_posit.w.h = 5;
    left_check((Uint16)-100);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1280, scra_hz);

    scra_h_posit.w.h = 100;
    left_check(1);
    TEST_ASSERT_EQ_INT(ctx, 101, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_hz);

    reset_scr41b_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 80;
    actwk[0].yposi.w.h = 200;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 2, scrflaga.b.h & 2);

    reset_scr41b_state();
    scra_vline = 96;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 101;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scra_vline = 96;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 200;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scra_vline = 96;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 100;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scra_vline = 96;
    limmoveflag = 1;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 100;
    scroll_v();
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);

    reset_scr41b_state();
    scralim_up = 0;
    scra_vline = 96;
    actwk[0].mspeed.w = -2048;
    sv_move_main((Uint16)-20);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scra_vline = 96;
    sv_move_main((Uint16)-7);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scralim_up = 0;
    scra_vline = 96;
    sv_move_main(3);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr41b_state();
    scra_vline = 80;
    sv_move_main1((Uint16)-3);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scra_vline = 80;
    sv_move_main1(1);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr41b_state();
    sv_move_main2(5);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr41b_state();
    sv_move_main2(20);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41b_state();
    scralim_up = 0;
    actwk[0].yposi.w.h = -100;
    scra_v_posit.w.h = -100;
    scrb_v_posit.w.h = -100;
    memset(&move, 0, sizeof(move));
    move.w.l = -300;
    scrv_up_ch(move);
    TEST_ASSERT_EQ_INT(ctx, 1748, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1948, actwk[0].yposi.w.h);

    reset_scr41b_state();
    scralim_down = 100;
    actwk[0].yposi.w.h = 4095;
    scra_v_posit.w.h = 4095;
    scrb_v_posit.w.h = 1025;
    memset(&move, 0, sizeof(move));
    move.w.l = 2200;
    scrv_down_ch(move);
    TEST_ASSERT_EQ_INT(ctx, 2047, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 152, scra_v_posit.w.h);

    reset_scr41b_state();
    scroll_start.b.h = 1;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);

    reset_scr41b_state();
    scrb_v_count = 16;
    scrollb_hv(0, -1);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_posit.l);
}

static void test_scr41b_layer_scroll_helpers(test_context *ctx) {
    reset_scr41b_state();
    scrb_v_count = 16;
    scrb_v_posit.w.h = 32;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h & 1);

    reset_scr41b_state();
    scrb_v_count = 16;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h & 2);

    reset_scr41b_state();
    scrb_h_count = 16;
    scrollb_h(0x00100000, 4);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h & 8);
    reset_scr41b_state();
    scrb_h_count = 16;
    scrb_h_posit.l = 0x00300000;
    scrollb_h(-0x00200000, 4);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h & 4);

    reset_scr41b_state();
    scrc_h_count = 16;
    scrollc_h(0x00100000, 4);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagc.b.h & 8);
    reset_scr41b_state();
    scrc_h_count = 16;
    scrc_h_posit.l = 0x00300000;
    scrollc_h(-0x00200000, 4);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagc.b.h & 4);

    reset_scr41b_state();
    scrz_h_count = 16;
    scrollz_h(0x00100000, 4);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagz.b.h & 8);
    reset_scr41b_state();
    scrz_h_count = 16;
    scrz_h_posit.l = 0x00300000;
    scrollz_h(-0x00200000, 4);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagz.b.h & 4);

    reset_scr41b_state();
    scrb_v_count = 16;
    scrb_v_posit.l = 0x00200000;
    scrollb_hv(0, -0x00100000);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h & 1);
}

static void test_scr41b_scroll_and_map_write_paths(test_context *ctx) {
    reset_scr41b_state();
    scr_set();
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 96;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, -13108, hscroll_buffer[0].w.h);

    reset_scr41b_state();
    hscrollwork[0] = 11;
    hscrollwork[1] = 22;
    waterposi = 10000;
    zonescrsetsub0(3, 7, 0);
    TEST_ASSERT_EQ_INT(ctx, 11, hscroll_buffer[0].w.l);
    TEST_ASSERT_EQ_INT(ctx, 22, hscroll_buffer[1].w.l);

    reset_scr41b_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);
    scrflagaw.b.h = 15;
    scrflagbw.b.h = 15;
    scrollwrt();
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagbw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr41b_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrflagb.b.h = 15;
    scrollwrtadva();
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagb.b.h);

    reset_scr41b_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);
    mapwrt();
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr41b_state();
    mapwka[0][0] = 1;
    blockwk[12][0] = 90;
    blockwk[12][1] = 91;
    blockwk[12][2] = 92;
    blockwk[12][3] = 93;
    scra_h_posit.w.h = -16;
    scra_v_posit.w.h = -256;
    block_wrt(12, 64, 0);
    TEST_ASSERT_EQ_INT(ctx, 12, map_work[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
}

static void test_scr41b_extra_map_and_block_paths(test_context *ctx) {
    POINT tile = {62, 30};
    Sint32 index = 77;
    Uint16 *block = (Uint16 *)1;

    reset_scr41b_state();
    mapwka[0][0] = 1;
    map_work[3 + 4 * 16] = 0x0055;
    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset1(999, 0, 48, 64, (Uint8 *)mapwka,
                                          &index));
    TEST_ASSERT_EQ_INT(ctx, 0x0055, index);

    mapwka[0][0] = 0;
    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset2(0, 0, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 1;
    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(0, 0, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_TRUE(ctx, block == &map_work[0]);

    vramadrset2(16, 32, 16, 16, &tile);
    TEST_ASSERT_EQ_INT(ctx, 4, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.y);

    TEST_ASSERT_EQ_INT(ctx, 0, mapadrset(0, 0, 0x4000, 0x0800,
                                         (Uint8 *)mapwka, &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    blockwk[6][0] = 20;
    blockwk[6][1] = 21;
    blockwk[6][2] = 22;
    blockwk[6][3] = 23;
    setgrid_count = 0;
    blockwrt(16384, &tile, 6 | 4096);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[3]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 6 | 2048);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[3]);

    reset_scr41b_state();
    seed_screen_zero((Uint8 *)mapwka);
    tile.x = 62;
    tile.y = 30;
    hblockwrt1(&tile, 16384, 99, 0, 0, 0, (Uint8 *)mapwka, 1);
    TEST_ASSERT_EQ_INT(ctx, 8, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);

    reset_scr41b_state();
    seed_screen_zero((Uint8 *)mapwka);
    scra_h_posit.w.h = 0;
    scra_v_posit.w.h = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, block_chk(1000, 1000));

    reset_scr41b_state();
    mapinittbl.colorno2 = 5;
    plflag = 1;
    mapinit();
    TEST_ASSERT_EQ_INT(ctx, 5, colorset_arg);
}

static void test_scr41b_mapinit_and_noop_entry_points(test_context *ctx) {
    reset_scr41b_state();
    mapinittbl.colorno2 = 7;
    play_start = 2;

    mapinit();
    scrollwrtc();
    scrollwrtz();
    mapset();
    divdevset();
    enecginit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset2_arg);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset_arg);
}

TEST_MAIN_BEGIN;
test_scr41b_main_chk_and_start_positions(&ctx);
test_scr41b_addressing_and_grid_writes(&ctx);
test_scr41b_horizontal_and_vertical_scroll_helpers(&ctx);
test_scr41b_extra_scroll_helper_branches(&ctx);
test_scr41b_layer_scroll_helpers(&ctx);
test_scr41b_scroll_and_map_write_paths(&ctx);
test_scr41b_extra_map_and_block_paths(&ctx);
test_scr41b_mapinit_and_noop_entry_points(&ctx);
TEST_MAIN_END

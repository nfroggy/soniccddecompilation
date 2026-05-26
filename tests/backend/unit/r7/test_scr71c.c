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
static int colorset2_count;
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

#include "src/r7/scr71c.c"

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

static void reset_scr71c_state(void) {
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
    SetGrid = test_setgrid;
}

static void test_scr71c_scr_set_and_play_position_defaults(test_context *ctx) {
    reset_scr71c_state();

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 11927, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_up);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 576, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_v_count);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 396, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 300, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 132, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 134, loopmapno2);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno2);
}

static void test_scr71c_playposiset_loads_and_clamps_saved_position(
    test_context *ctx) {
    reset_scr71c_state();
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
    TEST_ASSERT_EQ_INT(ctx, 187, scrz_h_posit.w.h);
}

static void test_scr71c_horizontal_scroll_clamps_and_sets_speed(
    test_context *ctx) {
    reset_scr71c_state();
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

    scra_h_posit.w.h = 995;
    right_check(100);

    TEST_ASSERT_EQ_INT(ctx, 1000, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1280, scra_hz);

    scra_h_posit.w.h = 105;
    left_check((Uint16)-100);

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1280, scra_hz);
}

static void test_scr71c_scroll_h_flags_direction_changes(test_context *ctx) {
    reset_scr71c_state();
    scralim_right = 1000;
    scralim_left = 0;
    scra_hline = 160;
    scra_h_count = 0;
    scra_h_posit.w.h = 15;
    actwk[0].xposi.w.h = 200;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 31, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflaga.b.h);

    scrflaga.w = 0;
    actwk[0].xposi.w.h = 0;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 15, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);
}

static void test_scr71c_vertical_scroll_paths_and_clamps(test_context *ctx) {
    int_union pos;

    reset_scr71c_state();
    scralim_up = 0;
    scralim_down = 1296;
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

    reset_scr71c_state();
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
}

static void test_scr71c_parallax_scroll_flags_and_values(test_context *ctx) {
    int_union dx;
    int_union dy;

    reset_scr71c_state();
    scrb_h_count = 16;
    scrb_v_count = 16;
    dx.l = 16 << 16;
    dy.l = 16 << 16;

    scrollb_hv(dx, dy);

    TEST_ASSERT_EQ_INT(ctx, 16, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8 | 2, scrflagb.b.h);

    reset_scr71c_state();
    scrb_v_count = 16;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_count);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);

    reset_scr71c_state();
    scrb_h_count = 16;
    scrollb_h(16 << 16, 4);
    TEST_ASSERT_EQ_INT(ctx, 16, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);

    scrc_h_count = 16;
    scrollc_h(16 << 16, 16);
    TEST_ASSERT_EQ_INT(ctx, 16, scrc_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32, scrflagc.b.h);

    scrz_h_count = 16;
    scrollz_h(16 << 16, 64);
    TEST_ASSERT_EQ_INT(ctx, 16, scrz_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 128, scrflagz.b.h);
}

static void test_scr71c_hscroll_line_builders_and_zone_copy(test_context *ctx) {
    Sint32 count;

    reset_scr71c_state();
    scra_h_posit.w.h = 100;
    scrb_h_posit.w.h = 40;
    scrz_h_posit.w.h = 20;
    scrc_h_posit.w.h = 70;

    count = z71aline(0);
    TEST_ASSERT_EQ_INT(ctx, 46, count);
    TEST_ASSERT_EQ_INT(ctx, -98, hscrollwork[0]);

    count = z71aline0(count);
    TEST_ASSERT_EQ_INT(ctx, 80, count);
    TEST_ASSERT_EQ_INT(ctx, -41, hscrollwork[44]);

    hscrollwork[0] = -11;
    hscrollwork[1] = -22;
    hscrollwork[2] = -33;
    zonescrsetsub0(6, 0);

    TEST_ASSERT_EQ_INT(ctx, -11, hscroll_buffer[0].w.l);
    TEST_ASSERT_EQ_INT(ctx, -11, hscroll_buffer[1].w.l);
    TEST_ASSERT_EQ_INT(ctx, -22, hscroll_buffer[2].w.l);
    TEST_ASSERT_EQ_INT(ctx, -22, hscroll_buffer[9].w.l);

}

static void test_scr71c_block_write_and_mapinit(test_context *ctx) {
    POINT tile;

    reset_scr71c_state();
    blockwk[5][0] = 101;
    blockwk[5][1] = 102;
    blockwk[5][2] = 103;
    blockwk[5][3] = 104;
    tile.x = 2;
    tile.y = 3;

    blockwrt(24576, &tile, 5);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, setgrid_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 3, setgrid_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 101, setgrid_block[0]);

    reset_scr71c_state();
    mapinittbl.colorno2 = 7;

    mapinit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset_arg);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset2_arg);
}

static void test_scr71c_map_address_and_vram_helpers(test_context *ctx) {
    Sint32 index;
    Uint16 *block_ptr;
    POINT tile;

    reset_scr71c_state();
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

    vramadrset(16, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    vramadrset1(999, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    vramadrset0(48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.y);
}

static void test_scr71c_blockwrt_flip_orders(test_context *ctx) {
    POINT tile;

    reset_scr71c_state();
    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;
    tile.x = 0;
    tile.y = 0;

    blockwrt(16384, &tile, 5 | 2048);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 2048, setgrid_frip[0]);

    reset_scr71c_state();
    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;
    blockwrt(16384, &tile, 5 | 4096);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[3]);

    reset_scr71c_state();
    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;
    blockwrt(16384, &tile, 5 | 6144);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[3]);
}

static void test_scr71c_scroll_wrapper_and_pose_vertical_branches(
    test_context *ctx) {
    reset_scr71c_state();
    scroll_start.b.h = 1;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);

    reset_scr71c_state();
    scralim_right = 1000;
    scralim_down = 1296;
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

    reset_scr71c_state();
    scra_vline = 96;
    scralim_down = 1296;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 120;
    limmoveflag = 1;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_vz);

    reset_scr71c_state();
    scra_vline = 96;
    scralim_down = 1296;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 300;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    reset_scr71c_state();
    scra_vline = 80;
    scralim_down = 1296;
    scra_v_posit.w.h = 100;
    sv_move_main(3);
    TEST_ASSERT_EQ_INT(ctx, 102, scra_v_posit.w.h);
}

static void test_scr71c_scroll_write_helpers_touch_grid(test_context *ctx) {
    POINT tile;

    reset_scr71c_state();
    mapwka[0][0] = 1;
    pmapwk[0] = 5;
    blockwk[5][0] = 21;
    blockwk[5][1] = 22;
    blockwk[5][2] = 23;
    blockwk[5][3] = 24;
    tile.x = 0;
    tile.y = 0;

    hblockwrt(&tile, 16384, 0, 0, 0, 0, &mapwka[0][0], 0);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);

    reset_scr71c_state();
    mapwka[0][0] = 1;
    pmapwk[0] = 5;
    blockwk[5][0] = 31;
    blockwk[5][1] = 32;
    blockwk[5][2] = 33;
    blockwk[5][3] = 34;
    tile.x = 0;
    tile.y = 0;

    vblockwrt(&tile, 16384, 0, 0, 0, 0, &mapwka[0][0], 0);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 31, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 2, tile.y);
}

static void test_scr71c_additional_scroll_edges(test_context *ctx) {
    int_union delta;

    reset_scr71c_state();
    plflag = 1;
    scralim_right = 1000;
    scralim_down = 1000;
    actwk[0].xposi.w.h = 100;
    actwk[0].yposi.w.h = -5;

    playposiset();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);

    reset_scr71c_state();
    scra_h_posit.w.h = 100;
    scra_hline = 160;
    actwk[0].xposi.w.h = 260;
    scra_hz = 1234;

    scrh_move();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);

    reset_scr71c_state();
    scralim_right = 1000;
    scralim_left = 0;
    scra_h_posit.w.h = 200;
    right_check((Uint16)-5);

    TEST_ASSERT_EQ_INT(ctx, 195, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1280, scra_hz);

    scra_h_posit.w.h = 200;
    left_check(5);

    TEST_ASSERT_EQ_INT(ctx, 205, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1280, scra_hz);

    reset_scr71c_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 201;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr71c_state();
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 227;
    limmoveflag = 0;
    scra_vz = 777;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr71c_state();
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 180;
    limmoveflag = 1;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);

    reset_scr71c_state();
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 228;
    limmoveflag = 0;
    scra_vz = 777;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr71c_state();
    limmoveflag = 1;
    scra_v_posit.w.h = 80;

    sv_move_sub2();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 80, scra_v_posit.w.h);

    reset_scr71c_state();
    scralim_up = 0;
    scra_v_posit.w.h = 100;
    scra_v_count = 0;
    delta.l = 0;
    delta.w.l = -1;
    scrv_up_ch(delta);

    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflaga.b.h);

    reset_scr71c_state();
    scralim_down = 1296;
    scra_v_posit.w.h = 1200;
    delta.l = 0;
    delta.w.l = 1400;
    scrv_down_ch(delta);

    TEST_ASSERT_EQ_INT(ctx, 1296, scra_v_posit.w.h);
}

static void test_scr71c_additional_vertical_speed_selectors(test_context *ctx) {
    reset_scr71c_state();
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].mspeed.w = -2048;

    sv_move_main(20);

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    reset_scr71c_state();
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;

    sv_move_main(3);

    TEST_ASSERT_EQ_INT(ctx, 103, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 768, scra_vz);

    reset_scr71c_state();
    scralim_down = 1296;
    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main1((Uint16)-3);

    TEST_ASSERT_EQ_INT(ctx, 98, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, scra_vz);

    reset_scr71c_state();
    scralim_down = 1296;
    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main1(1);

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);

    reset_scr71c_state();
    scralim_down = 1296;
    scra_v_posit.w.h = 100;

    sv_move_main2(4);

    TEST_ASSERT_EQ_INT(ctx, 104, scra_v_posit.w.h);
}

static void test_scr71c_negative_parallax_flags(test_context *ctx) {
    int_union dx;
    int_union dy;

    reset_scr71c_state();
    scrb_h_posit.w.h = 16;
    scrb_h_count = 0;
    scrb_v_posit.w.h = 16;
    scrb_v_count = 0;
    dx.l = -(16 << 16);
    dy.l = -(16 << 16);

    scrollb_hv(dx, dy);

    TEST_ASSERT_EQ_INT(ctx, 0, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4 | 1, scrflagb.b.h);

    reset_scr71c_state();
    scrb_v_count = 0;
    dx.l = 0;
    dy.l = -(1 << 16);
    scrollb_hv(dx, dy);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 10, scrflagb.b.h);

    reset_scr71c_state();
    scrb_v_posit.w.h = 16;
    scrb_v_count = 0;
    scrollb_v(0);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    reset_scr71c_state();
    scrb_h_posit.w.h = 16;
    scrb_h_count = 0;
    scrollb_h(-(16 << 16), 4);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);

    reset_scr71c_state();
    scrc_h_posit.w.h = 16;
    scrc_h_count = 0;
    scrollc_h(-(16 << 16), 16);
    TEST_ASSERT_EQ_INT(ctx, 16, scrflagc.b.h);

    reset_scr71c_state();
    scrz_h_posit.w.h = 16;
    scrz_h_count = 0;
    scrollz_h(-(16 << 16), 64);
    TEST_ASSERT_EQ_INT(ctx, 64, scrflagz.b.h);
}

static void test_scr71c_map_block_edges_and_noops(test_context *ctx) {
    Sint32 index;
    Uint16 *block_ptr;
    POINT tile;

    reset_scr71c_state();
    enecginit();
    divdevset();
    scrollwrtc();
    scrollwrtz();
    mapset();

    mapwka[0][0] = 1;
    mapwka[7][63] = 1;
    pmapwk[0] = 41;
    pmapwk[255] = 99;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset99(0, 0, 65520, 65520, &mapwka[0][0],
                                   &index));
    TEST_ASSERT_EQ_INT(ctx, 41, index);

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset99(0, 0, 16384, 2048, &mapwka[0][0],
                                   &index));
    TEST_ASSERT_EQ_INT(ctx, 99, index);

    memset(mapwka, 0, sizeof(mapwka));
    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset2(0, 0, &mapwka[0][0], &index, &block_ptr));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 1;
    pmapwk[0] = 7;
    blockwk[9][0] = 61;
    blockwk[9][1] = 62;
    blockwk[9][2] = 63;
    blockwk[9][3] = 64;
    block_wrt(9, 0, 0);

    TEST_ASSERT_EQ_INT(ctx, 9, pmapwk[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);

    scra_v_posit.w.h = -16;
    scra_h_posit.w.h = -16;
    TEST_ASSERT_EQ_INT(ctx, 0, block_chk(400, 300));

    vramadrset2(16, 32, 48, 64, &tile);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);

    reset_scr71c_state();
    mapinittbl.colorno2 = 3;
    plflag = 1;
    play_start = 2;

    mapinit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
}

static void test_scr71c_map_writer_smoke_paths(test_context *ctx) {
    Uint8 flag;
    Uint8 wrt_tbl[1] = {2};

    reset_scr71c_state();
    blockwk[0][0] = 1;
    blockwk[0][1] = 2;
    blockwk[0][2] = 3;
    blockwk[0][3] = 4;

    mapwrt2(0, 0, &mapwka[0][0], 16384);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_block[0]);

    reset_scr71c_state();
    blockwk[0][0] = 5;
    blockwk[0][1] = 6;
    blockwk[0][2] = 7;
    blockwk[0][3] = 8;

    mapwrt_z81a(&mapwkb[0][0], 24576);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 5, setgrid_block[0]);

    reset_scr71c_state();
    scrollwrtadva();
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);

    reset_scr71c_state();
    scrollwrt();
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);

    reset_scr71c_state();
    flag = 0;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);

    reset_scr71c_state();
    blockwk[0][0] = 17;
    blockwk[0][1] = 18;
    blockwk[0][2] = 19;
    blockwk[0][3] = 20;
    mapwrt_sub(wrt_tbl, 0, 0, &mapwkb[0][0], 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);

    reset_scr71c_state();
    blockwk[0][0] = 21;
    blockwk[0][1] = 22;
    blockwk[0][2] = 23;
    blockwk[0][3] = 24;
    scrb_v_posit.w.h = 16;
    flag = 1;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 25;
    blockwk[0][1] = 26;
    blockwk[0][2] = 27;
    blockwk[0][3] = 28;
    flag = 2;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 29;
    blockwk[0][1] = 30;
    blockwk[0][2] = 31;
    blockwk[0][3] = 32;
    z81awrttbl[23] = 2;
    scrb_v_posit.w.h = 368;
    flag = 1;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 33;
    blockwk[0][1] = 34;
    blockwk[0][2] = 35;
    blockwk[0][3] = 36;
    z81awrttbl[23] = 2;
    scrb_v_posit.w.h = 23 * 16;
    flag = 4;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr71c_state();
    blockwk[0][0] = 37;
    blockwk[0][1] = 38;
    blockwk[0][2] = 39;
    blockwk[0][3] = 40;
    z81awrttbl[23] = 2;
    scrb_v_posit.w.h = 23 * 16;
    flag = 8;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr71c_state();
    blockwk[0][0] = 41;
    blockwk[0][1] = 42;
    blockwk[0][2] = 43;
    blockwk[0][3] = 44;
    z81awrttbl[113] = 2;
    scrb_v_posit.w.h = 2000;
    flag = 4;
    scrollwrtb(&flag, &mapwkb[0][0], 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, flag);
}

static void test_scr71c_scrollwrt_foreground_flag_paths(test_context *ctx) {
    reset_scr71c_state();
    blockwk[0][0] = 61;
    blockwk[0][1] = 62;
    blockwk[0][2] = 63;
    blockwk[0][3] = 64;
    scrflagaw.b.h = 1;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 65;
    blockwk[0][1] = 66;
    blockwk[0][2] = 67;
    blockwk[0][3] = 68;
    scrflagaw.b.h = 2;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 69;
    blockwk[0][1] = 70;
    blockwk[0][2] = 71;
    blockwk[0][3] = 72;
    scrflagaw.b.h = 4;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 73;
    blockwk[0][1] = 74;
    blockwk[0][2] = 75;
    blockwk[0][3] = 76;
    scrflagaw.b.h = 8;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);

    reset_scr71c_state();
    blockwk[0][0] = 77;
    blockwk[0][1] = 78;
    blockwk[0][2] = 79;
    blockwk[0][3] = 80;

    mapwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 16);
}

static void test_scr71c_block_write_visible_and_vertical_wrap(
    test_context *ctx) {
    POINT tile;

    reset_scr71c_state();
    mapwka[1][1] = 1;
    pmapwk[41] = 7;
    blockwk[9][0] = 41;
    blockwk[9][1] = 42;
    blockwk[9][2] = 43;
    blockwk[9][3] = 44;
    scra_v_posit.w.h = -16;
    scra_h_posit.w.h = -16;

    block_wrt(9, 400, 300);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);
    TEST_ASSERT_EQ_INT(ctx, 41, setgrid_block[0]);

    reset_scr71c_state();
    blockwk[0][0] = 51;
    blockwk[0][1] = 52;
    blockwk[0][2] = 53;
    blockwk[0][3] = 54;
    tile.x = 0;
    tile.y = 30;

    vblockwrt(&tile, 16384, 0, 0, 0, 0, &mapwka[0][0], 0);

    TEST_ASSERT_EQ_INT(ctx, 0, tile.y);
}

TEST_MAIN_BEGIN;
    test_scr71c_scr_set_and_play_position_defaults(&ctx);
    test_scr71c_playposiset_loads_and_clamps_saved_position(&ctx);
    test_scr71c_horizontal_scroll_clamps_and_sets_speed(&ctx);
    test_scr71c_scroll_h_flags_direction_changes(&ctx);
    test_scr71c_vertical_scroll_paths_and_clamps(&ctx);
    test_scr71c_parallax_scroll_flags_and_values(&ctx);
    test_scr71c_hscroll_line_builders_and_zone_copy(&ctx);
    test_scr71c_block_write_and_mapinit(&ctx);
    test_scr71c_map_address_and_vram_helpers(&ctx);
    test_scr71c_blockwrt_flip_orders(&ctx);
    test_scr71c_scroll_wrapper_and_pose_vertical_branches(&ctx);
    test_scr71c_scroll_write_helpers_touch_grid(&ctx);
    test_scr71c_additional_scroll_edges(&ctx);
    test_scr71c_additional_vertical_speed_selectors(&ctx);
    test_scr71c_negative_parallax_flags(&ctx);
    test_scr71c_map_block_edges_and_noops(&ctx);
    test_scr71c_map_writer_smoke_paths(&ctx);
    test_scr71c_scrollwrt_foreground_flag_paths(&ctx);
    test_scr71c_block_write_visible_and_vertical_wrap(&ctx);
TEST_MAIN_END;

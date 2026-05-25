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
static Sint32 setgrid_base[8];
static Sint32 setgrid_x[8];
static Sint32 setgrid_y[8];
static Sint32 setgrid_block[8];
static Sint32 setgrid_frip[8];

void playload(void);
void scrchk(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);

#include "src/r4/scr41a.c"

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

static void reset_scr41a_state(void) {
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

static void test_scr41a_scr_set_uses_default_start(test_context *ctx) {
    reset_scr41a_state();

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 7319, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_up);
    TEST_ASSERT_EQ_INT(ctx, 1296, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 576, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_v_count);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 80, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno2);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno2);
    TEST_ASSERT_EQ_INT(ctx, 0, playload_count);
}

static void test_scr41a_scr_set_saved_position_clamps(test_context *ctx) {
    reset_scr41a_state();
    plflag = 1;
    actwk[0].xposi.w.h = 9000;
    actwk[0].yposi.w.h = 3000;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 7319, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1296, scra_v_posit.w.h);
}

static void test_scr41a_mapadrset_empty_and_populated(test_context *ctx) {
    Sint32 index = 123;

    reset_scr41a_state();

    TEST_ASSERT_EQ_INT(ctx, 0, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 2;
    map_work[2 + 3 * 16 + 256] = 0x1234;

    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0x1234, index);
}

static void test_scr41a_vram_address_masks_to_tile_grid(test_context *ctx) {
    POINT tile = {0, 0};

    reset_scr41a_state();

    vramadrset(32, 48, 16, 32, &tile);
    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);

    vramadrset1(999, 48, 16, 32, &tile);
    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);
}

static void test_scr41a_blockwrt_emits_unflipped_and_flipped_quads(
    test_context *ctx) {
    POINT tile = {4, 6};

    reset_scr41a_state();
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

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 6144, setgrid_frip[0]);
}

#define SCR81_RESET reset_scr41a_state
#include "../r8/test_scr81_common.inc"
#undef SCR81_RESET

static void test_scr41a_scroll_respects_start_guard(test_context *ctx) {
    reset_scr41a_state();
    scroll_start.b.h = 1;
    scrflaga.w = 0x1234;
    actwk[0].xposi.w.h = 999;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0x1234, scrflaga.w);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
}

static void test_scr41a_scroll_moves_right_and_builds_hscroll(
    test_context *ctx) {
    reset_scr41a_state();
    scr_set();
    actwk[0].xposi.w.h = 300;
    actwk[0].yposi.w.h = 96;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 0, vscroll.w.h);
    TEST_ASSERT_TRUE(ctx, hscroll_buffer[0].w.h <= 0);
}

static void test_scr41a_scroll_h_left_and_center_paths(test_context *ctx) {
    reset_scr41a_state();
    scralim_left = 0;
    scralim_right = 7319;
    scra_hline = 160;
    scra_h_posit.w.h = 100;
    scra_h_count = 16;
    actwk[0].xposi.w.h = 120;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflaga.b.h);

    actwk[0].xposi.w.h = 244;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflaga.b.h);

    scra_h_posit.w.h = 100;
    scra_h_count = 0;
    actwk[0].xposi.w.h = 120;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);
}

static void test_scr41a_horizontal_helpers_cover_clamps(test_context *ctx) {
    reset_scr41a_state();
    scralim_left = 10;
    scralim_right = 100;
    scra_h_posit.w.h = 5;

    left_check(0);

    TEST_ASSERT_EQ_INT(ctx, 10, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1280, scra_hz);

    scra_h_posit.w.h = 20;

    right_check((Uint16)-1);

    TEST_ASSERT_EQ_INT(ctx, 19, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -256, scra_hz);

    scra_h_posit.w.h = 0;
    scralim_left = 0;
    scralim_right = 100;
    scra_h_count = 0;
    actwk[0].xposi.w.h = 300;
    scra_hline = 160;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflaga.b.h);
}

static void test_scr41a_vertical_edge_and_helper_paths(test_context *ctx) {
    int_union lSpd;

    reset_scr41a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 202;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    limmoveflag = 1;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = scra_v_posit.w.h + scra_vline;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    reset_scr41a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_v_posit.w.h = 100;
    scra_v_count = 16;

    sv_move_main2(4);

    TEST_ASSERT_EQ_INT(ctx, 104, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1024, scra_vz);

    memset(&lSpd, 0, sizeof(lSpd));
    lSpd.w.l = -1;
    scra_v_posit.w.h = 8;

    scrv_up_ch(lSpd);

    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2048, scra_vz);

    reset_scr41a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].mspeed.w = -2048;

    sv_move_main(20);

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    scra_vline = 80;
    scra_v_posit.w.h = 100;

    sv_move_main1((Uint16)-4);

    TEST_ASSERT_EQ_INT(ctx, 98, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, scra_vz);

    scra_v_posit.w.h = 100;

    sv_move_main1(1);

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    reset_scr41a_state();
    scralim_up = 0;
    scralim_down = 1296;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 200;
    limmoveflag = 0;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
}

static void test_scr41a_parallax_and_mapwrite_extra_paths(test_context *ctx) {
    int_union dx;
    int_union dy;

    reset_scr41a_state();
    memset(&dx, 0, sizeof(dx));
    memset(&dy, 0, sizeof(dy));
    dx.l = 0x00100000;
    dy.l = 0x00100000;
    scrb_h_count = 16;
    scrb_v_count = 16;

    scrollb_hv(dx, dy);

    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h & 8);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h & 2);

    reset_scr41a_state();
    scrb_h_count = 16;
    scrb_v_count = 16;
    scrb_v_posit.l = 0x00200000;
    dx.l = -0x00100000;
    dy.l = -0x00100000;
    scrollb_hv(dx, dy);

    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h & 4);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h & 1);

    reset_scr41a_state();
    scrb_v_posit.l = 0x00200000;
    scrb_v_count = 16;
    dx.l = 0;
    dy.l = -0x00400000;
    scrollb_hv(dx, dy);
    TEST_ASSERT_TRUE(ctx, scrb_v_posit.l >= 0);

    reset_scr41a_state();
    scrb_h_count = 16;
    scrc_h_count = 16;
    scrz_h_count = 16;
    scrollb_h(-0x00100000, 4);
    scrollc_h(-0x00100000, 16);
    scrollz_h(-0x00100000, 64);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h & 4);
    TEST_ASSERT_EQ_INT(ctx, 16, scrflagc.b.h & 16);
    TEST_ASSERT_EQ_INT(ctx, 64, scrflagz.b.h & 64);

    reset_scr41a_state();
    scra_v_posit.w.h = 0;
    scra_h_posit.w.h = 640;
    scrbinit(0);
    TEST_ASSERT_EQ_INT(ctx, 512, scrb_v_posit.w.h);

    reset_scr41a_state();
    seed_scr81_screen_zero((Uint8 *)mapwkb);
    scrflagb.b.h = 3;
    scrflagc.b.h = 4;
    scrollwrtadva();
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagb.b.h);

    reset_scr41a_state();
    seed_scr81_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 768;
    mapwrt_z81a((Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr41a_state();
    seed_scr81_screen_zero((Uint8 *)mapwkb);
    scra_h_posit.w.h = 640;
    mapwrt_z81a((Uint8 *)mapwkb, 24576);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 0);

    reset_scr41a_state();
    seed_scr81_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 2000;
    scrflagb.b.h = 128;
    scrollwrtb(&scrflagb.b.h, (Uint8 *)mapwkb, 24576);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagb.b.h);

    reset_scr41a_state();
    lphscrollbuff = hscroll_buffer;
    hscrollwork[0] = 11;
    hscrollwork[1] = 22;
    waterposi = 10000;
    zonescrsetsub0(3, 7, 0);
    TEST_ASSERT_EQ_INT(ctx, 11, hscroll_buffer[0].w.l);
    TEST_ASSERT_EQ_INT(ctx, 22, hscroll_buffer[1].w.l);

    reset_scr41a_state();
    scralim_left = 0;
    scralim_right = 7319;
    scralim_up = 0;
    scralim_down = 1296;
    scra_hline = 160;
    scra_vline = 96;
    scra_h_posit.w.h = 700;
    scra_v_posit.w.h = 600;
    actwk[0].xposi.w.h = 860;
    actwk[0].yposi.w.h = 696;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);

    reset_scr41a_state();
    scralim_left = 0;
    scralim_right = 7319;
    scralim_up = 0;
    scralim_down = 1296;
    scra_hline = 160;
    scra_vline = 96;
    scra_h_posit.w.h = 640;
    scra_v_posit.w.h = 100;
    actwk[0].xposi.w.h = 810;
    actwk[0].yposi.w.h = 196;
    scroll();
    TEST_ASSERT_EQ_INT(ctx, 512, scrb_v_posit.w.h);
}

static void test_scr41a_mapinit_sets_colors_and_calls_noop_hooks(
    test_context *ctx) {
    reset_scr41a_state();
    mapinittbl.colorno2 = 7;
    plflag = 1;
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
test_scr41a_scr_set_uses_default_start(&ctx);
test_scr41a_scr_set_saved_position_clamps(&ctx);
test_scr41a_mapadrset_empty_and_populated(&ctx);
test_scr81_mapadrset_clamps_high_offsets(&ctx);
test_scr81_mapadrset2_reports_block_pointer_and_empty_screen(&ctx);
test_scr41a_vram_address_masks_to_tile_grid(&ctx);
test_scr81_vram_address_extra_entry_points(&ctx);
test_scr41a_blockwrt_emits_unflipped_and_flipped_quads(&ctx);
test_scr81_blockwrt_emits_single_axis_flips(&ctx);
test_scr41a_scroll_respects_start_guard(&ctx);
test_scr41a_scroll_moves_right_and_builds_hscroll(&ctx);
test_scr41a_scroll_h_left_and_center_paths(&ctx);
test_scr41a_horizontal_helpers_cover_clamps(&ctx);
test_scr81_scroll_h_clamps_to_right_limit(&ctx);
test_scr81_scrollb_v_flags_up_and_down_edges(&ctx);
test_scr81_vertical_move_paths_and_limits(&ctx);
test_scr41a_vertical_edge_and_helper_paths(&ctx);
test_scr81_vertical_special_posture_paths(&ctx);
test_scr81_scroll_wraps_vertical_edges(&ctx);
test_scr41a_parallax_and_mapwrite_extra_paths(&ctx);
test_scr81_scrollwrt_processes_all_screen_flags(&ctx);
test_scr81_scrollwrtb_bit2_and_empty_paths(&ctx);
test_scr81_mapwrt_and_block_wrt_emit_grid_updates(&ctx);
test_scr81_block_wrt_visible_region_emits_grid_update(&ctx);
test_scr41a_mapinit_sets_colors_and_calls_noop_hooks(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

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

#include "src/r8/scr83d.c"

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
    (void)x;
    (void)y;
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

static void reset_scr83d_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(mapwkb, 0, sizeof(mapwkb));
    memset(map_work, 0, sizeof(map_work));
    memset(blockwk, 0, sizeof(blockwk));
    memset(hscrollwork, 0, sizeof(hscrollwork));
    memset(hscroll_buffer, 0, sizeof(hscroll_buffer));
    memset(&mapinittbl, 0, sizeof(mapinittbl));
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

static void seed_screen_zero(Uint8 *map);

static void test_scr83d_scr_set_uses_default_start_and_parallax(
    test_context *ctx) {
    reset_scr83d_state();

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 3735, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_up);
    TEST_ASSERT_EQ_INT(ctx, 784, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 1360, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_v_count);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 48, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 588, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 492, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 61, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 61, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 61, scrz_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno2);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno2);
    TEST_ASSERT_EQ_INT(ctx, 0, playload_count);
}

static void test_scr83d_scr_set_loads_saved_position_when_plflag_is_set(
    test_context *ctx) {
    reset_scr83d_state();
    plflag = 1;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = -12;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 500, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -12, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 340, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 282, scrb_h_posit.w.h);
}

static void test_scr83d_scr_set_saved_position_positive_and_clamped(
    test_context *ctx) {
    reset_scr83d_state();
    plflag = 1;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = 300;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 340, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 204, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 25, scrb_v_posit.w.h);

    reset_scr83d_state();
    plflag = 1;
    actwk[0].xposi.w.h = 9000;
    actwk[0].yposi.w.h = 3000;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 3735, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 784, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 98, scrb_v_posit.w.h);
}

static void test_scr83d_mapadrset_empty_and_populated_screens(
    test_context *ctx) {
    Sint32 index = 123;

    reset_scr83d_state();

    TEST_ASSERT_EQ_INT(ctx, 0, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 2;
    map_work[2 + 3 * 16 + 256] = 0x1234;
    index = 0;

    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset(0, 0, 32, 48, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0x1234, index);
}

static void test_scr83d_vram_address_masks_to_tile_grid(test_context *ctx) {
    POINT tile = {0, 0};

    reset_scr83d_state();

    vramadrset(32, 48, 16, 32, &tile);

    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);

    vramadrset1(999, 48, 16, 32, &tile);

    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);
}

static void test_scr83d_blockwrt_emits_unflipped_and_flipped_quads(
    test_context *ctx) {
    POINT tile = {4, 6};

    reset_scr83d_state();
    blockwk[5][0] = 10;
    blockwk[5][1] = 11;
    blockwk[5][2] = 12;
    blockwk[5][3] = 13;

    blockwrt(16384, &tile, 5);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 6, setgrid_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_frip[0]);

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

static void test_scr83d_blockwrt_emits_single_axis_flips(test_context *ctx) {
    POINT tile = {8, 10};

    reset_scr83d_state();
    blockwk[7][0] = 20;
    blockwk[7][1] = 21;
    blockwk[7][2] = 22;
    blockwk[7][3] = 23;

    blockwrt(16384, &tile, 7 | 4096);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 4096, setgrid_frip[0]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 7 | 2048);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 21, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 20, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 23, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 22, setgrid_block[3]);
    TEST_ASSERT_EQ_INT(ctx, 2048, setgrid_frip[0]);
}

static void test_scr83d_mapwrt_sub_zero_row_uses_full_width_write(
    test_context *ctx) {
    static Uint8 zero_row[1] = {0};
    POINT tile = {0, 0};
    Sint32 index = -1;

    reset_scr83d_state();
    seed_screen_zero((Uint8 *)mapwkb);

    mapwrt_sub(zero_row, 0, 0, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    setgrid_count = 0;
    hblockwrt1(&tile, 24576, 0, 0, 0, 0, (Uint8 *)mapwkb, 0);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset1(1234, 0, 0, 0, (Uint8 *)mapwkb,
                                          &index));
    TEST_ASSERT_EQ_INT(ctx, 0, index);
}

static void test_scr83d_scroll_respects_start_guard(test_context *ctx) {
    reset_scr83d_state();
    scroll_start.b.h = 1;
    scrflaga.w = 0x1234;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0x1234, scrflaga.w);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
}

static void test_scr83d_scroll_moves_right_and_builds_hscroll(
    test_context *ctx) {
    reset_scr83d_state();
    scr_set();
    actwk[0].xposi.w.h = 300;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 61, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 61, vscroll.w.l);
    TEST_ASSERT_EQ_INT(ctx, -16, hscroll_buffer[0].w.h);
}

static void test_scr83d_scroll_moves_left_and_flags_horizontal_edge(
    test_context *ctx) {
    reset_scr83d_state();
    scralim_left = 0;
    scralim_right = 7831;
    scra_hline = 160;
    scra_h_posit.w.h = 100;
    scra_h_count = 16;
    actwk[0].xposi.w.h = 120;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);
}

static void test_scr83d_scroll_h_clamps_to_right_limit(test_context *ctx) {
    reset_scr83d_state();
    scralim_left = 0;
    scralim_right = 120;
    scra_hline = 160;
    scra_h_posit.w.h = 110;
    actwk[0].xposi.w.h = 400;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 120, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2560, scra_hz);
}

static void test_scr83d_vertical_move_paths_and_limits(test_context *ctx) {
    reset_scr83d_state();
    scralim_up = 0;
    scralim_down = 1808;
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

    scra_v_posit.w.h = 1806;
    scra_vline = 96;
    actwk[0].yposi.w.h = 2048;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 1808, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, scra_vz);
}

static void test_scr83d_vertical_special_posture_paths(test_context *ctx) {
    reset_scr83d_state();
    scralim_up = 0;
    scralim_down = 1808;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 2;
    actwk[0].yposi.w.h = 130;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_vz);

    actwk[0].yposi.w.h = 280;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    limmoveflag = 1;
    actwk[0].yposi.w.h = 177;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
}

static void test_scr83d_scroll_wraps_vertical_edges(test_context *ctx) {
    int_union lSpd;

    reset_scr83d_state();
    scralim_up = 0;
    scralim_down = 2047;
    memset(&lSpd, 0, sizeof(lSpd));
    lSpd.w.l = -300;
    scra_v_posit.w.h = 3;
    scrb_v_posit.w.h = 1000;
    actwk[0].yposi.w.h = 3;

    scrv_up_ch(lSpd);

    TEST_ASSERT_EQ_INT(ctx, 1748, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 3, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1000, scrb_v_posit.w.h);

    memset(&lSpd, 0, sizeof(lSpd));
    lSpd.w.l = 2050;
    scra_v_posit.w.h = 2040;
    scrb_v_posit.w.h = 1000;
    actwk[0].yposi.w.h = 2040;

    scrv_down_ch(lSpd);

    TEST_ASSERT_EQ_INT(ctx, 2, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2040, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1000, scrb_v_posit.w.h);
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

static void test_scr83d_scrollwrt_processes_all_screen_flags(
    test_context *ctx) {
    reset_scr83d_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 16;
    scrflagaw.b.h = 15;
    scrflagbw.b.h = 175;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagbw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
}

static void test_scr83d_mapwrt_and_block_wrt_emit_grid_updates(
    test_context *ctx) {
    reset_scr83d_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);

    mapwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    setgrid_count = 0;
    scra_h_posit.w.h = 500;
    scra_v_posit.w.h = 500;
    block_wrt(9, 0, 0);

    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 0, map_work[0]);
}

static void test_scr83d_direct_positive_parallax_flags(test_context *ctx) {
    reset_scr83d_state();
    scrb_h_count = 16;
    scrz_h_count = 16;

    scrollb_h(16 << 16, 2);
    scrollz_h(16 << 16, 6);

    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);
    TEST_ASSERT_EQ_INT(ctx, 128, scrflagz.b.h);
}

static void test_scr83d_block_wrt_visible_region_draws_without_map_store(
    test_context *ctx) {
    reset_scr83d_state();
    mapwka[0][0] = 1;
    blockwk[12][0] = 90;
    blockwk[12][1] = 91;
    blockwk[12][2] = 92;
    blockwk[12][3] = 93;
    scra_h_posit.w.h = 16;
    scra_v_posit.w.h = 16;

    TEST_ASSERT_EQ_INT(ctx, 0, block_chk(64, 0));

    block_wrt(12, 64, 0);

    TEST_ASSERT_EQ_INT(ctx, 0, map_work[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 8, setgrid_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 90, setgrid_block[0]);
}

#define SCR82_RESET reset_scr83d_state
#include "test_scr82_common.inc"
#undef SCR82_RESET

static void test_scr83d_mapinit_sets_primary_and_secondary_colors(
    test_context *ctx) {
    reset_scr83d_state();
    mapinittbl.colorno2 = 7;

    mapinit();

    TEST_ASSERT_EQ_INT(ctx, 1, colorset2_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset2_arg);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 7, colorset_arg);
}

TEST_MAIN_BEGIN;
test_scr83d_scr_set_uses_default_start_and_parallax(&ctx);
test_scr83d_scr_set_loads_saved_position_when_plflag_is_set(&ctx);
test_scr83d_scr_set_saved_position_positive_and_clamped(&ctx);
test_scr83d_mapadrset_empty_and_populated_screens(&ctx);
test_scr82_mapadrset_clamps_high_offsets(&ctx);
test_scr82_mapadrset2_reports_block_pointer_and_empty_screen(&ctx);
test_scr83d_vram_address_masks_to_tile_grid(&ctx);
test_scr82_vram_address_extra_entry_points(&ctx);
test_scr83d_blockwrt_emits_unflipped_and_flipped_quads(&ctx);
test_scr83d_blockwrt_emits_single_axis_flips(&ctx);
test_scr83d_mapwrt_sub_zero_row_uses_full_width_write(&ctx);
test_scr83d_scroll_respects_start_guard(&ctx);
test_scr83d_scroll_moves_right_and_builds_hscroll(&ctx);
test_scr83d_direct_positive_parallax_flags(&ctx);
test_scr83d_scroll_moves_left_and_flags_horizontal_edge(&ctx);
test_scr82_scroll_h_exact_center_and_left_limit(&ctx);
test_scr83d_scroll_h_clamps_to_right_limit(&ctx);
test_scr82_scroll_negative_parallax_flags(&ctx);
test_scr82_scrollb_v_flags_up_and_down_edges(&ctx);
test_scr83d_vertical_move_paths_and_limits(&ctx);
test_scr82_vertical_edge_branches(&ctx);
test_scr82_direct_vertical_helpers_cover_small_moves(&ctx);
test_scr83d_vertical_special_posture_paths(&ctx);
test_scr83d_scroll_wraps_vertical_edges(&ctx);
test_scr83d_scrollwrt_processes_all_screen_flags(&ctx);
test_scr82_scrollwrtb_bit2_and_empty_paths(&ctx);
test_scr83d_mapwrt_and_block_wrt_emit_grid_updates(&ctx);
test_scr83d_block_wrt_visible_region_draws_without_map_store(&ctx);
test_scr83d_mapinit_sets_primary_and_secondary_colors(&ctx);
test_scr82_noop_entry_points_are_callable(&ctx);
TEST_MAIN_END

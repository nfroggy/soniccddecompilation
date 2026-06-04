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
Sint32 linework[128];
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
void (*sMemSet)(void *, Uint8, Sint32);
void (*sOutputDebugString)(char *);

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
static void debug_string_sink(char *text);

#include "src/r1/scr11a.c"

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

static void debug_string_sink(char *text) { (void)text; }

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

static void test_memset(void *dst, Uint8 value, Sint32 size) {
    memset(dst, value, (size_t)size);
}

static void reset_scr11a_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(mapwka, 0, sizeof(mapwka));
    memset(mapwkb, 0, sizeof(mapwkb));
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
    memset(linework, 0, sizeof(linework));
    lphscrollbuff = hscroll_buffer;
    pmapwk = map_work;
    SetGrid = test_setgrid;
    sMemSet = test_memset;
    sOutputDebugString = debug_string_sink;
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
    map[0] = 1;
    for (int i = 0; i < 16 * 16; ++i) {
        map_work[i] = (Uint16)(i & 1023);
        blockwk[i & 1023][0] = (Uint16)(100 + i);
        blockwk[i & 1023][1] = (Uint16)(200 + i);
        blockwk[i & 1023][2] = (Uint16)(300 + i);
        blockwk[i & 1023][3] = (Uint16)(400 + i);
    }
}

static void test_scr11a_main_chk_selects_active_player(test_context *ctx) {
    reset_scr11a_state();

    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[0]);

    main_play = 1;

    TEST_ASSERT_TRUE(ctx, main_chk() == &actwk[1]);
}

static void test_scr11a_scr_set_uses_default_start_and_parallax(
    test_context *ctx) {
    reset_scr11a_state();

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 4, scrar_no);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_left);
    TEST_ASSERT_EQ_INT(ctx, 10391, scralim_right);
    TEST_ASSERT_EQ_INT(ctx, 0, scralim_up);
    TEST_ASSERT_EQ_INT(ctx, 1808, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 576, scra_h_keep);
    TEST_ASSERT_EQ_INT(ctx, 96, scra_vline);
    TEST_ASSERT_EQ_INT(ctx, 160, scra_hline);
    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1899, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scrc_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scrz_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, loopmapno2);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno);
    TEST_ASSERT_EQ_INT(ctx, 127, ballmapno2);
    TEST_ASSERT_EQ_INT(ctx, 0, playload_count);
}

static void test_scr11a_scr_set_loads_saved_position_when_plflag_is_set(
    test_context *ctx) {
    reset_scr11a_state();
    plflag = 1;
    actwk[0].xposi.w.h = 500;
    actwk[0].yposi.w.h = -12;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 1, playload_count);
    TEST_ASSERT_EQ_INT(ctx, 500, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, -12, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 340, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 170, scrb_h_posit.w.h);
}

static void test_scr11a_scr_set_saved_position_clamps_to_limits(
    test_context *ctx) {
    reset_scr11a_state();
    plflag = 1;
    actwk[0].xposi.w.h = 12000;
    actwk[0].yposi.w.h = 3000;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 10391, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1808, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5195, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1144, scrb_v_posit.w.h);
}

static void test_scr11a_scr_set_ignores_end_demo_flags(test_context *ctx) {
    reset_scr11a_state();
    demoflag.w = -32768;
    enddemono = 2;

    scr_set();

    TEST_ASSERT_EQ_INT(ctx, 64, actwk[0].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1899, actwk[0].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrb_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, scrb_v_posit.w.h);
}

static void test_scr11a_mapadrset_empty_and_populated_screens(
    test_context *ctx) {
    Sint32 index = 123;

    reset_scr11a_state();

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

static void test_scr11a_mapadrset_clamps_high_offsets(test_context *ctx) {
    Sint32 index = 0;

    reset_scr11a_state();
    mapwka[7][63] = 3;
    map_work[(3 - 1) * 256 + 15 + 15 * 16] = 0x2222;

    TEST_ASSERT_EQ_INT(ctx, 1, mapadrset(0, 0, 16384, 2048, (Uint8 *)mapwka,
                                         &index));
    TEST_ASSERT_EQ_INT(ctx, 0x2222, index);
}

static void test_scr11a_mapadrset2_reports_block_pointer_and_empty_screen(
    test_context *ctx) {
    Sint32 index = 123;
    Uint16 *block = (Uint16 *)1;

    reset_scr11a_state();

    TEST_ASSERT_EQ_INT(ctx, 0,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0, index);

    mapwka[0][0] = 2;
    map_work[256 + 2 + 3 * 16] = 0x3456;

    TEST_ASSERT_EQ_INT(ctx, 1,
                       mapadrset2(32, 48, (Uint8 *)mapwka, &index, &block));
    TEST_ASSERT_EQ_INT(ctx, 0x3456, index);
    TEST_ASSERT_TRUE(ctx, block == &map_work[256 + 2 + 3 * 16]);
}

static void test_scr11a_vram_address_masks_to_tile_grid(test_context *ctx) {
    POINT tile = {0, 0};

    reset_scr11a_state();

    vramadrset(32, 48, 16, 32, &tile);

    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);

    vramadrset1(999, 48, 16, 32, &tile);

    TEST_ASSERT_EQ_INT(ctx, 2, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 10, tile.y);

    vramadrset0(48, 64, &tile);

    TEST_ASSERT_EQ_INT(ctx, 6, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 8, tile.y);

    vramadrset2(16, 32, 48, 64, &tile);

    TEST_ASSERT_EQ_INT(ctx, 8, tile.x);
    TEST_ASSERT_EQ_INT(ctx, 12, tile.y);
}

static void test_scr11a_blockwrt_emits_all_flip_variants(test_context *ctx) {
    POINT tile = {4, 6};

    reset_scr11a_state();
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
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_frip[0]);

    setgrid_count = 0;
    blockwrt(24576, &tile, 5 | 6144);

    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 1, setgrid_base[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[3]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 5 | 4096);

    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[3]);

    setgrid_count = 0;
    blockwrt(16384, &tile, 5 | 2048);

    TEST_ASSERT_EQ_INT(ctx, 11, setgrid_block[0]);
    TEST_ASSERT_EQ_INT(ctx, 10, setgrid_block[1]);
    TEST_ASSERT_EQ_INT(ctx, 13, setgrid_block[2]);
    TEST_ASSERT_EQ_INT(ctx, 12, setgrid_block[3]);
}

static void test_scr11a_scroll_respects_start_guard(test_context *ctx) {
    reset_scr11a_state();
    scroll_start.b.h = 1;
    scrflaga.w = 0x1234;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 0, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0x1234, scrflaga.w);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
}

static void test_scr11a_scroll_moves_right_and_builds_hscroll(
    test_context *ctx) {
    reset_scr11a_state();
    scr_set();
    actwk[0].xposi.w.h = 300;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 1803, scrb_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1803, vscroll.w.l);

    reset_scr11a_state();
    scralim_left = 0;
    scralim_right = 10391;
    scralim_up = 0;
    scralim_down = 1808;
    scra_hline = 160;
    scra_vline = 96;
    scra_h_posit.w.h = 2048;
    scra_v_posit.w.h = 600;
    actwk[0].xposi.w.h = 2200;
    actwk[0].yposi.w.h = 696;
    actwk[0].xspeed.w = 3;
    player_work_get(&actwk[0])->status_flags = 2;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 2040, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2048, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 540, scrb_v_posit.w.h);
}

static void test_scr11a_scroll_top_band_clamps_hscroll_fill(
    test_context *ctx) {
    reset_scr11a_state();
    scralim_left = 0;
    scralim_right = 3479;
    scralim_up = 0;
    scralim_down = 784;
    scra_hline = 160;
    scra_vline = 96;
    scrb_v_posit.w.h = 0;
    actwk[0].xposi.w.h = 160;
    actwk[0].yposi.w.h = 96;

    scroll();

    TEST_ASSERT_EQ_INT(ctx, 1, scrchk_count);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);
}

static void test_scr11a_parallax_helpers_flag_all_directions(
    test_context *ctx) {
    int_union pos;
    int_union neg;

    reset_scr11a_state();
    scrb_h_count = 16;
    scrb_v_count = 16;
    memset(&pos, 0, sizeof(pos));
    memset(&neg, 0, sizeof(neg));
    pos.l = 16 << 16;
    neg.l = -(16 << 16);

    scrollb_hv(pos, pos);

    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);

    reset_scr11a_state();
    scrb_h_count = 16;
    scrb_v_count = 16;

    scrollb_hv(neg, neg);

    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);

    reset_scr11a_state();
    scrc_h_count = 16;
    scrz_h_count = 16;

    scrollc_h(16 << 16, 4);
    scrollz_h(16 << 16, 6);

    TEST_ASSERT_EQ_INT(ctx, 8, scrflagc.b.h);
    TEST_ASSERT_EQ_INT(ctx, 12, scrflagz.b.h);

    reset_scr11a_state();
    scrc_h_count = 16;
    scrz_h_count = 16;

    scrollc_h(-(16 << 16), 4);
    scrollz_h(-(16 << 16), 6);

    TEST_ASSERT_EQ_INT(ctx, 4, scrflagc.b.h);
    TEST_ASSERT_EQ_INT(ctx, 6, scrflagz.b.h);

    reset_scr11a_state();
    scrb_v_count = 16;
    scrb_v_posit.w.h = 32;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflagb.b.h);

    reset_scr11a_state();
    scrb_v_count = 16;
    scrb_v_posit.w.h = 0;
    scrollb_v(16);
    TEST_ASSERT_EQ_INT(ctx, 2, scrflagb.b.h);

    reset_scr11a_state();
    scrb_h_count = 16;
    scrollb_h(-(16 << 16), 4);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflagb.b.h);

    reset_scr11a_state();
    scrb_h_count = 16;
    scrollb_h(16 << 16, 4);
    TEST_ASSERT_EQ_INT(ctx, 8, scrflagb.b.h);

    reset_scr11a_state();
    scrb_h_count = 16;
    scrb_v_count = 0;
    scrollb_hv(pos, pos);
    TEST_ASSERT_EQ_INT(ctx, 10, scrflagb.b.h);

    reset_scr11a_state();
    scrb_h_count = 16;
    scrb_v_count = 0;
    scrollb_hv(neg, neg);
    TEST_ASSERT_EQ_INT(ctx, 5, scrflagb.b.h);
}

static void test_scr11a_scroll_moves_left_and_flags_horizontal_edge(
    test_context *ctx) {
    reset_scr11a_state();
    scralim_left = 0;
    scralim_right = 3479;
    scra_hline = 160;
    scra_h_posit.w.h = 100;
    scra_h_count = 0;
    actwk[0].xposi.w.h = 120;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 84, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -4096, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 16, scra_h_count);
    TEST_ASSERT_EQ_INT(ctx, 4, scrflaga.b.h);
}

static void test_scr11a_scroll_h_center_and_limits(test_context *ctx) {
    reset_scr11a_state();
    scralim_left = 0;
    scralim_right = 120;
    scra_hline = 160;
    scra_h_posit.w.h = 110;
    actwk[0].xposi.w.h = 400;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 120, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2560, scra_hz);

    scra_h_posit.w.h = 100;
    scra_h_count = 16;
    actwk[0].xposi.w.h = 260;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 100, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_hz);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_count);

    scra_h_posit.w.h = 8;
    actwk[0].xposi.w.h = -100;

    scroll_h();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2048, scra_hz);

    scra_h_posit.w.h = 100;
    scralim_right = 80;
    right_check(10);
    TEST_ASSERT_EQ_INT(ctx, 80, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -5120, scra_hz);

    scra_h_posit.w.h = 0;
    scralim_left = 20;
    left_check(65520);
    TEST_ASSERT_EQ_INT(ctx, 20, scra_h_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 5120, scra_hz);
}

static void test_scr11a_vertical_move_paths_and_limits(test_context *ctx) {
    reset_scr11a_state();
    scralim_up = 0;
    scralim_down = 784;
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

    scra_v_posit.w.h = 782;
    scra_vline = 96;
    actwk[0].yposi.w.h = 2048;
    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 784, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 512, scra_vz);
}

static void test_scr11a_vertical_special_posture_paths(test_context *ctx) {
    reset_scr11a_state();
    scralim_up = 0;
    scralim_down = 784;
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

static void test_scr11a_vertical_edge_branches(test_context *ctx) {
    reset_scr11a_state();
    scralim_up = 0;
    scralim_down = 784;
    scra_vline = 96;
    scra_v_posit.w.h = 100;
    actwk[0].cddat = 4;
    actwk[0].yposi.w.h = 202;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    actwk[0].cddat = 2;
    limmoveflag = 0;
    actwk[0].yposi.w.h = 228;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    limmoveflag = 1;
    actwk[0].cddat = 0;
    actwk[0].yposi.w.h = scra_v_posit.w.h + scra_vline;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 1, limmoveflag);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_vz);

    scra_v_posit.w.h = 100;
    scra_vline = 96;
    actwk[0].mspeed.w = -2048;
    actwk[0].yposi.w.h = 213;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 116, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 4096, scra_vz);

    scra_v_posit.w.h = 100;
    scra_vline = 80;
    actwk[0].mspeed.w = 0;
    actwk[0].yposi.w.h = 177;

    scroll_v();

    TEST_ASSERT_EQ_INT(ctx, 98, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -512, scra_vz);
}

static void test_scr11a_direct_vertical_helpers_cover_small_moves(
    test_context *ctx) {
    int_union lSpd;

    reset_scr11a_state();
    scralim_up = 0;
    scralim_down = 784;
    scra_v_posit.w.h = 100;
    scra_v_count = 16;

    sv_move_main2(4);

    TEST_ASSERT_EQ_INT(ctx, 104, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 1024, scra_vz);

    scra_vline = 80;
    scra_v_posit.w.h = 100;
    scra_v_count = 16;

    sv_move_main(1);

    TEST_ASSERT_EQ_INT(ctx, 101, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, 256, scra_vz);

    scra_v_posit.w.h = 100;
    scra_v_count = 0;

    sv_move_sub(-4);

    TEST_ASSERT_EQ_INT(ctx, 96, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -1024, scra_vz);
    TEST_ASSERT_EQ_INT(ctx, 1, scrflaga.b.h);

    memset(&lSpd, 0, sizeof(lSpd));
    lSpd.w.l = -1;
    scra_v_posit.w.h = 8;

    scrv_up_ch(lSpd);

    TEST_ASSERT_EQ_INT(ctx, 0, scra_v_posit.w.h);
    TEST_ASSERT_EQ_INT(ctx, -2049, scra_vz);
}

static void test_scr11a_scroll_wraps_vertical_edges(test_context *ctx) {
    int_union lSpd;

    reset_scr11a_state();
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

static void test_scr11a_scrollwrt_processes_screen_flags(test_context *ctx) {
    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 16;
    scrflagaw.b.h = 15;
    scrflagbw.b.h = 175;

    scrollwrt();

    TEST_ASSERT_EQ_INT(ctx, 0, scrflagaw.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, scrflagbw.b.h);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);
}

static void test_scr11a_scrollwrtb_bit_paths_and_empty(test_context *ctx) {
    Uint8 flag;

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 240;
    flag = 1;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 16;
    flag = 2;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr11a_state();
    flag = 0;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 1168;
    flag = 1;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 1152;
    flag = 16;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwkb);
    scrb_v_posit.w.h = 2040;
    flag = 16;

    scrollwrtb(&flag, (Uint8 *)mapwkb, 24576);

    TEST_ASSERT_EQ_INT(ctx, 0, flag);
    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);
}

static void test_scr11a_mapwrt_and_block_wrt_emit_grid_updates(
    test_context *ctx) {
    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);
    scrflagb.b.h = 3;

    scrollwrtadva();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwka);
    seed_screen_zero((Uint8 *)mapwkb);

    mapwrt();

    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);

    setgrid_count = 0;
    scra_h_posit.w.h = 500;
    scra_v_posit.w.h = 500;
    block_wrt(9, 0, 0);

    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 9, map_work[0]);

    reset_scr11a_state();
    seed_screen_zero((Uint8 *)mapwka);
    setgrid_count = 0;

    mapwrt_sub(z81awrttbl, 72 * 16, 0, (Uint8 *)mapwka, 16384);

    TEST_ASSERT_TRUE(ctx, setgrid_count > 4);
}

static void test_scr11a_block_wrt_visible_region_emits_grid_update(
    test_context *ctx) {
    reset_scr11a_state();
    mapwka[0][0] = 1;
    blockwk[12][0] = 90;
    blockwk[12][1] = 91;
    blockwk[12][2] = 92;
    blockwk[12][3] = 93;
    scra_h_posit.w.h = -16;
    scra_v_posit.w.h = -256;

    TEST_ASSERT_EQ_INT(ctx, 0, block_chk(64, 0));

    block_wrt(12, 64, 0);

    TEST_ASSERT_EQ_INT(ctx, 12, map_work[4]);
    TEST_ASSERT_EQ_INT(ctx, 4, setgrid_count);
    TEST_ASSERT_EQ_INT(ctx, 8, setgrid_x[0]);
    TEST_ASSERT_EQ_INT(ctx, 0, setgrid_y[0]);
    TEST_ASSERT_EQ_INT(ctx, 90, setgrid_block[0]);
}

static void test_scr11a_mapinit_and_noop_entry_points_are_callable(
    test_context *ctx) {
    reset_scr11a_state();
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
    TEST_ASSERT_EQ_INT(ctx, 5, colorset2_arg);
    TEST_ASSERT_EQ_INT(ctx, 1, colorset_count);
    TEST_ASSERT_EQ_INT(ctx, 5, colorset_arg);
}

TEST_MAIN_BEGIN;
    test_scr11a_main_chk_selects_active_player(&ctx);
    test_scr11a_scr_set_uses_default_start_and_parallax(&ctx);
    test_scr11a_scr_set_loads_saved_position_when_plflag_is_set(&ctx);
    test_scr11a_scr_set_saved_position_clamps_to_limits(&ctx);
    test_scr11a_scr_set_ignores_end_demo_flags(&ctx);
    test_scr11a_mapadrset_empty_and_populated_screens(&ctx);
    test_scr11a_mapadrset_clamps_high_offsets(&ctx);
    test_scr11a_mapadrset2_reports_block_pointer_and_empty_screen(&ctx);
    test_scr11a_vram_address_masks_to_tile_grid(&ctx);
    test_scr11a_blockwrt_emits_all_flip_variants(&ctx);
    test_scr11a_scroll_respects_start_guard(&ctx);
    test_scr11a_scroll_moves_right_and_builds_hscroll(&ctx);
    test_scr11a_scroll_top_band_clamps_hscroll_fill(&ctx);
    test_scr11a_parallax_helpers_flag_all_directions(&ctx);
    test_scr11a_scroll_moves_left_and_flags_horizontal_edge(&ctx);
    test_scr11a_scroll_h_center_and_limits(&ctx);
    test_scr11a_vertical_move_paths_and_limits(&ctx);
    test_scr11a_vertical_special_posture_paths(&ctx);
    test_scr11a_vertical_edge_branches(&ctx);
    test_scr11a_direct_vertical_helpers_cover_small_moves(&ctx);
    test_scr11a_scroll_wraps_vertical_edges(&ctx);
    test_scr11a_scrollwrt_processes_screen_flags(&ctx);
    test_scr11a_scrollwrtb_bit_paths_and_empty(&ctx);
    test_scr11a_mapwrt_and_block_wrt_emit_grid_updates(&ctx);
    test_scr11a_block_wrt_visible_region_emits_grid_update(&ctx);
    test_scr11a_mapinit_and_noop_entry_points_are_callable(&ctx);
TEST_MAIN_END

#include <string.h>

#include "support/test_runner.h"
#include "types.h"

static int action_calls;
static int actionsub_calls;
static int frameout_calls;
static int patset_calls;
static int wave_requests[8];
static int wave_request_count;
static int clr_sprite_debug_calls;
static int easprset_calls;
static int set_grid_calls;
static Sint32 random_value;

static void stub_action(void) { ++action_calls; }

static void stub_actionsub(sprite_status *pActwk) {
    (void)pActwk;
    ++actionsub_calls;
}

static void stub_frameout(sprite_status *pActwk) {
    ++frameout_calls;
    pActwk->actno = 0;
}

static void stub_patset(void) { ++patset_calls; }

static void stub_speedset(sprite_status *pActwk) { (void)pActwk; }

static void stub_speedset2(sprite_status *pActwk) { (void)pActwk; }

static void stub_spatset(Sint16 xposi, Sint16 yposi, sprite_status *pActwk,
                         sprite_data *sprdat, Sint16 cnt) {
    (void)xposi;
    (void)yposi;
    (void)pActwk;
    (void)sprdat;
    (void)cnt;
}

static Sint32 stub_scronchk(sprite_status *pActwk) {
    (void)pActwk;
    return 0;
}

static Sint32 stub_scronchk2(sprite_status *pActwk) {
    (void)pActwk;
    return 0;
}

static void stub_wave_request(Sint16 req) {
    if (wave_request_count < (int)(sizeof(wave_requests) / sizeof(wave_requests[0]))) {
        wave_requests[wave_request_count] = req;
    }
    ++wave_request_count;
}

static void stub_cd_play(Sint16 req) { (void)req; }

static void stub_change_tile_bmp(Sint32 a, Sint32 b) {
    (void)a;
    (void)b;
}

static void stub_clr_sprite_debug(void) { ++clr_sprite_debug_calls; }

static void stub_easprset(Sint16 a, Sint16 b, Uint16 c, Uint16 d, Uint16 e) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    ++easprset_calls;
}

static Sint32 stub_set_grid(Sint32 a, Sint32 b, Sint32 c, Sint32 d) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    ++set_grid_calls;
    return 0;
}

static void stub_close_file(Sint32 file) { (void)file; }

static Sint32 stub_get_file_size(Sint32 file) {
    (void)file;
    return 0;
}

static Sint32 stub_read_file(Sint32 file, void *dst, Sint32 len) {
    (void)file;
    (void)dst;
    return len;
}

static Sint32 stub_open_file(char *name) {
    (void)name;
    return 0;
}

static void stub_memcpy(void *dst, void *src, Sint32 len) { memcpy(dst, src, len); }

static void stub_memset(void *dst, Uint8 value, Sint32 len) { memset(dst, value, len); }

static Sint32 stub_random(void) { return random_value; }

static void stub_printf(char *dst, const char *fmt, ...) {
    (void)dst;
    (void)fmt;
}

static void stub_output_debug_string(char *text) { (void)text; }

#define action stub_action
#define actionsub stub_actionsub
#define frameout stub_frameout
#define patset stub_patset
#define speedset stub_speedset
#define speedset2 stub_speedset2
#define spatset stub_spatset
#define scronchk stub_scronchk
#define scronchk2 stub_scronchk2

#include "src/warp/warp.c"

#undef action
#undef actionsub
#undef frameout
#undef patset
#undef speedset
#undef speedset2
#undef spatset
#undef scronchk
#undef scronchk2

void SWdataSet(ushort_union sw1, ushort_union sw2) {
    swdata1 = sw1;
    swdata2 = sw2;
}

bmp_info SprBmp[700];

static PALETTEENTRY colorwk[64];
static PALETTEENTRY colorwk2[64];
static PALETTEENTRY colorwk3[64];
static PALETTEENTRY colorwk4[64];
static Uint16 mapwk[16];
static int_union hscroll_buffer[16];
static Sint32 fade_flag_storage;
static game_info keep_work_storage;

static void reset_state(void) {
    memset(actwk, 0, sizeof(actwk));
    memset(colorwk, 0, sizeof(colorwk));
    memset(colorwk2, 0, sizeof(colorwk2));
    memset(colorwk3, 0, sizeof(colorwk3));
    memset(colorwk4, 0, sizeof(colorwk4));
    memset(mapwk, 0, sizeof(mapwk));
    memset(hscroll_buffer, 0, sizeof(hscroll_buffer));
    memset(&keep_work_storage, 0, sizeof(keep_work_storage));
    memset(SprBmp, 0, sizeof(SprBmp));

    lpcolorwk = colorwk;
    lpcolorwk2 = colorwk2;
    lpcolorwk3 = colorwk3;
    lpcolorwk4 = colorwk4;
    pmapwk = mapwk;
    lphscrollbuff = hscroll_buffer;
    lpFadeFlag = &fade_flag_storage;
    lpKeepWork = &keep_work_storage;

    fade_flag_storage = 0;
    action_calls = 0;
    actionsub_calls = 0;
    frameout_calls = 0;
    patset_calls = 0;
    wave_request_count = 0;
    clr_sprite_debug_calls = 0;
    easprset_calls = 0;
    set_grid_calls = 0;
    random_value = 0;

    scra_h_posit.l = 0;
    scra_v_posit.l = 0;
    scrb_h_posit.l = 0;
    scrb_v_posit.l = 0;
    scrc_h_posit.l = 0;
    scrc_v_posit.l = 0;
    scrz_h_posit.l = 0;
    scrz_v_posit.l = 0;
    vscroll.l = 0;
    swdata.w = 0;
    debugflag.w = 0;

    clchg_cnt[0] = 65535;
    clchg_cnt[1] = 0;
    setcnt = 0;
    memset(colorsave, 0, sizeof(colorsave));
    clfflg = 0;
    byMskwk = 0;
    wColwk = 0;
    FadeCount = 0;
    gmtime1 = 0;
    gmtime0 = 0;

    ChangeTileBmp = stub_change_tile_bmp;
    CDPlay = stub_cd_play;
    WaveRequest = stub_wave_request;
    ClrSpriteDebug = stub_clr_sprite_debug;
    EAsprset = stub_easprset;
    SetGrid = stub_set_grid;
    sCloseFile = stub_close_file;
    sGetFileSize = stub_get_file_size;
    sReadFile = stub_read_file;
    sOpenFile = stub_open_file;
    sMemCpy = stub_memcpy;
    sMemSet = stub_memset;
    sRandom = stub_random;
    sPrintf = stub_printf;
    sOutputDebugString = stub_output_debug_string;
}

static void test_dll_meminit_wires_callbacks_and_buffers(test_context *ctx) {
    char *buffers[8];
    char **buffer_table[8];
    void *funcs[24] = {0};
    Sint32 fade_flag = 0;
    game_info keep_work;

    reset_state();
    memset(&keep_work, 0, sizeof(keep_work));
    buffers[0] = (char *)mapwk;
    buffers[1] = (char *)colorwk;
    buffers[2] = (char *)colorwk2;
    buffers[3] = (char *)colorwk3;
    buffers[4] = (char *)colorwk4;
    buffers[5] = (char *)hscroll_buffer;
    for (int i = 0; i < 6; ++i)
        buffer_table[i] = &buffers[i];
    buffer_table[6] = (char **)&fade_flag;
    buffer_table[7] = (char **)&keep_work;

    funcs[0] = stub_set_grid;
    funcs[1] = stub_easprset;
    funcs[2] = stub_clr_sprite_debug;
    funcs[3] = stub_wave_request;
    funcs[4] = stub_cd_play;
    funcs[6] = stub_change_tile_bmp;
    funcs[13] = stub_memset;
    funcs[14] = stub_memcpy;
    funcs[16] = stub_random;
    funcs[20] = stub_printf;
    funcs[21] = stub_output_debug_string;
    funcs[22] = stub_open_file;
    funcs[23] = stub_read_file;

    DLL_meminit(buffer_table, funcs);

    TEST_ASSERT_TRUE(ctx, SetGrid == stub_set_grid);
    TEST_ASSERT_TRUE(ctx, EAsprset == stub_easprset);
    TEST_ASSERT_TRUE(ctx, WaveRequest == stub_wave_request);
    TEST_ASSERT_TRUE(ctx, ChangeTileBmp == stub_change_tile_bmp);
    TEST_ASSERT_TRUE(ctx, sMemSet == stub_memset);
    TEST_ASSERT_TRUE(ctx, sRandom == stub_random);
    TEST_ASSERT_TRUE(ctx, pmapwk == mapwk);
    TEST_ASSERT_TRUE(ctx, lpcolorwk == colorwk);
    TEST_ASSERT_TRUE(ctx, lpFadeFlag == &fade_flag);
    TEST_ASSERT_TRUE(ctx, lpKeepWork == &keep_work);
    TEST_ASSERT_TRUE(ctx, keep_work.pSprBmp == SprBmp);

    DLL_memfree();
}

static void test_scroll_getters_and_debug_flag(test_context *ctx) {
    reset_state();
    vscroll.l = 0x12345678;
    scra_h_posiw.l = 0x01020304;
    scrb_h_posiw.l = 0x05060708;

    TEST_ASSERT_EQ_INT(ctx, 0x12345678, Get_vscroll());
    TEST_ASSERT_EQ_INT(ctx, 0x01020304, Get_scra_h_posiw());
    TEST_ASSERT_EQ_INT(ctx, 0x05060708, Get_scrb_h_posiw());

    SetDebugFlag(1);
    TEST_ASSERT_TRUE(ctx, (debugflag.w & 256) != 0);
    SetDebugFlag(0);
    TEST_ASSERT_TRUE(ctx, (debugflag.w & 256) == 0);
}

static void test_game_init_clears_state_sets_palette_and_starts_fade(
    test_context *ctx) {
    reset_state();
    actwk[7].actno = 99;
    scra_h_posit.l = 1234;

    game_init();

    TEST_ASSERT_EQ_INT(ctx, 0, actwk[7].actno);
    TEST_ASSERT_EQ_INT(ctx, 0, scra_h_posit.l);
    TEST_ASSERT_EQ_INT(ctx, 0, scrz_v_posit.l);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[1].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, colorsave[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorsave[1].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 1, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 1, wave_request_count);
    TEST_ASSERT_EQ_INT(ctx, 35, wave_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 420, gmtime0);
}

static void test_atom_search_uses_first_free_slot_or_fallback(test_context *ctx) {
    reset_state();
    for (int i = 4; i < 32; ++i)
        actwk[i].actno = 1;
    actwk[11].actno = 0;
    TEST_ASSERT_TRUE(ctx, atomwk_search() == &actwk[11]);

    actwk[11].actno = 1;
    TEST_ASSERT_TRUE(ctx, atomwk_search() == &actwk[4]);
}

static void test_game_spawns_and_fades_when_time_expires(test_context *ctx) {
    reset_state();
    gmtime0 = 1;
    gmtime1 = 0;
    vscroll.w.h = 10;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].actno);
    TEST_ASSERT_EQ_INT(ctx, 2, actwk[4].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, action_calls);
    TEST_ASSERT_EQ_INT(ctx, 1, patset_calls);
    TEST_ASSERT_EQ_INT(ctx, 0, gmtime0);
    TEST_ASSERT_EQ_INT(ctx, 1, gmtime1);
    TEST_ASSERT_EQ_INT(ctx, 6, vscroll.w.h);
    TEST_ASSERT_EQ_INT(ctx, 2, fade_flag_storage);
    TEST_ASSERT_EQ_INT(ctx, 1, clfflg);

    TEST_ASSERT_EQ_INT(ctx, 1, game());
}

static void test_game_button_press_starts_fade_without_waiting_for_zero(
    test_context *ctx) {
    reset_state();
    gmtime0 = 5;
    swdata.b.l = 128;

    TEST_ASSERT_EQ_INT(ctx, 0, game());

    TEST_ASSERT_EQ_INT(ctx, 0, gmtime0);
    TEST_ASSERT_EQ_INT(ctx, 2, fade_flag_storage);
}

static void test_sonic_state_machine_and_animation(test_context *ctx) {
    sprite_status actor;

    reset_state();
    memset(&actor, 0, sizeof(actor));
    gmtime1 = 1;
    actwk[0].patno = 3;

    sonic_act(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 288, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 416, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_calls);

    actor.yposi.w.h = 256;
    sonic_act(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 248, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 240, warp_get_work(&actor)->timer);

    warp_get_work(&actor)->timer = 1;
    sonic_act(&actor);
    TEST_ASSERT_EQ_INT(ctx, 3, actor.r_no0);

    actor.yposi.w.h = 104;
    sonic_act(&actor);
    TEST_ASSERT_EQ_INT(ctx, 4, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 96, actor.yposi.w.h);

    actor.patno = 4;
    actor.r_no0 = 4;
    actwk[0].patno = 0;
    gmtime1 = 1;
    sonic_act(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
}

static void test_atom_initializes_moves_and_frames_out(test_context *ctx) {
    sprite_status actor;

    reset_state();
    memset(&actor, 0, sizeof(actor));
    actwk[0].xposi.w.h = 288;
    actwk[0].yposi.w.h = 416;
    random_value = 0;

    atom(&actor);

    TEST_ASSERT_EQ_INT(ctx, 1, actor.r_no0);
    TEST_ASSERT_EQ_INT(ctx, 272, actor.xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 392, actor.yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 44, warp_get_work(&actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 32768, actor.sproffset);
    TEST_ASSERT_EQ_INT(ctx, 1, actionsub_calls);

    warp_get_work(&actor)->timer = 1;
    atom(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, warp_get_work(&actor)->timer);
    TEST_ASSERT_EQ_INT(ctx, 1, frameout_calls);

    actor.patno = 4;
    actor.r_no0 = 1;
    warp_get_work(&actor)->timer = 2;
    gmtime1 = 1;
    atom(&actor);
    TEST_ASSERT_EQ_INT(ctx, 0, actor.patno);
    TEST_ASSERT_EQ_INT(ctx, 1, warp_get_work(&actor)->timer);

    actor.r_no0 = 2;
    atom(&actor);
    TEST_ASSERT_EQ_INT(ctx, 2, actor.r_no0);
}

static void test_palette_cycle_and_color_fades(test_context *ctx) {
    reset_state();

    clchg();
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[17].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[17].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 0, colorwk[17].peBlue);
    TEST_ASSERT_EQ_INT(ctx, 32, colorwk[18].peRed);
    TEST_ASSERT_EQ_INT(ctx, 1, clchg_cnt[1]);

    clchg_cnt[0] = 65535;
    clchg_cnt[1] = 13;
    clchg();
    TEST_ASSERT_EQ_INT(ctx, 0, clchg_cnt[1]);

    colorwk[0].peRed = 192;
    colorwk[1].peRed = 224;
    colorwk[2].peGreen = 160;
    colorwk[3].peBlue = 96;
    woutcolor(1);
    woutcolor(5);
    woutcolor(9);
    woutcolor(0);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[1].peRed);
    TEST_ASSERT_EQ_INT(ctx, 192, colorwk[2].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 128, colorwk[3].peBlue);

    clwout();
    for (int i = 0; i < 32; ++i)
        FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 0, fade_flag_storage);
}

static void test_window_fade_saves_palette_and_restores_channels(
    test_context *ctx) {
    reset_state();
    colorwk[0].peRed = 10;
    colorwk[0].peGreen = 20;
    colorwk[0].peBlue = 30;

    clwin_init();
    TEST_ASSERT_EQ_INT(ctx, 10, colorsave[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 224, colorwk[0].peBlue);

    wincolor(64, 1);
    wincolor(64, 5);
    wincolor(64, 9);
    wincolor(64, 0);
    TEST_ASSERT_EQ_INT(ctx, 64, colorwk[0].peRed);
    TEST_ASSERT_EQ_INT(ctx, 64, colorwk[0].peGreen);
    TEST_ASSERT_EQ_INT(ctx, 64, colorwk[0].peBlue);

    clwin();
    for (int i = 0; i < 32; ++i)
        FadeProc();
    TEST_ASSERT_EQ_INT(ctx, 0, fade_flag_storage);
}

static void test_soundset_maps_requests_and_ignores_sentinel(test_context *ctx) {
    reset_state();

    soundset(171);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_request_count);

    soundset(144);
    soundset(216);

    TEST_ASSERT_EQ_INT(ctx, 2, wave_request_count);
    TEST_ASSERT_EQ_INT(ctx, 0, wave_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, 35, wave_requests[1]);
}

TEST_MAIN_BEGIN;
test_dll_meminit_wires_callbacks_and_buffers(&ctx);
test_scroll_getters_and_debug_flag(&ctx);
test_game_init_clears_state_sets_palette_and_starts_fade(&ctx);
test_atom_search_uses_first_free_slot_or_fallback(&ctx);
test_game_spawns_and_fades_when_time_expires(&ctx);
test_game_button_press_starts_fade_without_waiting_for_zero(&ctx);
test_sonic_state_machine_and_animation(&ctx);
test_atom_initializes_moves_and_frames_out(&ctx);
test_palette_cycle_and_color_fades(&ctx);
test_window_fade_saves_palette_and_restores_channels(&ctx);
test_soundset_maps_requests_and_ignores_sentinel(&ctx);
TEST_MAIN_END

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "support/test_runner.h"
#include "src/types.h"

bmp_info SprBmp[700];
Uint8 zone1scd[1000];

static int setgrid_count;
static int easprset_count;
static int clrspritedebug_count;
static int waverequest_count;
static int cdplay_count;
static int cdpause_count;
static Sint16 cdpause_requests[8];
static int changetilebmp_count;
static int waveallstop_count;
static int memset_count;
static int memcpy_count;
static int read_scrol_count;
static int read_block_count;
static int soundset_count;
static Sint16 soundset_requests[16];
static int sub_sync_count;
static Sint16 sub_sync_requests[32];
static int flagwkclr_count;
static int actsetchk_count;
static int action_count;
static int patset_count;
static int clchgctr_count;
static int scroll_count;
static int scrollwrt_count;
static int mapwrt_count;
static int mapinit_count;
static int scr_set_count;
static int scoreset_count;
static int scrinit_count;
static int cg_change_count;
static int colorset_count;
static int colorset2_count;
static int scdcnv_count;
static int flashin_count;
static int fadein0_count;
static int getroundstr_count;
static int fadeproc_count;
static int swdataset_count;

static game_info keep_work;
static Uint16 map_buffer[128];
static PALETTEENTRY colorwk[64], colorwk2[16], colorwk3[16], colorwk4[16];
static int_union hscroll_buffer[256];
static Sint32 fade_flag;
static Uint32 fake_hwnd;

Sint32 stub_SetGrid(Sint32 a, Sint32 b, Sint32 c, Sint32 d);
void stub_EAsprset(Sint16 a, Sint16 b, Uint16 c, Uint16 d, Uint16 e);
void stub_ClrSpriteDebug(void);
void stub_WaveRequest(Sint16 request);
void stub_CDPlay(Sint16 request);
void stub_CDPause(Sint16 request);
void stub_ChangeTileBmp(Sint32 a, Sint32 b);
void stub_WaveAllStop(void);
void stub_MemSet(void *dest, Uint8 value, Sint32 count);
void stub_MemCpy(void *dest, void *src, Sint32 count);
Sint32 stub_Random(void);
Sint32 stub_OpenFile(char *path);
Sint32 stub_ReadFile(Sint32 file, void *dest, Sint32 count);
void stub_CloseFile(Sint32 file);
void stub_Printf(char *fmt, const char *arg, ...);
void stub_OutputDebugString(char *text);

void ReadScrolMap(void);
void ReadBlockMap(void);
void soundset(Sint16 ReqNo);
void sub_sync(Sint16 ReqNo);
void flagwkclr(void);
void actsetchk(void);
void action(void);
void patset(void);
void clchgctr(void);
void scroll(void);
void scrollwrt(void);
void mapwrt(void);
void mapinit(void);
void scr_set(void);
void scoreset(void);
void scrinit(void);
void cg_change(void);
void colorset(Sint32 ColorNo);
void colorset2(Sint32 ColorNo);
Sint16 scdcnv(void);
void flashin(void);
void fadein0(void);
void SWdataSet(ushort_union sw1, ushort_union sw2);
Sint32 FadeProc(void);
void GetRoundStr(Uint16 StageNo, Uint8 Time_Flag, char *buf);

#define blockwk blockwk_equ_layout_for_game_test
#include "src/equ.c"
#undef blockwk
#include "src/game.c"

Sint32 stub_SetGrid(Sint32 a, Sint32 b, Sint32 c, Sint32 d) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    return ++setgrid_count;
}

void stub_EAsprset(Sint16 a, Sint16 b, Uint16 c, Uint16 d, Uint16 e) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    ++easprset_count;
}

void stub_ClrSpriteDebug(void) { ++clrspritedebug_count; }
void stub_WaveRequest(Sint16 request) {
    (void)request;
    ++waverequest_count;
}
void stub_CDPlay(Sint16 request) {
    (void)request;
    ++cdplay_count;
}
void stub_CDPause(Sint16 request) {
    if (cdpause_count < 8) {
        cdpause_requests[cdpause_count] = request;
    }
    ++cdpause_count;
}
void stub_ChangeTileBmp(Sint32 a, Sint32 b) {
    (void)a;
    (void)b;
    ++changetilebmp_count;
}
void stub_WaveAllStop(void) { ++waveallstop_count; }
void stub_MemSet(void *dest, Uint8 value, Sint32 count) {
    ++memset_count;
    memset(dest, value, (size_t)count);
}
void stub_MemCpy(void *dest, void *src, Sint32 count) {
    ++memcpy_count;
    memcpy(dest, src, (size_t)count);
}
Sint32 stub_Random(void) { return 1234; }
Sint32 stub_OpenFile(char *path) {
    (void)path;
    return 10;
}
Sint32 stub_ReadFile(Sint32 file, void *dest, Sint32 count) {
    (void)file;
    memset(dest, 0, (size_t)count);
    return count;
}
void stub_CloseFile(Sint32 file) { (void)file; }
void stub_Printf(char *fmt, const char *arg, ...) {
    (void)fmt;
    (void)arg;
}
void stub_OutputDebugString(char *text) { (void)text; }

void ReadScrolMap(void) { ++read_scrol_count; }
void ReadBlockMap(void) { ++read_block_count; }
void soundset(Sint16 ReqNo) {
    if (soundset_count < 16) {
        soundset_requests[soundset_count] = ReqNo;
    }
    ++soundset_count;
}
void sub_sync(Sint16 ReqNo) {
    if (sub_sync_count < 32) {
        sub_sync_requests[sub_sync_count] = ReqNo;
    }
    ++sub_sync_count;
}
void flagwkclr(void) {
    ++flagwkclr_count;
    memset(flagwork, 0, sizeof(flagwork));
}
void actsetchk(void) { ++actsetchk_count; }
void action(void) { ++action_count; }
void patset(void) { ++patset_count; }
void clchgctr(void) { ++clchgctr_count; }
void scroll(void) { ++scroll_count; }
void scrollwrt(void) { ++scrollwrt_count; }
void mapwrt(void) { ++mapwrt_count; }
void mapinit(void) { ++mapinit_count; }
void scr_set(void) { ++scr_set_count; }
void scoreset(void) { ++scoreset_count; }
void scrinit(void) { ++scrinit_count; }
void cg_change(void) { ++cg_change_count; }
void colorset(Sint32 ColorNo) {
    (void)ColorNo;
    ++colorset_count;
}
void colorset2(Sint32 ColorNo) {
    (void)ColorNo;
    ++colorset2_count;
}
Sint16 scdcnv(void) { return ++scdcnv_count; }
void flashin(void) { ++flashin_count; }
void fadein0(void) { ++fadein0_count; }
void SWdataSet(ushort_union sw1, ushort_union sw2) {
    ++swdataset_count;
    swdata1 = sw1;
    swdata2 = sw2;
}
Sint32 FadeProc(void) { return ++fadeproc_count; }
void GetRoundStr(Uint16 StageNo, Uint8 Time_Flag, char *buf) {
    ++getroundstr_count;
    buf[0] = (char)StageNo;
    buf[1] = (char)Time_Flag;
    buf[2] = 0;
}

static void reset_game_state(void) {
    memset(&flagwork, 0, sizeof(flagwork));
    memset(&gameflag, 0, sizeof(gameflag));
    memset(&gametimer, 0, sizeof(gametimer));
    memset(&stageno, 0, sizeof(stageno));
    memset(&pltime, 0, sizeof(pltime));
    memset(&debugflag, 0, sizeof(debugflag));
    memset(&demoflag, 0, sizeof(demoflag));
    memset(&editmode, 0, sizeof(editmode));
    memset(&systemtimer, 0, sizeof(systemtimer));
    memset(&scra_h_posiw, 0, sizeof(scra_h_posiw));
    memset(&scra_v_posiw, 0, sizeof(scra_v_posiw));
    memset(&scrb_h_posiw, 0, sizeof(scrb_h_posiw));
    memset(&scrb_v_posiw, 0, sizeof(scrb_v_posiw));
    memset(&scrc_h_posiw, 0, sizeof(scrc_h_posiw));
    memset(&scrc_v_posiw, 0, sizeof(scrc_v_posiw));
    memset(&scrz_h_posiw, 0, sizeof(scrz_h_posiw));
    memset(&scrz_v_posiw, 0, sizeof(scrz_v_posiw));
    memset(&swdata, 0, sizeof(swdata));
    memset(&swdata1, 0, sizeof(swdata1));
    memset(&swdata2, 0, sizeof(swdata2));
    memset(&vscroll, 0, sizeof(vscroll));
    memset(&pauseflag, 0, sizeof(pauseflag));
    memset(&scra_h_posit, 0, sizeof(scra_h_posit));
    memset(&scra_v_posit, 0, sizeof(scra_v_posit));
    memset(&scrb_h_posit, 0, sizeof(scrb_h_posit));
    memset(&scrb_v_posit, 0, sizeof(scrb_v_posit));
    memset(&scrc_h_posit, 0, sizeof(scrc_h_posit));
    memset(&scrc_v_posit, 0, sizeof(scrc_v_posit));
    memset(&scrz_h_posit, 0, sizeof(scrz_h_posit));
    memset(&scrz_v_posit, 0, sizeof(scrz_v_posit));
    memset(&scrflaga, 0, sizeof(scrflaga));
    memset(&scrflagb, 0, sizeof(scrflagb));
    memset(&scrflagc, 0, sizeof(scrflagc));
    memset(&scrflagz, 0, sizeof(scrflagz));
    memset(&byecnt0, 0, sizeof(byecnt0));
    memset(&byecnt1, 0, sizeof(byecnt1));
    memset(&waterdirec, 0, sizeof(waterdirec));
    memset(&keep_work, 0, sizeof(keep_work));
    memset(actwk, 0, sizeof(actwk));
    memset(switchflag, 0, sizeof(switchflag));
    memset(flowercnt, 0, sizeof(flowercnt));
    memset(flowerposi, 0, sizeof(flowerposi));
    memset(playposiwk, 0, sizeof(playposiwk));
    pmapwk = map_buffer;
    lpcolorwk = colorwk;
    lpcolorwk2 = colorwk2;
    lpcolorwk3 = colorwk3;
    lpcolorwk4 = colorwk4;
    lphscrollbuff = hscroll_buffer;
    lpFadeFlag = &fade_flag;
    lpKeepWork = &keep_work;
    lpghWnd = &fake_hwnd;
    sMemSet = stub_MemSet;
    sMemCpy = stub_MemCpy;
    CDPause = stub_CDPause;
    SetGrid = stub_SetGrid;
    EAsprset = stub_EAsprset;
    ClrSpriteDebug = stub_ClrSpriteDebug;
    WaveRequest = stub_WaveRequest;
    CDPlay = stub_CDPlay;
    ChangeTileBmp = stub_ChangeTileBmp;
    WaveAllStop = stub_WaveAllStop;
    sRandom = stub_Random;
    sOpenFile = stub_OpenFile;
    sReadFile = stub_ReadFile;
    sCloseFile = stub_CloseFile;
    sPrintf = stub_Printf;
    sOutputDebugString = stub_OutputDebugString;
    setgrid_count = easprset_count = clrspritedebug_count = 0;
    waverequest_count = cdplay_count = cdpause_count = 0;
    memset(cdpause_requests, 0, sizeof(cdpause_requests));
    changetilebmp_count = waveallstop_count = 0;
    memset_count = memcpy_count = 0;
    read_scrol_count = read_block_count = 0;
    soundset_count = 0;
    memset(soundset_requests, 0, sizeof(soundset_requests));
    sub_sync_count = 0;
    memset(sub_sync_requests, 0, sizeof(sub_sync_requests));
    flagwkclr_count = actsetchk_count = action_count = patset_count = 0;
    clchgctr_count = scroll_count = scrollwrt_count = mapwrt_count = 0;
    mapinit_count = scr_set_count = scoreset_count = scrinit_count = 0;
    cg_change_count = 0;
    colorset_count = colorset2_count = 0;
    scdcnv_count = flashin_count = fadein0_count = 0;
    getroundstr_count = fadeproc_count = swdataset_count = 0;
    generate_flag = 0;
    time_flag = 0;
    ta_flag = 0;
    gf_flag = 0;
    projector_flag = 0;
    special_flag = 0;
    extrascore = 0;
    emie3end = 0;
    flagworkcnt = 0;
    flagworkcnt2 = 0;
    clrspflg_save = 0;
    play_start = 0;
    plflag = 0;
    plring = 0;
    plscore = 0;
    plpower_m = 0;
    plpower_s = 0;
    waterposi = 0;
    waterposi_m = 0;
    watermoveposi = 0;
    waterspeed = 0;
    waterflag = 0;
    waterflag2 = 0;
    water_flag = 0;
    watercoliflag = 0;
    backto_cnt = 0;
    demo_cnt = 0;
    markerno = 0;
    time_stop = 0;
    startcolor = 0;
    colorcnt = 0;
    tv_flag = 0;
    init_flag = 0;
    plautoflag = 0;
    enkeino = 0;
    scralim_down = 0;
    scralim_n_down = 0;
    scrflagaw.w = 0;
    scrflagbw.w = 0;
    scrflagcw.w = 0;
    scrflagzw.w = 0;
    PauseIcon = 0;
    Interupt_Counter = 0;
    pl_suu = 3;
    pl_air = 30;
}

static void test_dll_meminit_and_exports(test_context *ctx) {
    void *funcs[26] = {0};
    char *map_ptr = (char *)map_buffer;
    char *c1_ptr = (char *)colorwk;
    char *c2_ptr = (char *)colorwk2;
    char *c3_ptr = (char *)colorwk3;
    char *c4_ptr = (char *)colorwk4;
    char *hs_ptr = (char *)hscroll_buffer;
    char **bufs[10];

    reset_game_state();
    keep_work.plring = 12;
    keep_work.plscore = 3456;
    keep_work.stageno.w = 0x0201;
    keep_work.play_start = 3;
    keep_work.pl_suu = 7;
    keep_work.generate_flag = 1;
    keep_work.time_flag = 2;
    keep_work.plflag = 1;
    keep_work.clrspflg_save = 4;
    keep_work.ta_flag = 5;
    keep_work.gf_flag = 6;
    keep_work.projector_flag = 7;
    keep_work.extrascore = 8000;
    keep_work.emie3end = 9;
    keep_work.flagworkcnt = 10;
    keep_work.flagworkcnt2 = 11;
    keep_work.flagwork[3] = 99;

    funcs[0] = stub_SetGrid;
    funcs[1] = stub_EAsprset;
    funcs[2] = stub_ClrSpriteDebug;
    funcs[3] = stub_WaveRequest;
    funcs[4] = stub_CDPlay;
    funcs[5] = stub_CDPause;
    funcs[6] = stub_ChangeTileBmp;
    funcs[10] = stub_WaveAllStop;
    funcs[13] = stub_MemSet;
    funcs[14] = stub_MemCpy;
    funcs[16] = stub_Random;
    funcs[20] = stub_Printf;
    funcs[21] = stub_OutputDebugString;
    funcs[22] = stub_OpenFile;
    funcs[23] = stub_ReadFile;
    funcs[24] = stub_CloseFile;
    bufs[0] = &map_ptr;
    bufs[1] = &c1_ptr;
    bufs[2] = &c2_ptr;
    bufs[3] = &c3_ptr;
    bufs[4] = &c4_ptr;
    bufs[5] = &hs_ptr;
    bufs[6] = (char **)&fade_flag;
    bufs[7] = (char **)&keep_work;
    bufs[8] = 0;
    bufs[9] = (char **)&fake_hwnd;

    DLL_meminit(bufs, funcs);

    TEST_ASSERT_TRUE(ctx, SetGrid == stub_SetGrid);
    TEST_ASSERT_TRUE(ctx, CDPause == stub_CDPause);
    TEST_ASSERT_TRUE(ctx, pmapwk == map_buffer);
    TEST_ASSERT_TRUE(ctx, lpKeepWork == &keep_work);
    TEST_ASSERT_TRUE(ctx, lpKeepWork->pSprBmp == SprBmp);
    TEST_ASSERT_EQ_INT(ctx, 12, plring);
    TEST_ASSERT_EQ_INT(ctx, 3456, plscore);
    TEST_ASSERT_EQ_INT(ctx, 0x0201, stageno.w);
    TEST_ASSERT_EQ_INT(ctx, 99, flagwork[3]);
    TEST_ASSERT_EQ_INT(ctx, 1, memcpy_count);

    TEST_ASSERT_TRUE(ctx, ExportedFunctions.game_init == game_init);
    TEST_ASSERT_TRUE(ctx, ExportedFunctions.dll_meminit == DLL_meminit);
    ExportedFunctions.SetDebugFlag(1);
    TEST_ASSERT_EQ_INT(ctx, 256, debugflag.w);
    ExportedFunctions.SetDebugFlag(0);
    TEST_ASSERT_EQ_INT(ctx, 0, debugflag.w);
    ExportedFunctions.GetRoundStr(7, 8, (char *)map_buffer);
    TEST_ASSERT_EQ_INT(ctx, 1, getroundstr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, ExportedFunctions.FadeProc());
}

static void test_small_helpers_and_flower_setup(test_context *ctx) {
    reset_game_state();
    vscroll.l = 123456;
    scra_h_posiw.l = 234567;
    scrb_h_posiw.l = 345678;
    TEST_ASSERT_EQ_INT(ctx, 123456, Get_vscroll());
    TEST_ASSERT_EQ_INT(ctx, 234567, Get_scra_h_posiw());
    TEST_ASSERT_EQ_INT(ctx, 345678, Get_scrb_h_posiw());

    play_act_set();
    TEST_ASSERT_EQ_INT(ctx, 1, actwk[0].actno);
    plflag = 1;
    play_act_set();
    TEST_ASSERT_EQ_INT(
        ctx, 120,
        player_work_get(&actwk[0])->damage_invulnerability_timer);

    scdset();
    TEST_ASSERT_TRUE(ctx, scdadr == zone1scd);
    DLL_memfree();

    time_flag = 1;
    flowercnt[1] = 3;
    flowerposi[64].w.h = 10;
    flowerposi[64].w.l = 20;
    flowerposi[65].w.h = 30;
    flowerposi[65].w.l = 40;
    flow_act_set();
    TEST_ASSERT_EQ_INT(ctx, 31, actwk[32].actno);
    TEST_ASSERT_EQ_INT(ctx, 10, actwk[32].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 20, actwk[32].yposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 31, actwk[33].actno);
    TEST_ASSERT_EQ_INT(ctx, 30, actwk[33].xposi.w.h);
    TEST_ASSERT_EQ_INT(ctx, 40, actwk[33].yposi.w.h);

    sys_pattim = 0;
    sys_patno = 3;
    sys_pattim2 = 0;
    sys_patno2 = 3;
    sys_pattim3 = 0;
    sys_patno3 = 5;
    sys_pattim4 = 2;
    sys_ringtimer = 0;
    syspatchg();
    TEST_ASSERT_EQ_INT(ctx, 11, sys_pattim);
    TEST_ASSERT_EQ_INT(ctx, 2, sys_patno);
    TEST_ASSERT_EQ_INT(ctx, 7, sys_pattim2);
    TEST_ASSERT_EQ_INT(ctx, 0, sys_patno2);
    TEST_ASSERT_EQ_INT(ctx, 7, sys_pattim3);
    TEST_ASSERT_EQ_INT(ctx, 0, sys_patno3);
    TEST_ASSERT_EQ_INT(ctx, 1, sys_pattim4);
}

static void test_pause_sound_and_da_paths(test_context *ctx) {
    reset_game_state();
    pauseflag.b.h = 0;
    time_flag = 0;
    plpower_m = 1;
    sdfdout();
    TEST_ASSERT_EQ_INT(ctx, 171, soundset_requests[0]);

    pauseflag.b.h = 128;
    sdfdin();
    TEST_ASSERT_EQ_INT(ctx, 145, sub_sync_requests[0]);

    reset_game_state();
    pauseflag.b.h = 128;
    plpower_m = 0;
    sdfdin();
    TEST_ASSERT_EQ_INT(ctx, 214, sub_sync_requests[0]);

    reset_game_state();
    stageno.b.h = 0;
    time_flag = 2;
    generate_flag = 1;
    da_set();
    TEST_ASSERT_EQ_INT(ctx, 16, sub_sync_requests[0]);

    reset_game_state();
    stageno.b.h = 6;
    time_flag = 2;
    ta_flag = 1;
    da_set();
    TEST_ASSERT_EQ_INT(ctx, 102, sub_sync_requests[0]);
}

static void test_game_pause_exit_and_active_paths(test_context *ctx) {
    reset_game_state();
    swdata1.b.l = 128;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, pauseflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, cdpause_requests[0]);

    reset_game_state();
    pauseflag.b.h = 1;
    swdata1.b.l = 128;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 0, pauseflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, cdpause_requests[0]);

    reset_game_state();
    swdata1.b.l = 128;
    swdata2.w = 32896;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, pauseflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, PauseIcon);

    reset_game_state();
    pauseflag.b.h = 1;
    PauseIcon = 1;
    swdata1.b.l = 128;
    swdata2.w = 32896;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 0, pauseflag.b.h);
    TEST_ASSERT_EQ_INT(ctx, 0, PauseIcon);

    reset_game_state();
    swdata1.b.l = 112;
    pl_suu = 0;
    TEST_ASSERT_EQ_INT(ctx, 1, game());
    TEST_ASSERT_EQ_INT(ctx, 1, gameflag.w);
    TEST_ASSERT_EQ_INT(ctx, 0, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 14, sub_sync_requests[0]);
    TEST_ASSERT_EQ_INT(ctx, -1, (Sint32)keep_work.ta_time);

    reset_game_state();
    gameflag.w = 2;
    gameflag.b.h = 0;
    plring = 9;
    plscore = 1000;
    stageno.w = 0x0102;
    pltime.b.b3 = 1;
    pltime.b.b2 = 2;
    pltime.b.b4 = 3;
    TEST_ASSERT_EQ_INT(ctx, 2, game());
    TEST_ASSERT_EQ_INT(ctx, 9, keep_work.plring);
    TEST_ASSERT_EQ_INT(ctx, 7263, keep_work.ta_time);
    TEST_ASSERT_EQ_INT(ctx, 1, flagwkclr_count);

    reset_game_state();
    actwk[0].actno = 1;
    actwk[0].r_no0 = 0;
    byecnt0.w = 1;
    byecnt1.w = 3;
    backto_cnt = 1;
    scra_h_posit.l = 10;
    scra_v_posit.l = 20;
    scrb_h_posit.l = 30;
    scrb_v_posit.l = 40;
    scrflaga.w = 5;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, actsetchk_count);
    TEST_ASSERT_EQ_INT(ctx, 1, action_count);
    TEST_ASSERT_EQ_INT(ctx, 1, scroll_count);
    TEST_ASSERT_EQ_INT(ctx, 1, patset_count);
    TEST_ASSERT_EQ_INT(ctx, 1, clchgctr_count);
    TEST_ASSERT_EQ_INT(ctx, 1, scrollwrt_count);
    TEST_ASSERT_EQ_INT(ctx, 1, scoreset_count);
    TEST_ASSERT_EQ_INT(ctx, 2, byecnt0.w);
    TEST_ASSERT_EQ_INT(ctx, 4, byecnt1.w);
    TEST_ASSERT_EQ_INT(ctx, 2, backto_cnt);
    TEST_ASSERT_EQ_INT(ctx, 1, systemtimer.l);
    TEST_ASSERT_EQ_INT(ctx, 10, scra_h_posiw.l);
    TEST_ASSERT_EQ_INT(ctx, 5, scrflagaw.w);

    reset_game_state();
    editmode.w = 1;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 1, scroll_count);

    reset_game_state();
    actwk[0].r_no0 = 6;
    scra_v_posit.w.h = 777;
    TEST_ASSERT_EQ_INT(ctx, 0, game());
    TEST_ASSERT_EQ_INT(ctx, 777, scralim_down);
    TEST_ASSERT_EQ_INT(ctx, 0, scroll_count);
}

static void test_game_init_paths(test_context *ctx) {
    reset_game_state();
    clrspflg_save = 127;
    ta_flag = 0;
    play_start = 0;
    stageno.b.h = 2;
    stageno.b.l = 1;
    time_flag = 1;
    gmmode.b.h = 255;
    game_init();
    TEST_ASSERT_EQ_INT(ctx, 1, read_scrol_count);
    TEST_ASSERT_EQ_INT(ctx, 1, read_block_count);
    TEST_ASSERT_EQ_INT(ctx, 1, generate_flag);
    TEST_ASSERT_EQ_INT(ctx, 3, pl_suu);
    TEST_ASSERT_EQ_INT(ctx, 30, pl_air);
    TEST_ASSERT_EQ_INT(ctx, 28, actwk[2].actno);
    TEST_ASSERT_EQ_INT(ctx, 60, actwk[4].actno);
    TEST_ASSERT_EQ_INT(ctx, 1, plautoflag);
    TEST_ASSERT_EQ_INT(ctx, 32, startcolor);
    TEST_ASSERT_EQ_INT(ctx, 47, colorcnt);
    TEST_ASSERT_EQ_INT(ctx, 1, tv_flag);
    TEST_ASSERT_EQ_INT(ctx, 127, gmmode.b.h);
    TEST_ASSERT_EQ_INT(ctx, 1, init_flag);
    TEST_ASSERT_EQ_INT(ctx, 1, fadein0_count);

    reset_game_state();
    play_start = 0;
    ta_flag = 1;
    game_init();
    TEST_ASSERT_EQ_INT(ctx, 1, pl_suu);

    reset_game_state();
    play_start = 3;
    ta_flag = 1;
    time_flag = 128;
    game_init();
    TEST_ASSERT_EQ_INT(ctx, 1, flashin_count);
    TEST_ASSERT_EQ_INT(ctx, 3, play_start);
}

TEST_MAIN_BEGIN;
test_dll_meminit_and_exports(&ctx);
test_small_helpers_and_flower_setup(&ctx);
test_pause_sound_and_da_paths(&ctx);
test_game_pause_exit_and_active_paths(&ctx);
test_game_init_paths(&ctx);
TEST_MAIN_END

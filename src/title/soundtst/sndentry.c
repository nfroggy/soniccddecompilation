#include "../../types.h"
#include "../common/score_data_types.h"
#include "../common/engine_dll.h"
#include "../common/hmx_types.h"
#include "sndentry.h"
#include "../common/hmx_oeeactl.h"
#include "../common/palt.h"
#include "snddo.h"
#include "sndsprt.h"

static void SNDWait(void);
static void SetKeybordKey1(void);

Uint16 nSequenceNum;
Sint32 nTimerCunt;
UintPtr hSurf;
UintPtr hWnd;
PALETTEENTRY *lpColorwk;
Uint16 swData1;
Uint16 swData2;
draw_context *s_ctx;
extern PALETTEENTRY tblPal4[];
extern PALETTEENTRY tblPal3[];
extern PALETTEENTRY tblPal2[];
extern PALETTEENTRY tblPal1[];
draw_context *(*get_draw_context_module)(void);
void (*FlipToScreen_module)(void);
void *(*ld_load_cmpfile_module)(hmx_environment *, char *);
void (*ld_bitmap_4to8_module)(void *, void *, Sint32, Sint32, Sint32, Sint32,
                              Sint32);
void (*hmx_background_set_background_module)(hmx_background *, Sint32);
hmx_bitmap *(*hmx_bitmap_create_module)(hmx_environment *, Sint32, Sint32);
void (*hmx_bitmap_release_module)(hmx_environment *, hmx_bitmap *);
void *(*hmx_bitmap_get_scan0_module)(hmx_bitmap *);
void (*hmx_bitmap_set_transparency_module)(hmx_bitmap *, Sint32);
void (*hmx_free_module)(hmx_environment *, void *);
hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32, Sint32,
                                    Sint32);
void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);
hmx_renderer_base *(*hmx_grid_base_module)(hmx_grid *);
void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32, Sint32);
void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32, hmx_bitmap *,
                                 Sint32);
void (*hmx_grid_set_horz_offsets_module)(hmx_grid *, Sint32 *);
void (*hmx_renderer_context_add_module)(hmx_renderer_context *, Sint32,
                                        hmx_renderer_base *);
void (*hmx_renderer_context_clear_module)(hmx_renderer_context *);
void (*hmx_renderer_context_draw_module)(hmx_renderer_context *, hmx_surface *);
hmx_renderer_base *(*hmx_sprite_base_module)(hmx_sprite *);
void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
hmx_ddagrid *(*hmx_ddagrid_create_module)(hmx_environment *, Sint32, Sint32,
                                          Sint32, Sint32, Sint32, Sint32);
void (*hmx_ddagrid_release_module)(hmx_environment *, hmx_ddagrid *);
void (*hmx_ddagrid_set_scan_module)(hmx_ddagrid *, Sint32, Sint32, Sint32,
                                    Sint32, Sint32);
void (*hmx_ddagrid_set_tile_module)(hmx_ddagrid *, Sint32, Sint32, hmx_bitmap *,
                                    Sint32);
void *(*sMemAlloc)(Sint32);
void (*sMemFree)(void *);
void (*sMemCpy)(void *, void *, Sint32);
Sint32 (*sGetFileSize)(Sint32);
Sint32 (*sOpenFile)(char *);
Sint32 (*sReadFile)(Sint32, void *, Sint32);
void (*sCloseFile)(Sint32);
void (*sPrintf)(char *, char *);
void (*sOutputDebugString)(char *);
void (*CDPlay)(Sint16);
void (*CDPause)(void);
void (*WaveRequest)(Sint16);
static Uint16 nSaveSequenceNum;
static Sint32 nWaitTime;
static Uint32 bWaitKeyOff;
hmx_environment *g_loader_module;
hmx_environment *g_env_module;
extern Sint32 bKakusi;
dlink_export ExportedFunctions = {(void (*)(void)) & DLLInit,
                                  (void (*)(void)) & DLLMain,
                                  (void (*)(char ***, void **)) & DLLEnd,
                                  (void (*)(void)) & SWdataSet,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0};

void OESetSequenceNum(Uint16 num) {
    nSequenceNum = num;
    nTimerCunt = -1;
}

void KeyWait(void) {
    nSaveSequenceNum = nSequenceNum;
    nWaitTime = 6;
    OESetSequenceNum(3);
}

void WaitKeyOff(void) {
    nSaveSequenceNum = nSequenceNum;
    bWaitKeyOff = 1;
    nWaitTime = 0;
    OESetSequenceNum(3);
}

void Wait(Sint32 time) {
    nSaveSequenceNum = nSequenceNum;
    bWaitKeyOff = 0;
    nWaitTime = time;
    OESetSequenceNum(3);
}

static void SNDWait(void) {
    if (bWaitKeyOff == 0 && nTimerCunt > nWaitTime) {
        OESetSequenceNum(nSaveSequenceNum);
    }
    if (bWaitKeyOff != 0) {
        if (swData1 == 0)
            bWaitKeyOff = 0;
    }
}

void DLLInit(engine_dll *lpDllIn, char ***pBufTbl, void **pFuncTbl) {

    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;

    sMemAlloc = *pFuncTbl++;
    sMemFree = *pFuncTbl++;
    ++pFuncTbl;
    sMemCpy = *pFuncTbl++;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    sPrintf = *pFuncTbl++;
    sOutputDebugString = *pFuncTbl++;
    sOpenFile = *pFuncTbl++;
    sReadFile = *pFuncTbl++;
    sCloseFile = *pFuncTbl++;
    sGetFileSize = *pFuncTbl++;

    get_draw_context_module = *pFuncTbl++;
    FlipToScreen_module = *pFuncTbl++;
    ld_bitmap_4to8_module = *pFuncTbl++;
    ld_load_cmpfile_module = *pFuncTbl++;
    hmx_background_set_background_module = *pFuncTbl++;
    hmx_bitmap_create_module = *pFuncTbl++;
    hmx_bitmap_release_module = *pFuncTbl++;
    hmx_bitmap_get_scan0_module = *pFuncTbl++;
    hmx_bitmap_set_transparency_module = *pFuncTbl++;
    hmx_free_module = *pFuncTbl++;
    hmx_grid_create_module = *pFuncTbl++;
    hmx_grid_release_module = *pFuncTbl++;
    hmx_grid_base_module = *pFuncTbl++;
    hmx_grid_set_position_module = *pFuncTbl++;
    hmx_grid_set_view_module = *pFuncTbl++;
    hmx_grid_set_tile_module = *pFuncTbl++;
    hmx_grid_set_horz_offsets_module = *pFuncTbl++;
    hmx_renderer_context_add_module = *pFuncTbl++;
    hmx_renderer_context_clear_module = *pFuncTbl++;
    hmx_renderer_context_draw_module = *pFuncTbl++;
    hmx_sprite_base_module = *pFuncTbl++;
    hmx_sprite_set_position_module = *pFuncTbl++;
    hmx_sprite_set_bitmap_module = *pFuncTbl++;
    hmx_sprite_set_flags_module = *pFuncTbl++;

    hmx_ddagrid_create_module = *pFuncTbl++;
    hmx_ddagrid_release_module = *pFuncTbl++;
    hmx_ddagrid_set_scan_module = *pFuncTbl++;
    hmx_ddagrid_set_tile_module = *pFuncTbl++;

    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;
    ++pBufTbl;

    g_env_module = (hmx_environment *)**pBufTbl++;
    g_loader_module = (hmx_environment *)**pBufTbl++;

    s_ctx = get_draw_context_module();

    hWnd = lpDllIn->hWnd;
    hSurf = lpDllIn->hSurf;
    lpColorwk = lpDllIn->lpColorwk;
    CDPlay = lpDllIn->CDPlay;
    CDPause = lpDllIn->CDPause;
    WaveRequest = lpDllIn->WaveRequest;

    OEClrset(tblPal1, 0);
    OEClrset(tblPal2, 1);
    OEClrset(tblPal3, 2);
    OEClrset(tblPal4, 3);

    OECreateEA();
    nSequenceNum = 1;
    nTimerCunt = 0;
}

void DLLEnd(void) {
    if (bKakusi != 0) {
        MC_SONICDelete();
        return;
    }
    OEDeleteEA();
    DeleteAllCharSprt();
}

static void SetKeybordKey1(void) {}

void SWdataSet(ushort_union sw1, ushort_union sw2) {
    swData1 = sw1.w;
    swData2 = sw2.w;
    SetKeybordKey1();
}

Sint32 DLLMain(void) {
    Sint32 ret = 0;

    switch (nSequenceNum) {
    case 1:
        SNDStart();
        break;

    case 2:
        SNDMenu();
        break;

    case 3:
        SNDWait();
        break;
    case 4:
        SNDKakusi();
        break;
    case 999:
        ret = SNDEnd();
        break;
    case 5:
        SNDSpecialMes();
        break;
    case 6:
        ret = SNDEnd2();
        break;
    case 7:
        ret = SNDEnd3();
        break;
    }

    if (nTimerCunt == 0x7FFFFFFF)
        nTimerCunt = 5184000;
    else
        ++nTimerCunt;

    SNDDraw();
    return ret;
}

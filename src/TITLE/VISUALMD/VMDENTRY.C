#include "..\..\TYPES.H"
#include "..\COMMON\SCORE_DATA_TYPES.H"
#include "..\COMMON\ENGINE_DLL.H"
#include "..\COMMON\HMX_TYPES.H"
#include "VMDENTRY.H"
#include "..\COMMON\HMX_OEEACTL.H"
#include "..\COMMON\PALT.H"
#include "VMDDO.H"
#include "VMDSPRT.H"

static void VMDWait(void);

extern PALETTEENTRY tblPal4[];
extern PALETTEENTRY tblPal3[];
extern PALETTEENTRY tblPal2[];
extern PALETTEENTRY tblPal1[];
hmx_environment* g_env_module = 0;
hmx_environment* g_loader_module = 0;
static unsigned int bWaitKeyOff = 0;
static int nWaitTime = 0;
static unsigned short nSaveSequenceNum = 0;
void(*sOutputDebugString)(char*) = 0;
void(*sPrintf)(char*, char*) = 0;
void(*sCloseFile)(int) = 0;
int(*sReadFile)(int, void*, int) = 0;
int(*sOpenFile)(char*) = 0;
int(*sGetFileSize)(int) = 0;
void(*sMemCpy)(void*, void*, int) = 0;
void(*sMemFree)(void*) = 0;
void*(*sMemAlloc)(int) = 0;
void(*hmx_ddagrid_set_tile_module)(hmx_ddagrid*, int, int, hmx_bitmap*, int) = 0;
void(*hmx_ddagrid_set_scan_module)(hmx_ddagrid*, int, int, int, int, int) = 0;
void(*hmx_ddagrid_release_module)(hmx_environment*, hmx_ddagrid*) = 0;
hmx_ddagrid*(*hmx_ddagrid_create_module)(hmx_environment*, int, int, int, int, int, int) = 0;
void(*hmx_sprite_set_flags_module)(hmx_sprite*, int) = 0;
void(*hmx_sprite_set_bitmap_module)(hmx_sprite*, hmx_bitmap*) = 0;
void(*hmx_sprite_set_position_module)(hmx_sprite*, int, int) = 0;
hmx_renderer_base*(*hmx_sprite_base_module)(hmx_sprite*) = 0;
void(*hmx_renderer_context_draw_module)(hmx_renderer_context*, hmx_surface*) = 0;
void(*hmx_renderer_context_clear_module)(hmx_renderer_context*) = 0;
void(*hmx_renderer_context_add_module)(hmx_renderer_context*, int, hmx_renderer_base*) = 0;
void(*hmx_grid_set_horz_offsets_module)(hmx_grid*, int*) = 0;
void(*hmx_grid_set_tile_module)(hmx_grid*, int, int, hmx_bitmap*, int) = 0;
void(*hmx_grid_set_view_module)(hmx_grid*, int, int, int, int) = 0;
void(*hmx_grid_set_position_module)(hmx_grid*, int, int) = 0;
hmx_renderer_base*(*hmx_grid_base_module)(hmx_grid*) = 0;
void(*hmx_grid_release_module)(hmx_environment*, hmx_grid*) = 0;
hmx_grid*(*hmx_grid_create_module)(hmx_environment*, int, int, int, int) = 0;
void(*hmx_free_module)(hmx_environment*, void*) = 0;
void(*hmx_bitmap_set_transparency_module)(hmx_bitmap*, int) = 0;
void*(*hmx_bitmap_get_scan0_module)(hmx_bitmap*) = 0;
void(*hmx_bitmap_release_module)(hmx_environment*, hmx_bitmap*) = 0;
hmx_bitmap*(*hmx_bitmap_create_module)(hmx_environment*, int, int) = 0;
void(*hmx_background_set_background_module)(hmx_background*, int) = 0;
void(*ld_bitmap_4to8_module)(void*, void*, int, int, int, int, int) = 0;
void*(*ld_load_cmpfile_module)(hmx_environment*, char*) = 0;
void(*FlipToScreen_module)(void) = 0;
draw_context*(*get_draw_context_module)(void) = 0;
unsigned int(*WriteIndx)(int, unsigned int) = 0;
int(*ReadIndx)(unsigned int) = 0;
unsigned int(*CreateScore)(void) = 0;
unsigned int(*CloseScore)(unsigned int) = 0;
unsigned int(*OpenScore)(int) = 0;
unsigned int(*WriteScore)(int, char*, unsigned int) = 0;
unsigned int(*ReadScore)(int, char*, unsigned int) = 0;
void(*WaveRequest)(short) = 0;
void(*CDPause)(short) = 0;
void(*CDPlay)(short) = 0;
draw_context* s_ctx = 0;
unsigned int* lpbVisualmode = 0;
unsigned short swData2 = 0;
unsigned short swData1 = 0;
PALETTEENTRY* lpColorwk = 0;
unsigned int hWnd = 0;
unsigned int hSurf = 0;
int nTimerCunt = 0;
unsigned short nSequenceNum = 0;
dlink_export ExportedFunctions = {
  (void (*)(void))&DLLInit,
  (void (*)(void))&DLLMain,
  (void (*)(char***, void**))&DLLEnd,
  (void (*)(void))&SWdataSet,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};


































void OESetSequenceNum(unsigned short num) { /* Line 128, Address: 0x1000450 */
  nSequenceNum = num; /* Line 129, Address: 0x1000458 */
  nTimerCunt = -1; /* Line 130, Address: 0x1000464 */
} /* Line 131, Address: 0x1000470 */




void KeyWait(void) { /* Line 136, Address: 0x1000480 */
  nSaveSequenceNum = nSequenceNum; /* Line 137, Address: 0x1000488 */
  nWaitTime = 6; /* Line 138, Address: 0x1000498 */
  OESetSequenceNum(3); /* Line 139, Address: 0x10004a4 */
} /* Line 140, Address: 0x10004b0 */




void WaitKeyOff(void) { /* Line 145, Address: 0x10004c0 */
  nSaveSequenceNum = nSequenceNum; /* Line 146, Address: 0x10004c8 */
  bWaitKeyOff = 1; /* Line 147, Address: 0x10004d8 */
  nWaitTime = 0; /* Line 148, Address: 0x10004e4 */
  OESetSequenceNum(3); /* Line 149, Address: 0x10004ec */
} /* Line 150, Address: 0x10004f8 */




void Wait(int time) { /* Line 155, Address: 0x1000510 */
  nSaveSequenceNum = nSequenceNum; /* Line 156, Address: 0x100051c */
  bWaitKeyOff = 0; /* Line 157, Address: 0x100052c */
  nWaitTime = time; /* Line 158, Address: 0x1000534 */
  OESetSequenceNum(3); /* Line 159, Address: 0x1000540 */
} /* Line 160, Address: 0x100054c */





static void VMDWait(void) { /* Line 166, Address: 0x1000560 */
  if (bWaitKeyOff == 0 && nTimerCunt > nWaitTime) /* Line 167, Address: 0x1000568 */
  {
    OESetSequenceNum(nSaveSequenceNum); /* Line 169, Address: 0x1000594 */
  }
  if (bWaitKeyOff != 0) /* Line 171, Address: 0x10005a4 */
  {
    if (swData1 == 0) bWaitKeyOff = 0; /* Line 173, Address: 0x10005b4 */
  }
} /* Line 175, Address: 0x10005d0 */




void DLLInit(engine_dll* lpDllIn, char*** pBufTbl, void** pFuncTbl) { /* Line 180, Address: 0x10005e0 */

  ++pFuncTbl; /* Line 182, Address: 0x10005f4 */
  ++pFuncTbl; /* Line 183, Address: 0x1000600 */
  ++pFuncTbl; /* Line 184, Address: 0x100060c */
  ++pFuncTbl; /* Line 185, Address: 0x1000618 */
  ++pFuncTbl; /* Line 186, Address: 0x1000624 */
  ++pFuncTbl; /* Line 187, Address: 0x1000630 */
  ++pFuncTbl; /* Line 188, Address: 0x100063c */
  ++pFuncTbl; /* Line 189, Address: 0x1000648 */
  ++pFuncTbl; /* Line 190, Address: 0x1000654 */
  ++pFuncTbl; /* Line 191, Address: 0x1000660 */
  ++pFuncTbl; /* Line 192, Address: 0x100066c */

  sMemAlloc = *pFuncTbl++; /* Line 194, Address: 0x1000678 */
  sMemFree = *pFuncTbl++; /* Line 195, Address: 0x1000690 */
  ++pFuncTbl; /* Line 196, Address: 0x10006a8 */
  sMemCpy = *pFuncTbl++; /* Line 197, Address: 0x10006b4 */
  ++pFuncTbl; /* Line 198, Address: 0x10006cc */
  ++pFuncTbl; /* Line 199, Address: 0x10006d8 */
  ++pFuncTbl; /* Line 200, Address: 0x10006e4 */
  ++pFuncTbl; /* Line 201, Address: 0x10006f0 */
  ++pFuncTbl; /* Line 202, Address: 0x10006fc */
  sPrintf = *pFuncTbl++; /* Line 203, Address: 0x1000708 */
  sOutputDebugString = *pFuncTbl++; /* Line 204, Address: 0x1000720 */
  sOpenFile = *pFuncTbl++; /* Line 205, Address: 0x1000738 */
  sReadFile = *pFuncTbl++; /* Line 206, Address: 0x1000750 */
  sCloseFile = *pFuncTbl++; /* Line 207, Address: 0x1000768 */
  sGetFileSize = *pFuncTbl++; /* Line 208, Address: 0x1000780 */


  get_draw_context_module = *pFuncTbl++; /* Line 211, Address: 0x1000798 */
  FlipToScreen_module = *pFuncTbl++; /* Line 212, Address: 0x10007b0 */
  ld_bitmap_4to8_module = *pFuncTbl++; /* Line 213, Address: 0x10007c8 */
  ld_load_cmpfile_module = *pFuncTbl++; /* Line 214, Address: 0x10007e0 */
  hmx_background_set_background_module = *pFuncTbl++; /* Line 215, Address: 0x10007f8 */
  hmx_bitmap_create_module = *pFuncTbl++; /* Line 216, Address: 0x1000810 */
  hmx_bitmap_release_module = *pFuncTbl++; /* Line 217, Address: 0x1000828 */
  hmx_bitmap_get_scan0_module = *pFuncTbl++; /* Line 218, Address: 0x1000840 */
  hmx_bitmap_set_transparency_module = *pFuncTbl++; /* Line 219, Address: 0x1000858 */
  hmx_free_module = *pFuncTbl++; /* Line 220, Address: 0x1000870 */
  hmx_grid_create_module = *pFuncTbl++; /* Line 221, Address: 0x1000888 */
  hmx_grid_release_module = *pFuncTbl++; /* Line 222, Address: 0x10008a0 */
  hmx_grid_base_module = *pFuncTbl++; /* Line 223, Address: 0x10008b8 */
  hmx_grid_set_position_module = *pFuncTbl++; /* Line 224, Address: 0x10008d0 */
  hmx_grid_set_view_module = *pFuncTbl++; /* Line 225, Address: 0x10008e8 */
  hmx_grid_set_tile_module = *pFuncTbl++; /* Line 226, Address: 0x1000900 */
  hmx_grid_set_horz_offsets_module = *pFuncTbl++; /* Line 227, Address: 0x1000918 */
  hmx_renderer_context_add_module = *pFuncTbl++; /* Line 228, Address: 0x1000930 */
  hmx_renderer_context_clear_module = *pFuncTbl++; /* Line 229, Address: 0x1000948 */
  hmx_renderer_context_draw_module = *pFuncTbl++; /* Line 230, Address: 0x1000960 */
  hmx_sprite_base_module = *pFuncTbl++; /* Line 231, Address: 0x1000978 */
  hmx_sprite_set_position_module = *pFuncTbl++; /* Line 232, Address: 0x1000990 */
  hmx_sprite_set_bitmap_module = *pFuncTbl++; /* Line 233, Address: 0x10009a8 */
  hmx_sprite_set_flags_module = *pFuncTbl++; /* Line 234, Address: 0x10009c0 */

  hmx_ddagrid_create_module = *pFuncTbl++; /* Line 236, Address: 0x10009d8 */
  hmx_ddagrid_release_module = *pFuncTbl++; /* Line 237, Address: 0x10009f0 */
  hmx_ddagrid_set_scan_module = *pFuncTbl++; /* Line 238, Address: 0x1000a08 */
  hmx_ddagrid_set_tile_module = *pFuncTbl++; /* Line 239, Address: 0x1000a20 */

  ++pBufTbl; /* Line 241, Address: 0x1000a38 */
  ++pBufTbl; /* Line 242, Address: 0x1000a44 */
  ++pBufTbl; /* Line 243, Address: 0x1000a50 */
  ++pBufTbl; /* Line 244, Address: 0x1000a5c */
  ++pBufTbl; /* Line 245, Address: 0x1000a68 */
  ++pBufTbl; /* Line 246, Address: 0x1000a74 */
  ++pBufTbl; /* Line 247, Address: 0x1000a80 */
  ++pBufTbl; /* Line 248, Address: 0x1000a8c */
  ++pBufTbl; /* Line 249, Address: 0x1000a98 */
  ++pBufTbl; /* Line 250, Address: 0x1000aa4 */
  ++pBufTbl; /* Line 251, Address: 0x1000ab0 */

  g_env_module = (hmx_environment*)**pBufTbl++; /* Line 253, Address: 0x1000abc */
  g_loader_module = (hmx_environment*)**pBufTbl++; /* Line 254, Address: 0x1000ad8 */






  lpColorwk = lpDllIn->lpColorwk; /* Line 261, Address: 0x1000af4 */
  lpbVisualmode = lpDllIn->lpbVisualmode; /* Line 262, Address: 0x1000b04 */

  CDPlay = lpDllIn->CDPlay; /* Line 264, Address: 0x1000b14 */
  CDPause = lpDllIn->CDPause; /* Line 265, Address: 0x1000b24 */


  OEClrset(tblPal1, 0); /* Line 268, Address: 0x1000b34 */
  OEClrset(tblPal2, 1); /* Line 269, Address: 0x1000b48 */
  OEClrset(tblPal3, 2); /* Line 270, Address: 0x1000b5c */
  OEClrset(tblPal4, 3); /* Line 271, Address: 0x1000b70 */

  s_ctx = get_draw_context_module(); /* Line 273, Address: 0x1000b84 */

  OECreateEA(); /* Line 275, Address: 0x1000b9c */
  nSequenceNum = 1; /* Line 276, Address: 0x1000ba4 */
  nTimerCunt = 0; /* Line 277, Address: 0x1000bb0 */
} /* Line 278, Address: 0x1000bb8 */




void DLLEnd(void) { /* Line 283, Address: 0x1000bd0 */
  OEDeleteEA(); /* Line 284, Address: 0x1000bd8 */
  DeleteAllCharSprt(); /* Line 285, Address: 0x1000be0 */


} /* Line 288, Address: 0x1000be8 */




void SWdataSet(ushort_union sw1, ushort_union sw2) { /* Line 293, Address: 0x1000c00 */
  swData1 = sw1.w; /* Line 294, Address: 0x1000c0c */
  swData2 = sw2.w; /* Line 295, Address: 0x1000c18 */





} /* Line 301, Address: 0x1000c24 */








int DLLMain(void) { /* Line 310, Address: 0x1000c30 */
  int ret = 0; /* Line 311, Address: 0x1000c3c */

  switch (nSequenceNum) /* Line 313, Address: 0x1000c40 */
  {
    case 1:
      VMDStart(); /* Line 316, Address: 0x1000c84 */
      CDPlay(26); /* Line 317, Address: 0x1000c8c */
      break; /* Line 318, Address: 0x1000ca0 */

    case 2:
      VMDMenu(); /* Line 321, Address: 0x1000ca8 */
      break; /* Line 322, Address: 0x1000cb0 */

    case 3:
      VMDWait(); /* Line 325, Address: 0x1000cb8 */
      break; /* Line 326, Address: 0x1000cc0 */

    case 999:
      CDPause(2); /* Line 329, Address: 0x1000cc8 */
      ret = VMDEnd(); /* Line 330, Address: 0x1000cdc */
      break;
  }

  if (nTimerCunt == 0x7FFFFFFF) /* Line 334, Address: 0x1000ce8 */
    nTimerCunt = 5184000; /* Line 335, Address: 0x1000d00 */
  else
    ++nTimerCunt; /* Line 337, Address: 0x1000d18 */

  VMDDraw(); /* Line 339, Address: 0x1000d2c */


  return ret; /* Line 342, Address: 0x1000d34 */
} /* Line 343, Address: 0x1000d38 */

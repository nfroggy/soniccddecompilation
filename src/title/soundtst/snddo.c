#include "../../types.h"
#include "../common/hmx_types.h"
#include "snddo.h"
#include "../common/grid.h"
#include "../common/hmx_oeeactl.h"
#include "../common/palt.h"
#include "../../impfuncs.h"
#include "sndentry.h"
#include "sndsprt.h"

static void UpdateMenu(void);

static Uint8 tblPCM[80] = {
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
  10, 11, 11, 12, 13, 14, 15, 16, 17, 18,
  19, 20, 20, 21, 22, 22, 23, 23, 24, 25,
  26, 27, 28, 29, 30, 31, 43, 44, 45, 46,
  47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
  57, 58, 58, 59, 60, 61, 62, 63, 64, 65,
  67, 68, 69, 70, 71, 71, 72, 73, 74, 32,
  33, 34, 35, 36, 37, 38, 39, 40, 41, 42
};
static Uint8 tblDA[34] = {
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  30, 31, 32, 33
};
extern Uint16 nSequenceNum;
static Uint16 nMenuKind;
static Uint16 nPCMNumberIndx;
static Uint16 nDANumberIndx;
static char ErrBuf[250];
static Sint32 ErrRet;
static Uint32 hmapwk;
static Uint32 hBkg;
static Uint32* hBmpBkg;
static Uint32 HorizInit;
Sint32 bKakusi;
extern Uint16 swData1;
extern Sint32 nTimerCunt;
PALETTEENTRY MIYAKEcolor[16] = {
  { 240, 240, 240, 1 }, {   0,   0,   0, 1 }, {   0,   0,  64, 1 }, {   0,   0, 128, 1 },
  {   0,   0, 192, 1 }, {  32,   0, 224, 1 }, {  32,  32, 224, 1 }, {  64, 192, 224, 1 },
  {   0,  64, 224, 1 }, {   0,  32, 224, 1 }, {   0,  32, 192, 1 }, {   0,  32, 160, 1 },
  {   0,  32, 128, 1 }, {   0,  32,  96, 1 }, {   0,  32,  64, 1 }, {   0,  32,  32, 1 }
};
PALETTEENTRY NISIMURAcolor[16] = {
  {   0,   0,   0, 1 }, { 224, 224, 224, 1 }, {  96,  96,  96, 1 }, { 160, 160, 160, 1 },
  {  32,  32,  32, 1 }, { 240, 240, 240, 1 }, { 240, 240, 240, 1 }, { 240, 240, 240, 1 },
  { 240, 240, 240, 1 }, {  96,  64, 160, 1 }, { 240, 240, 240, 1 }, {   0,   0, 128, 1 },
  {   0,   0,  32, 1 }, {  64,  32, 128, 1 }, { 240, 240, 240, 1 }, { 240, 240, 240, 1 }
};
PALETTEENTRY SANPEIcolor[16] = {
  {   0,   0,   0, 1 }, {  32,  32, 128, 1 }, {  64,  64, 192, 1 }, {  64,  64, 224, 1 },
  { 240, 240, 240, 1 }, { 224, 224, 192, 1 }, { 224, 224, 224, 1 }, { 160, 160, 160, 1 },
  { 128, 128, 128, 1 }, {  64,  64,  64, 1 }, { 224, 160, 128, 1 }, { 160,  96,  64, 1 },
  { 224,   0,   0, 1 }, { 128,   0,   0, 1 }, {  64,   0,   0, 1 }, { 224, 192, 160, 1 }
};
PALETTEENTRY YAMAGU1color[16] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, { 224, 224, 224, 1 }, {   0,  32,   0, 1 },
  {  32,  64,   0, 1 }, {  96, 128,  32, 1 }, {  32,  32,  32, 1 }, {  64,  64,  64, 1 },
  {  96,  96, 128, 1 }, { 128, 128, 160, 1 }, { 160, 160, 160, 1 }, { 192, 192, 192, 1 },
  { 224, 224, 128, 1 }, {  64, 128, 224, 1 }, { 128, 128,  96, 1 }, {   0,  32, 208, 1 }
};
PALETTEENTRY YAMAGU2color[16] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  64,  64, 128, 1 }, {  96,  96, 160, 1 },
  { 128, 160, 224, 1 }, { 160, 192, 224, 1 }, { 224, 224, 224, 1 }, { 224, 160,   0, 1 },
  { 224, 128,   0, 1 }, { 192,  96,   0, 1 }, { 224, 192, 128, 1 }, { 192, 224, 224, 1 },
  { 160,  64,   0, 1 }, {  96,   0,   0, 1 }, { 192,  32,   0, 1 }, {   0,  32, 192, 1 }
};
PALETTEENTRY HOSHINOcolor[16] = {
  {   0,   0,   0, 1 }, {   0,  96,   0, 1 }, {  32,  32, 192, 1 }, {  64,  64, 224, 1 },
  {  96,  96, 224, 1 }, { 128, 128, 224, 1 }, { 224, 224, 224, 1 }, { 160, 160, 160, 1 },
  { 128, 128, 128, 1 }, {  64,  64,  64, 1 }, { 224, 160, 128, 1 }, { 160,  96,  64, 1 },
  { 224,   0,   0, 1 }, { 128,   0,   0, 1 }, {  64,   0,   0, 1 }, { 224, 224,   0, 1 }
};
static grid_rect CG_Rect = { 0, 0, 320, 224 };
extern draw_context* s_ctx;
extern void(*hmx_sprite_set_bitmap_module)(hmx_sprite*, hmx_bitmap*);
extern void(*hmx_sprite_set_position_module)(hmx_sprite*, Sint32, Sint32);
extern void*(*hmx_bitmap_get_scan0_module)(hmx_bitmap*);
extern hmx_bitmap*(*hmx_bitmap_create_module)(hmx_environment*, Sint32, Sint32);
extern hmx_environment* g_env_module;
extern void(*hmx_bitmap_release_module)(hmx_environment*, hmx_bitmap*);

void SNDDraw(void) {
  if (nSequenceNum > 1) {

    srfDraw();
  }
}

static void UpdateMenu(void) {
  if (nMenuKind == 1) StrPCM(2);
  else StrPCM(1);
  if (nMenuKind == 2) StrDA(2);
  else StrDA(1);
}

void SNDStart(void) {
  Sint32 n;

  OEGridCreate(0);
  for (n = 0; n < 16; ++n) {

    OESprCreateIndx(n + 83);
  }
  for (n = 0; n < 9; ++n) {

    OESprCreateIndx(n);
  }
  nMenuKind = 1;
  UpdateMenu();
  StrPCMNO(tblPCM[0]);
  StrDANO(tblDA[0]);
  nPCMNumberIndx = nDANumberIndx = 0;
  OESetSequenceNum(2);
}

void SNDMenu(void) {
  Uint16 nMenuKindOld;

  if (swData1 & 2048) {

    nMenuKindOld = nMenuKind;
    nMenuKind = nMenuKind + 1 < 2 ? nMenuKind + 1 : 2;
    if (nMenuKind == nMenuKindOld) return;

    UpdateMenu();
    KeyWait();

  }
  else if (swData1 & 1024) {

    nMenuKindOld = nMenuKind;
    nMenuKind = nMenuKind - 1 > 1 ? nMenuKind - 1 : 1;
    if (nMenuKind == nMenuKindOld) return;

    UpdateMenu();
    KeyWait();

  }
  else if (swData1 & 256) {

    if (nMenuKind == 1) {

      if (nPCMNumberIndx == 0) nPCMNumberIndx = 79;
      else --nPCMNumberIndx;

      StrPCMNO(nPCMNumberIndx);
    }
    else {

      if (nDANumberIndx == 0) nDANumberIndx = 33;
      else --nDANumberIndx;
      StrDANO(tblDA[nDANumberIndx]);
    }
    KeyWait();
  }
  else if (swData1 & 512) {

    if (nMenuKind == 1) {

      if (nPCMNumberIndx == 79) nPCMNumberIndx = 0;
      else ++nPCMNumberIndx;

      StrPCMNO(nPCMNumberIndx);
    }
    else {

      if (nDANumberIndx == 33) nDANumberIndx = 0;
      else ++nDANumberIndx;
      StrDANO(tblDA[nDANumberIndx]);
    }
    KeyWait();
  }

  else if (swData1 & 16384) {

if (nPCMNumberIndx == 3 && nDANumberIndx == 1) {
      OEDeleteEA();
      DeleteAllCharSprt();
      MC_SONICCreate(0);
      bKakusi = 1;
      CDPause(2);
      CDPlay(21);
      OESetSequenceNum(4);
      return;
    }
    if (nPCMNumberIndx == 12 && nDANumberIndx == 25) {
      OEDeleteEA();
      DeleteAllCharSprt();
      MC_SONICCreate(1);
      bKakusi = 1;
      CDPause(2);
      CDPlay(24);
      OESetSequenceNum(4);
      return;
    }
    if (nPCMNumberIndx == 4 && nDANumberIndx == 21) {
      OEDeleteEA();
      DeleteAllCharSprt();
      MC_SONICCreate(2);
      bKakusi = 1;
      CDPause(2);
      CDPlay(25);
      OESetSequenceNum(4);
      return;
    }
    if (nPCMNumberIndx == 11 && nDANumberIndx == 9) {
      OEDeleteEA();
      DeleteAllCharSprt();
      MC_SONICCreate(3);
      bKakusi = 1;
      CDPause(2);
      CDPlay(4);
      OESetSequenceNum(4);
      return;
    }

    if (nPCMNumberIndx == 7 && nDANumberIndx == 7) {
      CDPause(2);
      StrWelcome();
      OESetSequenceNum(5);
      return;
    }

    if (nPCMNumberIndx == 12 && nDANumberIndx == 11) {
      CDPause(2);
      CDPlay(33);
      OEDeleteEA();
      DeleteAllCharSprt();
      MC_SONICCreate(4);
      bKakusi = 2;
      OESetSequenceNum(4);
      return;
    }

    if (nMenuKind == 1) {

WaveRequest(tblPCM[nPCMNumberIndx]);
    }
    else {

      CDPause(2);
      CDPlay(tblDA[nDANumberIndx]);
    }
    WaitKeyOff();
  }

  else if (swData1 & 4096) {

    CDPause(2);
    OESetSequenceNum(999);
  }
}

Sint32 SNDEnd(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0) {

    ret = 9;
  }
  return ret;
}

Sint32 SNDEnd2(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0) {

    ret = 18;
  }
  return ret;
}

Sint32 SNDEnd3(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0) {

    ret = 19;
  }
  return ret;
}

Sint32 MC_SONICCreate(Sint16 cgno) {
  Uint32 mask[4] = { 0, 0, 0, 0 };
  RECT rect = { 0, 0, 320, 224 };
  RECT rect2 = { 0, 0, 160, 224 };
  RECT rect3 = { 0, 0, 160, 224 };

  POINT pt = { 0, 0 };
  POINT pt2 = { 160, 0 };
  hmx_bitmap* bmp;

switch (cgno) {

    case 0:
      OEClrset(HOSHINOcolor, 0);
      break;
    case 1:
      OEClrset(NISIMURAcolor, 0);
      break;
    case 2:
      OEClrset(MIYAKEcolor, 0);
      break;
    case 3:
      OEClrset(SANPEIcolor, 0);
      break;
    case 4:
      OEClrset(YAMAGU1color, 0);
      OEClrset(YAMAGU2color, 1);
      break;
  }

  bmp = hmx_bitmap_create_module(g_env_module, 320, 224);

  switch (cgno) {

case 0:
      ld_bitmap_file("HOSHINO3.BMP", hmx_bitmap_get_scan0_module(bmp), rect.right, rect.bottom, 0, 0);
      break;

    case 1:
      ld_bitmap_file("NISIMUR3.BMP", hmx_bitmap_get_scan0_module(bmp), rect.right, rect.bottom, 0, 0);
      break;

    case 2:
      ld_bitmap_file("MIYAKE3.BMP", hmx_bitmap_get_scan0_module(bmp), rect.right, rect.bottom, 0, 0);
      break;

    case 3:
      ld_bitmap_file("SANPEI3.BMP", hmx_bitmap_get_scan0_module(bmp), rect.right, rect.bottom, 0, 0);
      break;

case 4:
      ld_bitmap_file2("YAMA_L3.BMP", hmx_bitmap_get_scan0_module(bmp), 320, 224, rect2.right, rect2.bottom, 0, 0, 0, 0);
      ld_bitmap_file2("YAMA_R3.BMP", hmx_bitmap_get_scan0_module(bmp), 320, 224, rect3.right, rect3.bottom, 160, 0, 0, 0);
      break;
  }

s_ctx->spr_bitmaps[0] = bmp;
  hmx_sprite_set_position_module(s_ctx->sprites[0], 0, 0);
  hmx_sprite_set_bitmap_module(s_ctx->sprites[0], s_ctx->spr_bitmaps[0]);
  s_ctx->spr_level[0] = 1;

  return 0;
}

Sint32 MC_SONICDelete(void) {
  hmx_bitmap_release_module(g_env_module, s_ctx->spr_bitmaps[0]);
  s_ctx->spr_bitmaps[0] = 0;
  s_ctx->spr_level[0] = 0;

return 0;
}

void SNDKakusi(void) {
  if (!(swData1 & 16384)) bKakusi |= 4;
  if (bKakusi & 4 && (swData1 & 16384 || swData1 & 4096)) {
    if ((bKakusi & 3) == 2) OESetSequenceNum(7);
    if ((bKakusi & 3) == 1) OESetSequenceNum(999);
  }

}

void SNDSpecialMes(void) {
  if (!(swData1 & 16384)) bKakusi |= 4;
  if (bKakusi & 4 && (swData1 & 16384 || swData1 & 4096)) {
    DeleteCharMes();
    bKakusi = 0;
    OESetSequenceNum(6);
  }

}

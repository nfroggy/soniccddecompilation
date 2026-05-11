#include "../../types.h"
#include "../common/score_data_types.h"
#include "../common/hmx_types.h"
#include "svddo.h"
#include "../common/hmx_oeeactl.h"
#include "svdedit.h"
#include "svdentry.h"
#include "svdfile.h"
#include "svdmenu.h"
#include "svdsprt.h"

static void UpdateMenu(Uint16 nMenuKind, Uint16 nMenuKindOld);

extern Uint16 nSequenceNum;
extern sprite_bmp infoSprtBmp[];
Uint16 nMenuKind;
Uint32 bMenuChosed;
extern void(*hmx_sprite_set_position_module)(hmx_sprite*, Sint32, Sint32);
extern draw_context* s_ctx;
extern score_data saveData[6];
extern Sint32 nFileNumber;
extern Sint32 nSelectFileNumber;
extern void(*WaveRequest)(Sint16);
extern Uint32 bDestFileChosed;
extern Uint32 bFileChosed;
extern Uint16 swData1;
extern Sint32 nTimerCunt;
extern Sint32 keybordInputTime;
extern Uint16 swData2;

void SDDraw(void) {
  if (nSequenceNum > 1) {

    srfDraw();
  }
}

void SDStart(void) {
  Sint32 n;
  POINT point;

for (n = 0; n < 4; ++n) {

    OESprCreateIndx(n + 199);
  }
  ReadDataFile();
  DispAll();
  nMenuKind = 1;
  OESetSequenceNum(2);
  point.x = infoSprtBmp[0].point.x + infoSprtBmp[0].w;
  point.y = infoSprtBmp[0].point.y - 8;
  DispSonicCursol(point);
}

static void UpdateMenu(Uint16 nMenuKind, Uint16 nMenuKindOld) {
  if (nMenuKind == nMenuKindOld) return;

  OESprDeleteIndx((nMenuKindOld - 1) * 2 + 1);
  OESprCreateIndx((nMenuKindOld - 1) * 2);
  OESprDeleteIndx((nMenuKind - 1) * 2);
  OESprCreateIndx((nMenuKind - 1) * 2 + 1);
}

void DeleteFileSelect(void) {

  OESprDeleteIndx(192);
}

void UpdateFileSelect(Uint16 nNewNumber) {
  POINT point;

  DeleteFileSelect();

OESprCreateIndx(192);
  point.x = infoSprtBmp[192].point.x;
  point.y = infoSprtBmp[192].point.y + nNewNumber * 32;

  hmx_sprite_set_position_module(s_ctx->sprites[*infoSprtBmp[192].lphSpr], point.x, point.y);
}

void UpdateSelectSonicMark(Uint16 nNewNumber) {
  POINT point;

  if (saveData[nNewNumber].saved == 0) {
    if (*infoSprtBmp[10].lphSpr != 0) {
      OESprDeleteIndx(10);
    }
    return;
  }
  if (*infoSprtBmp[10].lphSpr == 0)
    OESprCreateIndx(10);
  point.x = infoSprtBmp[10].point.x;
  point.y = infoSprtBmp[10].point.y + nNewNumber * 32;

  hmx_sprite_set_position_module(s_ctx->sprites[*infoSprtBmp[10].lphSpr], point.x, point.y);
}

void UpdateFileView(Uint16 nFileNumber) {
  DeleteStringSprt(56, nFileNumber + 2, 0);
  DispSaveData(nFileNumber);
}

void UpdateMessage(char* msg) {
  DeleteStringSprt(27, 1, 0);
  CreateStringSprt(msg, 2, 1, 0);
}

void DefaultMessage(void) {
  UpdateMessage("SONIC CD DATA-FILES");
}

Uint32 ChkSaveFile(void) {
  Sint32 i;
  for (i = 0; i < 6; ++i) {
    if (saveData[i].saved != 0)
      return 1;
  }
  return 0;
}

void SDMenu(void) {
  Uint16 nMenuKindOld;
  POINT point;

  if (bMenuChosed == 0) {

    if (swData1 & 512) {

      nMenuKindOld = nMenuKind;
      nMenuKind = nMenuKind + 1 < 5 ? nMenuKind + 1 : 5;
      if (nMenuKind == nMenuKindOld) return;

      WaveRequest(9);
      UpdateMenu(nMenuKind, nMenuKindOld);
      point.x = infoSprtBmp[(nMenuKind - 1) * 2].point.x
              + infoSprtBmp[(nMenuKind - 1) * 2].w;
      point.y = infoSprtBmp[(nMenuKind - 1) * 2].point.y - 8;
      RunSonicCursol(point);
      KeyWait();

    }
    else if (swData1 & 256) {

      nMenuKindOld = nMenuKind;
      nMenuKind = nMenuKind - 1 > 1 ? nMenuKind - 1 : 1;
      if (nMenuKind == nMenuKindOld) return;

      WaveRequest(9);
      UpdateMenu(nMenuKind, nMenuKindOld);
      point.x = infoSprtBmp[(nMenuKind - 1) * 2].point.x
              + infoSprtBmp[(nMenuKind - 1) * 2].w;
      point.y = infoSprtBmp[(nMenuKind - 1) * 2].point.y - 8;
      RunSonicCursol(point);
      KeyWait();

    }
    else if (swData1 & 16384) {

if (ChkSaveFile() == 0) {
        if (nMenuKind != 5) {

          WaveRequest(30);
          KeyWait();
          return;
        }
      }
      bMenuChosed = 1;
      bFileChosed = 0;
      bDestFileChosed = 0;
      nFileNumber = nSelectFileNumber;
      switch (nMenuKind) {

        case 1:
          WaveRequest(26);
          UpdateMessage("SELECT FILE TO USE IN GAME.");
          UpdateFileSelect(nSelectFileNumber);
          point.x = infoSprtBmp[nFileNumber + 193].point.x - 16;
          point.y = infoSprtBmp[nFileNumber + 193].point.y;
          RunSonicCursol(point);
          break;
        case 2:
          WaveRequest(26);
          UpdateMessage("SELECT FILE TO COPY FROM.");
          UpdateFileSelect(nSelectFileNumber);
          point.x = infoSprtBmp[nFileNumber + 193].point.x - 16;
          point.y = infoSprtBmp[nFileNumber + 193].point.y;
          RunSonicCursol(point);
          break;
        case 3:
          WaveRequest(26);
          UpdateMessage("SELECT FILE TO RENAME.");
          UpdateFileSelect(nSelectFileNumber);
          point.x = infoSprtBmp[nFileNumber + 193].point.x - 16;
          point.y = infoSprtBmp[nFileNumber + 193].point.y;
          RunSonicCursol(point);
          break;
        case 4:
          WaveRequest(26);
          UpdateMessage("SELECT FILE TO DELETE.");
          UpdateFileSelect(nSelectFileNumber);
          point.x = infoSprtBmp[nFileNumber + 193].point.x - 16;
          point.y = infoSprtBmp[nFileNumber + 193].point.y;
          RunSonicCursol(point);
          break;
        case 5:
          break;
      }
      WaitKeyOff();
    }
  }
  else {

    switch (nMenuKind) {

      case 1:
        SDMenuSelect();
        break;
      case 2:
        SDMenuCopy();
        break;
      case 3:
        SDMenuRename();
        break;
      case 4:
        SDMenuDelete();
        break;
      case 5:
        SDMenuExit();
        break;
    }
  }

}

Sint32 SDEnd(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0) {

    WriteDataFile();
    ret = 9;
  }

  return ret;
}

Sint32 CheckAutoEnd(void) {
  static Sint32 inputKeyTime;
  Sint32 ret = 0;

  if (swData1 || swData2)
    inputKeyTime = nTimerCunt;
  if (nTimerCunt - keybordInputTime > 36000
      && nTimerCunt -inputKeyTime > 36000) {

ret = 4;
  }
  return ret;
}

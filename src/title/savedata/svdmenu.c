#include "../../types.h"
#include "../common/score_data_types.h"
#include "../common/hmx_types.h"
#include "svdmenu.h"
#include "../../impfuncs.h"
#include "svddo.h"
#include "svdedit.h"
#include "svdentry.h"
#include "svdsprt.h"

extern sprite_bmp infoSprtBmp[];
extern Uint16 nMenuKind;
extern Uint32 bMenuChosed;
extern Uint16 swData1;
Uint32 bFileChosed;
Uint32 bDestFileChosed;
Sint32 nFileNumber;
Sint32 nSelectFileNumber;
score_data saveData[6];
static Sint32 nSrcFileNumber;
extern char editBuf[];
extern Sint32 nTimerCunt;

Uint32 SDFileSelect(Sint32* pFileNumber) {
  Sint32 oldNumber;
  POINT point;

  if (swData1 & 512) {

    oldNumber = *pFileNumber;
    *pFileNumber = *pFileNumber + 1 < 5 ? *pFileNumber + 1 : 5;
    if (oldNumber != *pFileNumber) {

      WaveRequest(9);
      UpdateFileSelect(*pFileNumber);
      point.x = infoSprtBmp[*pFileNumber + 193].point.x - 16;
      point.y = infoSprtBmp[*pFileNumber + 193].point.y;
      RunSonicCursol(point);
      KeyWait();
    }
  }
  else if (swData1 & 256) {

    oldNumber = *pFileNumber;
    *pFileNumber = *pFileNumber - 1 > 0 ? *pFileNumber - 1 : 0;
    if (oldNumber != *pFileNumber) {

      WaveRequest(9);
      UpdateFileSelect(*pFileNumber);
      point.x = infoSprtBmp[*pFileNumber + 193].point.x - 16;
      point.y = infoSprtBmp[*pFileNumber + 193].point.y;
      RunSonicCursol(point);
      KeyWait();
    }
  }
  else if (swData1 & 16384) {

    return 1;
  }
  else if (swData1 & 4096) {

    WaveRequest(25);
    bMenuChosed = 0;
    DeleteFileSelect();
    DefaultMessage();
    point.x = infoSprtBmp[(nMenuKind - 1) * 2].point.x
            + infoSprtBmp[(nMenuKind - 1) * 2].w;
    point.y = infoSprtBmp[(nMenuKind - 1) * 2].point.y - 8;
    RunSonicCursol(point);
  }
  return 0;
}

void SDMenuSelect(void) {
  POINT point;
  static Uint32 yesNoDialogOn;

  if (bFileChosed == 0) {

    bFileChosed = SDFileSelect(&nFileNumber);
    yesNoDialogOn = 0;
    if (bFileChosed == 0) return;

if (saveData[nFileNumber].saved != 0) {
      UpdateMessage("CONTINUE WITH THIS FILE?");
      yesNoDialogOn = 1;
      WaitKeyOff();
      WaveRequest(26);
      return;
    }
    bFileChosed = 0;
    WaitKeyOff();
    WaveRequest(30);

}
  else if (yesNoDialogOn != 0) {

    if (swData1 & 16384) {

      WaveRequest(5);
      nSelectFileNumber = nFileNumber;
      bMenuChosed = bFileChosed = 0;
      DefaultMessage();
      UpdateSelectSonicMark(nSelectFileNumber);
      DeleteFileSelect();
      point.x = infoSprtBmp[0].point.x
              + infoSprtBmp[0].w;
      point.y = infoSprtBmp[0].point.y - 8;
      RunSonicCursol(point);
      WaitKeyOff();
    }
    else if (swData1 & 4096) {

      WaveRequest(25);
      bFileChosed = 0;
      UpdateMessage("SELECT FILE TO USE IN GAME.");
      WaitKeyOff();
    }
  }
}

void SDMenuCopy(void) {
  POINT point;
  static Uint32 yesNoDialogOn;

  if (bFileChosed == 0) {

    bFileChosed = SDFileSelect(&nFileNumber);
    bDestFileChosed = 0;
    yesNoDialogOn = 0;
    if (bFileChosed == 0) return;

    if (saveData[nFileNumber].saved != 0) {

      WaveRequest(26);
      UpdateMessage("SELECT FILE TO COPY TO.");
      nSrcFileNumber = nFileNumber;
    }
    else {
      WaveRequest(30);
      bFileChosed = 0;
    }
    WaitKeyOff();

  }
  else if (bDestFileChosed == 0) {

    bDestFileChosed = SDFileSelect(&nFileNumber);
    if (bDestFileChosed == 0) return;

    if (nFileNumber == nSrcFileNumber) {

      WaveRequest(30);
      bDestFileChosed = 0;
    }
    else {

      WaveRequest(26);
      if (saveData[nFileNumber].saved != 0)
        UpdateMessage("OK TO OVERWRITE FILE?");
      else
        UpdateMessage("OK TO COPY?");
      yesNoDialogOn = 1;
      WaitKeyOff();
    }
  }
  else if (yesNoDialogOn != 0) {

    if (swData1 & 16384) {

      WaveRequest(5);
      sMemCpy(&saveData[nFileNumber], &saveData[nSrcFileNumber], sizeof(saveData[nSrcFileNumber]));

      bMenuChosed = 0;
      UpdateFileView(nFileNumber);
      UpdateMessage("COPY COMPLETED.");
      DeleteFileSelect();
      point.x = infoSprtBmp[2].point.x
              + infoSprtBmp[2].w;
      point.y = infoSprtBmp[2].point.y - 8;
      RunSonicCursol(point);
      WaitAndDfltMsg(60);
    }
    else if (swData1 & 4096) {

      WaveRequest(25);
      bFileChosed = 0;
      UpdateMessage("SELECT FILE TO COPY FROM.");
      WaitKeyOff();
    }
  }
}

void SDMenuRename(void) {
  Sint32 ret;
  POINT point;
  static Uint32 editMode;
  static Uint32 cursolOn;

  if (bFileChosed == 0) {

    bFileChosed = SDFileSelect(&nFileNumber);
    editMode = 0;
    if (bFileChosed == 0) return;

    if (saveData[nFileNumber].saved != 0) {

      WaveRequest(26);
      editMode = 1;

      sMemCpy(editBuf, saveData[nFileNumber].player, sizeof(((score_data*)0)->player));
      DispCursol();
      MovCursol(0);
    }
    else {
      WaveRequest(30);
      bFileChosed = 0;
    }
    WaitKeyOff();

  }
  else if (editMode != 0) {

    if (nTimerCunt % 10 == 0) {

      cursolOn = cursolOn == 0;
      if (cursolOn != 0) DispCursol();
      else HideCursol();
    }
    ret = Edit();

    if (ret == 1 || ret == -1) {

      WaveRequest(5);
      sMemCpy(saveData[nFileNumber].player, editBuf, sizeof(((score_data*)0)->player));
      bMenuChosed = bFileChosed = editMode = 0;
      DefaultMessage();
      DeleteFileSelect();
      HideCursol();
      point.x = infoSprtBmp[4].point.x
              + infoSprtBmp[4].w;
      point.y = infoSprtBmp[4].point.y - 8;
      RunSonicCursol(point);
    }
  }

}

void SDMenuDelete(void) {
  POINT point;
  Sint32 i;
  static Uint32 yesNoDialogOn;

  if (bFileChosed == 0) {

    bFileChosed = SDFileSelect(&nFileNumber);
    yesNoDialogOn = 0;
    if (bFileChosed == 0) return;

    if (saveData[nFileNumber].saved != 0) {

      WaveRequest(26);
      UpdateMessage("OK TO DELETE?");
      yesNoDialogOn = 1;
    }
    else {
      WaveRequest(30);
      bFileChosed = 0;
    }
    WaitKeyOff();

  }
  else if (yesNoDialogOn != 0) {

    if (swData1 & 16384) {

      WaveRequest(5);
      sMemSet(&saveData[nFileNumber], 0, sizeof(saveData[nFileNumber]));
      bMenuChosed = 0;

      if (nSelectFileNumber == nFileNumber) {

        for (i = 0; i < 6; ++i) {
          if (saveData[i].saved != 0) break;

        }
        if (i == 6) {

          nSelectFileNumber = 0;
        }
        else {
          nSelectFileNumber = i;
        }
        UpdateSelectSonicMark(nSelectFileNumber);
      }

UpdateFileView(nFileNumber);

      UpdateMessage("FILE DELETED.");
      DeleteFileSelect();
      point.x = infoSprtBmp[6].point.x
              + infoSprtBmp[6].w;
      point.y = infoSprtBmp[6].point.y - 8;
      RunSonicCursol(point);
      WaitAndDfltMsg(60);
    }
    else if (swData1 & 4096) {

      WaveRequest(25);
      bFileChosed = 0;
      UpdateMessage("SELECT FILE TO DELETE.");
      WaitKeyOff();
    }
  }
}

void SDMenuExit(void) {
  OESetSequenceNum(999);
}

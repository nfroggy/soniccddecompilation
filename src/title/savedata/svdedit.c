#include "../../types.h"
#include "../common/score_data_types.h"
#include "svdedit.h"
#include "../common/hmx_oeeactl.h"
#include "svddo.h"
#include "svdentry.h"
#include "svdsprt.h"

extern Uint16 swData1;
extern Sint32 nFileNumber;
char editBuf[12];
static Uint32 bEditMode;
extern void(*WaveRequest)(Sint16);
extern void(*sPrintf)(char*, const char*, ...);
extern score_data saveData[6];
extern Sint32 nSelectFileNumber;

Sint32 Edit(void) {
  Sint32 posi;

  bEditMode = 1;
  if (swData1 & 1024)
  {
    WaveRequest(54);
    posi = GetCursolPositon();
    posi = posi - 1 > 0 ? posi - 1 : 0;
    DispCursol();
    MovCursol(posi);
    Wait(2);
  }
  else if (swData1 & 2048)
  {
    WaveRequest(54);
    posi = GetCursolPositon();
    posi = posi + 1 < 9 ? posi + 1 : 9;
    DispCursol();
    MovCursol(posi);
    Wait(2);
  }
  else if (swData1 & 512)
  {
    WaveRequest(54);
    posi = GetCursolPositon();
    if (editBuf[posi] == 32) editBuf[posi] = 90;
    else if (editBuf[posi] == 45) editBuf[posi] = 32;
    else if (editBuf[posi] == 48) editBuf[posi] = 45;
    else if (editBuf[posi] == 65) editBuf[posi] = 57;
    else --editBuf[posi];
    DeleteCharSprt(nFileNumber + 2, posi);
    CreateCharSprt(editBuf[posi], 0, nFileNumber + 2, posi);
    Wait(2);
  }
  else if (swData1 & 256)
  {
    WaveRequest(54);
    posi = GetCursolPositon();
    if (editBuf[posi] == 32) editBuf[posi] = 45;
    else if (editBuf[posi] == 45) editBuf[posi] = 48;
    else if (editBuf[posi] == 57) editBuf[posi] = 65;
    else if (editBuf[posi] == 90) editBuf[posi] = 32;
    else ++editBuf[posi];
    DeleteCharSprt(nFileNumber + 2, posi);
    CreateCharSprt(editBuf[posi], 0, nFileNumber + 2, posi);
    Wait(2);
  }
  else if (swData1 & 16384)
  {
    bEditMode = 0;
    WaitKeyOff();
    return 1;
  }
  else if (swData1 & 4096)
  {
    bEditMode = 0;
    WaitKeyOff();
    return -1;
  }
  return 0;
}

void DispSaveData(Sint32 indx) {
  char str[256];
  Sint32 round;

if (saveData[indx].saved != 0)
  {
    CreateStringSprt(saveData[indx].player, 0, indx + 2, 0);

    sPrintf(str, "%4u/%2u/%2u", saveData[indx].year, saveData[indx].month, saveData[indx].date);

    CreateStringSprt(str, 0, indx + 2, 19);

    round = saveData[indx].roundNo + 1;
    if (round > 7) round = 7;
    sPrintf(str, "ROUND-%1d", round);
    CreateStringSprt(str, 2, indx + 2, 12);

sPrintf(str, "%02u\"%02u\"%02u", saveData[indx].hour, saveData[indx].min, saveData[indx].sec);

CreateStringSprt(str, 0, indx + 2, 30);

    OESprCreateIndx(indx + 193);
  }
  else
  {

    CreateStringSprt("NO DATA-FILE", 0, indx + 2, 44);
    OESprDeleteIndx(indx + 193);
  }
}

void DispAll(void) {
  Sint32 i;

  DefaultMessage();
  OESprCreateIndx(1);
  OESprCreateIndx(2);
  OESprCreateIndx(4);
  OESprCreateIndx(6);
  OESprCreateIndx(8);
  for (i = 0; i < 6; ++i)
  {
    DispSaveData(i);
  }
  UpdateSelectSonicMark(nSelectFileNumber);

}

#include "../../types.h"
#include "stgitem.h"
#include "stgdo.h"
#include "../common/grid.h"
#include "../common/hmx_oeeactl.h"
#include "stgentry.h"
#include "stgsprt.h"

static void UpdateMenu(void);

extern Uint16 nSequenceNum;
extern stgitem menuTbl[];
static Sint32 nMenuIndx;
extern Uint16 swData1;
extern Sint32 MENUITEMMAX;
extern Uint16* lpSelectedStage;
extern Sint32 nTimerCunt;

void STGDraw(void) {
  if (nSequenceNum > 1)
  {
    srfDraw();
  }
}

static void UpdateMenu(void) {
  StrMenu(menuTbl[nMenuIndx].str);
}

void STGStart(void) {
  Sint32 n;

  OEGridCreate(0);
  for (n = 0; n < 16; ++n)
  {
    OESprCreateIndx(n + 48);
  }
  for (n = 0; n < 11; ++n)
  {
    OESprCreateIndx(n);
  }
  nMenuIndx = 0;
  UpdateMenu();
  OESetSequenceNum(2);
}

void STGMenu(void) {
  if (swData1 & 256)
  {
    if (nMenuIndx == 0) nMenuIndx = MENUITEMMAX - 1;
    else --nMenuIndx;
    UpdateMenu();
    KeyWait();
  }
  else if (swData1 & 512)
  {
    if (nMenuIndx == MENUITEMMAX - 1) nMenuIndx = 0;
    else ++nMenuIndx;
    UpdateMenu();
    KeyWait();
  }

  else if (swData1 & 16384)
  {
    OESetSequenceNum(999);
  }

  if (swData1 & 4096)
  {
    OESetSequenceNum(998);
  }
}

Sint32 STGEnd(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0)
  {
    *lpSelectedStage = menuTbl[nMenuIndx].retcode;
    ret = 15;
  }
  return ret;
}

Sint32 STGEnd2(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0)
  {
    ret = 9;
  }
  return ret;
}

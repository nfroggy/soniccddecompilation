#include "../../types.h"
#include "bestitem.h"
#include "../common/grid.h"
#include "../common/hmx_oeeactl.h"
#include "../../impfuncs.h"
#include "besentry.h"
#include "bestsprt.h"

static void UpdateMenu(void);
static Sint32 STGMenuMaxCheck(Sint32 index);

extern Uint16 nSequenceNum;
extern bestitem menuTbl[];
extern Sint32 MENUITEMMAX;
extern Uint16 swData1;
extern Sint32 nTimerCunt;
Sint32 ScrollCount = 0;
static Sint32 nMenuIndx = 0;

void STGDraw(void) {
  if (nSequenceNum > 1)
  {
    srfDraw();
  }
}

static void UpdateMenu(void) {
  Sint32 i, index;

  for (i = 0; i < 9; ++i)
  {
    index = STGMenuMaxCheck(nMenuIndx + i);
    if (menuTbl[index].color != 0)
      StrMenu(i, menuTbl[index].str);
    else
      StrMenu2(i, menuTbl[index].str);
  }
}

static Sint32 STGMenuMaxCheck(Sint32 index) {
  if (index >= MENUITEMMAX) return index - MENUITEMMAX;
  return index;
}

void STGStart(void) {
  Sint32 n;

  CDPlay(29);
  OEGridCreate(0);
  for (n = 0; n < 16; ++n)
  {
    OESprCreateIndx(n + 73);
  }
  for (n = 0; n < 8; ++n)
  {
    OESprCreateIndx(n + 92);
  }

OESprCreateIndx(89);
  OESprCreateIndx(90);
  OESprCreateIndx(91);
  nMenuIndx = 0;
  ScrollCount = 0;
  UpdateMenu();
  OESetSequenceNum(2);

}

void STGMenu(void) {
  if (swData1 & 256)
  {
    if (nMenuIndx > 0 || (nMenuIndx == 0 && ScrollCount < 0))
    {
      ++ScrollCount;
      if (ScrollCount == 3)
      {
        ScrollCount = 0;

        if (nMenuIndx != 0)
          --nMenuIndx;
      }
      UpdateMenu();
      KeyWait();
    }
  } else
  {
    if (swData1 & 512)
    {
      if (nMenuIndx <= MENUITEMMAX - 11
          || (nMenuIndx == MENUITEMMAX - 10 && ScrollCount >= -1))
      {
        --ScrollCount;
        if (ScrollCount == -3)
        {
          ScrollCount = 0;

          if (nMenuIndx != MENUITEMMAX - 1)
            ++nMenuIndx;
        }
        UpdateMenu();
        KeyWait();
      }
    }
  }
  if (swData1 & 4096)
  {
    OESetSequenceNum(998);
  }
}

Sint32 STGEnd2(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0)
  {
    ret = 9;
    CDPause(2);
  }
  return ret;
}

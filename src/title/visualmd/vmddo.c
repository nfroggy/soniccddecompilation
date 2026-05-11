#include "../../types.h"
#include "vmddo.h"
#include "../common/grid.h"
#include "../common/hmx_oeeactl.h"
#include "vmdentry.h"
#include "vmdsprt.h"

static void UpdateMenu(void);

extern Uint16 nSequenceNum;
Uint16 nMenuKind;
extern Uint16 swData1;
extern Uint32* lpbVisualmode;
extern Sint32 nTimerCunt;

void VMDDraw(void) {
  if (nSequenceNum > 1)
  {
    srfDraw();
  }

}

static void UpdateMenu(void) {
  if (nMenuKind == 1) StrEXIT(1);
  else StrEXIT(0);
  if (nMenuKind == 2) StrOPENING(1);
  else StrOPENING(0);
  if (nMenuKind == 3) StrGOODENDING(1);
  else StrGOODENDING(0);
  if (nMenuKind == 4) StrBADENDING(1);
  else StrBADENDING(0);
  if (nMenuKind == 5) StrPENCILTEST(1);
  else StrPENCILTEST(0);
}

void VMDStart(void) {
  Sint32 n;

  OEGridCreate(0);
  for (n = 0; n < 16; ++n)
  {
    OESprCreateIndx(n + 52);
  }
  nMenuKind = 1;
  UpdateMenu();
  OESetSequenceNum(2);
}

void VMDMenu(void) {
  Uint16 nMenuKindOld;

  if (swData1 & 512)
  {
    nMenuKindOld = nMenuKind;
    nMenuKind = nMenuKind + 1 < 5 ? nMenuKind + 1 : 5;
    if (nMenuKind == nMenuKindOld) return;

    UpdateMenu();
    KeyWait();

  }
  else if (swData1 & 256)
  {
    nMenuKindOld = nMenuKind;
    nMenuKind = nMenuKind - 1 > 1 ? nMenuKind - 1 : 1;
    if (nMenuKind == nMenuKindOld) return;

    UpdateMenu();
    KeyWait();

  }
  else if (swData1 & 16384 || swData1 & 4096)
  {
    OESetSequenceNum(999);
  }

}

Sint32 VMDEnd(void) {
  Sint32 ret = 0;

  if (nTimerCunt == 0)
  {
    switch (nMenuKind)
    {
      case 1:
        ret = 9;
        *lpbVisualmode = 0;
        break;
      case 2: ret = 8; break;
      case 3: ret = 10; break;
      case 4: ret = 11; break;
      case 5: ret = 12; break;
    }
  }
  return ret;
}

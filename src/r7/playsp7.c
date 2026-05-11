#include "../equ.h"
#include "playsp7.h"
#include "../actset.h"
#include "../fcol.h"

Sint16 playsp_tbla[7] = { 10, 190, 334, 335, 344, 345, -1 };
Sint16 playsp_tblb[11] = { 10, 190, 334, 335, 344, 345, 154, 155, 164, 165, -1 };
Sint16 playsp_tblc[6] = { 618, 517, 527, 571, 581, -1 };
Sint16 playsp_tbld[7] = { 10, 210, 354, 355, 364, 365, -1 };
Sint16* playsp_tbl[4] =
{
  playsp_tblb,
  playsp_tbla,
  playsp_tbld,
  playsp_tblc
};

void playsp(void) {
  Sint32 i;
  Sint16 wD0, wD1, wD2, wD3;
  Sint16* pPlaysptbl;
  sprite_status* pNewactwk;

  if (gametimer.b.l & 3) return;

  wD0 = actwk[0].xspeed.w;
  if (wD0 < 0) wD0 *= -1;

  if (wD0 < 2560) return;

  wD2 = actwk[0].yposi.w.h + 8;
  wD3 = actwk[0].xposi.w.h;
  wD0 = scramapad(&actwk[0], wD3, wD2) & 2047;
  wD1 = time_flag & 127;
  if (wD1 == 2)
    wD1 += generate_flag;

  pPlaysptbl = playsp_tbl[wD1];

  i = 0;
  do
  {
    wD1 = pPlaysptbl[i++];
    if (wD1 < 0) return;
  } while (wD0 != wD1);

  if (actwkchk(&pNewactwk) == 0)
  {
    pNewactwk->actno = 9;
    pNewactwk->xposi.w.h = actwk[0].xposi.w.h;
    pNewactwk->yposi.w.h = actwk[0].yposi.w.h;
    pNewactwk->yposi.w.h += (char)actwk[0].sprvsize;
    if (actwk[0].xspeed.w < 0)
      pNewactwk->cddat |= 1;
  }
}

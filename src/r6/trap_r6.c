#include "../equ.h"
#include "trap_r6.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

void(*optbr6_tbl[5])(sprite_status*) =
{
  &optbr6_init,
  &optbr6_chk1,
  &optbr6_mov1,
  &optbr6_chk2,
  &optbr6_mov2
};
sprite_pattern opat0 =
{
  1,
  { { -4, -32, 0, 311 } }
};
sprite_pattern opat1 =
{
  1,
  { { -4, -32, 0, 312 } }
};
sprite_pattern opat2 =
{
  1,
  { { -4, -32, 0, 313 } }
};
sprite_pattern* optbr6pat[4] =
{
  &opat0,
  &opat1,
  &opat2,
  &opat2
};

void optbr6(sprite_status* pActwk) {
  optbr6_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

void optbr6_ridechk(sprite_status* pActwk) {
  if (pActwk->patno != 0) return;

  ride_on_chk(pActwk, &actwk[0]);
}

void optbr6_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 17424;
  pActwk->patbase = optbr6pat;
  pActwk->sprhsize = 4;
  pActwk->sprvsize = 32;

  if (pActwk->userflag.b.h != 0)
  {
    pActwk->actflg |= 1;
    pActwk->cddat |= 1;
  }

  optbr6_chk1(pActwk);
}

void optbr6_chk1(sprite_status* pActwk) {
  Sint16 iD0, iD1;

  iD0 = pActwk->yposi.w.h - actwk[0].yposi.w.h;
  if (iD0 < 0)
    iD0 = -iD0;

  if (iD0 >= 64) goto label1;

  if (pActwk->userflag.b.h != 0)
  {

    iD1 = pActwk->xposi.w.h;
    iD0 = actwk[0].xposi.w.h;
  }
  else
  {

    iD0 = pActwk->xposi.w.h;
    iD1 = actwk[0].xposi.w.h;
  }

  iD0 -= iD1;
  if (iD0 < 0) goto label1;
  if (iD0 >= 64) goto label1;

  ((Sint16*)pActwk)[31] = 0;
  pActwk->r_no0 += 2;
  if (pActwk->actflg & 128)
    soundset(164);
label1:
  optbr6_ridechk(pActwk);
}

void optbr6_mov1(sprite_status* pActwk) {
  pActwk->actfree[18] = 0;
  tobira_cnt(pActwk);
  if (pActwk->patno == 3)
    pActwk->r_no0 += 2;
  optbr6_ridechk(pActwk);
}

void optbr6_chk2(sprite_status* pActwk) {
  Sint16 iD0, iD1;

  iD0 = pActwk->yposi.w.h - actwk[0].yposi.w.h;
  if (iD0 < 0)
    iD0 = -iD0;

  if (iD0 >= 64) goto label1;

  if (pActwk->userflag.b.h != 0)
  {

    iD1 = actwk[0].xposi.w.h;
    iD0 = pActwk->xposi.w.h;
  }
  else
  {

    iD0 = actwk[0].xposi.w.h;
    iD1 = pActwk->xposi.w.h;
  }

  iD0 -= iD1;
  if (iD0 < 0) goto label1;
  if (iD0 < 64) goto label1;

  ((Sint16*)pActwk)[31] = 0;
  pActwk->r_no0 += 2;
  soundset(164);
label1:
  optbr6_ridechk(pActwk);
}

void optbr6_mov2(sprite_status* pActwk) {
  pActwk->actfree[18] = 1;
  tobira_cnt(pActwk);
  if (pActwk->patno == 0)
    pActwk->r_no0 = 2;

  optbr6_ridechk(pActwk);
}

void tobira_cnt(sprite_status* pActwk) {
  pActwk->actfree[16] += 64;
  if (((char*)pActwk)[62] >= 0) return;

  if (pActwk->actfree[18] == 0)
    ++pActwk->patno;
  else
  {
    if (pActwk->patno > 0)
      --pActwk->patno;
  }
}

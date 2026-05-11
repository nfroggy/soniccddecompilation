#include "../equ.h"
#include "batta.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../suicide.h"

#if defined(R61B) || defined(R62B)
  #define SPRITE_BATTA_BASE 477
#else
  #define SPRITE_BATTA_BASE 483
#endif

void(*batta_tbl[4])(sprite_status*) =
{
  &batta_init,
  &batta_down,
  &batta_wait,
  &batta_up
};
sprite_pattern ep_batta0 =
{
  1,
  { { -16, -18, 0, SPRITE_BATTA_BASE } }
};
sprite_pattern ep_batta1 =
{
  1,
  { { -16, -29, 0, SPRITE_BATTA_BASE + 1 } }
};
sprite_pattern* pat_batta_e[2] =
{
  &ep_batta0,
  &ep_batta1
};
sprite_pattern bp_batta0 =
{
  1,
  { { -16, -18, 0, SPRITE_BATTA_BASE + 2 } }
};
sprite_pattern bp_batta1 =
{
  1,
  { { -16, -29, 0, SPRITE_BATTA_BASE + 3 } }
};
sprite_pattern* pat_batta_b[2] =
{
  &bp_batta0,
  &bp_batta1
};

void batta(sprite_status* pActwk) {
  if (enemy_suicide(pActwk)) return;
  batta_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

void batta_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 9272;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  *(Sint32*)&pActwk->actfree[4] = 458752;
  pActwk->actfree[12] = 255;
  ((Sint16*)pActwk)[28] = -16;
  pActwk->patno = 1;

  batta_patexg(pActwk);
  if (pActwk->userflag.b.h == 0)
  {
    pActwk->patbase = pat_batta_e;
    *(Sint32*)&pActwk->actfree[0] = -40960;
    pActwk->actfree[8] = 7;
    pActwk->actfree[9] = 7;
  }
  else
  {
    pActwk->patbase = pat_batta_b;
    *(Sint32*)&pActwk->actfree[0] = -32768;
    pActwk->actfree[8] = 3;
    pActwk->actfree[9] = 3;
  }

  ++pActwk->actfree[8];
}

Sint16 emylr_jump(sprite_status* pActwk, Uint8 byD3) {
  if (pActwk->actfree[12] == 255)
    emycol_r(pActwk, byD3);
  else
    emycol_l(pActwk, byD3);
}

void batta_down(sprite_status* pActwk) {
  Sint16 iD1, iD3;

  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[4];

  iD1 = emycol_d(pActwk);
  ((Sint16*)pActwk)[33] = iD1;
  iD3 = ((Sint16*)pActwk)[28];
  iD1 = emylr_jump(pActwk, iD3);
  if (iD1 < 0)
  {
    if (((Sint16*)pActwk)[33] >= 0)
    {
      batta_wall(pActwk);
      return;
    }
    if (iD1 <= ((Sint16*)pActwk)[33])
      batta_wall(pActwk);
    else
      batta_floor(pActwk);
    return;
  }

  if (((Sint16*)pActwk)[33] < 0)
  {
    batta_floor(pActwk);
    return;
  }
  *(Sint32*)&pActwk->actfree[4] += 8192;
  if (*(Sint32*)&pActwk->actfree[4] >= 458752)
    *(Sint32*)&pActwk->actfree[4] = 458752;

}

void batta_floor(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->yposi.w.h += ((Sint16*)pActwk)[33];
  if (pActwk->userflag.b.h == 0)
    ((Sint16*)pActwk)[31] = 1;
  else
    ((Sint16*)pActwk)[31] = 20;
}

void batta_wait(sprite_status* pActwk) {
  Sint16 iD6;

  if (pActwk->userflag.b.h != 0)
  {
    iD6 = 7;
    switch (((Sint16*)pActwk)[31])
    {
      case 11:
      case 18:
        iD6 = -iD6;
      case 6:
      case 15:
        pActwk->yposi.w.h += iD6;
        batta_patexg(pActwk);
        break;
    }
  }

--((Sint16*)pActwk)[31];
  if (((Sint16*)pActwk)[31] == 0)
  {
    pActwk->r_no0 += 2;
    pActwk->yposi.w.h -= 7;
    batta_patexg(pActwk);
    if (pActwk->userflag.b.h == 0)
      *(Sint32*)&pActwk->actfree[4] = -393216;
    else
      *(Sint32*)&pActwk->actfree[4] = -327680;
    --((char*)pActwk)[54];
    if (((char*)pActwk)[54] < 0)
      batta_wall(pActwk);
  }
}

void batta_up(sprite_status* pActwk) {
  Sint16 iD1, iD3;

  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[4];
  iD1 = emycol_u(pActwk);
  ((Sint16*)pActwk)[33] = iD1;
  iD3 = ((Sint16*)pActwk)[28];
  iD1 = emylr_jump(pActwk, iD3);
  if (iD1 < 0)
  {
    if (((Sint16*)pActwk)[33] >= 0 || iD1 <= ((Sint16*)pActwk)[33])
    {

      batta_wall(pActwk);
    }
    else
      batta_ceiling(pActwk);
    return;
  }

  if (((Sint16*)pActwk)[33] < 0)
  {
    batta_ceiling(pActwk);
    return;
  }
  *(Sint32*)&pActwk->actfree[4] += 8192;
  if (*(Sint32*)&pActwk->actfree[4] >= 0)
    batta_fall(pActwk);
}

void batta_ceiling(sprite_status* pActwk) {
  pActwk->yposi.w.h -= ((Sint16*)pActwk)[33];
  *(Sint32*)&pActwk->actfree[4] = 0;
  batta_fall(pActwk);
}

void batta_fall(sprite_status* pActwk) {
  pActwk->r_no0 -= 4;
  pActwk->yposi.w.h -= 11;
  batta_patexg(pActwk);
}

void batta_wall(sprite_status* pActwk) {
  pActwk->actfree[8] = pActwk->actfree[9];
  pActwk->actflg ^= 1;
  pActwk->cddat ^= 1;
  *(Sint32*)&pActwk->actfree[0] = -(long int)*(Sint32*)&pActwk->actfree[0];
  ((Sint16*)pActwk)[28] = -((Sint16*)pActwk)[28];
  if (pActwk->actfree[12] == 255)
    pActwk->actfree[12] = 1;
  else
    pActwk->actfree[12] = 255;
}

void batta_patexg(sprite_status* pActwk) {
  if (pActwk->patno != 0)
  {
    pActwk->patno = 0;
    pActwk->sprvsize = 19;
    pActwk->colino = 47;
  }
  else
  {
    pActwk->patno = 1;
    pActwk->sprvsize = 28;
    pActwk->colino = 48;
  }
}

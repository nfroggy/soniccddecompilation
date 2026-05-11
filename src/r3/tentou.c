#include "../equ.h"
#include "tentou.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R31A) || defined(R31B) || defined(R32A)
  #define SPRITE_TENTOU_BASE 521
#elif defined(R33C) || defined(R33D)
  #define SPRITE_TENTOU_BASE 525
#else
  #define SPRITE_TENTOU_BASE 474
#endif

static Uint8 bCarry;
void(*tentou_tbl[6])(sprite_status*) = {
  &ten_a_init,
  &ten_a_fall,
  &ten_a_wait,
  &ten_a_lr,
  &ten_a_up,
  &ten_a_gake
};
void(*tentou1_tbl[5])(sprite_status*) = {
  &ten_b_init,
  &ten_b_fall,
  &ten_b_wait,
  &ten_b_blink,
  &ten_b_die
};
Uint8 pchg0_0[5] = { 3, 0, 1, 2, 255 };
Uint8* pchg0[1] = { pchg0_0 };
sprite_pattern ten_b_tentou0 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE } }
};
sprite_pattern ten_b_tentou1 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE + 1 } }
};
sprite_pattern ten_b_tentou2 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE + 2 } }
};
sprite_pattern* pat_ten_b_tentou[3] = {
  &ten_b_tentou0,
  &ten_b_tentou1,
  &ten_b_tentou2
};
sprite_pattern e_tentou0 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE + 3 } }
};
sprite_pattern e_tentou1 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE + 4 } }
};
sprite_pattern e_tentou2 = {
  1,
  { { -24, -24, 0, SPRITE_TENTOU_BASE + 5 } }
};
sprite_pattern* pat_e_tentou[3] = {
  &e_tentou0,
  &e_tentou1,
  &e_tentou2
};
Uint8 pchg1_0[4] = { 1, 0, 1, 255 };
Uint8* pchg1[1] = { pchg1_0 };
sprite_pattern bomten_b_0 = {
  1,
  { { -8, -8, 0, SPRITE_TENTOU_BASE + 6 } }
};
sprite_pattern bomten_b_1 = {
  1,
  { { -8, -8, 0, SPRITE_TENTOU_BASE + 7 } }
};
sprite_pattern* tentou_bomten_b_pat[2] = {
  &bomten_b_0,
  &bomten_b_1
};

void tentou(sprite_status* pActwk) {
  if (pActwk->userflag.b.h - 1 >= 0)
  {
    tentou1(pActwk);
    return;
  }
  if (enemy_suicide(pActwk) == -1) return;

  tentou_tbl[pActwk->r_no0 / 2](pActwk);
  patchg(pActwk, pchg0);
  actionsub(pActwk);
  frameout_s(pActwk);
}

void ten_a_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 132;
  pActwk->sprpri = 1;
  pActwk->colino = 6;
  pActwk->sprhsize = 14;
  pActwk->sprvsize = 14;
  pActwk->sproffset = 42090;
  if (pActwk->userflag.w >= 0)
  {
    pActwk->patbase = pat_e_tentou;
    pActwk->actfree[21] = 255;
  }
  else
    pActwk->patbase = pat_ten_b_tentou;
  ten_a_fall(pActwk);
}

void ten_a_fall(sprite_status* pActwk) {
  Sint16 iD1;

  pActwk->yposi.l += 65536;
  iD1 = emycol_d(pActwk);
  iD1 -= 8;
  if (iD1 <= 0)
  {
    pActwk->yposi.w.h -= iD1;
    *(Sint32*)&pActwk->actfree[8] = pActwk->yposi.l;
    *(Sint32*)&pActwk->actfree[4] = 12288;
    ((Sint16*)pActwk)[29] = 8;
    pActwk->r_no0 += 2;
    if ((long int)*(Sint32*)&pActwk->actfree[0] != 0)
      pActwk->r_no0 += 2;
  }
}

Sint32 abs(Sint32 i) {
  if (i >= 0)
    return i;

  return -i;
}

void ten_a_wait(sprite_status* pActwk) {
  sprite_status* pPlaywk = &actwk[0];
  Sint16 iD0, iD1;

  ten_a_hover(pActwk);

  iD0 = pActwk->yposi.w.h - pPlaywk->yposi.w.h;
  iD0 = abs(iD0);
  if (iD0 <= 80)
  {

iD0 = pActwk->xposi.w.h - pPlaywk->xposi.w.h;
    iD1 = iD0;
    iD0 = abs(iD0);
    if (iD0 <= 80)
    {

pActwk->r_no0 += 2;
      *(Sint32*)&pActwk->actfree[0] = 4294918144;
      if (iD1 >= 0)
      {
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        *(Sint32*)&pActwk->actfree[0] = -(long int)*(Sint32*)&pActwk->actfree[0];
      }
    }
  }
}

void ten_a_lr(sprite_status* pActwk) {
  Sint16 iD1;
  sprite_status* pActfree;

  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l = *(Sint32*)&pActwk->actfree[8];

  if (*(Sint32*)&pActwk->actfree[0] >= 0)
    iD1 = emycol_r(pActwk, pActwk->sprhs);
  else
    iD1 = emycol_l(pActwk, pActwk->sprhs);

  if (iD1 >= 0)
  {
    iD1 = emycol_d(pActwk);
    if (iD1 < 16)
    {
      iD1 -= 8;
      pActwk->yposi.w.h += iD1;
      *(Sint32*)&pActwk->actfree[8] = pActwk->yposi.l;
      ten_a_hover(pActwk);
      if (pActwk->actfree[21] != 0)
      {
        ((Uint16*)pActwk)[29] &= 127;
        if (((Uint16*)pActwk)[29] == 0)
        {
          if (actwkchk(&pActfree) == 0)
          {
            pActfree->actno = pActwk->actno;
            pActfree->xposi.l = pActwk->xposi.l;
            pActfree->yposi.l = pActwk->yposi.l;
            pActfree->yposi.w.h += 16;
            pActfree->userflag.b.h = 1;
          }
        }
      }
    }
    else
    {

      pActwk->r_no0 += 4;
      ten_a_hover(pActwk);
      *(Sint32*)&pActwk->actfree[8] = pActwk->yposi.l;
    }
  }
  else
  {

    pActwk->r_no0 += 2;
  }
}

void ten_a_up(sprite_status* pActwk) {
  pActwk->yposi.l -= 32768;
}

void ten_a_gake(sprite_status* pActwk) {
  char cwk, cRwk;
  Sint16 iD1, iD3, iD4;

  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  iD3 = pActwk->xposi.w.h;
  iD4 = pActwk->sprhsize;
  iD3 -= iD4;
  if ((long int)*(Sint32*)&pActwk->actfree[0] > 0)
    iD3 += iD4 + iD4;
  iD1 = emycol_d2(pActwk, iD3);
  if (iD1 >= 16)
  {

    cwk = 2;
    cwk = -cwk * 4;
    cRwk = pActwk->r_no0;
    cRwk = cRwk + cwk;
    pActwk->r_no0 = cRwk;
  }
}

void ten_a_hover(sprite_status* pActwk) {
  Uint16 wD0;

  ++((Sint16*)pActwk)[29];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[4];
  wD0 = ((Uint16*)pActwk)[29];
  wD0 &= 15;
  if (wD0 != 0) return;
  *(Sint32*)&pActwk->actfree[4] = -(long int)*(Sint32*)&pActwk->actfree[4];
}

void ten_a_range(sprite_status* pActwk) {
  Sint16 iD0;
  sprite_status* pPlaywk = &actwk[0];

  iD0 = pPlaywk->yposi.w.h;
  iD0 -= pActwk->yposi.w.h;
  iD0 += 256;
  bCarry = CCset(iD0, 512);
  iD0 -= 512;
  if (bCarry == 1)
  {
    iD0 = pPlaywk->yposi.w.h - pActwk->yposi.w.h;
    iD0 += 256;
    iD0 -= 512;
  }
}

void tentou1(sprite_status* pActwk) {
  Uint8 byRwk;

  byRwk = pActwk->r_no0 / 2;
  tentou1_tbl[byRwk](pActwk);
  actionsub(pActwk);
}

void ten_b_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 2;
  pActwk->colino = 139;
  pActwk->sprhs = 6;
  pActwk->sprhsize = 6;
  pActwk->sprvsize = 6;
  pActwk->sproffset = 1173;
  pActwk->patbase = tentou_bomten_b_pat;
  ten_b_fall(pActwk);
}

void ten_b_fall(sprite_status* pActwk) {
  Sint16 iD1;

  if (pActwk->colicnt != 0)
  {
    ten_b_die(pActwk);
    return;
  }
  pActwk->yposi.l += 65536;
  iD1 = emycol_d(pActwk);
  if (iD1 < 0)
  {
    pActwk->yposi.w.h += iD1;
    ((Sint16*)pActwk)[23] = 120;
    pActwk->r_no0 += 2;
  }
}

void ten_b_wait(sprite_status* pActwk) {
  if (pActwk->colicnt != 0)
  {
    ten_b_die(pActwk);
    return;
  }
  --((Sint16*)pActwk)[23];
  if (((Sint16*)pActwk)[23] == 0)
  {
    ((Sint16*)pActwk)[23] = 120;
    pActwk->r_no0 += 2;
  }
}

void ten_b_blink(sprite_status* pActwk) {
  if (pActwk->colicnt != 0)
  {
    ten_b_die(pActwk);
    return;
  }
  --((Sint16*)pActwk)[23];
  if (((Sint16*)pActwk)[23] == 0)
  {
    pActwk->r_no0 += 2;
  }
  patchg(pActwk, pchg1);
}

void ten_b_die(sprite_status* pActwk) {
  pActwk->actno = 24;
  pActwk->r_no0 = 0;
  pActwk->r_no1 = 1;
  if ((char)pActwk->actflg < 0)
    soundset(158);
}

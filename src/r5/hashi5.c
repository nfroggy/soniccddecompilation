#include "../equ.h"
#include "hashi5.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

static void hashi5_init(sprite_status* pActwk);
static void hashi5_move(sprite_status* pActwk);
static void hashi5_move2(sprite_status* pActwk);
static void hashi5_posiget1p(sprite_status* pActwk);
static void hashi5_rideplayset1p(sprite_status* pActwk);
static void hashi5_posiset(sprite_status* pActwk);
static Sint32 hashi5_ridechk1p(sprite_status* pActwk);

static sprite_pattern hashi5_pat0 = {
  1,
  { { -8, -8, 0, 429 } }
};
static sprite_pattern hashi5_pat1;
static sprite_pattern hashi5_pat2 = {
  1,
  { { -72, -8, 0, 430 } }
};
sprite_pattern* hashi5pat[3] = {
  &hashi5_pat0,
  &hashi5_pat1,
  &hashi5_pat2
};
static Uint8 hashitbl[272] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  8,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  8, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  8, 10, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,
   2,  4,  6,  8, 10, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,
   2,  4,  6,  8, 10, 12, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,
   2,  4,  6,  8, 10, 12, 14, 12, 10,  8,  6,  4,  2,  0,  0,  0,
   2,  4,  6,  8, 10, 12, 14, 14, 12, 10,  8,  6,  4,  2,  0,  0,
   2,  4,  6,  8, 10, 12, 14, 16, 14, 12, 10,  8,  6,  4,  2,  0,
   2,  4,  6,  8, 10, 12, 14, 16, 16, 14, 12, 10,  8,  6,  4,  2
};
static Uint8 hashitbl2[256] = {
  255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  181, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  126, 219, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   97, 181, 236, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   74, 147, 205, 243, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   62, 126, 176, 219, 246, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   56, 109, 157, 197, 228, 248, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   49,  97, 142, 181, 212, 236, 251, 255,   0,   0,   0,   0,   0,   0,   0,   0,
   43,  86, 126, 162, 193, 219, 238, 251, 255,   0,   0,   0,   0,   0,   0,   0,
   37,  74, 115, 147, 176, 205, 225, 243, 252, 255,   0,   0,   0,   0,   0,   0,
   31,  68, 103, 136, 167, 189, 212, 231, 244, 253, 255,   0,   0,   0,   0,   0,
   31,  62,  92, 126, 152, 176, 201, 219, 234, 246, 253, 255,   0,   0,   0,   0,
   25,  56,  86, 115, 142, 167, 189, 209, 225, 238, 248, 254, 255,   0,   0,   0,
   25,  56,  80, 109, 131, 157, 176, 197, 216, 228, 241, 248, 254, 255,   0,   0,
   25,  49,  74, 103, 126, 147, 167, 189, 205, 219, 231, 243, 249, 254, 255,   0,
   25,  49,  74,  97, 120, 142, 162, 181, 197, 212, 225, 236, 244, 251, 254, 255
};

void hashi5(sprite_status* pActwk) {
  void(*hashi5_tbl[3])(sprite_status*) = {
    &hashi5_init,
    &hashi5_move,
    &hashi5_move2
  };

  hashi5_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

static void hashi5_init(sprite_status* pActwk) {
  sprite_status* pNewactwk;
  Sint32 i;

Sint16 xofsettbl[8] = { 48, 32, 16, 0, -16, -32, -48, -64 };

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 17152;
  pActwk->patbase = hashi5pat;
  ((Sint16*)pActwk)[29] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[27] = pActwk->yposi.w.h;
  pActwk->sprhsize = 64;
  pActwk->sprvsize = 8;
  pActwk->patno = 1;

  for (i = 0; i < 8; ++i) {

    if (actwkchk(&pNewactwk) == 0) {
      pActwk->actfree[i] = (char)(pNewactwk - actwk);
      pNewactwk->actno = 47;
      pNewactwk->r_no0 = 4;
      ((Sint16*)pNewactwk)[29] = pActwk->xposi.w.h;
      ((Sint16*)pNewactwk)[27] = pActwk->yposi.w.h;
      pNewactwk->yposi.w.h = pActwk->yposi.w.h;
      pNewactwk->actflg |= 4;
      pNewactwk->sprpri = 3;
      pNewactwk->sproffset = 17152;
      pNewactwk->patbase = hashi5pat;
      pNewactwk->sprvsize = 8;
      pNewactwk->sprhsize = 8;
      ((Sint16*)pNewactwk)[28] = pActwk - actwk;
      pNewactwk->actfree[21] = 255;

      pNewactwk->actfree[18] = 7 - i;
      pNewactwk->xposi.w.h = xofsettbl[7 - i] + pActwk->xposi.w.h;
    }

  }
}

static void hashi5_move(sprite_status* pActwk) {
  pActwk->actfree[19] = 0;
  if (hashi5_ridechk1p(pActwk) != 0) {
    hashi5_posiget1p(pActwk);
  }
  if (pActwk->actfree[19]) {
    if (pActwk->actfree[16] != 64) {
      pActwk->actfree[16] += 4;
    }
    hashi5_posiset(pActwk);

    if (pActwk->actfree[19]) {
      hashi5_rideplayset1p(pActwk);
    }
  }
  else {

if (pActwk->actfree[16]) {
      pActwk->actfree[16] -= 4;
      hashi5_posiset(pActwk);
    }
  }

  frameout_s(pActwk);
}

static void hashi5_move2(sprite_status* pActwk) {
  if (actwk[((Sint16*)pActwk)[28]].actno != 47) frameout(pActwk);
  if (actwk[((Sint16*)pActwk)[28]].actfree[21]) frameout(pActwk);
}

static void hashi5_posiget1p(sprite_status* pActwk) {
  Uint16 wD0;

  if (!(actwk[0].cddat & 2)) {
    wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h + 72;
    if (!(wD0 & 32768)) {
      if (wD0 < 128) {

        pActwk->actfree[19] = wD0 >> 4;
        return;
      }
    }
  }

  pActwk->actfree[19] = 0;
}

static void hashi5_rideplayset1p(sprite_status* pActwk) {
  Uint8 bD0;

  bD0 = pActwk->actfree[19];
  actwk[0].yposi.w.h = actwk[pActwk->actfree[bD0]].yposi.w.h - 8 - (Sint16)(Uint16)actwk[0].sprvsize;
}

static void hashi5_posiset(sprite_status* pActwk) {
  sprite_status* pSubactwk;
  Uint16 Sin, Cos;
  int_union lD0;
  Sint16 wD2;
  Uint8 bD5;
  Uint8* pA2;
  Uint8* pA3;

  wD2 = (Uint16)pActwk->actfree[19];

  sinset(pActwk->actfree[16], (Sint16*)&Sin, (Sint16*)&Cos);
  bD5 = hashitbl[wD2 + 128];
  pA3 = &hashitbl2[(wD2 + 128 & 15) * 16];
  pA2 = pActwk->actfree;

  for ( ; wD2 >= 0; --wD2) {
    pSubactwk = &actwk[*pA2++];
    lD0.l = 0;
    lD0.b.b4 = *pA3++;
    lD0.l = (lD0.w.l + 1) * bD5 * Sin;
    pSubactwk->yposi.w.h = ((Sint16*)pSubactwk)[27] + lD0.w.h;
  }

  lD0.l = 0;
  lD0.b.b4 = pActwk->actfree[19];
  lD0.b.b4 = -(lD0.b.b4 - 7);
  if (lD0.b.b4 >= 0) {
    wD2 = lD0.w.l;
    lD0.w.l *= 16;
    pA3 = &hashitbl2[lD0.w.l + wD2];
    if (--wD2 < 0) return;

    for ( ; wD2 >= 0; --wD2) {
      pSubactwk = &actwk[*pA2++];
      lD0.l = 0;
      lD0.w.l = (Uint16)(*--pA3) + 1;
      lD0.l = (lD0.w.l + 1) * bD5 * Sin;
      pSubactwk->yposi.w.h = ((Sint16*)pSubactwk)[27] + lD0.w.h;
    }
  }
}

static Sint32 hashi5_ridechk1p(sprite_status* pActwk) {
  Sint16 wD0, wD1, wD2;

if (actwk[0].r_no0 != 6) {
    if (!editmode.b.h) {
      wD1 = 72;
      wD2 = 128;

      if (actwk[0].yspeed.w >= 0) {
        wD0 = wD1 + (actwk[0].xposi.w.h - pActwk->xposi.w.h);
        if (wD0 >= 0) {
          if (wD0 < wD2) {

            wD0 = pActwk->yposi.w.h - 8;

            wD2 = actwk[0].yposi.w.h;
            wD1 = wD2 + (Sint16)actwk[0].sprvsize + 4;
            if ((Uint16)wD0 <= (Uint16)wD1) {
              wD0 -= wD1;
              if ((Uint16)wD0 >= 65520) {
                if (actwk[0].r_no0 < 6) {
                  actwk[0].yposi.w.h = wD2 + wD0 + 3;

                  ride_on_set(pActwk, &actwk[0]);
                  return 1;
                }
              }
            }
          }
        }
      }
    }
  }

  if (actwk[0].actfree[19] == pActwk - actwk) {
    actwk[0].actfree[14] = 0;
    actwk[0].cddat &= 247;
  }

  return 0;
}

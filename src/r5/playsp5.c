#include "../equ.h"
#include "playsp5.h"
#include "../dircol.h"
#include "../fcol.h"

Sint16 belt_a[51] = {
   44,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   14,  15,  17,  18,  19,  35,  36,  37,  38,  39,
   49,  61,  62,  63,  64,  71,  72,  73,  74, 400,
  403, 404, 405, 406, 407, 408, 409, 410, 411, 413,
  415, 422, 423, 424, 448, 450,   0,   0,   0,   0,
    0
};
Sint16 belt_b[51] = {
   49,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   10,  11,  14,  15,  17,  18,  19,  35,  36,  37,
   38,  39,  49,  59,  60,  61,  62,  63,  64,  71,
   72,  73,  74, 400, 403, 404, 405, 406, 407, 408,
  409, 410, 411, 413, 415, 422, 423, 424, 483, 484,
  485
};
Sint16 belt_c[61] = {
   51,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   10,  11,  12,  13,  14,  15,  17,  18,  19,  35,
   36,  37,  38,  39,  49,  61,  62,  63,  64,  71,
   72,  73,  74,  75, 400, 403, 404, 405, 406, 407,
  408, 409, 410, 411, 413, 414, 415, 423, 424, 487,
  488, 490, 493,   0,   0,   0,   0,   0,   0,   0,
    0
};
Sint16 belt_d[51] = {
   49,   1,   2,   3,   4,   5,   6,   7,   8,   9,
   10,  11,  14,  15,  17,  18,  19,  35,  36,  37,
   38,  39,  49,  61,  62,  63,  64,  71,  72,  73,
   74,  75, 400, 403, 404, 405, 406, 407, 408, 409,
  410, 411, 413, 414, 415, 423, 424, 488, 489, 491,
  500
};
Sint16* belt_tbl[4] = {
  belt_b,
  belt_a,
  belt_d,
  belt_c
};

void playsp(void) {
  belt();
}

void belt(void) {
  Sint16 wD0, wD2, wD3;

  if (bossflag) return;

  wD3 = actwk[0].xposi.w.h - (Sint16)(Uint16)actwk[0].sprhs;
  wD2 = actwk[0].yposi.w.h + (Sint16)(Uint16)actwk[0].sprvsize + 2;
  wD0 = scramapad(&actwk[0], wD3, wD2) & 2047;
  if (belt_chk(wD0) == 0) {
    wD3 = actwk[0].xposi.w.h + (Sint16)(Uint16)actwk[0].sprhs;

    wD0 = scramapad(&actwk[0], wD3, wD2) & 2047;
    if (belt_chk(wD0) == 0) return;
  }
  belt_effect();
}

void belt_effect(void) {
  Sint32 lD0;
  char bD1;
  Sint16 wD1;

  switch (time_flag) {
    case 0:
      lD0 = 32768;
      break;
    case 1:
      lD0 = 65536;
      break;
    default:
      lD0 = 0x20000;
      break;
  }

  bD1 = -64;
  if (colrevflag) {
    lD0 = -lD0;
    bD1 = -bD1;
  }

  actwk[0].xposi.l += lD0;
  if ((wD1 = dircol(&actwk[0], &bD1)) < 0) {
    if (colrevflag) wD1 *= -1;

    actwk[0].xposi.w.h += wD1;
  }

}

Sint32 belt_chk(Sint16 wD0) {
  Uint8 bD1;
  Sint16* pBelt;
  Sint16 i, wD6;

  bD1 = time_flag & 127;
  if (bD1 == 2) bD1 += generate_flag;

  pBelt = belt_tbl[bD1];
  wD6 = *pBelt;

  for (i = 1; wD6 >= 0; --wD6) {
    if (wD0 == pBelt[i++]) return 1;
  }
  return 0;
}

#include "../types.h"
#include "common.h"
#include "sps_equ.h"
#include "etc_s.h"
#include "ens.h"

Uint16 sincostbl[130] = {
    255, 65535,  1024, 65528,  2047, 65504,  3069, 65464,  4088, 65408,  5104, 65337,  6117,
  65250,  7126, 65147,  8129, 65030,  9126, 64897, 10117, 64750, 11101, 64589, 12078, 64414,
  13046, 64224, 14005, 64022, 14955, 63807, 15895, 63579, 16825, 63340, 17744, 63088, 18651,
  62826, 19548, 62553, 20432, 62270, 21304, 61977, 22164, 61674, 23011, 61363, 23845, 61044,
  24666, 60717, 25474, 60383, 26268, 60041, 27049, 59694, 27816, 59340, 28569, 58981, 29309,
  58617, 30034, 58249, 30747, 57876, 31445, 57499, 32130, 57120, 32801, 56737, 33459, 56351,
  34103, 55964, 34734, 55574, 35352, 55183, 35957, 54791, 36549, 54398, 37128, 54004, 37695,
  53610, 38249, 53216, 38791, 52822, 39322, 52429, 39840, 52036, 40347, 51644, 40842, 51253,
  41327, 50863, 41800, 50475, 42262, 50089, 42714, 49704, 43156, 49321, 43587, 48940, 44009,
  48561, 44420, 48185, 44823, 47811, 45216, 47439, 45600, 47071, 45975, 46704, 46341, 46341
};
Uint16 acostbl[65] = {
  255, 256, 256, 256, 256, 256, 257, 257, 257, 258,
  259, 259, 260, 261, 262, 262, 263, 264, 265, 267,
  268, 269, 270, 272, 273, 274, 276, 277, 279, 281,
  282, 284, 286, 288, 289, 291, 293, 295, 297, 299,
  301, 304, 306, 308, 310, 312, 315, 317, 320, 322,
  324, 327, 329, 332, 334, 337, 340, 342, 345, 348,
  350, 353, 356, 359, 362
};

void ufo_dec(void) {
  --ufoleft;
}

void ring_add(Uint16 iD0) {
  ringno += iD0;
  if (ringno <= 999) return;
  ringno = 999;
}

void time_dec(void) {
  if (spgmmode & 2)
  {
    time_time_attack();
    return;
  }
  --tcnt20;
  if (tcnt20 == 0)
  {
    tcnt20 = 20;
    dec();
  }

if (tdecflg != 0)
  {
    --tdecflg;
    dec();
  }
}

void dec(void) {
  if (time_stop != 0) return;
  if (game_start != 0) return;
  if (--spe_time.l < 0)
  {
    spe_time.l = 0;
    tdecflg = 0;
    pldeadflg = 1;
  }

  tufo_initial();
  if (spe_time.l >= 15) return;
  se_no.b.b1 = 223;
}

void time_time_attack(void) {
  if (time_stop != 0) return;
  if (game_start != 0) return;
  spe_time.b.b4 += 3;
  if ((Uint8)spe_time.b.b4 < 60) return;

  spe_time.b.b4 -= 60;
  ++spe_time.b.b3;
  if ((Uint8)spe_time.b.b3 < 60) return;

  spe_time.b.b3 -= 60;
  ++spe_time.b.b2;
  if ((Uint8)spe_time.b.b2 < 10) return;

  spe_time.l = 604987;
  pldeadflg = 1;
}

void angleget(Sint16 iXtget, Sint16 iYtget, Sint16 iXbase, Sint16 iYbase, Uint8* iAngle, Uint8* iDirflg) {
  Sint16 iD1, iD2, iD3;
  Sint16 iwk;
  Sint32 lD1;

  iD2 = 0;
  iD3 = iXtget;
  iD3 ^= iXbase;
  iXtget -= iXbase;

  if (iXtget < 0)
  {
    if ((Uint16)iD3 & 32768) goto label2;

label1:
    iD2 |= 1;
    iXtget = -iXtget;
  }
  else
    if ((Uint16)iD3 & 32768) goto label1;

label2:
  iYtget -= iYbase;
  if (iYtget < 0)

    if (iYbase >= 0)
    {
      iD2 |= 2;
      iYtget = -iYtget;
    }
  if (iYtget >= iXtget)
  {
    iD2 |= 4;
    iwk = iXtget;
    iXtget = iYtget;
    iYtget = iwk;
  }
  lD1 = iYtget;
  lD1 <<= 6;
  if (iXtget == 0)
  {
    lD1 = 0;
  }
  else
    lD1 /= iXtget;

  iD1 = lD1 & 255;
  if ((Uint16)iD1 >= 64)
    iD1 = 63;

  *iAngle = iD1;
  *iDirflg = iD2;
}

void speedget(Uint8 bAngle, Uint8 bDirflg, Sint16 iBaseSpd, Sint32* lXspeed, Sint32* lYspeed) {
  Sint32 lwk;
  int_union lD0, lD1;

  lD0.l = 0;
  lD1.l = 0;
  lD1.w.l = bAngle;

  lD1.w.l *= 2;

  if (lD1.w.l == 0)
  {
    lD0.l = 0;
    lD1.w.l = (Uint16)iBaseSpd;
  }
  else
  {

    lD0.w.l = sincostbl[lD1.w.l++];
    lD1.w.l = sincostbl[lD1.w.l];

    lD0.l = iBaseSpd * lD0.w.l;
    lD0.l = ((Uint32)lD0.l >> 16) & 65535 | ((Uint32)lD0.l << 16) & (Uint32)65535 << 16;
    lD1.l = iBaseSpd * lD1.w.l;
    lD1.l = ((Uint32)lD1.l >> 16) & 65535 | ((Uint32)lD1.l << 16) & (Uint32)65535 << 16;
  }

  if (bDirflg & 4)
  {
    lwk = lD0.l;
    lD0.l = lD1.l;
    lD1.l = lwk;
  }

  if (bDirflg & 2)
    lD0.l = -lD0.l;

  lD0.l = ((Uint32)lD0.l >> 16) & 65535 | ((Uint32)lD0.l << 16) & (Uint32)65535 << 16;
  lD0.w.l = 0;
  lD0.l >>= 8;
  if (bDirflg & 1)
    lD1.w.l = -lD1.w.l;

  lD1.l = ((Uint32)lD1.l >> 16) & 65535 | ((Uint32)lD1.l << 16) & (Uint32)65535 << 16;
  lD1.w.l = 0;
  lD1.l >>= 8;

  *lXspeed = lD1.l;
  *lYspeed = lD0.l;
}

Sint32 dstnsget(Uint8 bAngle, Uint8 bDirflg, Sint16 iXposi, Sint16 iYposi, Sint16 iXbase, Sint16 iYbase) {
  Sint16 iD0, iD1, iD2;
  Sint32 lD0;

  iD1 = bAngle;

  iD0 = acostbl[iD1];
  if (bDirflg & 4)
  {
    iD1 = iYbase;
    iD2 = iYposi;
  }
  else
  {

    iD1 = iXbase;
    iD2 = iXposi;
  }

  iD1 -= iD2;
  if (iD1 < 0)
    iD1 = -iD1;

  lD0 = iD0 * iD1;
  lD0 >>= 8;
  return lD0;
}

Sint32 random(void) {
  int_union lD0, lD1;

  lD1.l = ranum;
  if (lD1.l == 0)
    lD1.l = 711800410;

  lD0.l = lD1.l;
  if (lD1.l & 32768 << 16) lD1.l *= 4, lD1.l |= 0x80000000; else lD1.l *= 4;
  lD1.l += lD0.l;
  if (lD1.l & 32768 << 16) lD1.l *= 8, lD1.l |= 0x80000000; else lD1.l *= 8;
  lD1.l += lD0.l;
  lD0.w.l = lD1.w.l;
  lD1.l = ((Uint32)lD1.l >> 16) & 65535 | ((Uint32)lD1.l << 16) & (Uint32)65535 << 16;
  lD0.w.l += lD1.w.l;
  lD1.w.l = lD0.w.l;
  lD1.l = ((Uint32)lD1.l >> 16) & 65535 | ((Uint32)lD1.l << 16) & (Uint32)65535 << 16;
  ranum = lD1.l;
  return lD0.l;
}

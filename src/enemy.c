#include "equ.h"
#include "enemy.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "dummy.h"
#include "etc.h"
#include "loader2.h"
#include "playsub.h"
#include "suicide.h"

static Uint8 ari_pchg00[4] = { 3, 0, 1, 255 };
static Uint8 ari_pchg01[4] = { 7, 2, 3, 255 };
Uint8* ari_pchg[2] =
{
  ari_pchg00,
  ari_pchg01
};
static sprite_pattern ari0 =
{
  3,
  {
    { -12, -19, 0, 370 },
    {  -8,   4, 0, 371 },
    {  14,  -4, 0, 372 }
  }
};
static sprite_pattern ari1 =
{
  3,
  {
    { -12, -20, 0, 373 },
    {  -8,   4, 0, 374 },
    {  18,  -4, 0, 375 }
  }
};
static sprite_pattern ari2 =
{
  3,
  {
    { -12, -19, 0, 376 },
    {  -8,   4, 0, 377 },
    {  14,  -4, 0, 378 }
  }
};
static sprite_pattern ari3 =
{
  3,
  {
    { -12, -20, 0, 379 },
    {  -8,   4, 0, 380 },
    {  18,  -4, 0, 381 }
  }
};
sprite_pattern* ari_pat[4] =
{
  &ari0,
  &ari1,
  &ari2,
  &ari3
};
static Uint8 chou_pchg00[7] = { 7, 0, 0, 1, 2, 1, 255 };
static Uint8 chou_pchg01[7] = { 3, 0, 0, 1, 2, 1, 255 };
Uint8* chou_pchg[2] =
{
  chou_pchg00,
  chou_pchg01
};
static sprite_pattern chou1e =
{
  1,
  { { -16, -16, 0, 394 } }
};
static sprite_pattern chou2e =
{
  1,
  { { -16, -8, 0, 395 } }
};
static sprite_pattern chou3e =
{
  1,
  { { -16, -16, 0, 396 } }
};
static sprite_pattern chou1b =
{
  1,
  { { -16, -16, 0, 397 } }
};
static sprite_pattern chou2b =
{
  1,
  { { -16, -8, 0, 398 } }
};
static sprite_pattern chou3b =
{
  1,
  { { -16, -16, 0, 399 } }
};
sprite_pattern* e_chou_pat[3] =
{
  &chou1e,
  &chou2e,
  &chou3e
};
sprite_pattern* b_chou_pat[3] =
{
  &chou1b,
  &chou2b,
  &chou3b
};
static Uint8 pchg_e_0[4] = { 1, 0, 1, 255 };
static Uint8 pchg_e_1[5] = { 6, 2, 3, 4, 252 };
Uint8* pchg_e[2] =
{
  pchg_e_0,
  pchg_e_1
};
static Uint8 pchg_b_0[4] = { 4, 0, 1, 255 };
static Uint8 pchg_b_1[5] = { 14, 2, 3, 4, 252 };
Uint8* pchg_b[2] =
{
  pchg_b_0,
  pchg_b_1
};
static sprite_pattern ka1e =
{
  1,
  { { -16, -12, 0, 400 } }
};
static sprite_pattern ka2e =
{
  1,
  { { -16, -4, 0, 401 } }
};
static sprite_pattern ka3e =
{
  1,
  { { -16, -16, 0, 402 } }
};
static sprite_pattern ka4e =
{
  1,
  { { -8, -16, 0, 403 } }
};
static sprite_pattern ka5e =
{
  1,
  { { -12, -16, 0, 404 } }
};
sprite_pattern* e_ka_pat[5] =
{
  &ka1e,
  &ka2e,
  &ka3e,
  &ka4e,
  &ka5e
};
static sprite_pattern ka1b =
{
  1,
  { { -16, -12, 0, 405 } }
};
static sprite_pattern ka2b =
{
  1,
  { { -16, -4, 0, 406 } }
};
static sprite_pattern ka3b =
{
  1,
  { { -16, -16, 0, 407 } }
};
static sprite_pattern ka4b =
{
  1,
  { { -8, -16, 0, 408 } }
};
static sprite_pattern ka5b =
{
  1,
  { { -12, -16, 0, 409 } }
};
sprite_pattern* b_ka_pat[5] =
{
  &ka1b,
  &ka2b,
  &ka3b,
  &ka4b,
  &ka5b
};
static Uint8 e_kamem_pchg00[4] = { 29, 0, 1, 255 };
static Uint8 e_kamem_pchg01[3] = { 59, 2, 252 };
Uint8* e_kamem_pchg[2] =
{
  e_kamem_pchg00,
  e_kamem_pchg01
};
static Uint8 b_kamem_pchg00[18] =
{
  5,
  0, 1,
  0, 1,
  0, 1,
  0, 1,
  0, 1,
  0, 1,
  0, 1,
  0, 1,
  252
};
static Uint8 b_kamem_pchg01[3] = { 11, 2, 252 };
Uint8* b_kamem_pchg[2] =
{
  b_kamem_pchg00,
  b_kamem_pchg01
};
static sprite_pattern kamem_pat00e =
{
  1,
  { { -16, -15, 0, 410 } }
};
static sprite_pattern kamem_pat00b =
{
  1,
  { { -16, -16, 0, 411 } }
};
static sprite_pattern kamem_pat01 =
{
  1,
  { { -16, -14, 0, 412 } }
};
static sprite_pattern kamem_pat02 =
{
  1,
  { { -17, -16, 0, 413 } }
};
sprite_pattern* e_kamem_pat[3] =
{
  &kamem_pat00e,
  &kamem_pat01,
  &kamem_pat02
};
sprite_pattern* b_kamem_pat[3] =
{
  &kamem_pat00b,
  &kamem_pat01,
  &kamem_pat02
};
static Uint8 tama_pchg00[6] = { 2, 0, 1, 2, 3, 255 };
Uint8* tama_pchg[1] = { tama_pchg00 };
static sprite_pattern tama_pat00 =
{
  1,
  { { -4, -4, 0, 414 } }
};
static sprite_pattern tama_pat01 =
{
  1,
 { { -4, -4, 0, 415 } }
};
static sprite_pattern tama_pat02 =
{
  1,
  { { -8, -8, 0, 416 } }
};
static sprite_pattern tama_pat03 =
{
  1,
  { { -8, -8, 0, 417 } }
};
sprite_pattern* tama_pat[4] =
{
  &tama_pat00,
  &tama_pat01,
  &tama_pat02,
  &tama_pat03
};
static Uint8 e_tagame_pchg00[3] = { 89, 6, 252 };
static Uint8 e_tagame_pchg01[20] =
{
  7,
  0, 0,
  0, 1,
  0, 1,
  2, 3,
  5, 5,
  5, 5,
  4, 5,
  4, 5,
  4, 5,
  255
};
Uint8* e_tagame_pchg[2] =
{
  e_tagame_pchg00,
  e_tagame_pchg01
};
static Uint8 b_tagame_pchg00[3] = { 89, 6, 252 };
static Uint8 b_tagame_pchg01[25] = {
  4,
  0, 0,
  0, 0,
  0, 0,
  0, 0,
  2, 2,
  3, 3,
  4, 4,
  4, 4,
  4, 4,
  4, 4,
  4, 4,
  4,
  255
};
Uint8* b_tagame_pchg[2] =
{
  b_tagame_pchg00,
  b_tagame_pchg01
};
static sprite_pattern tagame_pat00 =
{
  2,
  {
    { -16, -10, 0, 418 },
    { -16, -22, 0, 419 }
  }
};
static sprite_pattern tagame_pat01 =
{
  2,
  {
    { -16, -10, 0, 420 },
    { -16, -22, 0, 421 }
  }
};
static sprite_pattern tagame_pat02 =
{
  1,
  { { -16, -16, 0, 422 } }
};
static sprite_pattern tagame_pat03e =
{
  1,
  { { -16, -16, 0, 423 } }
};
static sprite_pattern tagame_pat04e =
{
  2,
  {
    { -16, -22, 0, 424 },
    { -16, -2, 0, 425 }
  }
};
static sprite_pattern tagame_pat05e =
{
  2,
  {
    { -16, -22, 0, 426 },
    { -16, -2, 0, 427 }
  }
};
static sprite_pattern tagame_pat03b =
{
  1,
  { { -16, -16, 0, 428 } }
};
static sprite_pattern tagame_pat04b =
{
  2,
  {
    { -16, -22, 0, 429 },
    { -16, -2, 0, 430 }
  }
};
static sprite_pattern tagame_pat05b =
{
  2,
  {
    { -16, -22, 0, 431 },
    { -16, -2, 0, 432 }
  }
};
static sprite_pattern tagame_pat06 =
{
  1,
  { { -16, -16, 0, 433 } }
};
sprite_pattern* e_tagame_pat[7] =
{
  &tagame_pat00,
  &tagame_pat01,
  &tagame_pat02,
  &tagame_pat03e,
  &tagame_pat04e,
  &tagame_pat05e,
  &tagame_pat06
};
sprite_pattern* b_tagame_pat[7] =
{
  &tagame_pat00,
  &tagame_pat01,
  &tagame_pat02,
  &tagame_pat03b,
  &tagame_pat04b,
  &tagame_pat05b,
  &tagame_pat06
};

void ene_ari(sprite_status* pActwk) {
  void(*tbl[3])(sprite_status*) =
  {
    &ari_init,
    &ari_fall,
    &ari_move
  };

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[25]);
}

void ari_init(sprite_status* pActwk) {
  Sint16 c;

  pActwk->actflg |= 4;
  pActwk->sprpri = 4;
  pActwk->patbase = ari_pat;
  pActwk->sprhs = 24;
  pActwk->sprhsize = 24;
  pActwk->sprvsize = 19;
  pActwk->colino = 41;
  ((Sint16*)pActwk)[25] = pActwk->xposi.w.h;
  pa_set(pActwk, 2);

  if (pActwk->userflag.b.h == 0)
  {
    *(Sint32*)&pActwk->actfree[0] = -65536;
    pActwk->mstno.b.h = 0;
  }
  else
  {
    *(Sint32*)&pActwk->actfree[0] = -32768;
    pActwk->mstno.b.h = 1;
  }

pActwk->yposi.l += 65536;
  c = emycol_d(pActwk);
  if (c < 0)
  {
    pActwk->r_no0 += 2;
  }
}

void ari_fall(sprite_status* pActwk) {
  Sint16 c;

  pActwk->yposi.l += 65536;
  c = emycol_d(pActwk);
  if (c < 0)
  {
    pActwk->r_no0 += 2;
  }
}

void ari_move(sprite_status* pActwk) {
  Sint16 xpos;
  Sint16 xpos_m;
  Sint16 c;
  Sint32 spd_x;

  do
  {
    spd_x = *(Sint32*)&pActwk->actfree[0];
    pActwk->xposi.l = pActwk->xposi.l + spd_x;
    xpos = pActwk->xposi.w.h;
    xpos_m = ((Sint16*)pActwk)[25];
    xpos = xpos - xpos_m;
    if (xpos < 0) xpos = -xpos;
    if (xpos < 128)
    {
      c = emycol_d(pActwk);
      if (c >= -7)
      {
        if (c < 8)
        {
          pActwk->yposi.w.h = pActwk->yposi.w.h + c;
          patchg(pActwk, ari_pchg);
          break;
        }
      }
    }
    spd_x = *(Sint32*)&pActwk->actfree[0];
    *(Sint32*)&pActwk->actfree[0] = -spd_x;
    pActwk->actflg = pActwk->actflg ^ 1;
    pActwk->cddat = pActwk->cddat ^ 1;
  } while (1);
}

void ene_chou(sprite_status* pActwk) {
  if (enemy_suicide(pActwk)) return;
  if (pActwk->r_no0 == 0) chou_init(pActwk);
  else chou_move(pActwk);
}

void chou_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->colino = 42;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 16;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[25] = pActwk->yposi.w.h;
  ((Sint16*)pActwk)[24] = -32768;
  pa_set(pActwk, 1);
  if (pActwk->userflag.b.h == 0)
  {
    ((Sint32*)pActwk)[13] = -32768;
    ((Sint16*)pActwk)[29] = -512;
    ((Sint16*)pActwk)[30] = 3;
    pActwk->mstno.b.h = 0;
    pActwk->patbase = e_chou_pat;
  }
  else
  {
    ((Sint32*)pActwk)[13] = -16384;
    ((Sint16*)pActwk)[29] = -256;
    ((Sint16*)pActwk)[30] = 4;
    pActwk->mstno.b.h = 1;
    pActwk->patbase = b_chou_pat;
  }
}

void chou_move(sprite_status* pActwk) {
  Sint16 xpos;
  Sint16 shift;
  Sint16 sSin, sCos;
  int_union lSin;

  pActwk->xposi.l += ((Sint32*)pActwk)[13];
  xpos = pActwk->xposi.w.h;
  xpos -= ((Sint16*)pActwk)[23];
  if (xpos < 0) xpos *= -1;
  if (xpos >= 128)
  {
    ((Sint32*)pActwk)[13] *= -1;
    pActwk->xposi.l += ((Sint32*)pActwk)[13];
    pActwk->actflg = pActwk->actflg ^ 1;
    pActwk->cddat = pActwk->cddat ^ 1;
    ((Sint16*)pActwk)[28] = 0;
  }
  ((Sint16*)pActwk)[28] += ((Sint16*)pActwk)[29];
  sinset(pActwk->actfree[11], &sSin, &sCos);
  lSin.w.h = sSin;
  lSin.w.l = 0;
  shift = ((Sint16*)pActwk)[30];
  while (shift--) lSin.l /= 2;
  lSin.l += ((Sint32*)pActwk)[12];
  pActwk->yposi.l = lSin.l;

  patchg(pActwk, chou_pchg);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);
}

void ene_ka(sprite_status* pActwk) {
  void(*tbl[5])(sprite_status*) =
  {
    &ka_init,
    &ka_move,
    &ka_turn,
    &ka_down,
    &ka_stop
  };

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);
}

void ka_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 16;
  pActwk->colino = 43;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  pa_set(pActwk, 0);

  if (pActwk->userflag.b.h == 0)
  {
    pActwk->patbase = e_ka_pat;
    ((Uint8***)pActwk)[13] = pchg_e;
    ((Sint32*)pActwk)[12] = -65536;
  }
  else
  {
    pActwk->patbase = b_ka_pat;
    ((Uint8***)pActwk)[13] = pchg_b;
    ((Sint32*)pActwk)[12] = -32768;
  }
  ka_move(pActwk);
}

void ka_move(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  Sint16 d0;

  if (editmode.w == 0)
  {
    pPlayerwk = &actwk[0];
    d0 = pPlayerwk->yposi.w.h;
    d0 -= pActwk->yposi.w.h;
    d0 += 48;
    if ((Uint16)d0 < 112)
    {
      d0 = pPlayerwk->xposi.w.h;
      d0 -= pActwk->xposi.w.h;
      d0 += 48;
      if ((Uint16)d0 < 96)
      {
        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 1;
        return;
      }
    }
  }
  do
  {
    pActwk->xposi.l += ((Sint32*)pActwk)[12];
    d0 = pActwk->xposi.w.h;
    d0 -= ((Sint16*)pActwk)[23];
    if (d0 < 0) d0 *= -1;
    if (d0 < 128) break;
    ((Sint32*)pActwk)[12] *= -1;
    pActwk->actflg = pActwk->actflg ^ 1;
    pActwk->cddat = pActwk->cddat ^ 1;
  } while (1);
  patchg(pActwk, ((Uint8***)pActwk)[13]);
}

void ka_turn(sprite_status* pActwk) {
  patchg(pActwk, ((Uint8***)pActwk)[13]);

}

void ka_down(sprite_status* pActwk) {
  Sint16 c;

  pActwk->yposi.w.h += 6;
  c = emycol_d(pActwk);
  if (c < -7)
  {
    c += 8;
    pActwk->yposi.w.h += c;
    pActwk->r_no0 += 2;
    if ((char)pActwk->actflg < 0) soundset(167);
  }
}

void ka_stop(sprite_status* pActwk) {
  if ((char)pActwk->actflg < 0) return;
  frameout_s0(pActwk);
}

void ene_kamemusi(sprite_status* pActwk) {
  void(*tbl[6])(sprite_status*) =
  {
    &kamemusi_init,
    &kamemusi_fall,
    &kamemusi_move,
    &kamemusi_stop,
    &kamemusi_stop1,
    &kamemusi_tama
  };

  if (pActwk->userflag.b.h == 1)
  {
    tama(pActwk);
    return;
  }

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);

}

void kamemusi_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 4;
  pActwk->colino = 44;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 15;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  pa_set(pActwk, 4);

  if (pActwk->userflag.b.h == 0)
  {
    pActwk->patbase = e_kamem_pat;
    ((Uint8***)pActwk)[13] = e_kamem_pchg;
    ((Sint32*)pActwk)[12] = -40960;
  }
  else
  {
    pActwk->patbase = b_kamem_pat;
    ((Uint8***)pActwk)[13] = b_kamem_pchg;
    ((Sint32*)pActwk)[12] = -20480;
  }
  kamemusi_fall(pActwk);
}

void kamemusi_fall(sprite_status* pActwk) {
  Sint16 d1;

  pActwk->yposi.l += 65536;
  d1 = emycol_d(pActwk);
  if (d1 < 0) pActwk->r_no0 += 2;
}

void kamemusi_move(sprite_status* pActwk) {
  Sint16 d0, d1;

  do
  {
    if (editmode.w == 0)
    {
      if (pActwk->userflag.b.h == 0)
      {
        if (((Sint16*)pActwk)[28])
        {
          --((Sint16*)pActwk)[28];
        }
        else
        {
          if (area(pActwk))
          {
            pActwk->r_no0 += 2;
            break;
          }
        }
      }
    }
    pActwk->xposi.l += ((Sint32*)pActwk)[12];
    d0 = pActwk->xposi.w.h;
    d0 -= ((Sint16*)pActwk)[23];
    if (d0 < 0) d0 *= -1;
    if (d0 < 128)
    {
      d1 = emycol_d(pActwk);
      if (d1 >= -7 && d1 < 8)
      {
        pActwk->yposi.w.h += d1;
        patchg(pActwk, ((Uint8***)pActwk)[13]);
        break;
      }
    }
    ((Sint32*)pActwk)[12] *= -1;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
  } while (1);
}

Sint16 area(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  Sint16 d0, d1;
  Sint16 carry_flag;

  pPlayerwk = &actwk[0];
  d0 = pPlayerwk->yposi.w.h;
  d0 -= pActwk->yposi.w.h;
  d0 += 80;
  if ((Uint16)d0 < 160)
  {
    carry_flag = 1;
    d0 = pPlayerwk->xposi.w.h;
    d0 -= pActwk->xposi.w.h;
    d1 = d0;
    d1 += 80;
    if ((Uint16)d1 < 160) carry_flag = 1;
    else carry_flag = 0;
  }
  else
  {
    carry_flag = 0;
  }
  return carry_flag;
}

void kamemusi_stop(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->mstno.b.h = 1;

  kamemusi_stop1(pActwk);
}

void kamemusi_stop1(sprite_status* pActwk) {
  patchg(pActwk, ((Uint8***)pActwk)[13]);

}

void kamemusi_tama(sprite_status* pActwk) {
  sprite_status* pTama1wk;
  sprite_status* pTama2wk;

  pActwk->r_no0 = 4;
  pActwk->mstno.b.h = 0;
  ((Sint16*)pActwk)[28] = 120;

  if (pActwk->userflag.b.h == 0)
  {
    if (actwkchk(&pTama1wk) == 0)
    {
      if (pActwk->actflg & 128)
      {
        soundset(160);
      }
      kamemusi_tama_init(pActwk, pTama1wk);
      pTama1wk->actfree[21] = 0;
      if (actwkchk(&pTama2wk) == 0)
      {
        kamemusi_tama_init(pActwk, pTama2wk);
        pTama2wk->actfree[21] = 255;
      }
    }
  }
}

void kamemusi_tama_init(sprite_status* pActwk, sprite_status* pTamawk) {
  pTamawk->actno = pActwk->actno;
  pTamawk->userflag.b.h = 1;
  pTamawk->sproffset = pActwk->sproffset;
  pTamawk->sprpri = pActwk->sprpri + 1;
  pTamawk->xposi.w.h = pActwk->xposi.w.h;
  pTamawk->yposi.w.h = pActwk->yposi.w.h - 10;

  pTamawk->actflg |= 128;
}

void tama(sprite_status* pActwk) {
  void(*tbl[2])(sprite_status*) =
  {
    &tama_init,
    &tama_move
  };

  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

void tama_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->actflg |= 128;
  pActwk->colino = 173;
  pActwk->sprhs = 8;
  pActwk->sprhsize = 8;
  pActwk->sprvsize = 8;
  pActwk->patbase = tama_pat;
  *(Sint32*)&pActwk->actfree[8] = 0;
  *(Sint32*)&pActwk->actfree[12] = 8192;

  if (pActwk->actfree[21] == 0)
  {
    *(Sint32*)&pActwk->actfree[0] = 0x20000;
    *(Sint32*)&pActwk->actfree[4] = -0x40000;
  }
  else
  {
    *(Sint32*)&pActwk->actfree[0] = -0x20000;
    *(Sint32*)&pActwk->actfree[4] = -0x40000;
  }
}

void tama_move(sprite_status* pActwk) {
  Sint16 d1;

  if (!(pActwk->actflg & 128))
  {
    frameout(pActwk);
    return;
  }
  d1 = emycol_d(pActwk);
  if (d1 < 0)
  {
    frameout(pActwk);
    return;
  }

  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[4];
  *(Sint32*)&pActwk->actfree[0] += *(Sint32*)&pActwk->actfree[8];
  *(Sint32*)&pActwk->actfree[4] += *(Sint32*)&pActwk->actfree[12];

  patchg(pActwk, tama_pchg);
}

void ene_tagame_a(sprite_status* pActwk) {
  void(*tbl[5])(sprite_status*) =
  {
    &tagame_init,
    &tagame_wait,
    &tagame_wait1,
    &tagame_jump,
    &tagame_jump1
  };

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);
}

void tagame_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->colino = 173;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 22;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[24] = pActwk->yposi.w.h;
  pa_set(pActwk, 3);

  if (pActwk->userflag.b.h == 0)
  {
    pActwk->patbase = e_tagame_pat;
    ((Uint8***)pActwk)[16] = e_tagame_pchg;
    ((Sint32*)pActwk)[13] = -245760;
    ((Sint32*)pActwk)[15] = 4096;
  }
  else
  {
    pActwk->patbase = b_tagame_pat;
    ((Uint8***)pActwk)[16] = b_tagame_pchg;
    ((Sint32*)pActwk)[13] = -196608;
    ((Sint32*)pActwk)[15] = 4096;
  }
  tagame_wait(pActwk);
}

void tagame_wait(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->mstno.w = 255;
  pActwk->colino = 0;
  pActwk->yposi.w.h = ((Sint16*)pActwk)[24];
}

void tagame_wait1(sprite_status* pActwk) {
  patchg(pActwk, ((Uint8***)pActwk)[16]);

}

void tagame_jump(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->mstno.w = 511;
  pActwk->colino = 46;
  pActwk->yposi.w.h = ((Sint16*)pActwk)[24];
  ((Sint32*)pActwk)[14] = ((Sint32*)pActwk)[13];
  if ((char)pActwk->actflg < 0)
  {
    soundset(162);
  }
  tagame_jump1(pActwk);
}

void tagame_jump1(sprite_status* pActwk) {
  pActwk->yposi.l += ((Sint32*)pActwk)[14];
  ((Sint32*)pActwk)[14] += ((Sint32*)pActwk)[15];

  if (pActwk->yposi.w.h > ((Sint16*)pActwk)[24])
  {
    pActwk->r_no0 = 2;
    if ((char)pActwk->actflg < 0)
    {
      soundset(162);
    }
  }
  patchg(pActwk, ((Uint8***)pActwk)[16]);
}

void ene_tama(sprite_status* pActwk) {
}

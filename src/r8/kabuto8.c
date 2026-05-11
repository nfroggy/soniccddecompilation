#include "../equ.h"
#include "kabuto8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

static void kabuto0(sprite_status* kabutowk); static void a_init(sprite_status* kabutowk); static void a_fall(sprite_status* kabutowk); static void a_move(sprite_status* kabutowk);
static void kabuto1(sprite_status* tunowk); static void b_init(sprite_status* tunowk); static void b_move(sprite_status* tunowk);

void kabuto(sprite_status* kabutowk) {
  if (kabutowk->userflag.b.l >= 0) kabuto0(kabutowk);
  else kabuto1(kabutowk);
}

#if defined(R82)
  #define SPRITE_KABUTO8_BASE 309
#elif defined(R83)
  #define SPRITE_KABUTO8_BASE 302
#else
  #define SPRITE_KABUTO8_BASE 300
#endif

static sprite_pattern cub00 = {
  1,
  { { -24, -16, 0, SPRITE_KABUTO8_BASE } }
};
sprite_pattern* pat_kabuto[1] = { &cub00 };

static void kabuto0(sprite_status* kabutowk) {
  void(*tbl[3])(sprite_status*) = { &a_init, &a_fall, &a_move };
  if (enemy_suicide(kabutowk)) return;
  tbl[kabutowk->r_no0 / 2](kabutowk);
  actionsub(kabutowk);
  frameout_s00(kabutowk, ((Sint16*)kabutowk)[26]);
}

static void a_init(sprite_status* kabutowk) {
  sprite_status* new_actwk;

  ((Sint16*)kabutowk)[26] = kabutowk->xposi.w.h;

  kabutowk->actflg |= 4;
  kabutowk->sprpri = 4;

  kabutowk->patbase = pat_kabuto;
  kabutowk->sprhsize = 24;
  kabutowk->sprvsize = 14;
  kabutowk->colino = 42;
  ((Sint32*)kabutowk)[12] = -20480;
  kabutowk->r_no0 += 2;
  if (actwkchk2(kabutowk, &new_actwk) != 0) {
    frameout(kabutowk);
    return;
  }
  new_actwk->actno = kabutowk->actno;
  new_actwk->userflag.b.h = kabutowk->userflag.b.h;
  new_actwk->userflag.b.l = -1;
  new_actwk->xposi.w.h = kabutowk->xposi.w.h;
  new_actwk->yposi.w.h = kabutowk->yposi.w.h;
  ((Uint16*)new_actwk)[33] = kabutowk - actwk;
  if (kabutowk->userflag.b.l != 0) {
    kabutowk->actflg |= 1;
    kabutowk->cddat |= 1;
    new_actwk->actflg |= 1;
    new_actwk->cddat |= 1;
  }
  a_fall(kabutowk);
}

static void a_fall(sprite_status* kabutowk) {
  Sint16 colli_data;

  kabutowk->yposi.l += 65536;
  if ((colli_data = emycol_d(kabutowk)) < 0) {
    kabutowk->yposi.w.h += colli_data;
    kabutowk->r_no0 += 2;
  }
}

static void a_move(sprite_status* kabutowk) {
  if ((char)kabutowk->actflg < 0 && !(++((Sint16*)kabutowk)[27] & 31)) {

    soundset(177);
  }
  kabutowk->xposi.l += ((Sint32*)kabutowk)[12];

  kabutowk->yposi.w.h += emycol_d(kabutowk);

  if (--((Sint16*)kabutowk)[23] < 0) {
    ((Sint16*)kabutowk)[23] = 409;
    ((Sint32*)kabutowk)[12] = -((Sint32*)kabutowk)[12];
  }

}

static Uint8 p00[4] = { 2, 0, 1, 255 };
static Uint8 p01[4] = { 2, 2, 3, 255 };
static Uint8* pchg[2] = { p00, p01 };
static sprite_pattern hea00 = {
  2,
  {
    { -8, -8, 0, SPRITE_KABUTO8_BASE + 2 },
    { -12, -16, 0, SPRITE_KABUTO8_BASE + 1 }
  }
};
static sprite_pattern hea01 = {
  2,
  {
    { -8, -8, 0, SPRITE_KABUTO8_BASE + 2 },
    { -12, -17, 16, SPRITE_KABUTO8_BASE + 1 }
  }
};
static sprite_pattern hea02 = {
  2,
  {
    { -4, -2, 0, SPRITE_KABUTO8_BASE + 3 },
    { -12, -16, 0, SPRITE_KABUTO8_BASE + 1 }
  }
};
static sprite_pattern hea03 = {
  2,
  {
    { -4, -2, 0, SPRITE_KABUTO8_BASE + 3 },
    { -12, -17, 16, SPRITE_KABUTO8_BASE + 1 }
  }
};
static sprite_pattern* pat[4] = { &hea00, &hea01, &hea02, &hea03 };

void kabuto1(sprite_status* tunowk) {
  void(*tbl[2])(sprite_status*) = { &b_init, &b_move };
  tbl[tunowk->r_no0 / 2](tunowk);
  actionsub(tunowk);
}

static void b_init(sprite_status* tunowk) {
  tunowk->r_no0 += 2;
  tunowk->actflg |= 4;
  tunowk->sprpri = 3;
  tunowk->patbase = pat;
  tunowk->sprhsize = 12;
  tunowk->sprvsize = 16;
  tunowk->colino = 171;
}

static void b_move(sprite_status* tunowk) {
  sprite_status* bodywk;

  bodywk = &actwk[((Uint16*)tunowk)[33]];
  if (bodywk->actno != 36) { frameout(tunowk); return; }
  if (tunowk->userflag.b.h) goto label1;

  if (--((Sint16*)tunowk)[23] < 0) {
    ((Sint16*)tunowk)[23] = 60;
    tunowk->mstno.b.h ^= 1;
label1:
    ((Sint16*)tunowk)[24] = -25;
    ((Sint16*)tunowk)[25] = 0;
    if (tunowk->mstno.b.h) {
      ((Sint16*)tunowk)[24] = -19;
      ((Sint16*)tunowk)[25] = -15;
    }
    if (tunowk->actflg & 1) {
      ((Sint16*)tunowk)[24] = -((Sint16*)tunowk)[24];
    }
  }

  tunowk->xposi.w.h = bodywk->xposi.w.h + ((Sint16*)tunowk)[24];
  tunowk->yposi.w.h = bodywk->yposi.w.h + ((Sint16*)tunowk)[25];

  patchg(tunowk, pchg);
}

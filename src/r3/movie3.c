#include "../equ.h"
#include "movie3.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"

#if defined(R33C) || defined(R33D)
  #define SPRITE_MOVIE3_BASE 320
#else
  #define SPRITE_MOVIE3_BASE 316
#endif

static void die(sprite_status* moviewk);
static void m_init(sprite_status* moviewk);
static void m_wait(sprite_status* moviewk);
static void m_die(sprite_status* moviewk);
static void m_baku(sprite_status* moviewk);
static void m1wait(sprite_status* moviewk);
static void sub(sprite_status* moviewk);
static void s_init(sprite_status* moviewk);
static void s_move(sprite_status* moviewk);

static Uint8 pchg00[6] = { 0, 2, 1, 3, 1, 255 };
static Uint8 pchg01[58] = { 0, 4, 4, 1, 4, 4, 1, 4, 4, 1, 4, 4, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 255 };
static Uint8* pchg[2] = {
  pchg00,
  pchg01
};
static sprite_pattern mov00 = { 1, { { -12, -12, 0, SPRITE_MOVIE3_BASE } } };
static sprite_pattern pat00 = { 1, { { -16, -20, 0, 0 } } };
static sprite_pattern mov01 = { 1, { { -8, -4, 0, SPRITE_MOVIE3_BASE + 1 } } };
static sprite_pattern mov02 = { 1, { { -8, -4, 0, SPRITE_MOVIE3_BASE + 2 } } };
static sprite_pattern mov03 = { 1, { { -20, -24, 0, SPRITE_MOVIE3_BASE + 3 } } };
static sprite_pattern mov04 = { 1, { { -20, -24, 0, SPRITE_MOVIE3_BASE + 4 } } };
sprite_pattern* pat_movie[6] = { &mov00, &pat00, &mov01, &mov02, &mov03, &mov04 };
static char tbl0[37] = {
    1,   0,   0,
    5, -18, -10,
   10, -10,  10,
   15,   0, -18,
   20, -10,  18,
   22,   8,  23,
   25,  13, -10,
   28,  -3, -25,
   30,  10,  20,
   32, -10,   2,
   35,  13, -10,
   40, -10,  10,
   -1
};

void movie(sprite_status* moviewk) {
  void(*act_tbl[5])(sprite_status*) = { &m_init, &m_wait, &m_die, &m_baku, &m1wait };

  if (moviewk->userflag.b.h != 0) { sub(moviewk); return; }

  act_tbl[moviewk->r_no0 / 2](moviewk);
  actionsub(moviewk);
  if (moviewk->r_no0 > 2) return;
  frameout_s(moviewk);
  if (moviewk->actno == 0) divdevset(4);
}

static void die(sprite_status* moviewk) {
  frameout(moviewk);
}

static void m_init(sprite_status* moviewk) {
  sprite_status* new_actwk;

  if (projector_flag != 0) { die(moviewk); return; }
  divdevset(5);

  moviewk->r_no0 += 2;
  moviewk->actflg |= 4;
  moviewk->sprpri = 4;
  moviewk->sprhs = moviewk->sprhsize = moviewk->sprvsize = 12;
  moviewk->colino = 251;

  moviewk->patbase = pat_movie;
  ((char**)moviewk)[12] = tbl0;

  if (actwkchk(&new_actwk) != 0) { die(moviewk); return; }
  new_actwk->actno = moviewk->actno;
  new_actwk->xposi.w.h = moviewk->xposi.w.h - 21;
  new_actwk->yposi.w.h = moviewk->yposi.w.h - 7;
  new_actwk->userflag.b.h = -1;
  ((Uint16*)new_actwk)[33] = moviewk - actwk;

  if (actwkchk(&new_actwk) != 0) { die(moviewk); return; }
  new_actwk->actno = moviewk->actno;
  new_actwk->xposi.w.h = moviewk->xposi.w.h - 88;
  new_actwk->yposi.w.h = moviewk->yposi.w.h - 4;
  new_actwk->userflag.b.h = 1;
  ((Uint16*)new_actwk)[33] = moviewk - actwk;

  if (actwkchk(&new_actwk) != 0) { die(moviewk); return; }
  new_actwk->actno = 57;
  new_actwk->xposi.w.h = moviewk->xposi.w.h - 88;
  new_actwk->yposi.w.h = moviewk->yposi.w.h - 24;
  new_actwk->userflag.b.h = -128;
  ((Uint16*)new_actwk)[33] = moviewk- actwk;

  if (actwkchk(&new_actwk) != 0) { die(moviewk); return; }
  new_actwk->actno = 57;
  new_actwk->xposi.w.h = moviewk->xposi.w.h - 98;
  new_actwk->yposi.w.h = (Sint32)moviewk->yposi.w.h;
  new_actwk->userflag.b.h = -127;
  ((Uint16*)new_actwk)[33] = moviewk - actwk;
}

static void m_wait(sprite_status* moviewk) {
  if (moviewk->colicnt != 0) {
    moviewk->colino = moviewk->colicnt = 0;
    moviewk->r_no0 += 2;
  }
  hitchk(moviewk, &actwk[0]);
}

static void m_die(sprite_status* moviewk) {
  moviewk->r_no0 += 2;
  moviewk->patno = 1;
  moviewk->actfree[21] = 255;
  divdevset(4);

  if (hitchk(moviewk, &actwk[0]) != 0) ride_on_clr(moviewk, &actwk[0]);
  m_baku(moviewk);
}

static void m_baku(sprite_status* moviewk) {
  char *data_tbl, time_limit, x_add, y_add;
  sprite_status* new_actwk;

  data_tbl = ((char**)moviewk)[12];
  if ((time_limit = data_tbl[0]) < 0) {
    moviewk->r_no0 += 2;
    ((Sint16*)moviewk)[23] = 60;
    return;
  }

  if (++moviewk->actfree[0] != time_limit) return;

  x_add = data_tbl[1], y_add = data_tbl[2];
  ((char**)moviewk)[12] = data_tbl + 3;

  if (actwkchk(&new_actwk) != 0) return;
  new_actwk->actno = 24;
  new_actwk->r_no1 = 1;
  new_actwk->xposi.w.h = moviewk->xposi.w.h + (Sint16)x_add;
  new_actwk->yposi.w.h = moviewk->yposi.w.h + (Sint16)y_add;
  soundset(158);
}

static void m1wait(sprite_status* moviewk) {
  if (--((Sint16*)moviewk)[23] == 0) {
    projector_flag = 255;
    die(moviewk);
  }
}

static void sub(sprite_status* moviewk) {
  sprite_status* parent;

  parent = &actwk[((Uint16*)moviewk)[33]];
  if (parent->actno != 56) { die(moviewk); return; }
  if (parent->actfree[21] != 0) { die(moviewk); return; }

  if (moviewk->r_no0 == 0) { s_init(moviewk); return; }
  s_move(moviewk);
}

static void s_init(sprite_status* moviewk) {
  moviewk->r_no0 += 2;
  moviewk->actflg |= 4;
  moviewk->sprpri = 4;

  moviewk->patbase = pat_movie;
  if (moviewk->userflag.b.h < 0) {
    moviewk->sprhs = moviewk->sprhsize = 8;
    moviewk->sprvsize = 4;
    moviewk->mstno.b.h = 0;
  } else {
    moviewk->sprhs = moviewk->sprhsize = 20;
    moviewk->sprvsize = 24;
    moviewk->mstno.b.h = 1;
  }
}

static void s_move(sprite_status* moviewk) {
  patchg(moviewk, pchg);
  actionsub(moviewk);
}

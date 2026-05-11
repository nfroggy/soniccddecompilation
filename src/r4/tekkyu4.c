#include "../equ.h"
#include "tekkyu4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R41C)
  #define SPRITE_TEKKYU4_BASE 449
#elif defined(R41D)
  #define SPRITE_TEKKYU4_BASE 435
#elif defined(R42A)
  #define SPRITE_TEKKYU4_BASE 469
#else
  #define SPRITE_TEKKYU4_BASE 474
#endif

sprite_pattern tekkyu4_pat0 = {
  1,
  { { -16, -16, 0, SPRITE_TEKKYU4_BASE } }
};
sprite_pattern* pat_tekkyu4[1] = { &tekkyu4_pat0 };

void tekkyu4(sprite_status* pActwk) {
  sprite_status* pMainactwk;

  void(*tekkyu4_acttbl[2])(sprite_status*) = {
    &tekkyu4init,
    &tekkyu4move
  };

if (((Sint16*)pActwk)[33]) {
    pMainactwk = &actwk[((Sint16*)pActwk)[33]];
    if (pMainactwk->actno != 56) {
      frameout(pActwk);
      return;
    }

  }
  tekkyu4_acttbl[pActwk->r_no0 / 2](pActwk);

}

void tekkyu4init(sprite_status* pActwk) {
  sprite_status *pNewactwk1, *pNewactwk2, *pNewactwk3;
  Sint16 wD0;

  ((Sint16*)pActwk)[28] = pActwk->xposi.w.h;

  if (actwkchk(&pNewactwk3) != 0) {
    frameout(pActwk);
    return;
  }

  pNewactwk3->actno = pActwk->actno;
  pNewactwk3->r_no0 = 2;
  ((Sint16*)pNewactwk3)[33] = pActwk - actwk;

  if (actwkchk(&pNewactwk2) != 0) {
    frameout(pActwk);
    return;
  }

  pNewactwk2->actno = pActwk->actno;
  pNewactwk2->r_no0 = 2;
  ((Sint16*)pNewactwk2)[33] = pActwk - actwk;

  if (actwkchk(&pNewactwk1) != 0) {
    frameout(pActwk);
    return;
  }

  pNewactwk1->actno = pActwk->actno;
  pNewactwk1->r_no0 = 2;
  ((Sint16*)pNewactwk1)[33] = pActwk - actwk;

  wD0 = -2048;

  if (pActwk->userflag.b.h & 128) wD0 *= -1;

  ((Sint16*)pNewactwk1)[25] = (Sint32)wD0;
  ((Sint16*)pNewactwk2)[25] = wD0 * 2;
  ((Sint16*)pNewactwk3)[25] = wD0 * 3;

  ((Sint16*)pActwk)[26] = 256;
  if (pActwk->userflag.b.h & 128) ((Sint16*)pActwk)[26] *= -1;

  if ((wD0 = pActwk->userflag.b.h) & 128) wD0 *= -1;

  ((Sint16*)pActwk)[27] = wD0 & 127;

  pActwk->sprpri = 1;
  pNewactwk1->sprpri = 2;
  pNewactwk2->sprpri = 3;
  pNewactwk3->sprpri = 4;

  act_init_sub(pActwk, pActwk);
  act_init_sub(pActwk, pNewactwk1);
  act_init_sub(pActwk, pNewactwk2);
  act_init_sub(pActwk, pNewactwk3);
}

void act_init_sub(sprite_status* pActwk, sprite_status* pNewactwk) {
  pNewactwk->actflg |= 4;
  pNewactwk->colino = 169;
  pNewactwk->sprhs = 16;
  pNewactwk->sprhsize = 16;
  pNewactwk->sprvsize = 16;
  pNewactwk->sproffset = 880;
  pNewactwk->patbase = pat_tekkyu4;
  pNewactwk->r_no0 = 2;
  ((Sint16*)pNewactwk)[23] = pActwk->xposi.w.h;
  ((Sint16*)pNewactwk)[24] = pActwk->yposi.w.h;
  ((Sint16*)pNewactwk)[26] = ((Sint16*)pActwk)[26];
  ((Sint16*)pNewactwk)[27] = ((Sint16*)pActwk)[27];
}

void tekkyu4move(sprite_status* pActwk) {
  Uint16 wD0, wD1;

  ((Uint16*)pActwk)[25] += ((Uint16*)pActwk)[26];

sinset(((Sint16*)pActwk)[25] >> 8, (Sint16*)&wD0, (Sint16*)&wD1);

wD0 = (Sint16)wD0 >> ((Uint16*)pActwk)[27];
  wD1 = (Sint16)wD1 >> ((Uint16*)pActwk)[27];

pActwk->xposi.w.h = ((Sint16*)pActwk)[23] + (Sint16)wD1;
  pActwk->yposi.w.h = ((Sint16*)pActwk)[24] + (Sint16)wD0;

  actionsub(pActwk);

  if (!((Sint16*)pActwk)[33]) frameout_s(pActwk);

}

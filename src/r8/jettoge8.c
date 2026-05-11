#include "../equ.h"
#include "jettoge8.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"

#if defined(R83)
  #define SPRITE_JETTOGE8_BASE 505
#else
  #define SPRITE_JETTOGE8_BASE 475
#endif

static char p00[4] = { 1, 0, 1, -1 };
static char* pchg[1] = { p00 };
static sprite_pattern pat00 =
{
  1,
  { { -16, -76, 0, SPRITE_JETTOGE8_BASE } }
};
static sprite_pattern pat01 =
{
  1,
  { { -16, -76, 0, SPRITE_JETTOGE8_BASE + 1 } }
};
sprite_pattern* pat_jettoge[2] =
{
  &pat00,
  &pat01
};

void jettoge(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      a_init(actionwk);
    case 2:
      a_move(actionwk);
      break;
  }
}

void a_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->patbase = pat_jettoge;
  actionwk->sproffset = 33856;
  ((Sint16*)actionwk)[26] = actionwk->xposi.w.h;
  actionwk->actfree[9] = 177;
  ((Sint32*)actionwk)[12] = 20480;
  actionwk->sprvsize = 76;
  actionwk->sprpri = 1;
  actionwk->sprhsize = 16;
}

void a_move(sprite_status* actionwk) {
  Sint16 t;

  actionwk->xposi.l += ((Sint32*)actionwk)[12];
  if (prio_flag == 0)
    actionwk->colino = actionwk->actfree[9];
  else
    actionwk->colino = 0;
  t = ((Sint16*)actionwk)[23]--;
  if (t <= 0)
  {
    ((Sint16*)actionwk)[23] = 409;
    ((Sint32*)actionwk)[12] = -((Sint32*)actionwk)[12];

    actionwk->actflg ^= 1;
    actionwk->cddat ^= 1;
  }
  patchg(actionwk, (Uint8**)pchg);
  actionsub(actionwk);
  frameout_s00(actionwk, ((Sint16*)actionwk)[26]);

}

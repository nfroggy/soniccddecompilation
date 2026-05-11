#include "../equ.h"
#include "tekkyu1.h"
#include "../action.h"
#include "../actset.h"

#if defined (R42A)
  #define SPRITE_TEKKYU1_BASE 532
#else
  #define SPRITE_TEKKYU1_BASE 505
#endif

static sprite_pattern pat0 = {
  1,
  { { -16, -16, 0, SPRITE_TEKKYU1_BASE } }
};
static sprite_pattern* pat_tekkyu1[1] = { &pat0 };

void tekkyu1(sprite_status* pActwk) {
  if (pActwk->r_no0 == 0) {

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 169;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 880;
    pActwk->patbase = pat_tekkyu1;
  }
  actionsub(pActwk);
  frameout_s(pActwk);
}

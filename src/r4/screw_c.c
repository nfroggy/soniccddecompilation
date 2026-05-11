#include "../equ.h"
#include "screw_a.h"
#include "../action.h"

#if defined(R41C)
  #define SPRITE_SCREWC_BASE 441
#elif defined(R41D)
  #define SPRITE_SCREWC_BASE 442
#else
  #define SPRITE_SCREWC_BASE 432
#endif

sprite_pattern pat_screw00 = {
  1,
  { { -16, -8, 0, SPRITE_SCREWC_BASE } }
};
sprite_pattern pat_screw01 = {
  1,
  { { -16, -8, 0, SPRITE_SCREWC_BASE + 1 } }
};
sprite_pattern pat_screw02 = {
  1,
  { { -8, -16, 0, SPRITE_SCREWC_BASE + 2 } }
};
sprite_pattern pat_screw03 = {
  1,
  { { -8, -16, 0, SPRITE_SCREWC_BASE + 3 } }
};
sprite_pattern* pat_screw0[2] = {
  &pat_screw00,
  &pat_screw01
};
sprite_pattern* pat_screw1[2] = {
  &pat_screw02,
  &pat_screw03
};

void screw(sprite_status* pActwk) {
  sprite_status* pCallactwk;

  if (!pActwk->r_no0) {

    pCallactwk = &actwk[((Sint16*)pActwk)[23]];
    pActwk->actfree[2] = pCallactwk->actno;
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 17280;

    if (pActwk->userflag.b.h & 128)
      ++pActwk->patno;
    if (pActwk->userflag.b.h & 2) {
      pActwk->sprhsize = 8;
      pActwk->sprvsize = 16;
      pActwk->patbase = pat_screw1;
    } else {
      pActwk->sprhsize = 16;
      pActwk->sprvsize = 8;
      pActwk->patbase = pat_screw0;
    }
    if (pActwk->userflag.b.h & 1) {
      pActwk->actflg |= 3;
      pActwk->cddat |= 3;
    }
  }

  pCallactwk = &actwk[((Sint16*)pActwk)[23]];

  if (pActwk->actfree[2] == pCallactwk->actno) {
    if (!(pActwk->userflag.b.l & 128)) {
      if (!pActwk->actfree[20]) {
        if (switchflag[pActwk->userflag.b.l]) {
          pActwk->actfree[20] = 255;
          pActwk->patno ^= 1;
        }
      }
    }

    actionsub(pActwk);
    return;
  }

  frameout(pActwk);
}

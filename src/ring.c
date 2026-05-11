#include "equ.h"
#include "ring.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "etc.h"
#include "loader2.h"
#include "playsub.h"

#if defined (R31) || defined (R32)
  #define SPRITE_RING_BASE 321
#elif defined (R33)
  #define SPRITE_RING_BASE 325
#elif defined (R6)
  #define SPRITE_RING_BASE 320
#elif defined (R81)
  #define SPRITE_RING_BASE 310
#elif defined (R82)
  #define SPRITE_RING_BASE 319
#elif defined (R83)
  #define SPRITE_RING_BASE 312
#else
  #define SPRITE_RING_BASE 362
#endif

sprite_pattern ringsp0 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE } }
};
sprite_pattern ringsp1 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 1 } }
};
sprite_pattern ringsp2 = {
  1,
  { { -4, -8, 0, SPRITE_RING_BASE + 2 } }
};
sprite_pattern ringsp3 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 3 } }
};
sprite_pattern ringsp4 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 4 } }
};
sprite_pattern ringsp5 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 5 } }
};
sprite_pattern ringsp6 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 6 } }
};
sprite_pattern ringsp7 = {
  1,
  { { -8, -8, 0, SPRITE_RING_BASE + 7 } }
};
sprite_pattern ringsp8;
sprite_pattern* ringpat[9] = {
  &ringsp0,
  &ringsp1,
  &ringsp2,
  &ringsp3,
  &ringsp4,
  &ringsp5,
  &ringsp6,
  &ringsp7,
  &ringsp8
};
Uint8 ringchg0[6] = { 5, 4, 5, 6, 7, 252 };
Uint8 ringchg1[6] = { 7, 0, 1, 2, 3, 255 };
Uint8 ringchg2[6] = { 7, 0, 1, 2, 3, 255 };
Uint8* ringchg[3] = {
  ringchg0,
  ringchg1,
  ringchg2
};

void ring(sprite_status* pActwk) {
  void(*ring_move_tbl[5])(sprite_status*) = {
    &ringinit,
    &ringmove,
    &ringget,
    &ringdie,
    &ringerase
  };

  ring_move_tbl[pActwk->r_no0 / 2](pActwk);
}

void ringinit(sprite_status* pActwk) {
  char ringtbl[32] = {
     16,   0,
     24,   0,
     32,   0,
      0,  16,
      0,  24,
      0,  32,
     16,  16,
     24,  24,
     32,  32,
    -16,  16,
    -24,  24,
    -32,  32,
     16,   8,
     24,  16,
    -16,   8,
    -24,  16
  };
  Sint32 fw_index;
  Sint32 i;
  char ring_counter;
  Sint16 d0, d1, d2, d3, d5, d6;
  char d4;
  sprite_status* new_actwk;

  d1 = time_flag & 127;
  if (time_flag & 128) {
    d1 += -time_item;
    if (d1 < 0) d1 = 0;
    else if (d1 >= 3) d1 = 2;
  }
  fw_index = d1 + pActwk->cdsts * 3;

  d4 = flagwork[fw_index];
  d1 = pActwk->userflag.b.h & 7;
  if (d1 == 7) d1 = 6;
  ring_counter = 1;
  d0 = ((Uint8)pActwk->userflag.b.h >> 4) * 2;
  d5 = ringtbl[d0];
  d6 = ringtbl[d0 + 1];
  new_actwk = pActwk;
  d2 = pActwk->xposi.w.h;
  d3 = pActwk->yposi.w.h;

  d0 = time_flag & 127;
  if (time_flag & 128) {
    d0 += -time_item;
    if (d0 < 0) d0 = 0;
    else if (d0 >= 3) d0 = 2;
  }
  i = fw_index + 1;
  do {
    if (flagwork[--i] & (1 << (ring_counter - 1))) goto label3;
  } while (--d0 != -1);

  flagwork[fw_index] &= 127;
  goto label2;

label1:
  d0 = time_flag & 127;
  if (time_flag & 128) {
    d0 += -time_item;
    if (d0 < 0) d0 = 0;
    else if (d0 >= 3) d0 = 2;
  }
  i = fw_index + 1;
  do {
    if (flagwork[--i] & (1 << (ring_counter - 1))) goto label3;
  } while (--d0 != -1);

  flagwork[fw_index] &= 127;
  if (actwkchk2(pActwk, &new_actwk) == 0) {
label2:
    new_actwk->actno = 16;
    new_actwk->r_no0 = 2;
    new_actwk->xposi.w.h = d2;
    ((Sint16*)new_actwk)[27] = pActwk->xposi.w.h;
    new_actwk->yposi.w.h = d3;
    new_actwk->patbase = ringpat;
    new_actwk->sproffset = 42926;
    new_actwk->sprpri = 2;
    if (stageno.b.h == 6) {
      new_actwk->sprpri = 0;
      new_actwk->userflag.b.l = pActwk->userflag.b.l;
      if (new_actwk->userflag.b.l) {
        new_actwk->sproffset &= 127;
        new_actwk->sprpri = 2;
      }
    }
    new_actwk->actflg = 4;
    new_actwk->colino = 71;
    new_actwk->sprhsize = 8;
    new_actwk->sprvsize = 8;
    new_actwk->cdsts = pActwk->cdsts;
    new_actwk->actfree[10] = ring_counter;
    new_actwk->mstno.b.h = 1;
label3:
    ++ring_counter;
    d2 += d5;
    d3 += d6;
    if (--d1 != -1) goto label1;
  }
  d0 = time_flag & 127;
  if (time_flag & 128) {
    d0 += -time_item;
    if (d0 < 0) d0 = 0;
    else if (d0 >= 3) d0 = 2;
  }
  i = fw_index + 1;
  do {
    if (flagwork[--i] & 1) {
      frameout(pActwk);
      return;
    }
  } while (--d0 != -1);

  ringmove(pActwk);
}

void ringmove(sprite_status* pActwk) {
  Uint16 d0;

  if (pActwk->actflg >= 0) {
    d0 = (((Uint16*)pActwk)[27] & 65408) - (((uint_union*)&scra_h_posit)->w.h - 128 & 65408);
    if (d0 > 640) {
      ringerase(pActwk);
      return;
    }
  }

  pActwk->patcnt = (Uint8)((systemtimer.b.b4 >> 3) & 3) + 1;
  pActwk->pattim = systemtimer.b.b4 & 7;
  patchg(pActwk, ringchg);
  actionsub(pActwk);
}

void ringget(sprite_status* pActwk) {
  Sint16 d0, d1;

  pActwk->r_no0 += 2;
  pActwk->colino = 0;
  pActwk->sprpri = 1;
  pActwk->mstno.b.h = 0;
  ringgetsub();

  d0 = pActwk->cdsts * 3;
  d1 = time_flag & 127;
  if (time_flag & 128) {
    d1 += -time_item;
    if (d1 < 0) d1 = 0;
    else if (d1 >= 3) d1 = 2;
  }

  flagwork[d0 + d1] |= 1 << (pActwk->actfree[10] - 1);
  ringdie(pActwk);
}

void ringgetsub(void) {
  ++plring;
  plring_f |= 1;
  if (plring >= 100) {
    if (plring_f2 & 2) {
      if (plring < 200) {
        soundset(149);
        return;
      }
      if (plring_f2 & 4) {
        soundset(149);
        return;
      }
      plring_f2 |= 4;
    }
    plring_f2 |= 2;
    ++pl_suu;
    ++pl_suu_f;
    sub_sync(122);
  }
  else {
    soundset(149);
  }

}

void ringdie(sprite_status* pActwk) {
  patchg(pActwk, ringchg);
  actionsub(pActwk);
}

void ringerase(sprite_status* pActwk) {
  frameout(pActwk);
}

void flyring(sprite_status* pActwk) {
  void(*flyring_move_tbl[5])(sprite_status*) = {
    &flyringinit,
    &flyringmove,
    &flyringget,
    &flyringdie,
    &flyringerase
  };

  flyring_move_tbl[pActwk->r_no0 / 2](pActwk);
}

void flyringinit(sprite_status* pActwk) {
  Sint16 d2, d3, d5;
  sprite_status* new_actwk;
  Uint16 d0, d1;
  short_union d4;

  new_actwk = pActwk;
  if ((d5 = plring) > 32) d5 = 32;
  --d5;
  d4.w = 648;
  goto label1;

  do {
    if (actwkchk(&new_actwk) != 0) break;
label1:
    new_actwk->actno = 17;
    new_actwk->r_no0 += 2;
    new_actwk->sprvsize = 8;
    new_actwk->sprhs = 8;
    new_actwk->xposi.w.h = pActwk->xposi.w.h;
    new_actwk->yposi.w.h = pActwk->yposi.w.h;
    new_actwk->patbase = ringpat;
    new_actwk->userflag.b.l = pActwk->userflag.b.l;
    new_actwk->sproffset = 42926;
    new_actwk->sprpri = 3;
    if (stageno.b.h == 6) {
      new_actwk->sprpri = 0;
      if (pActwk->userflag.b.l) {
        new_actwk->sprpri = 3;
        new_actwk->sproffset &= 127;
      }
    }
    new_actwk->actflg = 4;
    new_actwk->colino = 71;
    new_actwk->sprhsize = 8;
    new_actwk->sprvsize = 8;
    sys_pattim4 = 255;
    new_actwk->mstno.b.h = 2;

    if (d4.w >= 0) {
      sinset(d4.b.l, (Sint16*)&d0, (Sint16*)&d1);
      d2 = d0 << (d4.w >> 8);
      d3 = d1 << (d4.w >> 8);
      if ((d4.b.l += 16) > 0)
        if ((d4.w -= 128) < 0) d4.w = 648;
    }
    new_actwk->xspeed.w = d2;
    new_actwk->yspeed.w = d3;
    d2 *= -1;
    d4.w *= -1;
  } while (--d5 != -1);

  if (lpKeepWork->User & 1) {

plring /= 2;
  }
  else {
    plring = 0;
  }
  plring_f = 128;
  plring_f2 = 0;
  soundset(148);
  flyringmove(pActwk);
}

void flyringmove(sprite_status* pActwk) {
  Sint16 d1;

speedset2(pActwk);
  if ((pActwk->yspeed.w += 24) >= 0) {
    if (!((systemtimer.b.b4 + ((Uint32)(pActwk - actwk) / 68)) & 3)) {
      if ((d1 = emycol_d(pActwk)) < 0) {
        pActwk->yposi.w.h += d1;
        pActwk->yspeed.w -= pActwk->yspeed.w / 4;
        pActwk->yspeed.w *= -1;
      }
    }
  }
  if (sys_pattim4) {
    if (pActwk->yposi.w.h < scralim_down + 224) {
      patchg(pActwk, ringchg);
      actionsub(pActwk);
      return;
    }
  }
  flyringerase(pActwk);
}

void flyringget(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->colino = 0;
  pActwk->sprpri = 1;
  pActwk->mstno.b.h = 0;
  ringgetsub();
  flyringdie(pActwk);
}

void flyringdie(sprite_status* pActwk) {
  patchg(pActwk, ringchg);
  actionsub(pActwk);
}

void flyringerase(sprite_status* pActwk) {
  frameout(pActwk);
}

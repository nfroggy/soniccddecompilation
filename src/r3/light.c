#include "../equ.h"
#include "light.h"
#include "../action.h"

static void act_init(sprite_status* actionwk);

static sprite_pattern light_pat0 =
{
  1,
  { { -8, -8, 0, 570 } }
};
static sprite_pattern* light_pat[1] = { &light_pat0 };

void light(sprite_status* actionwk) {
  sprite_status* a1;

  switch (actionwk->r_no0)
  {
    case 0:
      act_init(actionwk);
    case 2:

      break;
    case 4:
      act_die(actionwk);
  }

a1 = &actwk[((Uint16*)actionwk)[29]];
  if (a1->actno != 54)
  {
    frameout(actionwk);
    return;
  }
  actionsub(actionwk);

}

static void act_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri = 3;
  actionwk->sprhsize = 8;
  actionwk->sprvsize = 8;
  actionwk->sproffset = 25662;
  actionwk->patbase = light_pat;
}

void act_die(sprite_status* actionwk) {
  frameout(actionwk);
}

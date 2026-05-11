#include "../equ.h"
#include "gate.h"
#include "../action.h"
#include "../playsub.h"
#include "../ridechk.h"

static void act_init(sprite_status* actionwk);
static void act_open(sprite_status* actionwk);
static void act_move(sprite_status* actionwk);

static sprite_pattern pat00 = { 1, { { 48, -16, 0, 619 } } };
static sprite_pattern pat01 = { 1, { { 32, -16, 0, 620 } } };
static sprite_pattern pat02 = { 1, { { 16, -16, 0, 621 } } };
static sprite_pattern pat03 = { 1, { { 0, -16, 0, 622 } } };
static sprite_pattern pat04 = { 1, { { -16, -16, 0, 623 } } };
static sprite_pattern pat05 = { 1, { { -32, -16, 0, 624 } } };
static sprite_pattern pat06 = { 1, { { -48, -16, 0, 625 } } };
static sprite_pattern pat07 = { 1, { { -64, -16, 0, 626 } } };
sprite_pattern* gate_pat[8] = {
  &pat00,
  &pat01,
  &pat02,
  &pat03,
  &pat04,
  &pat05,
  &pat06,
  &pat07
};
static char pchg0[10] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 7 };
static char* pchg[1] = { pchg0 };

void gate(sprite_status* actionwk) {
  if (actionwk->r_no0 == 4) {

    act_move(actionwk);
  }

  switch (actionwk->r_no0)
  {
    case 0:
      act_init(actionwk);
      break;
    case 2:
      act_open(actionwk);
    case 4:
      act_move(actionwk);
      break;
    case 6:
      act_init(actionwk);
      break;
  }
}

void act_init6(sprite_status* actionwk) {
  actionwk->actfree[0] = 5;
  actionwk->r_no0 = 0;
  act_init10(actionwk);
}

static void act_init(sprite_status* actionwk) {
  actionwk->actfree[0] = 7;
  act_init10(actionwk);
}

void act_init10(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri = 3;
  actionwk->sproffset = 1140;
  actionwk->patbase = gate_pat;
  actionwk->sprhsize = 72;
  actionwk->sprvsize = 16;
  act_open(actionwk);
}

static void act_open(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)pchg);
  if (actionwk->patno == actionwk->actfree[0])
  {
    actionwk->r_no0 += 2;
  }
}

static void act_move(sprite_status* actionwk) {
  if ((char)actionwk->actflg < 0)
    ridechk(actionwk, &actwk[0]);
  actionsub(actionwk);
}

#include "../equ.h"
#include "act51a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "boss_5.h"
#include "dai_rd5.h"
#include "kuzure5.h"
#include "shoot5.h"

void(*act_tbl[60])(sprite_status*) = {
  &play00,
  &play00,
  &baria,
  &noact,
  &noact,
  &test_act,
  &shooter,
  &noact,
  &noact,
  &spring,
  &exit2,
  &exit2_set,
  &futa,
  &mizukiri,
  &koma,
  &ring,
  &flyring,
  &noact,
  &noact,
  &bigring,
  &gene,
  &mosugu,
  &goal,
  &bakuha,
  &item,
  &item2,
  &iwa,
  &score,
  &noact,
  &noact,
  &flower,
  &egg5hibana,
  &egg5hari,
  &egg5belt,
  &egg5bakuha,
  &noact,
  &noact,
  &noact,
  &noact,
  &kuzure5,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &dair5,
  &noact,
  &egg5,
  &egg5meca1,
  &egg5meca2,
  &egg5meca3,
  &egg5pipe,
  &egg5catch,
  &egg5bomb,
  &egg5bomb2,
  &clear,
  &over,
  &title
};

void debugact(sprite_status* pActwk) {
  if (pActwk->xposi.w.h++ >= 300)
    pActwk->xposi.w.h = 0;
  if (pActwk->yposi.w.h++ >= 200)
    pActwk->yposi.w.h = 0;

  actionsub(pActwk);
}

void noact(sprite_status* pActwk) {
  frameout(pActwk);
}

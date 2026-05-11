#include "../equ.h"
#include "act81a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "dai8.h"
#include "et8.h"
#include "friend8.h"
#include "hachi8.h"
#include "haguruma.h"
#include "kabuto8.h"
#include "koma8.h"
#include "konbea.h"
#include "kuzure8.h"
#include "nokogiri.h"
#include "okusieso.h"
#include "piston.h"
#include "propera8.h"
#include "scarab.h"
#include "sw8.h"
#include "tobira.h"
#include "trap_r81.h"

void(*act_tbl[67])(sprite_status*) = {
  &play00,
  &play00,
  &baria,
  &noact,
  &noact,
  &test_act,
  &noact,
  &noact,
  &noact,
  &spring,
  &exit2,
  &exit2_set,
  &futa,
  &mizukiri,
  &koma8,
  &ring,
  &flyring,
  &noact,
  &marker,
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
  &noact,
  &sw,
  &haguruma,
  &noact,
  &kabuto,
  &kuzure8,
  &noact,
  &nokogiri,
  &propera,
  &tobira,
  &konbea,
  &noact,
  &dai8,
  &noact,
  &hachi8,
  &scarab,
  &noact,
  &noact,
  &friend,
  &noact,
  &piston,
  &okusieso,
  &togedair8,
  &anar8,
  &futagor8,
  &noact,
  &clear,
  &over,
  &title,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &et
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

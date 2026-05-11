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
#include "beam.h"
#include "bigbom8.h"
#include "dai8.h"
#include "dango8.h"
#include "et8.h"
#include "friend8.h"
#include "haguruma.h"
#include "jettoge8.h"
#include "kabuto8.h"
#include "koma8.h"
#include "kuzure8.h"
#include "legmeca8.h"
#include "okusieso.h"
#include "pipe8.h"
#include "piston.h"
#include "playsp8.h"
#include "shoot.h"
#include "shut.h"
#include "sw8.h"
#include "tobira.h"
#include "trap_r82.h"

void(*act_tbl[67])(sprite_status*) = {
  &play00,
  &play00,
  &baria,
  &noact,
  &noact,
  &test_act,
  &shooter,
  &noact,
  &chibi_coli,
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
  &beam,
  &noact,
  &flower,
  &pipe8,
  &sw,
  &haguruma,
  &bigbom,
  &kabuto,
  &kuzure8,
  &legmeca,
  &noact,
  &noact,
  &tobira,
  &noact,
  &jettoge,
  &dai8,
  &dango,
  &noact,
  &noact,
  &gater8,
  &noact,
  &friend,
  &togeita,
  &piston,
  &okusieso,
  &togedair8,
  &noact,
  &noact,
  &noact,
  &clear,
  &over,
  &title,
  &shut,
  &kaiten,
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

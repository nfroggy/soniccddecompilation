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
#include "beltsw5.h"
#include "buranko5.h"
#include "dai_rd5.h"
#include "denden.h"
#include "et5.h"
#include "friend5.h"
#include "hari5f.h"
#include "harir5.h"
#include "hashi5.h"
#include "hasira5.h"
#include "iwa5.h"
#include "iwa5roll.h"
#include "iwa5wave.h"
#include "kemusi.h"
#include "kowasi5.h"
#include "kumo.h"
#include "kuzure5.h"
#include "movie5.h"
#include "sasori.h"
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
  &denden,
  &sasori,
  &kemusi,
  &kumo,
  &iwa5,
  &hari5f,
  &harir5,
  &iwa5roll,
  &kuzure5,
  &iwa5wave,
  &buranko5,
  &hasira5,
  &movie,
  &friend,
  &et,
  &hashi5,
  &dair5,
  &beltsw5,
  &kowasi,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
  &noact,
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

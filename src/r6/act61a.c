#include "../types.h"
#include "act61a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player6.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "batta.h"
#include "beem6.h"
#include "block6.h"
#include "dair6.h"
#include "egg6.h"
#include "et6.h"
#include "friend6.h"
#include "hachi6.h"
#include "kdai6.h"
#include "minomusi.h"
#include "movie6.h"
#include "piston6.h"
#include "seesaw6.h"
#include "semi.h"
#include "shoot6.h"
#include "sw6.h"
#include "tobidai6.h"
#include "tobira6.h"
#include "trap_r6.h"

void(*act_tbl[60])(sprite_status*) = {
  &play00,
  &play00,
  &baria,
  &catapalt,
  &gas,
  &test_act,
  &shooter,
  &megami,
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
  &ball,
  &flower,
  &piston6,
  &beem6,
  &dair6,
  &tobidai6,
  &sw6,
  &tobira6,
  &noact,
  &optbr6,
  &kdai6,
  &friend,
  &block6,
  &eggz6,
  &seesaw6,
  &noact,
  &et,
  &movie,
  &batta,
  &hachi6,
  &semi,
  &minomusi,
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

void noact(sprite_status* pAct) {
  frameout(pAct);
}

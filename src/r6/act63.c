#include "../equ.h"
#include "act61a.h"
#include "../goal.h"
#include "../item.h"
#include "../player6.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "beem6.h"
#include "block6.h"
#include "boss_6.h"
#include "dair6.h"
#include "egg6.h"
#include "et6.h"
#include "friend6.h"
#include "kdai6.h"
#include "movie6.h"
#include "piston6.h"
#include "seesaw6.h"
#include "shoot6.h"
#include "sw6.h"
#include "tobidai6.h"
#include "tobira6.h"
#include "togebl6a.h"
#include "trap_r6.h"
#include "udblk6.h"

void(*act_tbl[64])(sprite_status*) = {
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
  &ball,
  &flower,
  &piston6,
  &beem6,
  &dair6,
  &tobidai6,
  &sw6,
  &tobira6,
  &udblk6,
  &optbr6,
  &kdai6,
  &friend,
  &block6,
  &eggz6,
  &seesaw6,
  &togeball,
  &et,
  &movie,
  &noact,
  &noact,
  &noact,
  &noact,
  &egg6boss,
  &egg6meca0,
  &egg6meca1,
  &egg6meca2,
  &egg6toge,
  &egg6gareki,
  &clear,
  &over,
  &title,
  &egg6yuka,
  &egg6door,
  &egg6bakuha,
  &egg6beam
};

void noact(sprite_status* pAct) {
  frameout(pAct);
}

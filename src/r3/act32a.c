#include "../equ.h"
#include "act31a.h"
#include "../action.h"
#include "../block.h"
#include "../player.h"
#include "../playsub.h"
#include "../goal.h"
#include "../item.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "banpa.h"
#include "bobin.h"
#include "bobinb.h"
#include "et3.h"
#include "friend3.h"
#include "ga3.h"
#include "kama.h"
#include "miracle.h"
#include "movie3.h"
#include "pocket.h"
#include "tentou.h"
#include "togebl3a.h"
#include "trap_r3.h"

void (*act_tbl[60])(sprite_status *) = {
    &play00,    &play00,   &baria,    &noact,   &noact,      &test_act,
    &noact,     &noact,    &noact,    &spring,  &exit2,      &exit2_set,
    &futa,      &mizukiri, &koma,     &ring,    &flyring,    &block,
    &marker,    &bigring,  &gene,     &mosugu,  &goal,       &bakuha,
    &item,      &item2,    &iwa,      &score,   &bobin,      &frip,
    &flower,    &harir3,   &frdr3,    &trapdr3, &for3,       &ene_kama,
    &tama_kama, &getdair3, &gandair3, &drumr3,  &bobinbreak, &banpa,
    &miracle,   &togeball, &ga,       &tentou,  &pocket,     &noact,
    &noact,     &noact,    &noact,    &noact,   &ball,       &noact,
    &et,        &movie,    &friend,   &clear,   &over,       &title};

void noact(sprite_status *pAct) { frameout(pAct); }

#include "../equ.h"
#include "act71a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "boss_7.h"
#include "branko7.h"
#include "chgban.h"
#include "chgwall7.h"
#include "emie7.h"
#include "friend7.h"
#include "gaitou73.h"
#include "hari73.h"
#include "movie7.h"
#include "tekkyu7.h"
#include "tekkyu7j.h"
#include "wall7.h"
#include "wall73.h"

void (*act_tbl[60])(sprite_status *) = {
    &play00, &play00,  &baria,    &ami,      &noact,     &test_act, &noact,
    &noact,  &noact,   &spring,   &exit2,    &exit2_set, &futa,     &mizukiri,
    &koma,   &ring,    &flyring,  &noact,    &noact,     &bigring,  &gene,
    &mosugu, &goal,    &bakuha,   &item,     &item2,     &iwa,      &score,
    &noact,  &noact,   &flower,   &branko7,  &noact,     &noact,    &noact,
    &wall7,  &tekkyu7, &tekkyu7j, &movie,    &friend,    &noact,    &noact,
    &wall73, &egg7,    &egg7jet,  &egg7beam, &msnc,      &msnc_ele, &msnc_bara,
    &hari73, &emie7,   &heart7,   &chgwall7, &gaitou73,  &hahen73,  &noact,
    &noact,  &clear,   &over,     &title};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

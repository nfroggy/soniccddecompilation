#include "../equ.h"
#include "act71a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../ring.h"
#include "../player.h"
#include "../playsub.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "branko7.h"
#include "chgban.h"
#include "chgwall7.h"
#include "dango7.h"
#include "et7.h"
#include "friend7.h"
#include "hotaru7.h"
#include "kabasira.h"
#include "kanabun.h"
#include "movie7.h"
#include "slight7.h"
#include "tekkyu7.h"
#include "tekkyu7j.h"
#include "wall7.h"

void (*act_tbl[60])(sprite_status *) = {
    &play00, &play00,   &baria,    &ami,      &noact,     &test_act, &noact,
    &gun7,   &hibana,   &spring,   &exit2,    &exit2_set, &futa,     &mizukiri,
    &koma,   &ring,     &flyring,  &noact,    &marker,    &bigring,  &gene,
    &mosugu, &goal,     &bakuha,   &item,     &item2,     &iwa,      &score,
    &chgban, &spring_r, &flower,   &branko7,  &kanabun,   &kabasira, &hotaru7,
    &wall7,  &tekkyu7,  &tekkyu7j, &movie,    &friend,    &et,       &dango,
    &ball,   &noact,    &noact,    &noact,    &noact,     &noact,    &noact,
    &noact,  &noact,    &noact,    &chgwall7, &noact,     &noact,    &slight7,
    &noact,  &clear,    &over,     &title};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

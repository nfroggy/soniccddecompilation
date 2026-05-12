#include "../equ.h"
#include "act41a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "awa.h"
#include "baneiwa.h"
#include "daid4.h"
#include "friend4.h"
#include "lrblk4.h"
#include "harir4.h"
#include "sw4.h"
#include "amenbo.h"
#include "kowasi4.h"
#include "kuzure4.h"
#include "playsub4.h"
#include "renketu4.h"
#include "screw_a.h"
#include "tagameb4.h"
#include "tekkyu.h"
#include "tekkyu1.h"
#include "tekkyu4.h"
#include "tobira4.h"
#include "tobiras4.h"
#include "tonbo.h"
#include "udblk4.h"
#include "wall1.h"
#include "walls.h"
#include "yago.h"

void (*act_tbl[80])(sprite_status *) = {
    &play00,  &play00,   &baria,   &wave,  &bou,       &test_act, &noact,
    &noact,   &noact,    &spring,  &exit2, &exit2_set, &futa,     &mizukiri,
    &koma,    &ring,     &flyring, &noact, &marker,    &bigring,  &gene,
    &mosugu,  &goal,     &bakuha,  &item,  &item2,     &iwa,      &score,
    &noact,   &noact,    &flower,  &awa,   &plawa,     &harir4,   &udblk4,
    &lrblk4,  &tobiras4, &noact,   &sw4,   &tobira4,   &noact,    &noact,
    &tonbo,   &amenbo,   &tagameb, &yago,  &friend4,   &noact,    &noact,
    &noact,   &walls,    &wall1,   &screw, &tekkyu,    &tekkyu1,  &tekkyu4,
    &kuzure4, &clear,    &over,    &title, &noact,     &renketu4, &noact,
    &noact,   &noact,    &noact,   &noact, &noact,     &noact,    &noact,
    &noact,   &baneiwa,  &daid4,   &noact, &noact,     &noact,    &noact,
    &noact,   &noact,    &kowasi4};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

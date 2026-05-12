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
#include "amenbo.h"
#include "awa.h"
#include "brunko4.h"
#include "escal4.h"
#include "et4.h"
#include "friend4.h"
#include "funsui4.h"
#include "harid4.h"
#include "harir4.h"
#include "kuzure4.h"
#include "playsub4.h"
#include "screw_a.h"
#include "sw4.h"
#include "swblk4.h"
#include "tagameb4.h"
#include "tekkyu.h"
#include "tobira4.h"
#include "tonbo.h"
#include "wall1.h"
#include "walls.h"
#include "yago.h"

void (*act_tbl[82])(sprite_status *) = {
    &play00,  &play00, &baria,   &wave,  &bou,       &test_act, &noact,
    &noact,   &noact,  &spring,  &exit2, &exit2_set, &futa,     &mizukiri,
    &koma,    &ring,   &flyring, &noact, &marker,    &bigring,  &gene,
    &mosugu,  &goal,   &bakuha,  &item,  &item2,     &iwa,      &score,
    &noact,   &noact,  &flower,  &awa,   &plawa,     &harir4,   &noact,
    &noact,   &noact,  &escal4,  &sw4,   &tobira4,   &brunko4,  &funsui4,
    &tonbo,   &amenbo, &tagameb, &yago,  &friend4,   &swblkr4,  &switchr4,
    &harid4,  &walls,  &wall1,   &screw, &tekkyu,    &noact,    &noact,
    &kuzure4, &clear,  &over,    &title, &noact,     &noact,    &noact,
    &noact,   &noact,  &noact,   &noact, &noact,     &noact,    &noact,
    &noact,   &noact,  &noact,   &noact, &noact,     &noact,    &noact,
    &noact,   &noact,  &noact,   &et,    &noact};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

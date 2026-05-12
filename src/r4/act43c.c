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
#include "boss_4.h"
#include "boss_4_2.h"
#include "friend4.h"
#include "playsub4.h"
#include "renketu4.h"
#include "tonbo.h"
#include "wall1.h"
#include "wall4.h"

void (*act_tbl[80])(sprite_status *) = {
    &play00,   &play00,  &baria,   &wave,    &noact,       &test_act, &noact,
    &noact,    &noact,   &spring,  &exit2,   &exit2_set,   &futa,     &mizukiri,
    &koma,     &ring,    &flyring, &noact,   &noact,       &bigring,  &gene,
    &mosugu,   &goal,    &bakuha,  &item,    &item2,       &iwa,      &score,
    &noact,    &noact,   &flower,  &awa,     &plawa,       &noact,    &noact,
    &noact,    &noact,   &noact,   &noact,   &noact,       &noact,    &noact,
    &tonbo,    &noact,   &noact,   &noact,   &friend4,     &noact,    &noact,
    &noact,    &noact,   &wall1,   &noact,   &noact,       &noact,    &noact,
    &noact,    &clear,   &over,    &title,   &noact,       &renketu4, &noact,
    &noact,    &noact,   &noact,   &noact,   &noact,       &noact,    &noact,
    &wall4,    &noact,   &noact,   &egg4air, &egg4airhead, &egg4,     &egg4meca,
    &egg4tama, &egg4awa, &noact};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

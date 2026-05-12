#include "../equ.h"
#include "act11a.h"
#include "../action.h"
#include "../dai_rd1.h"
#include "../enemy.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "boss_1.h"
#include "friend1.h"
#include "sikake.h"

void (*act_tbl[64])(sprite_status *) = {
    &play00,   &play00,   &baria,    &noact,        &noact,    &test_act,
    &noact,    &noact,    &noact,    &spring,       &exit2,    &exit2_set,
    &futa,     &mizukiri, &koma,     &ring,         &flyring,  &noact,
    &noact,    &bigring,  &gene,     &mosugu,       &goal,     &bakuha,
    &item,     &item2,    &iwa,      &score,        &noact,    &noact,
    &flower,   &noact,    &dodai,    &ene_kamemusi, &ene_tama, &friend,
    &noact,    &hariyama, &noact,    &tobita1,      &noact,    &egg1,
    &egg1body, &egg1leg1, &egg1leg2, &egg1leg3,     &egg1arm1, &egg1arm2,
    &egg1arm3, &egg1arm4, &noact,    &ball,         &noact,    &noact,
    &noact,    &noact,    &noact,    &clear,        &over,     &title,
    &ene_ka,   &ene_chou, &ene_ari,  &ene_tagame_a,
};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

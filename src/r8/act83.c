#include "../equ.h"
#include "act81a.h"
#include "../action.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../zone.h"
#include "../r3/bobin.h"
#include "boss_8.h"
#include "dai8.h"
#include "emie8.h"
#include "friend8.h"
#include "haguruma.h"
#include "hota8c.h"
#include "jettoge8.h"
#include "kabuto8.h"
#include "koma8.h"
#include "konbea83.h"
#include "kuzure8.h"
#include "nokogiri.h"
#include "piston.h"
#include "playsp8.h"
#include "shut.h"
#include "sw8.h"
#include "tobira.h"
#include "trap_r83.h"

void (*act_tbl[67])(sprite_status *) = {
    &play00,  &play00,   &baria,      &noact,    &noact,    &test_act,
    &noact,   &noact,    &chibi_coli, &spring,   &exit2,    &exit2_set,
    &futa,    &mizukiri, &koma8,      &ring,     &flyring,  &noact,
    &noact,   &bigring,  &gene,       &mosugu,   &goal,     &bakuha,
    &item,    &item2,    &iwa,        &score,    &bobin,    &noact,
    &flower,  &noact,    &sw,         &haguruma, &noact,    &kabuto,
    &kuzure8, &noact,    &nokogiri,   &noact,    &tobira,   &konbea,
    &jettoge, &dai8,     &noact,      &noact,    &noact,    &noact,
    &noact,   &friend,   &togeita,    &piston,   &noact,    &togedair8,
    &noact,   &noact,    &hotaru8,    &clear,    &over,     &title,
    &shut,    &kaiten,   &egg8,       &egg8hane, &egg8meca, &egg8hibana,
    &emie8};

void debugact(sprite_status *pActwk) {
    if (pActwk->xposi.w.h++ >= 300)
        pActwk->xposi.w.h = 0;
    if (pActwk->yposi.w.h++ >= 200)
        pActwk->yposi.w.h = 0;

    actionsub(pActwk);
}

void noact(sprite_status *pActwk) { frameout(pActwk); }

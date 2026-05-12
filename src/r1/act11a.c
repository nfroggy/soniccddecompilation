#include "../equ.h"
#include "act11a.h"
#include "../action.h"
#include "../block.h"
#include "../dai_k.h"
#include "../dai_rd1.h"
#include "../emie1.h"
#include "../enemy.h"
#include "../et1.h"
#include "../goal.h"
#include "../item.h"
#include "../player.h"
#include "../playsub.h"
#include "../ring.h"
#include "../score.h"
#include "../spring.h"
#include "../taki.h"
#include "../tree.h"
#include "../zone.h"
#include "friend1.h"
#include "kowasi1.h"
#include "kuzure.h"
#include "movie1.h"
#include "shoot1.h"
#include "sikake.h"

void (*act_tbl[64])(sprite_status *) = {&play00,
                                        &play00,
                                        &baria,
                                        &taki,
                                        &jisin,
                                        &noact,
                                        &shooter,
                                        &jisin_set,
                                        &dai_k,
                                        &spring,
                                        &exit2,
                                        &exit2_set,
                                        &futa,
                                        &mizukiri,
                                        &koma,
                                        &ring,
                                        &flyring,
                                        &block,
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
                                        &noact,
                                        &flower,
                                        &kuzureru_tikei,
                                        &dodai,
                                        &ene_kamemusi,
                                        (void (*)(sprite_status *)) & ene_tama,
                                        &friend,
                                        &noact,
                                        &hariyama,
                                        &noact,
                                        &tobita1,
                                        &noact,
                                        &kasoku,
                                        &noact,
                                        &hoshi,
                                        &et,
                                        &movie1,
                                        &emie1,
                                        &heart1,
                                        &ana,
                                        &noact,
                                        &kage,
                                        &ball,
                                        &vfuta,
                                        &kowasi,
                                        &noact,
                                        &noact,
                                        &noact,
                                        &clear,
                                        &over,
                                        &title,
                                        &ene_ka,
                                        &ene_chou,
                                        &ene_ari,
                                        &ene_tagame_a};

void debugact(sprite_status *pAct) {
    if (pAct->xposi.w.h++ >= 300)
        pAct->xposi.w.h = 0;
    if (pAct->yposi.w.h++ >= 200)
        pAct->yposi.w.h = 0;

    actionsub(pAct);
}

void noact(sprite_status *pAct) { frameout(pAct); }

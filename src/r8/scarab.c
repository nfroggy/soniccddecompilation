#include "../equ.h"
#include "scarab.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../player_work.h"
#include "../ridechk.h"
#include "../suicide.h"
#include <stddef.h>

#pragma pack(push, 1)
typedef struct {
    union {
        Uint8 *flag_work;
        struct {
            Uint16 parent_index;
            Sint16 origin_x;
        };
    };
    union {
        struct {
            Sint16 child_enemy_index;
            Sint16 child_item_index;
        };
        Sint32 x_velocity;
    };
    Sint16 carried_actor_index;
    Sint16 animation_timer;
    Sint16 hold_timer;
    Uint8 **pattern_change;
    Sint16 shell_actor_index;
} scarab_work;
#pragma pack(pop)

_Static_assert(sizeof(Uint8 *) == 4, "scarab_work pointer fields are 32-bit");
_Static_assert(sizeof(Uint8 **) == 4, "scarab_work pointer fields are 32-bit");
_Static_assert(offsetof(scarab_work, flag_work) == 0,
               "scarab_work.flag_work offset");
_Static_assert(offsetof(scarab_work, parent_index) == 0,
               "scarab_work.parent_index offset");
_Static_assert(offsetof(scarab_work, origin_x) == 2,
               "scarab_work.origin_x offset");
_Static_assert(offsetof(scarab_work, child_enemy_index) == 4,
               "scarab_work.child_enemy_index offset");
_Static_assert(offsetof(scarab_work, x_velocity) == 4,
               "scarab_work.x_velocity offset");
_Static_assert(offsetof(scarab_work, child_item_index) == 6,
               "scarab_work.child_item_index offset");
_Static_assert(offsetof(scarab_work, carried_actor_index) == 8,
               "scarab_work.carried_actor_index offset");
_Static_assert(offsetof(scarab_work, animation_timer) == 10,
               "scarab_work.animation_timer offset");
_Static_assert(offsetof(scarab_work, hold_timer) == 12,
               "scarab_work.hold_timer offset");
_Static_assert(offsetof(scarab_work, pattern_change) == 14,
               "scarab_work.pattern_change offset");
_Static_assert(offsetof(scarab_work, shell_actor_index) == 18,
               "scarab_work.shell_actor_index offset");
_Static_assert(sizeof(scarab_work) <= sizeof(((sprite_status *)0)->actfree),
               "scarab_work fits in actfree");

static inline scarab_work *scarab_work_get(sprite_status *pActwk) {
    return (scarab_work *)pActwk->actfree;
}

static void c_init(sprite_status *pActwk);
static void c_wait(sprite_status *pActwk);
static void c_make(sprite_status *pActwk);
static void ini_e_i(sprite_status *pActwk, sprite_status *pNewact);
static void c_move(sprite_status *pActwk);
static void enemy(sprite_status *pActwk);
static void e_init(sprite_status *pActwk);
static void e_move(sprite_status *pActwk);
static void turn(sprite_status *pActwk);
static void e1_check(sprite_status *pActwk);
static void e1_chk_patchg(sprite_status *pActwk);
static void e1_catch(sprite_status *pActwk);
static void e1_keep(sprite_status *pActwk);
static void e1_wait(sprite_status *pActwk);
static void iteminit(sprite_status *pActwk);
static void itemmove(sprite_status *pActwk);
static void itemmove2(sprite_status *pActwk);
static void itemmove3(sprite_status *pActwk);
static void itemget(sprite_status *pActwk);
static void ride_on_chk_i(sprite_status *pActwk, sprite_status *pPlayerwk);

static Uint8 pchg_n00[4] = {9, 0, 1, 255};
static Uint8 *pchg_n[1] = {pchg_n00};
static Uint8 pchg_o00[4] = {19, 0, 1, 255};
static Uint8 *pchg_o[1] = {pchg_o00};
static sprite_pattern pat00 = {1, {{-32, -12, 0, 402}}};
static sprite_pattern pat01 = {1, {{-32, -12, 0, 403}}};
static sprite_pattern pat02 = {1, {{-40, -12, 0, 404}}};
static sprite_pattern pat03 = {1, {{-24, -12, 0, 405}}};
static sprite_pattern pat04 = {1, {{-32, -12, 0, 406}}};
static sprite_pattern pat05 = {1, {{-32, -12, 0, 407}}};
sprite_pattern *pat_scarab[6] = {&pat00, &pat01, &pat02,
                                 &pat03, &pat04, &pat05};
static void (*c_act_tbl[3])(sprite_status *) = {&c_init, &c_wait, &c_move};
static void (*item_move_tbl[5])(sprite_status *) = {
    &iteminit, &itemmove, &itemget, &itemmove2, &itemmove3};
static void (*e_act_tbl[2])(sprite_status *) = {&e_init, &e_move};
static void (*e_move_tbl[3])(sprite_status *) = {&e1_check, &e1_keep, &e1_wait};
extern sprite_pattern *itempat[];
extern Uint8 *itemchg[];

void scarab(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);

    if ((Uint8)pActwk->userflag.b.h == 2) {

        if (actwk[work->parent_index].actno != 47) {

            frameout(pActwk);
            return;
        }

        item_move_tbl[pActwk->r_no0 / 2](pActwk);
    } else if ((Uint8)pActwk->userflag.b.h < 2) {

        enemy(pActwk);
    } else {

        c_act_tbl[pActwk->r_no0 / 2](pActwk);
    }
}

static void c_init(sprite_status *pActwk) {
    Uint8 flgwk;
    scarab_work *work = scarab_work_get(pActwk);

    pActwk->r_no0 += 2;
    flgwk = pActwk->cdsts;
    flgwk = (time_flag & 127) + flgwk * 3;
    work->flag_work = &flagwork[flgwk];

    c_wait(pActwk);
}

static void c_wait(sprite_status *pActwk) {
    Uint16 xwk, ywk;

    xwk = actwk[0].xposi.w.h - pActwk->xposi.w.h + 320;
    ywk = actwk[0].yposi.w.h - pActwk->yposi.w.h + 320;

    if (ywk < 640 && xwk < 640) {
        c_make(pActwk);
    } else {
        frameout_s(pActwk);
    }
}

static void c_make(sprite_status *pActwk) {
    Uint8 *pFlagWork, flg;
    sprite_status *pNewact;
    scarab_work *work = scarab_work_get(pActwk);

    pActwk->r_no0 += 2;
    pFlagWork = work->flag_work;

    if (!(*pFlagWork & 1)) {
        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout_s0(pActwk);
            return;
        }

        ini_e_i(pActwk, pNewact);
        work->child_enemy_index = pNewact - actwk;
        pNewact->userflag.b.h = (Uint8)(pActwk->userflag.b.h & 1);

        if (pActwk->userflag.b.h & 16) {
            pNewact->userflag.b.l = -1;
        } else {
            pNewact->userflag.b.l = 0;
        }
    }

    if (!(*pFlagWork & 2)) {
        flg = pActwk->userflag.b.l;

        if ((flg & 128) || flg > 7) {

            *pFlagWork |= 2;
        } else {

            if (actwkchk2(pActwk, &pNewact) != 0) {
                frameout_s0(pActwk);
                return;
            }

            ini_e_i(pActwk, pNewact);
            work->child_item_index = pNewact - actwk;
            pNewact->userflag.b.h = 2;
            pNewact->userflag.b.l = flg;
            pNewact->yposi.w.h -= 4;

            if (pActwk->userflag.b.h & 16) {
                pNewact->xposi.w.h += 24;
            } else {
                pNewact->xposi.w.h -= 24;
            }
        }
    }
}

static void ini_e_i(sprite_status *pActwk, sprite_status *pNewact) {
    scarab_work *new_work = scarab_work_get(pNewact);

    new_work->parent_index = pActwk - actwk;
    pNewact->actno = 47;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;

    new_work->carried_actor_index = -1;
}

static void c_move(sprite_status *pActwk) {
    Uint8 *pFlagWork;
    Sint16 idx;
    sprite_status *pChildAct;
    ushort_union flagwk;
    scarab_work *work = scarab_work_get(pActwk);

    pFlagWork = work->flag_work;

    if (!(*pFlagWork & 1)) {
        if ((idx = work->child_enemy_index) != 0) {
            pChildAct = &actwk[idx];

            if (pChildAct->actno != 47) {

                *pFlagWork |= 1;
                work->child_enemy_index = 0;

                if ((idx = work->child_item_index) != 0) {
                    pChildAct = &actwk[idx];
                    pChildAct->xspeed.w = 0;
                }

                if ((idx = scarab_work_get(pChildAct)->carried_actor_index) >=
                    0) {
                    pChildAct = &actwk[idx];
                    player_work_get(pChildAct)->status_flags &= 254;
                }
            }
        }
    }

    if (!(*pFlagWork & 2)) {
        if ((idx = work->child_item_index) != 0) {
            pChildAct = &actwk[idx];

            if (pChildAct->actno != 47) {

                *pFlagWork |= 2;
                work->child_item_index = 0;
            }
        }
    }

    if (*pFlagWork & 1) {
        flagwk.b.h = 255;
    } else {
        flagwk.b.h = 0;
    }

    if (*pFlagWork & 2) {
        flagwk.b.l = 255;
    } else {
        flagwk.b.l = 0;
    }

    if (flagwk.w == 65535) {
        frameout(pActwk);
        return;
    }

    flagwk.w == 0;

    if (!(work->child_enemy_index | work->child_item_index)) {
        frameout_s0(pActwk);
    }
}

static void enemy(sprite_status *pActwk) {
    sprite_status *pCtrlact;
    scarab_work *work = scarab_work_get(pActwk);

    if (pActwk->userflag.b.l == 1) {

        if (actwk[work->parent_index].actno != 47) {

            frameout(pActwk);
            return;
        }

        if (pActwk->r_no0) {
            actionsub(pActwk);
        } else {
            pActwk->r_no0 += 2;
            pActwk->sprpri = 3;
        }
    } else {
        pCtrlact = &actwk[work->parent_index];

        if (pCtrlact->actno != 47) {
            frameout(pActwk);
            return;
        }

        if (!enemy_suicide(pActwk)) {

            e_act_tbl[pActwk->r_no0 / 2](pActwk);
            actionsub(pActwk);
            frameout_s(pActwk);

            if (!pActwk->actno) {
                scarab_work_get(pCtrlact)->child_enemy_index = 0;
            }
        }
    }
}

static void e_init(sprite_status *pActwk) {
    sprite_status *pNewact;
    scarab_work *work = scarab_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprhs = pActwk->sprhsize = 32;
    pActwk->sprvsize = 11;
    pActwk->colino = 240;
    pActwk->sprpri = 1;
    pActwk->sproffset = 9279;
    pActwk->patbase = pat_scarab;
    work->origin_x = pActwk->xposi.w.h;
    work->animation_timer = 60;
    pActwk->yposi.w.h += emycol_d(pActwk);

    if (actwkchk2(pActwk, &pNewact) != 0) {
        frameout_s0(pActwk);
        return;
    }

    pNewact->actno = 47;
    pNewact->userflag.b.l = 1;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->actflg = pActwk->actflg;
    pNewact->sprhs = pActwk->sprhs;
    pNewact->sprhsize = pActwk->sprhsize;
    pNewact->sprvsize = pActwk->sprvsize;
    pNewact->sproffset = pActwk->sproffset;
    pNewact->patbase = pActwk->patbase;
    work->shell_actor_index = pNewact - actwk;
    scarab_work_get(pNewact)->parent_index = pActwk - actwk;

    if (pActwk->userflag.b.l) {

        pActwk->actflg |= 1;
        pNewact->actflg |= 1;
        pActwk->cddat |= 1;
        pNewact->cddat |= 1;
    }

    if (!pActwk->userflag.b.h) {

        work->pattern_change = pchg_n;
        work->x_velocity = 24576;
    } else {
        work->pattern_change = pchg_o;
        work->x_velocity = 12288;
    }

    e_move(pActwk);
}

static void e_move(sprite_status *pActwk) {
    Sint16 xSav, ySav, wk;
    sprite_status *pSubact;
    scarab_work *work = scarab_work_get(pActwk);

    ySav = pActwk->yposi.w.h;
    xSav = pActwk->xposi.w.h;

    if (work->animation_timer >= 0) {
        pActwk->xposi.l += work->x_velocity;

        if ((wk = pActwk->xposi.w.h - work->origin_x) < 0) {
            wk *= -1;
        }

        if (wk >= 64) {
            turn(pActwk);
        }

        wk = emycol_d(pActwk);
        if (wk + 7 >= 14) {

            turn(pActwk);
        } else {
            pActwk->yposi.w.h += wk;
        }
    }

    xSav -= pActwk->xposi.w.h;
    ySav -= pActwk->yposi.w.h;

    pSubact = &actwk[work->parent_index];
    if ((wk = scarab_work_get(pSubact)->child_item_index) != 0) {
        pSubact = &actwk[wk];
        pSubact->xposi.w.h -= xSav;
        pSubact->yposi.w.h -= ySav;
        pSubact->xspeed.w = -xSav << 8;
    }

    if ((wk = work->carried_actor_index) >= 0) {
        pSubact = &actwk[wk];
        pSubact->xposi.w.h -= xSav;
        pSubact->yposi.w.h -= ySav;
    }

    e_move_tbl[pActwk->r_no1 / 2](pActwk);
    pSubact = &actwk[work->shell_actor_index];
    pSubact->xposi.w.h = pActwk->xposi.w.h;
    pSubact->yposi.w.h = pActwk->yposi.w.h;
    pSubact->patno = pActwk->patno + 3;
}

static void turn(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);

    pActwk->xposi.l -= work->x_velocity;
    work->x_velocity *= -1;
}

static void e1_check(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);
    scarab_work *parent_work = scarab_work_get(&actwk[work->parent_index]);

    if (!pActwk->colicnt)
        goto label1;
    if (actwk[0].r_no0 == 4 || actwk[0].r_no0 == 6 ||
        player_work_get(&actwk[0])->damage_invulnerability_timer ||
        parent_work->child_item_index) {
        pActwk->colicnt = 0;
        goto label1;
    }
    e1_catch(pActwk);
    return;

label1:
    e1_chk_patchg(pActwk);
}

static void e1_chk_patchg(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);

    if (--work->animation_timer >= 0) {

        patchg(pActwk, work->pattern_change);
    } else if (work->animation_timer <= -30) {
        work->animation_timer = 60;
    }
}

static void e1_catch(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);

    work->carried_actor_index = &actwk[0] - actwk;
    player_work_get(&actwk[0])->status_flags |= 1;
    actwk[0].cddat |= 4;
    actwk[0].mstno.b.h = 2;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;
    actwk[0].yposi.w.h = pActwk->yposi.w.h - 3;

    if (!pActwk->userflag.b.l) {

        actwk[0].xposi.w.h = pActwk->xposi.w.h - 24;
    } else {
        actwk[0].xposi.w.h = pActwk->xposi.w.h + 24;
    }

    pActwk->r_no1 += 2;
    work->hold_timer = 120;
    pActwk->colino = 240, pActwk->colicnt = 0;
}

static void e1_keep(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    scarab_work *work = scarab_work_get(pActwk);

    if (--work->hold_timer != 0) {
        pActwk->colino = 240, pActwk->colicnt = 0;
        e1_chk_patchg(pActwk);
    } else {
        pPlayerwk = &actwk[work->carried_actor_index];

        work->carried_actor_index = -1;
        player_work_get(pPlayerwk)->status_flags &= 254;
        pPlayerwk->yspeed.w = 0;

        if (!pActwk->userflag.b.h) {

            pPlayerwk->xspeed.w = -2560;
        } else {
            pPlayerwk->xspeed.w = -1280;
        }

        if (!pActwk->userflag.b.l) {

            pPlayerwk->cddat |= 1;
        } else {
            pPlayerwk->xspeed.w *= -1;
            pPlayerwk->cddat &= 254;
        }

        player_work_get(pPlayerwk)->mode_word = 15;
        pPlayerwk->mspeed.w = pPlayerwk->xspeed.w;
        pPlayerwk->direc.b.h = 0;
        pPlayerwk->cddat &= 223;

        pActwk->r_no1 += 2;
        pActwk->colino = pActwk->colicnt = 0;
        pActwk->patno = 2;
        work->hold_timer = 30;
    }
}

static void e1_wait(sprite_status *pActwk) {
    scarab_work *work = scarab_work_get(pActwk);

    if (!(--work->hold_timer)) {

        pActwk->r_no1 = 0;
        pActwk->mstno.b.l = -1;
        pActwk->colino = 240, pActwk->colicnt = 0;
    }
}

static void iteminit(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->sprvsize = 14;
    pActwk->sprhs = 14;
    pActwk->patbase = itempat;
    pActwk->sproffset = 1448;
    pActwk->actflg = 4;
    pActwk->sprpri = 2;
    pActwk->sprhsize = 15;
    pActwk->colino = 198;
    pActwk->mstno.b.h = pActwk->userflag.b.l;
    pActwk->yposi.w.h += emycol_d(pActwk);

    itemmove(pActwk);
}

static void itemmove(sprite_status *pActwk) {
    Sint16 col;

    if (!(pActwk->actflg & 128)) {
        itemmove3(pActwk);
    } else {
        if (pActwk->r_no1) {
            speedset(pActwk);

            if ((col = emycol_d(pActwk)) < 0) {
                pActwk->yposi.w.h += col;
                pActwk->yspeed.w = 0;
                pActwk->r_no1 = 0;
            }
        } else {

            ride_on_chk_i(pActwk, &actwk[0]);
        }

        itemmove2(pActwk);
    }
}

static void itemmove2(sprite_status *pActwk) {
    if (!time_stop) {
        patchg(pActwk, itemchg);
    }

    itemmove3(pActwk);
}

static void itemmove3(sprite_status *pActwk) {
    sprite_status *pCtrlact;
    scarab_work *work = scarab_work_get(pActwk);

    actionsub(pActwk);
    pCtrlact = &actwk[work->parent_index];
    frameout_s(pActwk);

    if (!pActwk->actno) {
        scarab_work_get(pCtrlact)->child_item_index = 0;
    }
}

static void itemget(sprite_status *pActwk) {
    sprite_status *pCtrlact, *pNewact;
    Sint16 idx;
    scarab_work *work = scarab_work_get(pActwk);
    scarab_work *ctrl_work;

    pCtrlact = &actwk[work->parent_index];
    ctrl_work = scarab_work_get(pCtrlact);

    if ((idx = ctrl_work->child_enemy_index) != 0) {
        actwk[idx].colicnt = 1;
    }

    ride_on_chk_i(pActwk, &actwk[0]);
    ride_on_clr(pActwk, &actwk[0]);

    soundset(150);
    pActwk->r_no0 += 4;
    pActwk->colino = 0;

    if (actwkchk(&pNewact) == 0) {
        pNewact->actno = 26;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
        pNewact->mstno.b.h = pActwk->mstno.b.h;

        if (actwkchk(&pNewact) == 0) {
            pNewact->actno = 24;
            pNewact->xposi.w.h = pActwk->xposi.w.h;
            pNewact->yposi.w.h = pActwk->yposi.w.h;
            pNewact->r_no1 = 1;
            pNewact->userflag.b.h = 1;
        }
    }

    frameout(pActwk);
}

static void ride_on_chk_i(sprite_status *pActwk, sprite_status *pPlayerwk) {
    pActwk->actno = 25;
    ride_on_chk(pActwk, pPlayerwk);
    pActwk->actno = 47;
}

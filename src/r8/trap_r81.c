#include "../equ.h"
#include "trap_r81.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"
#include "coli8.h"

static void dair8_init(sprite_status *trapwk);
static void dair8_move(sprite_status *trapwk);
static void harir8_init(sprite_status *trapwk);
static void harir8_move(sprite_status *trapwk);
static void anar8_ridechk(sprite_status *trapwk);
static void anar8_rideclr(sprite_status *trapwk);
static void anar8_init(sprite_status *trapwk);
static void anar8_move1(sprite_status *trapwk);
static void anar8_move2(sprite_status *trapwk);
static void futagor8_ridechk(sprite_status *trapwk);
static void futagor8_init(sprite_status *trapwk);
static void futagor8_move1(sprite_status *trapwk);

static sprite_pattern dai00 = {1, {{-16, -16, 0, 288}}};
sprite_pattern *togedair8pat[1] = {&dai00};

void togedair8(sprite_status *trapwk) {
    void (*tbl[2])(sprite_status *) = {&dair8_init, &dair8_move};

    if (trapwk->userflag.b.h < 0) {
        harir8(trapwk);
        return;
    }
    tbl[trapwk->r_no0 / 2](trapwk);
    actionsub(trapwk);
    frameout_s00(trapwk, ((Sint16 *)trapwk)[29]);
}

static void dair8_init(sprite_status *trapwk) {
    sprite_status *new_actwk;

    trapwk->r_no0 += 2;
    trapwk->actflg |= 4;

    trapwk->sprpri = 3;
    trapwk->patbase = togedair8pat;
    ((Sint16 *)trapwk)[29] = trapwk->xposi.w.h;
    ((Sint16 *)trapwk)[27] = trapwk->yposi.w.h;
    trapwk->sprhsize = trapwk->sprvsize = 16;
    if (actwkchk(&new_actwk) != 0) {
        frameout(trapwk);
        return;
    }

    if (trapwk->userflag.b.h != 1) {
        new_actwk->actno = 10;
        new_actwk->xposi.w.h = trapwk->xposi.w.h;
        new_actwk->yposi.w.h = trapwk->yposi.w.h;
        new_actwk->actfree[15] = 232;
        ((Uint16 *)new_actwk)[28] = trapwk - actwk;
        new_actwk->userflag.b.h = trapwk->userflag.b.h;
        dair8_move(trapwk);
    } else {
        new_actwk->actno = 54;
        new_actwk->userflag.b.h = -1;
        new_actwk->xposi.w.h = trapwk->xposi.w.h;
        new_actwk->yposi.w.h = trapwk->yposi.w.h;
        new_actwk->actfree[15] = 232;
        ((Uint16 *)new_actwk)[28] = trapwk - actwk;
        dair8_move(trapwk);
    }
}

static void dair8_move(sprite_status *trapwk) {
    Sint16 cal_position;

    if (!(trapwk->actflg & 128))
        return;

    if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
        cal_position = -cal_position;
    if (cal_position >= 32)
        return;

    hitchk(trapwk, &actwk[0]);
}

static sprite_pattern har00 = {1, {{-16, -16, 0, 289}}};
static sprite_pattern har01 = {1, {{-16, -16, 16, 289}}};
sprite_pattern *harir8pat[2] = {&har00, &har01};

void harir8(sprite_status *trapwk) {
    Uint16 master;
    void (*tbl[2])(sprite_status *) = {&harir8_init, &harir8_move};

    tbl[trapwk->r_no0 / 2](trapwk);
    actionsub(trapwk);
    if ((master = ((Uint16 *)trapwk)[28]) != 0) {

        if (actwk[master].actno == 0) {
            frameout(trapwk);
            return;
        }
        trapwk->xposi.w.h =
            actwk[master].xposi.w.h + (Sint16)(char)trapwk->actfree[14];

        trapwk->yposi.w.h =
            actwk[master].yposi.w.h + (Sint16)(char)trapwk->actfree[15];
    }

    frameout_s00(trapwk, ((Sint16 *)trapwk)[29]);
}

static void harir8_init(sprite_status *trapwk) {
    trapwk->r_no0 += 2;
    trapwk->actflg |= 4;

    trapwk->sprpri = 4;
    trapwk->patbase = harir8pat;
    ((Sint16 *)trapwk)[29] = trapwk->xposi.w.h;
    ((Sint16 *)trapwk)[27] = trapwk->yposi.w.h;

    trapwk->sprhsize = 16;
    trapwk->sprvsize = 8;
    if (trapwk->userflag.b.h != -1) {
        trapwk->patno = 1;
        trapwk->colino = 131;
        trapwk->sprhsize = 18;
    }
    harir8_move(trapwk);
}

static void harir8_move(sprite_status *trapwk) {
    Sint16 cal_position;

    if (trapwk->patno == 0) {

        if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
            cal_position = -cal_position;
        if (cal_position >= 32)
            return;

        if (hitchk(trapwk, &actwk[0]) == 0)
            return;
        if (!(trapwk->cddat & 8))
            return;
        if (plpower_a || plpower_m)
            return;

        if (actwk[0].r_no0 >= 4)
            return;
        if (((Sint16 *)&actwk[0])[26] != 0)
            return;
        actwk[0].yposi.l -= actwk[0].yspeed.w << 8;
        playdamageset(&actwk[0], trapwk);
        return;
    }

    if (!(trapwk->actflg & 128))
        return;

    if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
        cal_position = -cal_position;
    if (cal_position >= 32)
        return;

    hitchk(trapwk, &actwk[0]);
}

static Uint8 anar8pchgl[16] = {7, 32, 6, 28, 5, 24, 4, 20,
                               3, 16, 2, 12, 1, 8,  0, 4};
static Uint8 anar8pchgr[16] = {15, 32, 14, 28, 13, 24, 12, 20,
                               11, 16, 10, 12, 9,  8,  8,  4};
static Uint8 gater8pchgl[16] = {0, 4,  1, 8,  2, 12, 3, 16,
                                4, 20, 5, 24, 6, 28, 7, 32};
static Uint8 gater8pchgr[16] = {8,  4,  9,  8,  10, 12, 11, 16,
                                12, 20, 13, 24, 14, 28, 15, 32};
static sprite_pattern anal0 = {1, {{-4, -8, 0, 290}}};
static sprite_pattern anal1 = {1, {{-8, -8, 0, 291}}};
static sprite_pattern anal2 = {1, {{-12, -8, 0, 292}}};
static sprite_pattern anal3 = {1, {{-16, -8, 0, 293}}};
static sprite_pattern anal4 = {1, {{-20, -8, 0, 294}}};
static sprite_pattern anal5 = {1, {{-24, -8, 0, 295}}};
static sprite_pattern anal6 = {1, {{-28, -8, 0, 296}}};
static sprite_pattern anal7 = {1, {{-32, -8, 0, 297}}};
static sprite_pattern anar0 = {1, {{-4, -8, 8, 290}}};
static sprite_pattern anar1 = {1, {{-8, -8, 8, 291}}};
static sprite_pattern anar2 = {1, {{-12, -8, 8, 292}}};
static sprite_pattern anar3 = {1, {{-16, -8, 8, 293}}};
static sprite_pattern anar4 = {1, {{-20, -8, 8, 294}}};
static sprite_pattern anar5 = {1, {{-24, -8, 8, 295}}};
static sprite_pattern anar6 = {1, {{-28, -8, 8, 296}}};
static sprite_pattern anar7 = {1, {{-32, -8, 8, 297}}};
static sprite_pattern ana00 = {1, {{-16, -20, 0, 0}}};
static sprite_pattern ana01 = {2, {{-64, -8, 0, 297}, {0, -8, 8, 297}}};
sprite_pattern *anar8pat[18] = {&anal0, &anal1, &anal2, &anal3, &anal4, &anal5,
                                &anal6, &anal7, &anar0, &anar1, &anar2, &anar3,
                                &anar4, &anar5, &anar6, &anar7, &ana00, &ana01};

void anar8(sprite_status *trapwk) {
    void (*tbl[3])(sprite_status *) = {&anar8_init, &anar8_move1, &anar8_move2};

    tbl[trapwk->r_no0 / 2](trapwk);
    actionsub(trapwk);
    frameout_s00(trapwk, ((Sint16 *)trapwk)[29]);
}

static void anar8_ridechk(sprite_status *trapwk) { ridechk(trapwk, &actwk[0]); }

static void anar8_rideclr(sprite_status *trapwk) {
    ride_on_clr(trapwk, &actwk[0]);
}

static void anar8_init(sprite_status *trapwk) {
    sprite_status *new_actwk;

    trapwk->r_no0 += 2;
    trapwk->actflg |= 4;
    trapwk->sprpri = 2;

    trapwk->patbase = anar8pat;
    trapwk->sprvsize = 8;
    trapwk->sprhsize = 4;
    trapwk->patno = 16;

    if (trapwk->actfree[20] != 0) {
        anar8_move1(trapwk);
        return;
    }

    trapwk->xposi.w.h -= 4;
    ((Sint16 *)trapwk)[29] = trapwk->xposi.w.h;
    if (actwkchk(&new_actwk) != 0) {
        frameout(trapwk);
        return;
    }
    new_actwk->actno = 55;
    new_actwk->xposi.w.h = trapwk->xposi.w.h + 128;
    new_actwk->yposi.w.h = trapwk->yposi.w.h;
    ((Sint16 *)new_actwk)[29] = ((Sint16 *)trapwk)[29];

    new_actwk->mstno.w = 1;
    new_actwk->actfree[20] = 1;
    anar8_move1(trapwk);
}

static void anar8_move1(sprite_status *trapwk) {
    Sint16 x_adder;
    Uint8 *patchg_data;

    if (trapwk->actfree[16] != 0) {
        --trapwk->actfree[16];
        anar8_rideclr(trapwk);
        return;
    }

    if (trapwk->actfree[20] == 0) {
        patchg_data = gater8pchgl;
        x_adder = 4;
    } else {
        patchg_data = gater8pchgr;
        x_adder = -4;
    }

    if (trapwk->actflg & 128)
        soundset(164);

    trapwk->patno = patchg_data[trapwk->actfree[19] * 2];
    trapwk->sprhsize = patchg_data[trapwk->actfree[19] * 2 + 1];
    trapwk->xposi.w.h += x_adder;
    if (++trapwk->actfree[19] == 8) {
        trapwk->actfree[16] = 60;
        trapwk->actfree[19] = 0;
        trapwk->r_no0 += 2;
    }
    anar8_ridechk(trapwk);
}

static void anar8_move2(sprite_status *trapwk) {
    Sint16 x_adder;
    Uint8 *patchg_data;

    if (trapwk->actfree[16] != 0) {
        --trapwk->actfree[16];
        anar8_ridechk(trapwk);
        return;
    }

    if (trapwk->actfree[20] == 0) {
        patchg_data = anar8pchgl;
        x_adder = -4;
    } else {
        patchg_data = anar8pchgr;
        x_adder = 4;
    }

    if (trapwk->actflg & 128)
        soundset(164);

    trapwk->patno = patchg_data[trapwk->actfree[19] * 2];
    trapwk->sprhsize = patchg_data[trapwk->actfree[19] * 2 + 1];
    trapwk->xposi.w.h += x_adder;
    if (++trapwk->actfree[19] == 8) {
        trapwk->actfree[16] = 60;
        trapwk->actfree[19] = 0;
        trapwk->patno = 16;
        trapwk->sprhsize = 0;
        trapwk->r_no0 -= 2;
        anar8_rideclr(trapwk);
        return;
    }
    anar8_ridechk(trapwk);
}

static Uint8 futagor8_pchg[48] = {0, 120, 8,  56, 1, 4, 8,  40, 2, 4, 8,  24,
                                  3, 32,  8,  8,  4, 4, 24, 8,  5, 4, 40, 8,
                                  6, 120, 56, 8,  5, 4, 40, 8,  4, 4, 24, 8,
                                  3, 32,  8,  8,  2, 4, 24, 8,  1, 4, 40, 8};
static sprite_pattern fut00 = {7,
                               {{-8, -56, 0, 299},
                                {-8, -40, 0, 299},
                                {-8, -24, 0, 299},
                                {-8, -8, 0, 298},
                                {-8, 8, 0, 299},
                                {-8, 24, 0, 299},
                                {-8, 40, 0, 299}}};
static sprite_pattern fut01 = {5,
                               {{-8, -40, 0, 299},
                                {-8, -24, 0, 299},
                                {-8, -8, 0, 298},
                                {-8, 8, 0, 299},
                                {-8, 24, 0, 299}}};
static sprite_pattern fut02 = {
    3, {{-8, -24, 0, 299}, {-8, -8, 0, 298}, {-8, 8, 0, 299}}};
static sprite_pattern fut03 = {1, {{-8, -8, 0, 298}}};
static sprite_pattern fut04 = {
    3, {{-24, -8, 0, 299}, {-8, -8, 0, 298}, {8, -8, 0, 299}}};
static sprite_pattern fut05 = {5,
                               {{-40, -8, 0, 299},
                                {-24, -8, 0, 299},
                                {-8, -8, 0, 298},
                                {8, -8, 0, 299},
                                {24, -8, 0, 299}}};
static sprite_pattern fut06 = {7,
                               {{-56, -8, 0, 299},
                                {-40, -8, 0, 299},
                                {-24, -8, 0, 299},
                                {-8, -8, 0, 298},
                                {8, -8, 0, 299},
                                {24, -8, 0, 299},
                                {40, -8, 0, 299}}};
sprite_pattern *futagor8pat[7] = {&fut00, &fut01, &fut02, &fut03,
                                  &fut04, &fut05, &fut06};

void futagor8(sprite_status *trapwk) {
    void (*tbl[2])(sprite_status *) = {&futagor8_init, &futagor8_move1};

    tbl[trapwk->r_no0 / 2](trapwk);
    futagor8_ridechk(trapwk);
    actionsub(trapwk);
    frameout_s00(trapwk, ((Sint16 *)trapwk)[29]);
}

static void futagor8_ridechk(sprite_status *trapwk) {
    ride_on_chk(trapwk, &actwk[0]);
}

static void futagor8_init(sprite_status *trapwk) {
    trapwk->r_no0 += 2;
    trapwk->actflg |= 4;
    trapwk->sprpri = 3;

    trapwk->patbase = futagor8pat;
    ((Sint16 *)trapwk)[29] = trapwk->xposi.w.h;
    ((Sint16 *)trapwk)[27] = trapwk->yposi.w.h;
    futagor8_move1(trapwk);
}

static void futagor8_move1(sprite_status *trapwk) {
    Uint8 patchg_point;

    if (trapwk->actfree[16] == 0) {
        patchg_point = trapwk->actfree[18] * 4;
        trapwk->patno = futagor8_pchg[patchg_point];
        trapwk->actfree[16] = futagor8_pchg[patchg_point + 1];
        trapwk->sprhsize = futagor8_pchg[patchg_point + 2];
        trapwk->sprvsize = futagor8_pchg[patchg_point + 3];
    } else if (--trapwk->actfree[16] == 0) {
        if (++trapwk->actfree[18] >= 12)
            trapwk->actfree[18] = 0;
    }

    if (trapwk->patno != 0)
        trapwk->yspeed.w = 0;
    else
        trapwk->yspeed.w = 256;
}

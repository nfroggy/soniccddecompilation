#include "../equ.h"
#include "shut.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "scr81a.h"

#if defined(R83)
#define SPRITE_SHUT_BASE 495
#else
#define SPRITE_SHUT_BASE 304
#endif

static void shut_init(sprite_status *shutwk);
static void shut_wait(sprite_status *shutwk);
static void shut_move(sprite_status *shutwk);
static void kaiten_init(sprite_status *loopwk);
static void kaiten_move0(sprite_status *loopwk);
static void kaiten_move1(sprite_status *loopwk);
static void kaiten_move2(sprite_status *loopwk);
static Sint16 ridechk_k(sprite_status *loopwk);
static void kaiten_bou0(sprite_status *loopwk);
static void kaiten_bou1(sprite_status *loopwk);

static Uint8 kaitenchg0[4] = {2, 0, 1, 255};
static Uint8 *kaitenchg[1] = {kaitenchg0};
static sprite_pattern shu00 = {6,
                               {{-8, -96, 8, SPRITE_SHUT_BASE + 4},
                                {-8, -64, 8, SPRITE_SHUT_BASE + 4},
                                {-8, -32, 8, SPRITE_SHUT_BASE + 4},
                                {-8, 0, 8, SPRITE_SHUT_BASE + 4},
                                {-8, 32, 8, SPRITE_SHUT_BASE + 4},
                                {-8, 64, 8, SPRITE_SHUT_BASE + 4}}};
sprite_pattern *shutpat[1] = {&shu00};
static sprite_pattern kai00 = {3,
                               {{-40, -8, 0, SPRITE_SHUT_BASE},
                                {-24, -8, 0, SPRITE_SHUT_BASE + 1},
                                {8, -8, 0, SPRITE_SHUT_BASE + 2}}};
static sprite_pattern kai01 = {3,
                               {{-40, -8, 0, SPRITE_SHUT_BASE},
                                {-24, -8, 8, SPRITE_SHUT_BASE + 1},
                                {8, -8, 0, SPRITE_SHUT_BASE + 2}}};
static sprite_pattern kai02 = {5,
                               {{-4, -80, 0, SPRITE_SHUT_BASE + 3},
                                {-4, -48, 0, SPRITE_SHUT_BASE + 3},
                                {-4, -16, 0, SPRITE_SHUT_BASE + 3},
                                {-4, 16, 0, SPRITE_SHUT_BASE + 3},
                                {-4, 48, 0, SPRITE_SHUT_BASE + 3}}};
sprite_pattern *kaitenpat[3] = {&kai00, &kai01, &kai02};

void shut(sprite_status *shutwk) {
    void (*tbl[3])(sprite_status *) = {&shut_init, &shut_wait, &shut_move};

    tbl[shutwk->r_no0 / 2](shutwk);
    actionsub(shutwk);
    frameout_s(shutwk);
}

static void shut_init(sprite_status *shutwk) {
    sprite_status *new_actwk;

    ((Sint16 *)shutwk)[23] = shutwk->xposi.w.h;
    shutwk->userflag.b.h = 1;
    shutwk->actflg |= 1;

    if (shutwk->userflag.b.h)
        shutwk->xposi.w.h -= 8;
    else
        shutwk->xposi.w.h += 8;
    shutwk->r_no0 += 2;
    shutwk->actflg |= 4;
    shutwk->sprpri = 2;
    shutwk->patbase = shutpat;
    shutwk->sproffset = 32768;

    if (actwkchk(&new_actwk) != 0) {
        frameout_s0(shutwk);
        return;
    }
    new_actwk->actno = 61;
    new_actwk->xposi.w.h = ((Sint16 *)new_actwk)[23] = ((Sint16 *)shutwk)[23];

    new_actwk->yposi.w.h = shutwk->yposi.w.h;
    if (new_actwk->userflag.b.h)
        new_actwk->xposi.w.h -= 8;
    else
        new_actwk->xposi.w.h += 8;
    new_actwk->r_no0 += 2;
    new_actwk->actflg |= 4;
    new_actwk->sprpri = 2;
    new_actwk->patbase = shutpat;
    new_actwk->sproffset = 32768;
}

void shut_wait(sprite_status *shutwk) {
    if ((char)bossstart >= 0)
        return;
    shut_flag = 0;
    shutwk->r_no0 += 2;
}

void shut_move(sprite_status *shutwk) {
    Uint16 cal_xposi, block_wrt_x, block_wrt_y, i = 0;

    block_wrt_x = cal_xposi = shutwk->xposi.w.h - 8;
    if (!(cal_xposi & 15)) {
        if (++shutwk->actfree[2] >= 17) {
            frameout_s0(shutwk);
            return;
        }
        block_wrt_y = shutwk->yposi.w.h - 96;

        for (; i < 12; block_wrt_y += 16, ++i)
            block_wrt(0, block_wrt_x, block_wrt_y);
    }

    shutwk->xposi.w.h += 2;
    if (shutwk->userflag.b.h)
        shutwk->xposi.w.h -= 4;
}

void kaiten(sprite_status *loopwk) {
    Sint16 i;
    void (*tbl[6])(sprite_status *) = {&kaiten_init,  &kaiten_move0,
                                       &kaiten_move1, &kaiten_move2,
                                       &kaiten_bou0,  &kaiten_bou1};
    if (loopwk->yposi.w.h - actwk[0].yposi.w.h > 22 &&
        loopwk->yposi.w.h - actwk[0].yposi.w.h <= 156) {
        if (prio_flag) {
            actwk[0].sproffset &= 32767;
            if (actwk[6].actno)
                actwk[6].sproffset &= 32767;
            for (i = 8; i < 12; ++i)
                if (actwk[i].actno)
                    actwk[i].sproffset &= 32767;
        } else {
            actwk[0].sproffset |= 32768;
            if (actwk[6].actno)
                actwk[6].sproffset |= 32768;
            for (i = 8; i < 12; ++i)
                if (actwk[i].actno)
                    actwk[i].sproffset |= 32768;
        }
    }

    tbl[loopwk->r_no0 / 2](loopwk);
    if (loopwk->r_no0 == 0) {
        frameout_s0(loopwk);
        return;
    }
    actionsub(loopwk);
    if (loopwk->r_no0 < 8) {
        frameout_s(loopwk);
        return;
    }
    frameout_s00(loopwk, ((Sint16 *)loopwk)[23]);
}

static void kaiten_init(sprite_status *loopwk) {
    Sint16 i, sin_tmp, cos_tmp;
    Sint32 cos_data;
    sprite_status *old_actwk, *new_actwk;
    Uint8 tbl[3] = {64, 149, 234};

    old_actwk = loopwk;
    for (i = 0; i < 4; ++i) {
        if (actwkchk2(old_actwk, &new_actwk) != 0)
            return;
        old_actwk = new_actwk;
    }
    loopwk->r_no0 += 2;
    loopwk->actflg |= 4;
    loopwk->sprpri = 1;
    loopwk->sprhsize = 40;
    loopwk->sprvsize = 8;
    loopwk->patbase = kaitenpat;

    actwkchk2(loopwk, &new_actwk);
    new_actwk->actno = 62;
    new_actwk->actflg |= 4;
    new_actwk->sprpri = 1;
    new_actwk->patbase = kaitenpat;

    new_actwk->sprhsize = 40;
    new_actwk->sprvsize = 8;
    new_actwk->xposi.w.h = loopwk->xposi.w.h;
    new_actwk->yposi.w.h = loopwk->yposi.w.h - 176;
    new_actwk->userflag.b.h = 1;
    new_actwk->r_no0 += 2;

    for (i = 0; i < 3; ++i) {
        actwkchk2(loopwk, &new_actwk);
        new_actwk->actno = 62;
        new_actwk->actflg |= 4;
        new_actwk->sprpri = 1;
        new_actwk->patbase = kaitenpat;

        new_actwk->sprhsize = 8;
        new_actwk->sprvsize = 80;
        new_actwk->actfree[3] = tbl[2 - i];
        new_actwk->patno = 2;
        new_actwk->r_no0 = 8;
        ((Sint16 *)new_actwk)[23] = loopwk->xposi.w.h;
        new_actwk->yposi.w.h = loopwk->yposi.w.h - 88;
        sinset(new_actwk->actfree[3], &sin_tmp, &cos_tmp);
        cos_data = cos_tmp << 5 >> 8;
        new_actwk->xposi.w.h = ((Sint16 *)new_actwk)[23] + (Sint16)cos_data;
    }
}

void kaiten_move0(sprite_status *loopwk) {
    Sint16 cal_xposi;

    if (loopwk->userflag.b.h) {
        if (actwk[0].mstno.b.h >= 51)
            patchg(loopwk, kaitenchg);
        return;
    }

    if (ridechk_k(loopwk) == 0) {
        loopwk->actfree[2] = 0;
        return;
    }
    cal_xposi = loopwk->xposi.w.h - actwk[0].xposi.w.h + 8;
    if (cal_xposi < 0 || cal_xposi >= 16) {
        loopwk->actfree[2] = 0;
        return;
    }
    if (loopwk->actfree[2] & 1)
        return;
    loopwk->actfree[2] |= 1;
    actwk[0].actfree[2] |= 1;
    actwk[0].xposi.w.h = loopwk->xposi.w.h;
    actwk[0].xspeed.w = actwk[0].mspeed.w = 0;
    actwk[0].mstno.b.h = 51;
    if (prio_flag)
        actwk[0].mstno.b.h = 52;
    loopwk->r_no0 += 2;
}

void kaiten_move1(sprite_status *loopwk) {
    patchg(loopwk, kaitenchg);
    if (actwk[0].mstno.b.h != 5) {
        ridechk_k(loopwk);
        return;
    }
    loopwk->r_no0 += 2;
    kaiten_move2(loopwk);
}

void kaiten_move2(sprite_status *loopwk) {
    actwk[0].actfree[2] &= 254;
    ridechk_k(loopwk);
    prio_flag ^= 1;
    actwk[0].actflg ^= 1;
    actwk[0].cddat ^= 1;
    actwk[0].sproffset |= 32768;
    if (prio_flag)
        actwk[0].sproffset &= 32767;
    loopwk->r_no0 = 2;
}

Sint16 ridechk_k(sprite_status *loopwk) { ridechk(loopwk, &actwk[0]); }

void kaiten_bou0(sprite_status *loopwk) {
    if (actwk[0].mstno.b.h < 51)
        return;
    loopwk->r_no0 += 2;
    loopwk->actfree[4] = loopwk->actfree[3] + 128;
}

void kaiten_bou1(sprite_status *loopwk) {
    Uint8 cal_dir;
    Sint16 sin_tmp, cos_tmp;
    Sint32 cos_data;

    if (actwk[0].mstno.b.h < 51) {
        loopwk->r_no0 -= 2;
        loopwk->actfree[3] = loopwk->actfree[4];
    } else {
        cal_dir = 4;
        if (!(actwk[0].cddat & 1)) {
            if (prio_flag)
                cal_dir = -cal_dir;

        } else {
            if (!prio_flag)
                cal_dir = -cal_dir;
        }
        loopwk->actfree[3] += cal_dir;
    }
    sinset(loopwk->actfree[3], &sin_tmp, &cos_tmp);
    cos_data = cos_tmp * 24 >> 8;
    loopwk->xposi.w.h = ((Sint16 *)loopwk)[23] + (Sint16)cos_data;

    loopwk->sprpri = 1;
    if ((char)loopwk->actfree[3] >= 0)
        loopwk->sprpri = 4;
}

#include "../equ.h"
#include "shoot5.h"
#include "../actset.h"
#include "../loader2.h"
#include "../player_work.h"

typedef struct {
    union {
        Sint16 travel_timer;
        struct {
            Uint8 travel_timer_low;
            Sint8 travel_timer_high;
        };
    };
    Sint16 target_x;
    Sint16 target_y;
    Uint8 route_length;
    Uint8 route_offset;
    Sint16 *route_points;
} shoot5_work;

static shoot5_work *shoot5_get_work(sprite_status *pActwk) {
    return (shoot5_work *)pActwk->actfree;
}

static void shooterinit(sprite_status *pActwk);
static void shootermove(sprite_status *pActwk);
static void shootermove2(sprite_status *pActwk);
static void shootermove3(sprite_status *pActwk);

Sint16 shooterposi_0[21] = {40,   9808, 208,  9808, 240,  9888, 272,
                            9760, 304,  9888, 336,  9760, 368,  9888,
                            400,  9760, 432,  9904, 488,  9904, 520};
Sint16 shooterposi_1[19] = {36,    10320, 496,   10320, 528,   10416, 560,
                            10288, 592,   10416, 624,   10288, 656,   10416,
                            688,   10288, 720,   10416, 752};
Sint16 shooterposi_2[15] = {28,  2944, 672, 2944, 728, 2976, 752, 2848,
                            784, 2976, 816, 2848, 848, 2976, 880};
Sint16 shooterposi_3[31] = {60,  9840, 264, 9840, 288, 9920, 320, 9792,
                            352, 9920, 384, 9792, 416, 9920, 448, 9792,
                            480, 9920, 512, 9792, 544, 9920, 576, 9792,
                            608, 9920, 640, 9792, 672, 9920, 752};
Sint16 shooterposi_4[25] = {48,    10288, 296,   10432, 368,   10304, 400,
                            10432, 432,   10304, 464,   10432, 496,   10304,
                            528,   10432, 560,   10304, 592,   10432, 624,
                            10304, 656,   10432, 752};
Sint16 shooterposi_5[19] = {36,   6480, 488,  6480, 528,  6576, 560,
                            6448, 592,  6576, 624,  6448, 656,  6576,
                            688,  6448, 720,  6576, 752};
Sint16 shooterposi_6[19] = {36,   6512, 488,  6512, 528,  6592, 560,
                            6464, 592,  6592, 624,  6464, 656,  6592,
                            688,  6464, 720,  6592, 752};
Sint16 shooterposi_7[5] = {8, 1424, 200, 1424, 376};
Sint16 shooterposi_8[5] = {8, 1424, 200, 1424, 376};
Sint16 shooterposi_9[5] = {8, 1424, 376, 1424, 200};
Sint16 shooterposi_0a[5] = {8, 5744, 296, 5744, 400};
Sint16 shooterposi_0b[5] = {8, 5744, 400, 5744, 296};
Sint16 shooterposi_0c[5] = {8, 7088, 520, 7088, 600};
Sint16 shooterposi_0d[5] = {8, 7088, 600, 7088, 520};
Sint16 shooterposi_0e[5] = {8, 7120, 520, 7120, 600};
Sint16 shooterposi_0f[5] = {8, 7120, 600, 7120, 520};
Sint16 shooterposi_10[43] = {
    84,   6912, 112,  6768, 112,  6752, 128,  6736, 160,  6752, 192,
    6768, 208,  6800, 208,  6816, 224,  6832, 256,  6832, 296,  6856,
    344,  6832, 392,  6856, 440,  6832, 488,  6856, 536,  6832, 584,
    6856, 632,  6832, 680,  6856, 728,  6832, 776,  6832, 856};
Sint16 shooterposi_11[43] = {
    84,   6832, 856,  6832, 776,  6856, 728,  6832, 680,  6856, 632,
    6832, 584,  6856, 536,  6832, 488,  6856, 440,  6832, 392,  6856,
    344,  6832, 296,  6832, 256,  6816, 224,  6800, 208,  6768, 208,
    6752, 192,  6736, 160,  6752, 128,  6768, 112,  6912, 112};
Sint16 shooterposi_12[15] = {28,  5008, 672, 5008, 736, 5024, 752, 4896,
                             784, 5024, 816, 4896, 848, 5024, 880};
Sint16 shooterposi_13[41] = {
    80,  6832, 856, 6832, 760, 6856, 712, 6832, 664, 6856, 616, 6832, 568, 6832,
    504, 6856, 456, 6832, 408, 6856, 360, 6832, 312, 6832, 256, 6816, 224, 6800,
    208, 6768, 208, 6752, 192, 6736, 160, 6752, 128, 6768, 112, 6912, 112};
Sint16 shooterposi_14[41] = {
    80,  6912, 112, 6768, 112, 6752, 128, 6736, 160, 6752, 192, 6768, 208, 6800,
    208, 6816, 224, 6832, 256, 6832, 312, 6856, 360, 6832, 408, 6856, 456, 6832,
    504, 6832, 568, 6856, 616, 6832, 664, 6856, 712, 6832, 760, 6832, 856};
Sint16 shooterposi_15[19] = {36,   6480, 488,  6480, 528,  6576, 544,
                             6448, 576,  6576, 608,  6448, 640,  6576,
                             672,  6448, 704,  6576, 736};
Sint16 *shooterpositbl[22] = {
    shooterposi_0,  shooterposi_1,  shooterposi_2,  shooterposi_3,
    shooterposi_4,  shooterposi_5,  shooterposi_6,  shooterposi_7,
    shooterposi_8,  shooterposi_9,  shooterposi_0a, shooterposi_0b,
    shooterposi_0c, shooterposi_0d, shooterposi_0e, shooterposi_0f,
    shooterposi_10, shooterposi_11, shooterposi_12, shooterposi_13,
    shooterposi_14, shooterposi_15};

void shooter(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0, d1, d2;
    void (*shooter_move_tbl[4])(sprite_status *) = {
        &shooterinit, &shootermove, &shootermove2, &shootermove3};

    if (time_flag & 128) {
        d0 = (Uint16)pActwk->cdsts;
        if (d0) {
            d0 *= 3;
            d1 = (Uint16)time_flag;
            d1 &= 127;
            d2 = time_item;
            d2 *= -1;
            d1 += d2;
            if (d1 < 0) {
                d1 = 0;
            } else {
                if ((Uint16)d1 >= 3) {
                    d1 = 2;
                }
            }
            d0 += d1;
            flagwork[d0] &= 127;
        }
    }
    if (!(pActwk->userflag.b.h & 128))
        goto label2;
    if (colrevflag)
        goto label1;
    if (!(pActwk->userflag.b.h & 1))
        goto label2;
    goto label3;
label1:
    if (!(pActwk->userflag.b.h & 1))
        goto label3;
label2:
    pPlayerwk = &actwk[0];
    if (pPlayerwk->mstno.b.h == 43)
        return;
    if (pPlayerwk->r_no0 >= 6)
        return;
    shooter_move_tbl[pActwk->r_no0 / 2](pActwk);

    if (pActwk->r_no0 >= 4)
        return;
label3:
    frameout_s(pActwk);
}

static void shooterinit(sprite_status *pActwk) {
    Sint16 *a2;
    Sint16 d0;
    shoot5_work *pWork = shoot5_get_work(pActwk);

    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 16;

    pActwk->r_no0 += 2;
    d0 = (Uint16)pActwk->userflag.b.h;
    d0 &= 127;
    a2 = shooterpositbl[d0];
    pWork->route_length = (Uint8)*a2++;
    pWork->route_offset = 0;
    pWork->route_points = a2;
    pWork->target_x = *a2++;
    pWork->target_y = *a2++;

    shootermove(pActwk);
}

static void shootermove(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    player_work *pPlayerWork;
    Sint16 d0, d1;

    pPlayerwk = &actwk[0];
    pPlayerWork = player_work_get(pPlayerwk);
    d0 = pPlayerwk->xposi.w.h;
    d0 -= pActwk->xposi.w.h;
    d0 += 16;
    if ((Uint16)d0 >= 32)
        return;
    d1 = pPlayerwk->yposi.w.h;
    d1 -= pActwk->yposi.w.h;
    d1 += 16;
    if ((Uint16)d1 >= 32)
        return;
    if (pPlayerWork->status_flags)
        return;
    if (pPlayerwk->r_no0 == 4) {
        pPlayerwk->r_no0 -= 2;
        pPlayerWork->damage_invulnerability_timer = 120;
    }
    pActwk->r_no0 += 2;
    pPlayerWork->status_flags = 129;
    pPlayerwk->mstno.b.h = 2;
    pPlayerwk->mspeed.w = 2560;
    pPlayerwk->xspeed.w = 0;
    pPlayerwk->yspeed.w = 0;
    pActwk->cddat &= 223;
    pPlayerwk->cddat &= 223;
    pPlayerwk->cddat |= 2;
    pPlayerwk->xposi.w.h = pActwk->xposi.w.h;
    pPlayerwk->yposi.w.h = pActwk->yposi.w.h;
    soundset(145);
}

static void shootermove2(sprite_status *pActwk) {
    shooterspdset(pActwk);
    pActwk->r_no0 += 2;
    soundset(145);
}

static void shootermove3(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 *a2;
    Sint32 d0, d2, d3;
    Uint8 d1;
    shoot5_work *pWork = shoot5_get_work(pActwk);

    pPlayerwk = &actwk[0];
    --pWork->travel_timer_high;
    if (pWork->travel_timer_high < 0) {

        pPlayerwk->xposi.w.h = pWork->target_x;
        pPlayerwk->yposi.w.h = pWork->target_y;
        d1 = pWork->route_offset;
        d1 = (char)d1 + 4;
        if (d1 >= pWork->route_length) {

            d1 = 0;
            goto label1;
        }
        pWork->route_offset = d1;
        a2 = pWork->route_points;
        a2 += d1 / 2;
        pWork->target_x = *a2++;
        pWork->target_y = *a2++;
        shooterspdset(pActwk);
        return;
    }
    d2 = pPlayerwk->xposi.l;
    d3 = pPlayerwk->yposi.l;
    d0 = pPlayerwk->xspeed.w;
    d0 <<= 8;
    d2 += d0;
    d0 = pPlayerwk->yspeed.w;
    d0 <<= 8;
    d3 += d0;
    pPlayerwk->xposi.l = d2;
    pPlayerwk->yposi.l = d3;
    return;
label1:
    pPlayerwk->yposi.w.h &= 2047;
    pActwk->r_no0 = 0;
    player_work_get(pPlayerwk)->status_flags = 0;
    pActwk->r_no1 = 2;
}

void shooterspdset(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    int_union ld0, ld1;
    Sint16 d2, d3, tmp;
    shoot5_work *pWork = shoot5_get_work(pActwk);

    pPlayerwk = &actwk[0];
    ld0.l = 0;
    if (!pPlayerwk->mspeed.w)
        goto label1;
    d2 = pPlayerwk->mspeed.w;
    d3 = pPlayerwk->mspeed.w;
    goto label2;

label1:
    d2 = d3 = 2560;
label2:
    ld0.w.l = pWork->target_x;
    if (ld0.w.l < pPlayerwk->xposi.w.h) {
        ld0.w.l -= pPlayerwk->xposi.w.h;
        ld0.w.l *= -1;
        d2 *= -1;
    } else {
        ld0.w.l -= pPlayerwk->xposi.w.h;
    }
    ld1.l = 0;
    ld1.w.l = pWork->target_y;
    if (ld1.w.l < pPlayerwk->yposi.w.h) {
        ld1.w.l -= pPlayerwk->yposi.w.h;
        ld1.w.l *= -1;
        d3 *= -1;
    } else {
        ld1.w.l -= pPlayerwk->yposi.w.h;
    }
    if ((Uint16)ld1.w.l >= (Uint16)ld0.w.l) {
        ld1.l = 0;
        ld1.w.l = pWork->target_y;
        ld1.w.l -= pPlayerwk->yposi.w.h;
        tmp = ld1.w.h;
        ld1.w.h = ld1.w.l;
        ld1.w.l = tmp;
        ld1.w.l = ld1.l / d3;

        ld0.l = 0;
        ld0.w.l = pWork->target_x;
        ld0.w.l -= pPlayerwk->xposi.w.h;
        if (ld0.w.l) {
            tmp = ld0.w.h;
            ld0.w.h = ld0.w.l;
            ld0.w.l = tmp;
            ld0.w.l = ld0.l / ld1.w.l;
        }
        pPlayerwk->xspeed.w = ld0.w.l;
        pPlayerwk->yspeed.w = d3;
        if (ld1.w.l < 0) {
            ld1.w.l *= -1;
        }
        pWork->travel_timer = ld1.w.l;
    } else {
        ld0.l = 0;
        ld0.w.l = pWork->target_x;
        ld0.w.l -= pPlayerwk->xposi.w.h;
        tmp = ld0.w.h;
        ld0.w.h = ld0.w.l;
        ld0.w.l = tmp;
        ld0.w.l = ld0.l / d2;

        ld1.l = 0;
        ld1.w.l = pWork->target_y;
        ld1.w.l -= pPlayerwk->yposi.w.h;
        if (ld1.w.l) {
            tmp = ld1.w.h;
            ld1.w.h = ld1.w.l;
            ld1.w.l = tmp;
            ld1.w.l = ld1.l / ld0.w.l;
        }
        pPlayerwk->yspeed.w = ld1.w.l;
        pPlayerwk->xspeed.w = d2;
        if (ld0.w.l < 0) {
            ld0.w.l *= -1;
        }
        pWork->travel_timer = ld0.w.l;
    }
}

#include "../equ.h"
#include "shoot6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../io.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "coli6.h"

static void m_init(sprite_status *megamiwk);
static void m_move0(sprite_status *megamiwk);
static void m_move1(sprite_status *megamiwk);
static void m_move2(sprite_status *megamiwk);
static void ring_set(sprite_status *megamiwk);
static void gas_init(sprite_status *gaswk);
static void gas_move0(sprite_status *gaswk);
static void gas_move1(sprite_status *gaswk);
static void gas_move2(sprite_status *gaswk);
static void gas_move3(sprite_status *gaswk);
static void gas_move4(sprite_status *gaswk);
static void gas_move5(sprite_status *gaswk);
static Sint16 gas_coli_colig(sprite_status *gaswk, sprite_status *plwk);
static void cata_init(sprite_status *catawk);
static void cata_wait(sprite_status *catawk);
static void cata_move0(sprite_status *catawk);
static void cata_move1(sprite_status *catawk);
static void cata_move2(sprite_status *catawk);
static void shooterinit(sprite_status *shootwk);
static void shootermove(sprite_status *shootwk);
static void shootermove2(sprite_status *shootwk);
static void shootermove3(sprite_status *shootwk);

extern sprite_pattern *ringpat[9];
static Sint16 stackpointer;
extern sprite_pattern *bariapat[13];

void megami(sprite_status *megamiwk) {
    void (*tbl[4])(sprite_status *) = {&m_init, &m_move0, &m_move1, &m_move2};

    tbl[megamiwk->r_no0 / 2](megamiwk);
    frameout_s(megamiwk);
}

static void m_init(sprite_status *megamiwk) {
    megamiwk->r_no0 += 2;
    megamiwk->actflg |= 4;
    megamiwk->actfree[1] = 50;
    m_move0(megamiwk);
}

static void m_move0(sprite_status *megamiwk) {
    Sint16 cal_position;

    cal_position = actwk[0].xposi.w.h - megamiwk->xposi.w.h + 16;
    if (cal_position < 0 || cal_position >= 32)
        return;
    cal_position = actwk[0].yposi.w.h - megamiwk->yposi.w.h + 32;
    if (cal_position < 0 || cal_position >= 64)
        return;

    megamiwk->r_no0 += 2;
}

static void m_move1(sprite_status *megamiwk) {
    if ((char)--megamiwk->actfree[0] >= 0)
        return;
    megamiwk->actfree[0] = 10;
    if ((char)--megamiwk->actfree[1] >= 0)
        ring_set(megamiwk);
    else
        megamiwk->r_no0 += 2;
}

static void m_move2(sprite_status *megamiwk) {
    megamiwk->actno = megamiwk->actno;
}

static void ring_set(sprite_status *megamiwk) {
    Sint16 random_data;
    sprite_status *new_actwk;
    Sint16 tbl[8] = {-256, -128, 0, 128, 256, 384, 512, 640};

    if (actwkchk(&new_actwk) != 0)
        return;
    new_actwk->actno = 17;
    new_actwk->r_no0 += 2;
    new_actwk->mstno.b.h = 2;
    new_actwk->sprvsize = new_actwk->sprhs = 8;
    new_actwk->xposi.w.h = megamiwk->xposi.w.h + 24;
    new_actwk->yposi.w.h = megamiwk->yposi.w.h - 16;
    new_actwk->patbase = ringpat;
    new_actwk->sproffset = 32768;
    new_actwk->sprpri = 3;
    new_actwk->actflg = 4;
    new_actwk->colino = 71;
    new_actwk->sprhsize = new_actwk->sprvsize = 8;
    sys_pattim4 = 255;
    new_actwk->yspeed.w = -512;
    random_data = random() & 7;
    new_actwk->xspeed.w = tbl[random_data];
}

static Uint8 gaschg0[7] = {1, 3, 7, 6, 7, 253, 1};
static Uint8 gaschg1[90] = {
    1, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7, 4,   7, 5, 7,
    1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7,   4, 7, 5,
    7, 1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2,   7, 4, 7,
    5, 7, 1, 7, 2, 7, 4, 7, 5, 7, 1, 7, 2, 7, 7, 4, 7, 7, 5, 253, 2};
static Uint8 gaschg2[19] = {7, 7, 7, 2, 7, 7, 7, 5, 7,  7,
                            7, 7, 2, 7, 7, 7, 7, 5, 252};
static Uint8 *gaschg[3] = {gaschg0, gaschg1, gaschg2};
static sprite_pattern gas00 = {1, {{-32, -8, 0, 288}}};
static sprite_pattern gas01 = {1, {{-16, -32, 0, 289}}};
static sprite_pattern gas02 = {1, {{-16, -40, 0, 289}}};
static sprite_pattern gas03 = {1, {{-16, -32, 0, 290}}};
static sprite_pattern gas04 = {1, {{-16, -32, 0, 291}}};
static sprite_pattern gas05 = {1, {{-16, -40, 0, 291}}};
static sprite_pattern gas06 = {1, {{-16, -32, 0, 292}}};
static sprite_pattern gas07 = {1, {{-16, -20, 0, 0}}};
static sprite_pattern ice00 = {1, {{-24, -24, 0, 293}}};
static sprite_pattern ice01 = {1, {{-8, -12, 0, 294}}};
static sprite_pattern ice02 = {1, {{-8, -8, 0, 295}}};
static sprite_pattern ice03 = {1, {{-4, -4, 0, 296}}};
sprite_pattern *gaspat[12] = {&gas00, &gas01, &gas02, &gas03, &gas04, &gas05,
                              &gas06, &gas07, &ice00, &ice01, &ice02, &ice03};

void gas(sprite_status *gaswk) {
    void (*tbl[7])(sprite_status *) = {&gas_init,  &gas_move0, &gas_move1,
                                       &gas_move2, &gas_move3, &gas_move4,
                                       &gas_move5};
    tbl[gaswk->r_no0 / 2](gaswk);
    actionsub(gaswk);
    frameout_s(gaswk);
}

static void gas_init(sprite_status *gaswk) {
    gaswk->actflg |= 4;

    gaswk->patbase = gaspat;
    gaswk->actfree[0] = 120;
    gaswk->r_no0 += 2;
    gas_move0(gaswk);
}

static void gas_move0(sprite_status *gaswk) {
    sprite_status *new_actwk, **parent;

    if (gaswk->actfree[0] == 0)
        return;
    if (--gaswk->actfree[0] != 0)
        return;
    if (actwkchk(&new_actwk) != 0)
        return;
    parent = &((sprite_status **)new_actwk)[12];

    *parent = gaswk;
    new_actwk->actno = 5;
    new_actwk->sprpri = 3;
    new_actwk->actflg |= 4;
    new_actwk->sproffset = gaswk->sproffset;
    new_actwk->patbase = gaswk->patbase;
    new_actwk->xposi.w.h = gaswk->xposi.w.h;
    new_actwk->yposi.w.h = gaswk->yposi.w.h + 36;
    new_actwk->r_no0 = 4;
}

static void gas_move1(sprite_status *gaswk) {
    gas_coli(gaswk);
    patchg(gaswk, gaschg);
}

static void gas_move2(sprite_status *gaswk) {
    sprite_status **parent, *new_actwk;

    parent = &((sprite_status **)gaswk)[12];
    new_actwk = *parent;
    new_actwk->actfree[0] = 120;
    frameout(gaswk);
}

static void gas_move3(sprite_status *gaswk) {
    Sint16 collision_data;
    sprite_status **parent, *new_actwk;

    gaswk->yspeed.w += 56;
    gaswk->yposi.l += gaswk->yspeed.w << 8;
    if ((collision_data = emycol_d(gaswk)) < 0) {
        sub_sync(146);
        gaswk->actfree[6] = 15;
        gaswk->yposi.w.h += collision_data;
        gaswk->r_no0 += 2;
    }
    parent = &((sprite_status **)gaswk)[12];
    new_actwk = *parent;
    new_actwk->yposi.l = gaswk->yposi.l;
}

static void gas_move4(sprite_status *gaswk) {
    sprite_status **parent, *new_actwk;

    parent = &((sprite_status **)gaswk)[12];
    new_actwk = *parent;
    if (gaswk->actfree[6] != 0) {
        --gaswk->actfree[6];
        if (!(swdata.b.l & 112))
            return;
        new_actwk->actfree[2] &= 190;
        new_actwk->yspeed.w = -1664;
        new_actwk->sprvsize = 14;
        new_actwk->sprhs = 7;
        new_actwk->yposi.w.h += 5;
        new_actwk->cddat |= 4;
        new_actwk->cddat &= 223;
        new_actwk->mstno.b.h = 2;
        soundset(146);
    } else {
        new_actwk->actfree[2] &= 190;
        playdamageset(new_actwk, gaswk);
    }
    gaswk->r_no0 += 2;
    gaswk->patno = 10;
    gaswk->actfree[6] = 20;
    gaswk->actfree[7] = 2;
    ice_sub_set(gaswk);
}

static char tbl0[32] = {0, 0, 10, 11, 0, 0, -1, 0, 0, 0, 10, 11, 0, 1,  0, 0,
                        0, 0, 10, 11, 0, 0, 1,  0, 0, 0, 10, 11, 0, -1, 0, 0};
static char tbl1[32] = {0, 0, 10, 11, 0, -1, -1, 0, 0, 0, 10, 11, 0, 1,  -1, 0,
                        0, 0, 10, 11, 0, 0,  1,  0, 0, 0, 1,  11, 0, -1, 0,  0};
static char tbl2[32] = {0, 0, 10, 11, 0, -1, -1, 0, 0, 0, 10, 11, 0, 1,  -1, 0,
                        0, 0, 10, 11, 0, 1,  1,  0, 0, 0, 10, 11, 0, -1, 1,  0};

static void gas_move5(sprite_status *gaswk) {
    char *tbl[3] = {tbl0, tbl1, tbl2};

    if (--gaswk->actfree[6] == 0) {
        if (gaswk->patno == 11) {
            frameout(gaswk);
            return;
        }
        ice_sub0(gaswk, tbl[gaswk->actfree[7]], 3);
        frameout(gaswk);
        return;
    }
    gaswk->xposi.w.h += gaswk->xspeed.w;
    gaswk->yposi.w.h += gaswk->yspeed.w;
}

static char ice_tbl0[48] = {-16, -12, 10, 9, 0,  -1,  -1, 0,  -16, 12,  10, 9,
                            2,   -1,  1,  0, 16, -12, 10, 9,  1,   1,   -1, 0,
                            16,  12,  10, 9, 3,  1,   1,  0,  0,   -16, 15, 10,
                            1,   0,   -1, 1, 0,  16,  15, 10, 3,   0,   1,  1};

void ice_sub_set(sprite_status *gaswk) { ice_sub0(gaswk, ice_tbl0, 5); }

void ice_sub0(sprite_status *gaswk, char *tbl, Sint16 loop) {
    Sint16 i, index;
    sprite_status *new_actwk;

    index = 0;
    for (i = 0; i <= loop; ++i) {
        if (actwkchk(&new_actwk) != 0)
            break;
        new_actwk->actno = 5;
        new_actwk->r_no0 = 12;
        new_actwk->actflg |= 4;
        new_actwk->xposi.w.h = gaswk->xposi.w.h;
        new_actwk->yposi.w.h = gaswk->yposi.w.h;

        new_actwk->patbase = gaspat;
        new_actwk->xposi.w.h += tbl[index];
        new_actwk->yposi.w.h += tbl[index + 1];
        new_actwk->actfree[6] = tbl[index + 2];
        new_actwk->patno = tbl[index + 3];
        new_actwk->actflg |= tbl[index + 4];
        new_actwk->xspeed.w = tbl[index + 5];
        new_actwk->yspeed.w = tbl[index + 6];
        new_actwk->actfree[7] = tbl[index + 7];
        index += 8;
    }
}

void ice_set(sprite_status *plwk) {
    sprite_status *new_actwk, **parent;

    if (actwkchk(&new_actwk) != 0)
        return;
    plwk->actfree[2] |= 65;
    parent = &((sprite_status **)new_actwk)[12];
    *parent = plwk;
    new_actwk->actno = 5;
    new_actwk->actflg |= 4;
    new_actwk->xposi.w.h = plwk->xposi.w.h;
    new_actwk->yposi.w.h = plwk->yposi.w.h;

    new_actwk->patbase = gaspat;
    new_actwk->sprhs = new_actwk->sprhsize = new_actwk->sprvsize = 24;
    new_actwk->patno = 8;
    new_actwk->r_no0 = 8;
}

void gas_coli(sprite_status *gaswk) {
    if (gaswk->mstno.b.h != 1)
        return;
    if (actwk[0].mstno.b.h == 43)
        return;
    if (gas_coli_colig(gaswk, &actwk[0]))
        ice_set(&actwk[0]);
}

static Sint16 gas_coli_colig(sprite_status *gaswk, sprite_status *plwk) {
    Sint16 cal_posi, cal_size;

    if (plpower_m)
        return 0;
    if (plpower_a)
        return 0;
    if (plwk->r_no0 >= 4)
        return 0;
    if (plwk->actfree[2])
        return 0;
    cal_posi = plwk->xposi.w.h - gaswk->xposi.w.h +
               (cal_size = (Sint16)plwk->sprhs + 16);

    if (cal_posi < 0 || cal_posi >= cal_size * 2)
        return 0;
    cal_posi = plwk->yposi.w.h - gaswk->yposi.w.h +
               (cal_size = (Sint16)plwk->sprvsize + 32);

    if (cal_posi < 0 || cal_posi >= cal_size * 2)
        return 0;
    return 1;
}

static sprite_pattern cata00 = {1, {{-28, -28, 0, 297}}};
static sprite_pattern cata01 = {1, {{-16, -20, 0, 0}}};
sprite_pattern *cata_pat[2] = {&cata00, &cata01};

void catapalt(sprite_status *catawk) {
    void (*tbl[5])(sprite_status *) = {&cata_init, &cata_wait, &cata_move0,
                                       &cata_move1, &cata_move2};

    tbl[catawk->r_no0 / 2](catawk);
    actionsub(catawk);
    frameout_s(catawk);
}

static void cata_init(sprite_status *catawk) {
    sprite_status *new_actwk, **parent;

    catawk->actflg |= 4;

    catawk->patbase = cata_pat;
    ((Sint16 *)catawk)[25] = catawk->xposi.w.h;
    catawk->sprhs = catawk->sprhsize = 28;
    catawk->sprvsize = 4;
    catawk->r_no0 += 2;
    if (actwkchk2(catawk, &new_actwk) != 0)
        return;
    new_actwk->actno = 4;
    new_actwk->actflg |= 4;

    new_actwk->patbase = cata_pat;
    new_actwk->sprhs = 4;
    new_actwk->sprvsize = 12;
    new_actwk->patno = 1;
    parent = &((sprite_status **)new_actwk)[12];
    *parent = catawk;
    new_actwk->r_no0 = 8;
    cata_wait(catawk);
}

static void cata_wait(sprite_status *catawk) {
    if (ridechk(catawk, &actwk[0]) == 0)
        return;
    actwk[0].actfree[2] |= 1;
    actwk[0].xposi.w.h = catawk->xposi.w.h;
    actwk[0].cddat &= 254;
    actwk[0].mstno.b.h = 58;
    catawk->r_no0 += 2;
    catawk->xspeed.w = 3072;
}

static void cata_move0(sprite_status *catawk) {
    Sint16 cal_position;

    catawk->xposi.l += catawk->xspeed.w << 8;
    ridechk(catawk, &actwk[0]);
    if ((swdata1.b.h & 112) && (actwk[0].actfree[2] & 1)) {

        actwk[0].actfree[2] &= 254;

        actwk[0].yspeed.w = -1664;
        actwk[0].xspeed.w = catawk->xspeed.w;
        actwk[0].sprvsize = 14;
        actwk[0].sprhs = 7;
        actwk[0].yposi.w.h += 5;
        actwk[0].cddat |= 4;
        actwk[0].cddat |= 2;

        actwk[0].cddat &= 223;
        actwk[0].mstno.b.h = 2;
        soundset(146);
    }
    cal_position = ((Sint16 *)catawk)[25] + 912;
    if (cal_position >= catawk->xposi.w.h)
        return;
    catawk->xposi.w.h = cal_position;
    catawk->r_no0 += 2;
    if (!(catawk->cddat & 8))
        return;
    actwk[0].actfree[2] &= 254;
    actwk[0].xspeed.w = catawk->xspeed.w;
    actwk[0].mstno.b.h = 0;
    actwk[0].cddat |= 2;
    actwk[0].cddat &= 247;
}

static void cata_move1(sprite_status *catawk) {
    Sint16 cal_position;

    catawk->xposi.w.h -= 4;
    cal_position = ((Sint16 *)catawk)[25];
    if (cal_position < catawk->xposi.w.h)
        return;
    catawk->xposi.w.h = cal_position;
    catawk->r_no0 = 2;
}

static void cata_move2(sprite_status *catawk) {
    sprite_status *new_actwk, **parent;

    parent = &((sprite_status **)catawk)[12];
    new_actwk = *parent;
    if (new_actwk->r_no0 >= 4)
        return;
    catawk->xposi.w.h = new_actwk->xposi.w.h - 24;
    catawk->yposi.w.h = new_actwk->yposi.w.h - 16;
    hitchk(catawk, &actwk[0]);
}

static Uint16 shooterposi_0[46] = {
    78,    0,     0,     0,     8,     32,    0,     0,  72,    0,
    65512, 96,    0,     65464, 112,   0,     65416, 96, 0,     65392,
    72,    0,     65384, 32,    0,     65392, 0,     0,  65416, 65504,
    32769, 65464, 65488, 0,     65512, 65504, 32768, 0,  0,     32768,
    80,    65504, 0,     65416, 65432, 0};
static Uint16 shooterposi_1[16] = {24, 0,     0,   0,  48,    65520, 0, 96,
                                   0,  32768, 120, 32, 32768, 200,   0, 0};
static Uint16 shooterposi_2[85] = {
    150,   0,     0,     0,     8,     32,    0,     0,   64,    0,     65512,
    96,    0,     65464, 112,   0,     65416, 128,   0,   65392, 160,   0,
    65384, 192,   0,     65392, 224,   32770, 65416, 256, 32770, 65464, 272,
    0,     65512, 256,   0,     0,     224,   0,     8,   192,   0,     0,
    160,   0,     65512, 128,   0,     65464, 112,   0,   65416, 96,    0,
    65392, 64,    0,     65384, 32,    0,     65392, 0,   32769, 65416, 65504,
    32769, 65464, 65488, 0,     65512, 65504, 32768, 0,   0,     32768, 64,
    65504, 0,     65320, 65504, 0,     65352, 240,   0};
static Uint16 shooterposi_3[22] = {
    30,    0,     0,   32769, 24,    65504, 32769, 72, 65488, 0,     120,
    65504, 32768, 144, 0,     32768, 208,   65504, 0,  65464, 65504, 0};
static Uint16 shooterposi_4[49] = {
    78,    0,     0,     32770, 24,    32,    32770, 72,    48,    0,
    120,   32,    0,     144,   0,     0,     152,   65504, 0,     144,
    65472, 0,     120,   65440, 0,     72,    65424, 0,     24,    65408,
    0,     0,     65376, 0,     65528, 65344, 0,     0,     65312, 32769,
    208,   65280, 0,     65464, 65280, 0,     65496, 16,    0};
static Uint16 shooterposi_5[31] = {60, 0,     0,   0, 0,     336, 0, 65432, 336,
                                   0,  65432, 512, 0, 65528, 512, 0, 65480, 512,
                                   0,  65480, 808, 0, 536,   808, 0, 536,   880,
                                   0,  960,   880, 0};
static Uint16 shooterposi_6[7] = {12, 0, 0, 0, 56, 0, 0};
static Uint16 shooterposi_7[10] = {18, 0, 0, 0, 0, 680, 0, 80, 680, 0};
static Uint16 shooterposi_8[13] = {24, 0,     0, 0,  8,     0, 0,
                                   8,  65280, 0, 56, 65280, 0};
static Uint16 shooterposi_9[13] = {24, 0, 0, 0, 8, 0, 0, 8, 256, 0, 56, 256, 0};
static Uint16 shooterposi_0a[13] = {24, 0,     0, 0,  8,     0, 0,
                                    8,  65280, 0, 56, 65280, 0};
static Uint16 shooterposi_0b[7] = {12, 0, 0, 0, 96, 0, 0};
static Uint16 shooterposi_0c[13] = {24,  0,   0, 0,   432, 0, 0,
                                    432, 256, 0, 992, 256, 0};
static Uint16 shooterposi_0d[16] = {30,  0, 0,   0,   0, 680, 0,   960,
                                    680, 0, 960, 424, 0, 400, 424, 0};
static Uint16 shooterposi_0e[7] = {12, 0, 0, 0, 352, 0, 0};
static Uint16 shooterposi_0f[40] = {
    78,    0,   0,     0,     0,     696,   0,     65416, 696,  0,
    65416, 872, 0,     65512, 872,   0,     65464, 952,   0,    65368,
    952,   0,   65464, 952,   0,     65464, 1016,  0,     24,   1016,
    0,     24,  1112,  0,     65480, 1112,  0,     65480, 1248, 0};
static Uint16 shooterposi_10[22] = {
    42,  0, 0,     0,   0, 680,   0,   65416, 680,   0,    65416,
    856, 0, 65512, 856, 0, 65464, 856, 0,     65464, 1232, 0};
static Uint16 shooterposi_11[25] = {
    48,    0,   0, 0,     0,   520, 0,     65416, 520, 0,   65416, 696, 0,
    65512, 696, 0, 65480, 696, 0,   65480, 792,   0,   592, 792,   0};
static Uint16 shooterposi_12[7] = {12, 0, 0, 0, 296, 0, 0};
static Uint16 *shooterpositbl[19] = {
    shooterposi_0,  shooterposi_1,  shooterposi_2,  shooterposi_3,
    shooterposi_4,  shooterposi_5,  shooterposi_6,  shooterposi_7,
    shooterposi_8,  shooterposi_9,  shooterposi_0a, shooterposi_0b,
    shooterposi_0c, shooterposi_0d, shooterposi_0e, shooterposi_0f,
    shooterposi_10, shooterposi_11, shooterposi_12};

void shooter(sprite_status *shootwk) {
    Sint16 flag_no, addition;

    if (!(time_flag & 128))
        goto label1;
    if ((flag_no = shootwk->cdsts) == 0)
        goto label1;
    flag_no *= 3;
    addition = (Sint16)(time_flag & 127) - (Sint16)time_item;
    if (addition < 0)
        addition = 0;
    else if (addition >= 3)
        addition = 2;
    flagwork[flag_no + addition] &= 127;
label1:
    if (actwk[0].mstno.b.h == 43)
        return;
    if (actwk[0].r_no0 >= 6)
        return;
    stackpointer = 0;
    switch (shootwk->r_no0) {
    case 0:
        shooterinit(shootwk);
        break;
    case 2:
        shootermove(shootwk);
        break;
    case 4:
        shootermove2(shootwk);
        break;
    case 6:
        shootermove3(shootwk);
        break;
    }

    (Sint32) stackpointer;
}

static void shooterinit(sprite_status *shootwk) {
    Uint16 **move;
    Uint16 *movetbl;

    shootwk->patbase = bariapat;
    shootwk->actflg = 4;
    shootwk->sprpri = 1;
    shootwk->sprhsize = 16;

    shootwk->r_no0 += 2;
    movetbl = shooterpositbl[shootwk->userflag.b.h & 127];
    shootwk->actfree[16] = movetbl[0] >> 8;
    shootwk->actfree[17] = movetbl[0] & 255;
    move = &((Uint16 **)shootwk)[16];
    *move = movetbl + 1;
    ((Uint16 *)shootwk)[29] = movetbl[1];
    ((Uint16 *)shootwk)[30] = movetbl[2];
    shootermove(shootwk);
}

static void shootermove(sprite_status *shootwk) {
    Sint16 cal_position;

    if (actwk[0].r_no0 >= 6)
        return;
    cal_position = actwk[0].xposi.w.h - shootwk->xposi.w.h + 32;
    if (cal_position >= 64 || cal_position < 0)
        return;
    cal_position = actwk[0].yposi.w.h - shootwk->yposi.w.h + 48;
    if (cal_position >= 96 || cal_position < 0)
        return;
    if (actwk[0].actfree[2] != 0)
        return;
    if (actwk[0].r_no0 == 4) {
        actwk[0].r_no0 -= 2;
        ((Sint16 *)&actwk[0])[26] = 120;
    }
    shootwk->r_no0 += 2;
    actwk[0].actfree[2] = 129;
    if (shootwk->userflag.b.l != 0)
        actwk[0].actfree[2] |= 64;
    actwk[0].mstno.b.h = 2;
    actwk[0].mspeed.w = 2048;
    if (shootwk->userflag.b.h >= 0)
        actwk[0].actflg &= 191;
    else
        actwk[0].actflg |= 64;
    if (actwk[0].r_no0 == 4)
        actwk[0].r_no0 -= 2;
    actwk[0].xspeed.w = actwk[0].yspeed.w = 0;
    shootwk->cddat &= 223;
    actwk[0].cddat &= 223;
    actwk[0].cddat |= 2;
    actwk[0].xposi.w.h = shootwk->xposi.w.h;
    actwk[0].yposi.w.h = shootwk->yposi.w.h;
    shootwk->actfree[8] = 0;
    soundset(145);
}

static void shootermove2(sprite_status *shootwk) {
    actwk[0].mstno.b.h = 2;
    shooterspdset(shootwk);
    shootwk->r_no0 += 2;
    soundset(145);
}

static void shootermove3(sprite_status *shootwk) {
    Uint8 move_counter;
    Sint16 movetbl_data;
    Uint16 **move;
    Uint16 *movetbl;

    actwk[0].mstno.b.h = 2;
    stackpointer = 4;
    if ((char)--shootwk->actfree[4] < 0) {
        actwk[0].xposi.w.h = ((Sint16 *)shootwk)[29] + shootwk->xposi.w.h;
        actwk[0].yposi.w.h = ((Sint16 *)shootwk)[30] + shootwk->yposi.w.h;
        move_counter = shootwk->actfree[16] + 6;
        if (move_counter >= shootwk->actfree[17])
            goto label2;

        shootwk->actfree[16] = move_counter;
    label1:
        move = &((Uint16 **)shootwk)[16];
        movetbl = *move;
        ((Uint16 *)shootwk)[29] = movetbl[move_counter / 2];
        ((Uint16 *)shootwk)[30] = movetbl[move_counter / 2 + 1];
        shooterspdset(shootwk);
        return;
    }

    actwk[0].xposi.l += actwk[0].xspeed.w << 8;
    actwk[0].yposi.l += actwk[0].yspeed.w << 8;
    move = &((Uint16 **)shootwk)[16];
    movetbl = *move;
    movetbl_data = movetbl[shootwk->actfree[16] / 2 + 2];
    if (movetbl_data >= 0)
        return;
    if (!(swdata1.b.h & 112))
        return;
    movetbl_data &= 32767;
    move_counter = movetbl_data;
    move_counter = shootwk->actfree[17] + move_counter * 6;
    shootwk->actfree[16] = move_counter;
    goto label1;

label2:
    actwk[0].yposi.w.h &= 2047;
    shootwk->r_no0 = 0;
    actwk[0].actfree[2] = 0;
}

void shooterspdset(sprite_status *shootwk) {
    Sint16 ms_work0, ms_work1, cal_position_x, cal_position_y;
    Sint32 cal_long_x, cal_long_y;

    ms_work0 = ms_work1 = actwk[0].mspeed.w;
    cal_position_x = ((Sint16 *)shootwk)[29] + shootwk->xposi.w.h;
    cal_position_x -= actwk[0].xposi.w.h;
    if (cal_position_x < 0) {
        cal_position_x = -cal_position_x;
        ms_work0 = -ms_work0;
    }
    cal_position_y = ((Sint16 *)shootwk)[30] + shootwk->yposi.w.h;
    cal_position_y -= actwk[0].yposi.w.h;
    if (cal_position_y < 0) {
        cal_position_y = -cal_position_y;
        ms_work1 = -ms_work1;
    }
    if (cal_position_x <= cal_position_y) {

        cal_position_y = ((Sint16 *)shootwk)[30] + shootwk->yposi.w.h;
        cal_position_y -= actwk[0].yposi.w.h;
        cal_long_y = cal_position_y << 16;
        cal_long_y /= ms_work1;

        cal_position_x = ((Sint16 *)shootwk)[29] + shootwk->xposi.w.h;
        cal_position_x -= actwk[0].xposi.w.h;
        cal_long_x = cal_position_x << 16;
        if (cal_long_x != 0)
            cal_long_x /= cal_long_y;

        actwk[0].xspeed.w = cal_long_x;
        actwk[0].yspeed.w = ms_work1;
        if (cal_long_y < 0)
            cal_long_y = -cal_long_y;
        shootwk->actfree[4] = cal_long_y >> 8;
        shootwk->actfree[5] = cal_long_y & 255;
        return;
    }

    cal_position_x = ((Sint16 *)shootwk)[29] + shootwk->xposi.w.h;
    cal_position_x -= actwk[0].xposi.w.h;
    cal_long_x = cal_position_x << 16;
    cal_long_x /= ms_work0;

    cal_position_y = ((Sint16 *)shootwk)[30] + shootwk->yposi.w.h;
    cal_position_y -= actwk[0].yposi.w.h;
    cal_long_y = cal_position_y << 16;
    if (cal_long_y != 0)
        cal_long_y /= cal_long_x;

    actwk[0].yspeed.w = cal_long_y;
    actwk[0].xspeed.w = ms_work0;
    if (cal_long_x < 0)
        cal_long_x = -cal_long_x;
    shootwk->actfree[4] = cal_long_x >> 8;
    shootwk->actfree[5] = cal_long_x & 255;
}

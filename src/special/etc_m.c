#include "../types.h"
#include "common.h"
#include "spm_equ.h"
#include "etc_m.h"

static Sint16 sonic_upcnt = 0;
static Sint16 sonic_upcnt2 = 0;
static Sint32 subtbl[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
static Uint16 chibi_sonic_map[2][3][2] = {{{863, 864}, {869, 870}, {871, 872}},
                                          {{863, 864}, {869, 870}, {871, 873}}};
extern void (*SetGrid)(Sint32, Sint32, Sint32, Sint32, Sint32);
extern Sint32 time_bonus;
static Uint8 nullflg = 0;
extern Sint32 ring_bonus;
static Uint16 disp_ascii[20] = {503, 505, 507, 509, 510, 512, 514,
                                515, 516, 517, 504, 506, 508, 504,
                                511, 513, 504, 506, 504, 504};
static Sint32 chrtbl[24] = {32769, 32770, 32771, 32772, 32773, 32774,
                            32775, 32776, 32777, 32769, 32778, 32779,
                            32780, 32781, 32782, 32783, 32784, 32785,
                            32786, 32787, 32788, 32789, 0,     0};

void sonic_disp(void) {
    Uint16 sonic_extend;

    if ((sonic_upcnt2 = sonic_upcnt2 + 1 & 7) == 0) {
        sonic_upcnt = sonic_upcnt + 1 & 1;
        if ((sonic_extend = sonicup_no) != 0) {
            if (--sonic_extend >= 5)
                sonic_extend = 4;
            switch (sonic_extend) {
            case 4:
                sonic_disp_disp(8);
            case 3:
                sonic_disp_disp(6);
            case 2:
                sonic_disp_disp(4);
            case 1:
                sonic_disp_disp(2);
            case 0:
                sonic_disp_disp(0);
            }
        }
    }
}

void sonic_disp_disp(Sint16 tbl_no) {
    Sint32 x;
    Sint32 y;

    for (y = 0; y < 3; ++y) {
        for (x = 0; x < 2; ++x) {
            SetGrid(0, x + 26 + tbl_no, y + 23,
                    chibi_sonic_map[sonic_upcnt][y][x], 0);
        }
    }
}

void time_bonus_disp(void) {
    nullflg = 0;

    cset(subtbl, 24, 20, time_bonus, 7);
}

void ring_bonus_disp(void) {
    nullflg = 0;

    cset(subtbl, 24, 17, ring_bonus, 7);
}

void game_score_disp(void) {
    nullflg = 0;

    cset(subtbl, 24, 14, game_score, 7);
}

void cset(Sint32 *subtbl, Uint16 x, Uint16 y, Sint32 disp_data,
          Sint16 counter) {
    Sint16 one_number, i;

    if ((disp_data *= 10) != 0) {
        for (i = 0; i <= counter; ++i) {
            one_number = disp_data / subtbl[i];

            disp_data %= subtbl[i];
            if (nullflg != 0 || one_number != 0) {

                nullflg = 1;
                SetGrid(0, x, y, disp_ascii[one_number], 0);
                SetGrid(0, x, y + 1, disp_ascii[one_number + 10], 0);
            } else {
                SetGrid(0, x, y, 0, 0);
                SetGrid(0, x, y + 1, 0, 0);
            }
            ++x;
        }
    } else {

        for (i = 0; i < counter; ++i) {

            SetGrid(0, x, y, 0, 0);
            SetGrid(0, x, y + 1, 0, 0);
            ++x;
        }

        SetGrid(0, x, y, disp_ascii[0], 0);
        SetGrid(0, x, y + 1, disp_ascii[10], 0);
    }
}

Uint16 cntminus(Uint16 *work, Uint16 minus_data, Uint16 under_limit,
                Uint16 upper_limit) {
    if ((Sint16)(*work -= minus_data) < (Sint16)under_limit)
        *work = upper_limit;
    return *work;
}

Uint16 cntplus(Uint16 *work, Uint16 plus_data, Uint16 under_limit,
               Uint16 upper_limit) {
    if ((*work += plus_data) > upper_limit)
        *work = under_limit;
    return *work;
}

void spetime_disp(void) {
    if (!(spgmmode & 2)) {

        sset(&subtbl[5], 19, 0, spe_time.l, 2);
    } else {
        sset(&subtbl[6], 17, 0, spe_time.l >> 16 & 255, 1);
        sset(&subtbl[6], 20, 0, spe_time.l >> 8 & 255, 1);
        sset(&subtbl[6], 23, 0, (spe_time.l & 255) * 100 / 60, 1);
    }
}

void ringno_disp(void) { sset(&subtbl[5], 33, 0, ringno, 2); }

void ufoleft_disp(void) { sset(&subtbl[6], 7, 0, ufoleft, 1); }

void sset(Sint32 *subtbl, Uint16 x, Uint16 y, Sint32 disp_data,
          Sint16 counter) {
    Sint16 one_number, i = 0;
    Sint32 disp_x, disp_y;

    disp_x = (Sint16)x, disp_y = (Sint16)y;

    do {
        one_number = disp_data / subtbl[i];
        disp_data %= subtbl[i++];
        SetGrid(0, disp_x, disp_y, chrtbl[one_number], 0);
        SetGrid(0, disp_x, disp_y + 1, chrtbl[one_number + 12], 0);
        ++disp_x;
    } while (i <= counter);
}

void rfill(Uint32 *address, Uint32 data, Sint16 size) {
    for (size /= 4; size >= 0; --size)
        *address++ = data;
}

void rcopy(Uint32 *address1, Uint32 *address2, Sint16 size) {
    for (size /= 4; size >= 0; --size)
        *address2++ = *address1++;
}

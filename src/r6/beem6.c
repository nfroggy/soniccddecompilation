#include <stddef.h>

#include "../equ.h"
#include "beem6.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[4];
    Uint8 flash_toggle;
    Uint8 unused5;
    Sint16 pre_flash_timer;
    Uint8 unused8[8];
    Sint16 cycle_timer;
    Uint8 phase;
    Uint8 unused19;
    union {
        Sint16 palette_indices_word;
        struct {
            Uint8 palette_index;
            Uint8 secondary_palette_index;
        };
    };
} beem6_work;
#pragma pack(pop)

_Static_assert(offsetof(beem6_work, flash_toggle) == 4,
               "beem6_work.flash_toggle offset");
_Static_assert(offsetof(beem6_work, pre_flash_timer) == 6,
               "beem6_work.pre_flash_timer offset");
_Static_assert(offsetof(beem6_work, cycle_timer) == 16,
               "beem6_work.cycle_timer offset");
_Static_assert(offsetof(beem6_work, phase) == 18,
               "beem6_work.phase offset");
_Static_assert(offsetof(beem6_work, palette_indices_word) == 20,
               "beem6_work.palette_indices_word offset");
_Static_assert(offsetof(beem6_work, palette_index) == 20,
               "beem6_work.palette_index offset");
_Static_assert(offsetof(beem6_work, secondary_palette_index) == 21,
               "beem6_work.secondary_palette_index offset");
_Static_assert(sizeof(beem6_work) <= sizeof(((sprite_status *)0)->actfree),
               "beem6_work fits in actfree");

static beem6_work *beem6_get_work(sprite_status *actionwk) {
    return (beem6_work *)actionwk->actfree;
}

static sprite_pattern beem6pat0 = {1, {{-24, -24, 0, 417}}};
static sprite_pattern nullpat = {1, {{0, 0, 0, 0}}};
sprite_pattern *beem6pat[2] = {&beem6pat0, &nullpat};
Sint16 clrtblA[15] = {0, 0, 3808, 3808, 0, 0, 238, 238,
                      0, 0, 3598, 3598, 0, 0, -1};
Sint16 clrtblA2[9] = {2656, 2656, 2720, 2720, 2656, 2656, 2720, 2720, -1};
Sint16 clrtblB[15] = {0, 0, 3808, 3808, 0, 0, 238, 238,
                      0, 0, 3598, 3598, 0, 0, -1};
Sint16 clrtblB2[9] = {2118, 2118, 2122, 2122, 2118, 2118, 2122, 2122, -1};
Sint16 clrtblD[15] = {0, 0, 3808, 3808, 0, 0, 238, 238,
                      0, 0, 3598, 3598, 0, 0, -1};
Sint16 clrtblD2[9] = {580, 580, 584, 584, 580, 580, 584, 584, -1};

void beem6(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        beem6_init(actionwk);
    case 2:
        beem6_wait(actionwk);
        break;
    case 4:
        beem6_move(actionwk);
    }

    actionsub(actionwk);
    frameout_s(actionwk);
}

void beem6_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 4;
    actionwk->sproffset = 25432;
    actionwk->patbase = beem6pat;
    actionwk->sprvsize = 16;
    actionwk->sprhsize = 16;
    actionwk->patno = actionwk->userflag.b.h;
    beem6_actsch();

    clrset0_2();
}

void beem6_wait(sprite_status *actionwk) {
    beem6_work *work = beem6_get_work(actionwk);
    Sint16 d0;

    if (actionwk->userflag.b.h == 0)
        return;
    beem6_posiset(actionwk);
    if (work->cycle_timer != 0) {
        cntdwn(actionwk);
        return;
    }
    clrset0();
    if (time_flag == 2) {
        if (generate_flag)
            return;
    }

    if (stageno.b.h + 1 == 0) {
        if (scra_v_posit.w.h >= 1024)
            return;
    }

    d0 = 360;
    if (time_flag != 0) {
        d0 = 480;
        if (time_flag - 1 != 0)
            d0 = 240;
    }

    work->cycle_timer = d0;
}

void cntdwn(sprite_status *actionwk) {
    beem6_work *work = beem6_get_work(actionwk);

    if (--work->cycle_timer)
        return;
    actionwk->r_no0 += 2;
    work->cycle_timer = 120;
    work->pre_flash_timer = 90;
    work->flash_toggle = 0;

    if (!(actionwk->actflg & 128)) {
        beem6_move(actionwk);
        return;
    }
    soundset(178);
    beem6_move(actionwk);
}

void beem6_move(sprite_status *actionwk) {
    beem6_work *work = beem6_get_work(actionwk);

    beem6_posiset(actionwk);
    if (work->pre_flash_timer != 0) {
        maeclrset(actionwk, work->phase);
        if (--work->pre_flash_timer)
            return;
        clrset0();
    }

    st6clrchg = work->phase + 1;
    clrset1(actionwk, work->phase);
    if (--work->cycle_timer)
        return;
    actionwk->r_no0 -= 2;
    st6clrchg = 0;
    if (++work->phase < 3)
        return;
    work->phase = 0;
    work->palette_indices_word = 0;
}

void beem6_posiset(sprite_status *actionwk) {
    actionwk->xposi.w.h = actwk[0].xposi.w.h;
    actionwk->yposi.w.h = actwk[0].yposi.w.h;
}

void beem6_actsch(void) {
    sprite_status *a1;
    Sint16 d0;

    a1 = &actwk[32];

    for (d0 = 0; d0 < 96; ++d0, ++a1) {
        if (a1->actno == 33)
            if (a1->userflag.b.h)
                return;
    }
    if (actwkchk(&a1) != 0)
        return;
    a1->actno = 33;
    a1->userflag.b.h = 1;
    a1->xposi.w.h = actwk[0].xposi.w.h;
    a1->yposi.w.h = actwk[0].yposi.w.h;
}

void clrset0(void) {
    PALETTEENTRY *lpPe_a2, *lpPe_a3;
    PALETTEENTRY dummy;
    Sint16 d0, d2;

    lpPe_a3 = &lpcolorwk[32];
    d0 = 1574;
    d2 = 1606;
    if (time_flag == 0)
        goto label1;
    lpPe_a3 = &lpcolorwk[61];
    d0 = 546;
    d2 = 1664;
    if (time_flag - 1 == 0)
        goto label1;
    d0 = 1026;
    d2 = 582;
    if (generate_flag != 0)
        return;

label1:
    lpPe_a2 = &lpcolorwk[50];
    dummy.peRed = (d0 & 15) * 16;
    dummy.peGreen = d0 & 240;
    dummy.peBlue = (d0 & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a2++ = dummy;
    *lpPe_a2++ = dummy;
    *lpPe_a2++ = dummy;

    dummy.peRed = (d2 & 15) * 16;
    dummy.peGreen = d2 & 240;
    dummy.peBlue = (d2 & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a3++ = dummy;
}

void clrset0_2(void) {
    PALETTEENTRY *lpPe_a2, *lpPe_a3;
    PALETTEENTRY dummy;
    Sint16 d0, d2;

    lpPe_a3 = &lpcolorwk2[32];
    d0 = 1574;
    d2 = 1606;
    if (time_flag == 0)
        goto label1;
    lpPe_a3 = &lpcolorwk2[61];
    d0 = 546;
    d2 = 1664;
    if (time_flag - 1 == 0)
        goto label1;
    d0 = 1026;
    d2 = 582;
    if (generate_flag != 0)
        return;

label1:
    lpPe_a2 = &lpcolorwk2[50];
    dummy.peRed = (d0 & 15) * 16;
    dummy.peGreen = d0 & 240;
    dummy.peBlue = (d0 & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a2++ = dummy;
    *lpPe_a2++ = dummy;
    *lpPe_a2++ = dummy;

    dummy.peRed = (d2 & 15) * 16;
    dummy.peGreen = d2 & 240;
    dummy.peBlue = (d2 & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a3++ = dummy;
}

void clrset1(sprite_status *actionwk, char d0) {

    beem6_work *work = beem6_get_work(actionwk);
    char clrsel[3] = {0, 2, 4};
    PALETTEENTRY *lpPe_a2;
    PALETTEENTRY dummy;
    Sint16 *a1;
    Sint16 col;

    d0 = clrsel[d0] >> 1;
    lpPe_a2 = &lpcolorwk[d0 + 50];
    a1 = clrtblB;
    if (time_flag != 0) {
        a1 = clrtblA;
        if (time_flag - 1 != 0)
            a1 = clrtblD;
    }

    col = a1[work->palette_index];
    dummy.peRed = (col & 15) * 16;
    dummy.peGreen = col & 240;
    dummy.peBlue = (col & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a2++ = dummy;
    col = a1[++work->palette_index];
    if (col == -1)
        work->palette_index = 0;

    lpPe_a2 = &lpcolorwk[32];
    a1 = clrtblB2;
    if (time_flag != 0) {
        lpPe_a2 = &lpcolorwk[61];
        a1 = clrtblA2;
        if (time_flag - 1 != 0)
            a1 = clrtblD2;
    }

    col = a1[work->secondary_palette_index];
    dummy.peRed = (col & 15) * 16;
    dummy.peGreen = col & 240;
    dummy.peBlue = (col & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a2++ = dummy;
    col = a1[++work->secondary_palette_index];
    if (col == -1)
        work->secondary_palette_index = 0;
}

void maeclrset(sprite_status *actionwk, char d0) {
    beem6_work *work = beem6_get_work(actionwk);
    char clrsel[3] = {0, 2, 4};
    PALETTEENTRY *lpPe_a2;
    PALETTEENTRY dummy;
    Sint16 col;

    col = clrsel[d0] >> 1;
    lpPe_a2 = &lpcolorwk[col + 50];

    col = 128;

    if (work->flash_toggle & 2)
        col = 0;

    dummy.peRed = (col & 15) * 16;
    dummy.peGreen = col & 240;
    dummy.peBlue = (col & 3840) >> 4;
    dummy.peFlags = 1;
    *lpPe_a2++ = dummy;

    ++work->flash_toggle;
    work->flash_toggle &= 3;
}

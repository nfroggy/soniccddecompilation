#include "../types.h"
#include "sps_equ.h"
#include "act_s.h"
#include "ens.h"
#include "../impfuncs.h"
#include "pls.h"

static void (*act_tbl[16])(sprite_status_sp *) = {
    &player,    &ufo0,       &timeufo, &item_ring,  &kage,      &plkage,
    &kemuri,    &sibuki,     &press,   &title_logo, &title_obi, &explosion,
    &bara_ring, &time_stone, &tskira0, &tskira1};
extern bmp_info SprBmp[700];

void action(void) {
    Sint16 i;

    linkdata = 0;
    zbuf_init();

    for (i = 0; i < 14; ++i)
        actmain(&actwk[i + 2]);

    actmain(&actwk[0]);
    actmain(&actwk[1]);

    for (i = 0; i < 24; ++i)
        actmain(&actwk[i + 16]);

    zbuf_patset();

    for (i = 0; i < 8; ++i)
        actmain(&actwk[i + 40]);

    for (i = (Uint16)linkdata; i < 80; ++i) {
        EAsprset(0, 0, 0, i, 0);
    }
}

void actmain(sprite_status_sp *work) {
    if (work->actno == 0)
        return;
    act_tbl[work->actno - 1](work);
    if ((work->actflg & 1) == 0)
        return;
    sMemSet(work, 0, sizeof(*work));
}

void zbuf_init(void) {
    Sint16 i;

    for (i = 0; i < 512; i += 8)
        z_buffer[i] = 0;
}

void zbuf_set(sprite_status_sp *work, Sint32 length) {
    Sint16 i = 0;

    if (length >= 4096)
        length = 4095;
    length = (length >> 2 & 1008) / 2;

    for (i = 0; i < 7; ++i) {
        if (z_buffer[length + i] == 0)
            break;
    }
    z_buffer[length + i] = work - actwk;
    z_buffer[length + i + 1] = 0;
}

void zbuf_patset(void) {
    Sint16 i;

    for (i = 0; i < 512; i += 8)
        zpatset(i);
}

void zpatset(Sint16 z_buffer_start) {
    Sint16 i;

    for (i = 0; i < 8; ++i)
        if (zpatset_call(z_buffer_start + i) != 0)
            break;
    ;
}

Sint16 zpatset_call(Sint16 z_buffer_number) {
    Uint16 actwk_number;
    if ((actwk_number = z_buffer[z_buffer_number]) == 0)
        return 1;

    patset(&actwk[actwk_number]);
    return 0;
}

void patinit(sprite_status_sp *work, Uint8 pattbl_number) {
    sprite_patterns_sp **patchg_tbl;
    sprite_patterns_sp *patchg;

    work->pattblno = pattbl_number;
    work->patno = 0;
    patchg_tbl = work->pattbl;
    patchg = patchg_tbl[pattbl_number];
    work->pctime = work->pctime_b = patchg->timer;
}

void patinit1(sprite_status_sp *work, Uint8 pattbl_number) {
    sprite_patterns_sp **patchg_tbl;
    sprite_patterns_sp *patchg;

    work->pattblno = pattbl_number;
    patchg_tbl = work->pattbl;
    patchg = patchg_tbl[pattbl_number];
    work->pctime = work->pctime_b = patchg->timer;
}

void patset(sprite_status_sp *work) {
    sprite_patterns_sp **patchg_tbl;
    sprite_patterns_sp *patchg;
    sprite_pattern *sprpat_adr;

    if (!work->actno)
        return;
    patchg_tbl = work->pattbl;
    patchg = patchg_tbl[work->pattblno];
    if (work->patno >= patchg->cnt)
        work->patno = 0;
    if ((work->actflg & 2) == 0) {
        if (--work->pctime < 0) {
            work->pctime = patchg->timer;
            if (++work->patno >= patchg->cnt)
                work->patno = 0;
        }
    }
    if (work->actflg & 4)
        return;
    sprpat_adr = patchg->spr[work->patno];
    if (sprpat_adr->cnt == 0)
        return;

    n_patset(work, sprpat_adr);
}

void n_patset(sprite_status_sp *work, sprite_pattern *sprpat_adr) {
    Sint16 x_posi, y_posi, i, sx, sy;
    Uint16 rev;
    sprite_data sprdat;

    if (linkdata >= 80)
        return;

    for (i = 0; i < sprpat_adr->cnt; ++i) {
        sprdat = sprpat_adr->spra[i];

        sx = work->sx_posi.w.h * 5 / 4 - 32;
        sy = work->sy_posi.w.h;

        switch (sprdat.etc & 24) {
        case 0:
            y_posi = sy + (Sint16)sprdat.yoff;
            x_posi = sx + (Sint16)sprdat.xoff;
            rev = 0;
            break;
        case 8:
            y_posi = sy + (Sint16)sprdat.yoff;
            x_posi = sx - (Sint16)sprdat.xoff - (Sint16)SprBmp[sprdat.index].xs;
            rev = 1;
            break;
        case 16:
            y_posi = sy - (Sint16)sprdat.yoff - (Sint16)SprBmp[sprdat.index].ys;
            x_posi = sx + (Sint16)sprdat.xoff;
            rev = 2;
            break;
        case 24:
            y_posi = sy - (Sint16)sprdat.yoff - (Sint16)SprBmp[sprdat.index].ys;
            x_posi = sx - (Sint16)sprdat.xoff - (Sint16)SprBmp[sprdat.index].xs;
            rev = 3;
            break;
        default:
            x_posi = y_posi = 0;
            rev = 0;
            break;
        }

        if (y_posi + (Sint16)SprBmp[sprdat.index].ys > 128 && y_posi < 384 &&
            x_posi + (Sint16)SprBmp[sprdat.index].xs > 128 && x_posi < 448) {
            EAsprset(x_posi, y_posi, sprdat.index, linkdata++, rev);
        }
    }
}

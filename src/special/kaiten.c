#include "../types.h"
#include "common.h"
#include "sps_equ.h"
#include "kaiten.h"

void scalinit(void) {
    int_union lD0;

    lD0.l = zoomwk.h * zoomwk.cz;
    lD0.l >>= 8;
    zoomwk.hcz.w.h = lD0.w.l;
    lD0.l = zoomwk.h * zoomwk.sz;
    lD0.l >>= 8;
    zoomwk.hsz.w.h = lD0.w.l;
}

void scal(sprite_status_sp *pActwk) {
    int_union lD0, lD1, lD2, lD3, lD4;
    lD0.l = 0;
    lD1.l = 0;
    lD2.l = 0;

    lD0.w.l = actwk[0].x_posi.w.h - pActwk->x_posi.w.h;
    lD1.w.l = actwk[0].y_posi.w.h - pActwk->y_posi.w.h;

    lD2.w.l = zoomwk.h + zoomwk.i;
    lD3.l = lD0.w.l * zoomwk.sz;
    lD3.l >>= 8;
    lD2.l -= lD3.l;
    lD3.l = lD1.w.l * zoomwk.cz;
    lD3.l >>= 8;
    lD2.l += lD3.l;
    if (lD2.l == 0) {
        lD2.l = 1;
    }
    lD3.l = zoomwk.hcz.w.h * lD0.w.l;
    lD4.l = zoomwk.hsz.w.h * lD1.w.l;
    lD3.l += lD4.l;
    lD3.w.l = lD3.l / lD2.l;
    lD3.w.l += 256;
    pActwk->sx_posi.w.h = lD3.w.l;
    lD3.l = zoomwk.h * pActwk->z_posi.w.h;
    lD3.l >>= 3;
    lD3.w.l = lD3.l / lD2.l;
    lD3.w.l += 256;
    pActwk->sy_posi.w.h = lD3.w.l;
}

void cal_kaiten(void) {
    Sint16 i;
    Sint16 iD6;
    Sint32 lsv1, lsv2, lsv3, lsv4;
    int_union lD0, lD1, lD2, lD3, lD4, lD5;
    Sint16 *ptv_adr;

    ptv_adr = (Sint16 *)&tv_adr[0];

    lD0.w.l = (Uint16)zoomwk.x << 3;
    lD1.w.l = (Uint16)zoomwk.y << 3;

    lD2.w.l = -3;
    iD6 = 8;

    lD3.l = zoomwk.cx * zoomwk.sz;
    lD3.l >>= 5;
    zoomwk.cxsz.w.h = lD3.w.l;
    lD3.l = zoomwk.cx * zoomwk.cz;
    lD3.l >>= 5;
    zoomwk.cxcz.w.h = lD3.w.l;

    lD3.l = zoomwk.h * zoomwk.sx;
    lD3.l = lD3.w.l * zoomwk.sz;
    lD3.l >>= 5;
    zoomwk.hsxsz = lD3.l;

    lD3.l = zoomwk.h * zoomwk.sx;
    lD3.l = lD3.w.l * zoomwk.cz;
    lD3.l >>= 5;
    zoomwk.hsxcz = lD3.l;
    lD3.l = zoomwk.h * zoomwk.cx;
    zoomwk.hcx = lD3.l;

    lD3.l = -zoomwk.cz << 7;
    lsv1 = lD3.l << 3;
    lD3.l = -zoomwk.sz << 7;
    lsv2 = lD3.l << 3;
    lD3.l = zoomwk.cz * 127;
    lsv3 = lD3.l << 3;
    lD3.l = zoomwk.sz * 127;
    lsv4 = lD3.l << 3;

    lD4.l = zoomwk.sx * zoomwk.sz;
    lD4.l >>= 5;
    lD3.l = lD4.l * (zoomwk.h + zoomwk.i);
    lD3.l >>= iD6;
    zoomwk.hisxsz.w.h = lD3.w.l;
    lD4.l = zoomwk.sx * zoomwk.cz;
    lD4.l >>= 5;
    lD3.l = lD4.l * (zoomwk.h + zoomwk.i);
    lD3.l >>= iD6;
    zoomwk.hisxcz.w.h = lD3.w.l;

    for (i = 0; i < 3; ++i) {

        *ptv_adr++ = 0;
        *ptv_adr++ = -8;
        *ptv_adr++ = 0;
        *ptv_adr++ = 0;
    }

    for (i = 0; i < 93; ++i, --lD2.w.l) {

        lD3.l = zoomwk.sx * lD2.w.l;
        lD3.l += zoomwk.hcx;
        lD3.l >>= 5;
        if (lD3.l == 0) {
            lD3.l = 1;
        }
        lD5.l = zoomwk.cxsz.w.h * lD2.w.l;
        lD4.l = zoomwk.hsxsz + (lsv1 - lD5.l);
        lD4.l >>= iD6;
        lD4.l *= zoomwk.z;
        lD4.w.l = lD4.l / lD3.l;
        lD4.w.l += lD0.w.l;
        lD4.w.l -= zoomwk.hisxsz.w.h;
        *ptv_adr++ = lD4.w.l;

        lD5.l = zoomwk.cxcz.w.h * lD2.w.l;
        lD4.l = lsv2 + lD5.l - zoomwk.hsxcz;
        lD4.l >>= iD6;
        lD4.l *= zoomwk.z;
        lD4.w.l = lD4.l / lD3.l;
        lD4.w.l += lD1.w.l;
        lD4.w.l += zoomwk.hisxcz.w.h;
        *ptv_adr++ = lD4.w.l;

        lD5.l = zoomwk.cxsz.w.h * lD2.w.l;
        lD4.l = zoomwk.hsxsz + (lsv3 - lD5.l);
        lD4.l >>= iD6;
        lD4.l *= zoomwk.z;
        lD4.w.l = lD4.l / lD3.l;
        lD4.w.l += lD0.w.l;
        lD4.w.l -= zoomwk.hisxsz.w.h;
        lD4.w.l -= *(ptv_adr - 2);
        *ptv_adr++ = lD4.w.l;

        lD5.l = zoomwk.cxcz.w.h * lD2.w.l;
        lD4.l = lsv4 + lD5.l - zoomwk.hsxcz;
        lD4.l >>= iD6;
        lD4.l *= zoomwk.z;
        lD4.w.l = lD4.l / lD3.l;
        lD4.w.l += lD1.w.l;
        lD4.w.l += zoomwk.hisxcz.w.h;
        lD4.w.l -= *(ptv_adr - 2);
        *ptv_adr++ = lD4.w.l;
    }
}

void kaiten_init(void) {
    zoomwk.h = 128;
    zoomwk.i = -64;
}

void init_3dpara(void) {
    zoomwk.sx = k_sin(zoomwk.ax);
    zoomwk.cx = k_cos(zoomwk.ax);
    zoomwk.sz = k_sin(zoomwk.az);
    zoomwk.cz = k_cos(zoomwk.az);
    zoomwk.msz = k_sin(zoomwk.az + 256);
    zoomwk.mcz = k_cos(zoomwk.az + 256);
}

void kaiten(void) { cal_kaiten(); }

Sint16 k_sin(Sint16 iD3) {
    Sint16 sin_tbl[128] = {
        0,   3,   6,   9,   12,  15,  18,  22,  25,  28,  31,  34,  37,
        40,  43,  47,  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,
        80,  83,  86,  89,  92,  95,  98,  101, 104, 106, 109, 112, 115,
        118, 121, 123, 126, 129, 132, 134, 137, 140, 142, 145, 147, 150,
        153, 155, 158, 160, 162, 165, 167, 170, 172, 174, 177, 179, 181,
        183, 185, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208,
        209, 211, 213, 215, 216, 218, 220, 221, 223, 224, 226, 227, 229,
        230, 231, 233, 234, 235, 236, 238, 239, 240, 241, 242, 243, 244,
        245, 246, 247, 247, 248, 249, 250, 250, 251, 251, 252, 252, 253,
        253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 256,
    };
    Sint16 iD4;

    iD3 &= 511;
    iD4 = iD3;
    if (iD3 & 128) {
        iD4 = ~iD4;
    }
    iD4 &= 127;
    iD4 = sin_tbl[iD4];
    if (iD3 & 256) {
        iD4 = -iD4;
    }
    return iD4;
}

Sint16 k_cos(Sint16 iD3) { return k_sin(iD3 + 128); }

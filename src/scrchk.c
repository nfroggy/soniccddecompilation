#include "equ.h"
#include "scrchk.h"

void scrchk(void) {
    Sint16 sD1;

    switch (stageno.b.h) {
    case 0:
        zone1chk();
        break;
    case 1:
        zone3chk();
        break;
    case 2:
        zone4chk();
        break;
    case 3:
        zone5chk();
        break;
    case 4:
        zone6chk();
        break;
    case 5:
        zone7chk();
        break;
    case 6:
        zone8chk();
        break;
    }

    if (actwk[0].mstno.b.h == 43) {
        scralim_down = scra_v_posit.w.h;
        scralim_n_down = scra_v_posit.w.h;
    }
    sD1 = 4;
    if (scralim_n_down == scralim_down)
        return;

    if ((Uint16)scralim_n_down < (Uint16)scralim_down) {

        sD1 *= -1;
        if ((Uint16)scralim_n_down < (Uint16)scra_v_posit.w.h) {
            scralim_down = scra_v_posit.w.h & 65534;
        }

        scralim_down += sD1;
        limmoveflag = 1;

        return;
    }

    if ((Uint16)(scra_v_posit.w.h + 8) >= (Uint16)scralim_down) {

        if (actwk[0].cddat & 2) {
            sD1 *= 4;
        }
    }

    scralim_down += sD1;
    limmoveflag = 1;
}

void zone1chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone11chk();
        break;
    case 1:
        zone12chk();
        break;
    case 2:
        zone13chk();
        break;
    }
}

void zone11chk(void) {    /* Line 112, Address: 0x1017cf0*/
    if (time_flag != 1) { /* Line 113, Address: 0x1017cf8*/
        zone12chk();      /* Line 114, Address: 0x1017d10*/
    } /* Line 115, Address: 0x1017d18*/
    else {
        if ((Uint16)actwk[0].xposi.w.h >=
            7190) { /* Line 117, Address: 0x1017d20*/
            if ((Uint16)actwk[0].xposi.w.h <
                8646) {           /* Line 118, Address: 0x1017d3c*/
                scra_vline = 136; /* Line 119, Address: 0x1017d58*/
            }
        }

        scralim_n_down = 1808; /* Line 123, Address: 0x1017d64*/
        if ((Uint16)scra_h_posit.w.h >=
            2112) { /* Line 124, Address: 0x1017d70*/

            if (pltime_f != 0 &&
                (Uint16)scralim_left < 2080) { /* Line 128, Address: 0x1017d8c*/

                scralim_left = scralim_n_left =
                    2080; /* Line 130, Address: 0x1017dbc*/
            }
            if ((Uint16)scra_h_posit.w.h >=
                3584) {               /* Line 133, Address: 0x1017dd8*/
                scralim_n_down = 784; /* Line 134, Address: 0x1017df4*/
            } /* Line 135, Address: 0x1017e00*/
            else {
                scralim_n_down = 1040; /* Line 137, Address: 0x1017e08*/
            }
        }
    }
} /* Line 141, Address: 0x1017e14*/

void zone12chk(void) { scralim_n_down = 784; }

void zone13chk(void) { /* Line 150, Address: 0x1017e50*/
    if (bossflag)
        return;             /* Line 151, Address: 0x1017e58*/
    scralim_n_down = 784;   /* Line 152, Address: 0x1017e68*/
    bossclr_scr(3440, 784); /* Line 153, Address: 0x1017e74*/
} /* Line 154, Address: 0x1017e84*/

void zone3chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone31chk();
        break;
    case 1:
        zone31chk();
        break;
    case 2:
        zone33chk();
        break;
    }
}

void zone31chk(void) {
    scralim_n_down = 1296; /* Line 172, Address: 0x1017f20*/

} /* Line 174, Address: 0x1017f2c*/

void zone33chk(void) {
    if (bossflag) {
        bossclr_scrset(96);
    } else {
        scralim_n_down = 1296;
    }
}

void zone6chk(void) {
    Uint16 wD0, wD1;

    if (!(bossflag & 16)) {
        scralim_n_down = 1808;
        return;
    }

    if (bossclr_scr(2976, 464) == 0) {

        if (actwk[0].yposi.w.h <= 664) {

            wD0 = 464;
        } else if (actwk[0].yposi.w.h <= 1176) {

            wD0 = 976;
        } else {

            wD0 = 1488;
        }

        wD1 = wD0;
        scralim_n_down = wD0;
        wD0 -= scralim_down;
        if ((Sint16)wD0 < 0) {
            wD0 = -(Sint16)wD0;
        }
        if (wD0 < 3) {
            scralim_down = wD1;
        }
    }
}

void zone5chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone51chk();
        break;
    case 1:
        zone51chk();
        break;
    case 2:
        zone53chk();
        break;
    }
}

void zone51chk(void) { scralim_n_down = 784; }

void zone53chk(void) {
    if (bossclr_scr(3600, 504) == 0) {

        if (bossflag) {

            scralim_down = 504;
            scralim_n_down = 504;

        } else {
            scralim_n_down = 800;
        }
    }
}

void zone8chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone81chk();
        break;
    case 1:
        zone81chk();
        break;
    case 2:
        zone83chk();
        break;
    }
}

void zone81chk(void) { scralim_n_down = 1808; }

void zone83chk(void) {
    if (bossflag) {

        scralim_up = scralim_n_up = scralim_down = scralim_n_down = 268;

    } else {
        scralim_n_down = 784;
    }
}

void zone4chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone41chk();
        break;
    case 1:
        zone42chk();
        break;
    case 2:
        zone43chk();
        break;
    }
}

void zone41chk(void) { scralim_n_down = 1296; }

void zone42chk(void) {
    if (actwk[0].mstno.b.h == 43 || actwk[0].r_no0 >= 6) {

        scralim_down = 1808;
        scralim_n_down = 1808;
    } else {

        scralim_down = 2048;
        scralim_n_down = 2048;
        if (scra_h_posit.w.h >= 512) {
            scralim_down = 1808;
            scralim_n_down = 1808;
        }
    }
}

void zone43chk(void) {
    if (bossclr_scr(2808, 1216) == 0) {

        if (bossflag) {

            scralim_down = 1264;
            scralim_n_down = 1264;
        }
    }
}

void zone7chk(void) {
    switch (stageno.b.l) {
    case 0:
        zone71chk();
        break;
    case 1:
        zone72chk();
        break;
    case 2:
        zone73chk();
        break;
    }
}

void zone71chk(void) { scralim_n_down = 1296; }

void zone72chk(void) { scralim_n_down = 1808; }

void zone73chk(void) {
    Uint16 wD1;

    if (actwk[0].xposi.w.h < 2352) {
        scralim_n_down = 528;
        return;
    }

    if (actwk[0].xposi.w.h >= 3520) {

        scralim_n_down = 528;
    } else {

        scralim_n_down = 288;
        wD1 = 288 - (Uint16)scralim_down;
        if ((Sint16)wD1 < 0) {
            wD1 = -(Sint16)wD1;
        }
        if (wD1 < 4) {
            scralim_down = 288;
        }
    }
}

Sint32 bossclr_scr(Uint16 wD0, Uint16 wD1) {
    if (actwk[0].xposi.w.h >= (Sint16)wD0) {
        return bossclr_scrset(wD1);
    }
    return 0;
}

Sint32 bossclr_scrset(Uint16 wD1) {
    Uint16 wD0;

    scralim_n_down = wD1;
    wD1 -= scralim_down;
    if ((Sint16)wD1 < 0) {
        wD1 = -(Sint16)wD1;
    }
    if (wD1 < 4) {
        scralim_down = scralim_n_down;
    }
    wD0 = (Uint16)actwk[0].xposi.w.h - 160;
    if (scralim_left > (Sint16)wD0) {
        return 1;
    }
    if (scralim_right < (Sint16)wD0) {
        wD0 = scralim_right;
    }
    scralim_left = wD0;
    scralim_n_left = wD0;
    return 1;
}

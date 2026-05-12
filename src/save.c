#include "equ.h"
#include "save.h"

void playloadb(void) {
    sprite_status *pActwk;

    pActwk = &actwk[0];
    pActwk->xposi.w.h = lpKeepWork->plxposi_sb;
    pActwk->yposi.w.h = lpKeepWork->plyposi_sb;
    pActwk->cddat = lpKeepWork->cddat_sb;
    pActwk->mspeed.w = lpKeepWork->plmspd_sb;
    pActwk->xspeed.w = lpKeepWork->plxspd_sb;
    pActwk->yspeed.w = lpKeepWork->plyspd_sb;

    plring = lpKeepWork->plring_s;
    plring_s = lpKeepWork->plring_s;
    plring_f2 = lpKeepWork->plring_f2_s;
    plring_f2_s = lpKeepWork->plring_f2_s;

    pltime.l = lpKeepWork->pltime_sb;
    water_flag = lpKeepWork->water_flag_sb;
    scralim_down = lpKeepWork->scralim_down_sb;
    scralim_n_down = lpKeepWork->scralim_down_sb;
    scra_h_posit.w.h = lpKeepWork->scra_h_posit_sb;
    scra_v_posit.w.h = lpKeepWork->scra_v_posit_sb;
    scrb_h_posit.w.h = lpKeepWork->scrb_h_posit_sb;
    scrb_v_posit.w.h = lpKeepWork->scrb_v_posit_sb;
    scrc_h_posit.w.h = lpKeepWork->scrc_h_posit_sb;
    scrc_v_posit.w.h = lpKeepWork->scrc_v_posit_sb;
    scrz_h_posit.w.h = lpKeepWork->scrz_h_posit_sb;
    scrz_v_posit.w.h = lpKeepWork->scrz_v_posit_sb;

    if (stageno.b.h == 6) {
        chibi_flag = lpKeepWork->chibi_sb;
    }
    if ((char)plflag < 0) {
        scralim_left = lpKeepWork->plxposi_sb - 160;
    }
}

void playload(void) {
    sprite_status *pActwk;
    pActwk = &actwk[0];

    if (plflag == 2) {

        playloadb();
    } else {

        plflag = plflag_s;
        pActwk->xposi.w.h = plxposi_s;
        pActwk->yposi.w.h = plyposi_s;
        plring = 0;
        plring_f2 = 0;
        pltime.l = pltime_s;
        pltime.b.b4 = 59;
        --pltime.b.b3;
        water_flag = water_flag_s;
        scralim_down = scralim_down_s;
        scralim_n_down = scralim_down_s;
        scra_h_posit.w.h = scra_h_posit_s;
        scra_v_posit.w.h = scra_v_posit_s;
        scrb_h_posit.w.h = scrb_h_posit_s;
        scrb_v_posit.w.h = scrb_v_posit_s;
        scrc_h_posit.w.h = scrc_h_posit_s;
        scrc_v_posit.w.h = scrc_v_posit_s;
        scrz_h_posit.w.h = scrz_h_posit_s;
        scrz_v_posit.w.h = scrz_v_posit_s;

        if (stageno.b.h == 6) {
            chibi_flag = chibi_s;
        }

        if (stageno.b.h == 2) {
            waterposi_m = waterposi_m_s;
            water_flag = water_flag_s;
            waterflag = waterflag_s;
        }

        if ((char)plflag < 0) {
            scralim_left = plxposi_s - 160;
        }
    }
}

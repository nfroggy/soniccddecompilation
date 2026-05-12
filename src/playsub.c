#include "equ.h"
#include "playsub.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "dummy.h"
#include "etc.h"
#include "loader2.h"
#include "ridechk.h"

extern sprite_pattern *markerpat[];
extern Uint8 *markerchg[];
extern sprite_pattern *bariapat[];
extern Uint8 *bariachg[];
extern sprite_pattern *bakupat[];
extern Uint8 *bakuchg[];
extern sprite_pattern *mizukipat[];
extern Uint8 *mizukichg[];
extern sprite_pattern *futapat[];
extern Uint8 *futachg[];
extern sprite_pattern *exit2pat[];
extern Uint8 *exit2chg[];
static Uint8 flowchg0[4] = {3, 0, 1, 255}, flowchg1[6] = {3, 2, 3, 2, 3, 252},
             flowchg2[6] = {1, 5, 5, 4, 6, 252}, flowchg3[4] = {19, 6, 7, 255};
Uint8 *flowchg[4] = {flowchg0, flowchg1, flowchg2, flowchg3};
static sprite_pattern flo00 = {1, {{-4, -16, 0, 264}}};
static sprite_pattern flo01 = {1, {{-4, -16, 0, 265}}};
static sprite_pattern flo02 = {1, {{-8, -16, 0, 266}}};
static sprite_pattern flo03 = {1, {{-8, -16, 0, 267}}};
static sprite_pattern flo04 = {1, {{-12, -24, 0, 268}}};
static sprite_pattern flo05 = {1, {{-8, -16, 0, 269}}};
static sprite_pattern flo06 = {1, {{-12, -48, 0, 270}}};
static sprite_pattern flo07 = {1, {{-12, -48, 0, 271}}};
sprite_pattern *flow_pat[8] = {&flo00, &flo01, &flo02, &flo03,
                               &flo04, &flo05, &flo06, &flo07};

void patchg(sprite_status *patchgwk, Uint8 **pat_dat) {
    Uint8 pat_no;
    Uint8 *sprpat_adr;
    Sint16 search_end;

    if (patchgwk->mstno.b.h != patchgwk->mstno.b.l) {
        patchgwk->mstno.b.l = patchgwk->mstno.b.h;
        patchgwk->patcnt = patchgwk->pattim = 0;
    }

    if ((char)--patchgwk->pattim >= 0)
        return;
    sprpat_adr = pat_dat[patchgwk->mstno.b.h];

    search_end = 0;
    while (sprpat_adr[search_end++] < 250)
        ;
    patchgwk->pattim =
        sprpat_adr[search_end - 1] != 252 && *sprpat_adr <= 0 ? 1 : *sprpat_adr;

    if ((pat_no = sprpat_adr[patchgwk->patcnt + 1]) < 128) {
    label1:
        patchgwk->patno = pat_no & 31;
        pat_no >>= 5;

        pat_no ^= patchgwk->cddat;
        pat_no &= 3;
        patchgwk->actflg &= -4;
        patchgwk->actflg |= pat_no;
        ++patchgwk->patcnt;
    } else {
        switch (pat_no) {
        case 255:
            patchgwk->patcnt = 0;
            pat_no = sprpat_adr[1];
            goto label1;

        case 254:
            pat_no = sprpat_adr[patchgwk->patcnt + 2];
            patchgwk->patcnt = patchgwk->patcnt - pat_no;
            pat_no = sprpat_adr[patchgwk->patcnt + 1];
            goto label1;

        case 253:
            patchgwk->mstno.b.h = sprpat_adr[patchgwk->patcnt + 2];
            break;
        case 252:
            patchgwk->r_no0 += 2;
            break;
        case 251:
            patchgwk->patcnt = patchgwk->r_no1 = 0;
            break;
        case 250:
            patchgwk->r_no1 += 2;
            break;
        }
    }
}

void playsave0(sprite_status *savewk) {
    plflag_s = plflag;
    plxposi_s = savewk->xposi.w.h;
    plyposi_s = savewk->yposi.w.h;
    water_flag_s = water_flag;
    scralim_down_s = scralim_down;
    scra_h_posit_s = scra_h_posit.w.h;
    scra_v_posit_s = scra_v_posit.w.h;
    scrb_h_posit_s = scrb_h_posit.w.h;
    scrb_v_posit_s = scrb_v_posit.w.h;
    scrc_h_posit_s = scrc_h_posit.w.h;
    scrc_v_posit_s = scrc_v_posit.w.h;
    scrz_h_posit_s = scrz_h_posit.w.h;
    scrz_v_posit_s = scrz_v_posit.w.h;
    waterposi_m_s = waterposi_m;
    waterflag_s = waterflag;
    if (pltime.l >= 327680)
        pltime_s = 327680;
    else
        pltime_s = pltime.l;
    chibi_s = chibi_flag;
}

void marker(sprite_status *markerwk) {
    switch (markerwk->r_no0) {
    case 0:
        marker_init(markerwk);
        break;
    case 2:
        marker_move0(markerwk);
        break;
    case 4:
        marker_move1(markerwk);
        break;
    case 6:
        marker_move2(markerwk);
        break;
    }

    actionsub(markerwk);
    frameout_s(markerwk);
}

void marker_init(sprite_status *markerwk) {
    sprite_status *new_actwk, **parent;
    Uint16 marker_yposi_m_buf;

    markerwk->r_no0 += 2;
    markerwk->patbase = markerpat;
    markerwk->sproffset = 1739;
    markerwk->actflg = 4;
    markerwk->sprhsize = 8;
    markerwk->sprvsize = 24;
    markerwk->sprpri = 4;

    if (markerno >= (Uint8)markerwk->userflag.b.h)
        markerwk->actfree[4] = 1;
    else
        markerwk->colino = 227;
    if (actwkchk(&new_actwk) != 0) {
        frameout(markerwk);
        return;
    }
    new_actwk->actno = 19;
    new_actwk->r_no0 += 4;
    if (markerwk->actfree[4] != 0)
        new_actwk->r_no0 += 2;
    new_actwk->patbase = markerpat;
    new_actwk->sproffset = 1739;
    new_actwk->actflg = 4;
    new_actwk->sprhsize = 8;
    new_actwk->sprvsize = 8;
    new_actwk->sprpri = 3;
    new_actwk->patno = 1;
    parent = (sprite_status **)&new_actwk->actfree[6];
    *parent = markerwk;
    new_actwk->xposi.w.h = markerwk->xposi.w.h;
    new_actwk->yposi.w.h = markerwk->yposi.w.h - 32;
    ((Sint16 *)new_actwk)[23] = markerwk->xposi.w.h;
    marker_yposi_m_buf = markerwk->yposi.w.h - 24;
    ((Sint16 *)new_actwk)[24] = marker_yposi_m_buf;
}

void marker_move0(sprite_status *markerwk) {

    if (markerwk->actfree[4] != 0)
        return;

    if (markerwk->colicnt == 0)
        return;
    markerwk->colino = 0;
    markerwk->actfree[4] = 1;
    markerno = markerwk->userflag.b.h;
    plflag = 1;
    playsave0(&actwk[0]);
    soundset(174);
}

void marker_move1(sprite_status *markerwk) {
    Sint32 sin_data, cos_data;
    Sint16 sin_tmp, cos_tmp;
    sprite_status **parent, *new_actwk;

    if (markerwk->actfree[4] == 0) {
        parent = (sprite_status **)&markerwk->actfree[6];
        new_actwk = *parent;
        if (new_actwk->actfree[4] == 0)
            return;
        markerwk->actfree[4] = 1;
    }

    markerwk->actfree[10] += 8;
    sinset(markerwk->actfree[10], &sin_tmp, &cos_tmp);
    sin_data = sin_tmp;
    cos_data = cos_tmp;
    sin_data *= 8;
    sin_data /= 256;
    markerwk->xposi.w.h = ((Sint16 *)markerwk)[23];
    markerwk->xposi.w.h += sin_data;
    cos_data = -cos_data * 8;
    cos_data /= 256;
    markerwk->yposi.w.h = ((Sint16 *)markerwk)[24];
    markerwk->yposi.w.h += cos_data;
    if (markerwk->actfree[10] == 0)
        markerwk->r_no0 += 2;
}

void marker_move2(sprite_status *markerwk) { patchg(markerwk, markerchg); }

void plairset(void) {}

void test_act(sprite_status *testwk) {
    switch (testwk->r_no0) {
    case 0:
        test_init(testwk);
        break;
    case 2:
        test_move(testwk);
        break;
    }
}

void test_init(sprite_status *testwk) {
    if (testwk->cddat & 128) {
        frameout(testwk);
        return;
    }
    testwk->r_no0 += 2;
    testwk->actflg = 4;
    testwk->sprpri = 1;
    testwk->patbase = bariapat;
    testwk->sproffset = 1345;
    testwk->actfree[6] = testwk->xposi.b.b1;
    testwk->actfree[7] = testwk->xposi.b.b2;
    testwk->colino = 6;
    test_move(testwk);
}

void test_move(sprite_status *testwk) {
    Uint16 cal0, cal1;

    cal0 = ((Uint16 *)testwk)[26] & 65408;
    cal1 = (Uint16)(scra_h_posit.w.h - 128) & 65408;
    if ((cal0 - cal1) > 640) {
        frameout(testwk);
        return;
    }

    patchg(testwk, bariachg);
    actionsub(testwk);
}

void tensuu_set(sprite_status *tensuuwk) {
    Uint8 score_tmp;

    if (tensuuwk->r_no1 != 0)
        return;
    score_tmp = ((Uint16 *)tensuuwk)[33];
    tensuu0(tensuuwk, score_tmp / 2);
}

void tensuu0(sprite_status *tensuuwk, Uint8 uf_data) {
    sprite_status *new_actwk;

    uf_data |= 128;
    if (actwkchk(&new_actwk) != 0)
        return;
    new_actwk->actno = 28;
    new_actwk->xposi.w.h = tensuuwk->xposi.w.h;
    new_actwk->yposi.w.h = tensuuwk->yposi.w.h;
    new_actwk->userflag.b.h = uf_data;
}

void bakuha(sprite_status *bakuhawk) {
    switch (bakuhawk->r_no0) {
    case 0:
        baku_init(bakuhawk);
        break;
    case 2:
        baku_move(bakuhawk);
        break;
    case 4:
        baku_die(bakuhawk);
        break;
    }
}

void baku_init(sprite_status *bakuhawk) {
    bakuhawk->r_no0 += 2;
    bakuhawk->actflg |= 4;
    bakuhawk->sprpri = 1;
    bakuhawk->sproffset = 34432;
    if (bakuhawk->userflag.b.l != 0)
        bakuhawk->sproffset &= 32767;

    bakuhawk->patbase = bakupat;
    tensuu_set(bakuhawk);
    bakuhawk->colino = 0;
    bakuhawk->patcnt = 0;
    bakuhawk->pattim = 0;
    bakuhawk->mstno.w = 0;
    if (bakuhawk->userflag.b.h != 0)
        bakuhawk->mstno.w = 256;
    baku_move(bakuhawk);
}

void baku_move(sprite_status *bakuhawk) {
    patchg(bakuhawk, bakuchg);
    actionsub(bakuhawk);
}

void baku_die(sprite_status *bakuhawk) {
    if (bakuhawk->r_no1 != 0) {
        frameout(bakuhawk);
        return;
    }
    bakuhawk->actno = 31;
    bakuhawk->r_no0 = 0;
}

void flower(sprite_status *flowerwk) {
    switch (flowerwk->r_no0) {
    case 0:
        flow_init(flowerwk);
        break;
    case 2:
        flow_move0(flowerwk);
        break;
    case 4:
        flow_move1(flowerwk);
        break;
    case 6:
        flow_move2(flowerwk);
        break;
    case 8:
        flow_move3(flowerwk);
        break;
    }

    actionsub(flowerwk);
}

void flow_init(sprite_status *flowerwk) {
    Uint8 *fwcnt_adr;
    fwcnt_adr = &flagworkcnt;

    flowerwk->actflg |= 4;
    flowerwk->sprpri = 1;
    flowerwk->sprvsize = 0;
    flowerwk->sproffset = 42711;

    if (flowerwk->userflag.b.l != 0)
        flowerwk->sproffset &= 32767;

    flowerwk->patbase = flow_pat;
    if (flowerwk->userflag.b.h == 0) {
        fwcnt_adr = flow_sub(flowerwk);
        flowerwk->r_no0 = 4;
        flowerwk->mstno.b.h = 3;
        if (*fwcnt_adr & 64) {
            flow_move1(flowerwk);
            return;
        }
    }
    flowerwk->mstno.w = 2;
    flowerwk->r_no0 = 2;
    flowerwk->sproffset = 1751;
    flow_move0(flowerwk);
}

void flow_move0(sprite_status *flowerwk) {
    Sint16 y_move = 0, flower_index = 0;
    Uint8 *fwcnt_adr;

    y_move = emycol_d(flowerwk);
    if (y_move < 0) {
        flowerwk->yposi.w.h += y_move;
        if (flowerwk->userflag.b.h == 0) {
            fwcnt_adr = flow_sub(flowerwk);
            flower_index = flow_sub1(flowercnt[time_flag & 127]++);
            flowerposi[flower_index].w.h = flowerwk->xposi.w.h;
            flowerposi[flower_index].w.l = flowerwk->yposi.w.h;
        }
        flowerwk->r_no0 = 4;
        flowerwk->mstno.b.h = 1;
        flowerwk->sprvsize = 48;
        flow_move1(flowerwk);
    } else {
        flowerwk->yposi.w.h += 2;
        flow_move1(flowerwk);
    }
}

void flow_move1(sprite_status *flowerwk) { patchg(flowerwk, flowchg); }

Uint8 *flow_sub(sprite_status *flowerwk) {
    Sint16 index;

    index = (Sint16)(time_flag & 127) + flowerwk->cdsts * 3;
    return &flagwork[index];
}

Sint16 flow_sub1(Uint8 fl_counter) {
    Sint16 flower_index;

    flower_index = (Uint16)(time_flag & 127) * 64 + ((Uint16)fl_counter & 63);

    return flower_index;
}

void flow_move2(sprite_status *flowerwk) {
    flowerwk->sproffset = 9943;

    flowerwk->mstno.b.h = 2;
    flow_move1(flowerwk);
}

void flow_move3(sprite_status *flowerwk) {
    flowerwk->mstno.b.h = 3;
    flowerwk->r_no0 = 4;
    flow_move1(flowerwk);
}

void mizukiri(sprite_status *mizukiwk) {
    switch (mizukiwk->r_no0) {
    case 0:
        mizuki_init(mizukiwk);
        break;
    case 2:
        mizuki_move(mizukiwk);
        break;
    case 4:
        mizuki_move2(mizukiwk);
        break;
    }
}

void mizuki_init(sprite_status *mizukiwk) {
    mizukiwk->r_no0 += 2;
    mizukiwk->actflg |= 4;
    mizukiwk->patbase = mizukipat;
    mizukiwk->sproffset = 996;
    if (time_flag == 0)
        mizukiwk->sproffset = 926;
    mizukiwk->sprpri = 1;
    mizuki_move(mizukiwk);
}

void mizuki_move(sprite_status *mizukiwk) {
    patchg(mizukiwk, mizukichg);
    actionsub(mizukiwk);
}

void mizuki_move2(sprite_status *mizukiwk) { frameout(mizukiwk); }

void futa(sprite_status *futawk) {
    switch (futawk->r_no0) {
    case 0:
        futa_init(futawk);
        break;
    case 2:
        futa_move(futawk);
        break;
    case 4:
        futa_move2(futawk);
        break;
    case 6:
        futa_move3(futawk);
        break;
    }

    actionsub(futawk);
    frameout_s(futawk);
}

void ride_on_chk_f(sprite_status *futawk, sprite_status *sonicwk) {
    sprite_status *new_actwk;

    if (sonicwk->yspeed.w >= 0) {
        ridechk(futawk, &actwk[0]);
        return;
    }
    if (col_chk(futawk, sonicwk) == 0) {
        ridechk(futawk, &actwk[0]);
        return;
    }
    futawk->r_no0 = 4;
    if (futawk->userflag.b.h != 0)
        return;
    if (actwkchk(&new_actwk) != 0)
        return;
    new_actwk->actno = 11;
    new_actwk->xposi.w.h = futawk->xposi.w.h;
    new_actwk->yposi.w.h = futawk->yposi.w.h - 4;
    soundset(164);
}

void futa_init(sprite_status *futawk) {
    futawk->r_no0 += 2;
    futawk->patbase = futapat;
    futawk->sproffset = 32768;
    futawk->sprpri = 1;
    futawk->actflg |= 4;
    futawk->sprhsize = 44;
    if (futawk->userflag.b.h == 2)
        futawk->sprhsize = 24;
    futawk->sprvsize = 8;
    futa_move(futawk);
}

void futa_move(sprite_status *futawk) {
    ride_on_chk_f(futawk, &actwk[0]);
    ride_on_chk_f(futawk, &actwk[1]);
}

void futa_move2(sprite_status *futawk) { patchg(futawk, futachg); }

void futa_move3(sprite_status *futawk) {
    futawk->mstno.b.l = 1;
    futawk->patno = 0;
    futawk->r_no0 -= 4;
}

void exit2_set(sprite_status *testwk) {
    char a;

    a = col_chk(testwk, &actwk[0]);
}

char col_chk(sprite_status *thingwk, sprite_status *sonicwk) {
    Sint16 cal_posi;

    cal_posi =
        (Sint16)thingwk->sprhsize + (sonicwk->xposi.w.h - thingwk->xposi.w.h);
    if (cal_posi < 0)
        return 0;
    if (cal_posi >= (thingwk->sprhsize * 2))
        return 0;
    cal_posi =
        (Sint16)thingwk->sprvsize + (sonicwk->yposi.w.h - thingwk->yposi.w.h);
    if (cal_posi < 0)
        return 0;
    if (cal_posi >= (thingwk->sprvsize * 2))
        return 0;
    return 1;
}

void exit2(sprite_status *exitwk) {
    switch (exitwk->r_no0) {
    case 0:
        exit2_init(exitwk);
        break;
    case 2:
        exit2_move(exitwk);
        break;
    case 4:
        exit2_erase(exitwk);
        break;
    }
}

void exit2_init(sprite_status *exitwk) {
    exitwk->r_no0 += 2;
    exitwk->actflg = 4;
    exitwk->sprpri = 1;
    exitwk->patbase = exit2pat;
    exitwk->mstno.b.h = exitwk->userflag.b.h;
    if (exitwk->userflag.b.h < 2)
        soundset(162);
    else
        soundset(161);
    exit2_move(exitwk);
}

void exit2_move(sprite_status *exitwk) {
    patchg(exitwk, exit2chg);
    actionsub(exitwk);
}

void exit2_erase(sprite_status *exitwk) { frameout(exitwk); }

void baria(sprite_status *bariawk) {
    switch (bariawk->r_no0) {
    case 0:
        bariainit(bariawk);
        break;
    case 2:
        bariamove(bariawk);
        break;
    case 4:
        mutekimove(bariawk);
        break;
    case 6:
        backtomove(bariawk);
        break;
    }

    if (actwk[0].sproffset & 32768)
        bariawk->sproffset |= 32768;
    else
        bariawk->sproffset &= 32767;
    if (stageno.b.h == 6)
        bariawk->sproffset |= 32768;
}

void bariainit(sprite_status *bariawk) {
    bariawk->r_no0 += 2;
    bariawk->patbase = bariapat;
    bariawk->actflg = 4;
    bariawk->sprpri = 1;
    bariawk->sprhsize = 16;
    bariawk->sproffset = 1348;
    if (bariawk->mstno.b.h == 0)
        return;
    bariawk->r_no0 += 2;
    if (bariawk->mstno.b.h < 5)
        return;
    bariawk->r_no0 += 2;
}

void bariamove(sprite_status *bariawk) {
    if (plpower_b == 0) {
        frameout(bariawk);
        return;
    }
    if (plpower_a != 0 || plpower_m != 0)
        return;
    bariawk->xposi.w.h = actwk[0].xposi.w.h;
    bariawk->yposi.w.h = actwk[0].yposi.w.h;
    bariawk->cddat = actwk[0].cddat;
    if (stageno.b.h == 6) {
        bariawk->sproffset |= 128;
        if (prio_flag != 0)
            bariawk->sproffset &= 127;
    }
    patchg(bariawk, bariachg);
    actionsub0(bariawk);
}

void mutekimove(sprite_status *bariawk) {
    if (plpower_a != 0)
        return;
    if (plpower_m != 0)
        muteki_sub(bariawk);
    else
        frameout(bariawk);
}

void backtomove(sprite_status *bariawk) {
    if (plpower_a != 0)
        muteki_sub(bariawk);
    else
        frameout(bariawk);
}

void muteki_sub(sprite_status *bariawk) {
    short_union ppw_offset;
    Uint8 cal_no;

    if (stageno.b.h == 6) {
        bariawk->sproffset |= 128;
        if (prio_flag != 0)
            bariawk->sproffset &= 127;
    }

    ppw_offset.w = plposiwkadr.w;
    cal_no = bariawk->mstno.b.h - 1;
    if (cal_no >= 4)
        cal_no -= 4;
    cal_no = cal_no * 24 + 4;
    ((ushort_union *)&ppw_offset)->b.l =
        ((ushort_union *)&ppw_offset)->b.l - cal_no;
    cal_no = bariawk->actfree[6];
    ((ushort_union *)&ppw_offset)->b.l =
        ((ushort_union *)&ppw_offset)->b.l - cal_no;
    if ((cal_no += 4) >= 24)
        cal_no = 0;
    bariawk->actfree[6] = cal_no;

    bariawk->xposi.w.h = playposiwk[((ushort_union *)&ppw_offset)->w / 2];
    bariawk->yposi.w.h = playposiwk[((ushort_union *)&ppw_offset)->w / 2 + 1];

    bariawk->cddat = actwk[0].cddat;
    patchg(bariawk, bariachg);
    actionsub0(bariawk);
}

void actionsub0(sprite_status *bariawk) {
    if ((plsubchg_flag & 15) >= 8)
        return;

    if (bariawk->r_no0 != 0)
        plsubchg_flag = bariawk->r_no0 | 128;
    actionsub(bariawk);
}

void ball(sprite_status *ballwk) {
    switch (ballwk->r_no0) {
    case 0:
        ball_init(ballwk);
        break;
    case 2:
        ball_move(ballwk);
        break;
    }

    frameout_s(ballwk);
}

void ball_init(sprite_status *ballwk) {
    ballwk->r_no0 += 2;
    ballwk->actflg |= 4;
    ballwk->sproffset = 1348;
    ballwk->patbase = bariapat;
    ballwk->patno = ballwk->userflag.b.h + 1;
    ball_move(ballwk);
}

void ball_move(sprite_status *ballwk) {
    Sint16 cal_speed0, cal_speed1;

    if (actwk[0].mstno.b.h == 43)
        return;
    if (actwk[0].r_no0 >= 6)
        return;
    if (ball_coli(ballwk) == 0)
        return;

    if (ballwk->userflag.b.h == 0) {
        if ((cal_speed0 = actwk[0].xspeed.w) < 0)
            cal_speed0 = -cal_speed0;
        cal_speed1 = 2560;
        if (stageno.b.h == 5)
            cal_speed1 = 3328;
        if (cal_speed0 < cal_speed1)
            cal_speed0 = cal_speed1;
        if (actwk[0].xspeed.w < 0)
            cal_speed0 = -cal_speed0;
        actwk[0].xspeed.w = actwk[0].mspeed.w = cal_speed0;
        if (((Uint8)actwk[0].direc.b.h + 32 & 192) == 128)
            actwk[0].mspeed.w = -actwk[0].mspeed.w;
        goto label1;
    } else if ((Uint8)ballwk->userflag.b.h >= 2) {
        ball_rd7(ballwk);
        return;
    }
    if ((cal_speed0 = actwk[0].yspeed.w) < 0)
        cal_speed0 = -cal_speed0;
    if (cal_speed0 < 3328)
        cal_speed0 = 3328;
    if (actwk[0].yspeed.w < 0)
        cal_speed0 = -cal_speed0;
    actwk[0].yspeed.w = actwk[0].mspeed.w = cal_speed0;
    actwk[0].cddat |= 2;
label1:
    ball_ball();
}

void ball_ball(void) {
    if (actwk[0].cddat & 4)
        return;
    actwk[0].cddat |= 4;
    actwk[0].sprvsize = 14;
    actwk[0].sprhs = 7;
    actwk[0].yposi.w.h += 5;
    actwk[0].mstno.b.h = 2;
}

void ball_rd7(sprite_status *ballwk) {
    Sint16 cal_speed0;

    if (ballwk->userflag.b.h != 4) {
        if (ballwk->userflag.b.h == 2) {
            if (actwk[0].yspeed.w >= 0) {
                ball_ball();
                return;
            }
        } else if (actwk[0].yspeed.w < 0) {
            ball_ball();
            return;
        }
        cal_speed0 = 3328;
        if (!(swdata1.b.h & 8)) {
            if (!(swdata1.b.h & 4)) {
                ball_ball();
                return;
            } else
                cal_speed0 = -cal_speed0;
        }
        if (ballwk->userflag.b.h != 2)
            actwk[0].cddat |= 2;
        actwk[0].xspeed.w = actwk[0].mspeed.w = cal_speed0;
        ball_ball();
    } else {
        if (actwk[0].xspeed.w < 0) {
            ball_ball();
            return;
        }
        if (!(swdata1.b.h & 1)) {
            ball_ball();
            return;
        }
        actwk[0].yspeed.w = actwk[0].mspeed.w = -2560;
        actwk[0].cddat |= 2;
        ball_ball();
    }
}

Uint8 ball_coli(sprite_status *ballwk) {
    Sint16 cal_posi;

    if (editmode.b.h != 0)
        return 0;
    cal_posi = actwk[0].xposi.w.h - ballwk->xposi.w.h + 40;
    if (cal_posi < 0)
        return 0;
    if (cal_posi >= 80)
        return 0;
    cal_posi = actwk[0].yposi.w.h - ballwk->yposi.w.h + 40;
    if (cal_posi < 0)
        return 0;
    if (cal_posi >= 80)
        return 0;
    return 1;
}

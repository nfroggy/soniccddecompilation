#include "../equ.h"
#include "playsub4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../dummy.h"
#include "../etc.h"
#include "../io.h"
#include "../loader2.h"
#include "../player.h"

extern sprite_pattern* markerpat[]; extern Uint8* markerchg[]; extern sprite_pattern* bakupat[]; extern Uint8* bakuchg[]; extern sprite_pattern* exit2pat[]; extern Uint8* exit2chg[]; extern sprite_pattern* bariapat[]; extern Uint8* bariachg[]; extern Uint8 awasintbl[];
static Uint8 flowchg0[4] = { 3, 0, 1, 255 };
static Uint8 flowchg1[6] = { 3, 2, 3, 2, 3, 252 };
static Uint8 flowchg2[6] = { 1, 5, 5, 4, 6, 252 };
static Uint8 flowchg3[4] = { 19, 6, 7, 255 };
Uint8* flowchg[4] = {
  flowchg0,
  flowchg1,
  flowchg2,
  flowchg3
};
static sprite_pattern flo00 = {
  1, { { -4, -16, 0, 264 } }
};
static sprite_pattern flo01 = {
  1, { { -4, -16, 0, 265 } }
};
static sprite_pattern flo02 = {
  1, { { -8, -16, 0, 266 } }
};
static sprite_pattern flo03 = {
  1, { { -8, -16, 0, 267 } }
};
static sprite_pattern flo04 = {
  1, { { -12, -24, 0, 268 } }
};
static sprite_pattern flo05 = {
  1, { { -8, -16, 0, 269 } }
};
static sprite_pattern flo06 = {
  1, { { -12, -48, 0, 270 } }
};
static sprite_pattern flo07 = {
  1, { { -12, -48, 0, 271 } }
};
sprite_pattern* flow_pat[8] = {
  &flo00,
  &flo01,
  &flo02,
  &flo03,
  &flo04,
  &flo05,
  &flo06,
  &flo07
};
sprite_pattern boupat0 = { 1, { { -4, -46, 0, 423 } } }, boupat1 = { 1, { { -4, -46, 0, 424 } } };
sprite_pattern* boupat[2] = { &boupat0, &boupat1 };
sprite_pattern wavepat0 = { 1, { { -96, -8, 0, 425 } } }, wavepat1 = { 1, { { -96, -8, 0, 426 } } }, wavepat2 = { 1, { { -96, -8, 0, 427 } } }, wavepat3 = { 1, { { -96, -8, 0, 428 } } };
sprite_pattern* wavepat[8] = { &wavepat0, &wavepat0, &wavepat1, &wavepat1, &wavepat2, &wavepat2, &wavepat3, &wavepat3 };
sprite_pattern awasp0 = { 1, { { -4, -4, 0, 403 } } }, awasp1 = { 1, { { -4, -4, 0, 404 } } }, awasp2 = { 1, { { -4, -4, 0, 405 } } }, awasp3 = { 1, { { -8, -8, 0, 406 } } }, awasp4 = { 1, { { -8, -8, 0, 407 } } }, awasp5 = { 1, { { -8, -8, 0, 407 } } }, awasp6 = { 1, { { -16, -16, 0, 408 } } }, awasp7 = { 1, { { -16, -16, 0, 409 } } }, awasp8 = { 1, { { -16, -16, 0, 409 } } }, awasp9 = { 1, { { -8, -12, 0, 410 } } }, awasp10 = { 1, { { -8, -12, 0, 411 } } }, awasp11 = { 1, { { -8, -12, 0, 412 } } }, awasp12 = { 1, { { -8, -12, 0, 413 } } }, awasp13 = { 1, { { -8, -12, 0, 414 } } }, awasp14 = { 1, { { -8, -12, 0, 415 } } }, awasp15 = { 1, { { -8, -12, 0, 416 } } }, awasp16 = { 1, { { -8, -12, 0, 417 } } }, awasp17 = { 1, { { -8, -12, 0, 418 } } }, awasp18 = { 1, { { -8, -12, 0, 419 } } }, awasp19 = { 1, { { -8, -16, 0, 420 } } }, awasp20 = { 1, { { -8, -16, 0, 421 } } }, awasp21 = { 1, { { -8, -16, 0, 422 } } }, awasp22;
sprite_pattern* awapat[23] = { &awasp0, &awasp1, &awasp2, &awasp3, &awasp4, &awasp5, &awasp6, &awasp7, &awasp8, &awasp9, &awasp10, &awasp11, &awasp12, &awasp13, &awasp14, &awasp15, &awasp16, &awasp17, &awasp18, &awasp19, &awasp20, &awasp21, &awasp22 };
Uint8 plawachg0[9] = { 5, 0, 1, 2, 3, 4, 9, 13, 252 }, plawachg1[9] = { 5, 0, 1, 2, 3, 4, 12, 18, 252 }, plawachg2[9] = { 5, 0, 1, 2, 3, 4, 12, 17, 252 }, plawachg3[9] = { 5, 0, 1, 2, 3, 4, 11, 16, 252 }, plawachg4[9] = { 5, 0, 1, 2, 3, 4, 9, 15, 252 }, plawachg5[9] = { 5, 0, 1, 2, 3, 4, 10, 14, 252 }, plawachg6[5] = { 14, 0, 1, 2, 252 }, plawachg7[8] = { 7, 22, 13, 22, 13, 22, 13, 252 }, plawachg8[8] = { 7, 22, 18, 22, 18, 22, 18, 252 }, plawachg9[8] = { 7, 22, 17, 22, 17, 22, 17, 252 }, plawachg10[8] = { 7, 22, 16, 22, 16, 22, 16, 252 }, plawachg11[8] = { 7, 22, 15, 22, 15, 22, 15, 252 }, plawachg12[8] = { 7, 22, 14, 22, 14, 22, 14, 252 }, plawachg13[2] = { 14, 252 }, plawachg14[6] = { 14, 1, 2, 3, 4, 252 };
Uint8* plawachg[16] = { plawachg0, plawachg1, plawachg2, plawachg3, plawachg4, plawachg5, plawachg6, plawachg7, plawachg8, plawachg9, plawachg10, plawachg11, plawachg12, plawachg13, plawachg14, plawachg13 };
Uint8 wavechg0[50] = { 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 255 };
Uint8* wavechg[1] = { wavechg0 };

void patchg(sprite_status* patchgwk, Uint8** pat_dat) {
  Uint8 pat_no;
  Uint8* sprpat_adr;

  if (patchgwk->mstno.b.h != patchgwk->mstno.b.l) {
    patchgwk->mstno.b.l = patchgwk->mstno.b.h;
    patchgwk->patcnt = patchgwk->pattim = 0;
  }

  if ((char)--patchgwk->pattim < 0) {
    sprpat_adr = pat_dat[patchgwk->mstno.b.h];

    pat_no = 0;
    while (sprpat_adr[pat_no] < 250) ++pat_no;
    if (sprpat_adr[pat_no] != 252)
      patchgwk->pattim = sprpat_adr[0] <= 0 ? 1 : sprpat_adr[0];
    else
      patchgwk->pattim = sprpat_adr[0];

    if ((pat_no = sprpat_adr[patchgwk->patcnt + 1]) < 128) {
label1:
      patchgwk->patno = pat_no & 31;
      pat_no >>= 5;

      pat_no ^= patchgwk->cddat;
      pat_no &= 3;
      patchgwk->actflg &= 252;
      patchgwk->actflg |= pat_no;
      ++patchgwk->patcnt;
    }
    else {
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
}

void playsave0(sprite_status* savewk) {
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
  if (pltime.l >= 327680) pltime_s = 327680;
  else pltime_s = pltime.l;

}

void marker(sprite_status* markerwk) {
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

void marker_init(sprite_status* markerwk) {
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
  if (actwkchk(&new_actwk) != 0) { frameout(markerwk); return; }
  new_actwk->actno = 19;
  new_actwk->r_no0 += 4;
  if (markerwk->actfree[4] != 0) new_actwk->r_no0 += 2;
  new_actwk->patbase = markerpat;
  new_actwk->sproffset = 1739;
  new_actwk->actflg = 4;
  new_actwk->sprhsize = 8;
  new_actwk->sprvsize = 8;
  new_actwk->sprpri = 3;
  new_actwk->patno = 1;
  parent = &((sprite_status**)new_actwk)[13];
  *parent = markerwk;
  new_actwk->xposi.w.h = markerwk->xposi.w.h;
  new_actwk->yposi.w.h = markerwk->yposi.w.h - 32;
  ((Sint16*)new_actwk)[23] = markerwk->xposi.w.h;
  marker_yposi_m_buf = markerwk->yposi.w.h - 24;
  ((Sint16*)new_actwk)[24] = marker_yposi_m_buf;
}

void marker_move0(sprite_status* markerwk) {

if (markerwk->actfree[4] != 0) return;

if (markerwk->colicnt == 0) return;
  markerwk->colino = 0;
  markerwk->actfree[4] = 1;
  markerno = markerwk->userflag.b.h;
  plflag = 1;
  playsave0(markerwk);
  soundset(174);
}

void marker_move1(sprite_status* markerwk) {
  Sint32 sin_data, cos_data;
  Sint16 sin_tmp, cos_tmp;
  sprite_status **parent, *new_actwk;

  if (markerwk->actfree[4] == 0) {
    parent = &((sprite_status**)markerwk)[13];
    new_actwk = *parent;
    if (new_actwk->actfree[4] == 0) return;
    markerwk->actfree[4] = 1;
  }

  markerwk->actfree[10] += 8;
  sinset(markerwk->actfree[10], &sin_tmp, &cos_tmp);
  sin_data = sin_tmp;
  cos_data = cos_tmp;
  sin_data <<= 3;
  sin_data /= 256;
  markerwk->xposi.w.h = ((Sint16*)markerwk)[23];
  markerwk->xposi.w.h += sin_data;
  cos_data = -cos_data << 3;
  cos_data /= 256;
  markerwk->yposi.w.h = ((Sint16*)markerwk)[24];
  markerwk->yposi.w.h += cos_data;
  if (markerwk->actfree[10] == 0) markerwk->r_no0 += 2;
}

void marker_move2(sprite_status* markerwk) {
  patchg(markerwk, markerchg);
}

void tensuu_set(sprite_status* tensuuwk) {
  if (tensuuwk->r_no1 != 0) return;
  tensuu0(tensuuwk, tensuuwk->actfree[21] >> 1);
}

void tensuu0(sprite_status* tensuuwk, Uint8 uf_data) {
  sprite_status* new_actwk;

  uf_data |= 128;
  if (actwkchk(&new_actwk) != 0) return;
  new_actwk->actno = 28;
  new_actwk->xposi.w.h = tensuuwk->xposi.w.h;
  new_actwk->yposi.w.h = tensuuwk->yposi.w.h;
  new_actwk->userflag.b.h = uf_data;
}

void bakuha(sprite_status* bakuhawk) {
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

void baku_init(sprite_status* bakuhawk) {
  bakuhawk->r_no0 += 2;
  bakuhawk->actflg |= 4;
  bakuhawk->sprpri = 1;

  bakuhawk->sproffset = 1664;

bakuhawk->patbase = bakupat;
  tensuu_set(bakuhawk);
  bakuhawk->colino = 0;
  bakuhawk->patcnt = 0;
  bakuhawk->pattim = 0;
  bakuhawk->mstno.w = 0;
  if (bakuhawk->userflag.b.h != 0) bakuhawk->mstno.w = 256;
  baku_move(bakuhawk);
}

void baku_move(sprite_status* bakuhawk) {
  patchg(bakuhawk, bakuchg);
  actionsub(bakuhawk);
}

void baku_die(sprite_status* bakuhawk) {
  if (bakuhawk->r_no1 != 0) { frameout(bakuhawk); return; }
  bakuhawk->actno = 31;
  bakuhawk->r_no0 = 0;
}

void flower(sprite_status* flowerwk) {
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

void flow_init(sprite_status* flowerwk) {
  Uint8* fwcnt_adr;
  fwcnt_adr = &flagworkcnt;

  flowerwk->actflg |= 4;
  flowerwk->sprpri = 1;
  flowerwk->sprvsize = 0;

  flowerwk->sproffset = 9943;

flowerwk->patbase = flow_pat;
  if (flowerwk->userflag.b.h == 0) {
    fwcnt_adr = flow_sub(flowerwk);
    flowerwk->r_no0 = 4;
    flowerwk->mstno.b.h = 3;
    if (*fwcnt_adr & 64) { flow_move1(flowerwk); return; }
  }
  flowerwk->mstno.w = 2;
  flowerwk->r_no0 = 2;
  flowerwk->sproffset = 1751;
  flow_move0(flowerwk);
}

void flow_move0(sprite_status* flowerwk) {
  Sint16 y_move = 0, flower_index = 0;
  Uint8* fwcnt_adr;

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

void flow_move1(sprite_status* flowerwk) {
  patchg(flowerwk, flowchg);
}

Uint8* flow_sub(sprite_status* flowerwk) {
  Sint16 index;

  index = (Sint16)(time_flag & 127) + flowerwk->cdsts * 3;
  return &flagwork[index];
}

Sint16 flow_sub1(Uint8 fl_counter) {
  Sint16 flower_index;

  flower_index = ((Uint16)fl_counter & 63) + (Uint16)(time_flag & 127) * 64;

return flower_index;
}

void flow_move2(sprite_status* flowerwk) {
  flowerwk->sproffset = 9943;

  flowerwk->mstno.b.h = 2;
  flow_move1(flowerwk);
}

void flow_move3(sprite_status* flowerwk) {
  flowerwk->mstno.b.h = 3;
  flowerwk->r_no0 = 4;
  flow_move1(flowerwk);
}

void exit2(sprite_status* exitwk) {
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

void exit2_init(sprite_status* exitwk) {
  exitwk->r_no0 += 2;
  exitwk->actflg = 4;
  exitwk->sprpri = 1;
  exitwk->patbase = exit2pat;
  exitwk->sproffset = 664;
  exitwk->mstno.b.h = exitwk->userflag.b.h;

  if (exitwk->userflag.b.h < 2) soundset(162);
  else soundset(161);
  exit2_move(exitwk);
}

void exit2_move(sprite_status* exitwk) {
  exitwk->yposi.w.h = waterposi;
  exitwk->yposi.w.h += 4;
  patchg(exitwk, exit2chg);
  actionsub(exitwk);
}

void exit2_erase(sprite_status* exitwk) {
  frameout(exitwk);
}

void baria(sprite_status* bariawk) {
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

}

void bariainit(sprite_status* bariawk) {
  bariawk->r_no0 += 2;
  bariawk->patbase = bariapat;
  bariawk->actflg = 4;
  bariawk->sprpri = 1;
  bariawk->sprhsize = 16;
  bariawk->sproffset = 1348;
  if (bariawk->mstno.b.h == 0) return;
  bariawk->r_no0 += 2;
  if (bariawk->mstno.b.h < 5) return;
  bariawk->r_no0 += 2;
}

void bariamove(sprite_status* bariawk) {
  if (plpower_b == 0) { frameout(bariawk); return; }
  if (plpower_a != 0 || plpower_m != 0) return;
  bariawk->xposi.w.h = actwk[0].xposi.w.h;
  bariawk->yposi.w.h = actwk[0].yposi.w.h;
  bariawk->cddat = actwk[0].cddat;

patchg(bariawk, bariachg);
  actionsub0(bariawk);
}

void mutekimove(sprite_status* bariawk) {
  if (plpower_a != 0) return;
  if (plpower_m != 0) { muteki_sub(bariawk); return; }
  frameout(bariawk);
}

void backtomove(sprite_status* bariawk) {
  if (plpower_a != 0) { muteki_sub(bariawk); return; }
  frameout(bariawk);
}

void muteki_sub(sprite_status* bariawk) {
  ushort_union ppw_offset;
  Uint8 cal_no;

ppw_offset.w = plposiwkadr.w;
  cal_no = bariawk->mstno.b.h - 1;
  if (cal_no >= 4) cal_no -= 4;
  cal_no = cal_no * 24 + 4;
  ppw_offset.b.l = ppw_offset.b.l - cal_no;
  cal_no = bariawk->actfree[6];
  ppw_offset.b.l = ppw_offset.b.l - cal_no;
  if ((cal_no += 4) >= 24) cal_no = 0;
  bariawk->actfree[6] = cal_no;

  bariawk->xposi.w.h = playposiwk[ppw_offset.w / 2];
  bariawk->yposi.w.h = playposiwk[ppw_offset.w / 2 + 1];

  bariawk->cddat = actwk[0].cddat;
  patchg(bariawk, bariachg);
  actionsub0(bariawk);
}

void actionsub0(sprite_status* bariawk) {
  if ((plsubchg_flag & 15) >= 8) return;

  if (bariawk->r_no0 != 0) plsubchg_flag = bariawk->r_no0 | 128;
  actionsub(bariawk);
}

void test_act(sprite_status* testwk) {
  tensuu_set(testwk);
}

void mizukiri(sprite_status* mizukiwk) {
  exit2(mizukiwk);
}

void futa(sprite_status* futawk) {
  exit2(futawk);
}

void exit2_set(sprite_status* testwk) {
  exit2(testwk);
}

void plairset(void) {
  Sint16 wD0;

  if (pl_air < 13) {
    wD0 = 130;
    if (stageno.w == 259) wD0 = 134;

    if (plpower_m) wD0 = 135;

    if (bossstart) wD0 = 140;

    bgmset(wD0);
  }

  pl_air = 30;
  actwk[7].actfree[8] = 0;
}

void plawamaster(sprite_status* pActwk) {
  if (!((Uint16*)pActwk)[24]) {
    if (actwk[0].r_no0 >= 6) return;
    if (!(actwk[0].cddat & 64)) return;
    if (--((Sint16*)pActwk)[30] >= 0) {
      plawamaster_jump(pActwk);
      return;
    }
    ((Sint16*)pActwk)[30] = 59;
    ((Sint16*)pActwk)[29] = 1;
    pActwk->actfree[10] = random() & 1;

    if (pl_air != 25) {
      if (pl_air != 20) {
        if (pl_air != 15) {
          if (pl_air < 13) {

            if (!(gametimer.b.l & 32)) soundset(223);

            if (--pActwk->actfree[8] & 128) {
              pActwk->actfree[8] = pActwk->actfree[9];
              pActwk->actfree[12] |= 128;
            }
          }
        }
      }
    }

if (--pl_air >= 0) {
      plawamaster_jump0(pActwk);
      return;
    }

    plairset();
    actwk[0].actfree[2] = 129;
    soundset(182);
    pActwk->actfree[10] = 10;
    ((Sint16*)pActwk)[29] = 1;
    ((Sint16*)pActwk)[24] = 120;

    jumpcolsub();
    actwk[0].mstno.b.h = 23;
    actwk[0].cddat |= 2;

actwk[0].sproffset |= 32768;
    actwk[0].sprpri = 0;
    actwk[0].yspeed.w = 0;
    actwk[0].xspeed.w = 0;
    actwk[0].mspeed.w = 0;
    scroll_start.b.h = 1;
    return;
  }

  if (!(--((Sint16*)pActwk)[24])) {
    actwk[0].r_no0 = 6;
    return;
  }

  speedset2(&actwk[0]);
  actwk[0].yspeed.w += 16;
  plawamaster_jump(pActwk);
}

void plawamaster_jump0(sprite_status* pActwk) {
  plawamaster_jump2(pActwk);
}

void plawamaster_jump(sprite_status* pActwk) {
  if (!((Sint16*)pActwk)[29]) return;
  if (--((Sint16*)pActwk)[31] >= 0) return;
  plawamaster_jump2(pActwk);
}

void plawamaster_jump2(sprite_status* pActwk) {
  sprite_status* pNewactwk;
  Sint16 wD0;

  ((Sint16*)pActwk)[31] = (Uint16)random() & 15;
  if (actwkchk(&pNewactwk) == 0) {
    pNewactwk->actno = 33;
    pNewactwk->xposi.w.h = actwk[0].xposi.w.h;
    wD0 = 6;
    if (actwk[0].cddat & 1) {
      wD0 *= -1;
      pNewactwk->direc.b.h = 64;
    }

    pNewactwk->xposi.w.h += wD0;
    pNewactwk->yposi.w.h = actwk[0].yposi.w.h;
    pNewactwk->userflag.b.h = 6;

    if (((Uint16*)pActwk)[24]) {
      ((Uint16*)pActwk)[31] &= 7;
      pActwk + 1;
      pNewactwk->yposi.w.h = actwk[0].yposi.w.h - 12;
      pNewactwk->direc.b.h = random() & 255;
      if (!(gametimer.w & 3)) pNewactwk->userflag.b.h = 14;
    }
    else {

      if (pActwk->actfree[12] & 128) {
        pNewactwk->sproffset = 8192;
        if (!(random() & 3)) {
          if (pActwk->actfree[12] & 64) goto label1;
          pActwk->actfree[12] |= 64;
          pNewactwk->userflag.b.h = (Uint16)pl_air >> 1 & 255;
          ((Sint16*)pNewactwk)[30] = 28;
        }

        if (!pActwk->actfree[10]) {
          if (pActwk->actfree[12] & 64) goto label1;
          pActwk->actfree[12] |= 64;
          pNewactwk->userflag.b.h = pl_air >> 1 & 255;
          ((Sint16*)pNewactwk)[30] = 28;
        }
      }
    }

label1:
    if (--pActwk->actfree[10] & 128) ((Sint16*)pActwk)[29] = 0;
  }

}

void plawa(sprite_status* pActwk) {
  void(*plawa_move_tbl[9])(sprite_status*) = {
    &plawainit,
    &plawamove,
    &plawamove2,
    &plawamove3,
    &plawamove4,
    &plawamaster,
    &plawamove5,
    &plawamove3,
    &plawamove4
  };

  plawa_move_tbl[pActwk->r_no0 / 2](pActwk);
}

void plawainit(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->patbase = awapat;
  pActwk->sproffset |= 33930;
  pActwk->actflg = 132;
  pActwk->sprhsize = 16;
  pActwk->sprpri = 1;
  if (pActwk->userflag.b.h & 128) {
    pActwk->r_no0 += 8;
    pActwk->patbase = awapat;
    pActwk->sproffset |= 1162;
    pActwk->actfree[9] = pActwk->userflag.b.h & 127;
    plawamaster(pActwk);
  }
  else {

    pActwk->mstno.b.h = pActwk->userflag.b.h;
    ((Sint16*)pActwk)[26] = pActwk->xposi.w.h;
    pActwk->yspeed.w = -136;
    plawamove(pActwk);
  }
}

void plawamove(sprite_status* pActwk) {
  patchg(pActwk, plawachg);
  plawamove2(pActwk);
}

void plawamove2(sprite_status* pActwk) {
  if (waterposi >= pActwk->yposi.w.h) {

pActwk->r_no0 = 6;
    pActwk->mstno.b.h += 7;
    if (pActwk->mstno.b.h > 15) pActwk->mstno.b.h = 15;

    plawamove3(pActwk);
    return;
  }

if (watercoliflag) ((Sint16*)pActwk)[26] += 4;

  pActwk->xposi.w.h = (Sint16)(char)awasintbl[pActwk->direc.b.h++ + 1 & 127] + ((Sint16*)pActwk)[26];

  plawasub(pActwk);
  speedset2(pActwk);

  if (pActwk->actflg & 128) actionsub(pActwk);
  else frameout(pActwk);
}

void plawamove3(sprite_status* pActwk) {
  plawasub(pActwk);
  patchg(pActwk, plawachg);
  actionsub(pActwk);
}

void plawamove4(sprite_status* pActwk) {
  frameout(pActwk);
}

void plawamove5(sprite_status* pActwk) {
  if (pl_air > 12) {
    frameout(pActwk);
    return;
  }
  if (!(--((Sint16*)pActwk)[30])) {
    pActwk->r_no0 = 14;
    pActwk->mstno.b.h += 7;
    if (pActwk->mstno.b.h > 15) pActwk->mstno.b.h = 15;

    plawamove3(pActwk);
    return;
  }

  patchg(pActwk, plawachg);
  if (pActwk->actflg & 128) actionsub(pActwk);
  else frameout(pActwk);
}

void plawasub(sprite_status* pActwk) {
  if (((Sint16*)pActwk)[30]) {
    if (!(--((Sint16*)pActwk)[30])) {
      if (pActwk->mstno.b.h < 7) {
        ((Sint16*)pActwk)[30] = 15;
        pActwk->yspeed.w = 0;
        pActwk->actflg = 129;
        pActwk->xposi.w.h = pActwk->xposi.w.h - scra_h_posit.w.h + 128;
        pActwk->yposi.w.h = pActwk->yposi.w.h - scra_v_posit.w.h + 128;
        pActwk->r_no0 = 12;
      }
    }
  }

}

void wave(sprite_status* pActwk) {
  void(*wave_move_tbl[2])(sprite_status*) = {
    &wave_init,
    &wave_move
  };

  wave_move_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

void wave_init(sprite_status* pActwk) {
  pActwk->actflg |= 4;
  pActwk->sproffset = 33408;
  pActwk->patbase = wavepat;
  pActwk->r_no0 += 2;
  wave_move(pActwk);
}

void wave_move(sprite_status* pActwk) {
  Sint16 wave_tbl[2] = {
    -96, 96
  };

  if (++pActwk->actfree[0] >= 4) {
    pActwk->actfree[0] = 0;
  }
  pActwk->xposi.w.h = scra_h_posit.w.h + 160 + wave_tbl[pActwk->actfree[0] / 2];
  pActwk->yposi.w.h = waterposi + 2;
  patchg(pActwk, wavechg);
}

void bou(sprite_status* pActwk) {
  void(*bou_move_tbl[4])(sprite_status*) = {
    &bou_init,
    &bou_move0,
    &bou_move1,
    &bou_move2
  };

  bou_move_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

void bou_init(sprite_status* pActwk) {
  pActwk->actflg |= 4;
  pActwk->sproffset = 17144;
  pActwk->patbase = boupat;
  pActwk->r_no0 += 2;
  bou_move0(pActwk);
}

void bou_move0(sprite_status* pActwk) {
  Sint16 wD0, wD1;

  if (bou_coli(pActwk) != 0) {
    actwk[0].actfree[2] |= 1;

    actwk[0].xposi.w.h = pActwk->xposi.w.h;

    wD1 = 0;
    wD0 = 24;

    if (actwk[0].xspeed.w < 0) {
      wD1 = 1;
      wD0 *= -1;
    }

    actwk[0].cddat &= 254;
    actwk[0].cddat |= wD1;
    pActwk->actflg |= wD1;
    actwk[0].xposi.w.h += wD0;
    actwk[0].mstno.b.h = 17;
    ((Sint16*)pActwk)[23] = 180;
    if (pActwk->userflag.b.h) ((Sint16*)pActwk)[23] = 60;

pActwk->r_no0 += 2;
  }
}

void bou_move1(sprite_status* pActwk) {

if (--((Sint16*)pActwk)[23]) {

    if (!(swdata1.b.l & 112)) {
      if (swdata1.b.h & 1) {
        if (pActwk->yposi.w.h - 24 >= --actwk[0].yposi.w.h)
          actwk[0].yposi.w.h = pActwk->yposi.w.h - 24;
      }
      else {

        if (swdata1.b.h & 2) {
          if (pActwk->yposi.w.h + 24 < ++actwk[0].yposi.w.h)
            actwk[0].yposi.w.h = pActwk->yposi.w.h + 24;
        }
      }
      return;
    }
  }

  actwk[0].actfree[2] &= 254;
  pActwk->patno = 1;
  pActwk->r_no0 += 2;
}

void bou_move2(sprite_status* pActwk) {

}

Sint32 bou_coli(sprite_status* pActwk) {
  Sint16 wD0;

  if (editmode.b.h) return 0;

  wD0 = pActwk->xposi.w.h - actwk[0].xposi.w.h + 4;
  if (wD0 < 0) return 0;
  if (wD0 >= 8) return 0;

  wD0 = pActwk->yposi.w.h - actwk[0].yposi.w.h + 24;
  if (wD0 < 0) return 0;
  if (wD0 >= 48) return 0;

  return 1;
}

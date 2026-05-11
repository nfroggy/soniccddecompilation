#include "../../types.h"
#include "thanks_types.h"
#include "spm.h"
#include "../../impfuncs.h"

Uint16 comdata_s7 = 0;
Uint16 comdata_s6 = 0;
Uint16 comdata_s5 = 0;
Uint16 comdata_s4 = 0;
Uint16 comdata_s3 = 0;
Uint16 comdata_s2 = 0;
Uint16 comdata_s1 = 0;
Uint16 comdata_s0 = 0;
short_union swdata2 = { 0 };
short_union swdata1 = { 0 };
Uint16 comdata_m7 = 0;
Uint16 comdata_m6 = 0;
Uint16 comdata_m5 = 0;
Uint16 comdata_m4 = 0;
Uint16 comdata_m3 = 0;
Uint16 comdata_m2 = 0;
Uint16 comdata_m1 = 0;
Uint16 comdata_m0 = 0;
Uint8 comflag_s = 0;
Uint8 comflag_m = 0;
Sint16 end_timer = 0;
Sint16 heart_tm = 0;
Sint16 animal_timer = 0;
Uint8 animal_flg = 0;
Uint8 poseno = 0;
Sint16 posetimer = 0;
Sint16 coltimer2 = 0;
Sint16 colorno2 = 0;
Sint16 actset_tm[8] = { 0 };
Sint16 wkarea3 = 0;
Sint16 wkarea2 = 0;
Sint16 wkarea1 = 0;
Sint16 prg_mode = 0;
Uint8 colorcnt = 0;
Uint8 startcolor = 0;
Sint32 overint = 0;
Sint32 ranum = 0;
Sint16 sr_save = 0;
Sint16 gmtime1 = 0;
Sint16 gmtime0 = 0;
Sint16 intmode = 0;
Sint16 linkdata = 0;
Sint16 coltimer = 0;
Sint16 colorno = 0;
Uint8 colorflg = 0;
Uint8 int_flg = 0;
Uint8 push_flg = 0;
Uint8 quit_flg = 0;
sprite_status_thanks** priptr[4] = { 0 };
sprite_status_thanks* pribuf3[64] = { 0 };
sprite_status_thanks* pribuf2[64] = { 0 };
sprite_status_thanks* pribuf1[64] = { 0 };
sprite_status_thanks* pribuf0[64] = { 0 };
int_union* lphscrollbuff = 0;
PALETTEENTRY* lpcolorwk3 = 0;
PALETTEENTRY* lpcolorwk2 = 0;
PALETTEENTRY* lpcolorwk1 = 0;
PALETTEENTRY* lpcolorwk0 = 0;
sprite_status_thanks actwk[66] = { 0 };
bmp_info SprBmp[700] = { 0 };
Sint32 ProcMode = 0;
Sint32 DLLRetMode = 0;
Sint32* lpFadeFlag = 0;
Sint16 FadeFlag = 0;

void WorkRamClear(void) {
  FadeFlag = 0;
  *lpFadeFlag = 0;
  DLLRetMode = 0;
  ProcMode = 0;

  sMemSet(actwk, 0, sizeof(actwk));

sMemSet(pribuf0, 0, sizeof(pribuf0));
  sMemSet(pribuf1, 0, sizeof(pribuf1));
  sMemSet(pribuf2, 0, sizeof(pribuf2));
  sMemSet(pribuf3, 0, sizeof(pribuf3));

priptr[0] = 0;
  priptr[1] = 0;
  priptr[2] = 0;
  priptr[3] = 0;

  quit_flg = 0;
  push_flg = 0;
  int_flg = 0;
  colorflg = 0;
  colorno = 0;
  coltimer = 0;

  linkdata = 0;
  intmode = 0;
  gmtime0 = 0;
  gmtime1 = 0;
  sr_save = 0;
  ranum = 0;
  overint = 0;

  startcolor = 0;
  colorcnt = 0;

  prg_mode = 0;

  wkarea1 = 0;
  wkarea2 = 0;
  wkarea3 = 0;

  sMemSet(actset_tm, 0, sizeof(actset_tm));

  colorno2 = 0;
  coltimer2 = 0;

  posetimer = 0;
  poseno = 0;
  animal_flg = 0;
  animal_timer = 0;
  heart_tm = 0;

  end_timer = 0;

comflag_m = 0;
  comflag_s = 0;

  comdata_m0 = 0;
  comdata_m1 = 0;
  comdata_m2 = 0;
  comdata_m3 = 0;
  comdata_m4 = 0;
  comdata_m5 = 0;
  comdata_m6 = 0;
  comdata_m7 = 0;
  swdata1.w = 0;
  swdata2.w = 0;
  comdata_s0 = 0;
  comdata_s1 = 0;
  comdata_s2 = 0;
  comdata_s3 = 0;
  comdata_s4 = 0;
  comdata_s5 = 0;
  comdata_s6 = 0;
  comdata_s7 = 0;
}

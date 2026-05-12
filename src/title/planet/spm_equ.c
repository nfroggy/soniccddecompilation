#include "../../types.h"
#include "lpl_types.h"
#include "../../impfuncs.h"

Sint32 *lpFadeFlag;
Sint32 DLLRetMode;
Sint32 ProcMode;
bmp_info SprBmp[700];
Uint8 m2save[22528];
sprite_status_lpl actwk[18];
Uint16 coltimer;
Sint16 colorno;
Sint16 funka_pctimer;
Sint16 funka_patno;
Sint16 main_lpcnt;
Sint16 spr_coltimer;
Sint16 spr_colno;
Uint8 music_selflg;
Sint16 actset_tm[6];
Uint8 actset_flg;
Uint8 actset_wk;
Uint8 push_flg;
Uint8 int_flg;
Uint8 colorflg;
Sint16 linkdata;
Sint16 intmode;
Sint16 gmtime0;
Sint16 gmtime1;
Sint16 sr_save;
Sint32 ranum;
Sint32 overint;
Sint16 flash_timer;
Sint16 lplanet_no;
Uint8 startcolor;
Uint8 colorcnt;
Uint8 flash_flg;
Uint8 cgchg_timer;
Uint8 cgchg_end;
short_union stageno;
PALETTEENTRY *lpcolorwk0;
PALETTEENTRY *lpcolorwk1;
PALETTEENTRY *lpcolorwk2;
PALETTEENTRY *lpcolorwk3;
int_union *lphscrollbuff;
Uint8 comflag_m;
Uint8 comflag_s;
Uint16 comdata_m0;
Uint16 comdata_m1;
Uint16 comdata_m2;
Uint16 comdata_m3;
Uint16 comdata_m4;
Uint16 comdata_m5;
Uint16 comdata_m6;
Uint16 comdata_m7;
ushort_union swdata1;
ushort_union swdata2;
Uint16 comdata_s0;
Uint16 comdata_s1;
Uint16 comdata_s2;
Uint16 comdata_s3;
Uint16 comdata_s4;
Uint16 comdata_s5;
Uint16 comdata_s6;
Uint16 comdata_s7;
wordram_info wordram;
Sint16 tv_adr[896];

void WorkRamClear(void) {
    *lpFadeFlag = 0;
    DLLRetMode = 0;
    ProcMode = 0;

    sMemSet(actwk, 0, sizeof(actwk));

    coltimer = 0;
    colorno = 0;
    funka_pctimer = 0;
    funka_patno = 0;
    main_lpcnt = 0;
    spr_coltimer = 0;
    spr_colno = 0;

    sMemSet(actset_tm, 0, 6);
    actset_flg = 0;
    actset_wk = 0;
    push_flg = 0;
    int_flg = 0;
    colorflg = 0;

    linkdata = 0;
    intmode = 0;
    gmtime0 = 0;
    gmtime1 = 0;
    sr_save = 0;

    ranum = 0;
    overint = 0;

    flash_timer = 0;
    lplanet_no = 0;
    startcolor = 0;
    colorcnt = 0;
    flash_flg = 0;
    cgchg_timer = 0;
    cgchg_end = 0;

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

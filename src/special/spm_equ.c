#include "../types.h"

int_union overint = {0};
Uint8 *demoadr = 0;
Sint32 hsdata5 = 0;
Sint32 hsdata4 = 0;
Sint32 hsdata3 = 0;
Sint32 hsdata2 = 0;
Sint32 hsdata1 = 0;
Sint32 hsdata0 = 0;
Uint16 sonicup_no = 0;
Sint16 gmtime1 = 0;
Sint16 gmtime0 = 0;
Uint16 intmode = 0;
Uint8 spgmmode_tmp = 0;
Uint8 clfflg = 0;
Uint8 demoflg = 0;
Uint8 pauseflg = 0;
PALETTEENTRY colorsave[64] = {0};
Uint8 demowk[4096] = {0};
Uint16 dummy100[128] = {0};
Uint16 dummy000[128] = {0};
Uint32 hdatabuf_l[128] = {0};
Uint32 *hdatabuf = 0;
Uint16 mpopenwk[1024] = {0};
Uint8 rlscrflg_save = 0;

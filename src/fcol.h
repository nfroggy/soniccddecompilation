#include "types.h"
Sint16 fcol(sprite_status *pActwk);
Sint16 fcol_d(sprite_status *pActwk);
Sint16 fcol_r(sprite_status *pActwk);
Sint16 fcol_u(sprite_status *pActwk);
Sint16 fcol_l(sprite_status *pActwk);
Sint16 scdend(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon, char *cpDirStk);
Sint16 scdchk(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
              Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon, char *cpDirStk);
Sint16 scdchk2(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
               Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon, char *cpDirStk);
Sint16 scdend_r(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
                Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon, char *cpDirstk);
Sint16 scdchk_r(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
                Sint16 iOffset, Sint16 iBlkMsk, Sint16 iRideon, char *cpDirstk);
Sint16 scdchk2_r(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi,
                 Sint16 iBlkMsk, Sint16 iRideon, char *cpDirstk);
Sint16 scramapad(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dirchk(sprite_status *pActwk, Sint16 scd0, Sint16 scd1);
Sint16 scdcnv(void);
Sint16 st_elseDataChk(Sint16 iBlkNo, sprite_status *pActwk);
Sint16 st_wackyDataChk(Sint16 iBlkNo, sprite_status *pActwk);
Sint16 st_starDataChk(Sint16 iBlkNo, sprite_status *pActwk);
Sint16 st_metalDataChk(Sint16 iBlkNo, sprite_status *pActwk);
Uint8 CCset(Sint32 lSrc, Sint32 lDst);
Uint8 CSset(Uint16 wSrc, Uint16 wDst);
Uint8 bchg(Uint8 bySrc, Uint8 byDst);

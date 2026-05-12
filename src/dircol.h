#include "types.h"
Uint32 swap(Sint32 lSrc);
Sint16 dircolm(sprite_status *pActwk, char *cpDirec);
Sint16 dircol(sprite_status *pActwk, char *cpDirec);
Sint16 dircol2(sprite_status *pActwk, char *cpDirec);
Sint16 dircol_d(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_d3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dircol_r(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_r2(sprite_status *pActwk);
Sint16 dircol_r3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dircol_l(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_l2(sprite_status *pActwk);
Sint16 dircol_l3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dircol_u(sprite_status *pActwk, Sint16 *ipD0, Sint16 *ipD1,
                Sint16 *ipD3);
Sint16 dircol_u2(sprite_status *pActwk);
Sint16 dircol_u3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 emycol_u(sprite_status *pActwk);
Sint16 emycol_d(sprite_status *pActwk);
Sint16 emycol_d2(sprite_status *pActwk, Sint16 iXposi);
Sint16 emycol_d3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 emycol_l(sprite_status *pActwk, Uint8 sprhs);
Sint16 emycol_l3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 emycol_r(sprite_status *pActwk, Uint8 sprhs);
Sint16 emycol_r3(sprite_status *pActwk, Sint16 iXposi, Sint16 iYposi);
Sint16 dircolchk(Sint16 *ipD0, Sint16 *ipD1, Sint16 *ipD3, char *cpDirec);
Sint16 dircolchk1(char cDirec);

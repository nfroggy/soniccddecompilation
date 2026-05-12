#include "../types.h"
sprite_status *main_chk(void);
void scr_set(void);
void scroll(void);
void scrollwrt(void);
void scrollwrtb(Uint8 *pScrFlag, Uint8 *pMapWk, Sint32 VramBase);
void scrollwrtc(void);
void scrollwrtz(void);
void hblockwrt(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
               Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
               Sint32 lpcnt);
void hblockwrt1(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
                Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
                Sint32 lpcnt);
void vblockwrt(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
               Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
               Sint32 lpcnt);
void blockwrt(Sint32 VramBase, POINT *pTilePoint, Sint32 BlkIndex);
Sint32 mapadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 Uint8 *pMapWk, Sint32 *pIndex);
Sint32 mapadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                  Uint8 *pMapWk, Sint32 *pIndex);
Sint32 mapadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                   Uint8 *pMapWk, Sint32 *pIndex);
Sint32 mapadrset2(Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk, Sint32 *pIndex,
                  Uint16 **ppBlockNo);
void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs);
Sint32 block_chk(Uint16 xOffs, Uint16 yOffs);
void vramadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                POINT *lpTilePoint);
void vramadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 POINT *lpTilePoint);
void vramadrset0(Uint16 xOffs, Uint16 yOffs, POINT *lpTilePoint);
void vramadrset2(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 POINT *lpTilePoint);
void vramadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                  POINT *lpTilePoint);
void mapwrt(void);
void mapwrtb(void);
void mapwrt2(Uint16 wH_posiw, Uint16 wV_posiw, Uint8 *pMapWk, Sint32 VramBase);
void mapwrt3(Uint16 wH_posiw, Uint16 wV_posiw, Uint8 *pMapWk, Sint32 VramBase,
             Uint16 wD4, Uint16 wD6);
void mapwrt_z81a(Uint8 *pMapWk, Sint32 VramBase);
void mapwrt_sub(Uint8 *pWrttbl, Uint16 wD0, Uint16 wD4, Uint8 *pMapWk,
                Sint32 VramBase);
void mapinit(void);
void mapset(void);
void divdevset();
void enecginit(void);

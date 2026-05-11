#include "../types.h"
Sint16 pcol(sprite_status* pActwk);
Sint16 CollitblDataXchk(sprite_status* pActwk, sprite_status* pColliAct, Sint16 iXposi, Sint16 iYposi, Sint16 iD5);
Sint16 CollitblDataYchk(sprite_status* pActwk, sprite_status* pColliAct, Sint16 iXposi, Sint16 iYposi, Sint16 iColiNo, Sint16 iD5);
Sint16 ColliHitChk(sprite_status* pActwk, sprite_status* pColliAct, Sint16 iXposi, Sint16 iChkPosi, Sint16 iD5);
Sint16 pcolitem(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 pcolnomal(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 pcolplay2(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 pcolplay(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 pcole(sprite_status* pActwk, sprite_status* pColliAct);
void playdamagechk(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 playdamageset(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 playdieset(sprite_status* pActwk);
Sint16 pcolspecial(sprite_status* pActwk, sprite_status* pColliAct, Sint16 iXposi, Sint16 iChkPosi, Sint16 iD5);
Sint16 eggman_chk(sprite_status* pActwk, sprite_status* pColliAct);
Sint16 yago(sprite_status* pActwk, sprite_status* pColliAct, Sint16 iXposi, Sint16 iChkPosi, Sint16 iD5);
Sint16 main_attack(sprite_status* pActwk, sprite_status* pColliAct);

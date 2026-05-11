#include "../../types.h"
Uint32 CreateCharSprt(char c, Sint32 kind, Sint32 nBlockNo, Sint32 nSprNum);
Uint32 CreateStringSprt(char* str, Sint32 kind, Sint32 nBlockNo, Sint32 nStartSprNum);
void DeleteCharSprt(Sint32 nBlockNo, Sint32 nSprNum);
void DeleteStringSprt(Sint32 strLength, Sint32 nBlockNo, Sint32 nStartSprNum);
void DeleteAllCharSprt(void);
Sint32 GetCursolPositon(void);
void MovCursol(Sint32 x);
void DispCursol(void);
void HideCursol(void);
void HideSonicCursol(void);
void DispSonicCursol(POINT point);
void RunSonicCursol(POINT point);
void UpdateSonicCursol(void);

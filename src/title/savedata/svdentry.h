#include "../../types.h"
#include "../common/engine_dll.h"

void OESetSequenceNum(Uint16 num);
void KeyWait(void);
void WaitKeyOff(void);
void Wait(Sint32 time);
void WaitAndDfltMsg(Sint32 time);
void DLLInit(engine_dll* lpDllIn, char*** pBufTbl, void** pFuncTbl);
void DLLEnd(void);
void SWdataSet(ushort_union sw1, ushort_union sw2);
Sint32 DLLMain(void);

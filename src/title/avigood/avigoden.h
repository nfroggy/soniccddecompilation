#include "../../types.h"
#include "../common/engine_dll.h"

void OESetSequenceNum(Uint16 num);
void DLLInit(engine_dll *lpDllIn, char ***pBufTbl, void **pFuncTbl);
void DLLEnd(void);
void SWdataSet(ushort_union sw1, ushort_union sw2);
Sint32 DLLMain(void);

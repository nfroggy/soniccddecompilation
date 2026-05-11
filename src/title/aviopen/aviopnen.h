#include "../../types.h"
void OESetSequenceNum(Uint16 num);
void DLLInit(engine_dll* lpDllIn, char*** pBufTbl, void** pFuncTbl);
void DLLEnd(void);
void DLLPaint(Uint32 hdc);
Sint32 DLLNotify(Uint32 WPARAM, long LPARAM);
void DLLAVISizeChange(void);
void DLLAVIRealize(void);
void AVIPause(void);
void AVIResume(void);
void SWdataSet(ushort_union sw1, ushort_union sw2);
Sint32 DLLMain(void);

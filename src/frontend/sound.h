#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../types.h"

void WaveRequest(Sint16 req_no);
void CDPlay(Sint16 track_number);
void CDPause(Sint16 mode);
void WaveAllStop(void);

int Sound_Init(void);

#ifdef __cplusplus
}
#endif

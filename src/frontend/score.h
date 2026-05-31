#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../types.h"
#include "../title/common/score_data_types.h"

Sint32 ReadScoreData(Sint32 index, score_data *score, Uint32 existingFile);
Sint32 ReadScoreIndx(Uint32 existing_file);
Sint32 WriteScoreData(Sint32 index, score_data *score, Uint32 existing_file);
Uint32 WriteScoreIndx(Sint32 index, Uint32 existing_file);
Uint32 OpenScoreData(Sint32 index);
Uint32 CloseScoreData(Uint32 file);
Uint32 CreateScoreData(void);
void SetScoreDate(score_data *score);
Sint32 NewScoreData(void);

#ifdef __cplusplus
}
#endif

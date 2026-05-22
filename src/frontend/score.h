#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../types.h"
#include "../title/common/score_data_types.h"

Sint32 ReadScoreIndx(Uint32 existing_file);
Sint32 WriteScoreData(Sint32 index, score_data *score, Uint32 existing_file);
void SetScoreDate(score_data *score);

#ifdef __cplusplus
}
#endif

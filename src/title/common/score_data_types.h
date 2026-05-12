#include "../../types.h"
#ifndef SCORE_DATA_TYPES_H
#define SCORE_DATA_TYPES_H

typedef struct {
    Uint32 time;
    char name[4];
} time_data;

typedef struct {
    Uint32 saved;
    char player[12];
    Uint32 roundNo;
    Uint16 year;
    Uint16 month;
    Uint16 date;
    Uint16 hour;
    Uint16 min;
    Uint16 sec;
    time_data timeattack[7][3][3];
    time_data special[7][3];
    Uint32 total;
    Uint8 clrspflg_save;
    Uint8 clrgood;
    Uint8 stagenm;
    Uint8 reserved1;
    Uint8 reserved2;
    Uint32 checkSum;
} score_data;

#endif

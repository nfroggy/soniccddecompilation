#include "../common/sprite_pattern_types.h"

typedef struct {
  Uint16 ACT_NO;
  Uint16 EXE_NO;
  int_union XPOSI;
  int_union YPOSI;
  int_union X_SPEED;
  int_union Y_SPEED;
  int_union X_ACCEL;
  int_union Y_ACCEL;
  Sint16 X_OFFSET;
  Sint16 Y_OFFSET;
  Sint16 SIN_CNT;
  Sint16 TM_CNT;
  Sint16 SPR_TIMER;
  Sint16 PAT_NO;
  Sint16 CGBASE;
  sprite_patterns_title* PAT_ADR;
  Uint8 SPR_FLG;
  Uint8 ACT_FLG;
  Sint16 END_XPOSI;
  Sint16 END_YPOSI;
  Sint16 X_WIDE;
  Sint16 Y_WIDE;
  Uint8 free[8];
}
sprite_status_thanks;

typedef struct {
  Sint16 pts;
  char name[4];
}
high_score_entry;

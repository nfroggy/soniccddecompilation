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
  Uint8 timer1;
  Uint8 timer2;
}
sprite_status_lpl;

typedef struct {
  Sint16 number;
  Sint16* inipat;
}
ini_pats;

typedef struct {
  Uint8 mcntflg;
  Uint8 dummy;
  Uint8* copy_adr;
  Uint16 copy_size;
  Uint8* pc_adr;
  Uint8* user_hint;
  Uint8* user_vint;
}
mcnt_info;

typedef union {
  Uint8* wordram;
  mcnt_info mcntwk;
}
wordram_info;

typedef struct {
  Uint32 On;
  Uint32 Press;
  Uint32 Release;
  Sint16 X1;
  Sint16 Y1;
  Sint16 X2;
  Sint16 Y2;
}
pad_status;

typedef struct {
  Sint16 x;
  Sint16 y;
  Sint16 z;
  Sint16 az;
  Sint16 sz;
  Sint16 cz;
  Sint16 xscale;
  Sint16 yscale;
}
zoom_info_lpl;

typedef struct {
  Sint16 k_sin;
  Sint16 k_cos;
  Sint16 k_rx;
  Sint16 k_ry;
  Sint16 k_s;
  Sint16 k_msx;
  Sint16 k_msy;
  Sint16 k_dx;
  Sint16 k_dy;
  Sint16 k_x_rx;
  Sint16 k_y_ry;
  Sint32 k_xt_c;
  Sint32 k_yt_c;
  Sint16 k_s2;
  Sint16 k_eyelen;
}
rotate_1;

typedef struct {
  Sint16 actno;
  Uint8 actflg;
  Uint8 ACT_NO;
  Sint16 spdcnt;
  Sint16 k_kaku;
  Sint16 x_kaku;
  int_union x_posi;
  int_union y_posi;
  int_union z_posi;
  int_union k_speed;
  int_union x_speed;
  int_union y_speed;
  int_union z_speed;
}
rotate_2;

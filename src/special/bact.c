#include "../types.h"
#include "sps_equ.h"

static Uint16 stage_table_damage[8] = { 364, 428, 492, 460, 432, 364, 420, 364 };
static Uint16 stage_table_hane[8] = { 352, 416, 480, 448, 420, 352, 408, 352 };
extern game_info* lpKeepWork;

void Special_block_chg(Uint16* hane1, Uint16* hane2, Uint16* dmg1, Uint16* dmg2) {

  if ((Uint16)++hane_cnt > 2) hane_cnt = 0;
  *hane1 = stage_table_hane[lpKeepWork->stagenm] / 4 - 1;
  *hane2 = hane_cnt + stage_table_hane[lpKeepWork->stagenm] / 4 - 1;

  ++chg_timer;
  if (!(chg_timer & 1)) {

    if ((Uint16)++damage_cnt > 4 - 1) damage_cnt = 0;
  }

  *dmg1 = stage_table_damage[lpKeepWork->stagenm] / 4 - 1;
  *dmg2 = damage_cnt + stage_table_damage[lpKeepWork->stagenm] / 4 - 1;

}

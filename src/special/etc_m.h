#include "../types.h"
void sonic_disp(void);
void sonic_disp_disp(Sint16 tbl_no);
void time_bonus_disp(void);
void ring_bonus_disp(void);
void game_score_disp(void);
void cset(Sint32* subtbl, Uint16 x, Uint16 y, Sint32 disp_data, Sint16 counter);
Uint16 cntminus(Uint16* work, Uint16 minus_data, Uint16 under_limit, Uint16 upper_limit);
Uint16 cntplus(Uint16* work, Uint16 plus_data, Uint16 under_limit, Uint16 upper_limit);
void spetime_disp(void);
void ringno_disp(void);
void ufoleft_disp(void);
void sset(Sint32* subtbl, Uint16 x, Uint16 y, Sint32 disp_data, Sint16 counter);
void rfill(Uint32* address, Uint32 data, Sint16 size);
void rcopy(Uint32* address1, Uint32* address2, Sint16 size);

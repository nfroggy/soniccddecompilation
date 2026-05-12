#include "../types.h"
void action(void);
void actmain(sprite_status_sp *work);
void zbuf_init(void);
void zbuf_set(sprite_status_sp *work, Sint32 length);
void zbuf_patset(void);
void zpatset(Sint16 z_buffer_start);
Sint16 zpatset_call(Sint16 z_buffer_number);
void patinit(sprite_status_sp *work, Uint8 pattbl_number);
void patinit1(sprite_status_sp *work, Uint8 pattbl_number);
void patset(sprite_status_sp *work);
void n_patset(sprite_status_sp *work, sprite_pattern *sprpat_adr);

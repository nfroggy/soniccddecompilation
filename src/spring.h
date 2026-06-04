#ifndef SPRING_H
#define SPRING_H

#include "types.h"

typedef struct {
    Sint16 jump_speed;
    Sint16 ground_y;
    Uint16 parent_index;
    Sint16 origin_x;
    Sint8 follow_x_offset;
    Sint8 follow_y_offset;
} spring_work;

static inline spring_work *spring_work_get(sprite_status *actionwk) {
    return (spring_work *)actionwk->actfree;
}

void iwa(sprite_status *actionwk);
void iwa_init(sprite_status *actionwk);
void iwa_move(sprite_status *actionwk);
void koma(sprite_status *actionwk);
void koma_init(sprite_status *actionwk);
void koma_move(sprite_status *actionwk);
void koma_move2(sprite_status *actionwk);
void spring_d(sprite_status *actionwk);
void spr_d_init(sprite_status *actionwk);
void spr_d_move(sprite_status *actionwk);
void spring(sprite_status *actionwk);
void sjumpinit(sprite_status *actionwk);
Sint32 ride_on_chk_s(sprite_status *actionwk, sprite_status *a1);
void sjumpmove(sprite_status *actionwk);
void sjumpmove2(sprite_status *actionwk);
void sjumpmove3(sprite_status *actionwk);
Sint32 ride_on_chk_s1(sprite_status *actionwk, sprite_status *a1);
void sdushmove(sprite_status *actionwk);
void sdushmove2(sprite_status *actionwk);
void sdushmove3(sprite_status *actionwk);
Sint32 ride_on_chk_s2(sprite_status *actionwk, sprite_status *a1);
void sjump2move(sprite_status *actionwk);
void sjump2move2(sprite_status *actionwk);
void sjump2move3(sprite_status *actionwk);
void sjump3move(sprite_status *actionwk);
void sjump3move2(sprite_status *actionwk);
void sjump3move3(sprite_status *actionwk);

#endif

#ifndef SPRING_H
#define SPRING_H

#include <stddef.h>

#include "types.h"

#pragma pack(push, 1)
typedef struct {
    Sint8 unused0[6];
    Sint16 jump_speed;
    Sint16 ground_y;
    Uint16 parent_index;
    Sint16 origin_x;
    Sint8 follow_x_offset;
    Sint8 follow_y_offset;
} spring_work;
#pragma pack(pop)

_Static_assert(offsetof(spring_work, jump_speed) == 6,
               "spring_work.jump_speed offset");
_Static_assert(offsetof(spring_work, ground_y) == 8,
               "spring_work.ground_y offset");
_Static_assert(offsetof(spring_work, parent_index) == 10,
               "spring_work.parent_index offset");
_Static_assert(offsetof(spring_work, origin_x) == 12,
               "spring_work.origin_x offset");
_Static_assert(offsetof(spring_work, follow_x_offset) == 14,
               "spring_work.follow_x_offset offset");
_Static_assert(offsetof(spring_work, follow_y_offset) == 15,
               "spring_work.follow_y_offset offset");
_Static_assert(sizeof(spring_work) <= sizeof(((sprite_status *)0)->actfree),
               "spring_work fits in actfree");

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

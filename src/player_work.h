#ifndef PLAYER_WORK_H
#define PLAYER_WORK_H

#include "types.h"

typedef struct {
    Uint8 spin_dash_counter;
    Uint8 special_angle;
    Uint8 status_flags;
    Uint16 damage_invulnerability_timer;
    Uint16 invincibility_timer;
    Uint16 speed_shoes_timer;
    Uint8 floor_left;
    Uint8 floor_right;
    union {
        struct {
            Uint8 jump_lock;
            Uint8 orbit_radius;
        };
        Sint16 death_y;
    };
    Uint16 erase_timer;
    Uint8 jump_started;
    Uint8 ride_actor_index;
    Sint16 mode_word;
} player_work;

static inline player_work *player_work_get(sprite_status *pActwk) {
    return (player_work *)pActwk->actfree;
}

static inline const player_work *
player_work_get_const(const sprite_status *pActwk) {
    return (const player_work *)pActwk->actfree;
}

#endif

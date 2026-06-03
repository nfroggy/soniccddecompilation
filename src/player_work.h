#ifndef PLAYER_WORK_H
#define PLAYER_WORK_H

#include <stddef.h>

#include "types.h"

#pragma pack(push, 1)
typedef struct {
    Uint8 spin_dash_counter;
    Uint8 special_angle;
    Uint8 status_flags;
    Uint8 unused3[3];
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
#pragma pack(pop)

_Static_assert(offsetof(player_work, spin_dash_counter) == 0,
               "player_work.spin_dash_counter offset");
_Static_assert(offsetof(player_work, special_angle) == 1,
               "player_work.special_angle offset");
_Static_assert(offsetof(player_work, status_flags) == 2,
               "player_work.status_flags offset");
_Static_assert(offsetof(player_work, damage_invulnerability_timer) == 6,
               "player_work.damage_invulnerability_timer offset");
_Static_assert(offsetof(player_work, invincibility_timer) == 8,
               "player_work.invincibility_timer offset");
_Static_assert(offsetof(player_work, speed_shoes_timer) == 10,
               "player_work.speed_shoes_timer offset");
_Static_assert(offsetof(player_work, floor_left) == 12,
               "player_work.floor_left offset");
_Static_assert(offsetof(player_work, floor_right) == 13,
               "player_work.floor_right offset");
_Static_assert(offsetof(player_work, jump_lock) == 14,
               "player_work.jump_lock offset");
_Static_assert(offsetof(player_work, orbit_radius) == 15,
               "player_work.orbit_radius offset");
_Static_assert(offsetof(player_work, death_y) == 14,
               "player_work.death_y offset");
_Static_assert(offsetof(player_work, erase_timer) == 16,
               "player_work.erase_timer offset");
_Static_assert(offsetof(player_work, jump_started) == 18,
               "player_work.jump_started offset");
_Static_assert(offsetof(player_work, ride_actor_index) == 19,
               "player_work.ride_actor_index offset");
_Static_assert(offsetof(player_work, mode_word) == 20,
               "player_work.mode_word offset");
_Static_assert(sizeof(player_work) <= sizeof(((sprite_status *)0)->actfree),
               "player_work fits in actfree");

static inline player_work *player_work_get(sprite_status *pActwk) {
    return (player_work *)pActwk->actfree;
}

static inline const player_work *
player_work_get_const(const sprite_status *pActwk) {
    return (const player_work *)pActwk->actfree;
}

#endif

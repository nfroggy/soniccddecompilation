#include <stddef.h>

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
    sprite_patterns_title *PAT_ADR;
    Uint8 SPR_FLG;
    Uint8 ACT_FLG;
    Sint16 END_XPOSI;
    Sint16 END_YPOSI;
    Sint16 X_WIDE;
    Sint16 Y_WIDE;
    Uint8 free[8];
} sprite_status_thanks;

#pragma pack(push, 1)
typedef struct {
    Uint8 state_value;
    Uint8 secondary_state;
    Uint8 sound_flag;
    Uint8 action_flag;
    union {
        Sint16 pit_timer;
        Sint8 heart_timer;
        Uint8 heart_timer_raw;
    };
} thanks_sprite_work;
#pragma pack(pop)

_Static_assert(offsetof(thanks_sprite_work, state_value) == 0,
               "thanks_sprite_work.state_value offset");
_Static_assert(offsetof(thanks_sprite_work, secondary_state) == 1,
               "thanks_sprite_work.secondary_state offset");
_Static_assert(offsetof(thanks_sprite_work, sound_flag) == 2,
               "thanks_sprite_work.sound_flag offset");
_Static_assert(offsetof(thanks_sprite_work, action_flag) == 3,
               "thanks_sprite_work.action_flag offset");
_Static_assert(offsetof(thanks_sprite_work, pit_timer) == 4,
               "thanks_sprite_work.pit_timer offset");
_Static_assert(offsetof(thanks_sprite_work, heart_timer) == 4,
               "thanks_sprite_work.heart_timer offset");
_Static_assert(sizeof(thanks_sprite_work) <=
                   sizeof(((sprite_status_thanks *)0)->free),
               "thanks_sprite_work fits in free");

static inline thanks_sprite_work *
thanks_get_work(sprite_status_thanks *pActwk) {
    return (thanks_sprite_work *)pActwk->free;
}

typedef struct {
    Sint16 pts;
    char name[4];
} high_score_entry;

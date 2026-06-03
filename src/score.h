#ifndef SCORE_H
#define SCORE_H

#include <stddef.h>

#include "types.h"

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[20];
    Sint16 points_index;
} score_marker_work;
#pragma pack(pop)

_Static_assert(offsetof(score_marker_work, points_index) == 20,
               "score_marker_work.points_index offset");
_Static_assert(sizeof(score_marker_work) <= sizeof(((sprite_status *)0)->actfree),
               "score_marker_work must fit in sprite_status.actfree");

static inline score_marker_work *
score_marker_work_get(sprite_status *pActwk) {
    return (score_marker_work *)pActwk->actfree;
}

void score(sprite_status *pAct);
void scoreup(Uint32 updata);
void scoreset(void);

#endif

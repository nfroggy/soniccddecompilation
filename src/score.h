#ifndef SCORE_H
#define SCORE_H

#include "types.h"

typedef struct {
    Sint16 points_index;
} score_marker_work;

static inline score_marker_work *
score_marker_work_get(sprite_status *pActwk) {
    return (score_marker_work *)pActwk->actfree;
}

void score(sprite_status *pAct);
void scoreup(Uint32 updata);
void scoreset(void);

#endif

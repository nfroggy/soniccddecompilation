#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FRONTEND_STATE_EXIT,
    FRONTEND_STATE_TITLE,
    FRONTEND_STATE_ROUND,
    FRONTEND_STATE_WARP,
    FRONTEND_STATE_SPECIAL,
    FRONTEND_STATE_TIME_ATTACK,
} frontend_state;

#ifdef __cplusplus
}
#endif

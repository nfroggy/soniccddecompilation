#ifndef BACKEND_CALLBACK_LOG_H
#define BACKEND_CALLBACK_LOG_H

#include "types.h"

typedef struct easprset_call {
    Sint16 x;
    Sint16 y;
    Uint16 index;
    Uint16 link;
    Uint16 reverse;
} easprset_call;

void callback_log_reset(void);
void callback_log_easprset(Sint16 x, Sint16 y, Uint16 index, Uint16 link,
                           Uint16 reverse);
int callback_log_easprset_count(void);
const easprset_call *callback_log_easprset_at(int index);

#endif


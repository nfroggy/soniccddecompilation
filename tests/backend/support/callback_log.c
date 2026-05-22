#include "support/callback_log.h"

#include <stdlib.h>

#define EASPRSET_CALL_CAPACITY 256

static easprset_call easprset_calls[EASPRSET_CALL_CAPACITY];
static int easprset_call_count;

void callback_log_reset(void) { easprset_call_count = 0; }

void callback_log_easprset(Sint16 x, Sint16 y, Uint16 index, Uint16 link,
                           Uint16 reverse) {
    if (easprset_call_count >= EASPRSET_CALL_CAPACITY) {
        abort();
    }

    easprset_calls[easprset_call_count].x = x;
    easprset_calls[easprset_call_count].y = y;
    easprset_calls[easprset_call_count].index = index;
    easprset_calls[easprset_call_count].link = link;
    easprset_calls[easprset_call_count].reverse = reverse;
    ++easprset_call_count;
}

int callback_log_easprset_count(void) { return easprset_call_count; }

const easprset_call *callback_log_easprset_at(int index) {
    if (index < 0 || index >= easprset_call_count) {
        return 0;
    }

    return &easprset_calls[index];
}


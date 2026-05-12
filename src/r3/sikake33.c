#include "../equ.h"
#include "sikake33.h"
#include "light.h"
#include "rollplat.h"
#include "stopper.h"

void sikake33(sprite_status *actionwk) {

    if (actionwk->userflag.b.h < 0)
        kaitenban(actionwk);
    else {
        if (actionwk->userflag.b.h == 0)
            light(actionwk);
        else
            stopper(actionwk);
    }
}

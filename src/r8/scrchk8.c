#include "../equ.h"
#include "scrchk8.h"

static void zone8chk(void);

void scrchk(void) {
    Sint16 ywk, limwk;

    zone8chk();

    ywk = 4;

    if ((limwk = scralim_n_down - scralim_down) > 0) {

        if ((limwk = scra_v_posit.w.h + 8) >= scralim_down &&
            (actwk[0].cddat & 2)) {

            ywk *= 4;
        }

        scralim_down += ywk;
        limmoveflag = 1;
    } else if (limwk < 0) {
        ywk *= -1;

        if ((limwk = scra_v_posit.w.h) > scralim_n_down) {
            scralim_down = limwk & 65534;
        }

        scralim_down += ywk;
        limmoveflag = 1;
    }
}

static void zone8chk(void) {
    switch (stageno.b.l) {
    case 0:
    case 1:
        scralim_n_down = 1808;
        break;

    case 2:
        if (!bossflag) {
            scralim_n_down = 784;
        } else {
            scralim_down = scralim_n_down = 268;
        }
        break;
    }
}

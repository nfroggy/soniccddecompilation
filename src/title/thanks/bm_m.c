#include "../../types.h"
#include "../common/score_data_types.h"
#include "thanks_types.h"
#include "bm_m.h"
#include "../../impfuncs.h"
#include "act.h"
#include "chrset.h"
#include "game.h"

extern Sint16 prg_mode;
extern Sint16 actset_tm[8];
high_score_entry score[84];
char GreatestPlayer[4];
extern score_data *lpScorData;
extern Uint8 quit_flg;
extern short_union swdata1;

void mainloop(void) {
    void (*prg_tbl[2])(void) = {&get_tmdata, &get_keydata};

    prg_tbl[prg_mode]();
}

void get_tmdata(void) {
    get_usrname();
    dsp_usrname();

    actset_tm[0] = 600;
    actset_tm[1] = 0;
    actset_tm[2] = 0;
    actset_tm[3] = 0;
    actset_tm[4] = 0;
    actset_tm[5] = 0;
    actset_tm[6] = 0;
    actset_tm[7] = 600;

    prg_mode = 1;
}

void get_keydata(void) {
    player();
    sprinit();
    action();
}

void get_usrname(void) {
    Sint16 round, zone;
    Sint16 i, gp;

    sMemSet(score, 0, sizeof(score));

    for (round = 0; round < 7; ++round) {
        for (zone = 0; zone < 3; ++zone) {
            pointcnt(lpScorData->timeattack[round][zone][0].name, 3);
            pointcnt(lpScorData->timeattack[round][zone][1].name, 2);
            pointcnt(lpScorData->timeattack[round][zone][2].name, 1);
        }
    }
    for (round = 0; round < 7; ++round) {
        pointcnt(lpScorData->special[round][0].name, 3);
        pointcnt(lpScorData->special[round][1].name, 2);
        pointcnt(lpScorData->special[round][2].name, 1);
    }

    gp = 0;
    for (i = 0; i < 84; ++i) {
        if (score[i].pts == 0)
            break;
        if (score[i].pts >= score[gp].pts) {
            gp = i;
        }
    }
    sStrcpy(GreatestPlayer, score[gp].name);
}

void pointcnt(char *name, Sint16 pts) {
    Sint16 i;

    for (i = 0; i < 84; ++i) {
        if (score[i].pts) {
            if (sMemCmp(score[i].name, name, 3) != 0)
                continue;

            score[i].pts += pts;
            break;
        }

        score[i].pts = pts;
        sMemCpy(score[i].name, name, sizeof(score[i].name) - 1);
        score[i].name[3] = 0;
        break;
    }
}

Sint32 isdigit(Sint32 c) {
    if (c >= 48 && c < 58)
        return 1;
    else
        return 0;
}

Sint32 isupper(Sint32 c) {
    if (c >= 65 && c < 91)
        return 1;
    else
        return 0;
}

void dsp_usrname(void) {
    Sint16 i;
    Uint16 c;

    for (i = 0; i < 3; ++i) {
        if (isdigit(GreatestPlayer[i]) != 0) // goto 1000
        {
            c = GreatestPlayer[i] - 47;
        } else if (isupper(GreatestPlayer[i]) != 0) // goto 1080
        {
            c = GreatestPlayer[i] - 54;
        } else {
            continue;
        }
        PutAscii(c, i * 2 + 17, 12);
    }
}

void player(void) {
    if ((Uint8)swdata1.b.l & 240)
        quit_flg = 1;
    else
        quit_flg = 0;
}

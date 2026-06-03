#include <stddef.h>

#include "../equ.h"
#include "shoot1.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../player_work.h"

static sprite_pattern eda00 = {1, {{-16, -12, 0, 288}}};
static sprite_pattern eda01 = {1, {{-20, -16, 0, 289}}};
static sprite_pattern eda02 = {1, {{-12, -16, 0, 290}}};
sprite_pattern *edapat[3] = {&eda00, &eda01, &eda02};
static sprite_pattern kag00 = {1, {{-16, -16, 0, 291}}};
static sprite_pattern kag01 = {1, {{-16, -8, 0, 292}}};
sprite_pattern *kagepat[2] = {&kag00, &kag01};
static sprite_pattern ana00 = {1, {{-16, -16, 0, 293}}};
sprite_pattern *kage1pat[1] = {&ana00};
static Uint16 shooterposi_0[69] = {
    136, 5184, 240, 5240, 264, 5264, 320, 5264, 480, 5184, 504, 5120, 480, 5104,
    448, 5104, 384, 5120, 368, 5152, 360, 5184, 368, 5224, 424, 5728, 536, 5792,
    528, 5824, 504, 5840, 456, 5824, 424, 5760, 408, 5720, 416, 5696, 456, 5712,
    496, 5760, 512, 5824, 512, 5840, 528, 5840, 648, 5824, 704, 5760, 728, 5712,
    704, 5712, 672, 5760, 656, 5888, 656, 5928, 672, 5928, 736, 5888, 752};
static Uint16 shooterposi_1[35] = {
    68,  3848, 416, 3984, 416, 4040, 440, 4064, 496, 4064, 608, 4096,
    656, 4144, 672, 4200, 648, 4224, 592, 4200, 536, 4144, 512, 4080,
    544, 4064, 608, 4096, 656, 4144, 672, 4200, 648, 4400, 456};
static Uint16 shooterposi_2[35] = {
    68,  5680, 656, 5680, 792, 5688, 824, 5840, 976, 5888, 992, 5944,
    968, 5976, 912, 5944, 856, 5880, 832, 5824, 864, 5800, 912, 5840,
    976, 5888, 992, 5944, 968, 6072, 840, 6096, 800, 6096, 616};
Uint16 *shooterpositbl[3] = {shooterposi_0, shooterposi_1, shooterposi_2};
extern sprite_pattern *bariapat[];

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[4];
    Uint8 move_timer_high;
    Uint8 move_timer_low;
    Uint8 unused6[2];
    Uint8 entry_timer;
    Uint8 unused9[3];
    Sint16 target_x;
    Sint16 target_y;
    Uint8 move_index;
    Uint8 move_limit;
    Uint16 *move_table;
} shooter_work;
#pragma pack(pop)

_Static_assert(offsetof(shooter_work, move_timer_high) == 4,
               "shooter_work.move_timer_high offset");
_Static_assert(offsetof(shooter_work, entry_timer) == 8,
               "shooter_work.entry_timer offset");
_Static_assert(offsetof(shooter_work, target_x) == 12,
               "shooter_work.target_x offset");
_Static_assert(offsetof(shooter_work, target_y) == 14,
               "shooter_work.target_y offset");
_Static_assert(offsetof(shooter_work, move_index) == 16,
               "shooter_work.move_index offset");
_Static_assert(offsetof(shooter_work, move_limit) == 17,
               "shooter_work.move_limit offset");
_Static_assert(offsetof(shooter_work, move_table) == 18,
               "shooter_work.move_table offset");
_Static_assert(sizeof(shooter_work) <= sizeof(((sprite_status *)0)->actfree),
               "shooter_work fits in actfree");

static shooter_work *shooter_get_work(sprite_status *shootwk) {
    return (shooter_work *)shootwk->actfree;
}

void eda(sprite_status *edawk) {
    switch (edawk->r_no0) {
    case 0:
        eda_init(edawk);
        break;
    case 2:
        eda_move(edawk);
        break;
    }

    actionsub(edawk);
    frameout_s(edawk);
}

void eda_init(sprite_status *edawk) {
    edawk->r_no0 += 2;
    edawk->actflg |= 4;
    edawk->patbase = edapat;
    edawk->patno = edawk->userflag.b.h;
    edawk->sprhsize = 16;
    edawk->sprvsize = 16;

    eda_move(edawk);
}

void eda_move(sprite_status *edawk) {
    Uint8 action_no;
    action_no = edawk->actno;
}

void kage(sprite_status *kagewk) {
    switch (kagewk->r_no0) {
    case 0:
        kage_init(kagewk);
        break;
    case 2:
        kage_move(kagewk);
        break;
    }

    actionsub(kagewk);
    frameout_s(kagewk);
}

void kage_init(sprite_status *kagewk) {
    kagewk->r_no0 += 2;
    kagewk->actflg |= 4;
    kagewk->sprpri = 6;
    kagewk->patbase = kagepat;
    kagewk->patno = kagewk->userflag.b.h;
    kagewk->sprhsize = 16;
    kagewk->sprvsize = 16;

    kage_move(kagewk);
}

void kage_move(sprite_status *kagewk) {
    Uint8 action_no;
    action_no = kagewk->actno;
}

void ana(sprite_status *anawk) {
    switch (anawk->r_no0) {
    case 0:
        kage1_init(anawk);
        break;
    case 2:
        kage1_move0(anawk);
        break;
    case 4:
        kage1_move1(anawk);
        break;
    }

    frameout_s(anawk);
}

void kage1_init(sprite_status *anawk) {
    anawk->r_no0 += 2;
    anawk->actflg |= 4;

    anawk->patbase = kage1pat;
    anawk->sprhsize = 16;
    anawk->sprvsize = 16;
    anawk->sprpri = 6;
    if (kage1_chk(anawk) != 0) {
        anawk->r_no0 += 2;
        kage1_move1(anawk);
    } else {
        kage1_clr(anawk);
        kage1_move0(anawk);
    }
}

void kage1_move0(sprite_status *anawk) {
    Sint16 cal_position;

    if (player_work_get(&actwk[0])->status_flags == 0)
        return;
    cal_position = actwk[0].xposi.w.h - anawk->xposi.w.h + 32;
    if (cal_position < 0 || cal_position >= 64)
        return;
    cal_position = actwk[0].yposi.w.h - anawk->yposi.w.h + 32;
    if (cal_position < 0 || cal_position >= 64)
        return;
    kage1_chk(anawk);
    soundset(163);
    anawk->r_no0 += 2;
    kage1_move1(anawk);
}

void kage1_move1(sprite_status *anawk) { actionsub(anawk); }

Uint8 kage1_chk(sprite_status *anawk) {
    Sint16 flag_no;
    Uint8 zero_flag;

    flag_no = (Sint16)time_flag + (Sint16)anawk->cdsts * 3;
    zero_flag = flagwork[flag_no] & 64;
    flagwork[flag_no] |= 64;
    return zero_flag;
}

void kage1_clr(sprite_status *anawk) {
    Sint16 flag_no;

    flag_no = (Sint16)time_flag + (Sint16)anawk->cdsts * 3;
    flagwork[flag_no] &= 191;
}

void shooter(sprite_status *shootwk) {
    Sint16 flag_no, addition;

    if (!(time_flag & 128))
        goto label1;
    if ((flag_no = shootwk->cdsts) == 0)
        goto label1;
    flag_no *= 3;
    addition = (Sint16)(time_flag & 127) - (Sint16)time_item;
    if (addition < 0)
        addition = 0;
    else if (addition >= 3)
        addition = 2;
    flagwork[flag_no + addition] &= 127;
label1:
    if (actwk[0].mstno.b.h != 43) {
        if (actwk[0].r_no0 < 6) {
            switch (shootwk->r_no0) {
            case 0:
                shooterinit(shootwk);
                break;
            case 2:
                shootermove(shootwk);
                break;
            case 4:
                shootermove2(shootwk);
                break;
            case 6:
                shootermove3(shootwk);
                break;
            }
        }
    }
}

void shooterinit(sprite_status *shootwk) {
    Uint16 *movetbl;
    shooter_work *work = shooter_get_work(shootwk);

    shootwk->patbase = bariapat;
    shootwk->actflg = 4;
    shootwk->sprpri = 1;
    shootwk->sprhsize = 16;

    shootwk->r_no0 += 2;
    movetbl = shooterpositbl[shootwk->userflag.b.h & 127];
    work->move_index = *movetbl >> 8;
    work->move_limit = *movetbl & 255;
    work->move_table = movetbl + 1;
    work->target_x = movetbl[1];
    work->target_y = movetbl[2];
    shootermove(shootwk);
}

void shootermove(sprite_status *shootwk) {
    Sint16 cal_position;
    player_work *player = player_work_get(&actwk[0]);

    cal_position = actwk[0].xposi.w.h - shootwk->xposi.w.h + 16;
    if (cal_position >= 32 || cal_position < 0)
        return;
    cal_position = actwk[0].yposi.w.h - shootwk->yposi.w.h + 16;
    if (cal_position >= 32 || cal_position < 0)
        return;
    if (player->status_flags != 0)
        return;
    if (actwk[0].r_no0 == 4) {
        actwk[0].r_no0 -= 2;
        player->damage_invulnerability_timer = 120;
    }
    shootwk->r_no0 += 2;
    player->status_flags = 129;
    if (shootwk->userflag.b.l != 0)
        player->status_flags |= 64;
    actwk[0].mstno.b.h = 2;
    mspd_set(shootwk);
    actwk[0].xspeed.w = actwk[0].yspeed.w = 0;
    shootwk->cddat &= 223;
    actwk[0].cddat &= 223;
    actwk[0].cddat |= 2;
    player->jump_started = 0;
    actwk[0].xposi.w.h = shootwk->xposi.w.h;
    actwk[0].yposi.w.h = shootwk->yposi.w.h;
    shooter_get_work(shootwk)->entry_timer = 0;
    soundset(145);
}

void shootermove2(sprite_status *shootwk) {
    shooterspdset(shootwk);
    shootwk->r_no0 += 2;
    soundset(145);
}

void shootermove3(sprite_status *shootwk) {
    Uint8 move_counter;
    Uint16 *movetbl;
    shooter_work *work = shooter_get_work(shootwk);

    if ((char)--work->move_timer_high < 0) {
        actwk[0].xposi.w.h = work->target_x;
        actwk[0].yposi.w.h = work->target_y;
        move_counter = work->move_index + 4;
        if (move_counter < work->move_limit) {

            work->move_index = move_counter;
            movetbl = work->move_table;
            work->target_x = movetbl[move_counter / 2];
            work->target_y = movetbl[move_counter / 2 + 1];
            shooterspdset(shootwk);
            return;
        }
    } else {
        actwk[0].xposi.l += actwk[0].xspeed.w << 8;
        actwk[0].yposi.l += actwk[0].yspeed.w << 8;
        return;
    }

    actwk[0].yposi.w.h &= 2047;
    shootwk->r_no0 = 0;
    player_work_get(&actwk[0])->status_flags = 0;
}

void shooterspdset(sprite_status *shootwk) {
    Sint16 ms_work0, ms_work1, cal_position_x, cal_position_y;
    Sint32 cal_long_x, cal_long_y;
    shooter_work *work = shooter_get_work(shootwk);

    ms_work0 = ms_work1 = actwk[0].mspeed.w;
    cal_position_x = work->target_x;
    cal_position_x -= actwk[0].xposi.w.h;
    if (cal_position_x < 0) {
        cal_position_x = -cal_position_x;
        ms_work0 = -ms_work0;
    }
    cal_position_y = work->target_y;
    cal_position_y -= actwk[0].yposi.w.h;
    if (cal_position_y < 0) {
        cal_position_y = -cal_position_y;
        ms_work1 = -ms_work1;
    }
    if (cal_position_x <= cal_position_y) {

        cal_position_y = work->target_y;
        cal_position_y -= actwk[0].yposi.w.h;
        cal_long_y = cal_position_y << 16;
        cal_long_y /= ms_work1;

        cal_position_x = work->target_x;
        cal_position_x -= actwk[0].xposi.w.h;
        cal_long_x = cal_position_x << 16;
        if (cal_long_x != 0)
            cal_long_x /= cal_long_y;

        actwk[0].xspeed.w = cal_long_x;
        actwk[0].yspeed.w = ms_work1;
        if (cal_long_y < 0)
            cal_long_y = -cal_long_y;
        work->move_timer_high = cal_long_y >> 8;
        work->move_timer_low = cal_long_y & 255;
        return;
    }

    cal_position_x = work->target_x;
    cal_position_x -= actwk[0].xposi.w.h;
    cal_long_x = cal_position_x << 16;
    cal_long_x /= ms_work0;

    cal_position_y = work->target_y;
    cal_position_y -= actwk[0].yposi.w.h;
    cal_long_y = cal_position_y << 16;
    if (cal_long_y != 0)
        cal_long_y /= cal_long_x;

    actwk[0].yspeed.w = cal_long_y;
    actwk[0].xspeed.w = ms_work0;
    if (cal_long_x < 0)
        cal_long_x = -cal_long_x;
    work->move_timer_high = cal_long_x >> 8;
    work->move_timer_low = cal_long_x & 255;
}

void mspd_set(sprite_status *shootwk) {
    Sint16 mspdtbl[4] = {4096, 3072, 3072, 2048};
    Sint16 cal_mspeed;

    actwk[0].mspeed.w = 4096;
    if (shootwk->userflag.b.h < 0)
        return;
    cal_mspeed = mspdtbl[shootwk->userflag.b.h & 15];
    if (cal_mspeed > actwk[0].mspeed.w)
        actwk[0].mspeed.w = cal_mspeed;
}

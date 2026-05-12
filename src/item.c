#include "equ.h"
#include "item.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "loader2.h"
#include "playsub.h"
#include "ridechk.h"

static Uint8 itemchg0[11] = {1, 16, 0, 0, 8, 0, 0, 9, 0, 0, 255};
static Uint8 itemchg1[11] = {1, 16, 1, 1, 8, 1, 1, 9, 1, 1, 255};
static Uint8 itemchg2[11] = {1, 16, 2, 2, 8, 2, 2, 9, 2, 2, 255};
static Uint8 itemchg3[11] = {1, 16, 3, 3, 8, 3, 3, 9, 3, 3, 255};
static Uint8 itemchg4[11] = {1, 16, 4, 4, 8, 4, 4, 9, 4, 4, 255};
static Uint8 itemchg5[11] = {1, 16, 5, 5, 8, 5, 5, 9, 5, 5, 255};
static Uint8 itemchg6[11] = {1, 16, 6, 6, 8, 6, 6, 9, 6, 6, 255};
static Uint8 itemchg7[11] = {1, 16, 7, 7, 8, 7, 7, 9, 7, 7, 255};
static Uint8 itemchg8[10] = {1, 10, 14, 15, 14, 11, 14, 15, 14, 255};
static Uint8 itemchg9[10] = {1, 12, 14, 15, 14, 13, 14, 15, 14, 255};
Uint8 *itemchg[10] = {itemchg0, itemchg1, itemchg2, itemchg3, itemchg4,
                      itemchg5, itemchg6, itemchg7, itemchg8, itemchg9};
static sprite_pattern ite00 = {2, {{-8, -10, 0, 272}, {-16, -16, 0, 286}}};
static sprite_pattern ite01 = {2, {{-8, -10, 0, 273}, {-16, -16, 0, 286}}};
static sprite_pattern ite02 = {2, {{-8, -10, 0, 274}, {-16, -16, 0, 286}}};
static sprite_pattern ite03 = {2, {{-8, -10, 0, 275}, {-16, -16, 0, 286}}};
static sprite_pattern ite04 = {2, {{-8, -10, 0, 276}, {-16, -16, 0, 286}}};
static sprite_pattern ite05 = {2, {{-8, -10, 0, 277}, {-16, -16, 0, 286}}};
static sprite_pattern ite06 = {2, {{-8, -10, 0, 278}, {-16, -16, 0, 286}}};
static sprite_pattern ite07 = {2, {{-8, -10, 0, 279}, {-16, -16, 0, 286}}};
static sprite_pattern ite08 = {2, {{-8, -10, 0, 280}, {-16, -16, 0, 286}}};
static sprite_pattern ite09 = {2, {{-8, -10, 8, 280}, {-16, -16, 0, 286}}};
static sprite_pattern ite0a = {2, {{-16, -40, 0, 281}, {-8, -24, 0, 285}}};
static sprite_pattern ite0b = {2, {{-16, -40, 8, 281}, {-8, -24, 0, 285}}};
static sprite_pattern ite0c = {2, {{-16, -40, 0, 282}, {-8, -24, 0, 285}}};
static sprite_pattern ite0d = {2, {{-16, -40, 8, 282}, {-8, -24, 0, 285}}};
static sprite_pattern ite0e = {2, {{-8, -40, 8, 283}, {-8, -24, 0, 285}}};
static sprite_pattern ite0f = {2, {{-4, -40, 8, 284}, {-8, -24, 0, 285}}};
static sprite_pattern ite10 = {1, {{-16, -16, 0, 286}}};
static sprite_pattern ite11 = {1, {{-16, 0, 0, 287}}};
static sprite_pattern ite12 = {1, {{-16, -8, 0, 281}}};
static sprite_pattern ite13 = {1, {{-16, -8, 0, 282}}};
sprite_pattern *itempat[20] = {&ite00, &ite01, &ite02, &ite03, &ite04,
                               &ite05, &ite06, &ite07, &ite08, &ite09,
                               &ite0a, &ite0b, &ite0c, &ite0d, &ite0e,
                               &ite0f, &ite10, &ite11, &ite12, &ite13};
static sprite_pattern cor00 = {1, {{-8, -10, 0, 272}}};
static sprite_pattern cor01 = {1, {{-8, -10, 0, 273}}};
static sprite_pattern cor02 = {1, {{-8, -10, 0, 274}}};
static sprite_pattern cor03 = {1, {{-8, -10, 0, 275}}};
static sprite_pattern cor04 = {1, {{-8, -10, 0, 276}}};
static sprite_pattern cor05 = {1, {{-8, -10, 0, 277}}};
static sprite_pattern cor06 = {1, {{-8, -10, 0, 278}}};
static sprite_pattern cor07 = {1, {{-8, -10, 0, 279}}};
static sprite_pattern cor08 = {1, {{-8, -10, 0, 280}}};
static sprite_pattern cor09 = {1, {{-8, -10, 8, 280}}};
static sprite_pattern cor0a = {1, {{-16, -40, 0, 281}}};
static sprite_pattern cor0b = {1, {{-16, -40, 8, 281}}};
static sprite_pattern cor0c = {1, {{-16, -40, 0, 282}}};
static sprite_pattern cor0d = {1, {{-16, -40, 8, 282}}};
static sprite_pattern cor0e = {1, {{-8, -40, 0, 283}}};
static sprite_pattern cor0f = {1, {{-4, -40, 0, 284}}};
sprite_pattern *corepat[16] = {&cor00, &cor01, &cor02, &cor03, &cor04, &cor05,
                               &cor06, &cor07, &cor08, &cor09, &cor0a, &cor0b,
                               &cor0c, &cor0d, &cor0e, &cor0f};

void timedisp(sprite_status *timewk) {
    switch (timewk->r_no0) {
    case 0:
        td_init(timewk);
        break;
    case 2:
        td_move(timewk);
        break;
    }

    if (time_item == 0)
        return;
    if (backto_cnt < 90)
        actionsub(timewk);
    else if (!(gametimer.b.l & 2))
        actionsub(timewk);
}

void td_init(sprite_status *timewk) {
    timewk->r_no0 += 2;
    timewk->patbase = itempat;
    timewk->sproffset = 32768;
    timewk->xposi.w.h = 196;
    timewk->yposi.w.h = 338;
    td_move(timewk);
}

void td_move(sprite_status *timewk) {
    timewk->patno = 18;
    if (time_item >= 0)
        timewk->patno = 19;
}

void t_item(sprite_status *t_itemwk) {
    if (ta_flag != 0) {
        frameout(t_itemwk);
        return;
    }
    if (t_itemwk->userflag.b.h == 10) {
        timedisp(t_itemwk);
        return;
    }
    switch (t_itemwk->r_no0) {
    case 0:
        t_item_init(t_itemwk);
        break;
    case 2:
        t_item_move0(t_itemwk);
        break;
    case 4:
        t_item_move1(t_itemwk);
        break;
    case 6:
        t_item_move2(t_itemwk);
        break;
    }

    actionsub(t_itemwk);
    frameout_s(t_itemwk);
}

void t_item_init(sprite_status *t_itemwk) {
    Sint16 flag_no;

    t_itemwk->r_no0 += 2;
    t_itemwk->sprvsize = 32;
    t_itemwk->sprhs = 14;
    t_itemwk->patbase = itempat;

    t_itemwk->actflg = 4;
    t_itemwk->sprpri = 3;
    if (stageno.b.h == 6 && t_itemwk->userflag.b.l == 0)
        t_itemwk->sprpri = 0, t_itemwk->sproffset = 32768;
    t_itemwk->sprhsize = 15;
    t_itemwk->mstno.b.h = t_itemwk->userflag.b.h;

    flag_no = flagwkadr(t_itemwk);
    flagwork[flag_no] &= 127;

    t_itemwk->patno = 10;
    if (t_itemwk->userflag.b.h != 8)
        t_itemwk->patno += 2;

    if (flagwork[flag_no] & 1) {
        ++t_itemwk->patno;
        t_itemwk->r_no0 = 6;
    } else {
        t_itemwk->colino = 223;
        t_item_move0(t_itemwk);
    }
}

void t_item_move0(sprite_status *t_itemwk) {
    Sint16 flag_no;

    if (t_itemwk->colicnt == 0)
        return;
    t_itemwk->colicnt = 0;
    if (stageno.b.h == 6) {
        if (t_itemwk->userflag.b.l != 0) {

            if (prio_flag == 0)
                return;
        } else {
            if (prio_flag != 0)
                return;
        }
    }

    t_itemwk->actfree[0] = 0;
    t_itemwk->actfree[1] = 60;
    t_itemwk->r_no0 += 2;
    flag_no = flagwkadr(t_itemwk);
    flagwork[flag_no] |= 1;
    if (t_itemwk->userflag.b.h != 8) {
        time_item = 1;
        sub_sync(118);
    } else {
        time_item = -1;
        sub_sync(119);
    }
}

void t_item_move1(sprite_status *t_itemwk) {
    if (--t_itemwk->actfree[1] != 0) {
        patchg(t_itemwk, itemchg);
        return;
    }
    t_itemwk->r_no0 += 2;
    t_itemwk->patno = 11;
    if (t_itemwk->userflag.b.h != 8)
        t_itemwk->patno += 2;
}

void t_item_move2(sprite_status *t_itemwk) {
    Uint8 action_no;
    action_no = t_itemwk->actno;
}

Sint16 flagwkadr(sprite_status *t_itemwk) {
    Sint16 flag_no, t_flag, t_item;

    flag_no = t_itemwk->cdsts, flag_no *= 3;
    t_flag = time_flag;
    if (!(t_flag & 128)) {
        flag_no += t_flag;
        return flag_no;
    }
    t_flag &= -129;
    t_item = time_item;
    t_item = -t_item;
    t_flag += t_item;
    if (t_flag < 0)
        return flag_no;
    if (t_flag >= 3)
        t_flag = 2;
    flag_no += t_flag;
    return flag_no;
}

void ride_on_chk_i(sprite_status *itemwk) {
    if (stageno.b.h == 6) {
        if (prio_flag != 0) {

            if (itemwk->userflag.b.l == 0)
                return;
        } else {

            if (itemwk->userflag.b.l != 0)
                return;
        }
    }
    ride_on_chk(itemwk, &actwk[0]);
}

void item(sprite_status *itemwk) {
    if (itemwk->userflag.b.h == 0 && ta_flag != 0) {
        frameout_s(itemwk);
        return;
    }
    if (itemwk->userflag.b.h >= 8) {
        t_item(itemwk);
        return;
    }
    switch (itemwk->r_no0) {
    case 0:
        iteminit(itemwk);
        break;
    case 2:
        itemmove(itemwk);
        break;
    case 4:
        itemget(itemwk);
        break;
    case 6:
        itemmove2(itemwk);
        break;
    case 8:
        itemmove3(itemwk);
        break;
    }
}

void iteminit(sprite_status *itemwk) {
    Sint16 flag_no;

    itemwk->r_no0 += 2;
    itemwk->sprvsize = 14;
    itemwk->sprhs = 14;
    itemwk->patbase = itempat;

    itemwk->sprpri = 3;
    if (stageno.b.h == 6 && itemwk->userflag.b.l == 0)
        itemwk->sprpri = 0, itemwk->sproffset = 32768;
    itemwk->actflg = 4;
    itemwk->sprhsize = 15;

    flag_no = flagwkadr(itemwk);
    flagwork[flag_no] &= 127;
    if (flagwork[flag_no] & 1) {
        itemwk->r_no0 = 8;
        itemwk->patno = 17;
        return;
    }
    itemwk->colino = 70;
    itemwk->mstno.b.h = itemwk->userflag.b.h;
    if (itemwk->patno == 0)
        itemwk->patno = 16;

    itemmove(itemwk);
}

void itemmove(sprite_status *itemwk) {
    Sint16 col_ofs;

    if (itemwk->patbase != itempat) {
        error_item(itemwk);
        return;
    }
    if ((char)itemwk->actflg >= 0) {
        itemmove3(itemwk);
        return;
    }
    if (itemwk->r_no1 != 0) {
        speedset(itemwk);
        if ((col_ofs = emycol_d(itemwk)) < 0) {
            itemwk->yposi.w.h += col_ofs;
            itemwk->yspeed.w = 0;
            itemwk->r_no1 = 0;
        }
    } else if ((char)itemwk->actflg < 0)
        ride_on_chk_i(itemwk);

    itemmove2(itemwk);
}

void itemmove2(sprite_status *itemwk) {
    if (time_stop == 0)
        patchg(itemwk, itemchg);
    itemmove3(itemwk);
}

void itemmove3(sprite_status *itemwk) {
    actionsub(itemwk);
    frameout_s(itemwk);
}

void itemget(sprite_status *itemwk) {
    sprite_status *new_actwk;
    Sint16 flag_no;

    soundset(150);
    itemwk->r_no0 += 4;
    itemwk->colino = 0;
    if (actwkchk(&new_actwk) == 0) {
        new_actwk->actno = 26;
        new_actwk->xposi.w.h = itemwk->xposi.w.h;
        new_actwk->yposi.w.h = itemwk->yposi.w.h;
        new_actwk->mstno.b.h = itemwk->mstno.b.h;
        new_actwk->userflag.b.l = itemwk->userflag.b.l;
    }
    if (actwkchk(&new_actwk) == 0) {
        new_actwk->actno = 24;
        new_actwk->xposi.w.h = itemwk->xposi.w.h;
        new_actwk->yposi.w.h = itemwk->yposi.w.h;
        new_actwk->r_no1 = 1;
        new_actwk->userflag.b.h = 1;
        new_actwk->userflag.b.l = itemwk->userflag.b.l;
    }
    flag_no = flagwkadr(itemwk);
    flagwork[flag_no] |= 1;
    itemwk->patno = 17;
    actionsub(itemwk);
}

void powerup_init(sprite_status *powerwk) {
    powerwk->r_no0 = 0;
    powerwk->mstno.w = 0;
}

void item2(sprite_status *itemwk) {
    switch (itemwk->r_no0) {
    case 0:
        item2init(itemwk);
        break;
    case 2:
        item2move(itemwk);
        break;
    case 4:
        item2die(itemwk);
        break;
    }

    actionsub(itemwk);
}

void item2init(sprite_status *itemwk) {
    itemwk->r_no0 += 2;
    itemwk->sproffset = 32768;

    itemwk->actflg = 4;
    itemwk->sprpri = 3;
    itemwk->sprhsize = 8;
    itemwk->yspeed.w = -768;
    itemwk->patno = itemwk->mstno.b.h;
    itemwk->patbase = corepat;
    item2move(itemwk);
}

void item2move(sprite_status *itemwk) {
    if (itemwk->yspeed.w < 0) {
        speedset2(itemwk);
        itemwk->yspeed.w += 24;
        return;
    }
    itemwk->r_no0 += 2;
    itemwk->pattim = 29;
    switch (itemwk->mstno.b.h) {
    case 0:
        ++pl_suu, ++pl_suu_f;
        sub_sync(122);
        break;
    case 1:
        plring += 10;
        plring_f |= 1;
        if ((plring >= 100 && !(plring_f2 & 2)) ||
            (plring >= 200 && !(plring_f2 & 4))) {

            if (plring < 200)
                plring_f2 |= 2;
            else
                plring_f2 |= 4;
            ++pl_suu, ++pl_suu_f;
            sub_sync(122);
        } else {
            soundset(149);
        }
        break;
    case 2:
        plpower_b = 1;
        powerup_init(&actwk[6]);
        actwk[6].actno = 3;
        soundset(151);
        break;
    case 3:
        plpower_m = 1;
        ((Sint16 *)&actwk[0])[27] = 1320;
        powerup_init(&actwk[8]);
        actwk[8].actno = 3;
        actwk[8].mstno.b.h = 1;
        powerup_init(&actwk[9]);
        actwk[9].actno = 3;
        actwk[9].mstno.b.h = 2;
        powerup_init(&actwk[10]);
        actwk[10].actno = 3;
        actwk[10].mstno.b.h = 3, actwk[10].r_no0 = 0;
        powerup_init(&actwk[11]);
        actwk[11].actno = 3;
        actwk[11].mstno.b.h = 4, actwk[11].r_no0 = 0;
        if (time_flag == 0) {
            sub_sync(130);
        }
        sub_sync(109);
        break;
    case 4:
        plpower_s = 1;
        ((Sint16 *)&actwk[0])[28] = 1320;
        plmaxspdwk = 3072;
        pladdspdwk = 24;
        plretspdwk = 128;
        if (time_flag == 0) {
            sub_sync(130);
        }
        sub_sync(108);
        break;
    case 5:
        time_stop = 300;
        break;
    case 6:
        soundset(157);
        conbine_flag = 1;
        break;
    default:
        plpower_b = 1;
        powerup_init(&actwk[6]);
        actwk[6].actno = 3;
        plpower_m = 1;
        ((Sint16 *)&actwk[0])[27] = 1320;
        powerup_init(&actwk[8]);
        actwk[8].actno = 3;
        actwk[8].mstno.b.h = 1;
        powerup_init(&actwk[9]);
        actwk[9].actno = 3;
        actwk[9].mstno.b.h = 2;
        powerup_init(&actwk[10]);
        actwk[10].actno = 3;
        actwk[10].mstno.b.h = 3;
        powerup_init(&actwk[11]);
        actwk[11].actno = 3;
        actwk[11].mstno.b.h = 4;
        plpower_s = 1;
        ((Sint16 *)&actwk[0])[28] = 1320;
        plmaxspdwk = 3072;
        pladdspdwk = 24;
        plretspdwk = 128;
        break;
    }
}

void item2die(sprite_status *itemwk) {
    if (--itemwk->pattim == 255)
        frameout(itemwk);
}

void error_item(sprite_status *itemwk) {

    (Uint32)((Sint32)itemwk->patbase ^ (Sint32)corepat) < 1;

    itemwk->patbase = itempat;
}

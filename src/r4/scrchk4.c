#include "../equ.h"
#include "scrchk4.h"

void scrchk(void) {
  Sint16 wD1;
  int_union temp1, temp2;

  zone4chk();
  wD1 = 4;
  if (scralim_n_down != scralim_down)
  {
    if (scralim_n_down < scralim_down)
    {
      wD1 *= -1;
      if (scra_v_posit.w.h >= scralim_n_down)
      {
        scralim_down = scra_v_posit.w.h;
        scralim_down &= -2;
      }
      scralim_down += wD1;
      limmoveflag = 1;
      return;
    }

temp1.w.l = scra_v_posit.w.h + 8;
    temp2.w.l = scralim_down;
    if (temp1.l >= temp2.l)
    {
      if (actwk[0].cddat & 2)
        wD1 *= 4;
    }
    scralim_down = wD1;
    limmoveflag = 1;
  }
}

void zone4chk(void) {
  void(*tbl[3])(void) =
  {
    &zone41chk,
    &zone42chk,
    &zone43chk
  };

  tbl[stageno.b.l]();
}

void zone41chk(void) {
  scralim_n_down = 1296;
}

void zone42chk(void) {
  int_union temp;

  if (actwk[0].mstno.b.h != 43)
  {
    if (actwk[0].r_no0 < 6)
    {
      scralim_down = 2048;
      scralim_n_down = 2048;
      temp.l = 0;
      temp.w.h = scra_h_posit.w.h;
      if (temp.l < 512) return;
    }
  }

  scralim_down = 1808;
  scralim_n_down = 1808;
}

void zone43chk(void) {
  if (bossclr_scr(2808, 1216) == 0)
  {
    if (bossflag)
    {

      scralim_down = 1264;
      scralim_n_down = 1264;
    }
  }
}

Sint32 bossclr_scr(Sint16 XPosi, Sint16 DownLim) {
  Sint32 ret = 0;

  if (XPosi <= actwk[0].xposi.w.h)
    ret = bossclr_scrset(DownLim);
  return ret;
}

Sint32 bossclr_scrset(Sint16 DownLim) {
  Sint16 temp;

  scralim_n_down = DownLim;
  DownLim -= scralim_down;
  if (DownLim < 0)
    DownLim *= -1;

  if (DownLim < 4)
    scralim_down = scralim_n_down;

  temp = actwk[0].xposi.w.h - 160;
  if (temp >= scralim_left)
  {
    if (temp > scralim_right)
      temp = scralim_right;

    scralim_left = temp;
    scralim_n_left = temp;
  }
  return 1;
}

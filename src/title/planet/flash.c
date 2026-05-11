#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "flash.h"
#include "lplmain.h"

static void fadein1(void);
static void fadein2(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc);
static Sint32 fadeout_new(void);
static void fadeout1(void);
static void fadeout2(PALETTEENTRY* lpPeDest);
static Sint32 flashin_new(void);
static void flashin1(void);
static void flashin2(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc);
static Sint32 flashout_new(void);
static void flashout1(void);
static void flashout2(PALETTEENTRY* lpPeDest);

static Sint32 FadeCount;

void fadein(void) {
  startcolor = 0;
  colorcnt = 63;
  fadein0();
}

void fadein0(void) {
  PALETTEENTRY* lpPe;
  Sint32 i;

  lpPe = &lpcolorwk0[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    lpPe->peRed = 0;
    lpPe->peGreen = 0;
    lpPe->peBlue = 0;
    lpPe->peFlags = 1;
    ++lpPe;
  }
  FadeCount = 0;
  *lpFadeFlag = 1;

}

Sint32 fadein0_new(void) {

  int_flg = 10;

  fadein1();

if (++FadeCount > 21) {
    FadeCount = 0;
    return 1;
  }
  return 0;
}

static void fadein1(void) {
  PALETTEENTRY *lpPe, *lpPe2;
  Sint32 i;

  lpPe = &lpcolorwk0[startcolor / 2];
  lpPe2 = &lpcolorwk1[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    fadein2(lpPe, lpPe2);
    ++lpPe;
    ++lpPe2;
  }

}

static void fadein2(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc) {
  if (lpPeSrc->peBlue != lpPeDest->peBlue
      || lpPeSrc->peGreen != lpPeDest->peGreen
      || lpPeSrc->peRed != lpPeDest->peRed) {
    if (lpPeSrc->peBlue > lpPeDest->peBlue) {
      lpPeDest->peBlue += 32;
    }
    else if (lpPeSrc->peGreen > lpPeDest->peGreen) {
      lpPeDest->peGreen += 32;
    }
    else if (lpPeSrc->peRed > lpPeDest->peRed) {
      lpPeDest->peRed += 32;
    }
  }

}

void fadeout(void) {
  startcolor = 0;
  colorcnt = 63;

  FadeCount = 0;
  *lpFadeFlag = 2;

}

static Sint32 fadeout_new(void) {

  int_flg = 10;

  fadeout1();

if (++FadeCount > 21) {
    FadeCount = 0;
    return 1;
  }
  return 0;
}

static void fadeout1(void) {
  PALETTEENTRY* lpPe;
  Sint32 i;

  lpPe = &lpcolorwk0[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    fadeout2(lpPe);
    ++lpPe;
  }
  lpPe = &lpcolorwk3[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    fadeout2(lpPe);
    ++lpPe;
  }

}

static void fadeout2(PALETTEENTRY* lpPeDest) {
  if (lpPeDest->peRed) {
    lpPeDest->peRed -= 32;
  }
  else if (lpPeDest->peGreen) {
    lpPeDest->peGreen -= 32;
  }
  else if (lpPeDest->peBlue) {
    lpPeDest->peBlue -= 32;
  }

}

void flashin(void) {
  PALETTEENTRY* lpPe;
  Sint32 i;

  startcolor = 0;
  colorcnt = 63;

  lpPe = &lpcolorwk0[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    lpPe->peRed = 224;
    lpPe->peGreen = 224;
    lpPe->peBlue = 224;
    lpPe->peFlags = 1;
    ++lpPe;
  }
  FadeCount = 0;
  *lpFadeFlag = 3;

}

static Sint32 flashin_new(void) {

  int_flg = 10;

  flashin1();

if (++FadeCount > 21) {
    FadeCount = 0;
    return 1;
  }
  return 0;
}

static void flashin1(void) {
  PALETTEENTRY *lpPe, *lpPe2;
  Sint32 i;

  lpPe = &lpcolorwk0[startcolor / 2];
  lpPe2 = &lpcolorwk1[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    flashin2(lpPe, lpPe2);
    ++lpPe;
    ++lpPe2;
  }

}

static void flashin2(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc) {
  if (lpPeSrc->peBlue != lpPeDest->peBlue
      || lpPeSrc->peGreen != lpPeDest->peGreen
      || lpPeSrc->peRed != lpPeDest->peRed) {
    if (lpPeSrc->peBlue < lpPeDest->peBlue) {
      lpPeDest->peBlue -= 32;
    }
    else if (lpPeSrc->peGreen < lpPeDest->peGreen) {
      lpPeDest->peGreen -= 32;
    }
    else if (lpPeSrc->peRed < lpPeDest->peRed) {
      lpPeDest->peRed -= 32;
    }
  }

}

void flashout(void) {
  startcolor = 0;
  colorcnt = 63;

  FadeCount = 0;
  *lpFadeFlag = 4;

}

static Sint32 flashout_new(void) {

  int_flg = 10;

  flashout1();

if (++FadeCount > 21) {
    FadeCount = 0;
    return 1;
  }
  return 0;
}

static void flashout1(void) {
  PALETTEENTRY* lpPe;
  Sint32 i;

  lpPe = &lpcolorwk0[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    flashout2(lpPe);
    ++lpPe;
  }
  lpPe = &lpcolorwk3[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    flashout2(lpPe);
    ++lpPe;
  }

}

static void flashout2(PALETTEENTRY* lpPeDest) {
  if (lpPeDest->peRed != 224) {
    lpPeDest->peRed += 32;
  }
  else if (lpPeDest->peGreen != 224) {
    lpPeDest->peGreen += 32;
  }
  else if (lpPeDest->peBlue != 224) {
    lpPeDest->peBlue += 32;
  }

}

Sint32 FadeProc(void) {
  Sint32 ret;

  switch (*lpFadeFlag) {
    case 1:
      ret = fadein0_new();
      break;

    case 2:
      ret = fadeout_new();
      break;

    case 3:
      ret = flashin_new();
      break;

    case 4:
      ret = flashout_new();
      break;

    default:
      ret = 1;
  }

v_int();
  if (ret != 0) flash_flg = 0;
  return ret;
}

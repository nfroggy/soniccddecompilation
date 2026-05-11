#include "equ.h"
#include "io.h"

void SWdataSet(ushort_union sw1, ushort_union sw2) {
  swdata1.w = sw1.w;
  swdata2.w = sw2.w;

}

Sint32 random(void) {
  int_union lD0, lD1;
  Uint16 w;

  lD1.l = ranum;
  if (ranum == 0) lD1.l = 711800410;
  lD0.l = lD1.l;
  if (lD1.l < 0) {
    lD1.l *= 4;
    lD1.l |= 0x80000000;
  }
  else {
    lD1.l *= 4;
  }
  lD1.l += lD0.l;
  if (lD1.l < 0) {
    lD1.l *= 8;
    lD1.l |= 0x80000000;
  }
  else {
    lD1.l *= 8;
  }
  lD1.l += lD0.l;
  lD0.w.l += lD1.w.l;

  w = lD1.w.h;
  lD1.w.h = lD1.w.l;
  lD1.w.l = w;

  lD0.w.l += lD1.w.l;
  lD1.w.l = lD0.w.l;

  w = lD1.w.h;
  lD1.w.h = lD1.w.l;
  lD1.w.l = w;

  ranum = lD1.l;
  return lD0.l;
}

void scrinit(void) {
  vscroll.l = 0;
  hscroll.l = 0;
}

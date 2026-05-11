#include "../equ.h"
#include "col8d.h"

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
static void colorset00(Sint32 ColorNo, Sint32 WorkOffs);

Uint8 col8a0cnt[8] = { 38, 3, 4, 2, 4, 1, 4, 0 };
PALETTEENTRY col8a0col[3] = { { 224, 160,   0, 1 }, { 192,  64,   0, 1 }, { 160,  32,   0, 1 } };
Uint8 col8a1cnt[8] = { 39, 3, 4, 2, 4, 1, 4, 0 };
PALETTEENTRY col8a1col[3] = { { 192,  64,   0, 1 }, { 160,  32,   0, 1 }, { 224, 160,   0, 1 } };
Uint8 col8a2cnt[8] = { 40, 3, 4, 2, 4, 1, 4, 0 };
PALETTEENTRY col8a2col[3] = { { 160,  32,   0, 1 }, { 224, 160,   0, 1 }, { 192,  64,   0, 1 } };
Uint8 col8a3cnt[10] = { 49, 4, 3, 0, 3, 1, 3, 0, 84, 1 };
PALETTEENTRY col8a3col[2] = { { 224, 224,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8a4cnt[12] = { 50, 5, 12, 0, 3, 1, 3, 0, 3, 1, 72, 0 };
PALETTEENTRY col8a4col[2] = { {   0,   0,   0, 1 }, { 224, 224,   0, 1 } };
Uint8 col8a5cnt[12] = { 51, 5, 24, 0, 3, 1, 3, 0, 3, 1, 60, 0 };
PALETTEENTRY col8a5col[2] = { {   0,   0,   0, 1 }, { 224, 224,   0, 1 } };
Uint8 col8a6cnt[14] = { 49, 6, 19, 0, 19, 1, 19, 2, 19, 3, 19, 2, 19, 1 };
PALETTEENTRY col8a6col[4] = { { 224,   0,   0, 1 }, { 128,   0,   0, 1 }, {  64,   0,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8a7cnt[6] = { 50, 2, 2, 0, 2, 1 };
PALETTEENTRY col8a7col[2] = { {   0, 224,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8a8cnt[38] = { 51, 18, 2, 0, 2, 1, 2, 0, 2, 1, 2, 0, 55, 1, 2, 0, 2, 1, 2, 0, 10, 1, 2, 0, 10, 1, 2, 0, 2, 1, 2, 0, 2, 1, 2, 0, 25, 1 };
PALETTEENTRY col8a8col[2] = { { 224, 128,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8a9cnt[18] = { 49, 8, 11, 0, 11, 1, 11, 2, 11, 3, 11, 3, 11, 3, 11, 2, 11, 1 };
PALETTEENTRY col8a9col[4] = { { 224,   0,   0, 1 }, { 160,   0,   0, 1 }, {  96,   0,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8aacnt[18] = { 50, 8, 11, 0, 11, 1, 11, 2, 11, 3, 11, 4, 11, 3, 11, 2, 11, 1 };
PALETTEENTRY col8aacol[5] = { { 224, 128,   0, 1 }, { 192,  96,   0, 1 }, {  96,  64,   0, 1 }, {  32,  32,   0, 1 }, {   0,   0,   0, 1 } };
Uint8 col8abcnt[6] = { 51, 2, 2, 0, 2, 1 };
PALETTEENTRY col8abcol[2] = { {   0, 224,   0, 1 }, {   0,   0,   0, 1 } };
PALETTEENTRY advacolor[64] = {
  {   0,  32, 160, 1 }, {   0,   0,  96, 1 }, {   0,   0, 192, 1 }, {  64,  64, 224, 1 },
  {  96,  96, 224, 1 }, { 128, 128, 224, 1 }, { 224, 224, 224, 1 }, { 224, 160,   0, 1 },
  { 160,  96,   0, 1 }, {  96,  32,   0, 1 }, { 224, 224,   0, 1 }, { 160, 160, 224, 1 },
  { 192,   0,   0, 1 }, {  96,   0,   0, 1 }, {  32,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0, 192, 1 }, {  32,  32, 224, 1 }, {  64,  64, 224, 1 },
  {  96,  96, 224, 1 }, { 128, 128, 224, 1 }, { 224, 224, 224, 1 }, { 160, 160, 160, 1 },
  { 128, 128, 128, 1 }, {  96,  96,  96, 1 }, {  64,  64,  64, 1 }, { 128,  64,  32, 1 },
  { 224, 160, 128, 1 }, { 192, 128,  96, 1 }, {   0,   0,   0, 1 }, { 224,   0,   0, 1 },
  {   0,   0, 128, 1 }, {  32,   0,   0, 1 }, { 224, 224, 224, 1 }, {  96,  32,   0, 1 },
  { 128,  64,   0, 1 }, { 192,  96,   0, 1 }, { 224, 128,   0, 1 }, { 224, 192,   0, 1 },
  {  32,  64, 192, 1 }, {  96, 128, 224, 1 }, { 160, 192, 224, 1 }, { 192, 224, 224, 1 },
  {   0,  64,   0, 1 }, {   0,  96,   0, 1 }, {  64, 160,   0, 1 }, { 128, 224,   0, 1 },
  {  32, 128, 192, 1 }, {  32,   0, 160, 1 }, {  32,  64, 192, 1 }, {  96, 128, 224, 1 },
  { 160, 192, 224, 1 }, { 192, 224, 224, 1 }, { 224, 224, 224, 1 }, { 192, 160, 224, 1 },
  { 160, 128, 224, 1 }, { 128,  96, 224, 1 }, { 128, 224,   0, 1 }, {  64, 160,   0, 1 },
  {  32,   0,   0, 1 }, {  96,  32,   0, 1 }, { 192,  96,   0, 1 }, { 224, 192,   0, 1 }
};
PALETTEENTRY advacolor2[64] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  32,   0,   0, 1 }, {  32,   0,   0, 1 },
  {  64,  32,  32, 1 }, {  64,  32,  32, 1 }, {  96,  64,  64, 1 }, {  96,  64,  64, 1 },
  {  64,  32,  32, 1 }, {  64,  32,  32, 1 }, {  96,  64,  64, 1 }, { 128,  96,  96, 1 },
  {  64,  32,  32, 1 }, {  32,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  32,   0,   0, 1 }, {  64,  32,  32, 1 },
  {  64,  32,  32, 1 }, {  96,  64,  64, 1 }, { 128,  96,  96, 1 }, {  64,  32,  32, 1 },
  {  96,  64,  64, 1 }, {  64,  32,  32, 1 }, {  32,   0,   0, 1 }, {  64,  32,  32, 1 },
  {  96,  64,  64, 1 }, {  64,  32,  32, 1 }, {   0,   0,   0, 1 }, {  64,  32,  32, 1 },
  {   0,   0,   0, 1 }, { 224, 224,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, { 224, 224,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, { 192, 224, 224, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, { 192, 224, 224, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }
};
PALETTEENTRY gamecolor[16] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  32,  32, 128, 1 }, {  64,  64, 160, 1 },
  {  96,  96, 192, 1 }, { 128, 128, 224, 1 }, { 224, 224, 224, 1 }, { 160, 160, 160, 1 },
  { 128, 128, 128, 1 }, {  64,  64,  64, 1 }, { 224, 160, 128, 1 }, { 160,  96,  64, 1 },
  { 224,   0,   0, 1 }, { 128,   0,   0, 1 }, {  64,   0,   0, 1 }, { 224, 224,   0, 1 }
};
PALETTEENTRY zone8colora[48] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0, 192, 1 }, {   0,  64, 224, 1 },
  {   0, 128, 192, 1 }, { 160, 160, 160, 1 }, { 224, 224, 224, 1 }, { 128, 128, 128, 1 },
  {  64,  64,  64, 1 }, {   0,  96,  64, 1 }, {   0, 160,  96, 1 }, { 192,  96,   0, 1 },
  { 224, 224,   0, 1 }, { 128, 128,   0, 1 }, {  64,  64,   0, 1 }, { 224,   0,   0, 1 },
  {   0,  32,  32, 1 }, {   0,   0,   0, 1 }, {  96,  32,   0, 1 }, { 160,  64,  32, 1 },
  { 224, 128,  96, 1 }, { 224, 192, 128, 1 }, { 224, 160,   0, 1 }, { 192,  64,   0, 1 },
  { 160,  32,   0, 1 }, {  64,   0,  32, 1 }, { 128,   0,  64, 1 }, { 224,   0, 128, 1 },
  {  32,  32,   0, 1 }, {  96,  64,   0, 1 }, { 128,  96,  32, 1 }, { 192, 128,  64, 1 },
  {   0,  32,  32, 1 }, { 224, 224,   0, 1 }, { 160, 160,   0, 1 }, {  64,  64,   0, 1 },
  {  32,  32,   0, 1 }, {  64,  64,   0, 1 }, { 128,  96,   0, 1 }, {  32,   0,   0, 1 },
  {  64,   0,   0, 1 }, {  96,  32,   0, 1 }, { 160,  96,   0, 1 }, {   0,   0,   0, 1 },
  {   0,  32,  64, 1 }, {   0,  64,  96, 1 }, {   0,  96, 128, 1 }, {   0, 160, 192, 1 }
};
PALETTEENTRY zone82dcolor[48] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0, 192, 1 }, {   0,  64, 224, 1 },
  {   0, 128, 192, 1 }, { 160, 160, 160, 1 }, { 224, 224, 224, 1 }, { 128, 128, 128, 1 },
  {  64,  64,  64, 1 }, {   0,  96,  64, 1 }, {   0, 160,  96, 1 }, { 192,  96,   0, 1 },
  { 224, 224,   0, 1 }, { 128, 128,   0, 1 }, {  64,  64,   0, 1 }, { 224,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  96,  32,   0, 1 }, { 160,  64,  32, 1 },
  { 224, 128,  96, 1 }, { 224, 192, 128, 1 }, { 224, 160,   0, 1 }, { 192,  64,   0, 1 },
  { 160,  32,   0, 1 }, {  64,   0,  32, 1 }, { 128,   0,  64, 1 }, { 224,   0, 128, 1 },
  {  32,  32,   0, 1 }, {  96,  64,   0, 1 }, { 128,  96,  32, 1 }, { 192, 128,  64, 1 },
  {   0,   0,   0, 1 }, { 224,   0,   0, 1 }, { 224, 128,   0, 1 }, {   0, 224,   0, 1 },
  {   0,   0,  64, 1 }, {   0,   0, 160, 1 }, {   0,  96, 224, 1 }, {  96, 192,   0, 1 },
  {  32,   0,   0, 1 }, {  32,  32,   0, 1 }, {  64,  64,  32, 1 }, {  64,  96,  64, 1 },
  {  64,   0,   0, 1 }, { 128,  32,  64, 1 }, { 192,  96,  32, 1 }, { 224, 224,   0, 1 }
};
PALETTEENTRY zone83dcolor[48] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0, 192, 1 }, {   0,  64, 224, 1 },
  {   0, 128, 192, 1 }, { 160, 160, 160, 1 }, { 224, 224, 224, 1 }, { 128, 128, 128, 1 },
  {  64,  64,  64, 1 }, {   0,  96,  64, 1 }, {   0, 160,  96, 1 }, { 192,  96,   0, 1 },
  { 224, 224,   0, 1 }, { 128, 128,   0, 1 }, {  64,  64,   0, 1 }, { 224,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {  96,  32,   0, 1 }, { 160,  64,  32, 1 },
  { 224, 128,  96, 1 }, { 224, 192, 128, 1 }, { 224, 160,   0, 1 }, { 192,  64,   0, 1 },
  { 160,  32,   0, 1 }, {  64,   0,  32, 1 }, { 128,   0,  64, 1 }, { 224,   0, 128, 1 },
  {  32,  32,   0, 1 }, {  96,  64,   0, 1 }, { 128,  96,  32, 1 }, { 192, 128,  64, 1 },
  {   0,   0,   0, 1 }, { 224,   0,   0, 1 }, { 224, 128,   0, 1 }, {   0, 224,   0, 1 },
  {  64,  32,   0, 1 }, {  64,  64,   0, 1 }, {  96, 128,  32, 1 }, {  64, 192, 128, 1 },
  {  32,   0,   0, 1 }, {   0,  64, 128, 1 }, {   0,  32,  96, 1 }, {   0,  32,  64, 1 },
  {  32,   0,  32, 1 }, {  64,  32,  64, 1 }, {  96,  64, 128, 1 }, {  96, 128, 192, 1 }
};
PALETTEENTRY boss8col_0[16] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, { 224,   0,   0, 1 }
};
PALETTEENTRY boss8col_00[16] = {
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }
};
PALETTEENTRY boss8col_1[16] = {
  { 224, 224,  32, 1 }, {   0,   0,   0, 1 }, {  96, 128, 160, 1 }, {   0,  32,  64, 1 },
  {  32,  64,  96, 1 }, {  64,  96, 128, 1 }, { 224, 224, 224, 1 }, { 128, 160, 192, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  { 224, 224,   0, 1 }, { 128, 128,   0, 1 }, {   0,   0,   0, 1 }, { 224,   0,   0, 1 }
};
PALETTEENTRY emie8col[16] = {
  {  96,  96, 192, 1 }, {   0,   0,   0, 1 }, { 128,  32,  96, 1 }, { 160,  64, 128, 1 },
  { 224,  96, 224, 1 }, { 224, 128, 224, 1 }, { 224, 224, 224, 1 }, { 160, 160, 160, 1 },
  { 128, 128, 128, 1 }, {  64,  64,  64, 1 }, { 224, 160, 128, 1 }, { 192,  96,   0, 1 },
  {  32, 192,   0, 1 }, {   0, 128,   0, 1 }, {  96,   0, 128, 1 }, { 224,   0,   0, 1 }
};
palette_part colortbl[12] = {
  { advacolor,     0, 64 }, { advacolor,     0, 64 }, { advacolor2,    0, 64 }, { gamecolor,     0, 16 },
  { zone8colora,  16, 48 }, { zone83dcolor, 16, 48 }, { 0,             0,  0 }, { boss8col_1,   80, 16 },
  { boss8col_0,   16, 16 }, { boss8col_00,  48, 16 }, { zone82dcolor, 16, 48 }, { emie8col,     16, 16 }
};
PALETTEENTRY boss8_col1[40] = {
  {   0,   0,   0, 1 }, { 224,  96,   0, 1 }, {   0,   0, 224, 1 }, { 224,   0, 224, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  { 192,  96,   0, 1 }, {  64,   0,  96, 1 }, {   0, 160, 224, 1 }, { 128, 224,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  { 224, 224,   0, 1 }, {   0, 224,   0, 1 }, { 224,   0, 224, 1 }, {   0,   0,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  { 224,   0,   0, 1 }, {   0,   0,   0, 1 }, { 224, 224,   0, 1 }, {   0,  96, 224, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 },
  {   0,   0, 224, 1 }, { 224, 224,   0, 1 }, {   0, 224, 224, 1 }, { 224,  96,   0, 1 },
  {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }, {   0,   0,   0, 1 }
};
static Sint32 FadeCount;

void clchgctr(void) {
  colchg8d();
}

void colchg8d(void) {
  clchg_sub0(&clchgtim[0], &clchgcnt[0], col8a0cnt, col8a0col);
  clchg_sub0(&clchgtim[1], &clchgcnt[1], col8a1cnt, col8a1col);
  clchg_sub0(&clchgtim[2], &clchgcnt[2], col8a2cnt, col8a2col);
  if (stageno.b.l == 0) {

    clchg_sub0(&clchgtim[3], &clchgcnt[3], col8a3cnt, col8a3col);
    clchg_sub0(&clchgtim[4], &clchgcnt[4], col8a4cnt, col8a4col);
    clchg_sub0(&clchgtim[5], &clchgcnt[5], col8a5cnt, col8a5col);
    return;
  }

  if (stageno.b.h + 1 == 1) {

    clchg_sub0(&clchgtim[3], &clchgcnt[3], col8a6cnt, col8a6col);
    clchg_sub0(&clchgtim[4], &clchgcnt[4], col8a7cnt, col8a7col);
    clchg_sub0(&clchgtim[5], &clchgcnt[5], col8a8cnt, col8a8col);
    return;
  }

  clchg_sub0(&clchgtim[3], &clchgcnt[3], col8a9cnt, col8a9col);
  clchg_sub0(&clchgtim[4], &clchgcnt[4], col8aacnt, col8aacol);
  clchg_sub0(&clchgtim[5], &clchgcnt[5], col8abcnt, col8abcol);

}

void colchg_boss8(Uint8* a3, Uint8* a4) {
  PALETTEENTRY* lpPe;
  char d0;

  if (bossstart == 0) return;
  if (++*a4 >= 4) {
    *a4 = 0;
    d0 = *a3 + 4;
    if (d0 >= 40) d0 = 0;

    *a3 = d0;
    lpPe = &lpcolorwk[24];
    *lpPe++ = boss8_col1[d0];
    *lpPe++ = boss8_col1[d0 + 1];
    *lpPe++ = boss8_col1[d0 + 2];
    *lpPe++ = boss8_col1[d0 + 3];
  }
}

void clchg_sub0(Uint8* pChgTime, Uint8* pChgCnt, Uint8* pCntTbl, PALETTEENTRY* pColTbl) {
  PALETTEENTRY* lpPe;

  --*pChgTime;
  if ((char)*pChgTime < 0) {

    lpPe = &lpcolorwk[*pCntTbl++];
    ++*pChgCnt;
    if (*pChgCnt >= *pCntTbl++) {

      *pChgCnt = 0;
    }

    pCntTbl = &pCntTbl[*pChgCnt * 2];
    *pChgTime = pCntTbl[0];
    *lpPe = pColTbl[pCntTbl[1]];
  }

}

void fadein0(void) {
  PALETTEENTRY* lpPe;
  Sint32 i;

  lpPe = &lpcolorwk[startcolor / 2];

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

  int_flg = 18;

  fadein1();

if (++FadeCount > 21) {
    FadeCount = 0;
    return 1;
  }
  return 0;
}

void fadein1(void) {
  PALETTEENTRY *lpPe, *lpPe2;
  Sint32 i;

  lpPe = &lpcolorwk[startcolor / 2];
  lpPe2 = &lpcolorwk2[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    fadein2(lpPe, lpPe2);
    ++lpPe;
    ++lpPe2;
  }
  if (stageno.b.h == 1) return;

  lpPe = &lpcolorwk3[startcolor / 2];
  lpPe2 = &lpcolorwk4[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    fadein2(lpPe, lpPe2);
    ++lpPe;
    ++lpPe2;
  }

}

void fadein2(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc) {
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

void fadein_boss8(void) {
  PALETTEENTRY *lpPe, *lpPe2;

  int_flg = 18;
  lpPe = &lpcolorwk[16];
  lpPe2 = &lpcolorwk2[16];
  fadein1_boss8(lpPe, lpPe2);

  lpPe = &lpcolorwk[48];
  lpPe2 = &lpcolorwk2[48];
  fadein1_boss8(lpPe, lpPe2);
}

void fadein1_boss8(PALETTEENTRY* lpPeDest, PALETTEENTRY* lpPeSrc) {
  Sint16 i;

  for (i = 0; i < 16; ++i) {
    fadein2(lpPeDest, lpPeSrc);
    ++lpPeDest;
    ++lpPeSrc;
  }
}

void fadeout(void) {
  startcolor = 0;
  colorcnt = 63;

  FadeCount = 0;
  *lpFadeFlag = 2;

}

static Sint32 fadeout_new(void) {

  int_flg = 18;

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

  lpPe = &lpcolorwk[startcolor / 2];

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

  lpPe = &lpcolorwk[startcolor / 2];

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

  int_flg = 18;

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

  lpPe = &lpcolorwk[startcolor / 2];
  lpPe2 = &lpcolorwk2[startcolor / 2];

  for (i = 0; colorcnt >= i; ++i) {
    flashin2(lpPe, lpPe2);
    ++lpPe;
    ++lpPe2;
  }
  if (stageno.b.h == 1) return;

  lpPe = &lpcolorwk3[startcolor / 2];
  lpPe2 = &lpcolorwk4[startcolor / 2];

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

  int_flg = 18;

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

  lpPe = &lpcolorwk[startcolor / 2];

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

void colorset(Sint32 ColorNo) {
  colorset00(ColorNo, 1);
}

void colorset2(Sint32 ColorNo) {
  colorset00(ColorNo, 0);
}

void colorset3(Sint32 ColorNo) {
  colorset00(ColorNo, 0);
}

static void colorset00(Sint32 ColorNo, Sint32 WorkOffs) {
  PALETTEENTRY* lppe;
  PALETTEENTRY *lpPeSrc, *lpPeDest;
  Sint32 i, n;

  switch (WorkOffs + colortbl[ColorNo].offset / 64) {
    case 0:
      lppe = lpcolorwk; break;
    case 1:
      lppe = lpcolorwk2; break;
    case 2:
      lppe = lpcolorwk3; break;
    default:
      lppe = lpcolorwk4; break;
  }

  lpPeSrc = colortbl[ColorNo].PeSrc;
  lpPeDest = &lppe[colortbl[ColorNo].offset % 64];
  n = colortbl[ColorNo].cnt;
  for (i = 0; i < n; ++i) {
    *lpPeDest++ = *lpPeSrc++;
  }

}

Sint32 FadeProc(void) {
  switch (*lpFadeFlag) {
    case 1:
      return fadein0_new();
    case 2:
      return fadeout_new();
    case 3:
      return flashin_new();
    case 4:
      return flashout_new();
  }
  return 1;

}

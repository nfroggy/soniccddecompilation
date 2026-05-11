#include "../types.h"

#if defined(R82)
  #define TITLE_SPRITE_BASE 358
#elif defined(R83)
  #define TITLE_SPRITE_BASE 351
#else
  #define TITLE_SPRITE_BASE 349
#endif

Uint16 title_tbl[32] = {
   304,  552,  360,  346,  256,  568,  376,  602,
   256,  576,  384,  602,  256,  584,  392,  602,
   288,  564,  372,  858,  320,  584,  392, 1114,
   256,  464,  272, 1882,  256,  464,  272, 2138
};
static sprite_pattern titlepat0 = {
  1,
  { { -16, -112, 0, TITLE_SPRITE_BASE } }
};
static sprite_pattern titlepat1 = {
  1,
  { { -24, -8, 0, TITLE_SPRITE_BASE + 1 } }
};
static sprite_pattern titlepat2 = {
  1,
  { { -4, -24, 0, TITLE_SPRITE_BASE + 2 } }
};
static sprite_pattern titlepat3 = {
  1,
  { { -80, -8, 0, TITLE_SPRITE_BASE + 3 } }
};
static sprite_pattern titlepat4 = {
  1,
  { { -24, -24, 0, TITLE_SPRITE_BASE + 4 } }
};
static sprite_pattern titlepat5 = {
  1,
  { { -24, -24, 0, TITLE_SPRITE_BASE + 5 } }
};
static sprite_pattern titlepat6 = {
  1,
  { { -24, -24, 0, TITLE_SPRITE_BASE + 6 } }
};
static sprite_pattern titlepat7 = {
  1,
  { { 0, -56, 0, TITLE_SPRITE_BASE + 7 } }
};
static sprite_pattern titlepat8 = {
  1,
  { { 0, 0, 0, TITLE_SPRITE_BASE + 8 } }
};
sprite_pattern* title_pat[9] = {
  &titlepat0,
  &titlepat1,
  &titlepat2,
  &titlepat3,
  &titlepat4,
  &titlepat5,
  &titlepat6,
  &titlepat7,
  &titlepat8
};

#include "../../types.h"
typedef struct ld_pack_header {
  Sint32 type;
  Sint32 size;
  Sint32 count;
  Sint32 offset;
}
ld_pack_header;

typedef struct ld_sprite_inf {
  Sint16 xoff;
  Sint16 yoff;
  Sint16 wx;
  Sint16 wy;
  Sint16 palette;
  Sint16 reserved;
}
ld_sprite_inf;

typedef struct ld_sprite_header {
  ld_pack_header header;
  ld_sprite_inf spr[1];
}
ld_sprite_header;

typedef struct ld_bitmap_inf {
  Sint16 wx;
  Sint16 wy;
}
ld_bitmap_inf;

typedef struct ld_scroll_header {
  ld_pack_header header;
  Sint16 plt[4];
  ld_bitmap_inf bmp[1];
}
ld_scroll_header;

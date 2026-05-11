#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef int8_t Sint8;
typedef uint8_t Uint8;
typedef int16_t Sint16;
typedef uint16_t Uint16;
typedef int32_t Sint32;
typedef uint32_t Uint32;

typedef struct {
  Sint8 l;
  Sint8 h;
}
short_chars;

typedef union {
  Sint16 w;
  short_chars b;
}
short_union;

typedef struct {
  Sint16 l;
  Sint16 h;
}
int_shorts;

typedef struct {
  Sint8 b4;
  Sint8 b3;
  Sint8 b2;
  Sint8 b1;
}
int_chars;

typedef union {
  Sint32 l;
  int_shorts w;
  int_chars b;
}
int_union;

typedef struct {
  Uint8 l;
  Uint8 h;
}
ushort_uchars;

typedef union {
  Uint16 w;
  ushort_uchars b;
}
ushort_union;

typedef struct {
  Uint16 l;
  Uint16 h;
}
uint_ushorts;

typedef struct {
  Uint8 b4;
  Uint8 b3;
  Uint8 b2;
  Uint8 b1;
}
uint_uchars;

typedef union {
  Uint32 l;
  uint_ushorts w;
  uint_uchars b;
}
uint_union;

typedef struct {
  Sint8 xoff;
  Sint8 yoff;
  Uint8 etc;
  Uint16 index;
}
sprite_data;

typedef struct {
  Uint8 cnt;
  sprite_data spra[20];
}
sprite_pattern;

typedef struct {
  Sint16 plring;
  Sint32 plscore;
  short_union stageno;
  Uint32 TimeWarp;
  void* pSprBmp;
  Uint32 GamePass;
  Uint8 play_start;
  Uint8 pl_suu;
  Uint8 generate_flag;
  Uint8 time_flag;
  Uint8 special_flag;
  Sint32 pltime_sb;
  Sint16 plxposi_sb;
  Sint16 plyposi_sb;
  Sint16 scralim_down_sb;
  Sint16 scra_h_posit_sb;
  Sint16 scra_v_posit_sb;
  Sint16 scrb_h_posit_sb;
  Sint16 scrb_v_posit_sb;
  Sint16 scrc_h_posit_sb;
  Sint16 scrc_v_posit_sb;
  Sint16 scrz_h_posit_sb;
  Sint16 scrz_v_posit_sb;
  Sint16 waterposi_m_sb;
  Sint16 plmspd_sb;
  Sint16 plxspd_sb;
  Sint16 plyspd_sb;
  Uint8 plflag_sb;
  Uint8 cddat_sb;
  Uint8 water_flag_sb;
  Uint8 waterflag_sb;
  Uint8 chibi_sb;
  Uint8 prio_sb;
  Sint16 plring_s;
  Uint8 plring_f2_s;
  Uint8 plflag;
  short_union demoflag;
  short_union hintposi;
  Uint8 waterflag;
  Uint16* ptv_adr;
  Uint16* sm_adr0;
  Sint32 SPEMode;
  Uint8 stagenm;
  Uint8 clrspflg_save;
  Uint8 ta_flag;
  Uint8 gf_flag;
  Uint8 projector_flag;
  Uint8 flagworkcnt;
  Uint8 flagworkcnt2;
  Uint8 flagwork[766];
  Uint32 ta_time;
  Uint8 ta_Menu1;
  Uint8 ta_Round;
  Uint8 ta_Zone;
  Uint8 EndingMesType;
  Sint32 extrascore;
  Uint8 emie3end;
  Uint8 SpecialTime;
  Uint8 bRestart;
  Uint8 User;
  Uint16 wReserved1;
  Uint16 wReserved2;
}
game_info;

typedef struct {
  Uint8 actno;
  Uint8 actflg;
  Uint16 sproffset;
  sprite_pattern** patbase;
  int_union xposi;
  int_union yposi;
  short_union xspeed;
  short_union yspeed;
  short_union mspeed;
  Uint8 sprhsize;
  Uint8 sprvsize;
  Uint8 sprhs;
  Uint8 sprpri;
  Uint8 patno;
  short_union mstno;
  Uint8 patcnt;
  Uint8 pattim;
  Uint8 pattimm;
  Uint8 colino;
  Uint8 colicnt;
  Uint8 cddat;
  Uint8 cdsts;
  Uint8 r_no0;
  Uint8 r_no1;
  short_union direc;
  short_union userflag;
  Uint8 dummy[2];
  Uint8 actfree[22];
}
sprite_status;

typedef struct {
  Sint32 cnt;
  sprite_status* pActwk[63];
}
sprite_statuses;

typedef struct {
  Uint16 x;
  Uint16 y;
  Uint8 a;
  Uint8 b;
  Uint8 c;
  Uint8 d;
}
aset_info;

typedef struct {
  Uint16 x;
  Uint16 y;
  Uint8 a;
  Uint8 b;
}
asetz_info;

typedef struct {
  Uint8 xs;
  Uint8 ys;
  Uint8 ofs;
}
bmp_info;

typedef struct brankodata {
  Sint16 angle;
  Sint16 accel;
  Sint16 area1;
  Sint16 area2;
}
brankodata;

typedef struct dlink_export {
  void(*game_init)(void);
  void(*game)(void);
  void(*dll_meminit)(char***, void**);
  void(*dll_memfree)(void);
  void(*SWdataSet)(Sint16, Sint16);
  Sint32(*Get_vscroll)(void);
  Sint32(*Get_scra_h_posiw)(void);
  Sint32(*Get_scrb_h_posiw)(void);
  Sint32(*FadeProc)(void);
  void(*SetDebugFlag)(Uint32);
  void(*GetRoundStr)(Uint16, Uint8, char*);
  void(*Special_block_chg)(Uint16*, Uint16*, Uint16*, Uint16*);
}
dlink_export;

typedef struct {
  Uint8 actno;
  Uint8 sprpri;
  sprite_pattern** patbase;
  Uint16 offset;
  Uint8 userflag_h;
  Uint8 rludrv;
  Uint8 userflag_l;
  Uint8 patno;
}
edit_data_entry;

#ifndef EDIT_DATA_ENTRY_COUNT
#define EDIT_DATA_ENTRY_COUNT 1
#endif

typedef struct {
  Sint8 num;
  Sint8 dummy;
  edit_data_entry dat[EDIT_DATA_ENTRY_COUNT];
}
edit_data __attribute__((aligned(8)));

typedef struct {
  Uint8* CGdata;
  Uint8* Blkdata;
  Uint8* Mapdata;
  Uint8 iniCGno1;
  Uint8 stCGno1;
  Uint8 iniCGno2;
  Uint8 stCGno2;
  Uint8 colorno1;
  Uint8 colorno2;
  Uint8 soundNo;
}
map_init_data;

typedef struct tagPALETTEENTRY {
  Uint8 peRed;
  Uint8 peGreen;
  Uint8 peBlue;
  Uint8 peFlags;
}
PALETTEENTRY;

typedef struct {
  PALETTEENTRY* PeSrc;
  Sint16 offset;
  Sint16 cnt;
}
palette_part;

typedef struct tagPOINT {
  Sint32 x;
  Sint32 y;
}
POINT;

typedef struct {
  Sint8 tim;
  Sint8 no;
}
tile_change_data;

typedef struct {
  Sint8 chgcnt;
  Sint8 dmy;
  tile_change_data dattbl[25];
  Sint32* TileNoTbl[8];
  Sint32 TileStart;
}
tile_changes;

/* SPECIAL STAGE */

typedef struct {
  Uint8 cnt;
  Sint8 timer;
  sprite_pattern* spr[64];
}
sprite_patterns_sp;

typedef struct {
  Uint8 actno;
  Uint8 cgno;
  Uint8 actflg;
  Uint8 exeno;
  Uint16 sprcolor;
  sprite_patterns_sp** pattbl;
  Uint8 pattblno;
  Uint8 patno;
  Sint8 pctime;
  Sint8 pctime_b;
  Uint8 scno_ce;
  Uint8 scno_ul;
  Uint8 scno_ur;
  Uint8 scno_dr;
  Uint8 scno_dl;
  Uint8 scno_sdir;
  Sint16 spdcnt;
  Sint16 spdcnt_shock;
  Sint16 spdcnt_max;
  int_union x_posi;
  int_union y_posi;
  int_union z_posi;
  int_union sx_posi;
  int_union sy_posi;
  int_union x_speed;
  int_union y_speed;
  int_union z_speed;
  int_union sx_speed;
  int_union sy_speed;
  Uint8 colliflg;
  Uint8 dummy[3];
  Uint8 actfree[48];
}
sprite_status_sp;

typedef struct {
  Sint16 base;
  Sint16 data[4];
}
damage_info;

typedef struct {
  Sint16 base;
  Sint16 data[3];
}
hane_info;

typedef struct {
  Sint16 x;
  Sint16 y;
  Sint16 z;
  Sint16 ax;
  Sint16 sx;
  Sint16 cx;
  Sint16 az;
  Sint16 sz;
  Sint16 cz;
  Sint16 msz;
  Sint16 mcz;
  Sint16 h;
  Sint16 i;
  Sint32 hsxsz;
  Sint32 hsxcz;
  Sint32 hcxcz;
  Sint32 hcxsz;
  Sint32 hcx;
  Sint32 hzsx;
  int_union hsz;
  int_union hcz;
  int_union zcx;
  int_union hisxsz;
  int_union hisxcz;
  int_union cxsz;
  int_union cxcz;
  int_union sxsz;
  int_union sxcz;
}
zoom_info;

typedef struct {
  Sint8 no;
  Sint16* mv_tbl[6];
}
mvtbl;

typedef struct {
  Uint16* tbl;
  Uint16 vadr;
  Uint16 xsize;
  Uint16 ysize;
}
bwrt_info;

#endif

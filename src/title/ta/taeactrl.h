#include "ta_types.h"

void EAError(Sint32 ret, Sint32 line, char *str);
void srfDraw(void);
Sint32 ld_load_grid_module(hmx_environment *buffer, char *file,
                           hmx_environment *env, hmx_bitmap **bitmaps,
                           Sint32 size);
Sint32 TAGridBmpCreate(void);
Sint32 TAGridCreate(void);
Sint32 GridInitSub(char *path, map_info *pInfo);
Sint32 TAGridInit(void);
Sint32 BkgBmpCreate_TA(void);
Sint32 BkgBmpDelete_TA(void);
Sint32 BkgCreate_TA(void);
Sint32 BkgDelete_TA(void);
Sint32 LoadPicBmp(void);
Uint32 CreatePic(void);
void SetPic(Sint32 mode, Sint32 round, Sint32 zurasi);
Uint32 TA_EACreate(void);
void DeleteEA(void);
void DrawTA(void);
Uint16 GetLFont(char c);
Uint16 GetSFont(char c);
void TimeToAsc8(Uint32 Time, char *p);
void TimeToAsc9(Uint32 Time, char *p);
void PrintTotalTime(Sint32 mode, Uint32 b);
void PrintRoundName(Sint32 mode, Sint32 round, Uint32 b, Sint32 grid);
void PrintZoneNo(Sint32 mode, Sint32 zone);
void PrintZoneSelect(Sint32 mode, Sint32 round, Sint32 zone);
void PrintZoneRecord(Sint32 mode, Sint32 round, Sint32 zone, Sint32 rank);
void PrintZoneRecordAll(Sint32 mode, Sint32 round, Sint32 zone);
void PrintMenu2(Sint32 mode, Sint32 cur, Uint32 b);
void PrintRank(Sint32 mode, Sint32 rank, Uint32 b);
void PrintRankName(Sint32 mode, Sint32 ranky, Sint32 rankx, char c, Uint32 b);
void Tenmetu(void);
Sint32 TAMove(void);

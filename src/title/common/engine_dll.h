#ifndef ENGINE_DLL_H
#define ENGINE_DLL_H

#include "score_data_types.h"

typedef struct {
  Uint32 hWnd;
  Uint32 hSurf;
  PALETTEENTRY* lpColorwk;
  Uint32* lpbFullScreen;
  Uint32 bFirstTitle;
  Uint32* lpbVisualmode;
  score_data* lpCrntScorData;
  Uint32 selectIndx;
  Uint16* lpSelectedStage;
  void* ReadScoreData;
  void* WriteScoreData;
  void* OpenScoreData;
  void* CloseScoreData;
  void* CreateScoreData;
  void* ReadScoreIndx;
  void* WriteScoreIndx;
  void* CDPlay;
  void* CDPause;
  void* WaveRequest;
  void* EASetBitmapBits;
  void* EASetBitmapTransparency;
  void* EACreateBitmap;
  void* EADeleteBitmap;
  void* EACreateSequencedSprite;
  void* EASetDestination;
  void* EASetDrawOrder;
  void* EASetPosition;
  void* EADeleteSprite;
  void* EASetSequencedSpriteData;
  void* EACreateFixedGrid;
  void* EACreateTile;
  void* EASetTileData;
  void* EASetFixedGridData;
  void* EASetView;
  void* EASetEffect;
  void* EADeleteFixedGrid;
  void* EADraw;
  void* EAClearEffect;
  void* EADeleteTile;
  void* EASetDestRect;
  Uint16* lpUserKey;
}
engine_dll;

#endif

#include "../../types.h"
typedef struct {
  Sint16 index;
  Uint8 etc;
  Sint16 hoffs;
  Sint16 hrevoffs;
  Sint16 voffs;
  Sint16 vrevoffs;
}
sprite_pattern_mapping_title;

typedef struct {
  Sint16 mapnum;
  sprite_pattern_mapping_title aMapDat[2];
}
sprite_pattern_mappings_title;

typedef struct {
  Sint16 timer;
  sprite_pattern_mappings_title* paMapTbl;
}
sprite_pattern_title;

typedef struct {
  Sint16 patnum;
  sprite_pattern_title aPatDat[10];
}
sprite_patterns_title;

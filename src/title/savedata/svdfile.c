#include "../../types.h"
#include "../common/score_data_types.h"
#include "svdfile.h"
#include "../../impfuncs.h"

extern score_data saveData[6];
extern Sint32 nSelectFileNumber;
extern score_data* lpCrntScorData;
extern Uint32(*CloseScore)(Uint32);

void ReadDataFile(void) {
  Sint32 n;
  Sint32 indx;
  Uint32 hFile;

  hFile = OpenScore(0);

indx = ReadIndx(hFile);
  if (indx < 0 || indx > 5)
    return;
  nSelectFileNumber = indx;
  CloseScore(hFile);
  for (n = 0; n < 6; ++n)
  {
    ReadScore(n, (char*)&saveData[n], 0);
  }

}

void WriteDataFile(void) {
  Sint32 n;
  Uint32 hFile;

  hFile = OpenScore(1);

WriteIndx(nSelectFileNumber, hFile);
  CloseScore(hFile);
  for (n = 0; n < 6; ++n)
  {
    WriteScore(n, (char*)&saveData[n], 0);
  }

  sMemCpy(lpCrntScorData, &saveData[nSelectFileNumber], sizeof(saveData[nSelectFileNumber]));
}

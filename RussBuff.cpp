//---------------------------------------------------------------------------

#pragma hdrstop

#include "RussBuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TRusClipboard.Close();
{
  SetCodePage(0x0419);
  inherited;
};

void TRusClipboard.Open();
{
  inherited;
  SetCodePage(0x0419);
};

void TRusClipboard.SetCodePage(const long long CodePage);
{
  Pointer *DataPtr;
  THandle *Data;
  Data = GlobalAlloc(GMEM_MOVEABLE + GMEM_DDESHARE, 4);
  try
  {
    DataPtr = GlobalLock(Data);
    try
    {
      Move(CodePage, DataPtr^, 4);
      SetClipboardData(CF_LOCALE, Data);
    }
    __finally
    {
      GlobalUnlock(Data);
    };
  }
  catch(...)
  {
    GlobalFree(Data);
  };
};

  TClipboard *FClipboard;
  TClipboard *OldClipboard;
  FClipboard = TRusClipboard->Create();
  OldClipboard = SetClipboard(FClipboard);
  if OldClipboard != NULL then
    OldClipboard->Free();

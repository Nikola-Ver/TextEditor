//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <vcl.Clipbrd.hpp>
#include "Buff.h"
#include "BuffUnit.h"
#include "RussBuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBuffForm *BuffForm;
#define VK_ALT 18

int MoveFlag = false;
int dX, dY;
//---------------------------------------------------------------------------
__fastcall TBuffForm::TBuffForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void ChangeFontBuff()
{
    BuffForm->lblNum1->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblBuff1->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblNum2->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblBuff2->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblNum3->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblBuff3->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblNum4->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblBuff4->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblNum5->Font = BuffForm->dlgFontNotActive->Font;
    BuffForm->lblBuff5->Font = BuffForm->dlgFontNotActive->Font;
};

void ChangeFont()
{
    ChangeFontBuff();
    switch (BuffForm->CurrVar)
    {
      case 1:
          BuffForm->lblNum1->Font = BuffForm->dlgFontActive->Font;
          BuffForm->lblBuff1->Font = BuffForm->dlgFontActive->Font;
        break;
      case 2:
          BuffForm->lblNum2->Font = BuffForm->dlgFontActive->Font;
          BuffForm->lblBuff2->Font = BuffForm->dlgFontActive->Font;
        break;
      case 3:
          BuffForm->lblNum3->Font = BuffForm->dlgFontActive->Font;
          BuffForm->lblBuff3->Font = BuffForm->dlgFontActive->Font;
        break;
      case 4:
          BuffForm->lblNum4->Font = BuffForm->dlgFontActive->Font;
          BuffForm->lblBuff4->Font = BuffForm->dlgFontActive->Font;
        break;
      case 5:
          BuffForm->lblNum5->Font = BuffForm->dlgFontActive->Font;
          BuffForm->lblBuff5->Font = BuffForm->dlgFontActive->Font;
        break;
    };
};

void WriteToClipboard()
{
  try
  {
      switch (BuffForm->CurrVar)
      {
        case 1: BuffForm->lblBuff1->Caption = Clipboard()->AsText; break;
        case 2: BuffForm->lblBuff2->Caption = Clipboard()->AsText; break;
        case 3: BuffForm->lblBuff3->Caption = Clipboard()->AsText; break;
        case 4: BuffForm->lblBuff4->Caption = Clipboard()->AsText; break;
        case 5: BuffForm->lblBuff5->Caption = Clipboard()->AsText; break;
      };
  }
  catch (...) {WriteToClipboard();}
};

void AlignElement()
{
    BuffForm->lblNum2->Top = BuffForm->lblBuff1->Height;
    BuffForm->lblBuff2->Top = BuffForm->lblBuff1->Height;
    BuffForm->lblNum3->Top = BuffForm->lblBuff2->Top + BuffForm->lblBuff2->Height;
    BuffForm->lblBuff3->Top = BuffForm->lblBuff2->Top + BuffForm->lblBuff2->Height;
    BuffForm->lblNum4->Top = BuffForm->lblBuff3->Top + BuffForm->lblBuff3->Height;
    BuffForm->lblBuff4->Top = BuffForm->lblBuff3->Top + BuffForm->lblBuff3->Height;
    BuffForm->lblNum5->Top = BuffForm->lblBuff4->Top + BuffForm->lblBuff4->Height;
    BuffForm->lblBuff5->Top = BuffForm->lblBuff4->Top + BuffForm->lblBuff4->Height;
};

void __fastcall TBuffForm::TimerTimer(TObject *Sender)
{
   try
   {
     if (GetKeyState(112) & 0x8000)
     {
      ChangeFontBuff();
      Clipboard()->AsText = BuffForm->lblBuff1->Caption;
      lblNum1->Font = dlgFontActive->Font;
      lblBuff1->Font = dlgFontActive->Font;
      CurrVar = 1;
     };
     if (GetKeyState(113) & 0x8000)
     {
      ChangeFontBuff();
      Clipboard()->AsText = lblBuff2->Caption;
      lblNum2->Font = dlgFontActive->Font;
      lblBuff2->Font = dlgFontActive->Font;
      CurrVar = 2;
     };
     if (GetKeyState(114) & 0x8000)
     {
      ChangeFontBuff();
      Clipboard()->AsText = lblBuff3->Caption;
      lblNum3->Font = dlgFontActive->Font;
      lblBuff3->Font = dlgFontActive->Font;
      CurrVar = 3;
     };
     if (GetKeyState(115) & 0x8000)
     {
      ChangeFontBuff();
      Clipboard()->AsText = lblBuff4->Caption;
      lblNum4->Font = dlgFontActive->Font;
      lblBuff4->Font = dlgFontActive->Font;
      CurrVar = 4;
     };
     if (GetKeyState(116) & 0x8000)
     {
      ChangeFontBuff();
      Clipboard()->AsText = lblBuff5->Caption;
      lblNum5->Font = dlgFontActive->Font;
      lblBuff5->Font = dlgFontActive->Font;
      CurrVar = 5;
     };
   }
   catch (...) {};
   // Вырезаем или копируем строку
   if (((GetKeyState(VK_CONTROL) & GetKeyState(67)) & 0x8000) ||
      ((GetKeyState(VK_CONTROL) & GetKeyState(88)) & 0x8000))

   {
    WriteToClipboard();
    // И после того как скопируем наши строки в буфер и в наши label, скоректируем расстояние между label-ами
    AlignElement();
   };
   // Скрыть программу (F11)
   if (GetKeyState(122) & 0x8000)
      BuffForm->AlphaBlendValue = 0;
   // Показать программу (F12)
   if (GetKeyState(123) & 0x8000)
      BuffForm->AlphaBlendValue = 255;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniColorActiveClick(TObject *Sender)
{
  dlgFontActive->Execute();
  ChangeFont();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniColorNotActiveClick(TObject *Sender)
{
  dlgFontNotActive->Execute();
  ChangeFont();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::grpSetTranspMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  MoveFlag = true;
  dX = X;
  dY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::grpSetTranspMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
  if (MoveFlag)
  {
    grpSetTransp->Left = X - dX + grpSetTransp->Left;
    grpSetTransp->Top = Y - dY + grpSetTransp->Top;
  };
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::grpSetTranspMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  MoveFlag = false;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::ButtonButtCloseTranspClick(TObject *Sender)
{
  grpSetTransp->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniTransparencyFormClick(TObject *Sender)
{
  grpSetTransp->Visible = true;
  grpSetTransp->Left = 25;
  grpSetTransp->Top = 0;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::trckbrbarChange(TObject *Sender)
{
  grpSetTransp->SetFocus();
  BuffForm->AlphaBlendValue = trckbrbar->Position;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniClearBuffClick(TObject *Sender)
{
    switch (CurrVar)
    {
      case 1: lblBuff1->Caption = ""; break;
      case 2: lblBuff2->Caption = ""; break;
      case 3: lblBuff3->Caption = ""; break;
      case 4: lblBuff4->Caption = ""; break;
      case 5: lblBuff5->Caption = ""; break;
    };
    AlignElement();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniClearAllBuffClick(TObject *Sender)
{
    lblBuff1->Caption = "";
    lblBuff2->Caption = "";
    lblBuff3->Caption = "";
    lblBuff4->Caption = "";
    lblBuff5->Caption = "";
    AlignElement();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniCloseFormClick(TObject *Sender)
{
  BuffForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::lblBuff1Click(TObject *Sender)
{
  try
  {
    ChangeFontBuff();
    switch (((TLabel*)Sender)->HelpContext)
    {
      case 1:
          Clipboard()->AsText = lblBuff1->Caption;
          lblNum1->Font = dlgFontActive->Font;
          lblBuff1->Font = dlgFontActive->Font;
          CurrVar = 1;
        break;
      case 2:
          Clipboard()->AsText = lblBuff2->Caption;
          lblNum2->Font = dlgFontActive->Font;
          lblBuff2->Font = dlgFontActive->Font;
          CurrVar = 2;
        break;
      case 3:
          Clipboard()->AsText = lblBuff3->Caption;
          lblNum3->Font = dlgFontActive->Font;
          lblBuff3->Font = dlgFontActive->Font;
          CurrVar = 3;
        break;
      case 4:
          Clipboard()->AsText = lblBuff4->Caption;
          lblNum4->Font = dlgFontActive->Font;
          lblBuff4->Font = dlgFontActive->Font;
          CurrVar = 4;
        break;
      case 5:
          Clipboard()->AsText = lblBuff5->Caption;
          lblNum5->Font = dlgFontActive->Font;
          lblBuff5->Font = dlgFontActive->Font;
          CurrVar = 5;
        break;
    };
  }
  catch (...) {
    lblBuff1Click(Sender);
  };
  ;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::FormShow(TObject *Sender)
{
  // Функция WinApi, для вывода нашей формы поверх других приложений и форм
  SetWindowPos(BuffForm->Handle,HWND_TOPMOST,Screen->Width - 444,0,450,175,
                                                                SWP_SHOWWINDOW);
  // Установить шрифт
  ChangeFontBuff();
  lblNum1->Font = dlgFontActive->Font;
  lblBuff1->Font = dlgFontActive->Font;
  // Установить прозрачность
  BuffForm->AlphaBlend = true;
  BuffForm->AlphaBlendValue = 255;
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::mniAboutClick(TObject *Sender)
{
  AboutBuffForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TBuffForm::trckbrbarEnter(TObject *Sender)
{
  grpSetTransp->SetFocus();
}
//---------------------------------------------------------------------------


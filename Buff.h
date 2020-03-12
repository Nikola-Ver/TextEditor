//---------------------------------------------------------------------------

#ifndef BuffH
#define BuffH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TBuffForm : public TForm
{
__published:	// IDE-managed Components
	TFontDialog *dlgFontActive;
	TFontDialog *dlgFontNotActive;
	TGroupBox *grpSetTransp;
	TTrackBar *trckbrbar;
	TButton *ButtonButtCloseTransp;
	TImageList *ImageList;
	TLabel *lblBuff1;
	TLabel *lblBuff2;
	TLabel *lblBuff3;
	TLabel *lblBuff4;
	TLabel *lblBuff5;
	TLabel *lblNum1;
	TLabel *lblNum2;
	TLabel *lblNum3;
	TLabel *lblNum4;
	TLabel *lblNum5;
	TMainMenu *MainMenu;
	TMenuItem *mniChange;
	TMenuItem *mniTransparencyForm;
	TMenuItem *mniN3;
	TMenuItem *mniColorActive;
	TMenuItem *mniColorNotActive;
	TMenuItem *mniN1;
	TMenuItem *mniClearBuff;
	TMenuItem *mniClearAllBuff;
	TMenuItem *mniN2;
	TMenuItem *mniCloseForm;
	TMenuItem *mniHelp;
	TMenuItem *mniAbout;
	TTimer *Timer;
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall mniColorActiveClick(TObject *Sender);
	void __fastcall mniColorNotActiveClick(TObject *Sender);
	void __fastcall grpSetTranspMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall grpSetTranspMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall grpSetTranspMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButtonButtCloseTranspClick(TObject *Sender);
	void __fastcall mniTransparencyFormClick(TObject *Sender);
	void __fastcall trckbrbarChange(TObject *Sender);
	void __fastcall mniClearBuffClick(TObject *Sender);
	void __fastcall mniClearAllBuffClick(TObject *Sender);
	void __fastcall mniCloseFormClick(TObject *Sender);
	void __fastcall lblBuff1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall mniAboutClick(TObject *Sender);
	void __fastcall trckbrbarEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
        int CurrVar = 1;
	__fastcall TBuffForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBuffForm *BuffForm;
//---------------------------------------------------------------------------
#endif

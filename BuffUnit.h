//---------------------------------------------------------------------------

#ifndef BuffUnitH
#define BuffUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TAboutBuffForm : public TForm
{
__published:	// IDE-managed Components
	TImage *img1;
	TLabel *lbl2;
	TLabel *lbl1;
private:	// User declarations
public:		// User declarations
	__fastcall TAboutBuffForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutBuffForm *AboutBuffForm;
//---------------------------------------------------------------------------
#endif

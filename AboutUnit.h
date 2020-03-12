//---------------------------------------------------------------------------

#ifndef AboutUnitH
#define AboutUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TAboutMainForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *HeadText;
private:	// User declarations
public:		// User declarations
	__fastcall TAboutMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutMainForm *AboutMainForm;
//---------------------------------------------------------------------------
#endif

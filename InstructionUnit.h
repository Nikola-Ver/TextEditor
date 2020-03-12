//---------------------------------------------------------------------------

#ifndef InstructionUnitH
#define InstructionUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TInstructionForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblText;
private:	// User declarations
public:		// User declarations
	__fastcall TInstructionForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInstructionForm *InstructionForm;
//---------------------------------------------------------------------------
#endif

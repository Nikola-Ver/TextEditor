//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include <Winapi.RichEdit.hpp>
USEFORM("MainUnit.cpp", MainForm);
USEFORM("InstructionUnit.cpp", InstructionForm);
USEFORM("Buff.cpp", BuffForm);
USEFORM("BuffUnit.cpp", AboutBuffForm);
USEFORM("AboutUnit.cpp", AboutMainForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
        setlocale( LC_ALL, "" );
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		TStyleManager::TrySetStyle("Windows10 Dark");
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TBuffForm), &BuffForm);
		Application->CreateForm(__classid(TAboutBuffForm), &AboutBuffForm);
		Application->CreateForm(__classid(TInstructionForm), &InstructionForm);
		Application->CreateForm(__classid(TAboutMainForm), &AboutMainForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include <vcl.Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

void SearchWords(TEdit *Word, TRichEdit *Text, int Var, TSearchTypes SearchSettings)
{
        long long currCaretPos = -1;
        long long Pos = -1;
        long long caretPos = Text->SelStart + Text->SelLength;
        if (Var)
        {
            Text->Lines->BeginUpdate();
            Text->Text = " " + Text->Text;
            Pos = Text->FindText(Word->Text, caretPos + 1,
            				   Text->Text.Length(), SearchSettings);
            Text->Text = &Text->Text[2];
            if (Pos > 0)
            {
                Text->SelStart = Pos - 1;
                Text->SelLength = Word->Text.Length();
            }
        }
        else
        if (Word->Text.Length() > 0)
        {
            long long prevPos = -1;
            caretPos -= Text->SelLength;
            Text->Lines->BeginUpdate();
            Text->Text = " " + Text->Text;
            while (currCaretPos < caretPos)
            {
                Pos = Text->FindText(Word->Text, currCaretPos + 1,
                                           Text->Text.Length(), SearchSettings);
                if ((Pos > 0) && (Pos <= caretPos))
                {
                    prevPos = Pos;
                    currCaretPos = Pos + Word->Text.Length() - 1;
                }
                else currCaretPos = caretPos;
            }
            Pos = prevPos;
            Text->Text = &Text->Text[2];
            if (Pos > 0)
            {
                Text->SelStart = Pos - 1;
                Text->SelLength = Word->Text.Length();
            }
        }
        Text->Lines->EndUpdate();
}

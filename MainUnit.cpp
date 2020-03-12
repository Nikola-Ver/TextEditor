//---------------------------------------------------------------------------
#include <vcl.h>
#include <vcl.Clipbrd.hpp>

#pragma hdrstop
#include "Buff.h"
#include "BuffUnit.h"
#include "Instruction.h"
#include "InstructionUnit.h"
#include "AboutUnit.h"
#include "About.h"
#include "SearchUnit.cpp"
#include "MainUnit.h"
#include "BaseWords.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <stdio.h>
#include <cstring>
#include <windows.h>
#include <ctime>
#pragma resource "*.dfm"
#include "SAPI.h"
const CLSID CLSID_SpVoice= {0x96749377, 0x3391, 0x11D2, {0x9E, 0xE3,0x00,0xC0,0x4F,0x79,0x73,0x96}};
const IID IID_ISpVoice  = {0x6C44DF74, 0x72B9, 0x4992, {0xA1, 0xEC,0xEF,0x99,0x6E,0x04,0x22,0xD4}};
TMainForm *MainForm;
TBaseWords *BaseWords;
AnsiString NameSave = "";
TSearchTypes mySearchTypes = TSearchTypes();
bool flagBoxExSearch = false;
bool FlagChangeSelText = false;
TShiftState ShiftEmpty = TShiftState();
int cX,cY;
String locApp = ExtractFilePath(Application->ExeName);
bool moveSetUnderline = false;
bool moveSetHighlighted = false;
bool moveSetNumbering = false;
bool moveSetLine = false;
bool NeedSaveFlag = false;
bool ReplaceErrorFlag = false;
int ReplaceLeft = 0;
int ReplaceRight = 0;
int currPosReplace;
std::vector<std::vector<char>> ReplaceWords;
char Style[20];
long long PosT9 = 0;
long long iPosT9 = 0;
int AreaColor;
char FileEncoding[20];
int Zoom = 100;
int CarryWords;
TEncoding *myEncoding;
//----------------------Переменные базы слов---------------------------------
char MainLink[] = "BaseOfWords.txt";
long long lenWords = 0;
long long lenMas = 0;
std::vector<std::vector<char>> ListWords;
char WordT9[256];
bool inizFlag = false;
ISpVoice * pVoice = NULL;
int endVoice = 0;
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
__fastcall TBaseWords::TBaseWords(int Thread)
        : TThread(false)
{
    State = Thread;
    FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void __fastcall TBaseWords::Execute()
{

    if (State == 1)
    {
        if (lenMas == 0)
        {
            lenWords = SetLenWords(&MainLink[0]);
            lenMas = SetLenMas(&MainLink[0], lenWords);
            ListWords.resize(lenMas, std::vector<char>(lenWords + 1));
            ReplaceWords.resize(0, std::vector<char>(lenWords + 1));

            FILE *RTextFile;
            RTextFile = fopen(&MainLink[0], "r");

            long long i = 0;
            while (fscanf(RTextFile, "%s", &ListWords[i][0]) != EOF) i++;
            fclose(RTextFile);
            inizFlag = true;
            return;
        };

        Synchronize(GetPos);
        currPos = 1;
        Shift = 0;
        if (MainForm->TextArea->SelLength > 0)
        {
            Shift = MainForm->TextArea->CaretPos.Y;
            currPos = MainForm->TextArea->CaretPos.Y + 1 + MainForm->TextArea->SelStart;
            lenText = currPos + MainForm->TextArea->SelLength;
            if ((MainForm->TextArea->SelLength == 1) ||
            	(MainForm->TextArea->Text.Length() < lenText)) return;
        };
        wchar_t temp_t[lenWords + 1];

        MainForm->TextArea->Lines->BeginUpdate();
        try
        {
            while (currPos < lenText)
            {
                prevPos = currPos;
                while (
                        (
                            ((MainForm->TextArea->Text[currPos] >= L'а') &&
                            (MainForm->TextArea->Text[currPos] <= L'я')) ||
                            (MainForm->TextArea->Text[currPos] == L'ё') ||
                            ((MainForm->TextArea->Text[currPos] >= L'a') &&
                            (MainForm->TextArea->Text[currPos] <= L'z')) ||
                            ((MainForm->TextArea->Text[currPos] >= L'А') &&
                            (MainForm->TextArea->Text[currPos] <= L'Я')) ||
                            (MainForm->TextArea->Text[currPos] == L'Ё') ||
                            ((MainForm->TextArea->Text[currPos] >= L'A') &&
                            (MainForm->TextArea->Text[currPos] <= L'Z'))
                        ) && (currPos < lenText)
                       )
                {
                    currPos++;
                };

                int Pos = prevPos;

                for (; (Pos < currPos) && (currPos - prevPos <= lenWords) &&
                     (Pos >= 1); Pos++)
                {
                    temp_t[Pos - prevPos] = MainForm->TextArea->Text[Pos];
                };
                temp_t[Pos - prevPos] = '\0';
                int size_needed = WideCharToMultiByte(CP_ACP, 0, temp_t, currPos - prevPos, NULL, 0, NULL, NULL);
                char temp[size_needed + 1];
                WideCharToMultiByte(CP_ACP, 0, temp_t, currPos - prevPos, temp, size_needed, NULL, NULL);
                temp[size_needed] = '\0';

                if (prevPos == currPos)
                {
                    currPos++;
                    if (MainForm->TextArea->Text[currPos] == '\n') {
                            Shift++;
                    };
                }

                if (
                      (size_needed > lenWords) ||
                      (((strlen(temp) > 0) && (!Search(&ListWords, lenMas, size_needed + 1, temp)))
                      && !(((temp[0] >= 'А') && (temp[0] <= 'Я')) || (temp[0] == 'Ё') ||
                          ((temp[0] >= 'A') && (temp[0] <= 'Z')))    )
                   )
                {
                        Synchronize(SearchError);
                };
            };
            MainForm->TextArea->Lines->EndUpdate();
            Shift = 0;
        }
        catch(Exception *ex)
        {
            MainForm->TextArea->Lines->EndUpdate();
        };
    };

    if (State == 2)
    {
        if (FAILED(::CoInitialize(NULL)))
        return ;

        HRESULT hr = CoCreateInstance(CLSID_SpVoice,NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
        if( SUCCEEDED( hr ) )
        {
            if (MainForm->TextArea->SelText.Length() > 0)
            {
                endVoice = MainForm->TextArea->Text.Length();
                hr = pVoice->SetVolume(MainForm->VolumeSpeaker->Position);
                hr = pVoice->Speak(&MainForm->TextArea->SelText[1], 0, NULL);
                pVoice->Release();
                pVoice = NULL;
                endVoice = 0;
            }
            else
            {
                endVoice = MainForm->TextArea->Text.Length();
                hr = pVoice->SetVolume(MainForm->VolumeSpeaker->Position);
                int beginPos = MainForm->TextArea->CaretPos.Y + 1 + MainForm->TextArea->SelStart;
                if (beginPos <= MainForm->TextArea->Text.Length())
                    hr = pVoice->Speak(&MainForm->TextArea->Text[beginPos], 0, NULL);
                pVoice->Release();
                pVoice = NULL;
                endVoice = 0;
            }
	}
    }
}
//---------------------------------------------------------------------------
void __fastcall TBaseWords::GetPos()
{
    long long i = MainForm->TextArea->Text.Length();
    for (;(i > 1) && (MainForm->TextArea->Text[i] != L' ') ; i--){};
    lenText = i;
}
//---------------------------------------------------------------------------
void __fastcall TBaseWords::SearchError()
{
    int temp = MainForm->TextArea->SelStart;
    MainForm->TextArea->HideSelection = false;
    MainForm->TextArea->SelStart = prevPos - Shift - 1;
    MainForm->TextArea->SelLength = currPos - prevPos;
    ::CHARFORMAT2A cf;
    cf.cbSize = sizeof( cf );
    cf.dwMask = CFM_UNDERLINETYPE;
    cf.bUnderlineType = CFU_UNDERLINEWAVE | 0x50;
    SendMessage(MainForm->TextArea->Handle, EM_SETCHARFORMAT, SCF_SELECTION,
                                                              (LPARAM) &cf);
    MainForm->TextArea->SelStart = temp;
}
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
    mySearchTypes = mySearchTypes >> stMatchCase;
    mySearchTypes = mySearchTypes >> stWholeWord;
    int Charset, Size, Height, Bold, Italic, Underline, StrikeOut, pWidth,
        pHeight, pTop, pLeft;
    char FontStyle[256] = {'\0'};
    FILE *SetFile;
    SetFile = fopen("Settings.dat", "r");
    fscanf(SetFile, "%d %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
             &TextArea->Font->Color, &Style[0], &FileEncoding[0],
             &Charset, &Size, &Height, &Bold, &Italic, &Underline,
             &StrikeOut, &CarryWords, &pWidth, &pHeight, &pTop, &pLeft);
    StatusText->Caption = "Стр. 0, стлб. 0";
    QuantityWords->Caption = "Симв. 0  100%";
    StatusEncoding->Caption = FileEncoding;
    StatusEncoding->AutoSize = true;
    StatusText->AutoSize = true;
    QuantityWords->AutoSize = true;
    WordsBox->Width = StatusText->Left - 10;
    int i = 0;
    while (fscanf(SetFile, "%s",  &FontStyle[i]) != EOF)
    {
            FontStyle[strlen(&FontStyle[0])] = ' ';
            i = strlen(&FontStyle[0]);
    };
    FontStyle[i - 1] = '\0';
    TextArea->Font->Name = &FontStyle[0];
    MainForm->Width = pWidth;
    MainForm->Height = pHeight;
    MainForm->Top = pTop;
    MainForm->Left = pLeft;
    if (Bold != 0) TextArea->Font->Style = TextArea->Font->Style << fsBold;
    if (Italic != 0) TextArea->Font->Style = TextArea->Font->Style
                                                                << fsItalic;
    if (Underline != 0) TextArea->Font->Style = TextArea->Font->Style
                                                             << fsUnderline;
    if (StrikeOut != 0) TextArea->Font->Style = TextArea->Font->Style
                                                             << fsStrikeOut;
    TextArea->Font->Charset = Charset;
    TextArea->Font->Size = Size;
    TextArea->Font->Height = Height;
    fclose(SetFile);
    if (CarryWords == -1)
    {
            TextArea->ScrollBars = ssBoth;
            CarryWord->ImageIndex = -1;
    }
    else
    {
            TextArea->ScrollBars = ssVertical;
            CarryWord->ImageIndex = 0;
    };

    FormatUTF8->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = -1;
    FormatAnsi->ImageIndex = -1;
    FormatUnicode->ImageIndex = -1;
    FormatASCII->ImageIndex = -1;

    if (strcmp(&FileEncoding[0], "ASCII") == 0)
    {
        myEncoding = TEncoding::ASCII;
        FormatASCII->ImageIndex = 0;
    };

    if (strcmp(&FileEncoding[0], "UTF8") == 0)
    {
        myEncoding = TEncoding::UTF8;
        FormatUTF8->ImageIndex = 0;
    };

    if (strcmp(&FileEncoding[0], "ANSI") == 0)
    {
        myEncoding = TEncoding::ANSI;
        FormatAnsi->ImageIndex = 0;
    };

    if (strcmp(&FileEncoding[0], "Unicode") == 0)
    {
        myEncoding = TEncoding::Unicode;
        FormatUnicode->ImageIndex = 0;
    };

    if (strcmp(&FileEncoding[0], "BigEndianUnicode") == 0)
    {
        myEncoding = TEncoding::BigEndianUnicode;
        FormatBigEndianUnicode->ImageIndex = 0;
    };

//Цвет стиля
    Amakrits->ImageIndex = -1;
    Auric->ImageIndex = -1;
    Carbon->ImageIndex = -1;
    TabletDark->ImageIndex = -1;
    White->ImageIndex = -1;
    Dark->ImageIndex = -1;
    Green->ImageIndex = -1;

    if (strcmp(&Style[0], "Amakrits") == 0)
    {
        Amakrits->ImageIndex = 0;
        strcpy(Style, "Amakrits");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 48 + 48 * 256 + 48 * 256 * 256;
    };

    if (strcmp(&Style[0], "Auric") == 0)
    {
        Auric->ImageIndex = 0;
        strcpy(Style, "Auric");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 66 + 66 * 256 + 66 * 256 * 256;
    };

    if (strcmp(&Style[0], "Carbon") == 0)
    {
        Carbon->ImageIndex = 0;
        strcpy(Style, "Carbon");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 25 + 25 * 256 + 25 * 256 * 256;
    };

    if (strcmp(&Style[0], "TabletDark") == 0)
    {
        TabletDark->ImageIndex = 0;
        strcpy(Style, "TabletDark");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 57 + 66 * 256 + 100 * 256 * 256;
    };

    if (strcmp(&Style[0], "Windows10") == 0)
    {
        White->ImageIndex = 0;
        strcpy(Style, "Windows10");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 255 + 255 * 256 + 255 * 256 * 256;
    };

    if (strcmp(&Style[0], "Windows10 Dark") == 0)
    {
        Dark->ImageIndex = 0;
        strcpy(Style, "Windows10 Dark");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 0;
    };

    if (strcmp(&Style[0], "Windows10 Green") == 0)
    {
        Green->ImageIndex = 0;
        strcpy(Style, "Windows10 Green");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 255 + 255 * 256 + 255 * 256 * 256;
    };
    TouchKeyboard->Color = TColor(AreaColor);
    TouchKeyboard->Left = (MainForm->Width - TouchKeyboard->Width) / 2;
    TextArea->Color = TColor(AreaColor);
    BaseWords = new TBaseWords(1);
    SetColorUnderline->AddItem("Под цвет текста", (TObject*)TColor(255 + 255 * 256 + 255 * 256 * 256));
    SetColorUnderline->AddItem("Синий", (TObject*)TColor(255 * 256 * 256));
    SetColorUnderline->AddItem("Голубой", (TObject*)TColor(255 * 256 + 255 * 256 * 256));
    SetColorUnderline->AddItem("Зелёный", (TObject*)TColor(255 * 256));
    SetColorUnderline->AddItem("Розовый", (TObject*)TColor(255 + 255 * 256 * 256));
    SetColorUnderline->AddItem("Красный", (TObject*)TColor(255));
    SetColorUnderline->AddItem("Желтый", (TObject*)TColor(255 + 255 * 256));
    SetColorUnderline->AddItem("Белый", (TObject*)TColor(255 + 255 * 256 + 255 * 256 * 256));
    SetColorUnderline->AddItem("Темно-синий", (TObject*)TColor(100 * 256 * 256));
    SetColorUnderline->AddItem("Темно-голубой", (TObject*)TColor(55 * 256 + 55 * 256 * 256));
    SetColorUnderline->AddItem("Темно-зелёный", (TObject*)TColor(55 * 256));
    SetColorUnderline->AddItem("Темно-розовый", (TObject*)TColor(55 + 55 * 256 * 256));
    SetColorUnderline->AddItem("Темно-красный", (TObject*)TColor(55));
    SetColorUnderline->AddItem("Темно-желтый", (TObject*)TColor(55 + 55 * 256));
    SetColorUnderline->AddItem("Серый", (TObject*)TColor(55 + 55 * 256 + 55 * 256 * 256));
    SetColorUnderline->AddItem("Темно-белый", (TObject*)TColor(155 + 155 * 256 + 155 * 256 * 256));
    SetColorHighlighted->AddItem("Удалить выделения", (TObject*)TColor(255 + 255 * 256 + 255 * 256 * 256));
}
//---------------------------------------------------------------------------
void SwapKeyLanguage()
{
    int PosCur = MainForm->TextArea->SelStart;
    if (!(MainForm->TextArea->SelText.Length() > 0))
    {
        MainForm->TextArea->SelectAll();
    };

    if (MainForm->TextArea->SelText.Length() > 25000)
    {
            MainForm->TextArea->SelStart = PosCur;
            FlagChangeSelText = true;
            return;
    }

    wchar_t Str[MainForm->TextArea->SelText.Length()];
    wcscpy(Str, &MainForm->TextArea->SelText[1]);

    for (int i = 0; i < MainForm->TextArea->SelText.Length(); i++)
    {
        switch (Str[i])
        {
                case L'q': Str[i] = L'й'; break;
                case L'w': Str[i] = L'ц'; break;
                case L'e': Str[i] = L'у'; break;
                case L'r': Str[i] = L'к'; break;
                case L't': Str[i] = L'е'; break;
                case L'y': Str[i] = L'н'; break;
                case L'u': Str[i] = L'г'; break;
                case L'i': Str[i] = L'ш'; break;
                case L'o': Str[i] = L'щ'; break;
                case L'p': Str[i] = L'з'; break;
                case L'[': Str[i] = L'х'; break;
                case L']': Str[i] = L'ъ'; break;
                case L'a': Str[i] = L'ф'; break;
                case L's': Str[i] = L'ы'; break;
                case L'd': Str[i] = L'в'; break;
                case L'f': Str[i] = L'а'; break;
                case L'g': Str[i] = L'п'; break;
                case L'h': Str[i] = L'р'; break;
                case L'j': Str[i] = L'о'; break;
                case L'k': Str[i] = L'л'; break;
                case L'l': Str[i] = L'д'; break;
                case L';': Str[i] = L'ж'; break;
                case L'\'':Str[i] = L'э'; break;
                case L'z': Str[i] = L'я'; break;
                case L'x': Str[i] = L'ч'; break;
                case L'c': Str[i] = L'с'; break;
                case L'v': Str[i] = L'м'; break;
                case L'b': Str[i] = L'и'; break;
                case L'n': Str[i] = L'т'; break;
                case L'm': Str[i] = L'ь'; break;
                case L',': Str[i] = L'б'; break;
                case L'.': Str[i] = L'ю'; break;
                case L'Q': Str[i] = L'Й'; break;
                case L'W': Str[i] = L'Ц'; break;
                case L'E': Str[i] = L'У'; break;
                case L'R': Str[i] = L'К'; break;
                case L'T': Str[i] = L'Е'; break;
                case L'Y': Str[i] = L'Н'; break;
                case L'U': Str[i] = L'Г'; break;
                case L'I': Str[i] = L'Ш'; break;
                case L'O': Str[i] = L'Щ'; break;
                case L'P': Str[i] = L'З'; break;
                case L'{': Str[i] = L'Х'; break;
                case L'}': Str[i] = L'Ъ'; break;
                case L'A': Str[i] = L'Ф'; break;
                case L'S': Str[i] = L'Ы'; break;
                case L'D': Str[i] = L'В'; break;
                case L'F': Str[i] = L'А'; break;
                case L'G': Str[i] = L'П'; break;
                case L'H': Str[i] = L'Р'; break;
                case L'J': Str[i] = L'О'; break;
                case L'K': Str[i] = L'Л'; break;
                case L'L': Str[i] = L'Д'; break;
                case L':': Str[i] = L'Ж'; break;
                case L'"': Str[i] = L'Э'; break;
                case L'Z': Str[i] = L'Я'; break;
                case L'X': Str[i] = L'Ч'; break;
                case L'C': Str[i] = L'С'; break;
                case L'V': Str[i] = L'М'; break;
                case L'B': Str[i] = L'И'; break;
                case L'N': Str[i] = L'Т'; break;
                case L'M': Str[i] = L'Ь'; break;
                case L'<': Str[i] = L'Б'; break;
                case L'>': Str[i] = L'Ю'; break;
                case L'й': Str[i] = L'q'; break;
                case L'ц': Str[i] = L'w'; break;
                case L'у': Str[i] = L'e'; break;
                case L'к': Str[i] = L'r'; break;
                case L'е': Str[i] = L't'; break;
                case L'н': Str[i] = L'y'; break;
                case L'г': Str[i] = L'u'; break;
                case L'ш': Str[i] = L'i'; break;
                case L'щ': Str[i] = L'o'; break;
                case L'з': Str[i] = L'p'; break;
                case L'х': Str[i] = L'['; break;
                case L'ъ': Str[i] = L']'; break;
                case L'ф': Str[i] = L'a'; break;
                case L'ы': Str[i] = L's'; break;
                case L'в': Str[i] = L'd'; break;
                case L'а': Str[i] = L'f'; break;
                case L'п': Str[i] = L'g'; break;
                case L'р': Str[i] = L'h'; break;
                case L'о': Str[i] = L'j'; break;
                case L'л': Str[i] = L'k'; break;
                case L'д': Str[i] = L'l'; break;
                case L'ж': Str[i] = L';'; break;
                case L'э': Str[i] = L'\''; break;
                case L'я': Str[i] = L'z'; break;
                case L'ч': Str[i] = L'x'; break;
                case L'с': Str[i] = L'c'; break;
                case L'м': Str[i] = L'v'; break;
                case L'и': Str[i] = L'b'; break;
                case L'т': Str[i] = L'n'; break;
                case L'ь': Str[i] = L'm'; break;
                case L'б': Str[i] = L','; break;
                case L'ю': Str[i] = L'.'; break;
                case L'Й': Str[i] = L'Q'; break;
                case L'Ц': Str[i] = L'W'; break;
                case L'У': Str[i] = L'E'; break;
                case L'К': Str[i] = L'R'; break;
                case L'Е': Str[i] = L'T'; break;
                case L'Н': Str[i] = L'Y'; break;
                case L'Г': Str[i] = L'U'; break;
                case L'Ш': Str[i] = L'I'; break;
                case L'Щ': Str[i] = L'O'; break;
                case L'З': Str[i] = L'P'; break;
                case L'Х': Str[i] = L'{'; break;
                case L'Ъ': Str[i] = L'}'; break;
                case L'Ф': Str[i] = L'A'; break;
                case L'Ы': Str[i] = L'S'; break;
                case L'В': Str[i] = L'D'; break;
                case L'А': Str[i] = L'F'; break;
                case L'П': Str[i] = L'G'; break;
                case L'Р': Str[i] = L'H'; break;
                case L'О': Str[i] = L'J'; break;
                case L'Л': Str[i] = L'K'; break;
                case L'Д': Str[i] = L'L'; break;
                case L'Ж': Str[i] = L':'; break;
                case L'Э': Str[i] = L'"'; break;
                case L'Я': Str[i] = L'Z'; break;
                case L'Ч': Str[i] = L'X'; break;
                case L'С': Str[i] = L'C'; break;
                case L'М': Str[i] = L'V'; break;
                case L'И': Str[i] = L'B'; break;
                case L'Т': Str[i] = L'N'; break;
                case L'Ь': Str[i] = L'M'; break;
                case L'Б': Str[i] = L'<'; break;
                case L'Ю': Str[i] = L'>'; break;
        };
    };
    MainForm->TextArea->SelText = Str;
    MainForm->TextArea->SelStart = PosCur;
    FlagChangeSelText = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveFileClick(TObject *Sender)
{
    if ((NameSave.Length() == 0) && ((Open->FileName.Length() == 0))) {
        Save->Filter = "Файл редактора (*.rtf)|*.rtf|Текстовый файл (*.txt)|*.txt|Все файлы (*.*)|*.*";
        Save->DefaultExt = "*.*";
        Save->FilterIndex = 3;
        Save->Execute();
        NameSave = Save->FileName;
    };
    if (NameSave.Length() != 0)
    {
            NeedSaveFlag = false;
            MainForm->Caption = "Text editor";
            if (Save->FilterIndex == 1) {
                TextArea->PlainText = False;
                TextArea->Lines->SaveToFile(NameSave, myEncoding);
                TextArea->PlainText = True;
            }
            else
                TextArea->Lines->SaveToFile(NameSave, myEncoding);
    }
    else
        if (Open->FileName.Length() != 0)
        {
            NeedSaveFlag = false;
            MainForm->Caption = "Text editor";
            if (Open->FilterIndex == 1) {
                TextArea->PlainText = False;
                TextArea->Lines->SaveToFile(Open->FileName, myEncoding);
                TextArea->PlainText = True;
            }
            else
                TextArea->Lines->SaveToFile(Open->FileName, myEncoding);
        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveAsFileClick(TObject *Sender)
{
    Save->Filter = "Файл редактора (*.rtf)|*.rtf|Текстовый файл (*.txt)|*.txt|Все файлы (*.*)|*.*";
    Save->DefaultExt = "*.*";
    Save->FilterIndex = 3;
    Save->Execute();
    NameSave = Save->FileName;
    if (NameSave.Length() != 0)
    {
            NeedSaveFlag = false;
            MainForm->Caption = "Text editor";
            if (Save->FilterIndex == 1) {
                TextArea->PlainText = False;
                TextArea->Lines->SaveToFile(NameSave, myEncoding);
                TextArea->PlainText = True;
            }
            else
                TextArea->Lines->SaveToFile(NameSave, myEncoding);
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OpenFileClick(TObject *Sender)
{
    Open->Filter = "Файл редактора (*.rtf)|*.rtf|Текстовый файл (*.txt)|*.txt|Все файлы (*.*)|*.*";
    Open->DefaultExt = "*.*";
    Open->FilterIndex = 3;
    if (Open->Execute())
    {
        NeedSaveFlag = false;
        MainForm->Caption = "Text editor";
        if (Open->FilterIndex == 1) {
            TextArea->PlainText = False;
            TextArea->Lines->LoadFromFile(Open->FileName, myEncoding);
            TextArea->PlainText = True;
        }
        else
            TextArea->Lines->LoadFromFile(Open->FileName, myEncoding);
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormatASCIIClick(TObject *Sender)
{
    myEncoding = TEncoding::ASCII;
    strcpy(FileEncoding, "ASCII");
    StatusEncoding->Caption = FileEncoding;
    FormatUTF8->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = -1;
    FormatAnsi->ImageIndex = -1;
    FormatUnicode->ImageIndex = -1;
    FormatASCII->ImageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormatUTF8Click(TObject *Sender)
{
    myEncoding = TEncoding::UTF8;
    strcpy(FileEncoding, "UTF8");
    StatusEncoding->Caption = FileEncoding;
    FormatASCII->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = -1;
    FormatAnsi->ImageIndex = -1;
    FormatUnicode->ImageIndex = -1;
    FormatUTF8->ImageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormatAnsiClick(TObject *Sender)
{
    myEncoding = TEncoding::ANSI;
    strcpy(FileEncoding, "ANSI");
    StatusEncoding->Caption = FileEncoding;
    FormatASCII->ImageIndex = -1;
    FormatUTF8->ImageIndex = -1;
    FormatUnicode->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = -1;
    FormatAnsi->ImageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormatUnicodeClick(TObject *Sender)
{
    myEncoding = TEncoding::Unicode;
    strcpy(FileEncoding, "Unicode");
    StatusEncoding->Caption = FileEncoding;
    FormatASCII->ImageIndex = -1;
    FormatUTF8->ImageIndex = -1;
    FormatAnsi->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = -1;
    FormatUnicode->ImageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GreenClick(TObject *Sender)
{
    Amakrits->ImageIndex = -1;
    Auric->ImageIndex = -1;
    Carbon->ImageIndex = -1;
    TabletDark->ImageIndex = -1;
    White->ImageIndex = -1;
    Dark->ImageIndex = -1;
    Green->ImageIndex = -1;
    if (Sender == Amakrits)
    {
        Amakrits->ImageIndex = 0;
        strcpy(Style, "Amakrits");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 48 + 48 * 256 + 48 * 256 * 256;
    };

    if (Sender == Auric)
    {
        Auric->ImageIndex = 0;
        strcpy(Style, "Auric");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 66 + 66 * 256 + 66 * 256 * 256;
    };

    if (Sender == Carbon)
    {
        Carbon->ImageIndex = 0;
        strcpy(Style, "Carbon");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 25 + 25 * 256 + 25 * 256 * 256;
    };

    if (Sender == TabletDark)
    {
        TabletDark->ImageIndex = 0;
        strcpy(Style, "TabletDark");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 57 + 66 * 256 + 100 * 256 * 256;
    };

    if (Sender == White)
    {
        White->ImageIndex = 0;
        strcpy(Style, "Windows10");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 255 + 255 * 256 + 255 * 256 * 256;
    };

    if (Sender == Dark)
    {
        Dark->ImageIndex = 0;
        strcpy(Style, "Windows10 Dark");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 0;
    };

    if (Sender == Green)
    {
        Green->ImageIndex = 0;
        strcpy(Style, "Windows10 Green");
        TStyleManager::TrySetStyle(Style);
        AreaColor = 255 + 255 * 256 + 255 * 256 * 256;
    };
    TextArea->Color = TColor(AreaColor);
    TouchKeyboard->Color = TColor(AreaColor);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FontClick(TObject *Sender)
{
    if (FontDialog->Execute())
    {
            if (TextArea->SelLength != 0)
            {
                TextArea->SelAttributes->Color = FontDialog->Font->Color;
                TextArea->SelAttributes->Height = FontDialog->Font->Height;
                TextArea->SelAttributes->Style = FontDialog->Font->Style;
                TextArea->SelAttributes->Name = FontDialog->Font->Name;
            }
            else
            {
                TextArea->SelectAll();
                TextArea->SelAttributes->Color = FontDialog->Font->Color;
                TextArea->SelAttributes->Height = FontDialog->Font->Height;
                TextArea->SelAttributes->Style = FontDialog->Font->Style;
                TextArea->SelAttributes->Name = FontDialog->Font->Name;
                TextArea->SelStart = TextArea->Text.Length();
                TextArea->Font = FontDialog->Font;
            }
    };
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CarryWordClick(TObject *Sender)
{
    if (CarryWords == 0)
    {
            TextArea->ScrollBars = ssBoth;
            CarryWord->ImageIndex = -1;
            CarryWords = -1;
    }
    else
    {
            TextArea->ScrollBars = ssVertical;
            CarryWord->ImageIndex = 0;
            CarryWords = 0;
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetupPrintClick(TObject *Sender)
{
    PrinterSetup->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PrintFileClick(TObject *Sender)
{
    if(PrintDialog->Execute())
    {
        TextArea->PlainText = False;
        TextArea->Print("Печать");
        TextArea->PlainText = True;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormatBigEndianUnicodeClick(TObject *Sender)
{
    myEncoding = TEncoding::BigEndianUnicode;
    strcpy(FileEncoding, "BigEndianUnicode");
    StatusEncoding->Caption = FileEncoding;
    FormatASCII->ImageIndex = -1;
    FormatUTF8->ImageIndex = -1;
    FormatAnsi->ImageIndex = -1;
    FormatUnicode->ImageIndex = -1;
    FormatBigEndianUnicode->ImageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    int Pos;
    char fname[] = "Settings.dat";
    wchar_t temp__t[locApp.Length()];
    for (Pos = 0; Pos < locApp.Length(); Pos++)
         temp__t[Pos] = locApp[Pos + 1];

    int size_needed = WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(),
                                                    NULL, 0, NULL, NULL);
    char BaseLink[strlen(fname) + size_needed + 1];
    WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(), BaseLink,
                                           size_needed, NULL, NULL);

    for (Pos = size_needed; Pos < strlen(fname) + size_needed; Pos++)
         BaseLink[Pos] = fname[Pos - size_needed];
    BaseLink[Pos] = '\0';

    HWND hwnd = (HWND)this->Handle;
    int len = TextArea->Font->Name.Length();
    char FontName[len + 1];
    for (int i = 0; i < len; i++) FontName[i] = TextArea->Font->Name[i + 1];
    FontName[len] = '\0';
    FILE *SetFile;
    SetFile = fopen(BaseLink, "w");
    fprintf
    (
        SetFile, "%d %s %s %d %d %d %d %d %d %d %d %d %d %d %d %s",
        TextArea->Font->Color, &Style[0], &FileEncoding[0],
        TextArea->Font->Charset, TextArea->Font->Size,
        TextArea->Font->Height, TextArea->Font->Style.Contains(fsBold),
        TextArea->Font->Style.Contains(fsItalic),
        TextArea->Font->Style.Contains(fsUnderline),
        TextArea->Font->Style.Contains(fsStrikeOut), CarryWords,
        MainForm->Width, MainForm->Height, MainForm->Top,
        MainForm->Left, &FontName[0]
    );
    fclose(SetFile);

    if ((NeedSaveFlag) && (TextArea->Text.Length()))
        switch (MessageBox(hwnd, L"Сохранить файл?",L"Сохранение",MB_YESNOCANCEL
                                                             | MB_ICONQUESTION))
        {
            case 2:
            {
                Action = caNone;
                break;
            };
            case 6:
            {
                if (NameSave.Length() != 0) SaveFileClick(Sender);
                else
                {
                    SaveAsFileClick(Sender);
                    if (NameSave.Length() == 0) Action = caNone;
                    else MainForm->CheckWords->ImageIndex = -1;
                };
                break;
            };
            case 7:
            {
                MainForm->CheckWords->ImageIndex = -1;
            };
        };
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckWordsClick(TObject *Sender)
{
    BaseWords = new TBaseWords(1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopFontClick(TObject *Sender)
{
    FontClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopBoldClick(TObject *Sender)
{
    if (TextArea->SelAttributes->Style.Contains(fsBold))
    {
            TextArea->SelAttributes->Style =
                                TextArea->SelAttributes->Style >> fsBold;
    }
    else
            TextArea->SelAttributes->Style =
                                TextArea->SelAttributes->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopItalicClick(TObject *Sender)
{
    if (TextArea->SelAttributes->Style.Contains(fsItalic))
    {
        TextArea->SelAttributes->Style =
                        TextArea->SelAttributes->Style >> fsItalic;
    }
    else
            TextArea->SelAttributes->Style =
                            TextArea->SelAttributes->Style << fsItalic;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopUnderlineClick(TObject *Sender)
{
    if (TextArea->SelAttributes->Style.Contains(fsUnderline))
    {
            TextArea->SelAttributes->Style =
                            TextArea->SelAttributes->Style >> fsUnderline;
    }
    else
            TextArea->SelAttributes->Style =
                            TextArea->SelAttributes->Style << fsUnderline;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopCutClick(TObject *Sender)
{
    TextArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
    TextArea->SelText = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopCopyClick(TObject *Sender)
{
    TextArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PopInsertClick(TObject *Sender)
{
    TextArea->SelText = Clipboard()->AsText;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextAreaMouseWheelUp(TObject *Sender, TShiftState Shift,
      TPoint &MousePos, bool &Handled)
{
    if (Shift.Contains(ssCtrl))
    {
            if (Zoom + 10 <= 5000) Zoom += 10;
            SendMessage(TextArea->Handle, EM_SETZOOM, Zoom, 100);
            int QuanChar = TextArea->Text.Length();
            QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextAreaMouseWheelDown(TObject *Sender, TShiftState Shift,
      TPoint &MousePos, bool &Handled)
{
    if (Shift.Contains(ssCtrl))
    {
            if (Zoom - 10 >= 10) Zoom -= 10;
            SendMessage(TextArea->Handle, EM_SETZOOM, Zoom, 100);
            int QuanChar = TextArea->Text.Length();
            QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextAreaChange(TObject *Sender)
{
    if (ReplaceErrorFlag) {
        if (AddWordT9->ImageIndex == -1) WordsBox->Visible = false;
        ReplaceErrorFlag = false;
    };

    if (!(inizFlag))
    {
        return;
    };

    if ((iPosT9 == 0) || (PosT9 <= 0))
    {
        txt1->Caption = "";
        txt2->Caption = "";
        txt3->Caption = "";
        return;
    };
    txt1->Left = ArrowLeft->Width + 30;
    if (strncmp(&ListWords[PosT9 - 1][0], WordT9, iPosT9) == 0)
        txt1->Caption = &ListWords[PosT9 - 1][0];
    else
        txt1->Caption = "";
    txt2->Left = txt1->Width + txt1->Left + 30;
    if (strncmp(&ListWords[PosT9][0], WordT9, iPosT9) == 0)
        txt2->Caption = &ListWords[PosT9][0];
    else
        txt2->Caption = "";
    txt3->Left = txt2->Width +txt2->Left + 30;
    if ((PosT9 + 1 < lenMas) && (strncmp(&ListWords[PosT9 + 1][0], WordT9, iPosT9) == 0))
        txt3->Caption = &ListWords[PosT9 + 1][0];
    else
        txt3->Caption = "";
    WordsBox->Width = txt3->Left + txt3->Width + 30;

    if (StatusText->Left - 10 < WordsBox->Width)
    {
            WordsBox->Width = StatusText->Left - 10;
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{
    TouchKeyboard->Left = (MainForm->Width - TouchKeyboard->Width) / 2;
    MainForm->Constraints->MinWidth = SearchBox->Width + 375;
    SearchBox->Top = TextArea->Top;
    SearchBox->Left = MainForm->Width - SearchBox->Width - 15;
    SetBoxLayout->Left = (TextArea->Width - SetBoxLayout->Width) / 2;
    SetBoxLayout->Top = SetBox->Height;
    BottomBoxLayout->Left = (TextArea->Width - BottomBoxLayout->Width) / 2;
    BottomBoxLayout->Top = MainForm->Height - BottomBox->Height - 70;
    WordsBox->Width = StatusText->Left - 10;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::TextAreaKeyPress(TObject *Sender, System::WideChar &Key)

{
    if (!(inizFlag))
    {
            return;
    };

    if ((AddWordT9->ImageIndex == 0) && ((Key == ' ') || (Key == '\r') || (iPosT9 > 255)))
    {
            iPosT9 = 0;
            WordT9[0] = 0;
            txt1->Caption = "";
            txt2->Caption= "";
            txt3->Caption = "";
    }
    else
    {
            if (Key == '\b')
            {
                    if (iPosT9 >= 1) iPosT9--;
                    PosT9 = SearchT9(&ListWords, lenMas, iPosT9, WordT9);
            }
            else
            {
                char temp[1];
                int size_needed = WideCharToMultiByte(CP_ACP, 0, &Key, 1, NULL, 0, NULL, NULL);
                WideCharToMultiByte(CP_ACP, 0, &Key, 1, temp, size_needed, NULL, NULL);
                WordT9[iPosT9] = temp[0];
                iPosT9++;
                PosT9 = SearchT9(&ListWords, lenMas, iPosT9, WordT9);
            }
    }
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::ArrowLeftMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ArrowLeft->Font->Style = ArrowLeft->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ArrowLeftMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ArrowLeft->Font->Style = ArrowLeft->Font->Style >> fsBold;
    if ((ReplaceErrorFlag) && (currPosReplace - 1 >= ReplaceLeft)) {
        if ((currPosReplace >= 0) && (currPosReplace < ReplaceRight))
                txt3->Caption = &ReplaceWords[currPosReplace][0];
        if ((currPosReplace - 1 >= 0) && (currPosReplace - 1 < ReplaceRight))
                txt2->Caption = &ReplaceWords[currPosReplace - 1][0];
        currPosReplace--;
        if ((currPosReplace - 1 > 0) && (currPosReplace - 1 < ReplaceRight))
            txt1->Caption = &ReplaceWords[currPosReplace - 1][0];

        txt2->Left = txt1->Width + txt1->Left + 30;
        txt3->Left = txt2->Width +txt2->Left + 30;
        WordsBox->Width = txt3->Left + txt3->Width + 30;

        if (StatusText->Left - 10 < WordsBox->Width)
        {
                WordsBox->Width = StatusText->Left - 10;
        };
    }
    else
        if ((txt1->Caption.Length() > 0) && (txt2->Caption.Length() > 0) && (txt3->Caption.Length() > 0))
        {
            if ((PosT9 >= 0) && (PosT9 < lenMas) && (strncmp(&ListWords[PosT9][0], WordT9, iPosT9) == 0))
                    txt3->Caption = &ListWords[PosT9][0];
            if ((PosT9 - 1 >= 0) && (PosT9 - 1 < lenMas)  && (strncmp(&ListWords[PosT9 - 1][0], WordT9, iPosT9) == 0))
                    txt2->Caption = &ListWords[PosT9 - 1][0];
            PosT9--;
            if ((PosT9 - 1 > 0) && (PosT9 - 1 < lenMas) && (strncmp(&ListWords[PosT9 - 1][0], WordT9, iPosT9) == 0))
                txt1->Caption = &ListWords[PosT9 - 1][0];

            txt2->Left = txt1->Width + txt1->Left + 30;
            txt3->Left = txt2->Width +txt2->Left + 30;
            WordsBox->Width = txt3->Left + txt3->Width + 30;

            if (StatusText->Left - 10 < WordsBox->Width)
            {
                    WordsBox->Width = StatusText->Left - 10;
            };
        }
        else return;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ArrowRightMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ArrowRight->Font->Style = ArrowRight->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ArrowRightMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ArrowRight->Font->Style = ArrowRight->Font->Style >> fsBold;
    if (ReplaceErrorFlag && (currPosReplace + 1 <= ReplaceRight)) {
        if ((currPosReplace >= 0) && (currPosReplace < ReplaceRight))
            txt1->Caption = &ReplaceWords[currPosReplace][0];
        if ((currPosReplace + 1 >= 0) && (currPosReplace + 1 < ReplaceRight))
            txt2->Caption = &ReplaceWords[currPosReplace + 1][0];
        currPosReplace++;
        if ((currPosReplace + 1 < ReplaceRight) && (currPosReplace + 1 > 0))
            txt3->Caption = &ReplaceWords[currPosReplace + 1][0];

        txt2->Left = txt1->Width + txt1->Left + 30;
        txt3->Left = txt2->Width +txt2->Left + 30;
        WordsBox->Width = txt3->Left + txt3->Width + 30;

        if (StatusText->Left - 10 < WordsBox->Width)
        {
                WordsBox->Width = StatusText->Left - 10;
        };
    }
    else
        if ((txt1->Caption.Length() > 0) && (txt2->Caption.Length() > 0) && (txt3->Caption.Length() > 0))
        {
            if ((PosT9 >= 0) && (PosT9 < lenMas) && (strncmp(&ListWords[PosT9][0], WordT9, iPosT9) == 0))
                txt1->Caption = &ListWords[PosT9][0];
            if ((PosT9 + 1 >= 0) && (PosT9 + 1 < lenMas) && (strncmp(&ListWords[PosT9 + 1][0], WordT9, iPosT9) == 0))
                txt2->Caption = &ListWords[PosT9 + 1][0];
            PosT9++;
            if ((PosT9 + 1 < lenMas) && (PosT9 + 1 > 0) && (strncmp(&ListWords[PosT9 + 1][0], WordT9, iPosT9) == 0))
                txt3->Caption = &ListWords[PosT9 + 1][0];

            txt2->Left = txt1->Width + txt1->Left + 30;
            txt3->Left = txt2->Width +txt2->Left + 30;
            WordsBox->Width = txt3->Left + txt3->Width + 30;

            if (StatusText->Left - 10 < WordsBox->Width)
            {
                    WordsBox->Width = StatusText->Left - 10;
            };
        }
        else return;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt1->Font->Style = txt1->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void EnterTxt1()
{
    if (ReplaceErrorFlag)
    {
        MainForm->TextArea->SelText = &MainForm->txt1->Caption[1];
        MainForm->TextArea->SelText = " ";
    }
    else
        if (MainForm->txt1->Caption.Length() > iPosT9)
        {
            MainForm->TextArea->SelText = &MainForm->txt1->Caption[iPosT9 + 1];
            MainForm->TextArea->SelText = " ";
        };
    iPosT9 = 0;
    WordT9[0] = 0;
    MainForm->txt1->Caption = "";
    MainForm->txt2->Caption = "";
    MainForm->txt3->Caption = "";
}
//---------------------------------------------------------------------------

void EnterTxt2()
{
    if (ReplaceErrorFlag)
    {
        MainForm->TextArea->SelText = &MainForm->txt2->Caption[1];
        MainForm->TextArea->SelText = " ";
    }
    else
        if (MainForm->txt2->Caption.Length() > iPosT9)
        {
            MainForm->TextArea->SelText = &MainForm->txt2->Caption[iPosT9 + 1];
            MainForm->TextArea->SelText = " ";
        };
    iPosT9 = 0;
    WordT9[0] = 0;
    MainForm->txt1->Caption = "";
    MainForm->txt2->Caption = "";
    MainForm->txt3->Caption = "";
}
//---------------------------------------------------------------------------

void EnterTxt3()
{
    if (ReplaceErrorFlag)
    {
        MainForm->TextArea->SelText = &MainForm->txt3->Caption[1];
        MainForm->TextArea->SelText = " ";
    }
    else
        if (MainForm->txt3->Caption.Length() > iPosT9)
        {
            MainForm->TextArea->SelText = &MainForm->txt3->Caption[iPosT9 + 1];
            MainForm->TextArea->SelText = " ";
        };
    iPosT9 = 0;
    WordT9[0] = 0;
    MainForm->txt1->Caption = "";
    MainForm->txt2->Caption = "";
    MainForm->txt3->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt1->Font->Style = txt1->Font->Style >> fsBold;
    EnterTxt1();
    ReplaceErrorFlag = false;
    if (AddWordT9->ImageIndex == -1) WordsBox->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt2->Font->Style = txt2->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt2->Font->Style = txt2->Font->Style >> fsBold;
    EnterTxt2();
    ReplaceErrorFlag = false;
    if (AddWordT9->ImageIndex == -1) WordsBox->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt3MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt3->Font->Style = txt3->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::txt3MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    txt3->Font->Style = txt3->Font->Style >> fsBold;
    EnterTxt3();
    ReplaceErrorFlag = false;
    if (AddWordT9->ImageIndex == -1) WordsBox->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextAreaClick(TObject *Sender)
{
    if (ReplaceErrorFlag) {
        if (AddWordT9->ImageIndex == -1) WordsBox->Visible = false;
        ReplaceErrorFlag = false;
    };
    int X,Y,QuanChar;
    X = TextArea->CaretPos.x;
    Y = TextArea->CaretPos.y;
    StatusText->Caption = "Стр. " + IntToStr(Y) + ", стлб. " + IntToStr(X);
    QuanChar = TextArea->Text.Length();
    QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";

    iPosT9 = 0;
    WordT9[0] = 0;
    txt1->Caption = "";
    txt2->Caption= "";
    txt3->Caption = "";
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::TextAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Shift == ShiftEmpty)
    {
        if (!NeedSaveFlag) MainForm->Caption = "*Text editor";
        NeedSaveFlag = true;
    }
    int X,Y,QuanChar;
    X = TextArea->CaretPos.x;
    Y = TextArea->CaretPos.y;
    StatusText->Caption = "Стр. " + IntToStr(Y) + ", стлб. " + IntToStr(X);
    QuanChar = TextArea->Text.Length();
    QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";

    if (Shift.Contains(ssCtrl))
    {
        switch (Key)
        {
                case 49:
                    EnterTxt1();
                    Key = 0;
                break;

                case 50:
                    EnterTxt2();
                    Key = 0;
                break;

                case 51:
                    EnterTxt3();
                    Key = 0;
                break;

                case 70:
                    if ((!SearchBox->Visible) || (HideReplace->Caption == L"⮟"))
                    {
                        ReplaceArea->Enabled = false;
                        LeftPanel->Height = 42;
                        HideReplace->Top = 3;
                        SearchBox->Height = 44;
                        SearchBox->Visible = true;
                        HideReplace->Caption = L"⮞";
                        SearchArea->SetFocus();
                    }
                    else SearchArea->SetFocus();
                break;

                case 72:
                    if ((!SearchBox->Visible) || (HideReplace->Caption == L"⮞"))
                    {
                        ReplaceArea->Enabled = true;
                        SearchBox->Visible = true;
                        LeftPanel->Height = 80;
                        HideReplace->Top = 20;
                        SearchBox->Height = 83;
                        HideReplace->Caption = L"⮟";
                        SearchArea->SetFocus();
                    }
                    else SearchArea->SetFocus();
                break;
        };
    }
    else
        switch (Key)
        {
            case 27:
                ReplaceArea->Enabled = false;
                SearchBox->Visible = false;
                SetSpeaker->Visible = false;
                if (endVoice)
                {
                        pVoice->Skip(L"Sentence", endVoice, 0);
                        endVoice = 0;
                }
            break;

            case 118:
            case 117:
                  time_t rawtime;
                  struct tm * timeinfo;
                  char buffer[80];

                  time (&rawtime);
                  timeinfo = localtime(&rawtime);

                  if (Key == 118)
                    strftime(buffer,sizeof(buffer),"%d.%m.%Y",timeinfo);
                  else
                    strftime(buffer,sizeof(buffer),"%H:%M:%S",timeinfo);
                  std::string str(buffer);
                  TextArea->SelText = &str[0];
            break;
        };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddWordT9Click(TObject *Sender)
{
    if (AddWordT9->ImageIndex == 0)
    {
        AddWordT9->ImageIndex = -1;
        WordsBox->Visible = false;
    }
    else
    {
        AddWordT9->ImageIndex = 0;
        WordsBox->Visible = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuCorrectEnterClick(TObject *Sender)
{
    SwapKeyLanguage();
    if (FlagChangeSelText) ShowMessage("      Текст слишком велик.\n\nВыделите область для замены!");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BottomBoxLayoutClick(TObject *Sender)
{
    if (BottomBoxLayout->Caption == L"⮟" ) {
            BottomBox->Visible = false;
            BottomBox->Height = 0;
            BottomBoxLayout->Caption = L"⮝";
    }
    else
    {
            BottomBox->Visible = true;
            BottomBox->Height = 36;
            BottomBoxLayout->Caption = L"⮟";
    };
    BottomBoxLayout->Left = (TextArea->Width - BottomBoxLayout->Width) / 2;
    BottomBoxLayout->Top = MainForm->Height - BottomBox->Height - 70;
    SearchBox->Top = TextArea->Top;
    SearchBox->Left = MainForm->Width - SearchBox->Width - 15;
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::TextAreaMouseMove(TObject *Sender, TShiftState Shift, int X,
      int Y)
{
    flagBoxExSearch = false;
    ButtSearch->Visible = false;
    ButtReplace->Visible = false;
    ZoomPlus->Visible = false;
    ZoomMinus->Visible = false;

    if (TextArea->Height - 25 < Y) {
        BottomBoxLayout->Visible = true;
    }
    else
    {
        BottomBoxLayout->Visible = false;
    }

    if (Y < 15) {
        SetBoxLayout->Visible = true;
    }
    else
    {
        SetBoxLayout->Visible = false;
    }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetBoxLayoutClick(TObject *Sender)
{
    if (SetBoxLayout->Caption == L"⮝" )
    {
            SetSpeaker->Top = 0;
            SetBox->Visible = false;
            SetBox->Height = 0;
            SetBoxLayout->Caption = L"⮟";
    }
    else
    {
            SetSpeaker->Top = 75;
            SetBox->Visible = true;
            SetBox->Height = 75;
            SetBoxLayout->Caption = L"⮝";
    };
    SetBoxLayout->Left = (TextArea->Width - SetBoxLayout->Width) / 2;
    SetBoxLayout->Top = SetBox->Height;
    SearchBox->Top = TextArea->Top;
    SearchBox->Left = MainForm->Width - SearchBox->Width - 15;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ImagetaLeftClick(TObject *Sender)
{
    TextArea->Paragraph->Alignment = taLeftJustify;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImagetaCenterClick(TObject *Sender)
{
    TextArea->Paragraph->Alignment = taCenter;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImagetaRightClick(TObject *Sender)
{
    TextArea->Paragraph->Alignment = taRightJustify;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageJustifutyClick(TObject *Sender)
{
    PARAFORMAT2 paraformat;

    SendMessage(TextArea->Handle, EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY);
    paraformat.cbSize = sizeof(PARAFORMAT2);
    paraformat.dwMask = PFM_ALIGNMENT;
    paraformat.wAlignment = PFA_JUSTIFY;
    SendMessage(TextArea->Handle, EM_SETPARAFORMAT, 0, Integer(&paraformat));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageFontClick(TObject *Sender)
{
    FontClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageCheckWordsClick(TObject *Sender)
{
    CheckWordsClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageCorrectEntrClick(TObject *Sender)
{
    if (TextArea->Text.Length() > 0) MenuCorrectEnterClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ImageSetUnderlineClick(TObject *Sender)
{
    OKButtonUnderline->Checked = false;
    BoxSetUnderline->Left = (MainForm->Width - BoxSetUnderline->Width) / 2;
    BoxSetUnderline->Top = (MainForm->Height - BoxSetUnderline->Height) / 2;
    BoxSetUnderline->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseUnderlineBoxClick(TObject *Sender)
{
    BoxSetUnderline->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OKUnderlineBoxClick(TObject *Sender)
{
    if (OKButtonUnderline->Checked) BoxSetUnderline->Visible = false;
    ::CHARFORMAT2A cf;
    cf.cbSize = sizeof( cf );
    cf.dwMask = CFM_UNDERLINETYPE;
    cf.bUnderlineType = 0;
    switch (SetColorUnderline->ItemIndex)
    {
            case 0: cf.bUnderlineType = 0; break;
            case 1: cf.bUnderlineType = 0x10; break;
            case 2: cf.bUnderlineType = 0x20; break;
            case 3: cf.bUnderlineType = 0x30; break;
            case 4: cf.bUnderlineType = 0x40; break;
            case 5: cf.bUnderlineType = 0x50; break;
            case 6: cf.bUnderlineType = 0x60; break;
            case 7: cf.bUnderlineType = 0x70; break;
            case 8: cf.bUnderlineType = 0x80; break;
            case 9: cf.bUnderlineType = 0x90; break;
            case 10: cf.bUnderlineType = 0xA0; break;
            case 11: cf.bUnderlineType = 0xB0; break;
            case 12: cf.bUnderlineType = 0xC0; break;
            case 13: cf.bUnderlineType = 0xD0; break;
            case 14: cf.bUnderlineType = 0xE0; break;
            case 15: cf.bUnderlineType = 0xF0; break;
    };

    switch (SetLine->ItemIndex)
    {
            case 0: cf.bUnderlineType = 0x01 | cf.bUnderlineType; break;
            case 1: cf.bUnderlineType = 0x04 | cf.bUnderlineType; break;
            case 2: cf.bUnderlineType = 0x05 | cf.bUnderlineType; break;
            case 3: cf.bUnderlineType = 0x06 | cf.bUnderlineType; break;
            case 4: cf.bUnderlineType = 0x07 | cf.bUnderlineType; break;
            case 5: cf.bUnderlineType = 0x08 | cf.bUnderlineType; break;
            case 6: cf.bUnderlineType = 0x09 | cf.bUnderlineType; break;
    };
    SendMessage(MainForm->TextArea->Handle, EM_SETCHARFORMAT, SCF_SELECTION,
                                                              (LPARAM) &cf);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetUnderlineMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    cX = X;
    cY = Y;
    moveSetUnderline = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetUnderlineMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (moveSetUnderline)
    {
            BoxSetUnderline->Left += X - cX;
            BoxSetUnderline->Top += Y - cY;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetUnderlineMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetUnderline = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CloseHighleghtedBoxClick(TObject *Sender)
{
    BoxSetHighlighted->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageFatPenClick(TObject *Sender)
{
    OKButtonHighlighted->Checked = false;
    BoxSetHighlighted->Left = (MainForm->Width - BoxSetHighlighted->Width) / 2;
    BoxSetHighlighted->Top = (MainForm->Height - BoxSetHighlighted->Height) / 2;
    BoxSetHighlighted->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OKHighleghtedBoxClick(TObject *Sender)
{
    if (OKButtonHighlighted->Checked) BoxSetHighlighted->Visible = false;
    ::CHARFORMAT2A cf;
    cf.cbSize = sizeof( cf );
    cf.dwMask = CFM_BACKCOLOR;
    if (SetColorHighlighted->ItemIndex == 1) cf.crBackColor = TextArea->Color;
    else cf.crBackColor = SetColorHighlighted->Selected;
    SendMessage(TextArea->Handle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetHighlightedMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetHighlighted = true;
    cX = X;
    cY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetHighlightedMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (moveSetHighlighted)
    {
            BoxSetHighlighted->Left += X - cX;
            BoxSetHighlighted->Top += Y - cY;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetHighlightedMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetHighlighted = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ZoomPlusClick(TObject *Sender)
{
    if (Zoom + 10 <= 5000) Zoom += 10;
    SendMessage(TextArea->Handle, EM_SETZOOM, Zoom, 100);
    int QuanChar = TextArea->Text.Length();
    QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ZoomMinusClick(TObject *Sender)
{
    if (Zoom - 10 >= 10) Zoom -= 10;
    SendMessage(TextArea->Handle, EM_SETZOOM, Zoom, 100);
    int QuanChar = TextArea->Text.Length();
    QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetBoxMouseMove(TObject *Sender, TShiftState Shift, int X,
      int Y)
{
    SetBoxLayout->Visible = false;
    flagBoxExSearch = false;
    if ((X >= ImageLoupe->Left) && (X <= ImageLoupe->Left + 80) && (Y <= 50))
    {
            ZoomPlus->Visible = true;
            ZoomMinus->Visible = true;
    }
    else
    {
            ZoomPlus->Visible = false;
            ZoomMinus->Visible = false;
    };

    if ((X >= ImageSearchOrReplace->Left) && (X <= ImageSearchOrReplace->Left + 100) && (Y <= 50))
    {
            ButtSearch->Visible = true;
            ButtReplace->Visible = true;
    }
    else
    {
            ButtSearch->Visible = false;
            ButtReplace->Visible = false;
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageSetUnderlineMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    ZoomPlus->Visible = false;
    ZoomMinus->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageLoupeMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    ZoomPlus->Visible = true;
    ZoomMinus->Visible = true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ImageNumberingClick(TObject *Sender)
{
    BoxSetNumbering->Visible = true;
    BoxSetNumbering->Left = (MainForm->Width - BoxSetNumbering->Width) / 2;
    BoxSetNumbering->Top = (MainForm->Height - BoxSetNumbering->Height) / 2;
    OKButtonNumbering->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageNumberingCloseClick(TObject *Sender)
{
    BoxSetNumbering->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageNumberingOKClick(TObject *Sender)
{
    if (OKButtonNumbering->Checked) BoxSetNumbering->Visible = false;
    ::TParaFormat2 cf;
    cf.cbSize = sizeof( cf );
    cf.dwMask = PFM_NUMBERING | PFM_NUMBERINGSTART | PFM_NUMBERINGSTYLE | PFM_NUMBERINGTAB;
    if (NumWay->ItemIndex == 7)
         cf.wNumbering = 9;
    else
         if (NumWay->ItemIndex != -1) cf.wNumbering = NumWay->ItemIndex;
         else cf.wNumbering = 1;
    if (NumStart->Text.Length() > 0) cf.wNumberingStart = StrToInt(NumStart->Text);
    else cf.wNumberingStart = 1;

    switch (NumStyle->ItemIndex)
    {
            case 0: cf.wNumberingStyle = 0; break;
            case 1: cf.wNumberingStyle = 0x100; break;
            case 2: cf.wNumberingStyle = 0x200; break;
            case 3: cf.wNumberingStyle = 0x300; break;
            case 4: cf.wNumberingStyle = 0x400; break;
            case 5: cf.wNumberingStyle = 0x8000; break;
            default: cf.wNumberingStyle = 0;
    };
    if (NumSpace->Text.Length() > 0) cf.wNumberingTab = StrToInt(NumSpace->Text);
    else cf.wNumberingTab = 0;
    SendMessage(TextArea->Handle, EM_SETPARAFORMAT, 0, LPARAM( &cf ));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetNumberingMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetNumbering = true;
    cX = X;
    cY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetNumberingMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (moveSetNumbering)
    {
            BoxSetNumbering->Left += X - cX;
            BoxSetNumbering->Top += Y - cY;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BoxSetNumberingMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetNumbering = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ImageSetSpaceClick(TObject *Sender)
{
    SetLineBox->Visible = true;
    SetLineBox->Left = (MainForm->Width - SetLineBox->Width) / 2;
    SetLineBox->Top = (MainForm->Height - SetLineBox->Height) / 2;
    OKButtonLine->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageSetLineCloseClick(TObject *Sender)
{
    SetLineBox->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageSetLineOKClick(TObject *Sender)
{
    if (OKButtonLine->Checked) SetLineBox->Visible = false;
    ::TParaFormat2 cf;
    cf.cbSize = sizeof( cf );
    cf.dwMask = PFM_LINESPACING;
    cf.bLineSpacingRule = ListLine->ItemIndex;
    SendMessage(TextArea->Handle, EM_SETPARAFORMAT, 0, LPARAM( &cf ));
    if (RedSpaceLine->Text.Length() > 0)
            TextArea->Paragraph->FirstIndent = StrToInt(RedSpaceLine->Text);
    else
            RedSpaceLine->Text = "0";
    if (SpaceLine->Text.Length() > 0)
            TextArea->Paragraph->LeftIndent = StrToInt(SpaceLine->Text) - StrToInt(RedSpaceLine->Text);
    else
            TextArea->Paragraph->LeftIndent = -StrToInt(RedSpaceLine->Text);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetLineBoxMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetLine = true;
    cX = X;
    cY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetLineBoxMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if (moveSetLine)
    {
            SetLineBox->Left += X - cX;
            SetLineBox->Top += Y - cY;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetLineBoxMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    moveSetLine = false;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::MenuReplaceButClick(TObject *Sender)
{
    if ((lenMas) && (MainForm->TextArea->SelLength > 0))
    {
        ReplaceErrorFlag = true;
        wchar_t temp_t[MainForm->TextArea->SelText.Length() + 1];
        char temp[MainForm->TextArea->SelText.Length() + 1];
        wcscpy(temp_t, &MainForm->TextArea->SelText[1]);
        temp_t[MainForm->TextArea->SelText.Length()] = '\0';
        int size_needed = WideCharToMultiByte(CP_ACP, 0, temp_t, MainForm->TextArea->SelText.Length() + 1, NULL, 0, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, temp_t, MainForm->TextArea->SelText.Length() + 1, temp, size_needed, NULL, NULL);
        if (size_needed > 8) size_needed = 8; // Ограничение поиска. Не думаю что пользователю норм ждать по 30 секунд обработки
        temp[size_needed] = '\0';
        ReplaceLeft = 0;
        ReplaceRight = lenMas;
        ReplaceError(&ListWords, lenMas, size_needed + 1, temp, &ReplaceLeft, &ReplaceRight, &ReplaceWords, lenWords);
        MainForm->txt1->Caption = &ReplaceWords[ReplaceLeft][0];
        currPosReplace = ReplaceLeft;
        if (ReplaceLeft + 1 < ReplaceRight)
        {
            currPosReplace = ReplaceLeft + 1;
            MainForm->txt2->Caption = &ReplaceWords[ReplaceLeft + 1][0];
        };
        if (ReplaceLeft + 2 < ReplaceRight) MainForm->txt3->Caption = &ReplaceWords[ReplaceLeft + 2][0];

        MainForm->txt1->Left = MainForm->ArrowLeft->Width + 30;
        MainForm->txt2->Left = MainForm->txt1->Width + MainForm->txt1->Left + 30;
        MainForm->txt3->Left = MainForm->txt2->Width + MainForm->txt2->Left + 30;
        MainForm->WordsBox->Width = MainForm->txt3->Left + MainForm->txt3->Width + 30;
        if (MainForm->StatusText->Left - 10 < MainForm->WordsBox->Width)
        {
                MainForm->WordsBox->Width = MainForm->StatusText->Left - 10;
        };

        if (MainForm->AddWordT9->ImageIndex == -1) MainForm->WordsBox->Visible = true;
    };
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ImageLoupeClick(TObject *Sender)
{
    Zoom = 100;
    SendMessage(TextArea->Handle, EM_SETZOOM, Zoom, 100);
    int QuanChar = TextArea->Text.Length();
    QuantityWords->Caption = "Симв. " + IntToStr(QuanChar) + "  " + IntToStr(Zoom) + "%";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BuffClick(TObject *Sender)
{
    BuffForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageBuffClick(TObject *Sender)
{
    BuffForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageSearchOrReplaceMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    ButtSearch->Visible = true;
    ButtReplace->Visible = true;
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::SearchCopyClick(TObject *Sender)
{
    SearchArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SearchCutClick(TObject *Sender)
{
    SearchArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
    SearchArea->SelText = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SearchPastClick(TObject *Sender)
{
    SearchArea->SelText = Clipboard()->AsText;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::LeftPanelMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    flagBoxExSearch = true;
    cX = X;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LeftPanelMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    if ((flagBoxExSearch) && (((X - cX > 0) && (SearchBox->Width > 490)) ||
        ((X - cX < 0) && (SearchBox->Width < TextArea->Width - 375))))
    {
            SearchBox->Left += X - cX;
            SearchArea->Width += cX - X;
            ReplaceArea->Width += cX - X;
            SearchBox->Width += cX -X;
            CloseBut->Left = SearchArea->Left + SearchArea->Width + 131 ;
            ViewDown->Left = SearchArea->Left + SearchArea->Width + 85 ;
            FullWord->Left = SearchArea->Left + SearchArea->Width + 32;
            iCaseWord->Left = SearchArea->Left + SearchArea->Width + 7;
            ButReplaceDown->Left = iCaseWord->Left;
            ButReplaceUp->Left = FullWord->Left - 5;
            ButReplaceAll->Left = SearchArea->Left + SearchArea->Width + 52;
            ViewUp->Left = SearchArea->Left + SearchArea->Width + 100;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SearchBoxMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    flagBoxExSearch = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::SearchBoxMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    flagBoxExSearch = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HideReplaceMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    HideReplace->Font->Style = HideReplace->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HideReplaceMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    HideReplace->Font->Style = HideReplace->Font->Style >> fsBold;
    if (HideReplace->Caption == L"⮞")
    {
        LeftPanel->Height = 80;
        HideReplace->Top = 20;
        SearchBox->Height = 83;
        HideReplace->Caption = L"⮟";
        ReplaceArea->Enabled = true;
    }
    else
    {
        ReplaceArea->Enabled = false;
        LeftPanel->Height = 42;
        HideReplace->Top = 3;
        SearchBox->Height = 44;
        HideReplace->Caption = L"⮞";
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ViewUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    ViewUp->Font->Style = ViewUp->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewUpMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    ViewUp->Font->Style = ViewUp->Font->Style >> fsBold;
    SearchWords(SearchArea, TextArea, 0, mySearchTypes);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewDownMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ViewDown->Font->Style = ViewDown->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewDownMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    ViewDown->Font->Style = ViewDown->Font->Style >> fsBold;
    SearchWords(SearchArea, TextArea, 1, mySearchTypes);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseButMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    CloseBut->Font->Style = CloseBut->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseButMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
      int X, int Y)
{
    CloseBut->Font->Style = CloseBut->Font->Style >> fsBold;
    SearchBox->Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ImageSearchOrReplaceMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ReplaceArea->Enabled = false;
    LeftPanel->Height = 42;
    HideReplace->Top = 3;
    SearchBox->Height = 44;
    SearchBox->Visible = true;
    HideReplace->Caption = L"⮞";
    SearchArea->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BottomBoxMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    BottomBoxLayout->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtReplaceMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    ReplaceArea->Enabled = true;
    SearchBox->Visible = true;
    LeftPanel->Height = 80;
    HideReplace->Top = 20;
    SearchBox->Height = 83;
    HideReplace->Caption = L"⮟";
    SearchArea->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SearchAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13)
    {
        if (Shift.Contains(ssShift))
        {
             SearchWords(SearchArea, TextArea, 0, mySearchTypes);
        }
        else
        {
      	     SearchWords(SearchArea, TextArea, 1, mySearchTypes);
        }
    }

    if (Key == 27)
    {
        TextArea->SetFocus();
        ReplaceArea->Enabled = false;
        SearchBox->Visible = false;

    }

    if (Shift.Contains(ssCtrl))
    {
        if (Key == 70)
        {
            ReplaceArea->Enabled = false;
            LeftPanel->Height = 42;
            HideReplace->Top = 3;
            SearchBox->Height = 44;
            SearchBox->Visible = true;
            HideReplace->Caption = L"⮞";
            SearchArea->SetFocus();
        };

        if (Key == 72)
        {
            ReplaceArea->Enabled = true;
            SearchBox->Visible = true;
            LeftPanel->Height = 80;
            HideReplace->Top = 20;
            SearchBox->Height = 83;
            HideReplace->Caption = L"⮟";
            SearchArea->SetFocus();
        };
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ReplaceAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13)
    {
        if (TextArea->SelLength > 0) TextArea->SelText = ReplaceArea->Text;
        if (Shift.Contains(ssShift))
        {
            SearchWords(SearchArea, TextArea, 0, mySearchTypes);
        }
        else
        {
            if (Shift.Contains(ssCtrl))
            {
                TextArea->SelStart = 0;
                bool flag = true;
                while (flag || (TextArea->SelLength > 0))
                {
                    if (TextArea->SelLength > 0) TextArea->SelText = ReplaceArea->Text;
                    SearchWords(SearchArea, TextArea, 1, mySearchTypes);
                    flag = false;
                }
            }
            else
            	SearchWords(SearchArea, TextArea, 1, mySearchTypes);
        }
    }

    if (Key == 27)
    {
        TextArea->SetFocus();
        ReplaceArea->Enabled = false;
        SearchBox->Visible = false;

    };
    if (Shift.Contains(ssCtrl) && (Key == 70))
    {
        ReplaceArea->Enabled = false;
        LeftPanel->Height = 42;
        HideReplace->Top = 3;
        SearchBox->Height = 44;
        SearchBox->Visible = true;
        HideReplace->Caption = L"⮞";
        SearchArea->SetFocus();
    };
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ReplaceCutClick(TObject *Sender)
{
    ReplaceArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuItem2Click(TObject *Sender)
{
    ReplaceArea->CopyToClipboard();
    WriteToClipboard();
    AlignElement();
    ReplaceArea->SelText = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuItem3Click(TObject *Sender)
{
    ReplaceArea->SelText = Clipboard()->AsText;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeechButClick(TObject *Sender)
{
    SetSpeaker->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopSpeakerClick(TObject *Sender)
{
    if (endVoice)
    {
            pVoice->Skip(L"Sentence", endVoice, 0);
            endVoice = 0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VolumeSpeakerChange(TObject *Sender)
{
    SetSpeaker->SetFocus();
    if (endVoice) pVoice->SetVolume(MainForm->VolumeSpeaker->Position);
    if ((MainForm->VolumeSpeaker->Position > 66) &&
        (MainForm->VolumeSpeaker->Position <= 100))
    {
        ImgVolume2->Visible = true;
        ImgVolume1->Visible = false;
        ImgVolume0->Visible = false;
        ImgMute->Visible = false;
    }
    if ((MainForm->VolumeSpeaker->Position > 33) &&
        (MainForm->VolumeSpeaker->Position <= 66))
    {
        ImgVolume2->Visible = false;
        ImgVolume1->Visible = true;
        ImgVolume0->Visible = false;
        ImgMute->Visible = false;
    }
    if ((MainForm->VolumeSpeaker->Position > 0) &&
        (MainForm->VolumeSpeaker->Position <= 33))
    {
        ImgVolume2->Visible = false;
        ImgVolume1->Visible = false;
        ImgVolume0->Visible = true;
        ImgMute->Visible = false;
    }
    if (MainForm->VolumeSpeaker->Position == 0)
    {
        ImgVolume2->Visible = false;
        ImgVolume1->Visible = false;
        ImgVolume0->Visible = false;
        ImgMute->Visible = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseSpeakerMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    CloseSpeaker->Font->Style = CloseSpeaker->Font->Style >> fsBold;
    SetSpeaker->Visible = false;
    if (endVoice)
    {
            pVoice->Skip(L"Sentence", endVoice, 0);
            endVoice = 0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseSpeakerMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    CloseSpeaker->Font->Style = CloseSpeaker->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartSpeakerClick(TObject *Sender)
{
    if (!endVoice) BaseWords = new TBaseWords(2);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StartSpeakerKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

    if (Key == 27) {
        SetSpeaker->Visible = false;
        if (endVoice)
        {
                pVoice->Skip(L"Sentence", endVoice, 0);
                endVoice = 0;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VolumeSpeakerEnter(TObject *Sender)
{
    SetSpeaker->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::iCaseWordMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    if (iCaseWord->Font->Style.Contains(fsBold))
    {
        mySearchTypes = mySearchTypes >> stMatchCase;
        iCaseWord->Font->Style = iCaseWord->Font->Style >> fsBold;
    }
    else
    {
        mySearchTypes = mySearchTypes << stMatchCase;
        iCaseWord->Font->Style = iCaseWord->Font->Style << fsBold;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FullWordMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    if (FullWord->Font->Style.Contains(fsBold))
    {
        mySearchTypes >> stWholeWord;
        FullWord->Font->Style = FullWord->Font->Style >> fsBold;
    }
    else
    {
        mySearchTypes << stWholeWord;
        FullWord->Font->Style = FullWord->Font->Style << fsBold;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceDown->Font->Style = ButReplaceDown->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceDownMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceDown->Font->Style = ButReplaceDown->Font->Style >> fsBold;
        if (TextArea->SelLength > 0) TextArea->SelText = ReplaceArea->Text;
        SearchWords(SearchArea, TextArea, 1, mySearchTypes);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceUp->Font->Style = ButReplaceUp->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceUp->Font->Style = ButReplaceUp->Font->Style >> fsBold;
        if (TextArea->SelLength > 0) TextArea->SelText = ReplaceArea->Text;
        SearchWords(SearchArea, TextArea, 0, mySearchTypes);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceAllMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceAll->Font->Style = ButReplaceAll->Font->Style << fsBold;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButReplaceAllMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
        ButReplaceAll->Font->Style = ButReplaceAll->Font->Style >> fsBold;
        TextArea->SelStart = 0;
        bool flag = true;
        while (flag || (TextArea->SelLength > 0))
        {
            if (TextArea->SelLength > 0) TextArea->SelText = ReplaceArea->Text;
            SearchWords(SearchArea, TextArea, 1, mySearchTypes);
            flag = false;
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddBaseClick(TObject *Sender)
{
        if (IncludeBase->Execute())
        {
                wchar_t temp_t[IncludeBase->FileName.Length() + 1];
                int Pos = 0;
                for (; Pos < IncludeBase->FileName.Length(); Pos++)
                    temp_t[Pos] = IncludeBase->FileName[Pos + 1];
                temp_t[Pos] = '\0';

                int size_needed = WideCharToMultiByte(CP_ACP, 0, temp_t,
                	   IncludeBase->FileName.Length(), NULL, 0, NULL, NULL);

                char temp[size_needed + 1];
                WideCharToMultiByte(CP_ACP, 0, temp_t, IncludeBase->FileName.Length(),
                	                         temp, size_needed, NULL, NULL);
                temp[size_needed] = '\0';

                wchar_t temp__t[locApp.Length() + 1];
                for (Pos = 0; Pos < locApp.Length(); Pos++)
                     temp__t[Pos] = locApp[Pos + 1];
                temp__t[locApp.Length()] = '\0';

                size_needed = WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(),
                						NULL, 0, NULL, NULL);
                char BaseLink[strlen(MainLink) + size_needed + 1];
                WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(), BaseLink,
                				       size_needed, NULL, NULL);

                for (Pos = size_needed; Pos < strlen(MainLink) + size_needed; Pos++)
                     BaseLink[Pos] = MainLink[Pos - size_needed];
                BaseLink[Pos] = '\0';

                WriteNewBase(&ListWords, &lenWords, &lenMas, temp, BaseLink);
        }
}
//---------------------------------------------------------------------------

void IncludeNewWords(String newWord)
{
        if (newWord.Length() == 0) return;
        wchar_t temp_t[newWord.Length() + 1];
        int Pos = 0;
        for (; Pos < newWord.Length(); Pos++)
             temp_t[Pos] = newWord[Pos + 1];
        temp_t[newWord.Length()] = '\0';
        int size_needed = WideCharToMultiByte(CP_ACP, 0, temp_t, newWord.Length(),
                                                        NULL, 0, NULL, NULL);
        char charWord[size_needed + 1];
        WideCharToMultiByte(CP_ACP, 0, temp_t, newWord.Length(), charWord,
                                               size_needed, NULL, NULL);
        charWord[size_needed] = '\0';

        wchar_t temp__t[locApp.Length() + 1];
        Pos = 0;
        for (; Pos < locApp.Length(); Pos++)
             temp__t[Pos] = locApp[Pos + 1];
        temp__t[locApp.Length()] = '\0';

        size_needed = WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(),
                                                        NULL, 0, NULL, NULL);
        char BaseLink[strlen(MainLink) + size_needed + 1];
        WideCharToMultiByte(CP_ACP, 0, temp__t, locApp.Length(), BaseLink,
                                               size_needed, NULL, NULL);

        for (Pos = size_needed; Pos < strlen(MainLink) + size_needed; Pos++)
             BaseLink[Pos] = MainLink[Pos - size_needed];
        BaseLink[Pos] = '\0';

        if (strlen(charWord) > lenWords) lenWords = strlen(charWord) + 1;
        lenMas++;
        ListWords.resize(lenMas, std::vector<char>(lenWords + 1));
        strcpy(&ListWords[lenMas - 1][0], charWord);
        Sort(&ListWords, lenMas, lenWords);
        IncludeNewBase(BaseLink, &ListWords, lenMas);
}

void __fastcall TMainForm::AddWordClick(TObject *Sender)
{
        String str;
        if (TextArea->SelLength > 0)
            str = TextArea->SelText;
        else
            str = (InputBox("Введите новое слово", "Новое слово: ", ""));

        if (str.Length() == 0) return;
        int prevI = 1;
        for (int i = 1; i <= str.Length(); i++)
            if ((str[i] == u' ') && (i < str.Length()) && (str[i + 1] != u' '))
            {
                str[i] = u'\0';
                IncludeNewWords(&str[prevI]);
                prevI = i + 1;
            }
        IncludeNewWords(&str[prevI]);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VirtKeyBoardClick(TObject *Sender)
{
        if (VirtKeyBoard->ImageIndex == 0)
        {
            VirtKeyBoard->ImageIndex = -1;
            KeyBox->Visible = false;
        }
        else
        {
            VirtKeyBoard->ImageIndex = 0;
            KeyBox->Visible = true;
        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuUndoClick(TObject *Sender)
{
	SendMessage(TextArea->Handle, WM_UNDO, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuRedoClick(TObject *Sender)
{
       	SendMessage(TextArea->Handle, EM_REDO, 0, 0);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MenuDelClick(TObject *Sender)
{
        TextArea->SelText = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuInstructionClick(TObject *Sender)
{
        InstructionForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuAboutProgramClick(TObject *Sender)
{
        AboutMainForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NewFileClick(TObject *Sender)
{
        if ((NeedSaveFlag) && (TextArea->Text.Length()))
            switch (MessageBox(MainForm->Handle, L"Сохранить файл?",L"Сохранение",MB_YESNOCANCEL
                                                                 | MB_ICONQUESTION))
            {
                case 6:
                {
                    if (NameSave.Length() != 0) SaveFileClick(Sender);
                    else
                    {
                        SaveAsFileClick(Sender);
                        if (NameSave.Length())
                        	MainForm->CheckWords->ImageIndex = -1;
                    };
                    break;
                };
                case 7:
                {
                    MainForm->CheckWords->ImageIndex = -1;
                }
            };
        NeedSaveFlag = false;
        MainForm->Caption = "Text editor";
        NameSave = "";
        TextArea->Text = "";
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <IdAttachment.hpp>
#include <IdBaseComponent.hpp>
#include <IdMessage.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ActnColorMaps.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Touch.Keyboard.hpp>
//---------------------------------------------------------------------------
class TBaseWords : public TThread
{
private:
        int State;
protected:
	void __fastcall Execute();
public:
        long long Shift = 0;
        long long prevPos = 1;
        long long currPos = 1;
        long long lenText = 0;
        __fastcall TBaseWords(int Thread);
        void __fastcall SearchError();
        void __fastcall SearchPerfect();
        void __fastcall GetPos();
};
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *Menu;
	TMenuItem *File;
	TMenuItem *OpenFile;
	TMenuItem *SaveFile;
	TMenuItem *SaveAsFile;
	TRichEdit *TextArea;
	TSaveDialog *Save;
	TOpenDialog *Open;
	TMenuItem *EncodFormat;
	TMenuItem *FormatASCII;
	TMenuItem *FormatUTF8;
	TMenuItem *FormatAnsi;
	TMenuItem *Separator1;
	TImageList *ImageList;
	TMenuItem *FormatUnicode;
	TMenuItem *Settings;
	TMenuItem *FormStyle;
	TMenuItem *Amakrits;
	TMenuItem *Auric;
	TMenuItem *Carbon;
	TMenuItem *TabletDark;
	TMenuItem *White;
	TMenuItem *Dark;
	TMenuItem *Green;
	TFontDialog *FontDialog;
	TMenuItem *Font;
	TMenuItem *CarryWord;
	TPrintDialog *PrintDialog;
	TPrinterSetupDialog *PrinterSetup;
	TMenuItem *Separator2;
	TMenuItem *SetupPrint;
	TMenuItem *PrintFile;
	TMenuItem *FormatBigEndianUnicode;
	TMenuItem *CheckWords;
	TPopupMenu *PopupMenu;
	TMenuItem *PopFont;
	TMenuItem *PopBold;
	TMenuItem *PopItalic;
	TMenuItem *PopUnderline;
	TMenuItem *PopCut;
	TMenuItem *PopCopy;
	TMenuItem *PopInsert;
	TGroupBox *BottomBox;
	TGroupBox *WordsBox;
	TLabel *StatusText;
	TLabel *StatusEncoding;
	TLabel *QuantityWords;
	TLabel *txt1;
	TLabel *ArrowLeft;
	TLabel *ArrowRight;
	TLabel *txt3;
	TLabel *txt2;
	TMenuItem *N1;
	TMenuItem *AddWordT9;
	TMenuItem *MenuCorrectEnter;
	TButton *BottomBoxLayout;
	TButton *SetBoxLayout;
	TScrollBox *SetBox;
	TImage *ImagetaLeft;
	TImage *ImagetaCenter;
	TImage *ImagetaRight;
	TImage *ImageJustifuty;
	TImage *ImageFont;
	TImage *ImageCheckWords;
	TImage *ImageCorrectEnter;
	TImage *ImageSetUnderline;
	TImage *ImageFatPen;
	TImage *ImageLoupe;
	TImage *ImageNumbering;
	TImage *ImageSetSpace;
	TImage *ImageSearchOrReplace;
	TImage *ButReplaceError;
	TGroupBox *BoxSetUnderline;
	TImage *CloseUnderlineBox;
	TImage *OKUnderlineBox;
	TLabel *HeadTextSetUnderline;
	TLabel *ColorTextSetUnderline;
	TColorBox *SetColorUnderline;
	TLabel *StyleTextSetUnderline;
	TComboBox *SetLine;
	TGroupBox *BoxSetHighlighted;
	TImage *CloseHighleghtedBox;
	TImage *OKHighleghtedBox;
	TLabel *StyleTextHighleghted;
	TLabel *ColorTextSetHighlighted;
	TColorBox *SetColorHighlighted;
	TRadioButton *OKButtonUnderline;
	TRadioButton *OKButtonHighlighted;
	TButton *ZoomPlus;
	TButton *ZoomMinus;
	TGroupBox *BoxSetNumbering;
	TImage *ImageNumberingClose;
	TImage *ImageNumberingOK;
	TLabel *StyleTextSetNumbering;
	TRadioButton *OKButtonNumbering;
	TLabel *NumberingSetBoxTextWay;
	TComboBox *NumWay;
	TLabel *NumberingSetBoxTextStyle;
	TComboBox *NumStyle;
	TLabel *NumberingSetBoxTextStart;
	TEdit *NumStart;
	TLabel *NumberingSetBoxTextSpace1;
	TEdit *NumSpace;
	TLabel *NumberingSetBoxTextSpace2;
	TGroupBox *SetLineBox;
	TImage *ImageSetLineClose;
	TImage *ImageSetLineOK;
	TLabel *LineHeadText;
	TLabel *LineListText1;
	TLabel *LineListText2;
	TComboBox *ListLine;
	TLabel *LineRedSpaceText1;
	TLabel *LineRedSpaceText2;
	TEdit *RedSpaceLine;
	TLabel *LineSpaceText;
	TEdit *SpaceLine;
	TRadioButton *OKButtonLine;
	TMenuItem *N2;
	TMenuItem *MenuReplaceBut;
	TMenuItem *Buff;
	TImage *ImageBuff;
	TMenuItem *N3;
	TButton *ButtSearch;
	TButton *ButtReplace;
	TGroupBox *SearchBox;
	TPopupMenu *SearchMenu;
	TMenuItem *SearchCopy;
	TMenuItem *SearchCut;
	TMenuItem *SearchPast;
	TLabel *HideReplace;
	TPanel *LeftPanel;
	TEdit *SearchArea;
	TEdit *ReplaceArea;
	TLabel *CloseBut;
	TLabel *ViewDown;
	TLabel *ViewUp;
	TPopupMenu *ReplaceMenu;
	TMenuItem *ReplaceCut;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem3;
	TMenuItem *SpeechBut;
	TMenuItem *N5;
	TGroupBox *SetSpeaker;
	TButton *StopSpeaker;
	TTrackBar *VolumeSpeaker;
	TButton *StartSpeaker;
	TLabel *CloseSpeaker;
	TPanel *SpeakerPanel;
	TImage *ImgVolume2;
	TLabel *FullWord;
	TLabel *iCaseWord;
	TLabel *ButReplaceDown;
	TLabel *ButReplaceUp;
	TLabel *ButReplaceAll;
	TImage *AddWord;
	TImage *AddNewBase;
	TOpenDialog *IncludeBase;
	TImage *ImgVolume1;
	TImage *ImgVolume0;
	TImage *ImgMute;
	TGroupBox *KeyBox;
	TTouchKeyboard *TouchKeyboard;
	TMenuItem *N4;
	TMenuItem *VirtKeyBoard;
	TMenuItem *Editing;
	TMenuItem *MenuUndo;
	TMenuItem *MenuRedo;
	TMenuItem *N6;
	TMenuItem *MenuCopy;
	TMenuItem *MenuCut;
	TMenuItem *MenuPast;
	TMenuItem *MenuDel;
	TMenuItem *MenuReference;
	TMenuItem *MenuAboutProgram;
	TMenuItem *N7;
	TMenuItem *MenuInstruction;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *N10;
	TMenuItem *N11;
	TMenuItem *N12;
	TMenuItem *NewFile;
	void __fastcall SaveFileClick(TObject *Sender);
	void __fastcall SaveAsFileClick(TObject *Sender);
	void __fastcall OpenFileClick(TObject *Sender);
	void __fastcall FormatASCIIClick(TObject *Sender);
	void __fastcall FormatUTF8Click(TObject *Sender);
	void __fastcall FormatAnsiClick(TObject *Sender);
	void __fastcall FormatUnicodeClick(TObject *Sender);
	void __fastcall GreenClick(TObject *Sender);
	void __fastcall FontClick(TObject *Sender);
	void __fastcall CarryWordClick(TObject *Sender);
	void __fastcall SetupPrintClick(TObject *Sender);
	void __fastcall PrintFileClick(TObject *Sender);
	void __fastcall FormatBigEndianUnicodeClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CheckWordsClick(TObject *Sender);
	void __fastcall PopFontClick(TObject *Sender);
	void __fastcall PopBoldClick(TObject *Sender);
	void __fastcall PopItalicClick(TObject *Sender);
	void __fastcall PopUnderlineClick(TObject *Sender);
	void __fastcall PopCutClick(TObject *Sender);
	void __fastcall PopCopyClick(TObject *Sender);
	void __fastcall PopInsertClick(TObject *Sender);
	void __fastcall TextAreaMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall TextAreaMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall TextAreaChange(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TextAreaKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ArrowLeftMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ArrowLeftMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ArrowRightMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ArrowRightMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt3MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall txt3MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall TextAreaClick(TObject *Sender);
	void __fastcall TextAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AddWordT9Click(TObject *Sender);
	void __fastcall MenuCorrectEnterClick(TObject *Sender);
	void __fastcall BottomBoxLayoutClick(TObject *Sender);
	void __fastcall TextAreaMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall SetBoxLayoutClick(TObject *Sender);
	void __fastcall ImagetaLeftClick(TObject *Sender);
	void __fastcall ImagetaCenterClick(TObject *Sender);
	void __fastcall ImagetaRightClick(TObject *Sender);
	void __fastcall ImageJustifutyClick(TObject *Sender);
	void __fastcall ImageFontClick(TObject *Sender);
	void __fastcall ImageCheckWordsClick(TObject *Sender);
	void __fastcall ImageCorrectEntrClick(TObject *Sender);
	void __fastcall ImageSetUnderlineClick(TObject *Sender);
	void __fastcall CloseUnderlineBoxClick(TObject *Sender);
	void __fastcall OKUnderlineBoxClick(TObject *Sender);
	void __fastcall BoxSetUnderlineMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall BoxSetUnderlineMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall BoxSetUnderlineMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseHighleghtedBoxClick(TObject *Sender);
	void __fastcall ImageFatPenClick(TObject *Sender);
	void __fastcall OKHighleghtedBoxClick(TObject *Sender);
	void __fastcall BoxSetHighlightedMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BoxSetHighlightedMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall BoxSetHighlightedMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ZoomPlusClick(TObject *Sender);
	void __fastcall ZoomMinusClick(TObject *Sender);
	void __fastcall SetBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ImageSetUnderlineMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall ImageLoupeMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ImageNumberingClick(TObject *Sender);
	void __fastcall ImageNumberingCloseClick(TObject *Sender);
	void __fastcall ImageNumberingOKClick(TObject *Sender);
	void __fastcall BoxSetNumberingMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall BoxSetNumberingMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall BoxSetNumberingMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ImageSetSpaceClick(TObject *Sender);
	void __fastcall ImageSetLineCloseClick(TObject *Sender);
	void __fastcall ImageSetLineOKClick(TObject *Sender);
	void __fastcall SetLineBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SetLineBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall SetLineBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall MenuReplaceButClick(TObject *Sender);
	void __fastcall ImageLoupeClick(TObject *Sender);
	void __fastcall BuffClick(TObject *Sender);
	void __fastcall ImageBuffClick(TObject *Sender);
	void __fastcall ImageSearchOrReplaceMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall SearchCopyClick(TObject *Sender);
	void __fastcall SearchCutClick(TObject *Sender);
	void __fastcall SearchPastClick(TObject *Sender);
	void __fastcall LeftPanelMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall LeftPanelMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall SearchBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SearchBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall HideReplaceMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall HideReplaceMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ViewUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ViewUpMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ViewDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ViewDownMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseButMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseButMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ImageSearchOrReplaceMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BottomBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ButtReplaceMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SearchAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ReplaceAreaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ReplaceCutClick(TObject *Sender);
	void __fastcall MenuItem2Click(TObject *Sender);
	void __fastcall MenuItem3Click(TObject *Sender);
	void __fastcall SpeechButClick(TObject *Sender);
	void __fastcall StopSpeakerClick(TObject *Sender);
	void __fastcall VolumeSpeakerChange(TObject *Sender);
	void __fastcall CloseSpeakerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseSpeakerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall StartSpeakerClick(TObject *Sender);
	void __fastcall StartSpeakerKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VolumeSpeakerEnter(TObject *Sender);
	void __fastcall iCaseWordMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FullWordMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceDownMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceUpMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceAllMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ButReplaceAllMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall AddBaseClick(TObject *Sender);
	void __fastcall AddWordClick(TObject *Sender);
	void __fastcall VirtKeyBoardClick(TObject *Sender);
	void __fastcall MenuUndoClick(TObject *Sender);
	void __fastcall MenuRedoClick(TObject *Sender);
	void __fastcall MenuDelClick(TObject *Sender);
	void __fastcall MenuInstructionClick(TObject *Sender);
	void __fastcall MenuAboutProgramClick(TObject *Sender);
	void __fastcall NewFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "jtalkbcc.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
}
// ---------------------------------------------------------------------------

JTalk *jtalk;

// コンボボックスのメニューを登録する
void SetComboBoxMenu() {
	String pred = (jtalk->Voice)->name;
	TComboBox *box = Form1->ComboBox1;
	if (box != NULL) {
		box->Clear();
	}
	std::vector<JTalk::VoiceFileInfo*>voices = jtalk->Voices;
	for (std::vector<JTalk::VoiceFileInfo*>::iterator it = voices.begin();
	it != voices.end(); it++) {
		box->Items->Add((*it)->name);
	}
	if (pred != "") {
		for (unsigned i = 0; i <= jtalk->Voices.size() - 1; i++) {
			if (pred.CompareIC(jtalk->Voices[i]->path) == 0) {
				box->ItemIndex = i;
				jtalk->VoicePath = pred.c_str();
				return;
			}
		}
	}
	box->ItemIndex = 0;
	if (jtalk->Voices.size() > 0) {
		jtalk->VoicePath = jtalk->Voices[0]->path;
	}
}

// イベントリスナ
class TListener {
public:
	void __fastcall VoiceListChanged();
};

// 音響モデルファイルのリストが更新された後の処理
void __fastcall TListener::VoiceListChanged() {
	SetComboBoxMenu();
}


void __fastcall TForm1::FormCreate(TObject * Sender) {
	jtalk = new JTalk("","","");
	SetComboBoxMenu();
	Memo1->SelStart = 0;
	TListener* listener = new TListener;
	jtalk->OnVoiceListChanged = listener->VoiceListChanged;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject * Sender, TCloseAction & Action) {
	delete jtalk;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject * Sender) {
	jtalk->SpeakAsync(Memo1->Text);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject * Sender) {
	jtalk->Stop();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject * Sender) {
	jtalk->VoicePath = jtalk->Voices[ComboBox1->ItemIndex]->path;
}
// ---------------------------------------------------------------------------


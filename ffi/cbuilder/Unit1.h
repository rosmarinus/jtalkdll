// ---------------------------------------------------------------------------

#ifndef test_bcb_unitH
#define test_bcb_unitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

// ---------------------------------------------------------------------------
class TForm1 : public TForm {
__published: // IDE で管理されるコンポーネント
	TButton *Button1;
	TButton *Button2;
	TMemo *Memo1;
	TComboBox *ComboBox1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);

private: // ユーザー宣言
public: // ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif

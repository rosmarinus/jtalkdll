#include "jtalk.h"
#include <msclr/marshal.h>
#include <functional>

#using < System.dll >
#using < System.Drawing.dll >
#using < System.Windows.Forms.dll >

using namespace System;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace jtalkForm
{

	enum class PressMode
	{
		PlayMode,
		PauseMode,
		ResumeMode,
	};

	public
	ref class Form1 : Form
	{
	private:
		OpenJTalk *oj;
		ComboBox ^ comboBox1;
		RichTextBox ^ richTextBox1;
		Button ^ button1;
		Button ^ button2;
		PressMode pMode;

		using OnFinishedCallbackFunc = void(__stdcall *)();
		delegate void OnFinishedCallbackDelegate();

	public:
		Form1()
		{
			this->oj = openjtalk_initialize(u8"tohoku-f01-neutral", NULL, NULL);

			auto fp = gcnew OnFinishedCallbackDelegate(this, &Form1::onSpeechFinished);
			auto gch = GCHandle::Alloc(fp);
			auto ip = Marshal::GetFunctionPointerForDelegate(fp);
			auto cb = static_cast<OnFinishedCallbackFunc>(ip.ToPointer());
			openjtalk_setOnFinishedCallback(this->oj, cb);

			this->Size = Drawing::Size(250, 160);
			this->MaximumSize = this->Size;
			this->MinimumSize = this->Size;
			this->Text = "発声テスト";

			this->comboBox1 = gcnew ComboBox();
			this->comboBox1->Location = Drawing::Point(8, 5);
			this->comboBox1->Size = Drawing::Size(218, 20);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->SelectedIndexChanged += gcnew EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			this->comboBox1->DropDownStyle = ComboBoxStyle::DropDownList;
			HtsVoiceFilelist *htsvList = openjtalk_getHTSVoiceListU16(this->oj);
			if (htsvList)
			{
				for (HtsVoiceFilelist *list = htsvList; list != NULL; list = list->succ)
				{
					this->comboBox1->Items->Add(gcnew String((wchar_t *)list->nameU16));
				}
				openjtalk_clearHTSVoiceList(this->oj, htsvList);
			}
			char16_t buff[MAX_PATH];
			char16_t *voiceName = openjtalk_getVoiceNameU16(this->oj, buff);
			this->comboBox1->SelectedItem = gcnew String((wchar_t *)voiceName);
			this->Controls->Add(this->comboBox1);

			this->richTextBox1 = gcnew RichTextBox();
			this->richTextBox1->Location = Drawing::Point(8, 32);
			this->richTextBox1->Size = Drawing::Size(218, 50);
			this->richTextBox1->Text = "何か入力してください";
			this->richTextBox1->SelectAll();
			this->richTextBox1->TabIndex = 1;
			this->Controls->Add(this->richTextBox1);
			this->ActiveControl = this->richTextBox1;

			this->button1 = gcnew Button();
			this->button1->Location = Drawing::Point(8, 88);
			this->button1->Size = Drawing::Size(98, 25);
			this->button1->Text = "発声";
			this->button1->TabIndex = 2;
			this->button1->Click += gcnew EventHandler(this, &Form1::button1_Click);
			this->Controls->Add(this->button1);

			this->button2 = gcnew Button();
			this->button2->Location = Drawing::Point(128, 88);
			this->button2->Size = Drawing::Size(98, 25);
			this->button2->Text = "停止";
			this->button2->TabIndex = 3;
			this->button2->Click += gcnew EventHandler(this, &Form1::button2_Click);
			this->Controls->Add(this->button2);

			this->pMode = PressMode::PlayMode;
		}

	private:
		void onSpeechFinished()
		{
			Monitor::Enter(this);
			try
			{
				this->button1->Text = "発声";
				this->pMode = PressMode::PlayMode;
			}
			finally
			{
				Monitor::Exit(this);
			}
		}

		//音響モデルディレクトリが変更されたとき、コンボボックスの項目を再設定する
		void sample_VoiceListChaneged(Object ^ sender, EventArgs ^ e)
		{
			this->comboBox1->Items->Clear();
			HtsVoiceFilelist *htsvList = openjtalk_getHTSVoiceListU16(this->oj);
			if (htsvList)
			{
				for (HtsVoiceFilelist *list = htsvList; list != NULL; list = list->succ)
				{
					this->comboBox1->Items->Add(gcnew String((wchar_t *)list->nameU16));
				}
				openjtalk_clearHTSVoiceList(this->oj, htsvList);
			}
			char16_t buff[MAX_PATH];
			char16_t *voiceName = openjtalk_getVoiceNameU16(this->oj, buff);
			this->comboBox1->SelectedItem = gcnew String((wchar_t *)voiceName);
		}

		void comboBox1_SelectedIndexChanged(Object ^ sender, EventArgs ^ e)
		{
			marshal_context ctx;
			const wchar_t *voice = ctx.marshal_as<const wchar_t *>((String ^) this->comboBox1->SelectedItem);
			openjtalk_setVoiceNameU16(this->oj, (const char16_t *)voice);
		}

		void button1_Click(Object ^ sender, EventArgs ^ e)
		{
			switch (this->pMode)
			{
			case PressMode::PlayMode:
				this->button1->Text = "一時停止";
				this->pMode = PressMode::PauseMode;
				break;
			case PressMode::PauseMode:
				this->button1->Text = "再開";
				this->pMode = PressMode::ResumeMode;
				openjtalk_pause(this->oj);
				return;
			case PressMode::ResumeMode:
				this->button1->Text = "一時停止";
				this->pMode = PressMode::PauseMode;
				openjtalk_resume(this->oj);
				return;
			}
			marshal_context ctx;
			const wchar_t *text = ctx.marshal_as<const wchar_t *>(this->richTextBox1->Text);
			openjtalk_speakAsyncU16(this->oj, (const char16_t *)text);
		}

		void button2_Click(Object ^ sender, EventArgs ^ e)
		{
			this->button1->Text = "発声";
			this->pMode = PressMode::PlayMode;
			;
			openjtalk_stop(this->oj);
		}

		static void Main()
		{
			Application::EnableVisualStyles();
			Application::Run(gcnew Form1());
		}
	};
}

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace JTalkCom;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace FormWithButton
{
    public ref class Form1 : Form
    {
    private:
        JTalkTTS^ tts;
        ComboBox^ comboBox1;
        RichTextBox^ richTextBox1;
        Button^ button1;
        Button^ button2;

    public:
        Form1()
        {
            this->tts = gcnew JTalkTTS(u8"tohoku-f01-neutral");

            // 音響モデルフォルダが変更されたときの処理の登録
            this->tts->VoiceListChanged += gcnew EventHandler(this,&Form1::sample_VoiceListChaneged);

            this->Size = Drawing::Size(250, 160);
            this->MaximumSize = this->Size;
            this->MinimumSize = this->Size;
            this->Text = "発声テスト";

            this->comboBox1 = gcnew ComboBox();
            this->comboBox1->Location = Drawing::Point(8, 5);
            this->comboBox1->Size = Drawing::Size(218, 20);
            this->comboBox1->TabIndex = 0;
            this->comboBox1->SelectedIndexChanged += gcnew EventHandler(this,&Form1::comboBox1_SelectedIndexChanged);
            this->comboBox1->DropDownStyle = ComboBoxStyle::DropDownList;
            for each (VoiceFileInfo^ v in this->tts->Voices) {
                this->comboBox1->Items->Add(v->Name);
            }
            this->comboBox1->SelectedItem = this->tts->Voice->Name;
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
            this->button1->Click += gcnew EventHandler(this,&Form1::button1_Click);
            this->Controls->Add(this->button1);

            this->button2 = gcnew Button();
            this->button2->Location = Drawing::Point(128, 88);
            this->button2->Size = Drawing::Size(98, 25);
            this->button2->Text = "停止";
            this->button2->TabIndex = 3;
            this->button2->Click += gcnew EventHandler(this,&Form1::button2_Click);
            this->Controls->Add(this->button2);
        }

    private:
        //音響モデルディレクトリが変更されたとき、コンボボックスの項目を再設定する
        void sample_VoiceListChaneged(Object^ sender, EventArgs^ e)
        {
            this->comboBox1->Items->Clear();
            for each (VoiceFileInfo^ v in this->tts->Voices)
            {
                this->comboBox1->Items->Add(v->Name);
            }
            this->comboBox1->SelectedItem = this->tts->Voice->Name;
        }

        void comboBox1_SelectedIndexChanged(Object^ sender, EventArgs^ e)
        {
            this->tts->Voice = this->tts->Voices[this->comboBox1->SelectedIndex];
        }
        
        void button1_Click(Object^ sender, EventArgs^ e)
        {
            this->tts->SpeakAsync(this->richTextBox1->Text);
        }

        void button2_Click(Object^ sender, EventArgs^ e)
        {
            this->tts->Stop();
        }

        static void Main()
        {
            Application::EnableVisualStyles();
            Application::Run(gcnew Form1());
        }
    };
}



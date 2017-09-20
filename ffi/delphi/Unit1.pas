unit Unit1;

interface

uses
  Jtalk, Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Memo1: TMemo;
    ComboBox1: TComboBox;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure ComboBox1Change(Sender: TObject);
  private
    { Private 宣言 }
  public
    { Public 宣言 }
  end;

type
  TListener = class
    procedure VoiceListChanged;
  end;

var
  Form1: TForm1;
  tts: JTalkTTS;

implementation

{$R *.dfm}

// コンボボックスにメニューを登録する
procedure SetComboBoxMenu;
var
  temp: PVoiceFileInfo;
  box: TComboBox;
  pred: string;
  i: Integer;
begin
  pred := tts.VoicePath;
  box := Form1.ComboBox1;
  if box <> Nil then
    box.Clear;
  if tts.Voices = nil then
    Exit;

  if tts.Voices.Count > 0 then
  begin
    for temp in tts.Voices do
    begin
      box.Items.Add(temp.name);
    end;

    if pred <> '' then
      for i := 0 to tts.Voices.Count - 1 do
        if tts.Voices[i].path = pred then
        begin
          box.ItemIndex := i;
          Exit;
        end;
    box.ItemIndex := 0;
    tts.VoicePath := tts.Voices[0].path;
  end;
end;

// 音響モデルファイルのリストが更新された後の処理
procedure TListener.VoiceListChanged;
begin
  SetComboBoxMenu
end;

// 発声ボタン
procedure TForm1.Button1Click(Sender: TObject);
begin
  tts.SpeakAsync(string(Memo1.Text));
end;

// 停止ボタン
procedure TForm1.Button2Click(Sender: TObject);
begin
  tts.Stop;
end;

// コンボボックス選択変更
procedure TForm1.ComboBox1Change(Sender: TObject);
begin
  tts.VoicePath := tts.Voices[ComboBox1.ItemIndex].path;
end;

// フォームを閉じる
procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  tts.Free;
end;

// フォームを生成する
procedure TForm1.FormCreate(Sender: TObject);
var
  Listener: TListener;
begin
  tts := JTalkTTS.Create('','','');
  SetComboBoxMenu;
  Listener := TListener.Create;
  tts.OnVoiceListChanged := Listener.VoiceListChanged;
  Memo1.SelStart := 0;
end;

initialization
//Set8087cw($133f);
//Set8087cw($0021);
end.

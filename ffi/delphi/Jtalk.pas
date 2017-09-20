unit Jtalk;

interface

uses
  SysUtils, Generics.Collections;

type
  PHtsVoiceFilelist = ^HtsVoiceFilelist;

  HtsVoiceFilelist = record
    succ: PHtsVoiceFilelist;
    path: PAnsiChar;
    name: PAnsiChar;
  end;

{$IFDEF Win64}
function openjtalk_getHTSVoiceListSjis(oj: Pointer): PHtsVoiceFilelist; stdcall; external 'jtalk64.dll';
procedure openjtalk_clearHTSVoiceList(oj: Pointer; list: PHtsVoiceFilelist); stdcall; external 'jtalk64.dll';

function openjtalk_initializeU16(voice: PChar; dic: PChar; voiceDir: PChar): Pointer; stdcall; external 'jtalk64.dll';
procedure openjtalk_clear(oj: Pointer); stdcall; external 'jtalk64.dll';
procedure openjtalk_refresh(oj: Pointer); stdcall; external 'jtalk64.dll';

procedure openjtalk_setSamplingFrequency(oj: Pointer; i: Cardinal); stdcall; external 'jtalk64.dll';
function openjtalk_getSamplingFrequency(oj: Pointer): Cardinal; stdcall; external 'jtalk64.dll';
procedure openjtalk_setFperiod(oj: Pointer; i: Cardinal); stdcall; external 'jtalk64.dll';
function openjtalk_getFperiod(oj: Pointer): Cardinal; stdcall; external 'jtalk64.dll';
procedure openjtalk_setAlpha(oj: Pointer; f: double); stdcall external 'jtalk64.dll';
function openjtalk_getAlpha(oj: Pointer): double; stdcall external 'jtalk64.dll';
procedure openjtalk_setBeta(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getBeta(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setSpeed(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getSpeed(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setAdditionalHalfTone(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getAdditionalHalfTone(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setMsdThreshold(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getMsdThreshold(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setGvWeightForSpectrum(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getGvWeightForSpectrum(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setGvWeightForLogF0(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getGvWeightForLogF0(oj: Pointer): double; stdcall; external 'jtalk64.dll';
procedure openjtalk_setVolume(oj: Pointer; f: double); stdcall; external 'jtalk64.dll';
function openjtalk_getVolume(oj: Pointer): double; stdcall; external 'jtalk64.dll';

function openjtalk_setDicU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk64.dll';
function openjtalk_getDicU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk64.dll';
function openjtalk_setVoiceDirU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk64.dll';
function openjtalk_getVoiceDirU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk64.dll';
function openjtalk_setVoiceU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk64.dll';
function openjtalk_getVoiceU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk64.dll';
function openjtalk_setVoicePathU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk64.dll';
function openjtalk_getVoicePathU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk64.dll';
function openjtalk_setVoiceNameU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk64.dll';
function openjtalk_getVoiceNameU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk64.dll';

procedure openjtalk_speakSyncU16(oj: Pointer; text: PChar); stdcall; external 'jtalk64.dll';
procedure openjtalk_speakAsyncU16(oj: Pointer; text: PChar); stdcall; external 'jtalk64.dll';
procedure openjtalk_stop(oj: Pointer); stdcall; external 'jtalk64.dll';
function openjtalk_isSpeaking(oj: Pointer): Boolean; stdcall; external 'jtalk64.dll';
procedure openjtalk_waitUntilDone(oj: Pointer); stdcall; external 'jtalk64.dll';
procedure openjtalk_wait(oj: Pointer; duration: Integer); stdcall; external 'jtalk64.dll';
function openjtalk_speakToFileU16(oj: Pointer; text: PChar; filename: PChar): Boolean; stdcall; external 'jtalk64.dll';
procedure openjtalk_test(oj: Pointer; text: Pointer); stdcall; external 'jtalk64.dll';
{$ELSE}
function openjtalk_getHTSVoiceListSjis(oj: Pointer): PHtsVoiceFilelist; stdcall; external 'jtalk32.dll';
procedure openjtalk_clearHTSVoiceList(oj: Pointer; list: PHtsVoiceFilelist); stdcall; external 'jtalk32.dll';

function openjtalk_initializeU16(voice: PChar; dic: PChar; voiceDir: PChar): Pointer; stdcall; external 'jtalk32.dll';
procedure openjtalk_clear(oj: Pointer); stdcall; external 'jtalk32.dll';
procedure openjtalk_refresh(oj: Pointer); stdcall; external 'jtalk32.dll';

procedure openjtalk_setSamplingFrequency(oj: Pointer; i: Cardinal); stdcall; external 'jtalk32.dll';
function openjtalk_getSamplingFrequency(oj: Pointer): Cardinal; stdcall; external 'jtalk32.dll';
procedure openjtalk_setFperiod(oj: Pointer; i: Cardinal); stdcall; external 'jtalk32.dll';
function openjtalk_getFperiod(oj: Pointer): Cardinal; stdcall; external 'jtalk32.dll';
procedure openjtalk_setAlpha(oj: Pointer; f: double); stdcall external 'jtalk32.dll';
function openjtalk_getAlpha(oj: Pointer): double; stdcall external 'jtalk32.dll';
procedure openjtalk_setBeta(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getBeta(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setSpeed(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getSpeed(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setAdditionalHalfTone(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getAdditionalHalfTone(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setMsdThreshold(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getMsdThreshold(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setGvWeightForSpectrum(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getGvWeightForSpectrum(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setGvWeightForLogF0(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getGvWeightForLogF0(oj: Pointer): double; stdcall; external 'jtalk32.dll';
procedure openjtalk_setVolume(oj: Pointer; f: double); stdcall; external 'jtalk32.dll';
function openjtalk_getVolume(oj: Pointer): double; stdcall; external 'jtalk32.dll';

function openjtalk_setDicU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk32.dll';
function openjtalk_getDicU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk32.dll';
function openjtalk_setVoiceDirU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk32.dll';
function openjtalk_getVoiceDirU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk32.dll';
function openjtalk_setVoiceU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk32.dll';
function openjtalk_getVoiceU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk32.dll';
function openjtalk_setVoicePathU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk32.dll';
function openjtalk_getVoicePathU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk32.dll';
function openjtalk_setVoiceNameU16(oj: Pointer; path: PChar): Boolean; stdcall; external 'jtalk32.dll';
function openjtalk_getVoiceNameU16(oj: Pointer; path: PChar): PChar; stdcall; external 'jtalk32.dll';

procedure openjtalk_speakSyncU16(oj: Pointer; text: PChar); stdcall; external 'jtalk32.dll';
procedure openjtalk_speakAsyncU16(oj: Pointer; text: PChar); stdcall; external 'jtalk32.dll';
procedure openjtalk_stop(oj: Pointer); stdcall; external 'jtalk32.dll';
function openjtalk_isSpeaking(oj: Pointer): Boolean; stdcall; external 'jtalk32.dll';
procedure openjtalk_waitUntilDone(oj: Pointer); stdcall; external 'jtalk32.dll';
procedure openjtalk_wait(oj: Pointer; duration: Integer); stdcall; external 'jtalk32.dll';
function openjtalk_speakToFileU16(oj: Pointer; text: PChar; filename: PChar): Boolean; stdcall; external 'jtalk32.dll';
procedure openjtalk_test(oj: Pointer; text: Pointer); stdcall; external 'jtalk32.dll';
{$ENDIF}

type
  PVoiceFileInfo = ^VoiceFileInfo;
  VoiceFileInfo = Record
    path: String;
    name: String;
  end;
  TVoiceList = TList<PVoiceFileInfo>;
  TJTalkEvent = procedure of object;
  TPathBuffer = array [0..259] of char;

type
  JTalkTTS = Class(TObject)

  private
    procedure CheckhHandleObject;
    procedure GenerateVoiceList;

  var
    oj: Pointer;
    FCurrentVoice: VoiceFileInfo;
    FVoiceList: TVoiceList;
    FVoiceListChanged: TJTalkEvent;

  private
    procedure SetAdditionalHalfTone(f: double);
    procedure SetAlpha(f: double);
    procedure SetBeta(f: double);
    procedure SetDic(path: String);
    procedure SetFperiod(i: Cardinal);
    procedure SetGvWeightForLogF0(f: double);
    procedure SetGvWeightForSpectrum(f: double);
    procedure SetMsdThreshold(f: double);
    procedure SetSamplingFrequency(i: Cardinal);
    procedure SetSpeed(f: double);
    procedure SetVoice(path: String); overload;
    procedure SetVoice(voice: PVoiceFileInfo); overload;
    procedure SetVoiceName(name: String);
    procedure SetVoicePath(path: String);
    procedure SetVoiceDir(path: String);
    procedure SetVolume(f: double);

    function GetAdditionalHalfTone: double;
    function GetAlpha: double;
    function GetBeta: double;
    function GetDic: String;
    function GetFperiod: Cardinal;
    function GetGvWeightForLogF0: double;
    function GetGvWeightForSpectrum: double;
    function GetMsdThreshold: double;
    function GetSamplingFrequency: Cardinal;
    function GetSpeed: double;
    function GetVoice: PVoiceFileInfo;
    function GetVoiceName: String;
    function GetVoicePath: String;
    function GetVoiceDir: String;
    function GetVolume: double;

    procedure ClearVoiceList;

  public
    constructor Create(voice:String; dic:String; voiceDir:String);
    destructor Destroy; override;

    procedure SpeakAsync(text: String);
    procedure SpeakSync(text: String);
    procedure SpeakToFile(text: String; filename: String);
    procedure Stop;
    procedure Wait(duration: Integer = 0);
    procedure WaitUntilDone;

    function IsSpeaking: Boolean;

    property A: double read GetAlpha write SetAlpha;
    property AdditionalHalfTone: double read GetAdditionalHalfTone write SetAdditionalHalfTone;
    property Alpha: double read GetAlpha write SetAlpha;
    property B: double read GetBeta write SetBeta;
    property Beta: double read GetBeta write SetBeta;
    property Dic: String read GetDic write SetDic;
    property FM: double read GetAdditionalHalfTone write SetAdditionalHalfTone;
    property Fperiod: Cardinal read GetFperiod write SetFperiod;
    property G: double read GetVolume write SetVolume;
    property GvWeightForLogF0: double read GetGvWeightForLogF0 write SetGvWeightForLogF0;
    property GvWeightForSpectrum: double read GetGvWeightForSpectrum write SetGvWeightForSpectrum;
    property JF: double read GetGvWeightForLogF0 write SetGvWeightForLogF0;
    property JM: double read GetGvWeightForSpectrum write SetGvWeightForSpectrum;
    property MsdThreshold: double read GetMsdThreshold write SetMsdThreshold;
    property P: Cardinal read GetFperiod write SetFperiod;
    property R: double read GetSpeed write SetSpeed;
    property S: Cardinal read GetSamplingFrequency write SetSamplingFrequency;
    property SamplingFrequency: Cardinal read GetSamplingFrequency write SetSamplingFrequency;
    property Speed: double read GetSpeed write SetSpeed;
    property U: double read GetMsdThreshold write SetMsdThreshold;
    property Voice: PVoiceFileInfo read GetVoice write SetVoice;
    property VoicePath: String read GetVoicePath write SetVoicePath;
    property VoiceName: String read GetVoiceName write SetVoiceName;
    property Voices: TList<PVoiceFileInfo> read FVoiceList;
    property VoiceDir: String read GetVoiceDir write SetVoiceDir;
    property Volume: double read GetVolume write SetVolume;

    // event
    property OnVoiceListChanged: TJTalkEvent read FVoiceListChanged write FVoiceListChanged;

  end;

implementation

  // 音声リストの作成
  procedure JTalkTTS.GenerateVoiceList;
  var
    voiceList: PHtsVoiceFilelist;
    list: PHtsVoiceFilelist;
    temp: PVoiceFileInfo;
  begin
    CheckhHandleObject;
    if Voices <> Nil then
      ClearVoiceList;
    voiceList := openjtalk_getHTSVoiceListSjis(oj);
    list := voiceList;
    FVoiceList := TVoiceList.Create;
    while list <> Nil do
    begin
      new(temp);
      temp.path := String(list.path);
      temp.name := String(list.name);
      FVoiceList.Add(temp);
      list := list.succ;
    end;
    openjtalk_clearHTSVoiceList(oj, voiceList);
  end;

  // コンストラクタ
  constructor JTalkTTS.Create(voice:String; dic:String; voiceDir:String);
  begin
    oj := openjtalk_initializeU16(PChar(voice),PChar(dic),PChar(voiceDir));
    CheckhHandleObject;
    GenerateVoiceList;
  end;

  // デストラクタ
  destructor JTalkTTS.Destroy;
  begin
    openjtalk_stop(oj);
    openjtalk_clear(oj);
    ClearVoiceList;
    inherited;
  end;

  procedure JTalkTTS.ClearVoiceList;
  var
    temp: PVoiceFileInfo;
  begin
    if FVoiceList <> nil then
      for temp in FVoiceList do
      begin
        temp^.name := '';
        temp^.path := '';
        Dispose(temp);
      end;
  end;

  // オブジェクトの存在確認
  procedure JTalkTTS.CheckhHandleObject;
  begin
    if oj = NIL then
      raise Exception.Create('Internal Error: OpenJTalk Object pointer is NULL');
  end;

  // サンプリング周波数
  procedure JTalkTTS.SetSamplingFrequency(i: Cardinal);
  begin
    CheckhHandleObject;
    if i < 1 then
      raise Exception.Create('sampling frequency（1以上の整数）が範囲外です: ' + IntToStr(i));
    openjtalk_setSamplingFrequency(oj, i);
  end;

  function JTalkTTS.GetSamplingFrequency: Cardinal;
  begin
    CheckhHandleObject;
    Result := openjtalk_getSamplingFrequency(oj);
  end;

  // フレームピリオド
  procedure JTalkTTS.SetFperiod(i: Cardinal);
  begin
    CheckhHandleObject;
    if i < 1 then
      raise Exception.Create('frame period（1以上の整数）が範囲外です: ' + IntToStr(i));
    openjtalk_setFperiod(oj, i);
  end;

  function JTalkTTS.GetFperiod: Cardinal;
  begin
    CheckhHandleObject;
    Result := openjtalk_getFperiod(oj);
  end;

  // オールパス値
  procedure JTalkTTS.SetAlpha(f: double);
  begin
    CheckhHandleObject;
    if (f < 0.0) or (f > 1.0) then
      raise Exception.Create('all-pass constant（0と1の間の実数）が範囲外です: ' + FloatToStr(f));
    openjtalk_setAlpha(oj, f);
  end;

  function JTalkTTS.GetAlpha: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getAlpha(oj);
  end;

  // ポストフィルター係数
  procedure JTalkTTS.SetBeta(f: double);
  begin
    CheckhHandleObject;
    if (f < 0.0) or (f > 1.0) then
      raise Exception.Create('postfiltering coefficient（0と1の間の実数）が範囲外です: ' + FloatToStr(f));
    openjtalk_setBeta(oj, f);
  end;

  function JTalkTTS.GetBeta: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getBeta(oj);
  end;

  // スピーチ速度
  procedure JTalkTTS.SetSpeed(f: double);
  begin
    CheckhHandleObject;
    if f < 0.0 then
      raise Exception.Create('speech speed rate（0以上の実数） が範囲外です: ' + FloatToStr(f));
    openjtalk_setSpeed(oj, f);
  end;

  function JTalkTTS.GetSpeed: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getSpeed(oj);
  end;

  // 追加ハーフトーン
  procedure JTalkTTS.SetAdditionalHalfTone(f: double);
  begin
    CheckhHandleObject;
    openjtalk_setAdditionalHalfTone(oj, f);
  end;

  function JTalkTTS.GetAdditionalHalfTone: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getAdditionalHalfTone(oj);
  end;

  // 有声 / 無声境界値
  procedure JTalkTTS.SetMsdThreshold(f: double);
  begin
    CheckhHandleObject;
    if (f < 0.0) or (f > 1.0) then
      raise Exception.Create('voiced/unvoiced threshold（0と1の間の実数）が範囲外です: ' + FloatToStr(f));
    openjtalk_setMsdThreshold(oj, f);
  end;

  function JTalkTTS.GetMsdThreshold: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getMsdThreshold(oj);
  end;

  // スペクトラム系列内変動の重み
  procedure JTalkTTS.SetGvWeightForSpectrum(f: double);
  begin
    CheckhHandleObject;
    openjtalk_setGvWeightForSpectrum(oj, f);
  end;

  function JTalkTTS.GetGvWeightForSpectrum: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getGvWeightForSpectrum(oj);
  end;

  // F0系列内変動重み
  procedure JTalkTTS.SetGvWeightForLogF0(f: double);
  begin
    CheckhHandleObject;
    if f < 0.0 then
      raise Exception.Create('weight of GV for spectrum（0以上の実数）が範囲外です: ' + FloatToStr(f));
    openjtalk_setGvWeightForLogF0(oj, f);
  end;

  function JTalkTTS.GetGvWeightForLogF0: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getGvWeightForLogF0(oj);
  end;

  // ボリューム
  procedure JTalkTTS.SetVolume(f: double);
  begin
    CheckhHandleObject;
    openjtalk_setVolume(oj, f);
  end;

  function JTalkTTS.GetVolume: double;
  begin
    CheckhHandleObject;
    Result := openjtalk_getVolume(oj);
  end;

  // 辞書ディレクトリ指定(UTF-16)
  procedure JTalkTTS.SetDic(path: String);
  begin
    CheckhHandleObject;
    if path = '' then
      raise Exception.Create('辞書フォルダを示す文字列が空です');
    if not DirectoryExists(path) then
      raise Exception.Create('辞書フォルダが見つかりません: ''' + path + '''');
    if not openjtalk_setDicU16(oj, PChar(path)) then
      raise Exception.Create('辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: ''' + path + '''');
  end;

  function JTalkTTS.GetDic: String;
  var buffer: TPathBuffer;
  begin
    CheckhHandleObject;
    Result := openjtalk_getDicU16(oj,@buffer);
  end;

  // 音響モデルファイルディレクトリ指定(UTF-16)
  procedure JTalkTTS.SetVoiceDir(path: String);
  begin
    CheckhHandleObject;
    if path = '' then
      raise Exception.Create('音響モデルフォルダを示す文字列が空です');
    if not DirectoryExists(path) then
      raise Exception.Create('音響モデルフォルダが見つかりません: ''' + path + '''');
    if not openjtalk_setVoiceDirU16(oj, PChar(path)) then
      raise Exception.Create('音響モデルフォルダを設定できません: ''' + path + '''');
    GenerateVoiceList;
  end;

  function JTalkTTS.GetVoiceDir: String;
  var buffer: TPathBuffer;
  begin
    CheckhHandleObject;
    Result := openjtalk_getVoiceDirU16(oj,@buffer);
  end;

  // 音響モデルファイル指定(UTF-16)
  // 絶対パス...直接、
  // 相対パス...音響モデルファイルディレクトリ基準、
  // 名前のみ...探索
  procedure JTalkTTS.SetVoice(path: String);
  begin
    CheckhHandleObject;
    if path = '' then
      raise Exception.Create('音響モデルを示す文字列が空です');
    if not openjtalk_setVoiceU16(oj, PChar(voice)) then
      raise Exception.Create('音響モデルを設定できません: ''' + path + '''');
  end;

  procedure JTalkTTS.SetVoice(voice: PVoiceFileInfo);
  var path: String;
  begin
    CheckhHandleObject;
    if voice = nil then
      raise Exception.Create('音響モデルを示す情報がNULLです');
    path := voice.path;
    if path = '' then
      raise Exception.Create('音響モデルを示す文字列が空です');
    if not openjtalk_setVoiceU16(oj, PChar(voice)) then
      raise Exception.Create('音響モデルを設定できません: ''' + path + '''');
  end;

  function JTalkTTS.GetVoice: PVoiceFileInfo;
  var bufferPath, bufferName: TPathBuffer;
  var path,name: String;
  begin
    CheckhHandleObject;
    path := openjtalk_getVoicePathU16(oj,@bufferPath);
    name := openjtalk_getVoiceNameU16(oj,@bufferName);
    FCurrentVoice.path := path;
    FCurrentVoice.name := name;
    Result := @FCurrentVoice;
  end;

  procedure JTalkTTS.SetVoicePath(path: String);
  begin
    CheckhHandleObject;
    if path = '' then
      raise Exception.Create('音響モデルを示す文字列が空です');
    if not openjtalk_setVoicePathU16(oj, PChar(path)) then
      raise Exception.Create('音響モデルを設定できません: ''' + path + '''');
  end;

  function JTalkTTS.GetVoicePath: String;
  var buffer: TPathBuffer;
  begin
    CheckhHandleObject;
    Result := openjtalk_getVoicePathU16(oj,@buffer);
  end;

  procedure JTalkTTS.SetVoiceName(name: String);
  begin
    CheckhHandleObject;
    if name = '' then
      raise Exception.Create('音響モデルを示す文字列が空です');
    if not openjtalk_setVoiceNameU16(oj, PChar(name)) then
      raise Exception.Create('音響モデルを設定できません: ''' + name + '''');
  end;

  function JTalkTTS.GetVoiceName: String;
  var buffer: TPathBuffer;
  begin
    CheckhHandleObject;
    Result := openjtalk_getVoiceNameU16(oj,@buffer);
  end;

  // 同期発声(UTF-16)
  procedure JTalkTTS.SpeakSync(text: String);
  begin
    openjtalk_speakSyncU16(oj, PChar(text));
  end;

  // 非同期発声(UTF-16)
  procedure JTalkTTS.SpeakAsync(text: String);
  begin
    openjtalk_speakAsyncU16(oj, PChar(text));
  end;

  // 発声の強制停止
  procedure JTalkTTS.Stop;
  begin
    CheckhHandleObject;
    openjtalk_stop(oj);
  end;

  // 発声中かどうか
  function JTalkTTS.IsSpeaking: Boolean;
  begin
    CheckhHandleObject;
    Result := openjtalk_isSpeaking(oj);
  end;

  // 発声している間待機する
  procedure JTalkTTS.WaitUntilDone;
  begin
    CheckhHandleObject;
    openjtalk_waitUntilDone(oj);
  end;

  // 指定時間待機する
  procedure JTalkTTS.Wait(duration: Integer = 0);
  begin
    CheckhHandleObject;
    if duration = 0 then
      WaitUntilDone
    else
      openjtalk_wait(oj, duration);
  end;

  // 指定ファイルに音声を記録する(UTF-16)
  procedure JTalkTTS.SpeakToFile(text: String; filename: String);
  begin
    CheckhHandleObject;
    if filename = '' then
      raise Exception.Create('ファイル名文字列が空です');
    if not openjtalk_speakToFileU16(oj, PChar(text), PChar(filename)) then
      raise Exception.Create('音声ファイルの作成中にエラーが発生しました');
  end;

end.

# jtalkdll

OpenJTalk DLL 

これは音声合成システムOpen JTalk使って手軽に音声合成プログラミングするためのDLLです。記述言語はC言語です。

PortAudioを使って、マルチプラットフォームでの音声出力を行います。
Windowsでは、WSH（ウィンドウズスクリプトホスト）やC#言語等で利用しやすいようにC++/CLIでラップしたマネージDLLも生成します。
また、他のプログラミング言語から使いやすくするために、それぞれの言語のFFI(多言語関数インターフェイス)を利用するコードをいくつか用意しています。

このリポジトリのファイルには、オリジナルなファイル以外に、いくつかの修正を加えた実行に必要な次のものを含んでいます。
利用はそれぞれのライセンスに従います。
なお、open_jtalk, hts_engine_API, PortAudio をスタティックライブラリとして取り込んでいます。

* [open_jtalk-1.10](http://open-jtalk.sourceforge.net/)
* [open_jtalk_dic_utf_8-1.10](http://open-jtalk.sourceforge.net/)
* [hts_voice_nitech_jp_atr503_m001-1.05](http://open-jtalk.sourceforge.net/)
* [hts_engine_API-1.10](http://hts-engine.sourceforge.net/)
* [htsvoice-tohoku-f01](https://github.com/icn-lab/htsvoice-tohoku-f01)
* [mei (MMDAgent)](http://www.mmdagent.jp/)
* [PortAudio](http://www.portaudio.com/)

また、Javaから利用するサンプルでは、[jna](https://github.com/java-native-access/jna) を用いています。

## 動作環境

* Windows 10
* macOS Sierra
* Ubuntu (他のLinuxディストリビューションは未確認)

## ビルド

基本的にコマンドラインでCMakeを使ってビルドします。

### Windows

マイクロソフト Visual Studio C++ (以下MSVC)によるビルド方法と、CygwinもしくはMSYS2上のMinGW-W64のコンパイラによるビルド方法を以下に示します。

#### MSVCを使ったビルド

必要なもの：

* Visual Studio 2017 Comunity　Edition もしくは Build Tools for Visual Studio 2017
* git （なくてもいいけどあったほうが便利）

[マイクロソフトのVisual Studioダウンロードサイト](https://www.visualstudio.com/ja/downloads/)から無償の Visual Studio　2017 Comunity　Edition もしくは Build Tools for Visual Studio 2017をダウンロードし、インストールしておく必要があります。前者は高機能な統合開発環境、後者はC/C++のコードのコンパイルだけを目的とした最小限の構成のビルドツールです。今回のコードのコンパイルはコマンドラインからだけの作業だけなのでこのビルドツールでも十分です。
(※Build Tools for Visual Studio 2017 のダウンロードボタンの場所が分かりにくいが、ページの最後の方の「そのツール及びフレームワーク」のところにある。)

gitは、このリポジトリからソースをコピーするために必要ですが、無ければ[ZIPファイル](https://github.com/rosmarinus/jtalkdll/archive/master.zip)をダウンロードし展開すればいいので必須ではありません。
Windowsでコマンドラインからgitを利用できるようにするにはいくつかの方法が考えられますが、おすすめはパッケージ管理の[Chocolatey](https://chocolatey.org/install)を入れて、gitパッケージを``cinst git``でインストールする方法です。

#### インストール手順

スタートメニューを開き、 Visual Studio 2017 フォルダ内にある 「x64 Native Tools Command Prompt for VS 2017」あるいは「x86 Native Tools Command Prompt for VS 2017」を起動します。どちらを使うかは64版か32版を作るかどうかで決めます。適当なフォルダをカレントフォルダに決めます。

次のコマンドで、このリポジトリをダウンロードします。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
```

gitコマンドではなく、ZIPファイルでダウンロードした場合は、適当な場所にそれを展開します。そして、そのjtaldllフォルダをコマンドプロンプトのカレントディレクトリにします。

次にjtalkdllフォルダ内のbuild.dirをフォルダをカレントディレクトリにして、次のコマンドをエンターすると、ビルドが開始されます。このスクリプトの内部ではcmakeとnmakeを実行しています。

```DOS:
build
```

* Windowsでビルドした場合の生成物は、c:\open_jtalk フォルダに出力されます。Windowsで今後jtalk.dllを利用する場合はシステムの詳細設定で環境変数PATHに c:\open_jtalk\bin を追加してください。

* build に引数として true を渡すと、jtalkcom.dllを生成します。ただしここで生成されるのは遅延署名されたアセンブリになります。実際の利用は配布版のjtalkcom.dllを使ってください。なお、ビルドするためには、Visual Studio のインストーラを使って C++/Cli サポートをインストールしておく必要があります。

* WindowsのCMakeではvcxprojファイルを出力し、MSBuildでビルドする方式もあります。この方がvcxprojファイルが生成されるので、Visual Studioでのデバッグなどには都合がいいです。しかしここではインストールまで自動化させるため nmake を使う方法にしています。vcxprojを出力する方法はbuild.batの中でコメントアウトしているコマンドを参考にしてください。

#### MSYS2 の MinGW-W64 を使ったビルド

#### 準備

* msys2のインストール

 [MSYS2 homepage](http://www.msys2.org/) から、msys2-i686-xxxxxxxx.exe （xxxxxxxxは日付の数列）もしくは msys2-x86_64-xxxxxxxx.exe をダウンロードします。それぞれ32ビットと64ビット版です。64ビット版では32ビットアプリの開発もできるので、特にこだわりがない限り、64ビット版がいいでしょう。セットアップの方法は割愛します。 セットアップが終わったら、jtalkdllの生成のためには次のパッケージを追加でインストールします。git, base-devel, mingw-w64-i686-toolchain, mingw-w64-x86_64-toolchain。最後の2つは32ビット版、64ビット版のビルドコマンド群なので、両方もしくは必要な方を入れてください。

```bash:
pacman -S git base-devel mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain
```

#### ビルド方法

環境が整ったら、開発用のコンソールを開きます。64bit版を作りたいときは、Windowsのスタート画面からMSYS2 MinGW 64-bit、32bit版ならばMSYS2 MinGW 32-bitです。
そして、カレントディレクトリを適当な場所に切り替えて、次の一連のコマンドを実行すると、dllが生成されます。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

なお、音声を再生するために利用しているPortAudioがCMakeではうまくmingw用のスタティックライブラリを生成できなかったので、congifureを一箇所改変（後述）して、CMmakeに先だってライブラリを生成させています。

生成物は MSVCと同じ場所、c:\open_jtalkフォルダに出力されます。Windowsで今後jtalk.dllを利用する場合はシステムの詳細設定で環境変数PATHにc:\open_jtalk\binを追加してください。

#### Cygwin の MinGW-W64 を使ったビルド

##### 準備

* Cygwin / Cygwin64 のインストール

 [Cygwin Installation](https://www.cygwin.com/install.html) から、setup-x86_64.exe もしくは setup-x86.exe をダウンロードします。
 32ビットマシンではない限り setup-x86_64.exeの方をインストールした方がいいでしょう。セットアップの方法は割愛します。

* 必要なパッケージのインストール

 セットアップが完了したら、dllの生成のために次のパッケージを追加でインストールしてください。ファイル名にi686とx86_64が含まれているそれぞれ二つのパッケージは32bit版、64ビット版をコンパイルするための実行ファイル群です。必要に応じて加減してください。
  cmake, make, git, gcc-core, gcc-g++, mingw64-i686-gcc-core, mingw64-i686-gcc-g++, mingw64-x86_64-gcc-core, mingw64-x86_64-gcc-g++

#### ビルド方法

環境が整ったら、次の一連のコマンドでリポジトリからソースファイルをコピーし、ビルドを開始します。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

Cygwinだと32bit版、Cygwin64だと64bitのdllが生成されます。64ビットマシーンでbuildスクリプトに引数として i686 か x86_64 を渡すと、指定したものを作ります。

なお、音声を再生するために利用しているPortAudioがCMakeではうまくmingw用のスタティックライブラリを生成できなかったので、congifureでCMmakeに先だってライブラリを生成させています。

生成物は MSVCと同じ場所、c:\open_jtalkフォルダに出力されます。Windowsで今後jtalk.dllを利用する場合はシステムの詳細設定で環境変数PATHにc:\open_jtalk\binを追加してください。

### macOS

#### 準備

* Xcodeのインストール
* Homebrewのインストール

macOSでプログラムの開発をするには、Xcodeがインストールされていなくてはいけません。統合環境としてのXcodeを利用しないこのプロジェクトも例外ではありません。Xcodeのインストール方法は割愛します。

macOS用のパッケージマネージャーHomebrewもインストールしておくと便利です。インストールの仕方と、使い方は、[Homebrew](https://brew.sh/index_ja.html)にあります。これも知っているものとしてすすめます。

（インストールすべきコマンドのリストアップ 記述予定）

#### ビルド

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

### ビルド結果

* jtalk.dll / libjtalk.so / libjtalk.dylib

* jtalk32COM.dll / jtalk64COM.dll

ビルドによる生成物は、以下のフォルダにインストールされます。

#### 実行ファイル

* Windows

c:\open_jtalk\bin\

* Linux / macOS

/usr/local/bin/

#### データファイル

音響モデルファイル
辞書データ

* Windows

c:\open_jtalk\voice\
c:\open_jtalk\dic\

* Linux / macOS

/usr/local/OpenJTalk/

### Ubuntu

以下に、Ubuntuでのビルド方法を示します。
他のlinuxディストリビューションでも同様の処理を行えば、生成できるでしょう。

#### 準備

portaudio で必要になるので、あらかじめ次のコマンドでALSAのSDKをインストールしておきます。

```bash:
sudo apt install libasound-dev
```

あとは次のコマンドで、ソースファイルをコピーして、ビルドします。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

## 動作確認

各プラットフォームにおいて上記の方法でビルドが成功したら、以下の方法で動作確認ができます。

次のコマンドをタイプしエンターしてください。うまくビルドできていれば、一緒にインストールしてあるhtsvoiceファイルをランダムに選んで言葉をしゃべってくれます。
Windowsの場合、c:\open_jtalk\binをPATHに追加していないときは、コマンドプロンプトを開いてカレントディレクトリをc:\open_jtalk\binフォルダにしてから実行してください。

```bash:
jtd_c
```

このコマンドのソースはjtalkフォルダにあるjtd_c.cで、C言語で書いたこの共有ライブラリのサンプルです。

次に、下のコマンドのように パラメータに日本語を入力して、エンターしてください。うまくいけば、その言葉をしゃべります。

```bash:
jsay -v mei_normal こんにちは
```

jsayはmacOSのsayコマンドに動作を似せたサンプルで、このソースもjtalkフォルダにあります。
-v?オプションで利用できる音声のリストが出ますので、それを参考にして音声を指定してください。
-oファイル名 オプションでファイルへの書き出しもできます。
文字列が指定されていないと、標準入力からの入力を待ちます。
エンターを連続して入力するまで続きます。

## 簡単な仕組みの説明

本来の open_jtalk.c ファイルは、エクスポートしないスタティックライブラリの定義にmain関数をくっつけたような構成になっています。
これをほとんどそのまま利用して、それにwavファイルに含まれる音声データをmallocして返す関数を追加したものを openjtalk.c とします。
今度はそれをjtalk.cでラップしてインターフェイス(API)を定義します。
jtalk.cは内部で portaudio を使うことでマルチプラットフォームでの非同期再生を実現しています。

## APIの簡単な説明

jtalk.cではopen_jtalkを利用するのに便利な関数をいくつか定義しています。
ここで定義されているAPI関数には接頭語'openjtalk_'をつけています。

本来の open_jtalk で指定できるパラメータの設定と取得ができます。
設定はset、取得はgetの文字列が名前に含まれます。
パラメータの名前はコマンドラインで指定する短い名前とその意味を反映した長い名前の両方を用意しています。

文字列を入出力するAPIにおける文字コードは基本的にUTF-8です。内部で使っているmecabにもUTF-8文字列を渡しています。
ただし、プログラミング言語によっては16ビットのUnicodeを文字コードとして使っているものもあり、UTF-16LEの文字列も関数の引数、戻り値として使えるようにしています。これらには名前の末尾にU16が付いています。
またWindowsではコマンドプロンプトの出力などShift-JIS（CP932）を使わざるを得ない場合もあるので、Shift-JISも関数の引数、戻り値として使えるようにしています。こちらには名前の末尾にSjisが付いています。

Utf-16leを扱う文字の型はchar16_t、文字列の型はchar16_t*を使っています。
Windowsの wchar_t は厳密には同じ型ではありませんが、内部では同じものとして使っています。
サロゲートペア文字列などへの対応は今後の課題とします。

### 主要なAPI

APIについての詳しい内容は [jtalk.h](https://github.com/rosmarinus/jtalkdll/blob/master/jtalk/jtalk.h) を直接見てください。

この中で主要なものを抜き出してみます。

```C:
// 初期化
OpenJTalk *openjtalk_initialize(const char *voice, const char *dic, const char *voiceDir);

// 後始末
void openjtalk_clear(OpenJTalk *oj);

// サンプリング周波数
void openjtalk_setSamplingFrequency(OpenJTalk *oj, unsigned int i);
unsigned int openjtalk_getSamplingFrequency(OpenJTalk *oj);


// 音響モデルファイル
//  絶対パス...直接、相対パス...音響モデルファイルディレクトリ基準、名前のみ...探索
bool openjtalk_setVoice(OpenJTalk *oj, const char *path);

// 現在の音響モデルファイルの取得
// pathは確実に長さMAX_PATHの配列
char *openjtalk_getVoice(OpenJTalk *oj, char *path);


// 同期発声。完了するまでこの関数から戻らない。
void openjtalk_speakSync(OpenJTalk *oj, const char *text);

// 非同期発声。発声を開始したらこの関数から戻る。
// この関数から戻った後は、次の関数によって音声の操作および状態の取得、待機を行う。
// openjtalk_pause、openjtalk_resume、openjtalk_stop、openjtalk_isSpeaking、
// openjtalk_isPaused、openjtalk_isFinished、openjtalk_waitUntilDone、openjtalk_waitUntilFinished、
// openjtalk_wait(0)
void openjtalk_speakAsync(OpenJTalk *oj, const char *text);

// 非同期発声を一時停止する。この一時停止はopenjtalk_resumeによってのみ再開される。
// 一時停止中の再度の一時停止は何もしない。
// 発声の停止が行われると、一時停止は無効となり、発声は完了する。
// 同期・非同期発声関数が呼び出されると、それが実行される前に、一時停止されていた発声は完了する。
void openjtalk_pause(OpenJTalk *oj);

// 非同期発声の一時停止からの再開。停止位置からの音声変換の再開ではなく音声データの再生再開。
void openjtalk_resume(OpenJTalk *oj);

// 非同期発声の強制停止。一時停止中の場合は、再開は無効となる。
void openjtalk_stop(OpenJTalk *oj);

// 非同期発声が発声中かどうか（一時停止中は偽）
// 一時停止の可能性がないときは、openjtalk_isFinishedの否定。
bool openjtalk_isSpeaking(OpenJTalk *oj);

// 一時停止中かどうか
bool openjtalk_isPaused(OpenJTalk *oj);

// 非同期発声が完了するまで待機する
void openjtalk_waitUntilDone(OpenJTalk *oj);
```

基本的な利用法は次のようになります。
* openjtalk_initialize でOpenJTalk構造体を確保します。このとき、標準の音響モデル、標準の辞書データ、標準の音響モデルフォルダが設定されます。同時に標準のパラメータも設定されます。
* openjtalk_initializeの戻り値であるOpenJTalk構造体へのポインタをハンドルにして、openjtalk_setSamplingFrequency や openjtalk_setVoice などで必要に応じて値を調整します。
* そして、openjtalk_speakAsync もしくは openjtalk_speakSyncに文字列を渡してしゃべらせます。
* 発声が全て終わったら、構造体を解放するために、openjtalk_clear を呼び出します。

単純な使用例をC言語で書くと次のようになります。

```C:hello.c
#include "jtalk.h"

void say(char*message)
{
  OpenJTalk *oj = openjtalk_initialize("","","");
  openjtalk_speakSync(oj,message);
  openjtalk_clear(oj);
}

int main()
{
  say(u8"こんにちは、世界");
  return 0;
}
```

## 他のプログラミング言語からの利用

このライブラリを他のプログラミング言語から利用するためのサンプルプログラムをffiフォルダの中に集めています。
これらの名前には'jtd_'の接頭辞を付けています。

これらのサンプルでは、各言語のFFI(Foreign function interface)機能を利用しています。
この機能を利用した各言語との仲介をするファイルの名前は、``jtalk.<言語の拡張子>`` になっています。

現在(2017.11.1)対応している言語は、
[C++Builder](https://www.embarcadero.com/jp/products/cbuilder/starter)、
C++、
C++/Cli、
C#、
[D](https://dlang.org/)、
[Delphi](https://www.embarcadero.com/jp/products/delphi/starter)、
[Java](https://www.java.com/ja/) (
  [Groovy](http://groovy-lang.org/index.html)、
  [Kotlin](https://kotlinlang.org/)、
  [Scala](https://www.scala-lang.org/)
)、
[Julia](https://julialang.org/)、 
[LuaJIT](http://luajit.org/)、
[node.js](https://nodejs.org/ja/)、
[Objective-C](https://developer.apple.com/jp/xcode/)、
[Python](https://www.python.org/)、
[Ruby](https://www.ruby-lang.org/ja/)、
[Swift](https://www.apple.com/jp/swift/)、
VisualBASIC、
WSH(
  JScript、
  VBScript
)
です。
C++からの利用はFFIとは言えませんが、仲介するヘッダファイルとサンプルプログラムは、ここに配置しています。
一部のプラットフォーム限定の言語もありますが、できる限りマルチプラットフォームで実行できるようにしています。
Objective-CとSwiftはmacOSでのみ動作します。

それぞれの言語には、上記のjtd_cと同等のコンソールプログラムを用意しています。
それ以外にもウィンドウを表示するGUIのサンプルもいくつかあります。
ffi/cbuilder/jtd_cb.cbproj, 
ffi/cpp/jtd_cppqt.cpp, 
ffi/cppcli/jtd_cli.cpp, 
ffi/cppcli/jtd_clim.cpp, 
ffi/csharp/jtd_cs.cs, 
ffi/csharp/jtd_csm.cs, 
ffi/delphi/jtd_delphi.dproj, 
ffi/java/JtdJnaJavaSwing.java, 
ffi/python/jtd_qt5.py, 
ffi/vb/jtd_vb.vb, 
ffi/vb/jtd_vbm.vb
はGUIプログラムです。

それぞれの言語は、最新の言語環境を構築して実行してください(2017/11/1時点)。
また、ffi/cpp/jtd_cppqt.cpp, ffi/python/jtd_qt5.pyはQT5を用いたサンプルです。
ビルド・実行には言語環境の他に、[Qt](https://www1.qt.io/download-open-source/)が必要になります。
JavaではC言語のDLLを利用するために[jna](https://github.com/java-native-access/jna)を利用しています。

### 命名の補足

CUIとGUIかどうかで対立するサンプルがあるときは、CUIの方に接尾辞'c'を付けています。
アンマネージDLLかマネージDLLを使うかで対立するサンプルがあるときは、マネージDLLを使う方に接尾辞'm'を付けています。
ビルドする手順が多少複雑なものには、接頭辞'build_'を付けたスクリプトを用意しています。
実行する手順が多少複雑なものには、接頭辞'run_'を付けたスクリプトを用意しています。

### 他の言語からの利用例

先ほどのC言語による例をLuaJITで書くと次のようになります。
同じ場所にffi/luajit/jtalk.luaが置いてあるとします。

```lua:hello.lua
function say(message)
  tts = require("jtalk").new()
  tts:speakSync(message)
  tts:destroy()
end
say("こんにちは、世界")
```

## 現在分かっている不具合

* ffi/csharp/jtd_cs.csはmacOSのmonoでは、実行できません。コアダンプを出力してエラー終了します。理由は分かりません。

* ubuntu 32bitにおいて、ffi/csharp/jtd_cs.cs と ffi/csharp/jtd_cppqt.cpp では何度か発声させると、portaudioがメモリ不足のエラーを起こします。

* ubuntu 64bitにおいて、ffi/python/jtd_py.py と ffi/ruby/jtd_rb.rbは、実行するとコアダンプを出力してエラー終了します。調べると問題はstrdupで起きています。strdupを同等のC言語による処理で置き換えるとエラーは起きなくなります。現在そうやってjpcommon_label.c, jpcommon_node.c, njd_node.cにコードを埋め込んで対処しています。

総合すると、何らかのメモリーリークが予想されますが解決できていません。

## ライセンス

このプロジェクトは、いくつかのプロジェクトのファイルを内部もしくは外部で利用しています。
それぞれのライセンスに従って利用してください。
それ以外の、主にjtalkフォルダ、ffiフォルダ、そしていくつかのその他の場所にあるファイルはオリジナルのものです。
このプロジェクトのライセンスは、[MIT ライセンス](https://opensource.org/licenses/MIT)とします。



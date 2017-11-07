# jtalkdll

これは音声合成システムOpen JTalk使って手軽に音声合成プログラミングするためのDLLです。

## 目次

この文書は以下の順序で記述します。

* 概要
* 動作環境
* ビルド
* 配置
* APIの説明
* 利用方法
* 現在分かっている不具合
* ライセンス

## 概要

PortAudioを使って、マルチプラットフォームでの音声出力を行います。
他のプログラミング言語から使いやすくするために、それぞれの言語のFFI(多言語関数インターフェイス)を利用するコードをいくつか用意しています。
WindowsではC#言語等の.NET Frameworkの言語や、WSH（ウィンドウズスクリプトホスト）で利用しやすいようにC++/CLIでラップしたマネージDLLもオプションで生成します。

このプロジェクトでは、オリジナルのファイル以外に、他のプロジェクトのファイルを含んでいます。

* [open_jtalk-1.10](http://open-jtalk.sourceforge.net/)
* [open_jtalk_dic_utf_8-1.10](http://open-jtalk.sourceforge.net/)
* [hts_voice_nitech_jp_atr503_m001-1.05](http://open-jtalk.sourceforge.net/)
* [hts_engine_API-1.10](http://hts-engine.sourceforge.net/)
* [htsvoice-tohoku-f01](https://github.com/icn-lab/htsvoice-tohoku-f01)
* [mei (MMDAgent)](http://www.mmdagent.jp/)
* [PortAudio](http://www.portaudio.com/)
* [jna](https://github.com/java-native-access/jna) サンプルプログラムにおいて

これらの外部プロジェクトはそれぞれのライセンスに従います。
なお、open_jtalk, hts_engine_APIはスタティックライブラリとして利用しています。
PortAudio はプラットフォームによって、スタティックライブラリまたは共有ライブラリとして利用しています。

### 仕組み

OpenJTalk の open_jtalk.c 
ファイルは、エクスポートしないライブラリの定義にmain関数をくっつけたような構成になっています。
これを利用して、それにwavファイルに含まれる音声データをmallocして返す関数を追加したものを
openjtalk.c とします。
それをラップしてインターフェイス(API)を定義したのが、jtalk.c です。
jtalk.c は内部で portaudio を呼び出すことでマルチプラットフォームでの同期再生、非同期再生を実現しています。
さらに、WindowsではマネージDLLにするために、C++/CLI でラップしたのが jtalkcom.cpp です。 

## 動作環境

以下のプラットフォームで動作確認しています。

* Windows 10
* macOS High Sierra
* Ubuntu 17.10 (他のLinuxディストリビューションは未確認)

## ビルド

コマンドラインで[CMake](https://cmake.org/)を使ってビルドします。
CMakeは、ソフトウェアのビルドを自動化するツールです。
一つの設定ファイルで、LinuxやmacOSだけでなく、WindowsのVC++のビルドも記述できるので、
マルチプラットフォームで共有ライブラリを作る今回のプロジェクトに適しています。

### CMakeLists.txtの設定

このプロジェクトのCMakeLists.txtはコマンドラインでの利用を前提として作られています。
コマンドラインでキャッシュ変数を指定して実行することもできますが、buildスクリプトから呼び出しているので、直接CMakeLists.txtの冒頭を編集する方が分かりやすいことがあります。

冒頭付近に次のようなコメントアウトしたsetコマンドがあり、これを必要に応じてアンコメントすることで設定を変えられます。

```CMake:
#set(target_arch i686)
#set(build_jtalkcom TRUE)
#set(install_open_jtalk TRUE)
```

上で引用した最初の行は、Cygwin64でビルドするときのアーキテクチャーの指定です。通常はx86_64ですが、この行を有効にすると、i686をターゲットにします。

二行目は、WindowsのコマンドラインでマネージDLLを作成するときに有効にします。

三行目は、open_jtalk, hts_engine, mecab-dict-index コマンドを一緒にbinフォルダにインストールするときに有効にします。
このjtalkdllを使うときには、open_jtalkそのものは必要ありませんが、動作の確認などに必要ならば、これを有効にしてください。
open_jtalk 用の mecab 辞書をコンパイルするときは、この mecab-dict-index が必要になるので、そのときもこの行をアンコメントしてください。

### Windows でのビルド

[マイクロソフト Visual Studio C++](https://www.visualstudio.com/ja/vs/cplusplus/) (以下MSVC)によるビルド方法と、
[Cygwin](https://www.cygwin.com/)および
[MSYS2](http://www.msys2.org/)上の
[MinGW-w64](http://mingw-w64.org/doku.php)のgccコンパイラによるビルド方法を以下に示します。
どちらで作っていいか分からないときは、MSVCでビルドしてください。

#### MSVCを使ったビルド

必要なもの：

* Visual Studio 2017
* CMake
* git （必須ではないけど）

まだ、Visual Studio 2017 がインストールされていない場合は、無償の Visual Studio 2017 Comunity　Edition もしくは Build Tools for Visual Studio 2017 をインストールしてください。
どちらも[マイクロソフトのVisual Studioダウンロードサイト](https://www.visualstudio.com/ja/downloads/)からダウンロードができます。
前者は高機能な統合開発環境、後者はC/C++のコードのコンパイルだけを目的とした最小限の構成のビルドツールですが、
このリポジトリのコンパイルはGUIを必要としない単純な作業なのでこのビルドツールでも十分です。

* Build Tools for Visual Studio 2017 のダウンロードボタンの場所が分かりにくいのですが、ページの最後の方の「その他ツール及びフレームワーク」のところにあります。
* COM相互運用のクラスライブラリjtalkCOMx86.dll または jtalkCOMx64.dllを作成するときは、Visual Studio のインストーラを使って C++/CLI サポートをインストールしておきます。
* 将来のバージョンのVisual Studioでもビルド可能か分かりませんが、そのときは後述のコマンドプロンプトを使った方法で試してください。

[CMake](https://cmake.org/)がまだインストールされていない場合は、
[ホームページ](https://cmake.org/)からダウンロードし、インストールします。なおインストールオプションの画面でPATHを通すラジオボタンを必ずチェックしてください。

gitは、このリポジトリからソースをコピーするために必要ですが、インストールされていなければ、[ZIPファイル](https://github.com/rosmarinus/jtalkdll/archive/master.zip)をダウンロードし展開すればいいので必須ではありません。
gitをインストールする場合は、[Git for Windows](https://git-for-windows.github.io/)からダウンロードしてインストールするか、
[Chocolatey](https://chocolatey.org/)を使ってgitパッケージを``cinst git``でインストールします。

#### インストール手順

##### コマンドプロンプトを使わない方法

Gitを使うか、[ZIPファイル](https://github.com/rosmarinus/jtalkdll/archive/master.zip)をダウンロードして、このリポジトリのコピーを取得します。
ZIPの場合は適当な場所で展開します。

自分のPCにコピーしたら、jtalkdllフォルダを開いて、buildx64.batとbuildx86.batを探します。
この二つのファイルがビルドするためのバッチファイルです。
64ビット版のjtalk.dllを作るときはbuildx64.batを、32ビット版はbuildx64.batを実行します。
ビルドが終わると、続けてインストールが行われます。

このビルドでは、C++/CLI サポートしているかを自動的に判断して、可能ならば、クラスライブラリjtalkCOMx86.dll または jtalkCOMx64.dllを生成します。
ただし生成されるのは署名されていないアセンブリになります。

##### コマンドプロンプトを使う方法

スタートメニューを開き、 Visual Studio 2017 フォルダ内にある 「x64 Native Tools Command Prompt for VS 2017」あるいは「x86 Native Tools Command Prompt for VS 2017」を起動します。どちらを使うかは64版か32版を作るかどうかで決めます。開いたら、適当なフォルダをカレントフォルダに決めます。

gitがインストールされているときは次のコマンドを実行します。

```DOS:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
```

gitが無いときは、上記のZIPファイルをダウンロード後、適当なフォルダに展開し、そのjtalkdllフォルダを前述のコマンドプロンプトのカレントディレクトリにします。

コピーが完了したら、次の一連のコマンドを実行すると、インストールされます。

```DOS:
cmake .. -G "NMake Makefiles"
nmake
nmake install
```

生成物は c:\open_jtalkフォルダに出力されます。
Windowsで今後jtalk.dllを利用する場合は、システムの詳細設定で環境変数PATHにc:\open_jtalk\binを追加してください。

cmake の行に ``-Dbuild_jtalkcom=true`` を追加すると、クラスライブラリjtalkCOMx86.dll または jtalkCOMx64.dllを生成します。
ただし生成されるのは、署名されていないアセンブリになります。
このクラスライブラリをビルドするためには、Visual Studio のインストーラを使って C++/CLI サポートをインストールしておく必要があります。

cmake の行に ``-Dinstall_open_jtalk=true`` を追加すると open_jtalk.exe, hts_engine.exe, mecab-dict-index.exe をインストールします。

WindowsのCMakeではvcxprojファイルを出力し、MSBuildでビルドする方式もあります。
この方がvcxprojファイルが生成されるので、Visual Studioでのデバッグなどには都合がいいです。
しかし今回はインストールまで自動化させるため nmake.exe を使う方法にしています。

vcxprojを出力して実行する方法は次の通りです。必要な場合は、展開後に以下のコマンドを実行してください。

vs2017で64ビット版をビルドする

```DOS:
cmake .. -G "Visual Studio 15 2017 Win64"
msbuild ALL_BUILD.vcxproj /p:Configuration=Release;Platform=x64
```

vs2017で32ビット版をビルドする

```DOS:
cmake .. -G "Visual Studio 15 2017"
msbuild ALL_BUILD.vcxproj /p:Configuration=Release;Platform=win32
```

#### MSYS2 の MinGW-W64 を使ったビルド

##### MSYS2の準備

* MSYS2のインストール
* 必要なパッケージのインストール

###### MSYS2のインストール

[MSYS2 homepage](http://www.msys2.org/) から、msys2-i686-xxxxxxxx.exe （xxxxxxxxは日付の数列）もしくは msys2-x86_64-xxxxxxxx.exe をダウンロードします。
それぞれ32ビットと64ビット版です。32ビットマシンでない限り、特にこだわりがなければ、64ビット版がいいでしょう。
セットアップの方法は割愛します。

###### 必要なパッケージのインストール

セットアップが終わったら、jtalkdllの生成のためには次のパッケージを追加でインストールします。
git, base-devel, mingw-w64-i686-toolchain, mingw-w64-x86_64-toolchain。
最後の2つは32ビット版、64ビット版のビルドコマンド群なので、両方もしくは必要な方を入れてください。

```bash:
pacman -S git base-devel mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain
```

##### MSYS2でのビルド方法

環境が整ったら、開発用のコンソールを開きます。64bit版を作りたいときは、Windowsのスタート画面からMSYS2 MinGW 64-bit、32bit版ならばMSYS2 MinGW 32-bitです。
そして、カレントディレクトリを適当な場所に切り替えて、次の一連のコマンドを実行すると、dllが生成されます。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

なお、音声を再生するために利用しているPortAudioがCMakeではうまくmingw用のスタティックライブラリを生成できなかったので、congifureを一箇所改変（後述）して、CMmakeに先だってライブラリを生成させています。

辞書ファイル、音響モデルファイルなどのデータファイルはMSVCのときと同じ場所、c:\open_jtalkフォルダにインストールされます。
jtalk.dllや、実行ファイルのサンプル、ヘッダファイルは、MSYS2のMinGWビルド用のディレクトリにインストールされます。
MSYS2の外で、jtalk.dllを使ったプログラムを動かすためには、そのプログラムと同じフォルダにjtalk.dllを配置するか、
jtalk.dllを手作業でc:\open_jtalk\binにコピーして、ここにPATHを通すかしてください。

buildスクリプト中のcmake の行に ``-Dinstall_open_jtalk=true`` を追加すると open_jtalk、hts_engine、mecab-dict-index をインストールします。

##### PortAudio でスタティックライブラリを作る改変

今回、MingGWではCMake中でスタティックライブラリをリンクできず、またportaudioのconfigureを使ったビルドでもスタティックライブラリそのものが作れませんでした。
試行錯誤でconfigureの15194行にあるSHARED_FLAGSの -shared オプションが邪魔をしているようなので、これを削除して、スタティックライブラリを作っています。

#### Cygwin の MinGW-W64 を使ったビルド

##### Cygwinの準備

* Cygwin / Cygwin64 のインストール
* 必要なパッケージのインストール

###### Cygwin / Cygwin64 のインストール

Cygwinがまだないときは、[Cygwin Installation](https://www.cygwin.com/install.html) から、setup-x86_64.exe もしくは setup-x86.exe をダウンロードします。
32ビットマシンではない限り setup-x86_64.exeの方をインストールした方がいいでしょう。セットアップの方法は割愛します。

###### 必要なパッケージの Cygwin へのインストール

Cygwinのセットアップが完了したら、dllの生成のために次のパッケージを追加でインストールします。
ファイル名にi686とx86_64が含まれているそれぞれ二つのパッケージは32bit版、64ビット版をコンパイルするための実行ファイル群です。必要なければどちらかを外してください。
cmake, make, git, gcc-core, gcc-g++, mingw64-i686-gcc-core, mingw64-i686-gcc-g++, mingw64-x86_64-gcc-core, mingw64-x86_64-gcc-g++

#### Cygwinでのビルド方法

環境が整ったら、次の一連のコマンドでリポジトリからソースファイルをコピーし、ビルドを開始します。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

Cygwinだと32bit版、Cygwin64だと64bitのdllが生成されます。64ビットマシンでbuildスクリプトに引数として i686 か x86_64 を渡すと、指定したものを作ります。

なお、音声を再生するために利用しているPortAudioがCMakeではうまくMinGW用のスタティックライブラリを生成できなかったので、congifureでCMmakeに先だってライブラリを生成させています。

辞書ファイル、音響モデルファイルなどのデータファイルはMSVCのときと同じ場所、c:\open_jtalkフォルダにインストールされます。
jtalk.dllや、実行ファイルのサンプル、ヘッダファイルは、CygwinのMinGWビルド用のディレクトリにインストールされます。
Cygwinの外で、jtalk.dllを使ったプログラムを動かすためには、そのプログラムと同じフォルダにjtalk.dllを配置するか、
jtalk.dllを手作業でc:\open_jtalk\binにコピーして、そこにPATHを通すかしてください。

### macOS でのビルド

#### 準備

* Xcodeのインストール
* Homebrewのインストール

##### Xcodeのインストール

macOSでプログラムの開発をするには、Xcodeがインストールされていなくてはいけません。
統合環境としてのXcodeを利用しないこのプロジェクトでも必要です。
Xcodeのインストール方法は割愛します。

##### Homebrewのインストール

macOS用のパッケージマネージャーHomebrewもインストールしておくとパッケージを追加するのに便利です。
インストールの仕方と、使い方は、[Homebrew](https://brew.sh/index_ja.html)にあります。

（インストールすべきコマンドのリストアップ 記述予定）

#### macOSでのビルド方法

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

### Ubuntu でのビルド

以下に、Ubuntuでのビルド方法を示します。

他のlinuxディストリビューションでも同様の処理を行えば、生成できるでしょうが、Ubuntuだけを例として示します。
libconvに関しては簡単な存在判定ルーチンを入れて自動処理しています。
万が一これがうなく働かないときは、CMakeFile.txtに手を加えて対処してください。

#### Ubuntuでの準備

portaudio で必要になるので、あらかじめ次のコマンドでALSAのSDKをインストールしておきます。

```bash:
sudo apt install libasound-dev
```

PortAudioパッケージがそのディストリビューション向けに用意されているときは、できればそれを、jtalkdllをビルドする前にインストールしておいてください。
CMakeを実行すると、PortAudioのライブラリを探索し見つからなければ、自動的に同梱しているPortAudioのソースからビルドします。

あとは次の一連のコマンドで、ソースファイルをコピーして、ビルドします。

```bash:
git clone https://github.com/rosmarinus/jtalkdll.git
cd jtalkdll
bash build
```

## 配置

### インストールされるファイル

このプロジェクトをビルドし、インストールすると以下の成果物が配置されます。

#### 共有ライブラリ

* jtalk.dll / libjtalk.so / libjtalk.dylib

#### COM 相互運用クラスライブラリ

MSVCのみ。署名なし。通常コマンドラインからのビルドではビルド無効。
完全署名されたアセンブリが必要な場合は、別途配布版をダウンロードしてください。

* jtalkCOMx86.exe, jtalkCOMx64.exe
* COMを登録・解除するためのバッチファイル regist_jtalkcom.bat, unregist_jtalkcom.bat

#### ヘッダファイル

* jtalk.h

#### インポートライブラリ

MSVCのみ

* jtalk.lib

#### サンプルプログラム

* jtd_c.exe / jtd_c
* jsay.exe / jsay

#### データファイル

* 音響モデルファイル
* 辞書データ

### 成果物の配置先

ビルドによる成果物は、プラットフォームによって以下のフォルダにインストールされます。

#### Windows MSVC での配置先

* 共有ライブラリ jtalk.dll ... c:\open_jtalk\bin
* COM 相互運用クラスライブラリ jtalkCOMx64.dll, jtalkCOMx86.dll ... c:\open_jtalk\bin
* ヘッダファイル jtalk.h ... c:\open_jtalk\include
* インポートライブラリ jtalk.lib ... c:\open_jtalk\lib
* サンプルプログラム jtd_c.exe, jsay.exe ... c:\open_jtalk\bin
* MeCab辞書ファイル ... c:\open_jtalk\dic_utf_8
* 音響モデルファイル ... c:\open_jtalk\voice
* バッチファイル regist_jtalkcom.bat, unregist_jtalkcom.bat ... c:\open_jtalk\bin

#### Windows Cygwin での配置先

* 共有ライブラリ jtalk.dll ... /usr/$MINGW_ARCH/bin
* ヘッダファイル jtalk.h ... /usr/$MINGW_ARCH/include
* サンプルプログラム jtd_c, jsay ... /usr/$MINGW_ARCH/bin
* MeCab辞書ファイル ... c:\open_jtalk\dic_utf_8
* 音響モデルファイル ... c:\open_jtalk\voice

上記の変数 MINGW_ARCH の値は i686-w64-mingw32 または x86_64-w64-mingw32 とする。

#### Windows MSYS2 での配置先

以下に出てくる環境変数 MINGW_PREFIX の値は mingw32 または mingw64 である。

* 共有ライブラリ jtalk.dll ... /$MINGW_PREFIX/bin
* ヘッダファイル jtalk.h ... /$MINGW_PREFIX/include
* サンプルプログラム jtd_c, jsay ... /$MINGW_PREFIX/bin
* MeCab辞書ファイル ... c:\open_jtalk\dic_utf_8
* 音響モデルファイル ... c:\open_jtalk\voice

#### macOS / Ubuntu での配置先

* 共有ライブラリ libjtalk.dylib / libjtalk.so ... /usr/local/bin
* ヘッダファイル jtalk.h ... /usr/local/include
* サンプルプログラム jtd_c, jsay ... /usr/local/bin
* MeCab辞書ファイル ... /usr/local/OpenJTalk/dic_utf_8
* 音響モデルファイル ... /usr/local/OpenJTalk/voice

### 音響モデルの追加

ソースファイルと一緒に配布している音響モデルの他に、新しく入手した日本語音響モデルファイル(htsvoice)を利用することもできます。

今後そのコンピュータで使う標準の音声として登録するには、次のフォルダに htsvoiceファイルをコピーします。

Windows（Cygwin、MinGW含めて）の場合
``c:\open_jtalk\voice``
Windows以外の場合
``/usr/local/OpenJTalk/voice``

再帰的にhtsvoice拡張子のファイルだけを探索するので、フォルダごとコピーしてもかまいません。

一時的な利用など、上記のフォルダに配置しなくても音声として使用できます。
関数 openjtalk_initializeの第一引数や、関数 openjtalk_setVoiceなどでhtsvoiceファイルの場所を指定します。
音響モデルファイルを途中で変えずに同じ音声だけを使うプログラムの場合はこの方法で差し支えありません。
プログラムの中で音響モデルファイルを選択があるときは、関数 openjtalk_getVoices で取得するリストに含まれなくなるので、あまり適していません。

他に、配布するプログラムでの利用を想定して、
プログラムと同じフォルダにvoiceで始まるフォルダを作り、その中に複数の音響モデルファイルを配置する方法や、
htsvoiceファイルのあるフォルダを 関数openjtalk_initializeの第三引数や、関数openjtalk_setVoiceDirで登録する方法もあります。

## 動作確認

対象のプラットフォームにおいて上記の方法でビルドが成功したら、以下の方法で動作確認ができます。

次のコマンドをタイプしエンターしてください。うまくビルドできていれば、一緒にインストールしてあるhtsvoiceファイルをランダムに選んで言葉をしゃべってくれます。
Windowsの場合は、Cygwin、MinGWを含め、インストール先をWindowsのコマンドプロンプトでカレントディレクトリにするか、エクスプローラで開いてマウスでダブルクリックするかして、実行してください。

```bash:
jtd_c
```

このコマンドのソースはjtalkフォルダにある[jtd_c.c](https://github.com/rosmarinus/jtalkdll/blob/master/jtalk/jtd_c.c)で、C言語で書いたこの共有ライブラリのサンプルコードです。

次はもう一つのサンプルの[jsay](https://github.com/rosmarinus/jtalkdll/blob/master/jtalk/jsay.c))です。下のコマンドラインのように パラメータに日本語を入力して、エンターしてください。うまくいけば、その言葉をしゃべります。

```bash:
jsay -v mei_normal こんにちは
```

Windowsの場合（Cygwin、MinGWでビルドしても）は、UTF-8エンコードで書かれたテキストを、jsay.exe のアイコンにマウスでドロップしても確認できます。

jsayはmacOSのsayコマンドに動作を似せたサンプルで、このソースもjtalkフォルダにあります。
-v?オプションで利用できる音声のリストが出ますので、それを参考にして音声を指定してください。
-oファイル名 オプションでファイルへの書き出しもできます。
文字列が指定されていないと、標準入力からの入力を待ちます。
エンターを連続して入力するまで続きます。

## APIの説明

jtalk.cではopen_jtalkを利用するのに便利な関数をAPI関数としていくつか定義しています。
ここで定義されているAPI関数には接頭語'openjtalk_'をつけています。

本来の open_jtalk で指定できるパラメータの設定と取得ができます。
設定はset、取得はgetの文字列が名前に含まれます。
パラメータの名前はコマンドラインで指定する短い名前とその意味を反映した長い名前の両方を用意しています。

文字列を入出力するAPIにおける文字コードは基本的にUTF-8です。内部で使っているmecabにもUTF-8文字列を渡しています。
しかし、プログラミング言語によっては16ビットのUnicodeを文字コードとして使っているものもあり、受け渡しが簡単になるようにUTF-16LEの文字列のを引数、戻り値になる関数も用意しています。これらには名前の末尾にU16が付いています。
また Windowsではコマンドプロンプトの出力などShift-JIS（CP932）を使わざるを得ない場合もあるので、Shift-JISも関数の引数、戻り値として使えるようにしています。こちらには名前の末尾にSjisが付いています。

Utf-16leを扱う文字の型はchar16_t、文字列の型はchar16_t*を使っています。
Windowsの wchar_t は厳密には同じ型ではありませんが、内部では同じものとして使っています。
サロゲートペア文字列などへの対応は今後の課題とします。

### 主要なAPI

API関数についての詳しい内容は [jtalk.h](https://github.com/rosmarinus/jtalkdll/blob/master/jtalk/jtalk.h) を直接見てください。

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
* openjtalk_initializeの戻り値であるOpenJTalk構造体へのポインタをハンドルにして、openjtalk_setVoice や openjtalk_setSamplingFrequency などを呼び出し、値を調整します。ただし、音響モデルファイルを変更すると、それに合わせてパラメータが初期化されるので、openjtalk_setVoiceの後に、パラメータの設定の関数を呼び出すようにします。
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

## 利用方法

### jtalk共有ライブラリ の C言語からの利用

jtalk共有ライブラリを使ったC言語でプログラミングの仕方を説明します。
一般的な共有ライブラリを使ったプログラミングの仕方が分かれば、問題ありません。
C言語でライブラリを使ったプログラミングをするには、ライブラリとそのライブラリで定義されている関数を記述したインクルードファイルが必要になります。
正常にビルドが完了していれば、ライブラリとインクルードファイルは所定の場所に配置されているはずです。

#### gcc による利用

gccやclangの場合は共有ライブラリ名をコマンドラインに-lオプションで指定します。
例えば、ubuntuのgccの場合は次のようになります。

```bash:
gcc hello.c -ljtalk -ohello
```

#### Windows Cygwin/Cygwin64 MinGW-w64 gcc.exe による利用

同じgccでもWindowsでCygwin/Cygwin64の MinGW-w64 gccを使う場合は、少し工夫が必要になります。
x86_64-w64-mingw32-gccでコンパイルするか、i686-w64-mingw32-gccでコンパイルするかで、リンクするライブラリが違ってくるためです。
つまり、-Lオプションによるライブラリの位置の明示的な指定を追加します。

例えば、i686-w64-mingw32-gccでコンパイルする場合は、次のようにします。なおjtalk.dllはbinフォルダに配置されています。

```bash:
MINGW_ARCH=i686-w64-mingw32
$MINGW_ARCH-gcc hello.c -L/usr/$MINGW_ARCH/bin -ljtalk -ohello
```

Cygwinのコンソール上から実行するには、hello.exeのすぐそばに、同じアーキテクチャでコンパイルしたjtalk.dllを配置します。
通常のWindowsアプリケーションと同じように実行するには、同じアーキテクチャでビルドしたjtalk.dllが、同じフォルダかパスが通ったところに存在する必要があります。

#### Windows MSYS2 MinGW-w64 gcc.exe による利用

MSYS2上のMinGW-w64は、MSYS2 MinGW 32-bitとMSYS2 MinGW 64-bitの、どちらのコンソールを開くかによって環境を区別できるので、Cygwinより少し楽にできます。

```bash:
gcc hello.c -L$MINGW_PREFIX/bin -ljtalk -ohello
```

環境変数MINGW_PREFIXには、コンソールに応じてmingw64かmingw32の文字列が入っているので、これを使ってjtalk.dllの位置を指定しています。
同じMSYS2 MinGWコンソール内では jtalk.dll にパスが通っているので、そのまま実行できます。
通常のWindowsアプリケーションと同じように実行するには、同じアーキテクチャでビルドしたjtalk.dllが、同じフォルダかパスが通ったところに存在する必要があります。

#### Windows cl.exe による利用

Windowsのcl.exeの場合は、共有ライブラリそのものではなく、インポートライブラリをコマンドラインに指定します。
このインポートライブラリはc:\open_jtalk\libにあります。
ここにあるライブラリはコンパイルしたアーキテクチャーによって名前が区別されています。
具体的には、x64でコンパイルしたものは jtalkx64.libで、x86はjtalkx86.libです。
利用するコンパイラのアーキテクチャに従ってこのライブラリを選びます。

例えば、x64版cl.exeで上記のhello.cをコンパイルするコマンドラインはこうになります。
このとき適切なVSコマンドプロンプトで開いているものとします。

```DOS:
set JTALKDIR=c:\open_jtalk
cl /I %JTALKDIR%\include hello.c jtalkx64.lib /link /LIBPATH:%JTALKDIR%\lib
```

もちろん、できあがった hello.exe を実行するときは、同じアーキテクチャでビルドしたjtalk.dllが、同じフォルダかパスが通ったところに存在する必要があります。

### 他のプログラミング言語からの利用

この共有ライブラリを他のプログラミング言語から利用するためのサンプルプログラムをffiフォルダの中に集めています。
これらの名前には'jtd_'の接頭辞を付けています。

サンプルでは、各言語のFFI(Foreign function interface)機能を利用しています。
各言語との仲介をするファイルの名前は、

``jtalk.<言語の拡張子>``

という形式になっています。例えば、luajit用は``jtalk.lua``です。

現在(2017.11.1)対応している言語は、
[C++Builder](https://www.embarcadero.com/jp/products/cbuilder/starter)、
C++、
C++/CLI、
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
C++からの利用はFFIとは言えませんが、仲介するヘッダファイルとサンプルプログラムは、ffiフォルダに配置しています。
一部のプラットフォーム限定の言語もありますが、できる限りマルチプラットフォームで実行できるようにしています。
Objective-CとSwiftはmacOS限定の言語ではありませんが、現在macOSでのみ動作します。

それぞれの言語には、上記のjtd_cと同等のコンソールプログラムを用意しています。

それ以外にいくつかウィンドウを表示するGUIのサンプルもあります。
[ffi/cbuilder/jtd_cb.cbproj](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/cbuilder/jtd_cb.cbproj), 
[ffi/cpp/jtd_cppqt.cpp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/cpp/jtd_cppqt.cpp), 
[ffi/cppcli/jtd_cli.cpp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/cppcli/jtd_cli.cpp), 
[ffi/cppcli/jtd_clim.cpp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/cppcli/jtd_clim.cpp), 
[ffi/csharp/jtd_cs.cs](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/csharp/jtd_cs.cs), 
[ffi/csharp/jtd_csm.cs](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/csharp/jtd_csm.cs), 
[ffi/delphi/jtd_delphi.dproj](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/delphi/jtd_delphi.dproj), 
[ffi/java/JtdJnaJavaSwing.java](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/java/JtdJnaJavaSwing.java), 
[ffi/python/jtd_qt5.py](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/python/jtd_qt5.py), 
[ffi/vb/jtd_vb.vb](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/vb/jtd_vb.vb), 
[ffi/vb/jtd_vbm.vb](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/vb/jtd_vbm.vb)
はGUIプログラムです。

またサンプルの名前には次のような規則で区別しています。
CUIとGUIかどうかで対立するサンプルがあるときは、CUIの方に接尾辞'c'を付けています。
アンマネージDLLかマネージDLLを使うかで対立するサンプルがあるときは、マネージDLLを使う方に接尾辞'm'を付けています。

それぞれの言語は、できるだけ最新の言語環境を構築して実行してください。
例えば Ubuntu の aptでインストールされる julia では is_unix や unsafe_string などが使えない古いものなので(2017/11/1時点)、
julia で使うには、ホームページから v0.60以降をダウンロードして利用してください。
それから、ffi/cpp/jtd_cppqt.cpp, ffi/python/jtd_qt5.pyは[QT5](https://www.qt.io/)を用いたサンプルです。
ビルド・実行には言語環境の他に、[Qt](https://www1.qt.io/download-open-source/)が必要になります。
また、JavaではC言語のDLLを利用するために[jna](https://github.com/java-native-access/jna)を利用しています。

ビルドする手順が複雑なものには、接頭辞'build_'を付けたスクリプトを用意しています。
実行する手順が複雑なものには、接頭辞'run_'を付けたスクリプトを用意しています。
これらのスクリプト内のフォルダ名やバージョン番号は環境に合わせて適宜書き換えてください。
単にコンパイラやインタプリタの引数にすればいいものには実行スクリプトは用意していません。

#### 他の言語からの利用例

##### LuaJIT での利用例

ここではLuaJITでの例を紹介します。

[LuaJIT](http://luajit.org/)は、[本家Lua](https://www.lua.org/)との大きな違いの一つに、
標準内蔵されているFFIの機能があります。
これを利用して簡単に共有ライブラリを利用するLuaプログラムを書くことができます。

C言語による先の例をLuaJITで書くと次のようになります。

```lua:hello0.lua
ffi = require("ffi")
jt = ffi.load("jtalk")
ffi.cdef [[
void *openjtalk_initialize(const char *voice, const char *dic, const char *voiceDir);
void openjtalk_speakSync(void *oj, const char *text);
void openjtalk_clear(void *oj);
]]
function say(message)
  local handle = jt.openjtalk_initialize("","","")
  jt.openjtalk_speakSync(handle, message)
  jt.openjtalk_clear(handle)
end
say("こんにちは、世界")
```

その都度C言語での定義を書いてもいいですが、
C言語の関数をluaの関数でラップして、モジュールとしてまとめたものを用意しています。
これは [ffi/luajit/jtalk.lua](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/luajit/jtalk.lua) に置いてあります。
jtalk.luaを使うと、このようになります。

```lua:hello.lua
function say(message)
  local tts = require("jtalk").new()
  tts:speakSync(message)
  tts:destroy()
end
say("こんにちは、世界")
```

##### Java VM からの利用例

jtalkdllをJavaから使うために、[JNA](https://github.com/java-native-access/jna)を利用しています。
簡単にJavaから共有ライブラリを使うことができるライブラリです。
API関数をラップしてJavaおよびJavaVM言語から利用しやすい形にしたのが、[JTalkJna.java](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/luajit/jtalk.lua) です。
この JTalkJna.java を``build_jtalk_jar``スクリプトで単純なjarファイル jtalk.jar にしてあるのでこれを import して使います。
残念ながら、jtalk.jar は JavaSpeechAPI の実装ではありません。

JTalkJnaの内容は、
[JTalkJna-JavaDoc](http://htmlpreview.github.io/?https://github.com/rosmarinus/jtalkdll/blob/master/ffi/java/doc/index.html)
を見てください。

ここでは、Java ではなく、[Kotlin](http://kotlinlang.org/) で例を示します。

```Kotlin:Hello.kt
import jtalk.JTalkJna
fun say(message:String) {
  var tts = JTalkJna()
  tts.speakAsync(message)
  while(tts.isSpeaking);
}
fun main(args:Array<String>) {
  say("こんにちは、世界")
}
```

なお、OpenJTalk には Java クローンである [Gyutan](https://github.com/icn-lab/Gyutan) があります。
本格的に Java で OpenJTalk の音声合成を使う場合は、こちらを使った方がいいでしょう。

こんな感じで他の言語も書いていけます。
ただし言語によってはffiモジュールをインストールする手間が必要になります。
各言語における共有ライブラリの参照の仕方さえ分かれば、ここで用意していないプログラミング言語でも利用できるはずです。

### JTalkCOM の利用

JTalkCOMx86.dll と JTalkCOMx64.dll を使った Windowsでのプログラミングの方法を示します。

#### マネージDLL としての利用

JTalkCOMは、Windows の C++/CLIのcl.exe, C#のcsc.exe, VisualBASICのvb.exe などで利用できます。
サンプルは 
[ffi/clicpp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/clicpp)、
[ffi/csharp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/csharp)、
[ffi/vb](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/vb) 
 の中の名前の末尾が``m``もしくは``mc``のものです。

C#での例を示します。

```C#:hello.cs
using JTalkCom;
using System;

namespace JTalkSample
{
    public class MainClass
    {
        static void say(string message)
        {
            using (var tts = new JTalkTTS {})
            {
                tts.SpeakSync(message);
            }
        }

        [STAThread]
        static void Main()
        {
          say("こんにちは、世界");
        }
    }
}
```

ビルドするためのコマンドラインは次のようになります。
jtalkdllのインストール時に示したVSコマンドプロンプトを使用します。

変数を使わないで64ビット向けを書くと：

```DOS:
rem csc /platform:x64 /target:exe /reference:C:\open_jtalk\bin\JTalkCOMx64.dll hello.cs
```

変数を使って汎用性をもたせると:

```DOS:
set JTALKDIR=c:\open_jtalk
set jtalkcom=JTalkCOM%VSCMD_ARG_HOST_ARCH%.dll
copy %JTALKDIR%\bin\%jtalkcom% .
csc /platform:%VSCMD_ARG_HOST_ARCH% /target:exe /reference:%jtalkcom% hello.cs
```

clやvbで使う場合は、対応する記述に書き換えます。
[ffi/clicpp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/clicpp)、
[ffi/csharp](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/csharp)、
[ffi/vb](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/vb) の例を見比べると分かりやすいでしょう。
同様にコマンドラインでのビルドオプションの違いは、サンプル付属のビルドスクリプトを確認ください。
VisualStudioのIDEを使う場合は、アセンブリの参照にjtalkCOMx64.dllかjtalkCOMx86.dllを追加します。

なお、これと同様な処理は共有ライブラリ jtalkdll を P/Invoke（プラットフォーム呼び出し）を使っても実現できます。
この方法だと、mono を使ってC#のコードを Windows以外でも実行できます。
[ffi/csharp/jtalk.cs](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/clicpp/jtalk.cs)を使用します。
jtalk.csを使うときは上記のコード冒頭の``using JTalkCom;``を``using JTalkDLL;``に書き換えます。

#### COMオブジェクトとしての利用

JTalkComはC++/CLIで書かれたマネージDLLですが、COM相互運用機能によりCOMオブジェクトとして振る舞います。
このDLLの名前の由来でもあります。

まず、COMへの登録は``regist_jtalkcom.bat``を管理者権限で実行して行います。
内部でregasm.exeコマンドを呼び出しています。
管理者権限での実行は、エクスプローラ上でこのファイルを右クリックして、コンテキストメニューを出し、
そのメニューの中の「管理者として実行」を選んで行います。
これは一度行えば十分です。解除するときは``unregist_jtalkcom.bat``を管理者権限で実行します。

JScriptのファイルの拡張子である``.js``は他のアプリケーションに関連付けされていることが多いので、同等なXML形式の``.wsf``ファイルで記述します。

```XML:hello.wsf
<?xml version="1.0" encoding="utf-8" ?>
<job id="hello"><script language="JScript">
<![CDATA[

var say=function(message){
  var tts = new ActiveXObject("JTalk.TTS");
  tts.SpeakSync(message);
}
say("こんにちは、世界");

]]>
</script></job>
```

これをUTF-8エンコードでhello.wsfという名前を付けて保存して、そのファイルのアイコンをダブルクリックすれば、しゃべります。
サンプルコードとして、[ffi/wsf/jtd_js.wsf](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/wsf/jtd_js.wsf)、
[ff/wsf/jtd/jtd_vbs.wsf](https://github.com/rosmarinus/jtalkdll/blob/master/ffi/wsf/jtd_vbs.wsf) を用意しています。

## 現在分かっている不具合

実用上問題の無いレベルにはなってきたとは思っていますが、未だにいくつか問題が存在しています。

* ffi/csharp/jtd_cs.csはmacOSのmonoでは、実行できません。コアダンプを出力してエラー終了します。理由は分かりません。

* ubuntu 32bitにおいて、ffi/csharp/jtd_cs.cs と ffi/csharp/jtd_cppqt.cpp では何度か発声させると、portaudioがメモリ不足のエラーを起こし、異常終了はしませんが、声が出なくなります。

* ubuntu 64bitにおいて、ffi/python/jtd_py.py と ffi/ruby/jtd_rb.rbは、実行するとコアダンプを出力してエラー終了します。調べると問題はstrdupで起きています。そこでstrdupを同等のC言語による処理で置き換えるとエラーは起きなくなります。現在そうやって jpcommon_label.c、jpcommon_node.c、njd_node.c にコードを埋め込んで対処しています。

総合すると、何らかのメモリーリークが予想されますが特定できていません。

## ライセンス

このプロジェクトにあるオリジナルのファイルのライセンスは、[MIT ライセンス](https://opensource.org/licenses/MIT)とします。
それらは、主に[jtalkフォルダ](https://github.com/rosmarinus/jtalkdll/tree/master/jtalk)、
[ffiフォルダ](https://github.com/rosmarinus/jtalkdll/tree/master/ffi)にあります。

このリポジトリのファイルには、オリジナルのファイル以外に、他のプロジェクトのファイルを含んでいます。
そのまま使っているものもあれば、一部修正を加えているものもあります。
他のプロジェクト由来のファイルは、それぞれのライセンスに従います。

# open_jtalk用Cmake関連ファイル

これは open_jtalk をビルドするための Cmake用 CMakeLists.txt、及び関連ファイルです。

## 目次

この文書は以下の順序で記述します。

* 概要
* 動作環境
* 使用するファイル
* ダウンロード
* ビルド
* インストールの内容
* 動作確認
* アンインストール
* ライセンス

## 概要

jtalkdllではなくopen_jtalk そのものをビルドするファイル群です。
先に作った jtalkdll の用の CmakeLists.txt から不必要なものを削り、アーカイブのダウンロードスクリプトを追加したものです。

次に示す他のプロジェクトのファイルをダウンロードして利用します。
これらの外部プロジェクトはそれぞれのライセンスに従います。

* [open_jtalk-1.11](http://open-jtalk.sourceforge.net/)
* [hts_voice_nitech_jp_atr503_m001-1.05](http://open-jtalk.sourceforge.net/)
* [hts_engine_API-1.10](http://hts-engine.sourceforge.net/)
* [htsvoice-tohoku-f01](https://github.com/icn-lab/htsvoice-tohoku-f01)
* [mei takumi (MMDAgent)](http://www.mmdagent.jp/)


### 仕組み

build(.bat)スクリプトで以下の prepare、cmake、make(nmake) を呼び出します。
* prepareスクリプトで、必要なアーカイブをダウンロード・展開します。
* cmakeコマンドによってプラットフォームに合わせたMakefileを生成します。
* それぞれのプラットフォームでmake そして make install を実行します。
  
※ Windows MSVCでのビルドでは WSL(Windows Subsystem for Linux)上のbashを起動して、prepare を実行します。
WSLがインストールされていない場合は、手作業で必要なアーカイブをダウンロードし、展開してください。

## 動作環境

以下のプラットフォームで動作確認しています。

* Windows 10 （MSVC および MSYS2上のMinGW）
* macOS Catalina
* Ubuntu 19.10 (他のLinuxディストリビューションは未確認、sudoを使える設定を)

## 使用するファイル

次のページにあるファイル群を使用します。
https://github.com/rosmarinus/jtalkdll/tree/master/extra/open_jtalk

* build(.bat) ... ビルド実行スクリプト本体
* prepare ... ダウンロード・展開スクリプト
* CMakeLists.txt ... cmake設定ファイル
* open_jtalk-1.11_mingw.patch ... MinGW用の修正情報
* hello(.bat) ... 確認用のスクリプト
* README.md ... このファイル

## ダウンロード

上記のファイルをまとめた [openjtalk_build_scripts](https://github.com/rosmarinus/jtalkdll/releases/download/v0.0.65/openjtalk_build_scripts-0.0.65.zip) をダウンロードしてください。

Windowsでリポジトリから直接ファイルをダウンロードする場合は、設定によってバッチファイルの改行コードがCRLFではないときがあります。
これではうまく実行できないので、エディタで保存し直したりして、改行コードを書き換えてください。
また、同様に prepareスクリプトの改行コードがCRLFになってしまっている時があります。この時は改行コードをLFに書き換えてください。

## ビルド

### Windows でのビルド

* [マイクロソフト Visual Studio C++](https://www.visualstudio.com/ja/vs/cplusplus/) (以下MSVC)によるビルド方法と、
* [MSYS2](http://www.msys2.org/)上の
[MinGW-w64](http://mingw-w64.org/doku.php)のgccコンパイラによるビルド方法を以下に示します。

#### MSVCを使ったビルド

##### 必要なもの

* Visual Studio 2019
* CMake
* WSL Ubuntuなど（あると便利）

###### Visual Studio 2019

Visual Studio 2019 がインストールされていない場合は、無償の Visual Studio 2019 Comunity　Edition もしくは Build Tools for Visual Studio 2019 をインストールしてください。
どちらも[マイクロソフトのVisual Studioダウンロードサイト](https://www.visualstudio.com/ja/downloads/)からダウンロードができます。
また[Chocolatey](https://chocolatey.org/)を使ってインストールすることもできます。
このときのパッケージ名は、``visualstudio2019community``、``visualstudio2019buildtools``です。

インストールが完了したら、``Visual Studio Installer``をスタートメニューから起動して、変更ボタンを押します。
出現したワークロード画面で、このプロジェクトに必要な構成にチェックして、右下の「変更」ボタンを押し、必要なコンポーネントをインストールします。
このとき必要な構成の指定は次のようにします。Visual Studio Community 2019では、ワークロードで「C++によるデスクトップ開発」にチェックします。
Visual Studio Build Topls 2019では、ワークロードで「C++ Build Tools」にチェックします。

2020/02/22現在の最新版は2019ですが、将来、後継バージョンに移行してVisual Studio 2019 が入手できない可能性があります。
そのとき、Visual Studio のインストール場所に大規模な変更が行われなければ、
``build.bat``の冒頭付近にある``2019``の文字をそのバージョンに書き換えれば対応できる可能性があります。

またVisual Studio は標準のインストール先にあるものとして、``build.bat``は記述されています。
あえて違う場所にインストールしている場合は、``build.bat``はうまく動作しません。
``build.bat``を修正してインストール先に合わせるか、
```x64 Native Tools Command Prompt for VS 2019```等の開発用のコマンドプロンプトをスタートメニューから起動して、
``cmake``,``nmake``を``build.bat``の内容を参考にしながら手作業で実行してください。

###### CMake

[CMake](https://cmake.org/)がまだインストールされていない場合は、
[ホームページ](https://cmake.org/)からダウンロードするか、[Chocolatey](https://chocolatey.org/)を使って、インストールします。
ダウンロードしたインストーラでインストールする場合は、インストールオプションの画面でPATHを通すラジオボタンを必ずチェックしてください。
このときコマンドラインから利用するためPATHが通っている必要があります。手作業で後からPATHを通してもかまいません。
[Chocolatey](https://chocolatey.org/)を利用する場合は、``choco install cmake``でインストールされます。

###### WSL

ダウンロードと展開の自動化のために、WSLのbashを利用しています。わざわざこのためだけにインストールする必要はありません。
インストールされている場合は、標準的なコマンドの他に、``wget``、``unzip``、``patch``が必要なので、
``apt``や``zypper``などを使ってあらかじめ入れておいて下さい。


##### 必要なアーカイブの展開

WSLをインストールしない場合は次の作業をおこなってください。

次のアーカイブをダウンロードして同じ場所に展開して配置します。

* [open_jtalk-1.11](http://open-jtalk.sourceforge.net/)
* [hts_engine_API-1.10](http://hts-engine.sourceforge.net/)

さらに、同じ場所に``voice``フォルダを作り、その中に次のアーカイブをフォルダーごと展開します。

* [hts_voice_nitech_jp_atr503_m001-1.05](http://open-jtalk.sourceforge.net/)

必要に応じて、音響モデルファイルをこのフォルダに追加しておきます。

※2020/02/22 現在の各ファイルのバージョンは上記の通りですが、最新版では違っているかもしれません。
最新のものを使いたいときは、prepareとCMakeLists.txt内のバージョン番号の部分を書き換えてください。
大規模な変更が行われない限り対応できるでしょう。


### ビルド手順

共通の準備

* CMakeLists.txt
* build.bat/build
* prepare
* open_jtalk-1.11_mingw.patch

テスト用のスクリプト
* hello.bat/hello

#### Windows MSVC でのビルド手順

extra/open_jtalkにある次の3つのファイルを使います。これを同じフォルダに置いておきます。

* CMakeLists.txt
* build.bat
* prepare

##### WSLがインストールされている場合

build.batを実行します。あとはインストールまでやってくれます。

##### WSLがインストールされていない場合

上記の「必要なアーカイブの展開」の手順通りに、アーカイブをダウンロードし、展開します。

あとは、``build.bat``を実行すれば、インストールまでやってくれます。

#### MSYS2 の MinGW-W64 を使ったビルド

##### MSYS2の準備

* MSYS2のインストール
* 必要なパッケージのインストール

###### MSYS2のインストール

[MSYS2 homepage](http://www.msys2.org/) から、msys2-i686-xxxxxxxx.exe （xxxxxxxxは日付の数列）もしくは msys2-x86_64-xxxxxxxx.exe をダウンロードします。
それぞれ32ビットと64ビット版です。32ビットマシンでない限り、特にこだわりがなければ、64ビット版がいいでしょう。
セットアップの方法は割愛します。

###### 必要なパッケージのインストール

セットアップが終わったら、次のパッケージを追加でインストールします。
base-devel, mingw-w64-i686-toolchain, mingw-w64-x86_64-toolchain。
最後の2つは32ビット版、64ビット版のビルドコマンド群なので、両方もしくは必要な方を入れてください。

```bash:
pacman -S base-devel mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain
```

##### MSYS2でのビルド方法

環境が整ったら、開発用のコンソールを開きます。
64bit版を作りたいときは、Windowsのスタート画面から``MSYS2 MinGW 64-bit``、
32bit版ならば``MSYS2 MinGW 32-bit``です。

上記の``build``などがあるところをカレントディレクトリにして、
下記コマンドを実行すると、インストールまでやってくれます。

```bash:
bash build
```

### macOS でのビルド

#### 準備

cmake が必要です。

そのために、まずコマンドライン・デベロッパ・ツールをインストールします。
これは cmake をインストールするために使うHomebrewに必要だからです。

```bash:
xcode-select --install
```

次に、macOS用のパッケージマネージャーHomebrewをインストールします。
Homebrewを使わないcmakeをインストール方法もありますが、今回はこれを使います。
インストールの仕方と、使い方は、[Homebrew](https://brew.sh/index_ja.html)にあります。

ようやく、cmakeのインストールです。

```bash:
brew install cmake
```

#### macOSでのビルド及びインストール

上記のファイルのあるところで端末を開いて、次のコマンドを入力します。
ビルドが終了し、インストールが始まると、管理者パスワードが求められます。

```bash:
bash build
```

### Ubuntu

Linuxの例としてUbuntuでの方法を示します。
他のlinuxディストリビューションでも同様の処理を行えば、生成できるはずです。

#### Ubuntuでの準備

buildスクリプト内でsudoコマンドを使っています。利用可能にしておいてください。

また、wget,unzip,cmake,gcc,makeなどのコマンドも使えるようにしてください。
無ければ、次のコマンドでインストールしておきます。
```bash:
sudo apt install wget unzip cmake build-essential 
```

#### Ubuntuでのビルド及びインストール

スクリプトのある場所で端末を開き、次のコマンドを入力します。
ビルドが終了し、インストールが始まると、管理者パスワードが求められます。

```bash:
bash build
```

## インストールの内容

### インストールされるファイル

標準の設定では以下の場所に実行ファイルとデータが配置されます。

#### Windows10 MSVC、MinGW

実行ファイル
* C:\open_jtalk\bin\open_jtalk.exe
* C:\open_jtalk\bin\hts_engine.exe
* C:\open_jtalk\bin\mecab-dict-index.exe

音響モデルファイルを格納するフォルダ
* C:\open_jtalk\voice

MeCab形式の辞書ファイルを格納するフォルダ（文字セットによってどれか一つ、既定はdic_utf_8）
* C:\open_jtalk\dic_utf_8
* C:\open_jtalk\dic_shift_jis
* C:\open_jtalk\dic_euc_jp

※MinGWでは、実行ファイルは上記の他にMinGWのファイルシステムの中にもインストールされます。

#### macOs, Ubuntu

実行ファイル
* /usr/local/bin/open_jtalk
* /usr/local/bin/hts_engine
* /usr/local/bin/mecab-dict-index

音響モデルファイルを格納するフォルダ
* /usr/local/OpenJTalk/voice

MeCab形式の辞書ファイルを格納するフォルダ（文字セットによってどれか一つ、既定はdic_utf_8）
* /usr/local/OpenJTalk/dic_utf_8
* /usr/local/OpenJTalk/dic_shift_jis
* /usr/local/OpenJTalk/dic_euc_jp

### インストール先の変更

上記のように、MSVC、MinGWでの標準のインストール先は、実行ファイルは``C:\open_jtalk\bin\``、
データフォルダは``C:\open_jtalk\``です。
macOS、Linuxでの標準のインストール先は、実行ファイルは``/usr/local/bin/``、
データフォルダは``/usr/local/OpenJTalk/``です。

変更するには、次のキャッシュ変数をCMakeLists.txtの冒頭で定義しておきます。
実行ファイルのインストール先は``BIN_INSTALL_PREFIX``のbin、
データフォルダのインストール先は``DATA_INSTALL_PREFIX``です。

### 文字セットの変更

解釈するテキストの文字セットは標準でUTF-8にしています。
これを他の文字セットに変更することができます。

#### Windows10

``build.bat``の冒頭付近にある次の行で、utf_8、shift_jis、euc_jpを指定します。
```bash
set param="-Dcharset=utf_8"
```

#### MinGW、macOS、Linux

``build``スクリプトの冒頭付近にある次の行の2行目で配列の番号で文字コードを指定します。

```bash:
charsets=( "utf_8" "shift_jis" "euc_jp" )
charset=${charsets[0]}
```
## 動作確認

ビルドがうまくいったかを確認するためのスクリプトを用意しています。
* hello(.bat)

Windowsならば、hello.batをダブルクリックします。
（MinGWでも、MinGWのコンソールから実行するのではなく、hello.batをダブルクリックします。
helloスクリプトはLinuxやmacOS向けのデータの配置で書かれているためです。）

Windows以外ではhelloを使います。次のようにタイプし、エンターします。

```bash:
bash hello
```

実行すると、``test.wav``という名の音声ファイルが作成されます。
これを再生して確認して下さい。

文字コードを変更した場合、インストール先を変更した場合、データフォルダを変更した場合などは、
hello(.bat)のスクリプトの内容や保存する文字コードをそれに合わせて入力してください。


## アンインストール

単純にインストールしたファイルを削除するだけです。上記のインストール先を参考に削除してください。

MSVC以外の場合、cmakeの作業フォルダにある``install_manifest.txt``を残していれば、
インストールしたファイルを下のコマンドでアンインストールできます。

cmake作業フォルダがbuild.dirだった場合は次のようになります。
```bash:
cd build.dir
xargs rm < install_manifest.txt
```

## ライセンス

* [MIT ライセンス](https://opensource.org/licenses/MIT)です。
* このスクリプトに呼び出されて、作成される実行ファイル、データファイルは、それぞれのライセンスに従います。

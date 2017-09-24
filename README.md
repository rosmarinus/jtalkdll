# jtalkdll
OpenJTalk DLL 

これは音声合成システムOpen JTalk使って手軽に音声合成プログラミングするためのDLLです。記述言語はC言語です。
さらにWindowsでは、WSH（ウィンドウズスクリプトホスト）やC#言語等で利用しやすいようにC++/CLIでラップしたマネージDLLも生成します。(VSC++ビルドのみ)
また、他のプログラミング言語から使いやすくするために、それぞれの言語のFFI(多言語関数インターフェイス)を利用するコードをいくつか用意しています。

## 動作環境
* Windows 10
* MacOS Sierra
* Ubuntu (他のLinuxディストリビューションは未確認)

## ビルド

### Windows

VisualStudioC++、Mingw、Cygwinのコンパイラでビルドができます。ただし、Mingw、Cygwinではビルドが成功するというだけで、現時点ではVSC++版と同等な単一のdllができるわけではありません。マネージDLLを作成するためにも、WindowsではコンパイラはVSC++を使います。

マイクロソフトのサイトから無償の Visual Studio　2017 Comunity　Edition もしくは Visual Studio Build Tools 2017 をダウンロードし、インストールしておく必要があります。前者は高機能、後者はC/C++のコードのコンパイルだけを目的とした最小限の構成です。今回コードをコンパイルするためであれば後者でも十分です。


* あらかじめ、Visual Studio　2017　Comunity　Edition（もしくはそれ以上）をインストールしておきます。

* このリポジトリをダウンロードします。ZIPファイルでダウンロードした場合は、さらに適当な場所に展開します。


* スタートメニューを開き、 Visual Studio 2017 フォルダ内にある 「x64 Native Tools Command Prompt for VS 2017」あるいは「x86 Native Tools Command Prompt for VS 2017」を起動します。どれを選ぶかは64版を作るか、32版を作るかで決めます。

標準設定では、64ビットマシンでは64ビット版と32ビット版の両方のDLLを作成し、64ビット版を数字の付かない名前で複製します。片方だけが必要なときはabuild
* コマンドプロンプトに、build.bat をタイプしてエンターすると、ビルドが始まります。

* ビルドが成功すると、c:\hts_engine_API と c:\open_jtalk に生成物がコピーされているはずです。

（以下、執筆中）
### MacOS
### Ubuntu

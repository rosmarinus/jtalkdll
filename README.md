# jtalkdll
OpenJTalk DLL 

これは音声合成システムOpen JTalk使って手軽に音声合成プログラミングするためのC言語で書かれたDLLです。
さらにC#等で利用しやすいようにC++/CLIでラップしたマネージドDLLも生成します。
現時点ではまだ不完全ですが、MacOS の dylib や Linuxの so の生成も目標にしています。

## 動作環境
* Windows 10
* MacOS （予定）
* Ubuntu (予定)

## ビルド

### Windows

* あらかじめ、Visual Studio　2017　Comunity　Edition（もしくはそれ以上）をインストールしておきます。
(注意 VS2015 ではホスト環境を示す環境変数がないため、32ビット版しか作成できません。)

* このリポジトリをダウンロードします。ZIPファイルでダウンロードした場合は、さらに適当な場所に展開します。

* スタートメニューを開き、 Visual Studio 2017 フォルダ内にある 「x64 Native Tools Command Prompt for VS 2017」あるいは「x86 Native Tools Command Prompt for VS 2017」を起動します。どれを選ぶかは64版を作るか、32版を作るかで決めます。

* 開いたDOS窓において、CD コマンドを使って、ダウンロードしたリポジトリのあるフォルダに移動します。
なおマウスを使う場合は、コマンドプロンプトに「CD (スペース)」と打ち込んだ後、エクスプローラ上の件のフォルダをDOS窓にD&Dすれば、パスがコピーされるので楽です。

* コマンドプロンプトに、build.bat をタイプしてエンターすると、ビルドが始まります。

* ビルドが成功すると、c:\hts_engine_API と c:\open_jtalk に生成物がコピーされているはずです。

（以下、執筆中）
### MacOS
### Ubuntu

/*****************************************************************
** OpenJTalk.TTS
*/
#pragma once
#define DISABLE_JTALK_DLLMAIN
#include "jtalk.h"
#include <msclr/marshal.h>
//#include <std/string>

//using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;


namespace JTalkCom
{

#if defined(_x64_)
//	[assembly:AssemblyKeyFileAttribute("JTalkCOMx64.snk")];
#else
//	[assembly:AssemblyKeyFileAttribute("JTalkCOMx86.snk")];
#endif

/// <summary>
	/// 音響モデルファイルデータのインターフェイス
	/// </summary>
	[Guid("75CDB8C8-BF88-47AA-97FA-D88EF80EFEB8")]
	[InterfaceType(ComInterfaceType::InterfaceIsDual)] public interface class IVoiceFileInfo
	{
		property System::String ^Path { System::String ^get(); } property System::String ^Name { System::String ^get(); }
	};

	/// <summary>
	/// <para>音響モデルファイルデータクラス</para>
	/// </summary>
	[ComVisible(true)]
#if defined(_x64_)
	[Guid("B5E3B9E9-5029-4158-AE70-95A59996644E")]
#else
	[Guid("1B16AF23-BD83-4C49-A1B7-0A3A4F0ECFF5")]
#endif
	[ClassInterface(ClassInterfaceType::None)]
	[ComDefaultInterface(IVoiceFileInfo::typeid)] public ref class VoiceFileInfo : IVoiceFileInfo
	{

	internal:
		/// <summary>
		/// 音響モデルファイルのパス
		/// </summary>
		System::String ^
			path;

		/// <summary>
		/// 音響モデルファイルの拡張子を除いたファイル名
		/// </summary>
		System::String ^name;

	public:
		/// <summary>
		/// <para>オブジェクトの文字列化</para>
		/// <para>その音響モデルファイル名を返す</para>
		/// </summary>
		/// <returns>その音響モデルファイル名</returns>
		virtual System::String ^ToString() override
		{
			return this->name;
		}

		/// <summary>
		/// プロパティ：音響モデルファイルのパス
		/// </summary>
		property System::String ^Path {
			/// <summary>
			/// 音響モデルファイルのパスを取得する
			/// </summary>
			/// <returns></returns>
			virtual System::String ^get() {
				return this->path;
			}
		}

		/// <summary>
		/// 音響モデルファイルの名前（拡張子を除いた部分）
		/// </summary>
		property System::String ^Name {
			/// <summary>
			/// 音響モデルファイルの名前を取得する
			/// </summary>
			/// <returns></returns>
			virtual System::String ^get() {
				return this->name;
			}
		}

		/// <summary>
		/// <para>引数がString^型のコンストラクタ</para>
		/// </summary>
		/// <param name="path">音響モデルファイルのパス</param>
		/// <param name="name">音響モデルファイル名</param>
		VoiceFileInfo(System::String ^path, System::String ^name)
		{
			this->path = path;
			this->name = name;
		}

		/// <summary>
		/// <para>引数がchar*型のコンストラクタ</para>
		/// </summary>
		/// <param name="path">音響モデルファイルのパス</param>
		/// <param name="name">音響モデルファイル名</param>
		VoiceFileInfo(char *path, char *name)
		{
			this->path = gcnew System::String(path);
			this->name = gcnew System::String(name);
		}

		/// <summary>
		/// <para>引数がwchar_t*型のコンストラクタ</para>
		/// </summary>
		/// <param name="path">音響モデルファイルのパス</param>
		/// <param name="name">音響モデルファイル名</param>
		VoiceFileInfo(wchar_t *path, wchar_t *name)
		{
			this->path = gcnew System::String(path);
			this->name = gcnew System::String(name);
		}

		/// <summary>
		/// <para>引数がHtsVoiceFilelist*型のコンストラクタ</para>
		/// </summary>
		/// <param name="data">HtsVoiceFilelist*型の音響モデルファイルデータ</param>
		/// <returns></returns>
		VoiceFileInfo(HtsVoiceFilelist *data)
		{
			this->path = gcnew System::String((const wchar_t *)data->pathU16);
			this->name = gcnew System::String((const wchar_t *)data->nameU16);
		}

		/// <summary>
		/// <para>引数がchar*型1つのコンストラクタ</para>
		/// </summary>
		/// <param name="path">引数がchar*型の音響モデルファイルのパス文字列</param>
		/// <returns></returns>
		VoiceFileInfo(wchar_t *path)
		{
			this->path = gcnew System::String(path);
			this->name = IO::Path::GetFileNameWithoutExtension(this->path);
		}

		/// <summary>
		/// <para>引数がSystem::String^型1つのコンストラクタ</para>
		/// </summary>
		/// <param name="path">引数がSystem::String型の音響モデルファイルのパス文字列</param>
		/// <returns></returns>
		VoiceFileInfo(System::String ^path)
		{
			this->path = path;
			this->name = IO::Path::GetFileNameWithoutExtension(this->path);
		}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~VoiceFileInfo()
		{
			delete this->path;
			delete this->name;
			this->!VoiceFileInfo();
		}

		/// <summary>
		/// ファイナライザ
		/// </summary>
		!VoiceFileInfo()
		{
		}
	};

	/// <summary>
	/// 音響モデルファイルデータコレクションのインターフェイス
	/// </summary>
	[Guid("B73BF2FC-CA54-499E-B7B4-FDFE806E8CFA")]
	[InterfaceType(ComInterfaceType::InterfaceIsDual)] public interface class IVoiceCollection : ICollection<VoiceFileInfo ^>
	{
		property int Count { int get(); };
		System::String ^GetPath(int index);
		System::String ^GetName(int index);
		VoiceFileInfo ^GetItem(int index);
		System::Collections::IEnumerator ^GetEnumerator(void);
	};

	/// <summary>
	/// 音響モデルファイルデータコレクション
	/// </summary>
	[ComVisible(true)]
#if defined(_x64_)
	[Guid("ACFB19F9-F94D-45EA-B424-63B415503CB3")]
#else
	[Guid("D5AF2B5D-C932-4A4A-936A-C21CD82D41B2")]
#endif
	[ClassInterface(ClassInterfaceType::None)]
	[ComDefaultInterface(IVoiceCollection::typeid)] public ref class VoiceCollection : IVoiceCollection
	{

	internal:

		/// <summary>
		/// 音響モデルファイルデータのリスト
		/// </summary>
		System::Collections::Generic::List<VoiceFileInfo ^> ^
			m_list = nullptr;

		/// <summary>
		/// <para>音響モデルファイルのリストがNULLでないかチェックする・</para>
		/// <para>NULLならば例外を投げる。</para>
		/// </summary>
		void check_voicelist()
		{
			if (m_list == nullptr)
			{
				throw gcnew System::Exception("内部エラー：音響モデルファイルリストがNULLです。");
			}
		}

		/// <summary>
		/// <para>音響モデルファイルリストのインデックスの範囲をチェックする。</para>
		/// <para>範囲がならば、例外を投げる。</para>
		/// </summary>
		/// <param name="index"></param>
		void check_index(int index)
		{
			if (m_list == nullptr)
			{
				throw gcnew System::Exception("内部エラー：音響モデルファイルリストがNULLです。");
			}

			if (index < 0 || index >= m_list->Count)
			{
				throw gcnew System::Exception("インデックスが範囲外です。");
			}
		}

	public:
		/// <summary>
		/// リストから要素を削除する。
		/// </summary>
		/// <param name="item">削除する要素</param>
		/// <returns>削除が成功したかどうか</returns>
		virtual bool Remove(VoiceFileInfo ^item)
		{
			return m_list->Remove(item);
		}

		/// <summary>
		/// リストをコピーする
		/// </summary>
		/// <param name="arr">コピー先</param>
		/// <param name="arrIndex">コピーを開始するインデックス</param>
		virtual void CopyTo(array<VoiceFileInfo ^> ^arr, int arrIndex)
		{
			m_list->CopyTo(arr, arrIndex);
		}

		/// <summary>
		/// 指定要素を含むかどうかを調べる。
		/// </summary>
		/// <param name="item">調べる要素</param>
		/// <returns>含んでいるかどうか</returns>
		virtual bool Contains(VoiceFileInfo ^item)
		{
			return m_list->Contains(item);
		}

		/// <summary>
		/// 音響モデルファイルデータを追加する。
		/// </summary>
		/// <param name="item"></param>
		virtual void Add(VoiceFileInfo ^item)
		{
			return m_list->Add(item);
		}

		/// <summary>
		/// HtsVoiceFilelist型の要素から音響モデルファイルデータを作り、追加する。
		/// </summary>
		/// <param name="data"></param>
		virtual void Add(HtsVoiceFilelist *data)
		{
			VoiceFileInfo ^item = gcnew VoiceFileInfo(data);
			return m_list->Add(item);
		}

		/// <summary>
		/// このコレクションのがリードオンリー属性
		/// </summary>
		property bool IsReadOnly
		{
			/// <summary>
			/// <para>リードオンリー属性を取得する。</para>
			/// <para>偽に固定</para>
			/// </summary>
			/// <returns>偽</returns>
			virtual bool get()
			{
				return false;
			}
		}

		/// <summary>
		/// GetEnumeratorの実装
		/// </summary>
		/// <returns></returns>
		virtual System::Collections::IEnumerator ^GetEnumerator() = System::Collections::IEnumerable::GetEnumerator
		{
			return m_list->GetEnumerator();
		}

			/// <summary>
			/// Generic型指定のGetEnumeratorの実装
			/// </summary>
			/// <returns></returns>
			virtual System::Collections::Generic::IEnumerator<VoiceFileInfo ^> ^GetEnumerator_generic() = System::Collections::Generic::IEnumerable<VoiceFileInfo ^>::GetEnumerator
		{
			return m_list->GetEnumerator();
		}

			/// <summary>
			/// <para>デフォルトインデクサ</para>
			/// <para>インデックスを使って配列のように要素にアクセスする</para>
			/// </summary>
			property VoiceFileInfo ^default[int]
		{
			/// <summary>
			/// 要素を取得する
			/// </summary>
			/// <param name="index">インデックス</param>
			/// <returns>音響モデルファイルデータへのポインタ</returns>
			VoiceFileInfo ^get(int index) {
			check_voicelist();
			check_index(index);
			return m_list[index];
		}
		}

			/// <summary>
			/// 初期化配列有りのコンストラクタ
			/// </summary>
			/// <param name="data">初期化リスト</param>
			VoiceCollection(array<VoiceFileInfo ^> ^data)
		{
			m_list = gcnew System::Collections::Generic::List<VoiceFileInfo ^>();
			m_list->AddRange(data);
		}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		VoiceCollection()
		{
			m_list = gcnew System::Collections::Generic::List<VoiceFileInfo ^>();
		}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~VoiceCollection()
		{
			m_list->Clear();
		}

		/// <summary>
		/// ファイナライザ
		/// </summary>
		!VoiceCollection()
		{
		}

		/// <summary>
		/// 指定されたインデックスの音響モデルファイルデータを取得する
		/// </summary>
		/// <param name="index">配列添え字</param>
		/// <returns>音響モデルファイルデータ</returns>
		virtual VoiceFileInfo ^GetItem(int index) {
			check_voicelist();
			check_index(index);
			return m_list[index];
		}

		/// <summary>
		/// 指定されたインデックスの音響モデルファイルのパスを取得する
		/// </summary>
		/// <param name="index">配列添え字</param>
		/// <returns>パス文字列</returns>
		virtual System::String
			^ GetPath(int index) {
			check_voicelist();
			check_index(index);
			return m_list[index]->path;
		}

		/// <summary>
		/// 指定されたインデックスの音響モデルファイルの名前を取得する
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		virtual System::String
			^ GetName(int index) {
			check_voicelist();
			check_index(index);
			return m_list[index]->name;
		}

		/// <summary>
		/// 音響モデルファイルコレクションをクリアする
		/// </summary>
		virtual void Clear(void)
		{
			check_voicelist();
			m_list->Clear();
		}

		/// <summary>
		/// 音響モデルファイルコレクションの数
		/// </summary>
		property int Count
		{
			/// <summary>
			/// カスを数える
			/// </summary>
			/// <returns></returns>
			virtual int get()
			{
				check_voicelist();
				return m_list->Count;
			}
		}
	};

	/// <summary>
	/// <para>TTSのためのインターフェイス</para>
	/// </summary>
	[Guid("6EEAAEE7-DD5F-44DD-A32F-A30A148E6D7A")]
	[InterfaceType(ComInterfaceType::InterfaceIsDual)] public interface class IJTalkTTS
	{
		void Refresh();
		property unsigned int SamplingFrequency
		{
			void set(unsigned int value);
			unsigned int get();
		}
		property unsigned int S
		{
			void set(unsigned int value);
			unsigned int get();
		}
		property unsigned int Fperiod
		{
			void set(unsigned int value);
			unsigned int get();
		}
		property unsigned int P
		{
			void set(unsigned int value);
			unsigned int get();
		}
		property double Alpha
		{
			void set(double value);
			double get();
		}
		property double A
		{
			void set(double value);
			double get();
		}
		property double Beta
		{
			void set(double value);
			double get();
		}
		property double B
		{
			void set(double value);
			double get();
		}
		property double Speed
		{
			void set(double value);
			double get();
		}
		property double R
		{
			void set(double value);
			double get();
		}
		property double AdditionalHalfTone
		{
			void set(double value);
			double get();
		}
		property double Fm
		{
			void set(double value);
			double get();
		}
		property double MsdThreshold
		{
			void set(double value);
			double get();
		}
		property double U
		{
			void set(double value);
			double get();
		}
		property double GvWeightForSpectrum
		{
			void set(double value);
			double get();
		}
		property double Jm
		{
			void set(double value);
			double get();
		}
		property double GvWeightForLogF0
		{
			void set(double value);
			double get();
		}
		property double Jf
		{
			void set(double value);
			double get();
		}
		property double Volume
		{
			void set(double value);
			double get();
		}
		property double G
		{
			void set(double value);
			double get();
		}
		property System::String ^Dic { void set(System::String^ value); System::String^ get(); }
		property System::String ^VoiceDir {  void set(System::String^ value);  System::String^ get(); }
		property System::String ^VoiceName { void set(System::String^ value); System::String^ get(); }
		property System::String ^VoicePath { void set(System::String^ value); System::String^ get(); }
		property VoiceFileInfo ^Voice { void set(VoiceFileInfo^ value); VoiceFileInfo^ get(); }
		property bool IsSpeaking { bool get(); }
		property bool IsPaused { bool get(); }
		property bool IsFinished { bool get(); }
		property VoiceCollection ^Voices { VoiceCollection ^get(); }
		void SpeakSync(System::String ^text);
		void SpeakAsync(System::String ^text);
		void Say(System::String ^text);
		void SpeakToFile(System::String ^text, System::String ^file);
		void Pause();
		void PauseMethod();
		void Resume();
		void ResumeMethod();
		void Stop();
		void StopMethod();
		void WaitUntilDone();
		void Wait();
		void Wait(int duration);
	};

	/// <summary>
	/// TTSクラス
	/// </summary>
	[ProgId("JTalk.TTS")]
	[ComVisible(true)]
#if defined(_x64_)
	[Guid("69535960-A7A5-4E9B-8A14-C1CD4BD97734")]
#else
	[Guid("C23F68E0-7D76-4D63-8BA3-8942EE49C6DA")]
#endif
	[ClassInterface(ClassInterfaceType::None)]
	[ComDefaultInterface(IJTalkTTS::typeid)]
	/// <summary>
	/// OpenJTalk を用いた TextToSpeech を実現するクラス
	/// </summary>
	public ref class JTalkTTS : IJTalkTTS
	{

	internal:
		/// <summary>
		/// 下位関数で用いるデータ構造体
		/// </summary>
		OpenJTalk *m_openjtalk = NULL;

		/// <summary>
		/// <para>音響モデルファイルデータのコレクション</para>
		/// <para>VBScriptで For Each のグループに指定可能</para>
		/// <para>JScriptで Enumerator() の引数に指定可能</para>
		/// </summary>
		VoiceCollection ^m_voiceCollection = nullptr;

		/// <summary>
		/// <para>openjtalk 構造体へのポインタがNULLかどうか調べる。</para>
		/// <para>NULLならば例外を投げる。</para>
		/// </summary>
		void check_openjtalk_object()
		{
			if (!m_openjtalk)
			{
				throw gcnew System::Exception("内部エラー：構造体へのポインタがNULLです。");
			}
		}

		/// <summary>
		/// <para>音響モデルファイルデータのリストを生成する。</para>
		/// <para>使用後はdelete_voice_listを使って解放する。</para>
		/// </summary>
		void generate_voice_list()
		{
			check_openjtalk_object();

			if (m_voiceCollection)
			{
				m_voiceCollection->Clear();
			}
			else
			{
				m_voiceCollection = gcnew VoiceCollection();
			}

			HtsVoiceFilelist *htsvList = openjtalk_getHTSVoiceListU16(m_openjtalk);
			if (htsvList)
			{
				for (HtsVoiceFilelist *list = htsvList; list != NULL; list = list->succ)
				{
					m_voiceCollection->Add(list);
				}
				openjtalk_clearHTSVoiceList(m_openjtalk, htsvList);
			}
		}

		/// <summary>
		/// <para>音響モデルファイルデータのリストを解放する。</para>
		/// </summary>
		void delete_voice_list()
		{
			check_openjtalk_object();
			m_voiceCollection->Clear();
		}

		/// <summary>
		/// <para>自分自身のパスを伝える</para>
		/// </summary>
		void set_own_path()
		{
			String^ dll_path = Assembly::GetExecutingAssembly()->Location;
			String^ dll_dir  = IO::Path::GetDirectoryName(dll_path);
			const char *path = "";
			marshal_context ctx;
			if (dll_path != nullptr)
			{
				path = (const char *)ctx.marshal_as<const char *>(dll_dir);
				set_current_dll_path(path);
			}
		}

	public:
		/// <summary>
		/// <para>コンストラクタ</para>
		/// </summary>
		JTalkTTS(System::String ^voicePath, System::String ^dicPath, System::String ^voiceDirPath)
		{
			set_own_path();
			const char16_t *voicePathPtr = NULL;
			const char16_t *dicPathPtr = NULL;
			const char16_t *voiceDirPathPtr = NULL;

			marshal_context ctx;
			if (voicePath != nullptr)
			{
				voicePathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(voicePath);
			}
			if (dicPath != nullptr)
			{
				dicPathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(dicPath);
			}
			if (voiceDirPath != nullptr)
			{
				voiceDirPathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(voiceDirPath);
			}
			m_openjtalk = openjtalk_initializeU16(voicePathPtr, dicPathPtr, voiceDirPathPtr);
			check_openjtalk_object();
			generate_voice_list();
		}

		JTalkTTS(System::String ^voicePath, System::String ^dicPath)
		{
			set_own_path();
			const char16_t *voicePathPtr = NULL;
			const char16_t *dicPathPtr = NULL;
			marshal_context ctx;
			if (voicePath != nullptr)
			{
				voicePathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(voicePath);
			}
			if (dicPath != nullptr)
			{
				dicPathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(dicPath);
			}
			m_openjtalk = openjtalk_initializeU16(voicePathPtr, dicPathPtr, NULL);
			check_openjtalk_object();
			generate_voice_list();
		}

		JTalkTTS(System::String ^voicePath)
		{
			set_own_path();
			const char16_t *voicePathPtr = NULL;
			marshal_context ctx;
			if (voicePath != nullptr)
			{
				voicePathPtr = (const char16_t *)ctx.marshal_as<const wchar_t *>(voicePath);
			}
			m_openjtalk = openjtalk_initializeU16(voicePathPtr, NULL, NULL);
			check_openjtalk_object();
			generate_voice_list();
		}

		JTalkTTS()
		{
			set_own_path();
			m_openjtalk = openjtalk_initializeU16(NULL, NULL, NULL);
			check_openjtalk_object();
			generate_voice_list();
		}

		/// <summary>
		/// <para>JTalkTTSクラスのデストラクタ</para>
		/// </summary>
		~JTalkTTS()
		{
			delete_voice_list();
			delete m_voiceCollection;
			this->!JTalkTTS();
		}

		/// <summary>
		/// <para>JTalkTTSクラスのファイナライザ</para>
		/// </summary>
		!JTalkTTS()
		{
			check_openjtalk_object();
			openjtalk_clear(m_openjtalk);
		}

		/// <summary>
		/// <para>音響モデルディレクトリが変更された後の処理のイベントハンドラ</para>
		/// </summary>
		event EventHandler ^VoiceListChanged;

	protected:
		/// <summary>
		/// <para>音響モデルディレクトリが変更されたあとに行う処理 </para>
		/// <param name="e">イベント引数</param>
		/// </summary>
		void OnVoiceListChanged(EventArgs ^e)
		{
			VoiceListChanged(this, e);
		}

	public:
		/// <summary>
		/// <para>クラスのリフレッシュを行う。 </para>
		/// <para>・設定のクリア</para>
		/// <para>・設定ファイルがあれば再読み込み</para>
		/// </summary>
		virtual void Refresh()
		{
			check_openjtalk_object();
			openjtalk_refresh(m_openjtalk);
		}

		/// <summary>
		/// プロパティ：音響モデルファイルフォルダ
		/// </summary>
		property System::String ^VoiceDir {
			/// <summary>
			/// <para>音響モデルファイルフォルダの設定</para>
			/// </summary>
			/// <param name="path">パス文字列</param>
			virtual void set(System::String ^path)
			{
				check_openjtalk_object();

				if (!path)
				{
					throw gcnew System::Exception("音響モデルファイルフォルダを示す文字列へのポインタがNULLです。");
				}
				if (!Directory::Exists(path))
				{
					throw gcnew System::Exception("音響モデルファイルフォルダが見つかりません。");
				}
				delete_voice_list();
				marshal_context ctx;
				const wchar_t *str = ctx.marshal_as<const wchar_t *>(path);
				if (!openjtalk_setVoiceDirU16(m_openjtalk, (const char16_t *)str))
				{
					throw gcnew System::Exception("音響モデルファイルフォルダを設定できません。");
				}
				generate_voice_list();
				OnVoiceListChanged(EventArgs::Empty);
			}

			virtual System::String ^get() {
				check_openjtalk_object();
				System::String ^res = nullptr;
				char16_t path[MAX_PATH];
				wchar_t *temp = (wchar_t *)openjtalk_getVoiceDirU16(m_openjtalk, path);
				if (temp)
				{
					res = gcnew System::String(temp);
				}
				return res;
			}
		}

		/// <summary>
		/// <para>プロパティ（設定のみ）</para>
		/// <para>以降の発声で使用する音響モデルファイルの設定</para>
		/// <para>三通りの指定方法</para>
		/// <para>・絶対パス...ファイルの直接指定</para>
		/// <para>・相対パス（拡張子有り）...実行ファイルの位置基準での相対指定</para>
		/// <para>・名前のみ（拡張子無し）...音響モデルファイルフォルダ内を探索</para>
		/// <para>探索について：</para>
		/// <para>探索範囲に複数適合するものがあっても、最初に見つけたものとする。</para>
		/// <para>このときの探索順序はreaddirの処理に依存している。</para>
		/// <para>探索名にはワイルドカード（*?）を使用できる。</para>
		/// <para>探索対象が多いときは1000ファイル探索しても見つからなければ探索を中止する。</para>
		/// </summary>
		property VoiceFileInfo ^
			Voice {
			virtual void set(VoiceFileInfo ^info)
			{
				check_openjtalk_object();
				if (!info)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列へのポインタがNULLです。");
				}
				System::String ^path = info->path;
				if (!path)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列へのポインタがNULLです。");
				}
				if (path->Length == 0)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列が空です。");
				}
				marshal_context ctx;
				const wchar_t *str = ctx.marshal_as<const wchar_t *>(path);
				if (!openjtalk_setVoiceU16(m_openjtalk, (const char16_t *)str))
				{
					throw gcnew System::Exception("音響モデルファイルを設定できません。");
				}
			}

			virtual VoiceFileInfo ^get() {
				check_openjtalk_object();
				VoiceFileInfo ^res = nullptr;
				char16_t path[MAX_PATH];
				if (openjtalk_getVoicePathU16(m_openjtalk, path) == nullptr)
				{
					return nullptr;
				}
				char16_t name[MAX_PATH];
				if (openjtalk_getVoiceNameU16(m_openjtalk, name) == nullptr)
				{
					return nullptr;
				}
				return gcnew VoiceFileInfo((wchar_t *)path, (wchar_t *)name);
			}
		}

		property System::String ^
			VoiceName {
			/// <summary>
			/// <para>以降の発声で使用する音響モデルファイルの設定</para>
			/// </summary>
			/// <param name="path">音響モデルファイルを示す文字列</param>
			virtual void set(System::String ^name)
			{
				check_openjtalk_object();

				if (!name)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列へのポインタがNULLです。");
				}
				if (name->Length == 0)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列が空です。");
				}
				marshal_context ctx;
				const wchar_t *str = ctx.marshal_as<const wchar_t *>(name);
				if (!openjtalk_setVoiceU16(m_openjtalk, (const char16_t *)str))
				{
					throw gcnew System::Exception("音響モデルファイルを設定できません。");
				}
			}

			virtual System::String ^get() {
				check_openjtalk_object();
				System::String ^res = nullptr;
				char16_t path[MAX_PATH];
				wchar_t *temp = (wchar_t *)openjtalk_getVoiceU16(m_openjtalk, path);
				if (temp)
				{
					System::String ^path = gcnew System::String(temp);
					System::String ^name = IO::Path::GetFileNameWithoutExtension(path);
					res = name;
				}
				return res;
			}
		}

		property System::String ^
			VoicePath {
			/// <summary>
			/// <para>以降の発声で使用する音響モデルファイルの設定</para>
			/// </summary>
			/// <param name="path">音響モデルファイルを示す文字列</param>
			virtual void set(System::String ^path)
			{
				check_openjtalk_object();

				if (!path)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列へのポインタがNULLです。");
				}
				if (path->Length == 0)
				{
					throw gcnew System::Exception("音響モデルファイルを示す文字列が空です。");
				}
				marshal_context ctx;
				const wchar_t *str = ctx.marshal_as<const wchar_t *>(path);
				if (!openjtalk_setVoiceU16(m_openjtalk, (const char16_t *)str))
				{
					throw gcnew System::Exception("音響モデルファイルを設定できません。");
				}
			}

			virtual System::String ^get() {
				check_openjtalk_object();
				System::String ^res = nullptr;
				char16_t path[MAX_PATH];
				wchar_t *temp = (wchar_t *)openjtalk_getVoiceU16(m_openjtalk, path);
				if (temp)
				{
					res = gcnew System::String(temp);
				}
				return res;
			}
		}

		/// <summary>
		/// <para>プロパティ：辞書フォルダ</para>
		/// </summary>
		property System::String ^
			Dic {
			/// <summary>
			/// <para>辞書フォルダの設定</para>
			/// </summary>
			/// <param name="path">パス文字列</param>
			virtual void set(System::String ^path)
			{
				check_openjtalk_object();

				if (!path)
				{
					throw gcnew System::Exception("辞書フォルダを示す文字列へのポインタがNULLです。");
				}
				if (!Directory::Exists(path))
				{
					throw gcnew System::Exception("辞書フォルダが見つかりません。");
				}
				marshal_context ctx;
				const wchar_t *str = ctx.marshal_as<const wchar_t *>(path);
				if (!openjtalk_setDicU16(m_openjtalk, (const char16_t *)str))
				{
					throw gcnew System::Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
				}
			}

			virtual System::String ^get() {
				check_openjtalk_object();
				System::String ^res = nullptr;
				char16_t path[MAX_PATH];
				wchar_t *temp = (wchar_t *)openjtalk_getDicU16(m_openjtalk, path);
				if (temp)
				{
					res = gcnew System::String(temp);
				}
				return res;
			}
		}

		/// <summary>
		/// 利用可能な音響モデルファイルのコレクション
		/// </summary>
		property VoiceCollection ^
			Voices {
			/// <summary>
			/// 利用可能な音響モデルファイルコレクションを取得する
			/// </summary>
			/// <returns>利用可能な音響モデルファイルコレクション</returns>
			virtual VoiceCollection ^get() {
				return m_voiceCollection;
			}
		}

		/// <summary>
		/// <para>プロパティ：サンプリング周波数</para>
		/// </summary>
		property unsigned int SamplingFrequency
		{
			/// <summary>
			/// <para>サンプリング周波数の設定</para>
			/// </summary>
			/// <param name="value">値（1≦i）</param>
			virtual void set(unsigned int value)
			{
				check_openjtalk_object();

				if (value < 1)
				{
					throw gcnew System::Exception("sampling frequency の範囲は1以上の整数です。");
				}
				openjtalk_setSamplingFrequency(m_openjtalk, value);
			}

			virtual unsigned int get()
			{
				check_openjtalk_object();
				return openjtalk_getSamplingFrequency(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：サンプリング周波数</para>
		/// </summary>
		property unsigned int S
		{
			/// <summary>
			/// <para>サンプリング周波数の設定</para>
			/// </summary>
			/// <param name="value">値（1≦i）</param>
			virtual void set(unsigned int value)
			{
				SamplingFrequency = value;
			}

			virtual unsigned int get()
			{
				return SamplingFrequency;
			}
		}

		/// <summary>
		/// <para>プロパティ：フレームピリオド</para>
		/// </summary>
		property unsigned int Fperiod
		{
			/// <summary>
			/// <para>フレームピリオドの設定</para>
			/// </summary>
			/// <param name="value">値（1≦i）</param>
			virtual void set(unsigned int value)
			{
				check_openjtalk_object();

				if (value < 1)
				{
					throw gcnew System::Exception("frame period の範囲は1以上の整数です。");
				}
				openjtalk_setFperiod(m_openjtalk, value);
			}

			virtual unsigned int get()
			{
				check_openjtalk_object();
				return openjtalk_getFperiod(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：フレームピリオド</para>
		/// </summary>
		property unsigned int P
		{
			/// <summary>
			/// <para>フレームピリオドの設定</para>
			/// </summary>
			/// <param name="value">値（1≦i）</param>
			virtual void set(unsigned int value)
			{
				Fperiod = value;
			}

			virtual unsigned int get()
			{
				return Fperiod;
			}
		}

		/// <summary>
		/// <para>プロパティ：オールパス値</para>
		/// </summary>
		property double Alpha
		{
			/// <summary>
			/// <para>オールパス値の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();
				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。");
				}
				openjtalk_setAlpha(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getAlpha(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：オールパス値</para>
		/// </summary>
		property double A
		{
			/// <summary>
			/// <para>オールパス値の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				Alpha = value;
			}

			virtual double get()
			{
				return Alpha;
			}
		}

		/// <summary>
		/// <para>プロパティ：ポストフィルター係数</para>
		/// </summary>
		property double Beta
		{
			/// <summary>
			/// <para>ポストフィルター係数の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();

				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。");
				}
				openjtalk_setBeta(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getBeta(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：ポストフィルター係数</para>
		/// </summary>
		property double B
		{
			/// <summary>
			/// <para>ポストフィルター係数の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				Beta = value;
			}

			virtual double get()
			{
				return Beta;
			}
		}

		/// <summary>
		/// <para>プロパティ：スピーチ速度</para>
		/// </summary>
		property double Speed
		{
			/// <summary>
			/// <para>スピーチ速度の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();

				if (value < 0.0)
				{
					throw gcnew System::Exception("speech speed rate の範囲は0以上の浮動小数点数です。");
				}
				openjtalk_setSpeed(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getSpeed(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：スピーチ速度</para>
		/// </summary>
		property double R
		{
			/// <summary>
			/// <para>スピーチ速度の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				Speed = value;
			}

			virtual double get()
			{
				return Speed;
			}
		}

		/// <summary>
		/// <para>プロパティ：追加ハーフトーン</para>
		/// </summary>
		property double AdditionalHalfTone
		{
			/// <summary>
			/// <para>追加ハーフトーンの設定</para>
			/// </summary>
			/// <param name="value">値（d）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();
				openjtalk_setAdditionalHalfTone(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getAdditionalHalfTone(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：追加ハーフトーン</para>
		/// </summary>
		property double Fm
		{
			/// <summary>
			/// <para>追加ハーフトーンの設定</para>
			/// </summary>
			/// <param name="value">値（d）</param>
			virtual void set(double value)
			{
				AdditionalHalfTone = value;
			}

			virtual double get()
			{
				return AdditionalHalfTone;
			}
		}

		/// <summary>
		/// <para>プロパティ：有声 / 無声境界値</para>
		/// </summary>
		property double MsdThreshold
		{
			/// <summary>
			/// <para>有声 / 無声境界値数の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();

				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。");
				}
				openjtalk_setMsdThreshold(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getMsdThreshold(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：有声 / 無声境界値</para>
		/// </summary>
		property double U
		{
			/// <summary>
			/// <para>有声 / 無声境界値数の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				MsdThreshold = value;
			}

			virtual double get()
			{
				return MsdThreshold;
			}
		}

		/// <summary>
		/// <para>プロパティ：スペクトラム系列内変動の重み</para>
		/// </summary>
		property double GvWeightForSpectrum
		{
			/// <summary>
			/// <para>スペクトラム系列内変動の重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();

				if (value < 0.0)
				{
					throw gcnew System::Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。");
				}
				openjtalk_setGvWeightForSpectrum(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getGvWeightForSpectrum(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：スペクトラム系列内変動の重み</para>
		/// </summary>
		property double Jm
		{
			/// <summary>
			/// <para>スペクトラム系列内変動の重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				GvWeightForSpectrum = value;
			}

			virtual double get()
			{
				return GvWeightForSpectrum;
			}
		}

		/// <summary>
		/// <para>プロパティ：F0系列内変動重み</para>
		/// </summary>
		property double GvWeightForLogF0
		{
			/// <summary>
			/// <para>F0系列内変動重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();

				if (value < 0.0)
				{
					throw gcnew System::Exception("weight of GV for log F0 の範囲は0以上の浮動小数点数です。");
				}
				openjtalk_setGvWeightForLogF0(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getGvWeightForLogF0(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：F0系列内変動重み</para>
		/// </summary>
		property double Jf
		{
			/// <summary>
			/// <para>F0系列内変動重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				GvWeightForLogF0 = value;
			}

			virtual double get()
			{
				return GvWeightForLogF0;
			}
		}

		/// <summary>
		/// <para>プロパティ：ボリューム</para>
		/// </summary>
		property double Volume
		{
			/// <summary>
			/// <para>ボリュームの設定</para>
			/// </summary>
			/// <param name="value">値（d）</param>
			virtual void set(double value)
			{
				check_openjtalk_object();
				openjtalk_setVolume(m_openjtalk, value);
			}

			virtual double get()
			{
				check_openjtalk_object();
				return openjtalk_getVolume(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：ボリューム</para>
		/// </summary>
		property double G
		{
			/// <summary>
			/// <para>ボリュームの設定</para>
			/// </summary>
			/// <param name="value">値（d）</param>
			virtual void set(double value)
			{
				Volume = value;
			}

			virtual double get()
			{
				return Volume;
			}
		}

		/// <summary>
		/// <para>同期発声する。</para>
		/// <para>読み上げが終わるまで処理は戻らない。</para>
		/// <para>発声中ならば、それを終了させて発声する。</para>
		/// </summary>
		/// <param name="text">読み上げ文字列</param>
		virtual void SpeakSync(System::String ^text)
		{
			check_openjtalk_object();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			marshal_context ctx;
			const wchar_t *str = ctx.marshal_as<const wchar_t *>(text);
			openjtalk_speakSyncU16(m_openjtalk, (const char16_t *)str);
		}

		/// <summary>
		/// <para>非同期発声する。</para>
		/// <para>読み上げ開始後、処理が戻ってくる。</para>
		/// <para>発声中ならば、それを終了させて発声する。</para>
		/// </summary>
		/// <param name="text">読み上げ文字列</param>
		virtual void SpeakAsync(System::String ^text)
		{
			check_openjtalk_object();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			marshal_context ctx;
			const wchar_t *str = ctx.marshal_as<const wchar_t *>(text);
			openjtalk_speakAsyncU16(m_openjtalk, (const char16_t *)str);
		}

		/// <summary>
		/// <para>非同期発声する。</para>
		/// <para>SpeakAsyncの別名</para>
		/// <para>読み上げ開始後、処理が戻ってくる。</para>
		/// <para>発声中ならば、それを終了させて発声する。</para>
		/// </summary>
		/// <param name="text">読み上げ文字列</param>
		virtual void Say(System::String ^text)
		{
			check_openjtalk_object();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			marshal_context ctx;
			const wchar_t *str = ctx.marshal_as<const wchar_t *>(text);
			openjtalk_speakAsyncU16(m_openjtalk, (const char16_t *)str);
		}

		/// <summary>
		/// 非同期発声中ならば、発声を一時停止する。
		/// </summary>
		virtual void Pause()
		{
			check_openjtalk_object();
			openjtalk_pause(m_openjtalk);
		}

		// pause が予約語の言語のための別名
		virtual void PauseMethod()
		{
			Pause();
		}

		/// <summary>
		/// 非同期発声が一時停止中ならば、発声を再開する。
		/// </summary>
		virtual void Resume()
		{
			check_openjtalk_object();
			openjtalk_resume(m_openjtalk);
		}

		// Resume が予約語の言語のための別名
		virtual void ResumeMethod()
		{
			Resume();
		}

		/// <summary>
		/// 非同期発声中ならば、発声を強制停止する。
		/// </summary>
		virtual void Stop()
		{
			check_openjtalk_object();
			openjtalk_stop(m_openjtalk);
		}

		// Stop が予約語の言語のための別名
		virtual void StopMethod()
		{
			Stop();
		}

		/// <summary>
		/// <para>プロパティ：非同期発声中かどうか</para>
		/// </summary>
		property bool IsSpeaking
		{
			/// <summary>
			/// 発声中かどうか調べる
			/// </summary>
			/// <returns>発声中かどうかの真偽値</returns>
			virtual bool get()
			{
				check_openjtalk_object();
				return openjtalk_isSpeaking(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：非同期発声が一時停止中かどうか</para>
		/// </summary>
		property bool IsPaused
		{
			/// <summary>
			/// 一時停止中かどうかを調べる
			/// </summary>
			/// <returns>一時停止中かどうかの真偽値</returns>
			virtual bool get()
			{
				check_openjtalk_object();
				return openjtalk_isPaused(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>プロパティ：非同期発声が終了したかどうか</para>
		/// </summary>
		property bool IsFinished
		{
			/// <summary>
			/// 非同期発声が終了したかどうか
			/// </summary>
			/// <returns>終了したかどうかの真偽値</returns>
			virtual bool get()
			{
				check_openjtalk_object();
				return openjtalk_isFinished(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>非同期発声中ならば、停止するまで待機する。</para>
		/// <para>Wait(void)の別名</para>
		/// </summary>
		virtual void WaitUntilDone()
		{
			check_openjtalk_object();
			openjtalk_waitUntilDone(m_openjtalk);
		}

		/// <summary>
		/// <para>非同期発声中ならば、停止するまで待機する。</para>
		/// <para>WaitUntilDoneの別名</para>
		/// <para>Wait(void)の別名</para>
		/// </summary>
		virtual void WaitUntilFinished()
		{
			check_openjtalk_object();
			openjtalk_waitUntilDone(m_openjtalk);
		}

		/// <summary>
		/// <para>指定ミリ秒の間待機する。</para>
		/// <para>非同期発声は続く。</para>
		/// <para>durationが0のときは、WaitUntilDoneと同じ処理</para>
		/// </summary>
		/// <param name="duration">待機時間（ミリ秒）</param>
		virtual void Wait(int duration)
		{
			check_openjtalk_object();

			if (duration == 0)
			{
				openjtalk_waitUntilDone(m_openjtalk);
			}
			else
			{
				openjtalk_wait(m_openjtalk, duration);
			}
		}

		/// <summary>
		/// <para>発声している間待機する。</para>
		/// <para>非同期発声は続く。</para>
		/// <para>WaitUntilDoneの別名</para>
		/// </summary>
		virtual void Wait()
		{
			check_openjtalk_object();
			openjtalk_waitUntilDone(m_openjtalk);
		}

		/// <summary>
		/// <para>指定ファイルに文字列の音声を保存する</para>
		/// </summary>
		/// <param name="text">文字列</param>
		/// <param name="file">ファイル名</param>
		virtual void SpeakToFile(System::String ^text, System::String ^file)
		{
			check_openjtalk_object();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			if (text->Length == 0)
			{
				throw gcnew System::Exception("読み上げ文字列が空です。");
			}
			if (file->Length == 0)
			{
				throw gcnew System::Exception("ファイル名文字列が空です。");
			}
			marshal_context ctx;
			const wchar_t *str_text = ctx.marshal_as<const wchar_t *>(text);
			const wchar_t *str_file = ctx.marshal_as<const wchar_t *>(file);
			if (!openjtalk_speakToFileU16(m_openjtalk, (const char16_t *)str_text, (const char16_t *)str_file))
			{
				throw gcnew System::Exception("音声ファイルの作成中にエラーが発生しました。");
			}
		}
	};
}

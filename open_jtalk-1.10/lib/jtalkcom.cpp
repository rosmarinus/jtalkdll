/*****************************************************************
** OpenJTalk.TTS
*/
#pragma once
#include "jtalk.h"
#include <msclr/marshal.h>

using namespace System::IO;
using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace Openjtalk {

	/// <summary>
	/// 音響ファイルデータのインターフェイス
	/// </summary>
	[Guid("75CDB8C8-BF88-47AA-97FA-D88EF80EFEB8")]
	[InterfaceType(ComInterfaceType::InterfaceIsDual)]
	public interface class IVoiceFileData
	{
		property System::String^ Path {System::String^ get(); }
		property System::String^ Name {System::String^ get(); }
	};


	/// <summary>
	/// <para>音響ファイルデータクラス</para>
	/// </summary>
	[ComVisible(true)]
#if defined(_x64_)
	[Guid("B5E3B9E9-5029-4158-AE70-95A59996644E")]
#else
	[Guid("1B16AF23-BD83-4C49-A1B7-0A3A4F0ECFF5")]
#endif
	[ClassInterface(ClassInterfaceType::None)]
	[ComDefaultInterface(IVoiceFileData::typeid)]
	public ref class VoiceFileData : IVoiceFileData {

	internal:
		/// <summary>
		/// 音響ファイルのパス
		/// </summary>
		System::String^ path;

		/// <summary>
		/// 音響ファイルの拡張子を除いたファイル名
		/// </summary>
		System::String^ name;

	public:
		/// <summary>
		/// <para>オブジェクトの文字列化</para>
		/// <para>その音響ファイル名を返す</para>
		/// </summary>
		/// <returns>その音響ファイル名</returns>
		virtual System::String^ ToString() override
		{
			return this->name;
		}

		/// <summary>
		/// プロパティ：音響ファイルのパス
		/// </summary>
		property System::String^ Path
		{
			/// <summary>
			/// 音響ファイルのパスを取得する
			/// </summary>
			/// <returns></returns>
			virtual System::String ^get()
			{
				return this->path;
			}
		}

		/// <summary>
		/// 音響ファイルの名前（拡張子を除いた部分）
		/// </summary>
		property System::String^ Name
		{
			/// <summary>
			/// 音響ファイルの名前を取得する
			/// </summary>
			/// <returns></returns>
			virtual System::String ^get()
			{
				return this->name;
			}
		}

		/// <summary>
		/// <para>引数がString^型のコンストラクタ</para>
		/// </summary>
		/// <param name="path">音響ファイルのパス</param>
		/// <param name="name">音響ファイル名</param>
		VoiceFileData(System::String^path, System::String^name)
		{
			this->path = path;
			this->name = name;
		}

		/// <summary>
		/// <para>引数がchar*型のコンストラクタ</para>
		/// </summary>
		/// <param name="path">音響ファイルのパス</param>
		/// <param name="name">音響ファイル名</param>
		VoiceFileData(char*path, char*name)
		{
			this->path = gcnew System::String(path);
			this->name = gcnew System::String(name);
		}

		/// <summary>
		/// <para>引数がHtsVoiceFilelist*型のコンストラクタ</para>
		/// </summary>
		/// <param name="data">HtsVoiceFilelist*型の音響ファイルデータ</param>
		/// <returns></returns>
		VoiceFileData(HtsVoiceFilelist*data)
		{
			this->path = gcnew System::String(data->path);
			this->name = gcnew System::String(data->name);
		}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~VoiceFileData()
		{
			delete this->path;
			delete this->name;
			this->!VoiceFileData();
		}

		/// <summary>
		/// ファイナライザ
		/// </summary>
		!VoiceFileData()
		{
		}
	};

	/// <summary>
	/// 音響ファイルデータコレクションのインターフェイス
	/// </summary>
	[Guid("B73BF2FC-CA54-499E-B7B4-FDFE806E8CFA")]
	[InterfaceType(ComInterfaceType::InterfaceIsDual)]
	public interface class IVoiceCollection : ICollection<VoiceFileData^>
	{
		property int Count { int get(); };
		System::String^ GetPath(int index);
		System::String^ GetName(int index);
		VoiceFileData^ GetItem(int index);
		System::Collections::IEnumerator^ GetEnumerator(void);
	};

	/// <summary>
	/// 音響ファイルデータコレクション
	/// </summary>
	[ComVisible(true)]
#if defined(_x64_)
	[Guid("ACFB19F9-F94D-45EA-B424-63B415503CB3")]
#else
	[Guid("D5AF2B5D-C932-4A4A-936A-C21CD82D41B2")]
#endif
	[ClassInterface(ClassInterfaceType::None)]
	[ComDefaultInterface(IVoiceCollection::typeid)]
	public ref class VoiceCollection : IVoiceCollection {

	internal:

		/// <summary>
		/// 音響ファイルデータのリスト
		/// </summary>
		System::Collections::Generic::List<VoiceFileData^>^ m_list = nullptr;

		/// <summary>
		/// <para>音響ファイルのリストがNULLでないかチェックする・</para>
		/// <para>NULLならば例外を投げる。</para>
		/// </summary>
		void check_voicelist()
		{
			if (m_list == nullptr)
			{
				throw gcnew System::Exception("内部エラー：音響ファイルリストがNULLです。");
			}
		}

		/// <summary>
		/// <para>音響ファイルリストのインデックスの範囲をチェックする。</para>
		/// <para>範囲がならば、例外を投げる。</para>
		/// </summary>
		/// <param name="index"></param>
		void check_index(int index)
		{
			if (m_list == nullptr)
			{
				throw gcnew System::Exception("内部エラー：音響ファイルリストがNULLです。");
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
		virtual bool Remove(VoiceFileData^ item)
		{
			return m_list->Remove(item);
		}

		/// <summary>
		/// リストをコピーする
		/// </summary>
		/// <param name="arr">コピー先</param>
		/// <param name="arrIndex">コピーを開始するインデックス</param>
		virtual void CopyTo(array<VoiceFileData^>^ arr, int arrIndex)
		{
			m_list->CopyTo(arr, arrIndex);
		}

		/// <summary>
		/// 指定要素を含むかどうかを調べる。
		/// </summary>
		/// <param name="item">調べる要素</param>
		/// <returns>含んでいるかどうか</returns>
		virtual bool Contains(VoiceFileData^ item)
		{
			return m_list->Contains(item);
		}

		/// <summary>
		/// 音響ファイルデータを追加する。
		/// </summary>
		/// <param name="item"></param>
		virtual void Add(VoiceFileData^ item)
		{
			return m_list->Add(item);
		}

		/// <summary>
		/// HtsVoiceFilelist型の要素から音響ファイルデータを作り、追加する。
		/// </summary>
		/// <param name="data"></param>
		virtual void Add(HtsVoiceFilelist* data)
		{
			VoiceFileData^ item = gcnew VoiceFileData(data);
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
		virtual System::Collections::IEnumerator^ GetEnumerator()
			= System::Collections::IEnumerable::GetEnumerator
		{
			return m_list->GetEnumerator();
		}

		/// <summary>
		/// Generic型指定のGetEnumeratorの実装
		/// </summary>
		/// <returns></returns>
		virtual System::Collections::Generic::IEnumerator<VoiceFileData^>^ GetEnumerator_generic()
			= System::Collections::Generic::IEnumerable<VoiceFileData^>::GetEnumerator
		{
			return m_list->GetEnumerator();
		}

		/// <summary>
		/// <para>デフォルトインデクサ</para>
		/// <para>インデックスを使って配列のように要素にアクセスする</para>
		/// </summary>
		property VoiceFileData^ default[int]
		{
			/// <summary>
			/// 要素を取得する
			/// </summary>
			/// <param name="index">インデックス</param>
			/// <returns>音響ファイルデータへのポインタ</returns>
			VoiceFileData^ get(int index)
			{
			return m_list[index];
			}
		}

		/// <summary>
		/// 初期化配列有りのコンストラクタ
		/// </summary>
		/// <param name="data">初期化リスト</param>
		VoiceCollection(array<VoiceFileData^>^data)
		{
			m_list = gcnew System::Collections::Generic::List<VoiceFileData^>();
			m_list->AddRange(data);
		}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		VoiceCollection()
		{
			m_list = gcnew System::Collections::Generic::List<VoiceFileData^>();
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
		/// 指定されたインデックスの音響ファイルデータを取得する
		/// </summary>
		/// <param name="index">配列添え字</param>
		/// <returns>音響ファイルデータ</returns>
		virtual VoiceFileData^ GetItem(int index)
		{
			check_voicelist();
			check_index(index);
			return m_list[index];
		}

		/// <summary>
		/// 指定されたインデックスの音響ファイルのパスを取得する
		/// </summary>
		/// <param name="index">配列添え字</param>
		/// <returns>パス文字列</returns>
		virtual System::String^ GetPath(int index)
		{
			check_voicelist();
			check_index(index);
			return m_list[index]->path;
		}

		/// <summary>
		/// 指定されたインデックスの音響ファイルの名前を取得する
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		virtual System::String^ GetName(int index)
		{
			check_voicelist();
			check_index(index);
			return m_list[index]->name;
		}

		/// <summary>
		/// 音響ファイルコレクションをクリアする
		/// </summary>
		virtual void Clear(void)
		{
			check_voicelist();
			m_list->Clear();
		}

		/// <summary>
		/// 音響ファイルコレクションの数
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
	[InterfaceType(ComInterfaceType::InterfaceIsDual)]
	public interface class IJTalkTTS
	{
		void Refresh();
		property unsigned int SamplingFrequency {  void set(unsigned int value); }
		property unsigned int S { void set(unsigned int value); }
		property unsigned int Fperiod {  void set(unsigned int value); }
		property unsigned int P {  void set(unsigned int value); }
		property double Alpha {  void set(double value); }
		property double A {  void set(double value); }
		property double Beta {  void set(double value); }
		property double B {  void set(double value); }
		property double Speed {  void set(double value); }
		property double R {  void set(double value); }
		property double AdditionalHalfTone {  void set(double value); }
		property double FM {  void set(double value); }
		property double MSDThreshold {  void set(double value); }
		property double U {  void set(double value); }
		property double GVWeightForSpectrum {  void set(double value); }
		property double JM {  void set(double value); }
		property double GVWeightForLogF0 {  void set(double value); }
		property double JF {  void set(double value); }
		property double Volume {  void set(double value); }
		property double G {  void set(double value); }
		property System::String^ Dic { void set(System::String^ value); }
		property System::String^ VoiceDir {  void set(System::String^ value); }
		property System::String^ Voice { void set(System::String^ value); }
		void SpeakSync(System::String^ text);
		void SpeakAsync(System::String^ text);
		void Stop();
		property bool IsSpeaking {bool get(); }
		void WaitUntilDone();
		void Wait();
		void Wait(int duration);
		void SpeakToFile(System::String^ text, System::String^ file);
		property VoiceCollection ^Voices {VoiceCollection ^get(); }
	};

	/// <summary>
	/// TTSクラス
	/// </summary>
	[ProgId("OpenJTalk.TTS")]
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
	public ref class JTalkTTS : IJTalkTTS {

	internal:
		/// <summary>
		/// 下位関数で用いるデータ構造体
		/// </summary>
		OpenJTalk *m_openjtalk = NULL;

		/// <summary>
		/// <para>音響ファイルデータのコレクション</para>
		/// <para>VBScriptで For Each のグループに指定可能</para>
		/// <para>JScriptで Enumerator() の引数に指定可能</para>
		/// </summary>
		VoiceCollection^ m_voiceCollection = nullptr;

		/// <summary>
		/// <para>openjtalk 構造体へのポインタがNULLかどうか調べる。</para>
		/// <para>NULLならば例外を投げる。</para>
		/// </summary>
		void check_struct()
		{
			if (!m_openjtalk)
			{
				throw gcnew System::Exception("内部エラー：構造体へのポインタがNULLです。");
			}
		}

		/// <summary>
		/// <para>音響ファイルデータのリストを生成する。</para>
		/// <para>使用後はdelete_voice_listを使って解放する。</para>
		/// </summary>
		void generate_voice_list()
		{
			check_struct();

			if (m_voiceCollection)
			{
				m_voiceCollection->Clear();
			}
			else
			{
				m_voiceCollection = gcnew VoiceCollection();
			}

			HtsVoiceFilelist *result = OpenJTalk_getHTSVoiceList(m_openjtalk);
			if (result)
			{
				for (HtsVoiceFilelist* list = result; list != NULL; list = list->succ)
				{
					m_voiceCollection->Add(list);
				}
				OpenJTalk_clearHTSVoiceList(m_openjtalk, result);
			}
		}

		/// <summary>
		/// <para>音響ファイルデータのリストを解放する。</para>
		/// </summary>
		void delete_voice_list()
		{
			check_struct();
			m_voiceCollection->Clear();
		}

	public:

		/// <summary>
		/// <para>コンストラクタ</para>
		/// </summary>
		JTalkTTS()
		{
			m_openjtalk = OpenJTalk_initialize();
			check_struct();
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
			check_struct();

			OpenJTalk_clear(m_openjtalk);
		}

		/// <summary>
		/// <para>クラスのリフレッシュを行う。 </para>
		/// <para>・設定のクリア</para>
		/// <para>・設定ファイルがあれば再読み込み</para>
		/// </summary>
		virtual void Refresh()
		{
			check_struct();

			OpenJTalk_refresh(m_openjtalk);

		}

		/// <summary>
		/// 利用可能な音響ファイルのコレクション
		/// </summary>
		property VoiceCollection^ Voices
		{
			/// <summary>
			/// 利用可能な音響ファイルコレクションを取得する
			/// </summary>
			/// <returns>利用可能な音響ファイルコレクション</returns>
			virtual VoiceCollection^get()
			{
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
				check_struct();

				if (value < 1)
				{
					throw gcnew System::Exception("sampling frequency の値は1以上の整数です。");
				}
				OpenJTalk_setSamplingFrequency(m_openjtalk, value);
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
				check_struct();

				if (value < 1)
				{
					throw gcnew System::Exception("frame period の値は1以上の整数です。");
				}
				OpenJTalk_setFperiod(m_openjtalk, value);
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
				check_struct();

				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("all-pass constant の値は0と1の間の浮動小数点数です。");
				}
				OpenJTalk_setAlpha(m_openjtalk, value);
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
				check_struct();

				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("postfiltering coefficient の値は0と1の間の浮動小数点数です。");
				}
				OpenJTalk_setBeta(m_openjtalk, value);
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
				check_struct();

				if (value < 0.0)
				{
					throw gcnew System::Exception("speech speed rate の値は0以上の浮動小数点数です。");
				}
				OpenJTalk_setSpeed(m_openjtalk, value);
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
				check_struct();

				OpenJTalk_addHalfTone(m_openjtalk, value);
			}
		}

		/// <summary>
		/// <para>プロパティ：追加ハーフトーン</para>
		/// </summary>
		property double FM
		{
			/// <summary>
			/// <para>追加ハーフトーンの設定</para>
			/// </summary>
			/// <param name="value">値（d）</param>
			virtual void set(double value)
			{
				AdditionalHalfTone = value;
			}
		}

		/// <summary>
		/// <para>プロパティ：有声 / 無声境界値</para>
		/// </summary>
		property double MSDThreshold
		{
			/// <summary>
			/// <para>有声 / 無声境界値数の設定</para>
			/// </summary>
			/// <param name="value">値（0≦d≦1）</param>
			virtual void set(double value)
			{
				check_struct();

				if (value < 0.0 || value > 1.0)
				{
					throw gcnew System::Exception("voiced/unvoiced threshold の値は0と1の間の浮動小数点数です。");
				}
				OpenJTalk_setMsdThreshold(m_openjtalk, value);
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
				MSDThreshold = value;
			}
		}

		/// <summary>
		/// <para>系列内変動の重みを設定する</para>
		/// </summary>
		/// <param name="i">ストリームインデックス（i=0,1）</param>
		/// <param name="f">GV重み（0≦d）</param>
		virtual void  SetGVWeight(size_t i, double f)
		{
			check_struct();

			if (i != 0 && i != 1)
			{
				throw gcnew System::Exception("stream index の値は0か1の整数です。");
			}
			if (f < 0.0)
			{
				throw gcnew System::Exception("GV weight の値は0以上の浮動小数点数です。");
			}
			OpenJTalk_setGvWeight(m_openjtalk, i, f);
		}

		/// <summary>
		/// <para>プロパティ：スペクトラム系列内変動の重み</para>
		/// </summary>
		property double GVWeightForSpectrum
		{
			/// <summary>
			/// <para>スペクトラム系列内変動の重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				check_struct();

				if (value < 0.0)
				{
					throw gcnew System::Exception("weight of GV for spectrum の値は0以上の浮動小数点数です。");
				}
				OpenJTalk_setGvWeightForSpectrum(m_openjtalk, value);
			}
		}

		/// <summary>
		/// <para>プロパティ：スペクトラム系列内変動の重み</para>
		/// </summary>
		property double JM
		{
			/// <summary>
			/// <para>スペクトラム系列内変動の重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				GVWeightForSpectrum = value;
			}
		}

		/// <summary>
		/// <para>プロパティ：F0系列内変動重み</para>
		/// </summary>
		property double GVWeightForLogF0
		{
			/// <summary>
			/// <para>F0系列内変動重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				check_struct();

				if (value < 0.0)
				{
					throw gcnew System::Exception("weight of GV for log F0 の値は0以上の浮動小数点数です。");
				}
				OpenJTalk_setGvWeightForLogF0(m_openjtalk, value);
			}
		}

		/// <summary>
		/// <para>プロパティ：F0系列内変動重み</para>
		/// </summary>
		property double JF
		{
			/// <summary>
			/// <para>F0系列内変動重みの設定</para>
			/// </summary>
			/// <param name="value">値（0≦d）</param>
			virtual void set(double value)
			{
				GVWeightForLogF0 = value;
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
				check_struct();

				OpenJTalk_setVolume(m_openjtalk, value);
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
		}

		/// <summary>
		/// <para>プロパティ：辞書フォルダ</para>
		/// </summary>
		property System::String^ Dic
		{
			/// <summary>
			/// <para>辞書フォルダの設定</para>
			/// </summary>
			/// <param name="path">パス文字列</param>
			virtual void set(System::String^path)
			{
				check_struct();

				if (!path)
				{
					throw gcnew System::Exception("辞書フォルダを示す文字列へのポインタがNULLです。");
				}
				if (!Directory::Exists(path))
				{
					throw gcnew System::Exception("辞書フォルダが見つかりません。");
				}
				marshal_context ctx;
				const wchar_t*str = ctx.marshal_as<const wchar_t*>(path);
				if (!OpenJTalk_setDic_u16(m_openjtalk, (const char16_t*)str))
				{
					throw gcnew System::Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
				}
			}
		}

		/// <summary>
		/// プロパティ：音響ファイルフォルダ
		/// </summary>
		property System::String^ VoiceDir
		{
			/// <summary>
			/// <para>音響ファイルフォルダの設定</para>
			/// </summary>
			/// <param name="path">パス文字列</param>
			virtual void set(System::String^path)
			{
				check_struct();

				if (!path)
				{
					throw gcnew System::Exception("音響ファイルフォルダを示す文字列へのポインタがNULLです。");
				}
				if (!Directory::Exists(path))
				{
					throw gcnew System::Exception("音響ファイルフォルダが見つかりません。");
				}
				delete_voice_list();
				marshal_context ctx;
				const wchar_t* str = ctx.marshal_as<const wchar_t*>(path);
				if (!OpenJTalk_setVoiceDir_u16(m_openjtalk, (const char16_t*)str))
				{
					throw gcnew System::Exception("音響ファイルフォルダを設定できません。");
				}
				generate_voice_list();
			}
		}

		/// <summary>
		/// <para>プロパティ（設定のみ）</para>
		/// <para>以降の発声で使用する音響ファイルの設定</para>
		/// <para>三通りの指定方法</para>
		/// <para>・絶対パス...ファイルの直接指定</para>
		/// <para>・相対パス（拡張子有り）...音響ファイルフォルダ内での相対指定</para>
		/// <para>・名前のみ（拡張子無し）...音響ファイルフォルダ内を探索</para>
		/// <para>探索について：</para>
		/// <para>探索範囲に複数適合するものがあっても、最初に見つけたものとする。</para>
		/// <para>このときの探索順序はreaddirの処理に依存している。</para>
		/// <para>探索名にはワイルドカード（*?）を使用できる。</para>
		/// <para>探索対象が多いときは1000ファイル探索しても見つからなければ探索を中止する。</para>
		/// </summary>
		property System::String^ Voice
		{
			/// <summary>
			/// <para>以降の発声で使用する音響ファイルの設定</para>
			/// </summary>
			/// <param name="path">音響ファイルを示す文字列</param>
			virtual void set(System::String^path)
			{
				check_struct();

				if (!path)
				{
					throw gcnew System::Exception("音響ファイルを示す文字列へのポインタがNULLです。");
				}
				if (path->Length == 0)
				{
					throw gcnew System::Exception("音響ファイルを示す文字列が空です。");
				}
				marshal_context ctx;
				const wchar_t* str = ctx.marshal_as<const wchar_t*>(path);
				if (!OpenJTalk_setVoice_u16(m_openjtalk, (const char16_t*)str))
				{
					throw gcnew System::Exception("音響ファイルを設定できません。");
				}
			}
		}

		/// <summary>
		/// <para>同期発声する。</para>
		/// <para>読み上げが終わるまで処理は戻らない。</para>
		/// <para>発声中ならば、それを終了させて発声する。</para>
		/// </summary>
		/// <param name="text">読み上げ文字列</param>
		virtual void SpeakSync(System::String^ text)
		{
			check_struct();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			marshal_context ctx;
			const wchar_t* str = ctx.marshal_as<const wchar_t*>(text);
			OpenJTalk_speakSync_u16(m_openjtalk, (const char16_t*)str);
		}

		/// <summary>
		/// <para>非同期発声する。</para>
		/// <para>読み上げ開始後、処理が戻ってくる。</para>
		/// <para>発声中ならば、それを終了させて発声する。</para>
		/// </summary>
		/// <param name="text">読み上げ文字列</param>
		virtual void SpeakAsync(System::String^ text)
		{
			check_struct();

			if (!text)
			{
				throw gcnew System::Exception("読み上げ文字列へのポインタがNULLです。");
			}
			marshal_context ctx;
			const wchar_t* str = ctx.marshal_as<const wchar_t*>(text);
			OpenJTalk_speakAsync_u16(m_openjtalk, (const char16_t*)str);
		}

		/// <summary>
		/// 発声中ならば、発声を強制停止する。
		/// </summary>
		virtual void  Stop()
		{
			check_struct();

			OpenJTalk_stop(m_openjtalk);
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
				check_struct();

				return OpenJTalk_isSpeaking(m_openjtalk);
			}
		}

		/// <summary>
		/// <para>非同期発声中ならば、停止するまで待機する。</para>
		/// <para>Wait(void)の別名</para>
		/// </summary>
		virtual void WaitUntilDone()
		{
			check_struct();

			OpenJTalk_waitUntilDone(m_openjtalk);
		}

		/// <summary>
		/// <para>指定ミリ秒の間待機する。</para>
		/// <para>非同期発声は続く。</para>
		/// <para>durationが0のときは、WaitUntilDoneと同じ処理</para>
		/// </summary>
		/// <param name="duration">待機時間（ミリ秒）</param>
		virtual void Wait(int duration)
		{
			check_struct();

			if (duration == 0)
			{
				OpenJTalk_waitUntilDone(m_openjtalk);
			}
			else
			{
				OpenJTalk_wait(m_openjtalk, duration);
			}
		}

		/// <summary>
		/// <para>発声している間待機する。</para>
		/// <para>非同期発声は続く。</para>
		/// <para>WaitUntilDoneの別名</para>
		/// </summary>
		virtual void Wait()
		{
			check_struct();

			OpenJTalk_waitUntilDone(m_openjtalk);
		}

		/// <summary>
		/// <para>指定ファイルに文字列の音声を保存する</para>
		/// </summary>
		/// <param name="text">文字列</param>
		/// <param name="file">ファイル名</param>
		virtual void SpeakToFile(System::String^text, System::String^file)
		{
			check_struct();

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
			const wchar_t* str_text = ctx.marshal_as<const wchar_t*>(text);
			const wchar_t* str_file = ctx.marshal_as<const wchar_t*>(file);
			if (!OpenJTalk_speakToFile_u16(m_openjtalk, (const char16_t*)str_text, (const char16_t*)str_file))
			{
				throw gcnew System::Exception("音声ファイルの作成中にエラーが発生しました。");
			}
		}
	};
}


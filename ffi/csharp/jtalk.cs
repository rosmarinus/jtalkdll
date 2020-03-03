using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
#if (PLATFORMx86)
using SizeT = System.UInt32;
#else
using SizeT = System.UInt64;
#endif

namespace JTalkDll
{
    public class VoiceFileInfo
    {
        private String path;
        private String name;

        internal VoiceFileInfo(String pathStr = "", String nameStr = "")
        {
            if (pathStr == "")
            {
                this.path = "";
                this.name = "";
            }
            else if (nameStr == "")
            {
                this.path = pathStr;
                this.name = System.IO.Path.GetFileNameWithoutExtension(pathStr);
            }
            else
            {
                this.path = pathStr;
                this.name = nameStr;
            }
        }

        internal String Path
        {
            get
            {
                return path;
            }
        }

        internal String Name
        {
            get
            {
                return name;
            }
        }
    };


    public class JTalkTTS : IDisposable
    {
        private const String dll = "jtalk";

        private const int MAX_PATH = 260;
        private bool disposed = false;

        public event EventHandler VoiceListChanged;

        protected void OnVoiceListChanged(EventArgs e)
        {
            VoiceListChanged?.Invoke(this, e);
        }

        /// <summary>
        /// <para>コンストラクタ</para>
        /// </summary>
        public JTalkTTS(String voicePath = "", String dicPath = "", String voiceDirPath = "")
        {
            handle = openjtalk_initialize(voicePath, dicPath, voiceDirPath);
            check_openjtalk_object();
            generate_voice_list();
        }

        /// <summary>
        /// <para>JTalkTTSクラスのデストラクタ</para>
        /// </summary>
        ~JTalkTTS()
        {
            delete_voice_list();
            this.Dispose();
        }


        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                {
                    check_openjtalk_object();
                    openjtalk_clear(handle);
                }
                disposed = true;
            }
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private class HtsVoiceFilelist
        {
            public IntPtr succ;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string path;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string name;
        };

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_clearHTSVoiceList(IntPtr handle, IntPtr list);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getHTSVoiceListU16")]
        private extern static IntPtr openjtalk_getHTSVoiceList(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_initializeU16")]
        private extern static IntPtr openjtalk_initialize(String voice, String dic, String voiceDir);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_clear(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_refresh(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setSamplingFrequency(IntPtr handle, SizeT i);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static SizeT openjtalk_getSamplingFrequency(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setFperiod(IntPtr handle, SizeT i);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static SizeT openjtalk_getFperiod(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setAlpha(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getAlpha(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setBeta(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getBeta(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setSpeed(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getSpeed(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setAdditionalHalfTone(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getAdditionalHalfTone(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setMsdThreshold(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getMsdThreshold(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setGvWeightForSpectrum(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getGvWeightForSpectrum(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setGvWeightForLogF0(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getGvWeightForLogF0(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_setVolume(IntPtr handle, double f);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static double openjtalk_getVolume(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_setDicU16")]
        private extern static bool openjtalk_setDic(IntPtr handle, String path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getDicU16")]
        private extern static IntPtr openjtalk_getDic(IntPtr handle, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_setVoiceDirU16")]
        private extern static bool openjtalk_setVoiceDir(IntPtr handle, String path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getVoiceDirU16")]
        private extern static IntPtr openjtalk_getVoiceDir(IntPtr handle, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_setVoiceU16")]
        private extern static bool openjtalk_setVoice(IntPtr handle, String path);

        //[DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getVoiceU16")]
        //private extern static IntPtr openjtalk_getVoice(IntPtr handle, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_setVoiceNameU16")]
        private extern static bool openjtalk_setVoiceName(IntPtr handle, String path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getVoiceNameU16")]
        private extern static IntPtr openjtalk_getVoiceName(IntPtr handle, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_setVoicePathU16")]
        private extern static bool openjtalk_setVoicePath(IntPtr handle, String path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_getVoicePathU16")]
        private extern static IntPtr openjtalk_getVoicePath(IntPtr handle, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder path);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_speakSyncU16")]
        private extern static void openjtalk_speakSync(IntPtr handle, String text);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_speakAsyncU16")]
        private extern static void openjtalk_speakAsync(IntPtr handle, String text);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_pause(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_resume(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_stop(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static bool openjtalk_isSpeaking(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static bool openjtalk_isPaused(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static bool openjtalk_isFinished(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_waitUntilDone(IntPtr handle);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private extern static void openjtalk_wait(IntPtr handle, int duration);

        [DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, EntryPoint = "openjtalk_speakToFileU16")]
        private extern static bool openjtalk_speakToFile(IntPtr handle, String text, String file);

        //[DllImport(dll, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        //private extern static void openjtalk_test(IntPtr handle, IntPtr text);

        /// <summary>
        /// 下位関数で用いるデータ構造体へのポインタ
        /// </summary>
        private IntPtr handle = IntPtr.Zero;

        /// <summary>
        /// <para>音響モデルファイルデータのコレクション</para>
        /// <para>VBScriptで For Each のグループに指定可能</para>
        /// <para>JScriptで Enumerator() の引数に指定可能</para>
        /// </summary>
        private List<VoiceFileInfo> voices = new List<VoiceFileInfo>();

        /// <summary>
        /// <para>openjtalk 構造体へのポインタがnullかどうか調べる。</para>
        /// <para>nullならば例外を投げる。</para>
        /// </summary>
        private void check_openjtalk_object()
        {
            if (handle == IntPtr.Zero)
            {
                throw new Exception("内部エラー：構造体へのポインタがnullです。");
            }
        }

        /// <summary>
        /// <para>音響モデルファイルデータのリストを生成する。</para>
        /// <para>使用後はdelete_voice_listを使って解放する。</para>
        /// </summary>
        private void generate_voice_list()
        {
            check_openjtalk_object();
            if (voices != null)
            {
                voices.Clear();
            }
            else
            {
                voices = new List<VoiceFileInfo>();
            }
            var list = new HtsVoiceFilelist();
            var ptr = openjtalk_getHTSVoiceList(handle);
            var top = ptr;
            if (top != IntPtr.Zero)
            {
                do
                {
                    list = (HtsVoiceFilelist)Marshal.PtrToStructure(ptr, list.GetType());
                    var path = list.path.ToString();
                    var name = list.name.ToString();
                    voices.Add(new VoiceFileInfo(path, name));
                    ptr = list.succ;
                }
                while (ptr != IntPtr.Zero);
                openjtalk_clearHTSVoiceList(handle, top);
            }
        }

        /// <summary>
        /// <para>音響モデルファイルデータのリストを解放する。</para>
        /// </summary>
        private void delete_voice_list()
        {
            check_openjtalk_object();
            voices.Clear();
        }

        /// <summary>
        /// <para>クラスのリフレッシュを行う。 </para>
        /// <para>・設定のクリア</para>
        /// <para>・設定ファイルがあれば再読み込み</para>
        /// </summary>
        public void Refresh()
        {
            check_openjtalk_object();
            openjtalk_refresh(handle);
        }

        /// <summary>
        /// 利用可能な音響モデルファイルのコレクション
        /// </summary>
        public List<VoiceFileInfo> Voices
        {
            /// <summary>
            /// 利用可能な音響モデルファイルコレクションを取得する
            /// </summary>
            /// <returns>利用可能な音響モデルファイルコレクション</returns>
            get
            {
                return voices;
            }
        }

        /// <summary>
        /// <para>プロパティ：サンプリング周波数</para>
        /// </summary>
        public uint SamplingFrequency
        {
            /// <summary>
            /// <para>サンプリング周波数の設定</para>
            /// </summary>
            /// <param name="value">値（1≦i）</param>
            set
            {
                check_openjtalk_object();
                if (value < 1)
                {
                    throw new Exception("sampling frequency の範囲は1以上の整数です。");
                }
                openjtalk_setSamplingFrequency(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return (uint)openjtalk_getSamplingFrequency(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：サンプリング周波数</para>
        /// </summary>
        public uint S
        {
            /// <summary>
            /// <para>サンプリング周波数の設定</para>
            /// </summary>
            /// <param name="value">値（1≦i）</param>
            set
            {
                SamplingFrequency = value;
            }

            get
            {
                return SamplingFrequency;
            }
        }

        /// <summary>
        /// <para>プロパティ：フレームピリオド</para>
        /// </summary>
        public uint Fperiod
        {
            /// <summary>
            /// <para>フレームピリオドの設定</para>
            /// </summary>
            /// <param name="value">値（1≦i）</param>
            set
            {
                check_openjtalk_object();
                if (value < 1)
                {
                    throw new Exception("frame period の範囲は1以上の整数です。");
                }
                openjtalk_setFperiod(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return (uint)openjtalk_getFperiod(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：フレームピリオド</para>
        /// </summary>
        public uint P
        {
            /// <summary>
            /// <para>フレームピリオドの設定</para>
            /// </summary>
            /// <param name="value">値（1≦i）</param>
            set
            {
                Fperiod = value;
            }

            get
            {
                return Fperiod;
            }
        }

        /// <summary>
        /// <para>プロパティ：オールパス値</para>
        /// </summary>
        public double Alpha
        {
            /// <summary>
            /// <para>オールパス値の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                check_openjtalk_object();
                if (value < 0.0 || value > 1.0)
                {
                    throw new Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。");
                }
                openjtalk_setAlpha(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getAlpha(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：オールパス値</para>
        /// </summary>
        public double A
        {
            /// <summary>
            /// <para>オールパス値の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                Alpha = value;
            }

            get
            {
                return Alpha;
            }
        }

        /// <summary>
        /// <para>プロパティ：ポストフィルター係数</para>
        /// </summary>
        public double Beta
        {
            /// <summary>
            /// <para>ポストフィルター係数の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                check_openjtalk_object();
                if (value < 0.0 || value > 1.0)
                {
                    throw new Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。");
                }
                openjtalk_setBeta(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getBeta(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：ポストフィルター係数</para>
        /// </summary>
        public double B
        {
            /// <summary>
            /// <para>ポストフィルター係数の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                Beta = value;
            }

            get
            {
                return Beta;
            }
        }

        /// <summary>
        /// <para>プロパティ：スピーチ速度</para>
        /// </summary>
        public double Speed
        {
            /// <summary>
            /// <para>スピーチ速度の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                check_openjtalk_object();

                if (value < 0.0)
                {
                    throw new Exception("speech speed rate の範囲は0以上の浮動小数点数です。");
                }
                openjtalk_setSpeed(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getSpeed(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：スピーチ速度</para>
        /// </summary>
        public double R
        {
            /// <summary>
            /// <para>スピーチ速度の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                Speed = value;
            }

            get
            {
                return Speed;
            }
        }

        /// <summary>
        /// <para>プロパティ：追加ハーフトーン</para>
        /// </summary>
        public double AdditionalHalfTone
        {
            /// <summary>
            /// <para>追加ハーフトーンの設定</para>
            /// </summary>
            /// <param name="value">値（d）</param>
            set
            {
                check_openjtalk_object();
                openjtalk_setAdditionalHalfTone(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getAdditionalHalfTone(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：追加ハーフトーン</para>
        /// </summary>
        public double FM
        {
            /// <summary>
            /// <para>追加ハーフトーンの設定</para>
            /// </summary>
            /// <param name="value">値（d）</param>
            set
            {
                AdditionalHalfTone = value;
            }

            get
            {
                return AdditionalHalfTone;
            }

        }

        /// <summary>
        /// <para>プロパティ：有声 / 無声境界値</para>
        /// </summary>
        public double MSDThreshold
        {
            /// <summary>
            /// <para>有声 / 無声境界値数の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                check_openjtalk_object();
                if (value < 0.0 || value > 1.0)
                {
                    throw new Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。");
                }
                openjtalk_setMsdThreshold(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getMsdThreshold(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：有声 / 無声境界値</para>
        /// </summary>
        public double U
        {
            /// <summary>
            /// <para>有声 / 無声境界値数の設定</para>
            /// </summary>
            /// <param name="value">値（0≦d≦1）</param>
            set
            {
                MSDThreshold = value;
            }

            get
            {
                return MSDThreshold;
            }
        }

        /// <summary>
        /// <para>プロパティ：スペクトラム系列内変動の重み</para>
        /// </summary>
        public double GVWeightForSpectrum
        {
            /// <summary>
            /// <para>スペクトラム系列内変動の重みの設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                check_openjtalk_object();
                if (value < 0.0)
                {
                    throw new Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。");
                }
                openjtalk_setGvWeightForSpectrum(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getGvWeightForSpectrum(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：スペクトラム系列内変動の重み</para>
        /// </summary>
        public double JM
        {
            /// <summary>
            /// <para>スペクトラム系列内変動の重みの設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                GVWeightForSpectrum = value;
            }

            get
            {
                return GVWeightForSpectrum;
            }
        }

        /// <summary>
        /// <para>プロパティ：F0系列内変動重み</para>
        /// </summary>
        public double GVWeightForLogF0
        {
            /// <summary>
            /// <para>F0系列内変動重みの設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                check_openjtalk_object();
                if (value < 0.0)
                {
                    throw new Exception("weight of GV for log F0 の範囲は0以上の浮動小数点数です。");
                }
                openjtalk_setGvWeightForLogF0(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getGvWeightForLogF0(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：F0系列内変動重み</para>
        /// </summary>
        public double JF
        {
            /// <summary>
            /// <para>F0系列内変動重みの設定</para>
            /// </summary>
            /// <param name="value">値（0≦d）</param>
            set
            {
                GVWeightForLogF0 = value;
            }

            get
            {
                return GVWeightForLogF0;
            }
        }

        /// <summary>
        /// <para>プロパティ：ボリューム</para>
        /// </summary>
        public double Volume
        {
            /// <summary>
            /// <para>ボリュームの設定</para>
            /// </summary>
            /// <param name="value">値（d）</param>
            set
            {
                check_openjtalk_object();
                openjtalk_setVolume(handle, value);
            }

            get
            {
                check_openjtalk_object();
                return openjtalk_getVolume(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：ボリューム</para>
        /// </summary>
        public double G
        {
            /// <summary>
            /// <para>ボリュームの設定</para>
            /// </summary>
            /// <param name="value">値（d）</param>
            set
            {
                Volume = value;
            }

            get
            {
                return Volume;
            }
        }

        /// <summary>
        /// <para>プロパティ：辞書フォルダ</para>
        /// </summary>
        public String Dic
        {
            /// <summary>
            /// <para>辞書フォルダの設定</para>
            /// </summary>
            /// <param name="path">パス文字列</param>
            set
            {
                check_openjtalk_object();
                if (value == null)
                {
                    throw new Exception("辞書フォルダを示す文字列へのポインタがnullです。");
                }
                if (!Directory.Exists(value))
                {
                    throw new Exception("辞書フォルダが見つかりません。");
                }
                if (!openjtalk_setDic(handle, value))
                {
                    throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
                }
            }

            get
            {
                check_openjtalk_object();
                var buff = new StringBuilder(MAX_PATH);
                if (openjtalk_getDic(handle, buff) == IntPtr.Zero)
                {
                    return null;
                }
                return buff.ToString();
            }
        }

        /// <summary>
        /// プロパティ：音響モデルファイルフォルダ
        /// </summary>
        public String VoiceDir
        {
            /// <summary>
            /// <para>音響モデルファイルフォルダの設定</para>
            /// </summary>
            /// <param name="path">パス文字列</param>
            set
            {
                check_openjtalk_object();
                if (value == null)
                {
                    throw new Exception("音響モデルファイルフォルダを示す文字列へのポインタがnullです。");
                }
                if (!Directory.Exists(value))
                {
                    throw new Exception("音響モデルファイルフォルダが見つかりません。");
                }
                if (!openjtalk_setVoiceDir(handle, value))
                {
                    throw new Exception("音響モデルファイルフォルダを設定できません。");
                }
                generate_voice_list();
                OnVoiceListChanged(EventArgs.Empty);
            }

            get
            {
                check_openjtalk_object();
                var buff = new StringBuilder(MAX_PATH);
                if (openjtalk_getVoiceDir(handle, buff) == IntPtr.Zero)
                {
                    return null;
                }
                return buff.ToString();
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
        public VoiceFileInfo Voice
        {
            /// <summary>
            /// <para>以降の発声で使用する音響モデルファイルの設定</para>
            /// </summary>
            /// <param name="value">音響モデルファイルを示す文字列</param>
            set
            {
                check_openjtalk_object();
                if (value == null)
                {
                    throw new Exception("音響モデルファイル情報へのポインタがnullです。");
                }
                var path = value.Path;
                if (path.Length == 0)
                {
                    throw new Exception("音響モデルファイルを示す文字列が空です。");
                }
                if (!openjtalk_setVoice(handle, path))
                {
                    throw new Exception("音響モデルファイルを設定できません: " + path);
                }
            }

            get
            {
                check_openjtalk_object();
                var buffPath = new StringBuilder(MAX_PATH);
                if (openjtalk_getVoicePath(handle, buffPath) == IntPtr.Zero)
                {
                    return null;
                }
                var buffName = new StringBuilder(MAX_PATH);
                if (openjtalk_getVoiceName(handle, buffName) == IntPtr.Zero)
                {
                    return null;
                }
                //Console.WriteLine($"path: {buffPath.ToString()}");
                //Console.WriteLine($"name: {buffName.ToString()}");
                return new VoiceFileInfo(buffPath.ToString(), buffName.ToString());
            }
        }

        public String VoiceName
        {
            /// <summary>
            /// <para>以降の発声で使用する音響モデルファイルの設定</para>
            /// </summary>
            /// <param name="value">音響モデルファイルを示す文字列</param>
            set
            {
                check_openjtalk_object();
                if (value == null)
                {
                    throw new Exception("音響モデルファイルを示す文字列へのポインタがnullです。");
                }
                if (value.Length == 0)
                {
                    throw new Exception("音響モデルファイルを示す文字列が空です。");
                }
                if (!openjtalk_setVoiceName(handle, value))
                {
                    throw new Exception("音響モデルファイルを設定できません: " + value);
                }
            }

            get
            {
                check_openjtalk_object();
                var buff = new StringBuilder(MAX_PATH);
                if (openjtalk_getVoiceName(handle, buff) == IntPtr.Zero)
                {
                    return null;
                }
                return buff.ToString();
            }
        }

        public String VoicePath
        {
            /// <summary>
            /// <para>以降の発声で使用する音響モデルファイルの設定</para>
            /// </summary>
            /// <param name="value">音響モデルファイルを示す文字列</param>
            set
            {
                check_openjtalk_object();
                if (value == null)
                {
                    throw new Exception("音響モデルファイルを示す文字列へのポインタがnullです。");
                }
                if (value.Length == 0)
                {
                    throw new Exception("音響モデルファイルを示す文字列が空です。");
                }
                if (!openjtalk_setVoicePath(handle, value))
                {
                    throw new Exception("音響モデルファイルを設定できません: " + value);
                }
            }

            get
            {
                check_openjtalk_object();
                var buff = new StringBuilder(MAX_PATH);
                if (openjtalk_getVoicePath(handle, buff) == IntPtr.Zero)
                {
                    return null;
                }
                return buff.ToString();
            }
        }

        /// <summary>
        /// <para>同期発声する。</para>
        /// <para>読み上げが終わるまで処理は戻らない。</para>
        /// <para>発声中ならば、それを終了させて発声する。</para>
        /// </summary>
        /// <param name="text">読み上げ文字列</param>
        public void SpeakSync(String text)
        {
            check_openjtalk_object();
            if (text == null)
            {
                throw new Exception("読み上げ文字列へのポインタがnullです。");
            }
            openjtalk_speakSync(handle, text);
        }

        /// <summary>
        /// <para>非同期発声する。</para>
        /// <para>読み上げ開始後、処理が戻ってくる。</para>
        /// <para>発声中ならば、それを終了させて発声する。</para>
        /// </summary>
        /// <param name="text">読み上げ文字列</param>
        public void SpeakAsync(String text)
        {
            check_openjtalk_object();
            if (text == null)
            {
                throw new Exception("読み上げ文字列へのポインタがnullです。");
            }
            openjtalk_speakAsync(handle, text);
        }

        /// <summary>
        /// 発声を一時停止する。
        /// </summary>
        public void Pause()
        {
            check_openjtalk_object();
            openjtalk_pause(handle);
        }

        /// <summary>
        /// 発声を再開する。
        /// </summary>
        public void Resume()
        {
            check_openjtalk_object();
            openjtalk_resume(handle);
        }

        /// <summary>
        /// 発声中ならば、発声を強制停止する。
        /// </summary>
        public void Stop()
        {
            check_openjtalk_object();
            openjtalk_stop(handle);
        }

        /// <summary>
        /// <para>プロパティ：非同期発声中かどうか</para>
        /// </summary>
        public bool IsSpeaking
        {
            /// <summary>
            /// 発声中かどうか調べる
            /// </summary>
            /// <returns>発声中かどうかの真偽値</returns>
            get
            {
                check_openjtalk_object();
                return openjtalk_isSpeaking(handle);
            }
        }
        /// <summary>
        /// <para>プロパティ：非同期発声が一時停止中かどうか</para>
        /// </summary>
        public bool IsPaused
        {
            /// <summary>
            /// 一時停止中かどうか調べる
            /// </summary>
            /// <returns>一時停止中かどうかの真偽値</returns>
            get
            {
                check_openjtalk_object();
                return openjtalk_isPaused(handle);
            }
        }

        /// <summary>
        /// <para>プロパティ：非同期発声が完了したかどうか</para>
        /// </summary>
        public bool IsFinished
        {
            /// <summary>
            /// 発声が完了したかどうか調べる
            /// </summary>
            /// <returns>発声が完了したかかどうかの真偽値</returns>
            get
            {
                check_openjtalk_object();
                return openjtalk_isFinished(handle);
            }
        }

        /// <summary>
        /// <para>非同期発声中ならば、停止するまで待機する。</para>
        /// <para>Wait(void)の別名</para>
        /// </summary>
        public void WaitUntilDone()
        {
            check_openjtalk_object();
            openjtalk_waitUntilDone(handle);
        }

        /// <summary>
        /// <para>指定ミリ秒の間待機する。</para>
        /// <para>非同期発声は続く。</para>
        /// <para>durationが0のときは、WaitUntilDoneと同じ処理</para>
        /// </summary>
        /// <param name="duration">待機時間（ミリ秒）</param>
        public void Wait(int duration = 0)
        {
            check_openjtalk_object();
            if (duration == 0)
            {
                openjtalk_waitUntilDone(handle);
            }
            else
            {
                openjtalk_wait(handle, duration);
            }
        }

        /// <summary>
        /// <para>指定ファイルに文字列の音声を保存する</para>
        /// </summary>
        /// <param name="text">文字列</param>
        /// <param name="file">ファイル名</param>
        public void SpeakToFile(String text, String file)
        {
            check_openjtalk_object();
            if (text == null)
            {
                throw new Exception("読み上げ文字列へのポインタがnullです。");
            }
            if (file == null)
            {
                throw new Exception("ファイル名文字列へのポインタがnullです。");
            }
            if (text.Length == 0)
            {
                throw new Exception("読み上げ文字列が空です。");
            }
            if (file.Length == 0)
            {
                throw new Exception("ファイル名文字列が空です。");
            }
            if (!openjtalk_speakToFile(handle, text, file))
            {
                throw new Exception("音声ファイルの作成中にエラーが発生しました。");
            }
        }

    }
}

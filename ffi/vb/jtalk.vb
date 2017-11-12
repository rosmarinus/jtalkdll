Imports System
Imports System.IO
Imports System.Text
Imports System.Collections.Generic
Imports System.Runtime.InteropServices
#If (PLATFORMx86) Then
Imports SizeT = System.UInt32
#Else
Imports SizeT = System.UInt64
#End If

Namespace JTalkDll

    Public Delegate sub OnFinishedCallbackDelegate(paused As Boolean)

    Public Class VoiceFileInfo
        Private fpath As String
        Private fname As String

        Public Sub New(Optional pathStr As String = "", Optional nameStr As String = "")
            If (pathStr = "") Then
                Me.fpath = ""
                Me.fname = ""
            Else
                If (nameStr = "") Then
                    Me.fpath = pathStr
                    Me.fname = System.IO.Path.GetFileNameWithoutExtension(pathStr)
                Else
                    Me.fpath = pathStr
                    Me.fname = nameStr
                End If
            End If
        End Sub

        Public ReadOnly Property Path As String
            Get
                Return fpath
            End Get
        End Property

        Public ReadOnly Property Name As String
            Get
                Return fname
            End Get
        End Property
    End Class

    Public Class JTalkTTS
        Implements IDisposable

        Private Const MAX_PATH = 260
        Protected disposed As Boolean = False

        ' <summary>
        ' <para>コンストラクタ</para>
        ' </summary>
        Public Sub New(Optional voicePath As String = "", Optional dicPath As String = "", Optional voiceDirPath As String = "")
            handle = openjtalk_initializeU16(voicePath, dicPath, voiceDirPath)
            check_openjtalk_object()
            generate_voice_list()
        End Sub

        ' <summary>
        ' <para>JTalkTTSクラスのデストラクタ</para>
        ' </summary>
        Protected Overrides Sub Finalize()
            delete_voice_list()
            Me.Dispose()
        End Sub

        Public Sub Dispose() Implements IDisposable.Dispose
            Dispose(True)
            GC.SuppressFinalize(Me)
        End Sub

        Protected Overridable Sub Dispose(ByVal disposing As Boolean)
            If (Not Me.disposed) Then
                If (disposing) Then
                    check_openjtalk_object()
                    openjtalk_clear(handle)
                End If
                Me.disposed = True
            End If
        End Sub

        <StructLayout(LayoutKind.Sequential, CharSet:=CharSet.Unicode)> Private Class HtsVoiceFilelist
            Public succ As IntPtr
            <MarshalAs(UnmanagedType.LPWStr)> Public path As String
            <MarshalAs(UnmanagedType.LPWStr)> Public name As String
        End Class

        Declare Unicode Sub openjtalk_clearHTSVoiceList Lib "jtalk.dll" (handle As IntPtr, list As IntPtr)
        Declare Unicode Function openjtalk_getHTSVoiceListU16 Lib "jtalk.dll" (handle As IntPtr) As IntPtr
        Declare Unicode Function openjtalk_initializeU16 Lib "jtalk.dll" (voice As String, dic As String, voiceDir As String) As IntPtr
        Declare Unicode Sub openjtalk_clear Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_refresh Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_setSamplingFrequency Lib "jtalk.dll" (handle As IntPtr, i As SizeT)
        Declare Unicode Function openjtalk_getSamplingFrequency Lib "jtalk.dll" (handle As IntPtr) As SizeT
        Declare Unicode Sub openjtalk_setFperiod Lib "jtalk.dll" (handle As IntPtr, i As SizeT)
        Declare Unicode Function openjtalk_getFperiod Lib "jtalk.dll" (handle As IntPtr) As SizeT
        Declare Unicode Sub openjtalk_setAlpha Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getAlpha Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setBeta Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getBeta Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setSpeed Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getSpeed Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setAdditionalHalfTone Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getAdditionalHalfTone Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setMsdThreshold Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getMsdThreshold Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setGvWeightForSpectrum Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getGvWeightForSpectrum Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setGvWeightForLogF0 Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getGvWeightForLogF0 Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Sub openjtalk_setVolume Lib "jtalk.dll" (handle As IntPtr, f As Double)
        Declare Unicode Function openjtalk_getVolume Lib "jtalk.dll" (handle As IntPtr) As Double
        Declare Unicode Function openjtalk_setDicU16 Lib "jtalk.dll" (handle As IntPtr, path As String) As Boolean
        Declare Unicode Function openjtalk_getDicU16 Lib "jtalk.dll" (handle As IntPtr, <MarshalAsAttribute(UnmanagedType.LPWStr)> path As StringBuilder) As IntPtr
        Declare Unicode Function openjtalk_setVoiceDirU16 Lib "jtalk.dll" (handle As IntPtr, path As String) As Boolean
        Declare Unicode Function openjtalk_getVoiceDirU16 Lib "jtalk.dll" (handle As IntPtr, <MarshalAsAttribute(UnmanagedType.LPWStr)> path As StringBuilder) As IntPtr
        Declare Unicode Function openjtalk_setVoiceU16 Lib "jtalk.dll" (handle As IntPtr, path As String) As Boolean
        Declare Unicode Function openjtalk_getVoiceU16 Lib "jtalk.dll" (handle As IntPtr, <MarshalAsAttribute(UnmanagedType.LPWStr)> path As StringBuilder) As IntPtr
        Declare Unicode Function openjtalk_setVoiceNameU16 Lib "jtalk.dll" (handle As IntPtr, path As String) As Boolean
        Declare Unicode Function openjtalk_getVoiceNameU16 Lib "jtalk.dll" (handle As IntPtr, <MarshalAsAttribute(UnmanagedType.LPWStr)> path As StringBuilder) As IntPtr
        Declare Unicode Function openjtalk_setVoicePathU16 Lib "jtalk.dll" (handle As IntPtr, path As String) As Boolean
        Declare Unicode Function openjtalk_getVoicePathU16 Lib "jtalk.dll" (handle As IntPtr, <MarshalAsAttribute(UnmanagedType.LPWStr)> path As StringBuilder) As IntPtr
        Declare Unicode Sub openjtalk_speakSyncU16 Lib "jtalk.dll" (handle As IntPtr, text As String)
        Declare Unicode Sub openjtalk_speakAsyncU16 Lib "jtalk.dll" (handle As IntPtr, text As String)
        Declare Unicode Sub openjtalk_pause Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_resume Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_stop Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Function openjtalk_isSpeaking Lib "jtalk.dll" (handle As IntPtr) As Boolean
        Declare Unicode Function openjtalk_isPaused Lib "jtalk.dll" (handle As IntPtr) As Boolean
        Declare Unicode Function openjtalk_isFinished Lib "jtalk.dll" (handle As IntPtr) As Boolean
        Declare Unicode Sub openjtalk_setOnFinishedCallback Lib "jtalk.dll" (handle As IntPtr,callback As OnFinishedCallbackDelegate)
        Declare Unicode Sub openjtalk_waitUntilDone Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_waitUntilFinished Lib "jtalk.dll" (handle As IntPtr)
        Declare Unicode Sub openjtalk_wait Lib "jtalk.dll" (handle As IntPtr, duration As Integer)
        Declare Unicode Function openjtalk_speakToFileU16 Lib "jtalk.dll" (handle As IntPtr, text As String, file As String) As Boolean
        Declare Unicode Sub openjtalk_test Lib "jtalk.dll" (handle As IntPtr, text As IntPtr)

        ' <summary>
        ' 下位関数で用いるデータ構造体へのポインタ
        ' </summary>
        Private handle As IntPtr = IntPtr.Zero

        ' <summary>
        ' <para>音響モデルファイルデータのコレクション</para>
        ' <para>VBScriptで For Each のグループに指定可能</para>
        ' <para>JScriptで Enumerator() の引数に指定可能</para>
        ' </summary>
        Private fVoices As List(Of VoiceFileInfo) = New List(Of VoiceFileInfo)()

        ' <summary>
        ' <para>音響モデルディレクトリが変更された後のイベント</para>
        ' </summary>
        Public Event VoiceListChanged As EventHandler

        ' <summary>
        ' <para>openjtalk 音響モデルディレクトリが変更されたときの処理</para>
        ' <param name="e">イベント引数</param>
        ' </summary>
        Protected Overridable Sub OnVoiceListChanged(e As EventArgs)
            RaiseEvent VoiceListChanged(Me, e)
        End Sub


        ' <summary>
        ' <para>openjtalk 構造体へのポインタがNothingかどうか調べる。</para>
        ' <para>Nothingならば例外を投げる。</para>
        ' </summary>
        Private Sub check_openjtalk_object()
            If (handle = IntPtr.Zero) Then
                Throw New Exception("内部エラー：構造体へのポインタがNothingです。")
            End If
        End Sub


        ' <summary>
        ' <para>音響モデルファイルデータのリストを生成する。</para>
        ' <para>使用後はdelete_voice_listを使って解放する。</para>
        ' </summary>
        Private Sub generate_voice_list()
            check_openjtalk_object()
            If (fVoices Is Nothing) Then
                fVoices.Clear()
            Else
                fVoices = New List(Of VoiceFileInfo)()
            End If
            Dim List As HtsVoiceFilelist = New HtsVoiceFilelist()
            Dim ptr As IntPtr = openjtalk_getHTSVoiceListU16(handle)
            Dim top As IntPtr = ptr
            If (top <> IntPtr.Zero) Then
                Do
                    List = DirectCast(Marshal.PtrToStructure(ptr, GetType(HtsVoiceFilelist)), HtsVoiceFilelist)
                    Dim Path As String = List.path.ToString()
                    Dim name As String = List.name.ToString()
                    fVoices.Add(New VoiceFileInfo(Path, name))
                    ptr = List.succ
                Loop While ptr <> IntPtr.Zero
                openjtalk_clearHTSVoiceList(handle, top)
            End If
        End Sub


        ' <summary>
        ' <para>音響モデルファイルデータのリストを解放する。</para>
        ' </summary>
        Private Sub delete_voice_list()
            check_openjtalk_object()
            fVoices.Clear()
        End Sub


        ' <summary>
        ' <para>クラスのリフレッシュを行う。 </para>
        ' <para>・設定のクリア</para>
        ' <para>・設定ファイルがあれば再読み込み</para>
        ' </summary>
        Public Sub Refresh()
            check_openjtalk_object()
            openjtalk_refresh(handle)
        End Sub


        ' <summary>
        ' 利用可能な音響モデルファイルのコレクション
        ' </summary>
        Public ReadOnly Property Voices As List(Of VoiceFileInfo)
            ' <summary>
            ' 利用可能な音響モデルファイルコレクションを取得する
            ' </summary>
            ' <returns>利用可能な音響モデルファイルコレクション</returns>
            Get
                Return fVoices
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：サンプリング周波数</para>
        ' </summary>
        Public Property SamplingFrequency As ULong
            ' <summary>
            ' <para>サンプリング周波数の設定</para>
            ' </summary>
            ' <param name="value">値（1≦i）</param>
            Set(ByVal value As ULong)
                check_openjtalk_object()
                If (value < 1) Then
                    Throw New Exception("sampling frequency の範囲は1以上の整数です。")
                End If
                openjtalk_setSamplingFrequency(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getSamplingFrequency(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：サンプリング周波数</para>
        ' </summary>
        Public Property S As ULong
            ' <summary>
            ' <para>サンプリング周波数の設定</para>
            ' </summary>
            ' <param name="value">値（1≦i）</param>
            Set(ByVal value As ULong)
                SamplingFrequency = value
            End Set
            Get
                Return SamplingFrequency
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：フレームピリオド</para>
        ' </summary>
        Public Property Fperiod As ULong
            ' <summary>
            ' <para>フレームピリオドの設定</para>
            ' </summary>
            ' <param name="value">値（1≦i）</param>
            Set(ByVal value As ULong)
                check_openjtalk_object()
                If (value < 1) Then
                    Throw New Exception("frame period の範囲は1以上の整数です。")
                End If
                openjtalk_setFperiod(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getFperiod(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：フレームピリオド</para>
        ' </summary>
        Public Property P As ULong
            ' <summary>
            ' <para>フレームピリオドの設定</para>
            ' </summary>
            ' <param name="value">値（1≦i）</param>
            Set(ByVal value As ULong)
                Fperiod = value
            End Set
            Get
                Return Fperiod
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：オールパス値</para>
        ' </summary>
        Public Property Alpha As Double
            ' <summary>
            ' <para>オールパス値の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If (value < 0.0 Or value > 1.0) Then
                    Throw New Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。")
                End If
                openjtalk_setAlpha(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getAlpha(handle)
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：オールパス値</para>
        ' </summary>
        Public Property A As Double
            ' <summary>
            ' <para>オールパス値の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                Alpha = value
            End Set
            Get
                Return Alpha
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：ポストフィルター係数</para>
        ' </summary>
        Public Property Beta As Double
            ' <summary>
            ' <para>ポストフィルター係数の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If (value < 0.0 Or value > 1.0) Then
                    Throw New Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。")
                End If
                openjtalk_setBeta(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getBeta(handle)
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：ポストフィルター係数</para>
        ' </summary>
        Public Property B As Double
            ' <summary>
            ' <para>ポストフィルター係数の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                Beta = value
            End Set
            Get
                Return Beta
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：スピーチ速度</para>
        ' </summary>
        Public Property Speed As Double
            ' <summary>
            ' <para>スピーチ速度の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If value < 0.0 Then
                    Throw New Exception("speech speed rate の範囲は0以上の浮動小数点数です。")
                End If
                openjtalk_setSpeed(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getSpeed(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：スピーチ速度</para>
        ' </summary>
        Public Property R As Double
            ' <summary>
            ' <para>スピーチ速度の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                Speed = value
            End Set
            Get
                Return Speed
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：追加ハーフトーン</para>
        ' </summary>
        Public Property AdditionalHalfTone As Double
            ' <summary>
            ' <para>追加ハーフトーンの設定</para>
            ' </summary>
            ' <param name="value">値（d）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                openjtalk_setAdditionalHalfTone(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getAdditionalHalfTone(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：追加ハーフトーン</para>
        ' </summary>
        Public Property FM As Double
            ' <summary>
            ' <para>追加ハーフトーンの設定</para>
            ' </summary>
            ' <param name="value">値（d）</param>
            Set(ByVal value As Double)
                AdditionalHalfTone = value
            End Set
            Get
                Return AdditionalHalfTone
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：有声 / 無声境界値</para>
        ' </summary>
        Public Property MSDThreshold As Double
            ' <summary>
            ' <para>有声 / 無声境界値数の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If (value < 0.0 Or value > 1.0) Then
                    Throw New Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。")
                End If
                openjtalk_setMsdThreshold(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getMsdThreshold(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：有声 / 無声境界値</para>
        ' </summary>
        Public Property U As Double
            ' <summary>
            ' <para>有声 / 無声境界値数の設定</para>
            ' </summary>
            ' <param name="value">値（0≦d≦1）</param>
            Set(ByVal value As Double)
                MSDThreshold = value
            End Set
            Get
                Return MSDThreshold
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：スペクトラム系列内変動の重み</para>
        ' </summary>
        Public Property GVWeightForSpectrum As Double
            ' <summary>
            ' <para>スペクトラム系列内変動の重みの設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If (value < 0.0) Then
                    Throw New Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。")
                End If
                openjtalk_setGvWeightForSpectrum(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getGvWeightForSpectrum(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：スペクトラム系列内変動の重み</para>
        ' </summary>
        Public Property JM As Double
            ' <summary>
            ' <para>スペクトラム系列内変動の重みの設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                GVWeightForSpectrum = value
            End Set
            Get
                Return GVWeightForSpectrum
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：F0系列内変動重み</para>
        ' </summary>
        Public Property GVWeightForLogF0 As Double
            ' <summary>
            ' <para>F0系列内変動重みの設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                If (value < 0.0) Then
                    Throw New Exception("weight of GV for log F0 の範囲は0以上の浮動小数点数です。")
                End If
                openjtalk_setGvWeightForLogF0(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getGvWeightForLogF0(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：F0系列内変動重み</para>
        ' </summary>
        Public Property JF As Double
            ' <summary>
            ' <para>F0系列内変動重みの設定</para>
            ' </summary>
            ' <param name="value">値（0≦d）</param>
            Set(ByVal value As Double)
                GVWeightForLogF0 = value
            End Set
            Get
                Return GVWeightForLogF0
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：ボリューム</para>
        ' </summary>
        Public Property Volume As Double
            ' <summary>
            ' <para>ボリュームの設定</para>
            ' </summary>
            ' <param name="value">値（d）</param>
            Set(ByVal value As Double)
                check_openjtalk_object()
                openjtalk_setVolume(handle, value)
            End Set
            Get
                check_openjtalk_object()
                Return openjtalk_getVolume(handle)
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：ボリューム</para>
        ' </summary>
        Public Property G As Double
            ' <summary>
            ' <para>ボリュームの設定</para>
            ' </summary>
            ' <param name="value">値（d）</param>
            Set(ByVal value As Double)
                Volume = value
            End Set
            Get
                Return Volume
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ：辞書フォルダ</para>
        ' </summary>
        Public Property Dic As String
            ' <summary>
            ' <para>辞書フォルダの設定</para>
            ' </summary>
            ' <param name="path">パス文字列</param>
            Set(ByVal value As String)
                check_openjtalk_object()
                If (value = Nothing) Then
                    Throw New Exception("辞書フォルダを示す文字列へのポインタがNothingです。")
                End If
                If (Not Directory.Exists(value)) Then
                    Throw New Exception("辞書フォルダが見つかりません。")
                End If
                If (Not openjtalk_setDicU16(handle, value)) Then
                    Throw New Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。")
                End If
            End Set
            Get
                check_openjtalk_object()
                Dim buff As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getDicU16(handle, buff) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Return buff.ToString()
            End Get
        End Property


        ' <summary>
        ' プロパティ：音響モデルファイルフォルダ
        ' </summary>
        Public Property VoiceDir As String
            ' <summary>
            ' <para>音響モデルファイルフォルダの設定</para>
            ' </summary>
            ' <param name="path">パス文字列</param>
            Set(ByVal value As String)
                check_openjtalk_object()
                If (value = Nothing) Then
                    Throw New Exception("音響モデルファイルフォルダを示す文字列へのポインタがNothingです。")
                End If
                If (Not Directory.Exists(value)) Then
                    Throw New Exception("音響モデルファイルフォルダが見つかりません。")
                End If
                If (Not openjtalk_setVoiceDirU16(handle, value)) Then
                    Throw New Exception("音響モデルファイルフォルダを設定できません。")
                End If
                generate_voice_list()
                OnVoiceListChanged(EventArgs.Empty)
            End Set
            Get
                check_openjtalk_object()
                Dim buff As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getVoiceDirU16(handle, buff) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Return buff.ToString()
            End Get
        End Property


        ' <summary>
        ' <para>プロパティ（設定のみ）</para>
        ' <para>以降の発声で使用する音響モデルファイルの設定</para>
        ' <para>三通りの指定方法</para>
        ' <para>・絶対パス...ファイルの直接指定</para>
        ' <para>・相対パス（拡張子有り）...実行ファイルの位置基準での相対指定</para>
        ' <para>・名前のみ（拡張子無し）...音響モデルファイルフォルダ内を探索</para>
        ' <para>探索について：</para>
        ' <para>探索範囲に複数適合するものがあっても、最初に見つけたものとする。</para>
        ' <para>このときの探索順序はreaddirの処理に依存している。</para>
        ' <para>探索名にはワイルドカード（*?）を使用できる。</para>
        ' <para>探索対象が多いときは1000ファイル探索しても見つからなければ探索を中止する。</para>
        ' </summary>
        Public Property Voice As VoiceFileInfo
            ' <summary>
            ' <para>以降の発声で使用する音響モデルファイルの設定</para>
            ' </summary>
            ' <param name="value">音響モデルファイルを示す文字列</param>
            Set(ByVal value As VoiceFileInfo)
                check_openjtalk_object()
                If (value Is Nothing) Then
                    Throw New Exception("音響モデルファイル情報へのポインタがNothingです。")
                End If
                Dim path As String = value.Path
                If (path.Length = 0) Then
                    Throw New Exception("音響モデルファイルを示す文字列が空です。")
                End If
                If (Not openjtalk_setVoiceU16(handle, path)) Then
                    Throw New Exception("音響モデルファイルを設定できません: " + path)
                End If
            End Set
            Get
                check_openjtalk_object()
                Dim buffPath As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getVoicePathU16(handle, buffPath) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Dim buffName As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getVoiceNameU16(handle, buffName) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Return New VoiceFileInfo(buffPath.ToString(), buffName.ToString())
            End Get
        End Property

        Public Property VoiceName As String
            ' <summary>
            ' <para>以降の発声で使用する音響モデルファイルの設定</para>
            ' </summary>
            ' <param name="value">音響モデルファイルを示す文字列</param>
            Set(ByVal value As String)
                check_openjtalk_object()
                If (value = Nothing) Then
                    Throw New Exception("音響モデルファイルを示す文字列へのポインタがNothingです。")
                End If
                If (value.Length = 0) Then
                    Throw New Exception("音響モデルファイルを示す文字列が空です。")
                End If
                If (Not openjtalk_setVoiceNameU16(handle, value)) Then
                    Throw New Exception("音響モデルファイルを設定できません: " + value)
                End If
            End Set
            Get
                check_openjtalk_object()
                Dim buff As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getVoiceNameU16(handle, buff) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Return buff.ToString()
            End Get
        End Property

        Public Property VoicePath As String
            ' <summary>
            ' <para>以降の発声で使用する音響モデルファイルの設定</para>
            ' </summary>
            ' <param name="value">音響モデルファイルを示す文字列</param>
            Set(ByVal value As String)
                check_openjtalk_object()
                If (value = Nothing) Then
                    Throw New Exception("音響モデルファイルを示す文字列へのポインタがNothingです。")
                End If
                If (value.Length = 0) Then
                    Throw New Exception("音響モデルファイルを示す文字列が空です。")
                End If
                If (Not openjtalk_setVoicePathU16(handle, value)) Then
                    Throw New Exception("音響モデルファイルを設定できません: " + value)
                End If
            End Set
            Get
                check_openjtalk_object()
                Dim buff As StringBuilder = New StringBuilder(MAX_PATH)
                If (openjtalk_getVoicePathU16(handle, buff) = IntPtr.Zero) Then
                    Return Nothing
                End If
                Return buff.ToString()
            End Get
        End Property


        ' <summary>
        ' <para>同期発声する。</para>
        ' <para>読み上げが終わるまで処理は戻らない。</para>
        ' <para>発声中ならば、それを終了させて発声する。</para>
        ' </summary>
        ' <param name="text">読み上げ文字列</param>
        Public Sub SpeakSync(text As String)
            check_openjtalk_object()
            If (text = Nothing) Then
                Throw New Exception("読み上げ文字列へのポインタがNothingです。")
            End If
            openjtalk_speakSyncU16(handle, text)
        End Sub

        ' <summary>
        ' <para>非同期発声する。</para>
        ' <para>読み上げ開始後、処理が戻ってくる。</para>
        ' <para>発声中ならば、それを終了させて発声する。</para>
        ' </summary>
        ' <param name="text">読み上げ文字列</param>
        Public Sub SpeakAsync(text As String)
            check_openjtalk_object()
            If (text = Nothing) Then
                Throw New Exception("読み上げ文字列へのポインタがNothingです。")
            End If
            openjtalk_speakAsyncU16(handle, text)
        End Sub


        ' <summary>
        ' 非同期発声中ならば、発声を一時停止する。
        ' </summary>
        Public Sub PauseMethod()
            check_openjtalk_object()
            openjtalk_pause(handle)
        End Sub


        ' <summary>
        ' 一時停止中ならば、発声を再開する。
        ' </summary>
        Public Sub ResumeMethod()
            check_openjtalk_object()
            openjtalk_resume(handle)
        End Sub


        ' <summary>
        ' 発声中ならば、発声を強制停止する。
        ' </summary>
        Public Sub StopMethod()
            check_openjtalk_object()
            openjtalk_stop(handle)
        End Sub


        ' <summary>
        ' <para>プロパティ：非同期発声中かどうか</para>
        ' </summary>
        Public ReadOnly Property IsSpeaking() As Boolean
            ' <summary>
            ' 発声中かどうか調べる
            ' </summary>
            ' <returns>発声中かどうかの真偽値</returns>
            Get
                check_openjtalk_object()
                Return openjtalk_isSpeaking(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：非同期発声の一時停止中かどうか</para>
        ' </summary>
        Public ReadOnly Property IsPaused() As Boolean
            ' <summary>
            ' 一時停止中かどうか調べる
            ' </summary>
            ' <returns>一時停止中かどうかの真偽値</returns>
            Get
                check_openjtalk_object()
                Return openjtalk_isPaused(handle)
            End Get
        End Property

        ' <summary>
        ' <para>プロパティ：非同期発声が完了したかどうか</para>
        ' </summary>
        Public ReadOnly Property IsFinished() As Boolean
            ' <summary>
            ' 発声が完了したかどうか調べる
            ' </summary>
            ' <returns>完了したかどうかの真偽値</returns>
            Get
                check_openjtalk_object()
                Return openjtalk_isFinished(handle)
            End Get
        End Property


        Public WriteOnly Property OnFinishedCallback As OnFinishedCallbackDelegate
            ' <summary>
            ' 発声が完了したときのコールバックを定義する
            ' </summary>
            ' <returns>デリゲート</returns>
            Set(ByVal value As OnFinishedCallbackDelegate)
                check_openjtalk_object()
                openjtalk_setOnFinishedCallback(handle,value)
            End Set
        End Property

        ' <summary>
        ' <para>非同期発声中ならば、停止するまで待機する。</para>
        ' <para>Wait(sub)の別名</para>
        ' </summary>
        Public Sub WaitUntilDone()
            check_openjtalk_object()
            openjtalk_waitUntilDone(handle)
        End Sub

        ' <summary>
        ' <para>非同期発声中ならば、停止するまで待機する。</para>
        ' <para>WaitUntilDoneの別名</para>
        ' <para>Wait(sub)の別名</para>
        ' </summary>
        Public Sub WaitUntilFinished()
            check_openjtalk_object()
            openjtalk_waitUntilFinished(handle)
        End Sub

        ' <summary>
        ' <para>指定ミリ秒の間待機する。</para>
        ' <para>非同期発声は続く。</para>
        ' <para>durationが0のときは、WaitUntilDoneと同じ処理</para>
        ' </summary>
        ' <param name="duration">待機時間（ミリ秒）</param>
        Public Sub Wait(Optional duration As Integer = 0)
            check_openjtalk_object()
            If (duration = 0) Then
                openjtalk_waitUntilDone(handle)
            Else
                openjtalk_wait(handle, duration)
            End If
        End Sub


        ' <summary>
        ' <para>指定ファイルに文字列の音声を保存する</para>
        ' </summary>
        ' <param name="text">文字列</param>
        ' <param name="file">ファイル名</param>
        Public Sub SpeakToFile(text As String, file As String)
            check_openjtalk_object()
            If (text = Nothing) Then
                Throw New Exception("読み上げ文字列へのポインタがNothingです。")
            End If
            If (file = Nothing) Then
                Throw New Exception("ファイル名文字列へのポインタがNothingです。")
            End If
            If (text.Length = 0) Then
                Throw New Exception("読み上げ文字列が空です。")
            End If
            If (file.Length = 0) Then
                Throw New Exception("ファイル名文字列が空です。")
            End If
            If (Not openjtalk_speakToFileU16(handle, text, file)) Then
                Throw New Exception("音声ファイルの作成中にエラーが発生しました。")
            End If
        End Sub

    End Class
End Namespace
package com.github.rosmarinus.jtalk;

import com.sun.jna.Callback;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.EventListener;
import java.util.EventObject;
import java.util.List;
import javax.swing.event.EventListenerList;

/**
 * jtalk.dllをJNAを使ってアクセスするクラス
 */
public class JTalkJna {

    private final int MAXPATH = 260;

    /**
     * 音響モデルファイルのリストの変化リスナーインターフェイス
     */
    public interface VoiceListChangedListener extends EventListener {
        public void onVoiceListChanged(VoiceListChangedEvent e);
    }

    /**
     * 音響モデルファイルのリストの変化イベントクラス
     */
    public class VoiceListChangedEvent extends EventObject {

        /**
         * シリアルバージョンID
         */
        private static final long serialVersionUID = 6425805994959760145L;

        /**
         * 音響モデルファイルの変化イベントコンストラクタ
         *
         * @param source ソースオブジェクト
         */
        public VoiceListChangedEvent(final Object source) {
            super(source);
        }
    }

    /**
     * 音響モデル変化イベントリスナーのリスト
     */
    protected EventListenerList listenerList = new EventListenerList();

    /**
     * 音響モデルファイルの変化リスナーを追加する
     *
     * @param listener 音響モデルファイルの変化リスナー
     */
    public void addVoiceListChangedListener(final VoiceListChangedListener listener) {
        listenerList.add(VoiceListChangedListener.class, listener);
    }

    /**
     * 音響モデルファイルの変化リスナーを削除する
     *
     * @param listener 音響モデルファイルの変化リスナー
     */
    public void removeVoiceListChangedListener(final VoiceListChangedListener listener) {
        listenerList.remove(VoiceListChangedListener.class, listener);
    }

    /**
     * 音響モデルファイルのリストが変化したときの処理を行う
     */
    public void fireVoiceListChanged() {
        final var e = new VoiceListChangedEvent(this);
        for (final var listener : listenerList.getListeners(VoiceListChangedListener.class)) {
            listener.onVoiceListChanged(e);
        }
    }

    /**
     * DLLで定義されている音響モデルリスト構造体を反映したJNAクラス
     */
    public static class HTSVoiceList extends Structure {
        public HTSVoiceList.ByReference succ;
        public String path;
        public String name;

        public HTSVoiceList() {
            super(Structure.ALIGN_NONE);
        }

        protected List<String> getFieldOrder() {
            return Arrays.asList(new String[] { "succ", "path", "name" });
        }

        public static class ByReference extends HTSVoiceList implements Structure.ByReference {
        }
        // public static class ByValue extends HTSVoiceList implements Structure.ByValue
        // { }
    }

    /**
     * 読み上げ完了時に発動されるメソッドを持ったインターフェイス
     */
    public interface OnSpeakingFinishedCallbackProc extends Callback {
        public void invoke();
    }

    /**
     * DLLで定義されている関数を反映したJNAインターフェイス
     */
    public interface API extends Library {

        API INSTANCE = (API) Native.load("jtalk", API.class);

        void openjtalk_clearHTSVoiceList(Pointer handle, HTSVoiceList list);

        // HTSVoiceList openjtalk_getHTSVoiceList(Pointer handle);

        Pointer openjtalk_initialize(String voicePath, String dicPath, String voiceDirPath);

        void openjtalk_clear(Pointer handle);

        // void openjtalk_refresh(Pointer handle);

        void openjtalk_setSamplingFrequency(Pointer handle, int i);

        int openjtalk_getSamplingFrequency(Pointer handle);

        void openjtalk_setFperiod(Pointer handle, int i);

        int openjtalk_getFperiod(Pointer handle);

        void openjtalk_setAlpha(Pointer handle, double f);

        double openjtalk_getAlpha(Pointer handle);

        void openjtalk_setBeta(Pointer handle, double f);

        double openjtalk_getBeta(Pointer handle);

        void openjtalk_setSpeed(Pointer handle, double f);

        double openjtalk_getSpeed(Pointer handle);

        void openjtalk_setAdditionalHalfTone(Pointer handle, double f);

        double openjtalk_getAdditionalHalfTone(Pointer handle);

        void openjtalk_setMsdThreshold(Pointer handle, double f);

        double openjtalk_getMsdThreshold(Pointer handle);

        void openjtalk_setGvWeightForSpectrum(Pointer handle, double f);

        double openjtalk_getGvWeightForSpectrum(Pointer handle);

        void openjtalk_setGvWeightForLogF0(Pointer handle, double f);

        double openjtalk_getGvWeightForLogF0(Pointer handle);

        void openjtalk_setVolume(Pointer handle, double f);

        double openjtalk_getVolume(Pointer handle);

        boolean openjtalk_setDic(Pointer handle, String path);

        int openjtalk_setDic2(Pointer handle, String path);

        String openjtalk_getDic(Pointer handle, byte[] path);

        boolean openjtalk_setVoiceDir(Pointer handle, String path);

        int openjtalk_setVoiceDir2(Pointer handle, String path);

        String openjtalk_getVoiceDir(Pointer handle, byte[] path);

        boolean openjtalk_setVoice(Pointer handle, String path);

        int openjtalk_setVoice2(Pointer handle, String path);

        boolean openjtalk_setVoicePath(Pointer handle, String path);

        int openjtalk_setVoicePath2(Pointer handle, String path);

        String openjtalk_getVoicePath(Pointer handle, byte[] path);

        boolean openjtalk_setVoiceName(Pointer handle, String path);

        int openjtalk_setVoiceName2(Pointer handle, String path);

        String openjtalk_getVoiceName(Pointer handle, byte[] path);

        void openjtalk_speakSync(Pointer handle, String text);

        void openjtalk_speakAsync(Pointer handle, String text);

        void openjtalk_pause(Pointer handle);

        void openjtalk_resume(Pointer handle);

        void openjtalk_stop(Pointer handle);

        boolean openjtalk_isSpeaking(Pointer handle);

        int openjtalk_isSpeaking2(Pointer handle);

        boolean openjtalk_isPaused(Pointer handle);

        int openjtalk_isPaused2(Pointer handle);

        boolean openjtalk_isFinished(Pointer handle);

        int openjtalk_isFinished2(Pointer handle);

        void openjtalk_waitUntilDone(Pointer handle);

        void openjtalk_wait(Pointer handle, int duration);

        boolean openjtalk_speakToFile(Pointer handle, String text, String file);

        int openjtalk_speakToFile2(Pointer handle, String text, String file);

        void openjtalk_setOnFinishedCallback(Pointer handle, OnSpeakingFinishedCallbackProc callback);

        void openjtalk_setVerbose(boolean sw);

        // int openjtalk_getCharCode(String text);
    }

    private Pointer handle = null;
    private ArrayList<VoiceFileInfo> voices = new ArrayList<VoiceFileInfo>();

    /**
     * jtalk.dllをJNAを使ってアクセスするクラスのコンストラクタ（3引数）
     *
     * @param voicePath    音響モデルファイルのパスもしくは名前
     * @param dicPath      辞書ディレクトリのパス
     * @param voiceDirPath 音響モデルディレクトリ
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public JTalkJna(final String voicePath, final String dicPath, final String voiceDirPath) throws Exception {
        System.setProperty("jna.encoding", "UTF-8");
        handle = API.INSTANCE.openjtalk_initialize(voicePath, dicPath, voiceDirPath);
        generateVoiceList();
    }

    /**
     * jtalk.dllをJNAを使ってアクセスするクラスのコンストラクタ（2引数）
     *
     * @param voicePath 音響モデルファイルのパスもしくは名前
     * @param dicPath   辞書ディレクトリのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public JTalkJna(final String voicePath, final String dicPath) throws Exception {
        this(voicePath, dicPath, null);
    }

    /**
     * jtalk.dllをJNAを使ってアクセスするクラスのコンストラクタ（1引数）
     *
     * @param voicePath 音響モデルファイルのパスもしくは名前
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public JTalkJna(final String voicePath) throws Exception {
        this(voicePath, null, null);
    }

    /**
     * jtalk.dllをJNAを使ってアクセスするクラスのコンストラクタ（引数なし）
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public JTalkJna() throws Exception {
        this(null, null, null);
    }

    /**
     * オブジェクトポインタがNULLであれば例外を発生させる
     *
     * @throws Exception オブジェクトポインタがNULLの例外
     */
    private void checkOpenjtalkObject() throws Exception {
        if (handle == null) {
            throw new Exception("Internal Error: OpenJTalk pointer is NULL");
        }
    }

    // // ubuntu 17.04 64bit JRE version: OpenJDK Runtime Environment
    // (8.0_144-b01)では異常終了してしまうので不使用
    // // <summary>
    // // <para>音響モデルファイルデータのリストを生成する。</para>
    // // <para>使用後はdelete_voice_listを使って解放する。</para>
    // // </summary>
    // private void generateVoiceList() throws Exception {
    // checkOpenjtalkObject();
    // if (voices!=null) {
    // voices.clear();
    // } else {
    // voices = new ArrayList<VoiceFileInfo>();
    // }
    // HTSVoiceList ptr = null;
    // if (this.fShiftJis) {
    // ptr = API.INSTANCE.openjtalk_getHTSVoiceListSjis(handle);
    // } else {
    // ptr = API.INSTANCE.openjtalk_getHTSVoiceList(handle);
    // }
    // if (ptr!=null) {
    // HTSVoiceList top = ptr;
    // do {
    // String path = new String(ptr.path);
    // String name = new String(ptr.name);
    // voices.add(new VoiceFileInfo(path,name));
    // ptr = ptr.succ;
    // } while (ptr!=null);
    // top.clear();
    // API.INSTANCE.openjtalk_clearHTSVoiceList(handle,top);
    // }
    // }

    private int fileCounter = 0;
    private final int MAXFILENUM = 200;

    /**
     * 再帰的に音響モデルファイルを登録する
     *
     * @param dir 探査対象ディレクトリ
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    private void setVoiceFile(final File dir) throws Exception {
        final var files = dir.listFiles();
        if (files == null) {
            return;
        }
        for (final var file : files) {
            if (!file.exists()) {
                continue;
            } else if (file.isDirectory()) {
                setVoiceFile(file);
            } else if (file.isFile()) {
                fileCounter++;
                if (fileCounter > MAXFILENUM) {
                    throw new Exception(String.format("音響モデルファイル探索フォルダ'%s'にファイルが多すぎます。", getVoiceDir()));
                }
                final var fileName = file.getName();
                final var index = fileName.lastIndexOf(".htsvoice");
                if (index > 0) {
                    final var path = file.getAbsolutePath();
                    final var name = fileName.substring(0, index);
                    voices.add(new VoiceFileInfo(path, name));
                }
            }
        }
    }

    /**
     * 音響モデルファイルのリストを生成する
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    private void generateVoiceList() throws Exception {
        checkOpenjtalkObject();
        final var dir = getVoiceDir();
        if (voices != null) {
            voices.clear();
        } else {
            voices = new ArrayList<VoiceFileInfo>();
        }
        fileCounter = 0;
        setVoiceFile(new File(dir));
    }

    /**
     * 音響モデルディレクトリにある全ての音響モデルファイルのリストを取得する
     *
     * @return 全ての音響モデルファイルのリスト
     */
    public ArrayList<VoiceFileInfo> getVoices() {
        return this.voices;
    }

    /**
     * サンプリング周波数(S)を設定する
     *
     * @param i サンプリング周波数(S)（整数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setSamplingFrequency(final int i) throws Exception {
        checkOpenjtalkObject();
        if (i < 1) {
            throw new Exception("sampling frequency の範囲は1以上の整数です。");
        }
        API.INSTANCE.openjtalk_setSamplingFrequency(handle, i);
    }

    /**
     * サンプリング周波数(S)を取得する
     *
     * @return サンプリング周波数(S)（整数）
     */
    public int getSamplingFrequency() {
        return API.INSTANCE.openjtalk_getSamplingFrequency(handle);
    }

    /**
     * サンプリング周波数(S)を設定する
     *
     * @param i サンプリング周波数(S)（整数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setS(final int i) throws Exception {
        setSamplingFrequency(i);
    }

    /**
     * サンプリング周波数(S)を取得する
     *
     * @return サンプリング周波数(S)（整数）
     */
    public int getS() {
        return getSamplingFrequency();
    }

    /**
     * フレームピリオド(P)を設定する
     *
     * @param i フレームピリオド(P)（整数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setFperiod(final int i) throws Exception {
        checkOpenjtalkObject();
        if (i < 1) {
            throw new Exception("frame period の範囲は1以上の整数です。");
        }
        API.INSTANCE.openjtalk_setFperiod(handle, i);
    }

    /**
     * フレームピリオド(P)を取得する
     *
     * @return フレームピリオド(P)（整数）
     */
    public int getFperiod() {
        return API.INSTANCE.openjtalk_getFperiod(handle);
    }

    /**
     * フレームピリオド(P)を設定する
     *
     * @param i フレームピリオド(P)（整数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setP(final int i) throws Exception {
        setFperiod(i);
    }

    /**
     * フレームピリオド(P)を取得する
     *
     * @return フレームピリオド(P)（整数）
     */
    public int getP() {
        return getFperiod();
    }

    /**
     * オールパス値(Alpha)を設定する
     *
     * @param f オールパス値（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setAlpha(final double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setAlpha(handle, f);
    }

    /**
     * オールパス値(Alpha)を取得する
     *
     * @return オールパス値(Alpha)（浮動小数点数）
     */
    public double getAlpha() {
        return API.INSTANCE.openjtalk_getAlpha(handle);
    }

    /**
     * オールパス値(Alpha)を設定する
     *
     * @param f オールパス値（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setA(final double f) throws Exception {
        setAlpha(f);
    }

    /**
     * オールパス値(Alpha)を取得する
     *
     * @return オールパス値(Alpha)（浮動小数点数）
     */
    public double getA() {
        return getAlpha();
    }

    /**
     * ポストフィルター係数(Beta)を設定する
     *
     * @param f ポストフィルター係数(Beta)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setBeta(final double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setBeta(handle, f);
    }

    /**
     * ポストフィルター係数(Beta)を取得する
     *
     * @return ポストフィルター係数(Beta)（浮動小数点数）
     */
    public double getBeta() {
        return API.INSTANCE.openjtalk_getBeta(handle);
    }

    /**
     * ポストフィルター係数(Beta)を設定する
     *
     * @param f ポストフィルター係数(Beta)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setB(final double f) throws Exception {
        setBeta(f);
    }

    /**
     * ポストフィルター係数(Beta)を取得する
     *
     * @return ポストフィルター係数(Beta)（浮動小数点数）
     */
    public double getB() {
        return getBeta();
    }

    /**
     * スピーチ速度(R)を設定する
     *
     * @param f スピーチ速度(R)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setSpeed(final double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0) {
            throw new Exception("speech speed rate の範囲は0以上の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setSpeed(handle, f);
    }

    /**
     * スピーチ速度(R)を取得する
     *
     * @return スピーチ速度(R)（浮動小数点数）
     */
    public double getSpeed() {
        return API.INSTANCE.openjtalk_getSpeed(handle);
    }

    /**
     * スピーチ速度(R)を設定する
     *
     * @param f スピーチ速度(R)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setR(final double f) throws Exception {
        setSpeed(f);
    }

    /**
     * スピーチ速度(R)を取得する
     *
     * @return スピーチ速度(R)（浮動小数点数）
     */
    public double getR() {
        return getSpeed();
    }

    /**
     * 追加ハーフトーン(Fm)を設定する
     *
     * @param f 追加ハーフトーン(Fm)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setAdditionalHalfTone(final double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setAdditionalHalfTone(handle, f);
    }

    /**
     * 追加ハーフトーン(Fm)を取得する
     *
     * @return 追加ハーフトーン(Fm)（浮動小数点数）
     */
    public double getAdditionalHalfTone() {
        return API.INSTANCE.openjtalk_getAdditionalHalfTone(handle);
    }

    /**
     * 追加ハーフトーン(Fm)を設定する
     *
     * @param f 追加ハーフトーン(Fm)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setFm(final double f) throws Exception {
        setAdditionalHalfTone(f);
    }

    /**
     * 追加ハーフトーン(Fm)を取得する
     *
     * @return 追加ハーフトーン(Fm)（浮動小数点数）
     */
    public double getFm() {
        return getAdditionalHalfTone();
    }

    /**
     * 有声 / 無声境界値(U)を設定する
     *
     * @param f 有声 / 無声境界値(U)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setMsdThreshold(final double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setMsdThreshold(handle, f);
    }

    /**
     * 有声 / 無声境界値(U)を取得する
     *
     * @return 有声 / 無声境界値(U)（浮動小数点数）
     */
    public double getMsdThreshold() {
        return API.INSTANCE.openjtalk_getMsdThreshold(handle);
    }

    /**
     * 有声 / 無声境界値(U)を設定する
     *
     * @param f 有声 / 無声境界値(U)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setU(final double f) throws Exception {
        setMsdThreshold(f);
    }

    /**
     * 有声 / 無声境界値(U)を取得する
     *
     * @return 有声 / 無声境界値(U)（浮動小数点数）
     */
    public double getU() {
        return getMsdThreshold();
    }

    /**
     * スペクトラム系列内変動の重み(Jm)を設定する
     *
     * @param f スペクトラム系列内変動の重み(Jm)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setGvWeightForSpectrum(final double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setGvWeightForSpectrum(handle, f);
    }

    /**
     * スペクトラム系列内変動の重み(Jm)を取得する
     *
     * @return スペクトラム系列内変動の重み(Jm)（浮動小数点数）
     */
    public double getGvWeightForSpectrum() {
        return API.INSTANCE.openjtalk_getGvWeightForSpectrum(handle);
    }

    /**
     * スペクトラム系列内変動の重み(Jm)を設定する
     *
     * @param f スペクトラム系列内変動の重み(Jm)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setJm(final double f) throws Exception {
        setGvWeightForSpectrum(f);
    }

    /**
     * スペクトラム系列内変動の重み(Jm)を取得する
     *
     * @return スペクトラム系列内変動の重み(Jm)（浮動小数点数）
     */
    public double getJm() {
        return getGvWeightForSpectrum();
    }

    /**
     * F0系列内変動重み(Jf)を設定する
     *
     * @param f F0系列内変動重み(Jf)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setGvWeightForLogF0(final double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0) {
            throw new Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setGvWeightForLogF0(handle, f);
    }

    /**
     * F0系列内変動重み(Jf)を取得する
     *
     * @return F0系列内変動重み(Jf)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public double getGvWeightForLogF0() throws Exception {
        return API.INSTANCE.openjtalk_getGvWeightForLogF0(handle);
    }

    /**
     * F0系列内変動重み(Jf)を設定する
     *
     * @param f F0系列内変動重み(Jf)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setJf(final double f) throws Exception {
        setGvWeightForLogF0(f);
    }

    /**
     * F0系列内変動重み(Jf)を取得する
     *
     * @return F0系列内変動重み(Jf)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public double getJf() throws Exception {
        return getGvWeightForLogF0();
    }

    /**
     * ボリューム(G)を設定する
     *
     * @param f ボリューム(G)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVolume(final double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setVolume(handle, f);
    }

    /**
     * ボリューム(G)を取得する
     *
     * @return ボリューム(G)（浮動小数点数）
     */
    public double getVolume() {
        return API.INSTANCE.openjtalk_getVolume(handle);
    }

    /**
     * ボリューム(G)を設定する
     *
     * @param f ボリューム(G)（浮動小数点数）
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setG(final double f) throws Exception {
        setVolume(f);
    }

    /**
     * ボリューム(G)を取得する
     *
     * @return ボリューム(G)（浮動小数点数）
     */
    public double getG() {
        return getVolume();
    }

    /**
     * 辞書ディレクトリを設定する
     *
     * @param path 辞書ディレクトリのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setDic(final String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("辞書フォルダを示す文字列が空です。");
        }
        final var file = new File(path);
        if (!file.exists()) {
            throw new Exception("辞書フォルダが見つかりません。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setDic2(handle, path) == 0) {
                throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setDic(handle, path)) {
                throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
            }
        }
    }

    /**
     * 辞書ディレクトリを取得する
     *
     * @return 辞書ディレクトリのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public String getDic() throws Exception {
        final var buff = new byte[MAXPATH];
        final var res = API.INSTANCE.openjtalk_getDic(handle, buff);
        final var path = new String(buff, "UTF-8");
        return res != null ? path.trim() : "";
    }

    /**
     * 音響モデルディレクトリを設定する
     *
     * @param path 音響モデルディレクトリ
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVoiceDir(final String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルフォルダを示す文字列が空です。");
        }
        final var file = new File(path);
        if (!file.exists()) {
            throw new Exception("音響モデルフォルダが見つかりません。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setVoiceDir2(handle, path) == 0) {
                throw new Exception("音響モデルフォルダを設定できません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setVoiceDir(handle, path)) {
                throw new Exception("音響モデルフォルダを設定できません。");
            }
        }
        generateVoiceList();
        fireVoiceListChanged();
    }

    /**
     * 現在の音響モデルディレクトリを取得する
     *
     * @return 音響モデルディレクトリのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public String getVoiceDir() throws Exception {
        final var buff = new byte[MAXPATH];
        final var res = API.INSTANCE.openjtalk_getVoiceDir(handle, buff);
        final var path = new String(buff, "UTF-8");
        return res != null ? path.trim() : "";
    }

    /**
     * 音響モデルファイルのパスによって現在の音響モデルを設定する
     *
     * @param path 音響モデルのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVoicePath(final String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setVoicePath2(handle, path) == 0) {
                throw new Exception("音響モデルを設定できません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setVoicePath(handle, path)) {
                throw new Exception("音響モデルを設定できません。");
            }
        }
    }

    /**
     * 現在の音響モデルファイルのパスを取得する
     *
     * @return 音響モデルのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public String getVoicePath() throws Exception {
        final var buff = new byte[MAXPATH];
        final var res = API.INSTANCE.openjtalk_getVoicePath(handle, buff);
        final var path = new String(buff, "UTF-8");
        return res != null ? path.trim() : "";
    }

    /**
     * 音響モデル名で現在の音響モデルを設定する
     *
     * @param name 音響モデル名
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVoiceName(final String name) throws Exception {
        checkOpenjtalkObject();
        if (name == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setVoiceName2(handle, name) == 0) {
                throw new Exception("音響モデルを設定できません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setVoiceName(handle, name)) {
                throw new Exception("音響モデルを設定できません。");
            }
        }
    }

    /**
     * 現在の音響モデル名を取得する
     *
     * @return 音響モデル名
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public String getVoiceName() throws Exception {
        final var buff = new byte[MAXPATH];
        final var res = API.INSTANCE.openjtalk_getVoiceName(handle, buff);
        final var path = new String(buff, "UTF-8");
        return res != null ? path.trim() : "";
    }

    /**
     * 音響モデルファイルのパスによって現在の音響モデルを設定する
     *
     * @param path 音響ファイルのパス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVoice(final String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setVoice2(handle, path) == 0) {
                throw new Exception("音響モデルを設定できません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setVoice(handle, path)) {
                throw new Exception("音響モデルを設定できません。");
            }
        }
    }

    /**
     * 音響モデル情報によって現在の音響モデルを設定する
     *
     * @param arg 音響モデル情報
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setVoice(final VoiceFileInfo arg) throws Exception {
        checkOpenjtalkObject();
        if (arg == null) {
            throw new Exception("音響モデルの指定がNULLです。");
        }
        if (arg.path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_setVoice2(handle, arg.path) == 0) {
                throw new Exception("音響モデルを設定できません。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_setVoice(handle, arg.path)) {
                throw new Exception("音響モデルを設定できません。");
            }
        }
    }

    /**
     * 現在の音響モデル情報を取得する
     *
     * @return 音響モデル情報オブジェクト
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public VoiceFileInfo getVoice() throws Exception {
        final var buffPath = new byte[MAXPATH];
        final var buffName = new byte[MAXPATH];
        final var resPath = API.INSTANCE.openjtalk_getVoicePath(handle, buffPath);
        final var resName = API.INSTANCE.openjtalk_getVoiceName(handle, buffName);
        final var result = new VoiceFileInfo();
        final var path = new String(buffPath, "UTF-8");
        final var name = new String(buffName, "UTF-8");
        result.path = resPath != null ? path.trim() : "";
        result.name = resName != null ? name.trim() : "";
        return result;
    }

    /**
     * 同期発声する
     *
     * @param text 文字列
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void speakSync(final String text) throws Exception {
        checkOpenjtalkObject();
        if (text != "") {
            API.INSTANCE.openjtalk_speakSync(handle, text);
        }
    }

    /**
     * 非同期発声をする
     *
     * @param text 文字列
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void speakAsync(final String text) throws Exception {
        checkOpenjtalkObject();
        if (text != "") {
            API.INSTANCE.openjtalk_speakAsync(handle, text);
        }
    }

    /**
     * 非同期発声を一時停止する
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void pause() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_pause(handle);
    }

    /**
     * 非同期発声の一時停止を再開する
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void resume() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_resume(handle);
    }

    /**
     * 非同期発声を強制停止する
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void stop() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_stop(handle);
    }

    /**
     * 非同期発声しているかどうか
     *
     * @return 発声しているかどうかの真偽値
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public boolean isSpeaking() throws Exception {
        checkOpenjtalkObject();
        if (Platform.isMac()) {
            return API.INSTANCE.openjtalk_isSpeaking2(handle) != 0;
        } else {
            return API.INSTANCE.openjtalk_isSpeaking(handle);
        }
    }

    /**
     * 非同期発声が一時停止しているかどうか
     *
     * @return 一時停止しているかどうかの真偽値
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public boolean isPaused() throws Exception {
        checkOpenjtalkObject();
        if (Platform.isMac()) {
            return API.INSTANCE.openjtalk_isPaused2(handle) != 0;
        } else {
            return API.INSTANCE.openjtalk_isPaused(handle);
        }
    }

    /**
     * 非同期発声が完了したかどうか
     *
     * @return 完了したかどうかの真偽値
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public boolean isFinished() throws Exception {
        checkOpenjtalkObject();
        if (Platform.isMac()) {
            return API.INSTANCE.openjtalk_isFinished2(handle) != 0;
        } else {
            return API.INSTANCE.openjtalk_isFinished(handle);
        }
    }

    /**
     * 発声している間待機する
     *
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void waitUntilDone() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_waitUntilDone(handle);
    }

    /**
     * 指定時間待機する
     *
     * @param duration 待機時間(ms)
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void wait(final int duration) throws Exception {
        checkOpenjtalkObject();
        if (duration == 0) {
            waitUntilDone();
        } else {
            API.INSTANCE.openjtalk_wait(handle, duration);
        }
    }

    /**
     * 文字列の音声をwav形式でファイルに保存する
     *
     * @param text 読み上げ文字列
     * @param file 保存ファイル名
     * @throws Exception ファイル書き込みエラーなどの例外
     */
    public void speakToFile(final String text, final String file) throws Exception {
        checkOpenjtalkObject();
        if (text == "") {
            throw new Exception("読み上げ文字列が空です。");
        }
        if (file == "") {
            throw new Exception("ファイル名文字列が空です。");
        }
        if (Platform.isMac()) {
            if (API.INSTANCE.openjtalk_speakToFile2(handle, text, file) == 0) {
                throw new Exception("音声ファイルの作成中にエラーが発生しました。");
            }
        } else {
            if (!API.INSTANCE.openjtalk_speakToFile(handle, text, file)) {
                throw new Exception("音声ファイルの作成中にエラーが発生しました。");
            }
        }
    }

    /**
     * 完了時に実行するメソッドを登録する。読み上げ中には登録できない 既に登録中の時は、置き換える。
     * 
     * @param proc コールバックインターフェイス
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void setOnSpeakingFinishedCallback(OnSpeakingFinishedCallbackProc proc) throws Exception {
        if (!isSpeaking()) {
            API.INSTANCE.openjtalk_setOnFinishedCallback(handle, proc);
        }
    }

    /**
     * 完了時に実行するメソッドを削除する。読み上げ中には削除できない。
     * 
     * @throws Exception オブジェクトポインタがNULLなどの例外
     */
    public void clearOnSpeakingFinishedCallback() throws Exception {
        if (!isSpeaking()) {
            API.INSTANCE.openjtalk_setOnFinishedCallback(handle, null);
        }
    }

    /**
     * くどく動作報告を行うかどうか
     * 
     * @param flag 真ならば行う、偽ならば行わない。
     */
    public void setVerbose(boolean flag) {
        API.INSTANCE.openjtalk_setVerbose(flag);
    }
}

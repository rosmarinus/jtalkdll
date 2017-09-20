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


public class JTalkJna {

    final int MAXPATH = 260;

    public interface VoiceListChangedListener extends EventListener {
        public void onVoiceListChanged(VoiceListChangedEvent e);
    }

    public class VoiceListChangedEvent extends EventObject {
        public VoiceListChangedEvent(Object source) {
            super(source);
        }
    }

    protected EventListenerList listenerList = new EventListenerList();

    public void addVoiceListChangedListener(VoiceListChangedListener listener) {
        listenerList.add(VoiceListChangedListener.class, listener);
    }

    public void removeVoiceListChangedListener(VoiceListChangedListener listener) {
        listenerList.remove(VoiceListChangedListener.class, listener);
    }

    public void fireVoiceListChanged() {
        VoiceListChangedEvent e = new VoiceListChangedEvent(this);
        for (VoiceListChangedListener listener : listenerList.getListeners(VoiceListChangedListener.class)) {
            listener.onVoiceListChanged(e);
        }
    }

    public class VoiceFileInfo {
        public String path;
        public String name;

        VoiceFileInfo(String path, String name) {
            this.path = new String(path);
            this.name = new String(name);
        }

        VoiceFileInfo(String path) {
            this(path, JTalkJna.baseName(path));
        }

        VoiceFileInfo() {
            this("", "");
        }
    }

    public static class HTSVoiceList extends Structure {
        public HTSVoiceList.ByReference succ;
        public String path;
        public String name;

        public HTSVoiceList() {
            super(Structure.ALIGN_NONE);
        }

        protected List<String> getFieldOrder() {
            return Arrays.asList(new String[]{"succ", "path", "name"});

        }

        public static class ByReference extends HTSVoiceList implements Structure.ByReference {
        }
        //public static class ByValue extends HTSVoiceList implements Structure.ByValue { }
    }

    public interface API extends Library {

        API INSTANCE = (API) Native.loadLibrary(
                (Platform.isWindows()
                        ? (Platform.is64Bit() ? "jtalk64" : "jtalk32")
                        : "jtalk"),
                API.class);

        void openjtalk_clearHTSVoiceList(Pointer handle, HTSVoiceList list);

        HTSVoiceList openjtalk_getHTSVoiceList(Pointer handle);

        HTSVoiceList openjtalk_getHTSVoiceListSjis(Pointer handle);

        Pointer openjtalk_initialize(String voicePath, String dicPath, String voiceDirPath);

        Pointer openjtalk_initializeSjis(String voicePath, String dicPath, String voiceDirPath);

        void openjtalk_clear(Pointer handle);

        void openjtalk_refresh(Pointer handle);

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

        Boolean openjtalk_setDic(Pointer handle, String path);

        Boolean openjtalk_setDicSjis(Pointer handle, String path);

        String openjtalk_getDic(Pointer handle, byte[] path);

        String openjtalk_getDicSjis(Pointer handle, byte[] path);

        Boolean openjtalk_setVoiceDir(Pointer handle, String path);

        Boolean openjtalk_setVoiceDirSjis(Pointer handle, String path);

        String openjtalk_getVoiceDir(Pointer handle, byte[] path);

        String openjtalk_getVoiceDirSjis(Pointer handle, byte[] path);

        Boolean openjtalk_setVoice(Pointer handle, String path);

        Boolean openjtalk_setVoiceSjis(Pointer handle, String path);

        Boolean openjtalk_setVoicePath(Pointer handle, String path);

        Boolean openjtalk_setVoicePathSjis(Pointer handle, String path);

        String openjtalk_getVoicePath(Pointer handle, byte[] path);

        String openjtalk_getVoicePathSjis(Pointer handle, byte[] path);

        Boolean openjtalk_setVoiceName(Pointer handle, String path);

        Boolean openjtalk_setVoiceNameSjis(Pointer handle, String path);

        String openjtalk_getVoiceName(Pointer handle, byte[] path);

        String openjtalk_getVoiceNameSjis(Pointer handle, byte[] path);

        void openjtalk_speakSync(Pointer handle, String text);

        void openjtalk_speakSyncSjis(Pointer handle, String text);

        void openjtalk_speakAsync(Pointer handle, String text);

        void openjtalk_speakAsyncSjis(Pointer handle, String text);

        void openjtalk_stop(Pointer handle);

        Boolean openjtalk_isSpeaking(Pointer handle);

        void openjtalk_waitUntilDone(Pointer handle);

        void openjtalk_wait(Pointer handle, int duration);

        Boolean openjtalk_speakToFile(Pointer handle, String text, String file);

        int openjtalk_getCharCode(String text);
    }

    private Pointer handle = null;
    private ArrayList<VoiceFileInfo> voices = new ArrayList<VoiceFileInfo>();
    private Boolean fShiftJis = false;

    JTalkJna(String voicePath, String dicPath, String voiceDirPath) throws Exception {

        switch(API.INSTANCE.openjtalk_getCharCode("あ")) {
            case 0x000082a0:
                fShiftJis = true;
                break;
            case 0x00e38182:
                fShiftJis = false;
                break;
            default:
                javax.swing.JOptionPane.showMessageDialog(null,"不明なエンコードです。");
                break;
        }

        if (fShiftJis) {
            handle = API.INSTANCE.openjtalk_initializeSjis(voicePath, dicPath, voiceDirPath);
        } else {
            handle = API.INSTANCE.openjtalk_initialize(voicePath, dicPath, voiceDirPath);
        }
        generateVoiceList();
    }

    JTalkJna(String voicePath, String dicPath) throws Exception {
        this(voicePath, dicPath, null);
    }

    JTalkJna(String voicePath) throws Exception {
        this(voicePath, null, null);
    }

    JTalkJna() throws Exception {
        this(null, null, null);
    }


    // OpenJTalkオブジェクトポインタがNULLでないことの確認
    private void checkOpenjtalkObject() throws Exception {
        if (handle == null) {
            throw new Exception("Internal Error: OpenJTalk pointer is NULL");
        }
    }

    // // ubuntu 17.04 64bit JRE version: OpenJDK Runtime Environment (8.0_144-b01)では異常終了してしまうので不使用
    // // <summary>
    // // <para>音響モデルファイルデータのリストを生成する。</para>
    // // <para>使用後はdelete_voice_listを使って解放する。</para>
    // // </summary>
    // private void generateVoiceList() throws Exception {
    // 	checkOpenjtalkObject();
    // 	if (voices!=null) {
    // 		voices.clear();
    // 	} else {
    // 		voices = new ArrayList<VoiceFileInfo>();
    // 	}
    // 	HTSVoiceList ptr = null;
    // 	if (this.fShiftJis) {
    // 		ptr = API.INSTANCE.openjtalk_getHTSVoiceListSjis(handle);
    // 	} else {
    // 		ptr = API.INSTANCE.openjtalk_getHTSVoiceList(handle);
    // 	}
    // 	if (ptr!=null) {
    // 		HTSVoiceList top = ptr;
    // 		do {
    // 			String path = new String(ptr.path);
    // 			String name = new String(ptr.name);
    // 			voices.add(new VoiceFileInfo(path,name));
    // 			ptr = ptr.succ;
    // 		} while (ptr!=null);
    // 		top.clear();
    // 		API.INSTANCE.openjtalk_clearHTSVoiceList(handle,top);
    // 	}
    // }

    private int fileCounter = 0;
    private final int MAXFILENUM = 200;

    private void setVoiceFile(File dir) throws Exception {
        File[] files = dir.listFiles();
        if (files == null) {
            return;
        }
        for (File file : files) {
            if (!file.exists()) {
                continue;
            } else if (file.isDirectory()) {
                setVoiceFile(file);
            } else if (file.isFile()) {
                fileCounter ++; 
                if (fileCounter>MAXFILENUM) {
                    throw new Exception(String.format("音響モデルファイル探索フォルダ'%s'にファイルが多すぎます。", getVoiceDir()));
                }
                String fileName = file.getName();
                int index = fileName.lastIndexOf(".htsvoice");
                if (index > 0) {
                    String path = file.getAbsolutePath();
                    String name = fileName.substring(0, index);
                    voices.add(new VoiceFileInfo(path, name));
                }
            }
        }
    }

    private void generateVoiceList() throws Exception {
        checkOpenjtalkObject();
        String dir = getVoiceDir();
        if (voices != null) {
            voices.clear();
        } else {
            voices = new ArrayList<VoiceFileInfo>();
        }
        fileCounter = 0;
        setVoiceFile(new File(dir));
    }

    private static String baseName(String path) {
        File file = new File(path);
        String fileName = file.getName();
        int index = fileName.lastIndexOf('.');
        if (index > 0) {
            return fileName.substring(0, index);
        } else {
            return "";
        }
    }

    public ArrayList<VoiceFileInfo> getVoices() {
        return this.voices;
    }

    // サンプリング周波数
    void setSamplingFrequency(int i) throws Exception {
        checkOpenjtalkObject();
        if (i < 1) {
            throw new Exception("sampling frequency の範囲は1以上の整数です。");
        }
        API.INSTANCE.openjtalk_setSamplingFrequency(handle, i);
    }

    int getSamplingFrequency() {
        return API.INSTANCE.openjtalk_getSamplingFrequency(handle);
    }

    void setS(int i) throws Exception {
        setSamplingFrequency(i);
    }

    int getS() {
        return getSamplingFrequency();
    }

    // フレームピリオド
    void setFperiod(int i) throws Exception {
        checkOpenjtalkObject();
        if (i < 1) {
            throw new Exception("frame period の範囲は1以上の整数です。");
        }
        API.INSTANCE.openjtalk_setFperiod(handle, i);
    }

    int getFperiod() {
        return API.INSTANCE.openjtalk_getFperiod(handle);
    }

    void setP(int i) throws Exception {
        setFperiod(i);
    }

    int getP() {
        return getFperiod();
    }

    // オールパス値
    void setAlpha(double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setAlpha(handle, f);
    }

    double getAlpha() {
        return API.INSTANCE.openjtalk_getAlpha(handle);
    }

    void setA(double f) throws Exception {
        setAlpha(f);
    }

    double getA() {
        return getAlpha();
    }

    // ポストフィルター係数
    void setBeta(double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setBeta(handle, f);
    }

    double getBeta() {
        return API.INSTANCE.openjtalk_getBeta(handle);
    }

    void setB(double f) throws Exception {
        setBeta(f);
    }

    double getB() {
        return getBeta();
    }

    // スピーチ速度
    void setSpeed(double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0) {
            throw new Exception("speech speed rate の範囲は0以上の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setSpeed(handle, f);
    }

    double getSpeed() {
        return API.INSTANCE.openjtalk_getSpeed(handle);
    }

    void setR(double f) throws Exception {
        setSpeed(f);
    }

    double getR() {
        return getSpeed();
    }

    // 追加ハーフトーン
    void setAdditionalHalfTone(double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setAdditionalHalfTone(handle, f);
    }

    double getAdditionalHalfTone() {
        return API.INSTANCE.openjtalk_getAdditionalHalfTone(handle);
    }

    void setFm(double f) throws Exception {
        setAdditionalHalfTone(f);
    }

    double getFm() {
        return getAdditionalHalfTone();
    }

    // 有声 / 無声境界値
    void setMsdThreshold(double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0 || f > 1.0) {
            throw new Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setMsdThreshold(handle, f);
    }

    double getMsdThreshold() {
        return API.INSTANCE.openjtalk_getMsdThreshold(handle);
    }

    void setU(double f) throws Exception {
        setMsdThreshold(f);
    }

    double getU() {
        return getMsdThreshold();
    }

    // スペクトラム系列内変動の重み
    void setGvWeightForSpectrum(double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setGvWeightForSpectrum(handle, f);
    }

    double getGvWeightForSpectrum() {
        return API.INSTANCE.openjtalk_getGvWeightForSpectrum(handle);
    }

    void setJm(double f) throws Exception {
        setGvWeightForSpectrum(f);
    }

    double getJm() {
        return getGvWeightForSpectrum();
    }

    // F0系列内変動重み
    void setGvWeightForLogF0(double f) throws Exception {
        checkOpenjtalkObject();
        if (f < 0.0) {
            throw new Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。");
        }
        API.INSTANCE.openjtalk_setGvWeightForLogF0(handle, f);
    }

    double getGvWeightForLogF0() throws Exception {
        return API.INSTANCE.openjtalk_getGvWeightForLogF0(handle);
    }

    void setJf(double f) throws Exception {
        setGvWeightForLogF0(f);
    }

    double getJf() throws Exception {
        return getGvWeightForLogF0();
    }

    // ボリューム
    void setVolume(double f) throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_setVolume(handle, f);
    }

    double getVolume() {
        return API.INSTANCE.openjtalk_getVolume(handle);
    }

    void setG(double f) throws Exception {
        setVolume(f);
    }

    double getG() {
        return getVolume();
    }

    // 辞書ディレクトリ指定(UTF-8)
    void setDic(String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("辞書フォルダを示す文字列が空です。");
        }
        File file = new File(path);
        if (!file.exists()) {
            throw new Exception("辞書フォルダが見つかりません。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setDicSjis(handle, path);
        } else {
            res = API.INSTANCE.openjtalk_setDic(handle, path);
        }

        if (!res) {
            throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
        }
    }

    String getDic() throws Exception {
        String path = "";
        byte[] buff = new byte[MAXPATH];
        String res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_getDicSjis(handle, buff);
            path = new String(buff, "Shift-JIS");
        } else {
            res = API.INSTANCE.openjtalk_getDic(handle, buff);
            path = new String(buff, "UTF-8");
        }
        return res != null ? new String(path.trim()) : null;
    }

    // 音響モデルディレクトリ指定(UTF-8)
    void setVoiceDir(String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルフォルダを示す文字列が空です。");
        }
        File file = new File(path);
        if (!file.exists()) {
            throw new Exception("音響モデルフォルダが見つかりません。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setVoiceDirSjis(handle, path);
        } else {
            res = API.INSTANCE.openjtalk_setVoiceDir(handle, path);
        }
        if (!res) {
            throw new Exception("音響モデルフォルダを設定できません。");
        }
        generateVoiceList();
        fireVoiceListChanged();
    }

    String getVoiceDir() throws Exception {
        String path = "";
        byte[] buff = new byte[MAXPATH];
        String res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_getVoiceDirSjis(handle, buff);
            path = new String(buff, "Shift-JIS");
        } else {
            res = API.INSTANCE.openjtalk_getVoiceDir(handle, buff);
            path = new String(buff, "UTF-8");
        }
        return res != null ? new String(path.trim()) : null;
    }

    void setVoicePath(String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setVoicePathSjis(handle, path);
        } else {
            res = API.INSTANCE.openjtalk_setVoicePath(handle, path);
        }
        if (!res) {
            throw new Exception("音響モデルを設定できません。");
        }
    }

    String getVoicePath() throws Exception {
        String path = "";
        byte[] buff = new byte[MAXPATH];
        String res = null;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_getVoicePathSjis(handle, buff);
            path = new String(buff, "Shift-JIS");
        } else {
            res = API.INSTANCE.openjtalk_getVoicePath(handle, buff);
            path = new String(buff, "UTF-8");
        }
        return path.trim();
    }


    void setVoiceName(String name) throws Exception {
        checkOpenjtalkObject();
        if (name == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setVoiceNameSjis(handle, name);
        } else {
            res = API.INSTANCE.openjtalk_setVoiceName(handle, name);
        }
        if (!res) {
            throw new Exception("音響モデルを設定できません。");
        }
    }

    String getVoiceName() throws Exception {
        String name = "";
        byte[] buff = new byte[MAXPATH];
        String res = null;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_getVoiceNameSjis(handle, buff);
            name = new String(buff, "Shift-JIS");
        } else {
            res = API.INSTANCE.openjtalk_getVoiceName(handle, buff);
            name = new String(buff, "UTF-8");
        }
        return name.trim();
    }


    // 音響モデル指定(UTF-8)
    // 絶対パス...直接、相対パス...音響モデルディレクトリ基準、名前のみ...探索
    void setVoice(String path) throws Exception {
        checkOpenjtalkObject();
        if (path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setVoiceSjis(handle, path);
        } else {
            res = API.INSTANCE.openjtalk_setVoice(handle, path);
        }
        if (!res) {
            throw new Exception("音響モデルを設定できません。");
        }
    }


    void setVoice(VoiceFileInfo arg) throws Exception {
        checkOpenjtalkObject();
        if (arg == null) {
            throw new Exception("音響モデルの指定をNULLです。");
        }
        String path = new String(arg.path);
        if (path == "") {
            throw new Exception("音響モデルを示す文字列が空です。");
        }
        Boolean res;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_setVoiceSjis(handle, path);
        } else {
            res = API.INSTANCE.openjtalk_setVoice(handle, path);
        }
        if (!res) {
            throw new Exception("音響モデルを設定できません。");
        }
    }


    VoiceFileInfo getVoice() throws Exception {
        String path = "";
        String name = "";
        byte[] buffPath = new byte[MAXPATH];
        byte[] buffName = new byte[MAXPATH];
        String res = null;
        if (this.fShiftJis) {
            res = API.INSTANCE.openjtalk_getVoicePathSjis(handle, buffPath);
            path = new String(buffPath, "Shift-JIS");
            res = API.INSTANCE.openjtalk_getVoiceNameSjis(handle, buffName);
            name = new String(buffName, "Shift-JIS");
        } else {
            res = API.INSTANCE.openjtalk_getVoicePath(handle, buffPath);
            path = new String(buffPath, "UTF-8");
            res = API.INSTANCE.openjtalk_getVoiceName(handle, buffName);
            name = new String(buffName, "UTF-8");
        }
        VoiceFileInfo result = new VoiceFileInfo();
        if (path != null) {
            result.path = path.trim();
            result.name = name.trim();
        } else {
            result.path = null;
            result.name = null;
        }
        return result;
    }

    // 同期発声(UTF-8)
    void speakSync(String text) throws Exception {
        checkOpenjtalkObject();
        if (text == "") {
            return;
        }
        if (this.fShiftJis) {
            API.INSTANCE.openjtalk_speakSyncSjis(handle, text);
        } else {
            API.INSTANCE.openjtalk_speakSync(handle, text);
        }
    }

    // 非同期発声(UTF-8)
    void speakAsync(String text) throws Exception {
        checkOpenjtalkObject();
        if (text == "") {
            return;
        }
        if (this.fShiftJis) {
            API.INSTANCE.openjtalk_speakAsyncSjis(handle, text);
        } else {
            API.INSTANCE.openjtalk_speakAsync(handle, text);
        }
    }

    // 発声の強制停止
    void stop() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_stop(handle);
    }

    // 発声中かどうか
    Boolean isSpeaking() throws Exception {
        checkOpenjtalkObject();
        return API.INSTANCE.openjtalk_isSpeaking(handle);
    }

    // 発声している間待機する
    void waitUntilDone() throws Exception {
        checkOpenjtalkObject();
        API.INSTANCE.openjtalk_waitUntilDone(handle);
    }

    /// 指定時間待機する
    void wait(int duration) throws Exception {
        checkOpenjtalkObject();
        if (duration == 0) {
            waitUntilDone();
        } else {
            API.INSTANCE.openjtalk_wait(handle, duration);
        }
    }

    void speakToFile(String text, String file) throws Exception {
        checkOpenjtalkObject();
        if (text == "") {
            throw new Exception("読み上げ文字列が空です。");
        }
        if (file == "") {
            throw new Exception("ファイル名文字列が空です。");
        }
        if (!API.INSTANCE.openjtalk_speakToFile(handle, text, file)) {
            throw new Exception("音声ファイルの作成中にエラーが発生しました。");
        }
    }
}
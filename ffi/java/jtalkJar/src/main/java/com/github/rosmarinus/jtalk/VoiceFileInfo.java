package com.github.rosmarinus.jtalk;
import java.io.File;

/**
 * 音響モデル情報クラス
 */
public class VoiceFileInfo {

    /**
     * 音響モデルファイルのパス
     */
    public String path;

    /**
     * 音響モデルの名前
     */
    public String name;

    /**
     * 音響モデル情報コンストラクタ（パスと名前）
     *
     * @param path 音響モデルファイルのパス
     * @param name 音響モデルの名前
     */
    VoiceFileInfo(final String path, final String name) {
        this.path = new String(path);
        this.name = new String(name);
    }

    /**
     * 音響モデル情報コンストラクタ（パスのみ）
     *
     * @param path 音響モデルファイルのパス
     */
    VoiceFileInfo(final String path) {
        this(path, baseName(path));
    }

    /**
     * 音響モデル情報コンストラクタ（空）
     */
    VoiceFileInfo() {
        this("", "");
    }

    /**
     * パス文字列から名前部分を取得する
     *
     * @param path パス文字列
     * @return ファイルの名前部分
     */
    private static String baseName(final String path) {
        final var file = new File(path);
        final var fileName = file.getName();
        final var index = fileName.lastIndexOf('.');
        if (index > 0) {
            return fileName.substring(0, index);
        } else {
            return "";
        }
    }
}

import com.github.rosmarinus.jtalk.JTalkJna;
import java.io.File;
import java.net.URL;
import java.util.ResourceBundle;
import java.util.regex.Pattern;
import javafx.application.Platform;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextArea;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.input.KeyEvent;
import javafx.stage.DirectoryChooser;

public class JavaFXSampleController implements Initializable {
    private JTalkJna tts;
    private boolean pauseFlag = false;
    private final int MAXLENGTH = 1000;
    private EventHandler<ActionEvent> eventHandler = null;

    @FXML
    private TextArea textArea;
    @FXML
    private Button buttonRead;
    @FXML
    private Button buttonPause;
    @FXML
    private Button buttonStop;
    @FXML
    private Button buttonReference;
    @FXML
    private ComboBox<String> comboBox;

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        try {
            tts = new JTalkJna();
        } catch (Exception e) {
            return;
        }
        try {
            var name = tts.getVoiceName();
            comboBox.getItems().clear();
            tts.getVoices().forEach(v -> comboBox.getItems().add(v.name));
            comboBox.getSelectionModel().select(name);
        } catch (Exception e) {
            return;
        }

        tts.addVoiceListChangedListener(event -> {
            try {
                var name = tts.getVoiceName();
                comboBox.getItems().clear();
                tts.getVoices().forEach(v -> comboBox.getItems().add(v.name));
                comboBox.getSelectionModel().select(name);
            } catch (Exception e) {
            }
        });
        buttonRead.setDisable(false);
        buttonPause.setDisable(true);
        buttonStop.setDisable(true);
        buttonReference.setDisable(false);
        textArea.setWrapText(true);
    }

    @FXML
    protected void readText(ActionEvent event) throws Exception {
        var text = textArea.getText();
        if (text.length() > MAXLENGTH) {
            var alrt = new Alert(AlertType.INFORMATION);
            alrt.setTitle("エラー");
            alrt.setHeaderText(null);
            alrt.setContentText(
                String.format("文字が多すぎます。上限は %d 文字です。：現在 %d 文字", 
                MAXLENGTH, text.length()));
            alrt.showAndWait();
            return;
        }
        var voice = comboBox.getValue();

        buttonRead.setDisable(true);
        buttonPause.setDisable(false);
        buttonStop.setDisable(false);
        buttonReference.setDisable(true);
        comboBox.setDisable(true);
        textArea.setEditable(false);

        if (pauseFlag) {
            pauseFlag = false;
            tts.resume();
            return;
        }
        try {
            tts.setVoiceName(voice);
            var sel = textArea.getSelectedText();
            var start = textArea.getSelection().getStart();
            var end = textArea.getSelection().getEnd();
            if (sel.length() > 0) {
                text = sel;
            } else {
                start = textArea.getCaretPosition();
                end = text.length();
                while (true) {
                    if (start == end) {
                        start = 0;
                        break;
                    }
                    if (text.charAt(start) == '\n') {
                        start += 1;
                    } else {
                        break;
                    }
                }
                text = text.substring(start);
            }
            final String speak_text = textArea.getText();
            final int speak_start = start;
            final int speak_end = end;
            final boolean selected = textArea.getSelectedText().length() > 0;
            var s = new Service<Boolean>() {
                @Override
                protected Task<Boolean> createTask() {
                    Task<Boolean> task = new Task<Boolean>() {
                        @Override
                        protected Boolean call() throws Exception {
                            var text = speak_text;
                            var pos = speak_start;
                            var end = speak_end;
                            var data = "";
                            var length = 0;
                            var lastLine = false;
                            try {
                                while (true) {
                                    var substr = text.substring(pos, end);
                                    var p = Pattern.compile("^([^。\n]*?)(。\n|。|\n)");
                                    var m = p.matcher(substr);
                                    if (m.find()) {
                                        var str = m.group();
                                        length = str.length();
                                        data = str;
                                    } else {
                                        lastLine = true;
                                        length = substr.length();
                                        data = substr;
                                    }
                                    data = data.trim();
                                    if (data.length() > 0) {
                                        textArea.selectRange(pos, pos + length);
                                        tts.speakAsync(data);
                                        Platform.runLater(() -> buttonRead.setText("再開"));
                                        while (tts.isSpeaking()) {
                                            try {
                                                Thread.sleep(100);
                                            } catch (InterruptedException e) {
                                            }
                                            if (tts.isPaused()) {
                                            }
                                            try {
                                                while (tts.isPaused()) {
                                                    Thread.sleep(100);
                                                }
                                            } catch (InterruptedException e) {
                                            }
                                            if (isCancelled()) {
                                                textArea.positionCaret(pos);
                                                throw new Exception("停止ボタンで読み上げがキャンセルされました。");
                                            }
                                        }
                                        textArea.positionCaret(pos + length);
                                    }
                                    if (lastLine) {
                                        break;
                                    }
                                    if (text.length() < pos + length) {
                                        break;
                                    }
                                    pos = pos + length;
                                    textArea.positionCaret(pos);
                                }
                            } catch (Exception e) {
                                // System.out.println(e.getMessage());
                            } finally {
                                buttonRead.setDisable(false);
                                buttonPause.setDisable(true);
                                buttonStop.setDisable(true);
                                buttonReference.setDisable(false);
                                comboBox.setDisable(false);
                                textArea.setEditable(true);
                                pauseFlag = false;
                                if (selected) {
                                    textArea.selectRange(speak_start, speak_end);
                                }
                                Platform.runLater(() -> buttonRead.setText("発声"));
                            }
                            return true;
                        };
                    };
                    return task;
                }
            };

            if (eventHandler != null) {
                buttonStop.removeEventHandler(ActionEvent.ACTION, eventHandler);
            }
            eventHandler = e -> s.cancel();
            buttonStop.addEventHandler(ActionEvent.ACTION, eventHandler);
            s.start();
        } catch (Exception e) {
        }
    }

    @FXML
    protected void pauseReading(ActionEvent event) {
        try {
            if (tts.isSpeaking()) {
                tts.pause();
                buttonRead.setDisable(false);
                buttonPause.setDisable(true);
                buttonStop.setDisable(false);
                buttonReference.setDisable(true);
                buttonRead.setText("再開");
                pauseFlag = true;
            }
        } catch (Exception e) {
        }
    }

    @FXML
    protected void stopReading(ActionEvent event) {
        try {
            tts.stop();
            buttonRead.setDisable(false);
            buttonPause.setDisable(true);
            buttonStop.setDisable(true);
            buttonReference.setDisable(false);
            comboBox.setDisable(false);
            textArea.setEditable(true);
            pauseFlag = false;
        } catch (Exception e) {
        }
    }

    @FXML
    protected void referenceVoices(ActionEvent event) throws Exception {
        var dirChooser = new DirectoryChooser();
        var vdir = tts.getVoiceDir();
        dirChooser.setTitle("音響モデルフォルダ選択");
        dirChooser.setInitialDirectory(new File(vdir));
        var file = dirChooser.showDialog(null);
        if (file != null) {
            tts.setVoiceDir(file.getPath());
        }
    }

    @FXML
    protected void keyPressed(KeyEvent event) throws Exception {
    }
}

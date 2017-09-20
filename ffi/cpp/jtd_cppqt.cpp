
#include "jtd_cppqt.h"

QString DandDableEdit::readJapaneseText(QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return NULL;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8")); // SJIS
    auto text = stream.readAll();
    file.close();

    if ( text.length() > 10000 ) {
        QMessageBox::about(this, u8"エラー", u8"ファイルが大きすぎます");
        return NULL;
    }
    return text;
}


void DandDableEdit::dragEnterEvent(QDragEnterEvent *event) {
    if ( event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }
}


void DandDableEdit::dragMoveEvent(QDragMoveEvent *event) {
    if ( event->mimeData()->hasUrls()) {
        event->accept();
    } else {
        event->ignore();
    }
}


void DandDableEdit::dropEvent(QDropEvent *event) {
    auto keep = this->toPlainText();
    QTextEdit::dropEvent(event);
    if (event->mimeData()->hasUrls()) {
        this->setText("");
        for (auto url : event->mimeData()->urls()){
            auto p = url.toLocalFile();
            auto t = this->toPlainText();
            auto d = this->readJapaneseText(p);
            if (d != NULL) {
                this->setText(t+d);
            } else {
                this->setText(keep);
                return;
            }
        }
    }
}


EnterClickButton::EnterClickButton(const char *text) : QPushButton(QString(text)) {}


void EnterClickButton::keyPressEvent(QKeyEvent *event) {
    QPushButton::keyPressEvent(event);
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        this->animateClick();
    }
}


void SpeakThread::emitFinishedSignal(const QString &text) {
    emit this->finished(text);
}


SpeakThread::SpeakThread(JTalk *tts,QTextEdit *edit) {
    this->stopped = false;
    this->tts = tts;
    this->edit = edit;
    this->cursor = edit->textCursor();
    this->selected = this->cursor.hasSelection();
    this->startpos = 0;
    auto end = 0;
    this->text = edit->toPlainText();
    if (this->selected) {
        this->text  = this->text.replace(u8"\u2029",u8"\n");
        this->startpos = this->cursor.selectionStart();
        end = this->cursor.selectionEnd();
        this->text  = this->text.mid(0,end);
    } else {
        this->startpos = this->cursor.position();
        end = this->text.length();
        while (true) {
            if (this->startpos == end) {
                this->startpos = 0;
                break;
            }
            if (this->text[this->startpos] == '\n') {
                this->startpos += 1;
            } else {
                break;
            }
        }
    }
}


void SpeakThread::stop() {
    this->stopped = true;
}


int SpeakThread::searchNextSentence(QString &text) {
    auto kuten = QString(u8"。");
    auto newline = QString(u8"\n");
    auto posKuten = text.indexOf(kuten) + 1;
    auto posNewLine = text.indexOf(newline) + 1;

    if (posNewLine == 0) {
        return posKuten;
    } 

    if (posKuten == 0) {
        return posNewLine;
    } 

    if (posKuten < posNewLine) {
        if (posKuten + 1 == posNewLine) {
            return posNewLine;
        } else {
            return posKuten;
        }
    } else {
        return posNewLine;
    }
}


void SpeakThread::run() {
    auto top = this->startpos;
    auto length = 0;
    auto lastLine = false;
    QString data = "";
    while (true) {
        auto substr = this->text.mid(top);
        auto pos = this->searchNextSentence(substr);
        data = substr;
        if (pos!=0) {
            length = pos;
            data = substr.left(pos);
        } else {
            length = substr.length();
            data = substr;
            lastLine = true;
        }
        data = data.trimmed();
        if (data != "") {
            this->cursor.setPosition(top,QTextCursor::MoveAnchor);
            this->cursor.setPosition(top+length,QTextCursor::KeepAnchor);
            this->edit->setTextCursor(this->cursor);
            this->tts->speakAsync(data.toLocal8Bit().constData());
            while (this->tts->isSpeaking()) {
                if (this->stopped) {
                    goto exit_run;
                }
            }
        }

        if (lastLine) {
            break;
        }

        if (this->text.length() <= top + length) {
            break;
        }

        top += length;
    } 

    exit_run:
        this->cursor.clearSelection();
        this->cursor.setPosition(top+length,QTextCursor::MoveAnchor);
        this->edit->setTextCursor(this->cursor);
        this->emitFinishedSignal(data);
}


JTalkWindow::JTalkWindow() {
    this->selectedText = "";
    this->tts = new JTalk();
    this->initUI();
    this->th  = NULL;
}


void JTalkWindow::buttonChangeClicked() {
    QTextCodec* codec = QTextCodec::codecForLocale();
    
    auto dialog = new QFileDialog();

    auto caption = u8"音響モデルフォルダ選択";
    auto qCaption = QString(codec->toUnicode(caption));

    auto dir = this->tts->getVoiceDir();
    auto qDir = QString(codec->toUnicode(dir.c_str()));

    auto folder = dialog->getExistingDirectory(this, qCaption, qDir, QFileDialog::ShowDirsOnly);
    if (folder == "") {
        return;
    }

    this->tts->setVoiceDir(folder.toLocal8Bit().constData());

    auto name = this->tts->getVoiceName();
    auto qName = QString(codec->toUnicode(name.c_str()));

    this->combo->clear();
    for (auto voice : tts->getVoices() ) {
        this->combo->addItem(QString(voice->name.c_str()));
    }
    this->combo->setCurrentText(qName);
}


void JTalkWindow::speechFinished(const QString &text) {
    this->selectedText = text;
    this->textEdit->setReadOnly(false);
}


void JTalkWindow::buttonSayClicked() {
    if (this->th && this->th->isRunning()) {
        this->th->stop();
        this->th->wait();
    }
    this->textEdit->setReadOnly(true);
    this->textEdit->setFocus();
    this->th = new SpeakThread(this->tts,this->textEdit);
    QObject::connect(
        this->th, SIGNAL(finished(QString)),
        this, SLOT(speechFinished(QString)));
    this->th->start();
}


void JTalkWindow::buttonStopClicked() {
    if (this->th && this->th->isRunning()) {
        this->th->stop();
        this->th->wait();
        this->th = NULL;
    }
    this->tts->stop();
}


void JTalkWindow::comboSelected(const QString &string) {
    this->tts->setVoiceName(string.toLocal8Bit().constData());
}


void JTalkWindow::initUI() {
    this->setWindowTitle(u8"発声テスト");

    this->buttonSay = new EnterClickButton(u8"発声");
    connect(
        this->buttonSay, SIGNAL(clicked()),
        this, SLOT(buttonSayClicked()));

    this->buttonStop = new EnterClickButton(u8"停止");
    connect(
        this->buttonStop, SIGNAL(clicked()),
        this, SLOT(buttonStopClicked()));

    this->buttonChange = new EnterClickButton(u8"音声フォルダ");
    connect(
        this->buttonChange, SIGNAL(clicked()),
        this, SLOT(buttonChangeClicked()));

    this->combo = new QComboBox();
    connect(
        this->combo, SIGNAL(activated(QString)),
        this, SLOT(comboSelected(QString)));

    for (auto voice : tts->getVoices() ) {
        this->combo->addItem(QString(voice->name.c_str()));
    }

    auto codec = QTextCodec::codecForLocale();
    auto name = this->tts->getVoiceName();
    auto qName = QString(codec->toUnicode(name.c_str()));
    auto index = this->combo->findText(qName);
    this->combo->setCurrentIndex(index);

    this->textEdit = new DandDableEdit();
    this->textEdit->setTabChangesFocus(true);
    this->textEdit->setText(u8"何か入力してください。");
    
    auto vbox = new QVBoxLayout;
    vbox->addWidget(this->combo);
    vbox->addWidget(this->textEdit);
    auto hbox = new QHBoxLayout();
    hbox->addWidget(this->buttonSay);
    hbox->addWidget(this->buttonStop);
    hbox->addWidget(this->buttonChange);
    vbox->addLayout(hbox);

    this->setLayout(vbox);
    this->setTabOrder(this->combo,this->textEdit);
    this->setTabOrder(this->textEdit,this->buttonSay);
    this->setTabOrder(this->buttonSay,this->buttonStop);
    this->setTabOrder(this->buttonStop,this->buttonChange);
    this->setTabOrder(this->buttonChange,this->combo);
    this->show();
    this->textEdit->setFocus();
}


int main(int argc, char ** argv)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    new JTalkWindow;
    return app.exec();
}

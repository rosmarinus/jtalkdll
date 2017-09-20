#pragma once

#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QThread>
#include <QtGui/QDropEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QTextCursor>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "jtalkcpp.h"

class DandDableEdit : public QTextEdit {
public:
    QString readJapaneseText(QString &path);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

class EnterClickButton : public QPushButton {
    
public:
    EnterClickButton(const char *text);

    void keyPressEvent(QKeyEvent *event);
};

class SpeakThread: public QThread {
    Q_OBJECT
    
private:
    JTalk *tts;
    QTextEdit *edit;
    QTextCursor cursor;
    QString text;
    bool stopped;
    bool selected;
    int startpos;
    int end;

public:
    SpeakThread(JTalk *tts,QTextEdit *edit);
    void stop();

protected:
    void run();

private:
    int searchNextSentence(QString &text);
    
signals:
    void finished(const QString &text);

public slots:
    void emitFinishedSignal(const QString &text); 


};


class JTalkWindow : public QWidget {
    Q_OBJECT

private:
    EnterClickButton *buttonSay;
    EnterClickButton *buttonStop;
    EnterClickButton *buttonChange;
    QComboBox *combo;
    QString selectedText;
    DandDableEdit *textEdit;
    JTalk *tts;
    SpeakThread *th;

public:
    JTalkWindow();

public slots:
    void buttonSayClicked();
    void buttonStopClicked();
    void buttonChangeClicked();
    void speechFinished(const QString &text);
    void comboSelected(const QString &string);
    //void comboSelected(int index);
    //void handleSelectionChanged(int index);
    
private:
    void initUI();
};

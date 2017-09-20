#! /usr/bin/python
# -*- coding: utf-8 -*-

import sys
import re
import platform
import jtalk

# PySide
#from PySide.QtGui import (QDropEvent,QTextCursor)
#from PySide.QtCore import (Qt,QThread,Signal)
#from PySide.QtGui import (QMainWindow, QApplication, QWidget, QPushButton, QMessageBox, QFileDialog, QHBoxLayout, QVBoxLayout, QComboBox, QTextEdit)

# PyQt5
from PyQt5.QtGui import (QDropEvent,QTextCursor)
from PyQt5.QtCore import (Qt,QThread,pyqtSignal)
from PyQt5.QtWidgets import (QApplication,QWidget,QPushButton,QMessageBox,QFileDialog,QHBoxLayout,QVBoxLayout,QComboBox,QTextEdit)
Signal = pyqtSignal


# py2 str -> unicode
#def ustr(text):
#    if text == None or text == "":
#        text = ""
#    if platform.python_version_tuple()[0] == '2' and type(text) == str:
#        text = text.decode('utf-8')
#    return text

class DandDableEdit(QTextEdit):
    def __init__(self):
        super(DandDableEdit,self).__init__()

    def readJapaneseText(self,path):
        encs = ('utf_8', 'shift_jis', 'euc_jp', 'utf_16')
        f = open(path,'rb')
        binary = f.read()
        if len(binary) > 10000:
            QMessageBox.about(self, u'エラー', u'ファイルが大きすぎます')
            return None
        for enc in encs:
            try:
                s = binary.decode(enc)
                return s
            except Exception as e:
                pass
        QMessageBox.about(self, u'エラー', u'不明なエンコードのファイルです')
        return None

    def dragEnterEvent(self,event):
        if event.mimeData().hasUrls():
            event.accept()
        else:
            event.ignore()

    def dragMoveEvent(self,event):
        if event.mimeData().hasUrls():
            event.accept()
        else:
            event.ignore()

    def dropEvent(self,event):
        keep = self.toPlainText()
        super(DandDableEdit,self).dropEvent(event)
        if event.mimeData().hasUrls():
            self.setText('')
            for url in event.mimeData().urls():
                p = url.toLocalFile()
                t = self.toPlainText()
                d = self.readJapaneseText(p)
                if d != None:
                    self.setText(t+d)
                else:
                    self.setText(keep)
                    return

class EnterClickButton(QPushButton):
    def keyPressEvent(self,event):
        QPushButton.keyPressEvent(self,event)
        if event.key() in (Qt.Key_Enter,Qt.Key_Return):
            self.animateClick()

class SpeakThread(QThread):

    finished = Signal(str)

    def __init__(self,tts,edit):
        super(SpeakThread,self).__init__()
        self._stop = False
        self._tts = tts
        self._edit = edit
        self._cursor = edit.textCursor()
        self._start = 0
        self._text = edit.toPlainText()
        if self._cursor.hasSelection():
            self._text = self._text.replace(u'\u2029',u'\n')
            self._start = self._cursor.selectionStart()
            self._text = self._text[:self._cursor.selectionEnd()]
        else:
            self._start = self._cursor.position()
            while True:
                if self._start == len(self._text):
                    self._start = 0
                    break
                if self._text[self._start] == '\n':
                    self._start += 1
                else:
                    break

    def stop(self):
        self._stop = True

    def run(self):
        top = self._start
        length = 0
        lastLine = False
        try:
            while True:
                substr = self._text[top:]
                m = re.match(u'([^\n。]*?)(。\n|。|\n)',substr)
                if m:
                    length = m.end()
                    data = substr[:length]
                else:
                    length = len(substr)
                    data = substr
                    lastLine = True

                data = data.strip()
                if data != "":
                    self._cursor.setPosition(top,QTextCursor.MoveAnchor)
                    self._cursor.setPosition(top+length,QTextCursor.KeepAnchor)
                    self._edit.setTextCursor(self._cursor)
                    self._tts.speakAsync(data)
                    while self._tts.isSpeaking:
                        if self._stop:
                            return

                if lastLine:
                    break
                if len(self._text) < top + length:
                    break
                top += length
        finally:
            self._cursor.clearSelection()
            self._cursor.setPosition(top+length,QTextCursor.MoveAnchor)
            self._edit.setTextCursor(self._cursor)
            self.finished.emit(self._text)

class JTalkWindow(QWidget):
    def __init__(self):
        super(JTalkWindow,self).__init__()
        self.tts = jtalk.JTalk()
        self.initUI()
        self.th = None

    def speechFinished(self,message):
        self.textEdit.setReadOnly(False)

    def buttonSayClicked(self):
        if self.th and self.th.isRunning():
            self.th.stop()
            self.th.wait()
        self.textEdit.setReadOnly(True)
        self.textEdit.setFocus()
        self.th = SpeakThread(self.tts,self.textEdit)
        self.th.finished.connect(self.speechFinished)
        self.th.start()

    def buttonStopClicked(self):
        if self.th and self.th.isRunning():
            self.th.stop()
            self.th.wait()
            self.th = None
        self.tts.stop()

    def comboSelected(self,string):
        self.tts.voiceName = string

    def buttonChangeClicked(self):
        dialog = QFileDialog()
        dir = dialog.getExistingDirectory(
            self, u'音響モデルフォルダ選択', self.tts.voiceDir, QFileDialog.ShowDirsOnly)
        if dir == '':
            return
        self.tts.voiceDir = dir
        name = self.tts.voiceName
        self.combo.clear()
        for it in self.tts.voices:
            self.combo.addItem(it['name'])
        self.combo.setCurrentText(name)

    def initUI(self):
        self.setWindowTitle(u'発声テスト')

        self.buttonSay = EnterClickButton(u'発声')
        self.buttonSay.clicked.connect(self.buttonSayClicked)

        self.buttonStop = EnterClickButton(u'停止')
        self.buttonStop.clicked.connect(self.buttonStopClicked)

        self.buttonChange = EnterClickButton(u'音声フォルダ')
        self.buttonChange.clicked.connect(self.buttonChangeClicked)

        self.combo = QComboBox()
        self.combo.activated[str].connect(self.comboSelected)
        for it in self.tts.voices:
            self.combo.addItem(it['name'])
        index = self.combo.findText(self.tts.voiceName)
        self.combo.setCurrentIndex(index)
        
        self.textEdit = DandDableEdit()
        self.textEdit.setTabChangesFocus(True)
        self.textEdit.setText(u'何か入力してください。')
        
        vbox = QVBoxLayout()
        vbox.addWidget(self.combo)
        vbox.addWidget(self.textEdit)
        hbox = QHBoxLayout()
        hbox.addWidget(self.buttonSay)
        hbox.addWidget(self.buttonStop)
        hbox.addWidget(self.buttonChange)
        vbox.addLayout(hbox)

        self.setLayout(vbox)
        self.setTabOrder(self.combo,self.textEdit)
        self.setTabOrder(self.textEdit,self.buttonSay)
        self.setTabOrder(self.buttonSay,self.buttonStop)
        self.setTabOrder(self.buttonStop,self.buttonChange)
        self.setTabOrder(self.buttonChange,self.combo)
        self.show()
        self.textEdit.setFocus()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = JTalkWindow()
    sys.exit(app.exec_())


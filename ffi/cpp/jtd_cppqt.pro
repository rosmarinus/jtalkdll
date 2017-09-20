# qmake project file

win32 {
    JTALKDIR = $$(JTALKDIR)
    isEmpty( JTALKDIR ) {
        JTALKDIR = "c:/open_jtalk"
    }
    TEMPLATE = vcapp
    LIBS = -L$$(JTALKDIR)/lib
    LIBS += jtalk64.lib
    INCLUDEPATH = $$(JTALKDIR)/include
}

unix {
    QTDIR = $$(QTDIR)
    isEmpty( QTDIR ) {
        error( "please, set QTDIR" )
    }
    TEMPLATE = app
    LIBS = -L$$(QTDIR)/lib
    LIBS += -L/usr/local/lib
    LIBS += -ljtalk
    INCLUDEPATH = $$(QTDIR)/include
    INCLUDEPATH += /usr/local/include
}

QT += core
QT += widgets
HEADERS += jtalkcpp.h
HEADERS += jtd_cppqt.h
SOURCES += jtalkcpp.cpp
SOURCES += jtd_cppqt.cpp
TARGET = jtd_cppqt
INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

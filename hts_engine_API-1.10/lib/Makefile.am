
EXTRA_DIST = Makefile.mak

AM_CPPFLAGS = -I @top_srcdir@/include

lib_LTLIBRARIES = libHTSEngine.la

libHTSEngine_la_SOURCES = HTS_audio.c  HTS_engine.c  HTS_hidden.h  HTS_misc.c  \
                         HTS_pstream.c  HTS_sstream.c  HTS_model.c  HTS_vocoder.c \
                         HTS_gstream.c HTS_label.c

libHTSEngine_la_LDFLAGS = -no-undefined


DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

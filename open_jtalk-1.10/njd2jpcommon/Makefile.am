
EXTRA_DIST = Makefile.mak

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = -I @top_srcdir@/njd -I @top_srcdir@/jpcommon @CHARSET@

noinst_LTLIBRARIES = libnjd2jpcommon.la

libnjd2jpcommon_la_SOURCES = njd2jpcommon.h \
                            njd2jpcommon.c \
                            njd2jpcommon_rule_ascii_for_euc_jp.h \
                            njd2jpcommon_rule_ascii_for_shift_jis.h \
                            njd2jpcommon_rule_ascii_for_utf_8.h \
                            njd2jpcommon_rule_euc_jp.h \
                            njd2jpcommon_rule_shift_jis.h \
                            njd2jpcommon_rule_utf_8.h

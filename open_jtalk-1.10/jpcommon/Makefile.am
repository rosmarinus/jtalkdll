
EXTRA_DIST = Makefile.mak

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = @CHARSET@

noinst_LTLIBRARIES = libjpcommon.la

libjpcommon_la_SOURCES = jpcommon.h \
                        jpcommon.c \
                        jpcommon_node.c \
                        jpcommon_label.c \
                        jpcommon_rule_ascii_for_euc_jp.h \
                        jpcommon_rule_ascii_for_shift_jis.h \
                        jpcommon_rule_ascii_for_utf_8.h \
                        jpcommon_rule_euc_jp.h \
                        jpcommon_rule_shift_jis.h \
                        jpcommon_rule_utf_8.h

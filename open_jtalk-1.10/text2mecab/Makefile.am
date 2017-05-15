
EXTRA_DIST = Makefile.mak

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = @CHARSET@

noinst_LTLIBRARIES = libtext2mecab.la

libtext2mecab_la_SOURCES = text2mecab.h \
                          text2mecab.c \
                          text2mecab_rule_ascii_for_euc_jp.h \
                          text2mecab_rule_ascii_for_shift_jis.h \
                          text2mecab_rule_ascii_for_utf_8.h \
                          text2mecab_rule_euc_jp.h \
                          text2mecab_rule_shift_jis.h \
                          text2mecab_rule_utf_8.h


EXTRA_DIST = Makefile.mak

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = -I @top_srcdir@/njd @CHARSET@

noinst_LTLIBRARIES = libnjd_set_unvoiced_vowel.la

libnjd_set_unvoiced_vowel_la_SOURCES = njd_set_unvoiced_vowel.h \
                                      njd_set_unvoiced_vowel.c \
                                      njd_set_unvoiced_vowel_rule_ascii_for_euc_jp.h \
                                      njd_set_unvoiced_vowel_rule_ascii_for_shift_jis.h \
                                      njd_set_unvoiced_vowel_rule_ascii_for_utf_8.h \
                                      njd_set_unvoiced_vowel_rule_euc_jp.h \
                                      njd_set_unvoiced_vowel_rule_shift_jis.h \
                                      njd_set_unvoiced_vowel_rule_utf_8.h

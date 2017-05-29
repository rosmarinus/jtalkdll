EXTRA_DIST = Makefile.mak

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES  = libjtalk.la

libjtalk_la_SOURCES = jtalk.c

#libjtalk_la_LIBADD = 	@top_srcdir@/lib/libopenjtalk.la 
libjtalk_la_LIBADD = libopenjtalk.la 

libjtalk_la_LDFLAGS =  -fvisibility=hidden -no-undefined -version-info 0:0:0 @LIB_ICONV@ @LIBS_PORTAUDIO@ -lm -lstdc++ -lHTSEngine

libjtalk_la_CFLAGS = -I ../lib/

noinst_LTLIBRARIES = libopenjtalk.la

libopenjtalk_la_LIBADD = @top_srcdir@/text2mecab/libtext2mecab.la \
	@top_srcdir@/mecab/src/libmecab.la \
	@top_srcdir@/mecab2njd/libmecab2njd.la \
	@top_srcdir@/njd/libnjd.la \
	@top_srcdir@/njd_set_pronunciation/libnjd_set_pronunciation.la \
	@top_srcdir@/njd_set_digit/libnjd_set_digit.la \
	@top_srcdir@/njd_set_accent_phrase/libnjd_set_accent_phrase.la \
	@top_srcdir@/njd_set_accent_type/libnjd_set_accent_type.la \
	@top_srcdir@/njd_set_unvoiced_vowel/libnjd_set_unvoiced_vowel.la \
	@top_srcdir@/njd_set_long_vowel/libnjd_set_long_vowel.la \
	@top_srcdir@/njd2jpcommon/libnjd2jpcommon.la \
	@top_srcdir@/jpcommon/libjpcommon.la 
#	@top_srcdir@/hts_engine_API/libHTSEngine.la

libopenjtalk_la_CFLAGS = -I @top_srcdir@/text2mecab \
              -I @top_srcdir@/mecab/src \
              -I @top_srcdir@/mecab2njd \
              -I @top_srcdir@/njd \
              -I @top_srcdir@/njd_set_pronunciation \
              -I @top_srcdir@/njd_set_digit \
              -I @top_srcdir@/njd_set_accent_phrase \
              -I @top_srcdir@/njd_set_accent_type \
              -I @top_srcdir@/njd_set_unvoiced_vowel \
              -I @top_srcdir@/njd_set_long_vowel \
              -I @top_srcdir@/njd2jpcommon \
              -I @top_srcdir@/jpcommon \
              -I @HTS_ENGINE_HEADER_DIR@

libopenjtalk_la_SOURCES = openjtalk.c

bin_PROGRAMS = jsay

jsay_SOURCES = jsay.c

jsay_LDADD = libjtalk.la

jsay_LDFLAGS =  -all-static -no-undefined -o jsay


INSTALLDIR = C:\open_jtalk

all:
	cd text2mecab
	nmake /f Makefile.mak
	cd ..
	cd mecab
	nmake /f Makefile.mak
	cd ..
	cd mecab2njd
	nmake /f Makefile.mak
	cd ..
	cd njd
	nmake /f Makefile.mak
	cd ..
	cd njd_set_pronunciation
	nmake /f Makefile.mak
	cd ..
	cd njd_set_digit
	nmake /f Makefile.mak
	cd ..
	cd njd_set_accent_phrase
	nmake /f Makefile.mak
	cd ..
	cd njd_set_accent_type
	nmake /f Makefile.mak
	cd ..
	cd njd_set_unvoiced_vowel
	nmake /f Makefile.mak
	cd ..
	cd njd_set_long_vowel
	nmake /f Makefile.mak
	cd ..
	cd njd2jpcommon
	nmake /f Makefile.mak
	cd ..
	cd jpcommon
	nmake /f Makefile.mak
	cd ..
	cd lib
	nmake /f Makefile.mak
	cd ..
	cd bin
	nmake /f Makefile.mak
	cd ..
	cd mecab-naist-jdic
	nmake /f Makefile.mak
	cd ..

clean:
	cd text2mecab
	nmake /f Makefile.mak clean
	cd ..
	cd mecab
	nmake /f Makefile.mak clean
	cd ..
	cd mecab2njd
	nmake /f Makefile.mak clean
	cd ..
	cd njd
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_pronunciation
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_digit
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_accent_phrase
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_accent_type
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_unvoiced_vowel
	nmake /f Makefile.mak clean
	cd ..
	cd njd_set_long_vowel
	nmake /f Makefile.mak clean
	cd ..
	cd njd2jpcommon
	nmake /f Makefile.mak clean
	cd ..
	cd jpcommon
	nmake /f Makefile.mak clean
	cd ..
	cd lib
	nmake /f Makefile.mak clean
	cd ..
	cd bin
	nmake /f Makefile.mak clean
	cd ..
	cd mecab-naist-jdic
	nmake /f Makefile.mak clean
	cd ..

install::
	@if not exist "$(INSTALLDIR)\include" mkdir "$(INSTALLDIR)\include"
	@if not exist "$(INSTALLDIR)\bin" mkdir "$(INSTALLDIR)\bin"
	@if not exist "$(INSTALLDIR)\dic_utf_8" mkdir "$(INSTALLDIR)\dic_utf_8"
	@if not exist "$(INSTALLDIR)\lib" mkdir "$(INSTALLDIR)\lib"
	cd lib
	copy *.exe $(INSTALLDIR)\bin
	copy *.dll $(INSTALLDIR)\bin
	copy *.lib $(INSTALLDIR)\lib
	copy jtalk.h $(INSTALLDIR)\include
	cd ..
	cd bin
	copy *.exe $(INSTALLDIR)\bin
	cd ..
	cd mecab-naist-jdic
	copy char.bin $(INSTALLDIR)\dic_utf_8
	copy matrix.bin $(INSTALLDIR)\dic_utf_8
	copy sys.dic $(INSTALLDIR)\dic_utf_8
	copy unk.dic $(INSTALLDIR)\dic_utf_8
	copy left-id.def $(INSTALLDIR)\dic_utf_8
	copy right-id.def $(INSTALLDIR)\dic_utf_8
	copy rewrite.def $(INSTALLDIR)\dic_utf_8
	copy pos-id.def $(INSTALLDIR)\dic_utf_8
	cd ..

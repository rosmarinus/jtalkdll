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
	@if not exist git.revision for /f "delims=" %%a in ('git show --format^="%h" -s') do echo GIT_REV=\^"%a\^">git.revision
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
	@cd lib
	copy jsay.exe $(INSTALLDIR)\bin
	copy jtd_c.exe $(INSTALLDIR)\bin
	@rem copy *.dll $(INSTALLDIR)\bin
	@rem copy *.lib $(INSTALLDIR)\lib
	copy jtalk.h $(INSTALLDIR)\include
	if exist "jtalk32.dll" copy jtalk32.dll $(INSTALLDIR)\bin\jtalk32.dll
	if exist "jtalk32.lib" copy jtalk32.lib $(INSTALLDIR)\bin\jtalk32.lib
	if exist "jtalk64.dll" copy jtalk64.dll $(INSTALLDIR)\bin\jtalk64.dll
	if exist "jtalk64.lib" copy jtalk64.lib $(INSTALLDIR)\bin\jtalk64.lib
	if $(REAL) == x86   if exist "jtalk.dll" copy jtalk.dll $(INSTALLDIR)\bin\jtalk.dll
	if $(REAL) == x86   if exist "jtalk.lib" copy jtalk.dll $(INSTALLDIR)\bin\jtalk.lib
	if $(REAL) == AMD64 if exist "jtalk.dll" copy jtalk.dll $(INSTALLDIR)\bin\jtalk.dll
	if $(REAL) == AMD64 if exist "jtalk.lib" copy jtalk.dll $(INSTALLDIR)\bin\jtalk.lib
	@rem if $(REAL) == x86   if exist "jtalk32.dll" copy jtalk32.dll ..\..\ffi\luajit\jtalk.dll
	@rem if $(REAL) == AMD64 if exist "jtalk64.dll" copy jtalk64.dll ..\..\ffi\luajit\jtalk.dll
	if exist "jtalkcom32.dll" copy regist_jtalkcom.bat $(INSTALLDIR)\bin
	if exist "jtalkcom32.dll" copy unregist_jtalkcom.bat $(INSTALLDIR)\bin
	if exist "jtalkcom64.dll" copy regist_jtalkcom.bat $(INSTALLDIR)\bin
	if exist "jtalkcom64.dll" copy unregist_jtalkcom.bat $(INSTALLDIR)\bin

	@rem if exist "jtalkcom32.dll" copy jtalkCOM32.dll ..\..\ffi\csharp
	@rem if exist "jtalkcom32.dll" copy jtalkCOM32.dll ..\..\ffi\cppcli
	@rem if exist "jtalkcom32.dll" copy jtalkCOM32.dll ..\..\ffi\vb
	@rem if exist "jtalkcom64.dll" copy jtalkCOM64.dll ..\..\ffi\csharp
	@rem if exist "jtalkcom64.dll" copy jtalkCOM64.dll ..\..\ffi\cppcli
	@rem if exist "jtalkcom64.dll" copy jtalkCOM64.dll ..\..\ffi\vb
	@rem copy jtalk*.dll ..\..\ffi\cbuilder
	@rem copy jtalk*.dll ..\..\ffi\d
	@rem copy jtalk*.lib ..\..\ffi\cppcli
	@rem copy jtalk*.lib ..\..\ffi\cpp
	@rem copy jtalk*.lib ..\..\ffi\d
	@rem copy jtalk.h ..\..\ffi\cppcli
	@rem copy jtalk.h ..\..\ffi\cpp
	@rem copy jtalk.h ..\..\ffi\swift
	@rem copy jtalk.h ..\..\ffi\objc

	@cd ..
	@cd bin
	copy open_jtalk.exe $(INSTALLDIR)\bin
	@rem copy *.exe $(INSTALLDIR)\bin
	@cd ..
	@cd mecab-naist-jdic
	copy char.bin $(INSTALLDIR)\dic_utf_8
	copy matrix.bin $(INSTALLDIR)\dic_utf_8
	copy sys.dic $(INSTALLDIR)\dic_utf_8
	copy unk.dic $(INSTALLDIR)\dic_utf_8
	copy left-id.def $(INSTALLDIR)\dic_utf_8
	copy right-id.def $(INSTALLDIR)\dic_utf_8
	copy rewrite.def $(INSTALLDIR)\dic_utf_8
	copy pos-id.def $(INSTALLDIR)\dic_utf_8
	@cd ..

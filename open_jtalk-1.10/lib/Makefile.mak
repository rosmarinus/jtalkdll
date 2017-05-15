CC = cl
CL = link
AR = lib

LIBNAME = openjtalk
DLLNAME = jtalk
MDLLNAME = JTalkCOM

!IF $(AMD64)
ARCH=64
DLLMACROS=/D _x64_
PORTAUDIOLIB = ..\..\portaudio\portaudio_static_x64.lib
!ELSE
DLLMACROS=
ARCH=32
PORTAUDIOLIB = ..\..\portaudio\portaudio_static_x86.lib
!ENDIF

DLLFILE = $(DLLNAME)$(ARCH).dll
MANAGEDDLLFILE =  $(MDLLNAME)$(ARCH).dll
IMPORTLIB = $(DLLNAME)$(ARCH).lib
DEFFILE = $(DLLNAME)$(ARCH).def
LIBFILE = $(LIBNAME).lib
SAMPLES = test_lib.exe test_c.exe test_cpp.exe
SNKFILE = $(MDLLNAME)$(ARCH).snk
TLBFILE = $(MDLLNAME)$(ARCH).tlb
DOCFILE = $(MDLLNAME)$(ARCH).xdc
TARGET = $(DLLFILE) $(MANAGEDDLLFILE) jsay.exe $(SAMPLES)

!IF EXIST($(SNKFILE))
OPTKEY = /KEYFILE:$(SNKFILE)
!ELSE
OPTKEY = 
!ENDIF

CFLAGS = /MD /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TC /I ..\text2mecab /I ..\mecab\src /I ..\mecab2njd /I ..\njd /I ..\njd_set_pronunciation /I ..\njd_set_digit /I ..\njd_set_accent_phrase /I ..\njd_set_accent_type /I ..\njd_set_unvoiced_vowel /I ..\njd_set_long_vowel /I ..\njd2jpcommon /I ..\jpcommon /I C:\hts_engine_API\include /I ..\..\portaudio
LFLAGS = /LTCG

LIBS = ..\text2mecab\text2mecab.lib ..\mecab\src\mecab.lib ..\mecab2njd\mecab2njd.lib ..\njd\njd.lib ..\njd_set_pronunciation\njd_set_pronunciation.lib ..\njd_set_digit\njd_set_digit.lib ..\njd_set_accent_phrase\njd_set_accent_phrase.lib ..\njd_set_accent_type\njd_set_accent_type.lib ..\njd_set_unvoiced_vowel\njd_set_unvoiced_vowel.lib ..\njd_set_long_vowel\njd_set_long_vowel.lib ..\njd2jpcommon\njd2jpcommon.lib ..\jpcommon\jpcommon.lib C:\hts_engine_API\lib\hts_engine_API.lib

all: $(TARGET)

.c.obj:
	$(CC) $(CFLAGS) /c $<

$(LIBFILE) : $(@B).obj
	$(AR) $(LFLAGS) /OUT:$@ $(@B).obj $(LIBS)

$(DLLFILE) : $(DLLNAME).obj $(LIBNAME).lib
	$(CL) /DEF:$(DEFFILE) /DLL /LTCG /OUT:$@ $(DLLNAME).obj $(LIBNAME).lib $(LIBS) $(PORTAUDIOLIB) user32.lib Advapi32.lib winmm.lib Shell32.lib

$(MANAGEDDLLFILE) : $(MDLLNAME).cpp $(DLLNAME).obj
	$(CC) /clr /c  $(MDLLNAME).cpp  $(DLLMACROS) /doc"$(DOCFILE)"
	$(CL) /LTCG /DLL /OUT:$@ $(MDLLNAME).obj $(DLLNAME).obj $(LIBNAME).lib $(PORTAUDIOLIB) user32.lib Advapi32.lib winmm.lib $(OPTKEY)  Shell32.lib

jsay.exe : $(@B).c
	$(CC) /MD /I ..\lib /O2 /c $(@B).c
	$(CL) $(LFLAGS) /OUT:$@ $(@B).obj $(DLLNAME).obj $(LIBNAME).lib $(PORTAUDIOLIB) User32.lib Advapi32.lib Shell32.lib
#	$(CL) /OUT:$@ $(@B).obj $(IMPORTLIB)

test_lib.exe : $(@B).c
	$(CC) /MD /I ..\..\portaudio /O2 /c $(@B).c
	$(CL) $(LFLAGS) /OUT:$@ $(@B).obj $(LIBNAME).lib $(PORTAUDIOLIB) User32.lib Advapi32.lib

test_c.exe : $(@B).c
	$(CC) /MD /I ..\lib /O2 /c $(@B).c
#	$(CL) $(LFLAGS) /OUT:$@ $(@B).obj $(DLLNAME).obj $(LIBNAME).lib $(PORTAUDIOLIB) User32.lib Advapi32.lib
	$(CL) /OUT:$@ $(@B).obj $(IMPORTLIB)

test_cpp.exe : $(@B).cpp
	$(CC) /MD /I ..\lib /O2 /c /EHsc $(@B).cpp
#	$(CL) $(LFLAGS) /OUT:$@ $(@B).obj $(DLLNAME).obj $(LIBNAME).lib $(PORTAUDIOLIB) User32.lib Advapi32.lib
	$(CL) /OUT:$@ $(@B).obj $(IMPORTLIB)

clean:
	del *.exe
	del *.obj
	del *.dll
	del *.exp
	del *.lib

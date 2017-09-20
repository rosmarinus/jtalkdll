CC = cl
CL = link
AR = lib

LIBNAME = openjtalk
DLLNAME = jtalk
MDLLNAME = JTalkCOM
STATICLIB = ON

# version
VER_MAJOR = 0
VER_MINOR = 0
VER_BUILD = 0
MAJOR=0
MINOR=0
!include main.version
!IF $(MAJOR)
VER_MAJOR = $(MAJOR)
!ENDIF
!IF $(MINOR)
VER_MINOR = $(MINOR)
!ENDIF
!include build.version
!IF $(BUILD)
VER_BUILD = $(BUILD)
!ENDIF

# git.revision
!IF EXIST(git.revision)
!include git.revision
!ENDIF

DLLMACROS = /DVER_MAJOR=$(VER_MAJOR) /DVER_MINOR=$(VER_MINOR) /DVER_BUILD=$(VER_BUILD) /DGIT_REV=$(GIT_REV)

!IF $(AMD64)
ARCH=64
DLLMACROS = $(DLLMACROS) /D _x64_
!ELSE
DLLMACROS = $(DLLMACROS)
ARCH=32
!ENDIF

PORTAUDIOLIB = ..\..\portaudio\lib\portaudio_static_$(ARCH).lib
DLLFILE = $(DLLNAME)$(ARCH).dll
DEFFILE = $(DLLNAME).def
MANAGEDDLLFILE = $(MDLLNAME)$(ARCH).dll
IMPORTLIB = $(DLLNAME)$(ARCH).lib
LIBFILE = $(LIBNAME)$(ARCH).lib
SNKFILE = $(MDLLNAME)$(ARCH).snk
TLBFILE = $(MDLLNAME)$(ARCH).tlb
DOCFILE = $(MDLLNAME)$(ARCH).xdc
SAMPLES = jtd_c.exe
TARGET = $(DLLFILE) $(MANAGEDDLLFILE) jsay.exe $(SAMPLES)

!IF EXIST($(SNKFILE))
OPTKEY = /KEYFILE:$(SNKFILE)
!ELSE
OPTKEY = 
!ENDIF

!IF DEFINED(DEFFILE)
COPTDEF =
OPTDEF = /DEF:$(DEFFILE)
!ELSE
COPTDEF = /DNODEFFILE
OPTDEF =
!ENDIF

CFLAGS = /MD /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TC /I ..\text2mecab /I ..\mecab\src /I ..\mecab2njd /I ..\njd /I ..\njd_set_pronunciation /I ..\njd_set_digit /I ..\njd_set_accent_phrase /I ..\njd_set_accent_type /I ..\njd_set_unvoiced_vowel /I ..\njd_set_long_vowel /I ..\njd2jpcommon /I ..\jpcommon /I C:\hts_engine_API\include /I ..\..\portaudio\include
LIBS = ..\text2mecab\text2mecab.lib ..\mecab\src\mecab.lib ..\mecab2njd\mecab2njd.lib ..\njd\njd.lib ..\njd_set_pronunciation\njd_set_pronunciation.lib ..\njd_set_digit\njd_set_digit.lib ..\njd_set_accent_phrase\njd_set_accent_phrase.lib ..\njd_set_accent_type\njd_set_accent_type.lib ..\njd_set_unvoiced_vowel\njd_set_unvoiced_vowel.lib ..\njd_set_long_vowel\njd_set_long_vowel.lib ..\njd2jpcommon\njd2jpcommon.lib ..\jpcommon\jpcommon.lib C:\hts_engine_API\lib\hts_engine_API.lib
LFLAGS = /LTCG

!IF DEFINED(STATICLIB)
LINKOBJ = $(DLLNAME).obj $(LIBNAME).obj
LINKFLAGS = $(LFLAGS) $(DLLNAME).obj $(LIBFILE) $(PORTAUDIOLIB) User32.lib Advapi32.lib
!ELSE
LINKOBJ = $(DLLNAME).obj $(LIBNAME).obj
LINKFLAGS = $(IMPORTLIB)
!ENDIF

all: $(TARGET)

.c.obj:
	$(CC) $(CFLAGS) /c $<

$(LIBFILE) : $(LIBNAME).obj main.version build.version git.revision
	$(AR) $(LFLAGS) /OUT:$@ $(LIBNAME).obj $(LIBS)

$(DLLFILE) : $(DLLNAME).obj $(LIBFILE) main.version build.version git.revision
	$(CC) $(COPTDEF) $(CFLAGS) /c jtalk.c $(DLLMACROS)
	$(CL) /LTCG $(OPTDEF) /DLL /implib:$(IMPORTLIB) /OUT:$@ $(DLLNAME).obj $(LIBFILE) $(LIBS) $(PORTAUDIOLIB) user32.lib Advapi32.lib winmm.lib Shell32.lib kernel32.lib Shlwapi.lib

$(MANAGEDDLLFILE) : $(MDLLNAME).cpp $(DLLNAME).obj $(LIBNAME).obj
	$(CC) /clr /c  $(MDLLNAME).cpp $(DLLMACROS) /doc"$(DOCFILE)"
	$(CL) /LTCG /DLL /OUT:$@ $(MDLLNAME).obj $(DLLNAME).obj $(LIBFILE) $(PORTAUDIOLIB) user32.lib Advapi32.lib winmm.lib $(OPTKEY) Shell32.lib

jsay.exe : $(@B).c $(DLLNAME).obj $(LIBNAME).obj main.version build.version git.revision
	$(CC) /MD /I ..\lib /O2 /c $(@B).c
	$(CL) /OUT:$@ $(@B).obj $(LINKFLAGS)

jtd_c.exe : $(@B).c $(DLLNAME).obj $(LIBNAME).obj main.version build.version git.revision
	$(CC) /MD /I ..\lib /O2 /c $(@B).c
	$(CL) /OUT:$@ $(@B).obj $(LINKFLAGS)

clean:
	del *.exe
	del *.obj
	del *.dll
	del *.exp
	del *.lib
	del git.revision

CC = cl

CFLAGS = /MD /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TC /I ../njd /D CHARSET_UTF_8 /D ASCII_HEADER
LFLAGS = /LTCG

CORES = njd_set_long_vowel.obj

all: njd_set_long_vowel.lib

njd_set_long_vowel.lib: $(CORES)
	lib $(LFLAGS) /OUT:$@ $(CORES)

.c.obj:
	$(CC) $(CFLAGS) /c $<

clean:
	del *.lib
	del *.obj

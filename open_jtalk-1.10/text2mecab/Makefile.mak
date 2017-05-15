
CC = cl

CFLAGS = /MD /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TC /D ASCII_HEADER /D CHARSET_UTF_8
LFLAGS = /LTCG

CORES = text2mecab.obj

all: text2mecab.lib

text2mecab.lib: $(CORES)
	lib $(LFLAGS) /OUT:$@ $(CORES)

.c.obj:
	$(CC) $(CFLAGS) /c $<

clean:
	del *.lib
	del *.obj

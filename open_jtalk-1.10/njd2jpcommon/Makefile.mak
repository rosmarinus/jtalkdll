
CC = cl

CFLAGS = /MD /O2 /Ob2 /Oi /Ot /Oy /GT /GL /TC /I ../njd /I ../jpcommon /D CHARSET_UTF_8  /D ASCII_HEADER
LFLAGS = /LTCG

CORES = njd2jpcommon.obj

all: njd2jpcommon.lib

njd2jpcommon.lib: $(CORES)
	lib $(LFLAGS) /OUT:$@ $(CORES)

.c.obj:
	$(CC) $(CFLAGS) /c $<

clean:
	del *.lib
	del *.obj

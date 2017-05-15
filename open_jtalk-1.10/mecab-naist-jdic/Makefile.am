
EXTRA_DIST = COPYING Makefile.mak naist-jdic.csv matrix.def _left-id.def _pos-id.def _rewrite.def _right-id.def char.def unk.def feature.def

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in char.bin matrix.bin sys.dic unk.dic left-id.def rewrite.def pos-id.def right-id.def

dicdir = @DICDIR@

dic_DATA   = char.bin matrix.bin sys.dic unk.dic left-id.def rewrite.def pos-id.def right-id.def

char.bin matrix.bin sys.dic unk.dic left-id.def rewrite.def pos-id.def right-id.def: naist-jdic.csv matrix.def _left-id.def _rewrite.def _pos-id.def _right-id.def char.def unk.def feature.def
	cp _left-id.def left-id.def
	cp _rewrite.def rewrite.def
	cp _pos-id.def pos-id.def
	cp _right-id.def right-id.def
	../mecab/src/mecab-dict-index -d . -o . -f UTF-8 -t UTF-8

clean:
	rm -f char.bin matrix.bin sys.dic unk.dic left-id.def rewrite.def pos-id.def right-id.def

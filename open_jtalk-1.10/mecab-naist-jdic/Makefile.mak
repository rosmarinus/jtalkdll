
all: char.bin matrix.bin sys.dic unk.dic

char.bin matrix.bin sys.dic unk.dic: naist-jdic.csv matrix.def left-id.def pos-id.def rewrite.def right-id.def char.def unk.def feature.def
	..\mecab\src\mecab-dict-index.exe -d . -o . -f UTF-8 -t UTF-8

left-id.def : _left-id.def
	copy _left-id.def left-id.def

pos-id.def : _pos-id.def
	copy _pos-id.def pos-id.def

rewrite.def : _rewrite.def
	copy _rewrite.def rewrite.def

right-id.def : _right-id.def
	copy _right-id.def right-id.def

clean:
	del char.bin matrix.bin sys.dic unk.dic

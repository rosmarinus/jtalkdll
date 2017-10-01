if not exist "left-id.def"  copy _left-id.def  left-id.def
if not exist "pos-id.def"   copy _pos-id.def   pos-id.def
if not exist "rewrite.def"  copy _rewrite.def  rewrite.def
if not exist "right-id.def" copy _right-id.def right-id.def
if not exist "left-id.def"  copy _left-id.def  left-id.def
if not exist "pos-id.def"   copy _pos-id.def   pos-id.def
if not exist "rewrite.def"  copy _rewrite.def  rewrite.def
if not exist "right-id.def" copy _right-id.def right-id.def
..\..\mecab-dict-index -d . -o . -f UTF-8 -t UTF-8
copy char.bin     $(INSTALLDIR)\dic_utf_8
copy matrix.bin   $(INSTALLDIR)\dic_utf_8
copy sys.dic      $(INSTALLDIR)\dic_utf_8
copy unk.dic      $(INSTALLDIR)\dic_utf_8
copy left-id.def  $(INSTALLDIR)\dic_utf_8
copy right-id.def $(INSTALLDIR)\dic_utf_8
copy rewrite.def  $(INSTALLDIR)\dic_utf_8
copy pos-id.def   $(INSTALLDIR)\dic_utf_8
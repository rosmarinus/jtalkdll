if not exist "left-id.def"  copy _left-id.def  left-id.def
if not exist "pos-id.def"   copy _pos-id.def   pos-id.def
if not exist "rewrite.def"  copy _rewrite.def  rewrite.def
if not exist "right-id.def" copy _right-id.def right-id.def
if not exist "left-id.def"  copy _left-id.def  left-id.def
if not exist "pos-id.def"   copy _pos-id.def   pos-id.def
if not exist "rewrite.def"  copy _rewrite.def  rewrite.def
if not exist "right-id.def" copy _right-id.def right-id.def
..\..\mecab-dict-index -d . -o . -f UTF-8 -t UTF-8

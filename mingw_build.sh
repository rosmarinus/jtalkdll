#!/usr/bin/env bash
# arch
archs=( "" "x86_64-w64-mingw32" "i686-w64-mingw32" )
arch=${archs[0]}

# version
open_jtalk_ver=1.10
hts_engine_api_ver=1.10
open_jtalk_dic_ver=1.10

# configure option
if [ "$arch" == "" ] ; then
	CC="gcc -std=c11"
	CXX="g++ -std=c++11"
else
	CC="$arch-gcc -std=c11"
	CXX="$arch-g++  -std=c++11"
fi
export CC
export CXX

hpath=/usr/local/include
lpath=/usr/local/lib
if [ -n "$MSYSTEM" ]
then
  case "$MSYSTEM" in
    MINGW32)
    hpath=/mingw32/include
	lpath=/mingw32/lib
    ;;
    MINGW64)
    hpath=/mingw64/include
	lpath=/mingw64/lib
    ;;
    *)
  esac
fi

open_jtalk=open_jtalk-${open_jtalk_ver}
hts_engine_api=hts_engine_API-${hts_engine_api_ver}

cd ${hts_engine_api}
./configure
make
if [[ $(uname) =~ $re ]]; then
make install
else
sudo make install
fi
cd ..

cd ${open_jtalk}
./configure --with-hts-engine-header-path=$hpath --with-hts-engine-library-path=$lpath
make
if [[ $(uname) =~ $re ]]; then
make install
else
sudo make install
fi
cd ..

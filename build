#!/usr/bin/env bash
# command line option
# $1: i686 | x86_64 : target architecture
re="(MINGW|CYGWIN)"
if [[ $(uname) =~ $re ]]; then
	bash build_portaudio_mingw $1
fi
if [ -f ../incbuild ]; then
    bash ../incbuild
fi
if [ ! "$1" == "" ] ; then
    param=-Dtarget_arch=$1
fi
if [[ $(uname) =~ "MINGW" ]]; then
    arch=${MSYSTEM_CHOST}
elif [[ $(uname) =~ "CYGWIN" ]]; then
	if [ "$1" == "" ] ; then
		arch=$(uname -m)
	else
		arch=$1
	fi
	arch=$arch-w64-mingw32
fi
build_dir=build$arch.dir
mkdir -p ${build_dir}
cd ${build_dir}
cmake .. $param
make
if [[ $(uname) =~ $re ]]; then
    make install/strip
else
    sudo make install/strip
fi
cd ..

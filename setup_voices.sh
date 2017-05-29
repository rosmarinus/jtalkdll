#!/usr/bin/env bash
# arch
archs=( "" "x86_64-w64-mingw32" "i686-w64-mingw32" )
arch=${archs[0]}

# version
mmdagent_example_ver=1.7

# dir name
voice=voice

# hts_voice
hts_voice=hts_voice_nitech_jp_atr503_m001-1.05

# download from open-jtalk
if [! type wget > /dev/null 2>&1 ]; then
	echo exit
fi
files=($hts_voice)
base_url=http://downloads.sourceforge.net/open-jtalk/
for file in ${files[@]}; do
	if [ ! -e $file.tar.gz ]; then
		wget ${base_url}$file.tar.gz
	fi
done

# make voice dir
if [ ! -d $voice ]; then
	mkdir $voice
fi

# hts_voice
if [! type tar > /dev/null 2>&1 ]; then
	exit
fi
if [ ! -d $voice/$hts_voice ]; then
	tar zxvf ${hts_voice}.tar.gz -C $voice
fi

# download from tohoku github
dir=$voice/htsvoice-tohoku-f01
if [ ! -d $dir ]; then
	mkdir $dir
	names=(angry happy neutral sad)
	base_url=https://github.com/icn-lab/htsvoice-tohoku-f01/raw/master/
	for name in ${names[@]}; do
		if [ ! -e $dir/tohoku-f01-${name}.htsvoice ]; then
			wget ${base_url}tohoku-f01-${name}.htsvoice -P $dir
		fi
	done
	files=(COPYRIGHT.txt README.md)
	for file in ${files[@]}; do
		if [ ! -e $dir/$file ]; then
			wget ${base_url}$file -P $dir
		fi
	done
fi

# download from mmdagent
if [! type unzip > /dev/null 2>&1 ]; then
	exit
fi
dir=$voice/mei
if [ ! -d $dir ]; then
	mkdir $dir
	base_url=http://downloads.sourceforge.net/mmdagent/
	file=MMDAgent_Example-${mmdagent_example_ver}
	if [ ! -e ${file}.zip ]; then
		wget ${base_url}${file}.zip
	fi
	unzip ${file}.zip ${file}/Voice/mei/*.*
	mv ${file}/Voice/mei/*.* $dir
	rm -r ${file}
fi

re="^CYGWIN|MINGW"
if [[ $(uname) =~ $re  ]]; then
mkdir -p /c/open_jtalk
cp -r $voice /c/open_jtalk
else
sudo mkdir -p /usr/local/OpenJTalk
sudo cp -r $voice /usr/local/OpenJTalk
fi
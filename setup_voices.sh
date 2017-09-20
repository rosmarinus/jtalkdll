#!/usr/bin/env bash

# target dir name
target=/usr/local/OpenJTalk
wintarget=/c/open_jtalk

# voice dir name
voice=voice

# version
mmdagent_example_ver=1.7

# check command
#commands=(wget tar git unzip)
#for command in ${commands[@]}; do
#	if ! type $command >/dev/null 2>&1; then
#		echo "$command: command not found"
#		exit
#	fi
#done

# make voice dir
if [ ! -d $voice ]; then
	mkdir $voice
fi

# download from open-jtalk
name=hts_voice_nitech_jp_atr503_m001-1.05
if [ ! -d $voice/$name ]; then
	if [ ! -e $name.tar.gz ]; then
		$command=wget
		if ! type $command >/dev/null 2>&1; then
			echo "$command: command not found"
			exit
		fi
		wget http://downloads.sourceforge.net/open-jtalk/$name.tar.gz
	fi
	$command=tar
	if ! type $command >/dev/null 2>&1; then
		echo "$command: command not found"
		exit
	fi
	tar zxvf $name.tar.gz -C $voice
fi

# download from tohoku github
name=htsvoice-tohoku-f01
if [ ! -d $voice/$name ]; then
	$command=git
	if ! type $command >/dev/null 2>&1; then
		echo "$command: command not found"
		exit
	fi
	cd $voice
	git clone https://github.com/icn-lab/$name.git
	cd ..
fi

# download from mmdagent
dir=$voice/mei
name=MMDAgent_Example-$mmdagent_example_ver
if [ ! -d $dir ]; then
	mkdir $dir
	if [ ! -e $name.zip ]; then
		$command=wget
		if ! type $command >/dev/null 2>&1; then
			echo "$command: command not found"
			exit
		fi
		wget $http://downloads.sourceforge.net/mmdagent/$name.zip
	fi
	$command=unzip
	if ! type $command >/dev/null 2>&1; then
		echo "$command: command not found"
		exit
	fi
	unzip $name.zip $name/Voice/mei/*.*
	mv $name/Voice/mei/*.* $dir
	rm -r $name
fi

# install
if [[ $(uname) =~ "MINGW"  ]]; then
	mkdir -p $wintarget
	cp -r $voice $wintarget
elif [[ $(uname) =~ "CYGWIN" ]]; then
#	mkdir -p /cygdrive$wintarget
#	cp -r $voice /cygdrive$wintarget
	mkdir -p $target
	cp -r $voice $target
elif [[ $(uname) =~ "-Microsoft" ]]; then
	mkdir -p /mnt$wintarget
	cp -r $voice /mnt$wintarget
else
	sudo mkdir -p $target
	sudo cp -r $voice $target
fi

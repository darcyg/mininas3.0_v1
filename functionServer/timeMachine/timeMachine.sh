#!/bin/bash

if [ $# -ne 1 ]
then 
	echo "usage: timeMachine.sh [path]"
	exit -1
fi

NETATALK_PATH=$1

require_package(){
	apt-get install avahi-daemon libavahi-client-dev libdb-dev libssl-dev libgcrypt11-dev libevent-dev libwrap0-dev libglib2.0-dev
}

install_netatalk(){
	make
	make install
}

require_package

cd NETATALK_PATH
./configure --prefix= --with-acls=no --with-init-style=debian-sysv CFLAGS=-DO_IGNORE

#install_netatalk

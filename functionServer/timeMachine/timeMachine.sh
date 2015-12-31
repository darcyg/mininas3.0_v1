#!/bin/bash

if [ $# -ne 1 ]
then 
	echo "usage: timeMachine.sh [path]"
	exit -1
fi

NETATALK_PATH=$1

require_package(){
	apt-get install build-essential libevent-dev libssl-dev libgcrypt11-dev libkrb5-dev libpam0g-dev libwrap0-dev libdb-dev libtdb-dev libmysqlclient-dev avahi-daemon libavahi-client-dev libldap2-dev libcrack2-dev systemtap-sdt-dev libdbus-1-dev libdbus-glib-1-dev libglib2.0-dev tracker libtracker-sparql-1.0-dev libtracker-miner-1.0-dev
}

install_netatalk(){
	make
	make install
}

configure_package(){
	./configure \
	        --with-init-style=debian-systemd \
			--without-libevent \
			--without-tdb \
			--with-cracklib \
			--enable-krbV-uam \
			--with-pam-confdir=/etc/pam.d \
			--with-dbus-sysconf-dir=/etc/dbus-1/system.d \
			--with-tracker-pkgconfig-version=1.0 \
			CFLAGS=-DO_IGNORE
}

require_package

cd $NETATALK_PATH
#iconfigure_package
#./configure --prefix= --with-acls=no --with-init-style=debian-sysv CFLAGS=-DO_IGNORE
#install_netatalk

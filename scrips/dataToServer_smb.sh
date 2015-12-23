#!/bin/bash

if [ $# -ne 1 ];
then
	printf "usage:./dataToServer <path>\n"
	exit
fi

HOMEPATH=$1

cat ${HOMEPATH}/passwd.mig >> /etc/passwd
cat ${HOMEPATH}/group.mig >> /etc/group
cat ${HOMEPATH}/shadow.mig >> /etc/shadow
cp ${HOMEPATH}/gshadow.mig /etc/gshadow

cp ${HOMEPATH}/smbpasswd.mig  /usr/bin/smbpasswd
cp ${HOMEPATH}/passdb.mig /var/lib/samba/passdb.tdb

printf "@finished\n"

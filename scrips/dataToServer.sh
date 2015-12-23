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
/bin/cp ${HOMEPATH}/gshadow.mig /etc/gshadow

printf "finished\n"

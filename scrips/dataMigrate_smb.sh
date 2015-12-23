#!/bin/sh

if [ $# -ne 1 ];
then
	printf "usage:Migrate.sh <path>\n"
	exit
fi

HOMEDIR=$1/MOVE
export UGIDLIMIT=500

rm -rf ${HOMEDIR}
mkdir ${HOMEDIR}
awk -v LIMIT=$UGIDLIMIT -F: '($3>=LIMIT) && ($3!=65534)' /etc/passwd > ${HOMEDIR}/passwd.mig
awk -v LIMIT=$UGIDLIMIT -F: '($3>=LIMIT) && ($3!=65534)' /etc/group > ${HOMEDIR}/group.mig
awk -v LIMIT=$UGIDLIMIT -F: '($3>=LIMIT) && ($3!=65534) {print $1}' /etc/passwd | tee - |egrep -f - /etc/shadow > ${HOMEDIR}/shadow.mig
cp /etc/gshadow ${HOMEDIR}/gshadow.mig

cp /usr/bin/smbpasswd ${HOMEDIR}/smbpasswd.mig
cp /var/lib/samba/passdb.tdb ${HOMEDIR}/passdb.mig

printf "@finished\n"

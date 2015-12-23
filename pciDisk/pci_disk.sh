#!/bin/bash

if [ $# -ne 1 ];
then
	printf "usage:./pci_disk <boardname>\n"
	exit -1
fi
i=1
BOARDNAME=$1
if [ "${BOARDNAME}" == "BARCELONA" ];
then
	tmp=$(lsblk |awk '$6 == "disk"  {print $1}')
	
	for SD in $tmp
	do
		h=$(udevadm info -a --name=/dev/$SD |grep KERNELS==|grep host)
		h=${h#*==\"}
		HOST=${h%*\"}		

		id=$(udevadm info --query=all --name=/dev/$SD |grep ID_BUS)
		IDBUS=${id#*=}
		
		if [ $IDBUS == "usb" ]
		then
			continue
		fi
		printf "${HOST}\t$SD\n"
	done
fi

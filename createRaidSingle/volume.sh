#!/bin/sh

if [ $# -ne 1 ]
then
	printf "usage:volume <function>\n"
	exit
fi

NAMEFUNCTION=$1
State(){
	printf "State function\n"
}

add_single(){
	printf "add_single function\n"
}

add_raid1(){
	printf "add_raid1 function\n"
}

delete_all(){
	printf "delete_all function\n"
}

recovery_raid1(){
	printf "recovery_raid1 function\n"
}

case ${NAMEFUNCTION} in
	"State")
		State
		;;
	"add_single")
		add_single
		;;
	"add_raid1")
		add_raid1
		;;
	"delete_all")
		delete_all
		;;
	"recovery_raid1")
		recovery_raid1
		;;
esac




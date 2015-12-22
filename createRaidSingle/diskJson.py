#!/usr/bin/python

import os

def write_disk_json_file():
	tmp = os.popen('udevadm info --query=all --name=/dev/sda').readlines()
	print len(tmp)
	for i in range(len(tmp)):
		print tmp[1],

write_disk_json_file()

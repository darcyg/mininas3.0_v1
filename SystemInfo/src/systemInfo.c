/*
 * systemInfo.c
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */
#include "../header/common_system.h"

int main(){
	int result = -1;
	char *buf = (char*)calloc(1024,sizeof(char));
	char *qury = "select serial from mini_diskinfo";
//	get_data_from_database(qury,buf);
	result = get_all_disk_info(buf);
//	run_shell("lsblk -ln -o NAME,TYPE | grep disk",buf);
	printf("result=%d\n",result);
	printf("buf=%s\n",buf);

	free(buf);
	return 0;
}

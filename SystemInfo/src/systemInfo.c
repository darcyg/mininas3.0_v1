/*
 * systemInfo.c
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */
#include "../header/common_system.h"

int main(){
	int i,result = -1;
	char *database_disk_buf = (char*)calloc(MIDDLE_LENGTH,sizeof(char));
	char *system_disk_buf = (char*)calloc(MIDDLE_LENGTH,sizeof(char));

	char *qury = "select serial from mini_diskinfo";

	for(i = 0;i < 5;i++){
		//get diskinfo from database
		result = get_data_from_database(qury,database_disk_buf);
		if(result != SUCCESS) {
			memset(database_disk_buf,0,MIDDLE_LENGTH);
			continue;
		}

		//get diskinfo from system
		result = get_all_disk_info(system_disk_buf);
		if(result != SUCCESS) {
			memset(system_disk_buf,0,MIDDLE_LENGTH);
			continue;
		}

		//if the result is SUCCESS,skip out
		if(result == SUCCESS)
			break;
	}

	printf("database_disk_buf=%s\n",database_disk_buf);
	printf("system_disk_buf=%s\n",system_disk_buf);
	free(database_disk_buf);
	free(system_disk_buf);
	return 0;
}

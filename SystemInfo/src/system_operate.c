/*
 * system_operate.c
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */

#include "../header/system_operate.h"

int get_all_disk_info(char *const caller_space)
{
	int result = -1;
	char disk_name[MAXIMUM_NUMBER][16];
	int disk_name_counter = 1;
	char disk_capacity[MAXIMUM_NUMBER][16];
	int disk_capacity_counter = 1;
	char tmp[LARGE_LENGTH] = "";
	char tmp_string[NORMAL_LENGTH] = "";
	char tmp_shell[NORMAL_LENGTH] = "";
	char tmp_disk_infor[LARGE_LENGTH] = "";

	char tmp_name[NORMAL_LENGTH] = "";
	char tmp_path[NORMAL_LENGTH] = "";
	char tmp_type[NORMAL_LENGTH] = "";
	char tmp_serial[NORMAL_LENGTH] = "";
	char tmp_model[NORMAL_LENGTH] = "";
	char tmp_rotation_rate[NORMAL_LENGTH] = "";
	char tmp_vendor[NORMAL_LENGTH] = "";
	char tmp_capacity[NORMAL_LENGTH] = "";
	char tmp_bus[NORMAL_LENGTH] = "";
	char tmp_status[NORMAL_LENGTH] = "";
	char tmp_serial_short[NORMAL_LENGTH] = "";

	//get all device name
	result = run_shell("lsblk -ln -o NAME,TYPE | grep disk | awk '{print $1}'", tmp);
	if (result != SUCCESS)
	{
		return result;
	}


	char *pointer = NULL;
	pointer = strtok(tmp, "\n");
	sprintf(disk_name[disk_name_counter - 1], "%s", pointer);
	while ((pointer = strtok(NULL, "\n")) != NULL)
	{
		disk_name_counter++;
		sprintf(disk_name[disk_name_counter - 1], "%s", pointer);
	}


	//get all device capacity
	tmp[0] = '\0';
	result = run_shell("lsblk -lnb | grep disk | awk '{print $4}'", tmp);
	if (result != SUCCESS)
	{
		return result;
	}

	pointer = strtok(tmp, "\n");
	sprintf(disk_capacity[disk_capacity_counter - 1], "%s", pointer);
	while ((pointer = strtok(NULL, "\n")) != NULL)
	{
		disk_capacity_counter++;
		sprintf(disk_capacity[disk_capacity_counter - 1], "%s", pointer);
	}

	//build json for caller
	strcat(caller_space, "[");

	//recycling sets every property
	int i;
	for(i = 0; i < disk_name_counter; i++)
	{
		tmp_name[0] = '\0';
		tmp_path[0] = '\0';
		tmp_type[0] = '\0';
		tmp_serial[0] = '\0';
		tmp_model[0] = '\0';
		tmp_rotation_rate[0] = '\0';
		tmp_vendor[0] = '\0';
		tmp_capacity[0] = '\0';
		tmp_bus[0] = '\0';
		tmp_status[0] = '\0';
		tmp_serial_short[0] = '\0';
		tmp_disk_infor[0] = '\0';

		sprintf(tmp_shell, "udevadm info --query=all --name=/dev/%s", disk_name[i]);
		result = run_shell(tmp_shell, tmp_disk_infor);
		if (result != SUCCESS)
		{
			return result;
		}

		strcat(tmp_name, disk_name[i]);
		tmp[0] = '\0';
		char tmp_status_shell[NORMAL_LENGTH] = "";
		sprintf(tmp_status_shell, "lsblk -lnb | grep '/$' | grep %s", disk_name[i]);
		result = run_shell(tmp_status_shell, tmp);
		if (result != SUCCESS)
		{
			return result;
		}
		else
		{
			if(strlen(tmp)){
				strcat(tmp_status, "system");
			}else{
				strcat(tmp_status, "normal");
			}
		}

//		printf("%s\n",tmp_status);

		strcat(tmp_capacity, disk_capacity[i]);
		char *tmp_pointer = NULL;
		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_BUS=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_BUS=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_bus[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_bus[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_ATA_ROTATION_RATE_RPM="))	!= NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_ATA_ROTATION_RATE_RPM=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_rotation_rate[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_rotation_rate[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_TYPE=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_TYPE=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_type[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_type[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "DEVNAME=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("DEVNAME=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_path[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_path[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_MODEL=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_MODEL=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_model[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_model[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_SERIAL=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_SERIAL=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_serial[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_serial[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_SERIAL_SHORT=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_SERIAL_SHORT=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_serial_short[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_serial_short[counter] = '\0';
		}

		if ((tmp_pointer = strstr(tmp_disk_infor, "ID_VENDOR=")) != NULL)
		{
			char *tmp_tmp_pointer = NULL;
			int counter = 0;
			tmp_tmp_pointer = &(tmp_pointer[strlen("ID_VENDOR=")]);

			while(tmp_tmp_pointer[counter] != '\n')
			{
				tmp_vendor[counter] = tmp_tmp_pointer[counter];

				counter++;
			}

			tmp_vendor[counter] = '\0';
		}


		//skip system disk
		if((strcmp(tmp_status, "system")) == 0)
		{
			continue;
		}

		strcat(caller_space, "{");
		strcat(caller_space, "\"path\":\"");
		strcat(caller_space, tmp_path);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"type\":\"");
		strcat(caller_space, tmp_type);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"model\":\"");
		strcat(caller_space, tmp_model);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"rotation_rate\":\"");
		strcat(caller_space, tmp_rotation_rate);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"vendor\":\"");
		strcat(caller_space, tmp_vendor);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"capacity\":\"");
		strcat(caller_space, tmp_capacity);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"status\":\"");
		strcat(caller_space, tmp_status);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"serial_short\":\"");
		strcat(caller_space, tmp_serial_short);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"bus\":\"");
		strcat(caller_space, tmp_bus);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"name\":\"");
		strcat(caller_space, tmp_name);
		strcat(caller_space, "\",");

		strcat(caller_space, "\"serial\":\"");
		strcat(caller_space, tmp_serial);
		strcat(caller_space, "\"");

		strcat(caller_space, "},");
	}

	caller_space[strlen(caller_space) - 1] = '\0';

	strcat(caller_space, "]");

//	printf("%s\n",caller_space);

	//skip system disk
	if((strcmp(tmp_status, "system")) == 0)
	{
		if(caller_space[1] == ']')
		{
			caller_space[0] = '\0';
		}
	}

	return SUCCESS;
}

//run shell
int run_shell(const char *shell_contents, char *const caller_space)
{
	FILE *pf;
	int result = -1;
	int counter = 0;
	int flag = 0;

	char tmp[LARGE_LENGTH] = "";
	char tmp_bk[LARGE_LENGTH] = "";
	char tmp_contents[MIDDLE_LENGTH] = "";
	char *pointer;

	pf = popen(shell_contents, "r");
	if(pf == NULL)
	{
		return EXECUTING_SHELL_FAILED;
	}

	memset(tmp, 0, LARGE_LENGTH);
	fread(tmp, LARGE_LENGTH - 1, 1, pf);

	sprintf(tmp_bk, "%s", tmp);

	pointer = strtok(tmp, "\n");
	if(pointer != NULL)
	{
//		sprintf(tmp_contents, "%s", pointer);
//		counter++;
		strcat(caller_space, pointer);
		strcat(caller_space, "\n");
	}
	while((pointer = strtok(NULL, "\n")) != NULL)
	{
//		sprintf(tmp_contents, "%s", pointer);
//		counter++;
		strcat(caller_space, pointer);
		strcat(caller_space, "\n");
	}

//	flag = counter;
//	counter = 0;
//
//	{
//		pointer = strtok(tmp_bk, "\n");
//		if(pointer != NULL)
//		{
//			counter++;
//
//			if(counter <= flag)
//			{
//				strcat(caller_space, pointer);
//				strcat(caller_space, "\n");
//			}
//		}
//		while((pointer = strtok(NULL, "\n")) != NULL)
//		{
//			counter++;
//
//			if(counter <= flag)
//			{
//				strcat(caller_space, pointer);
//				strcat(caller_space, "\n");
//			}
//		}
//	}

	*(caller_space+strlen(caller_space)-1) = '\0';
	pclose(pf);

	return SUCCESS;
}

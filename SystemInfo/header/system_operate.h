/*
 * system_operate.h
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */

#ifndef HEADER_SYSTEM_OPERATE_H_
#define HEADER_SYSTEM_OPERATE_H_

#include "./common_system.h"

int get_all_disk_info(char *const caller_space);

int run_shell(const char *shell_contents, char *const caller_space);

#endif /* HEADER_SYSTEM_OPERATE_H_ */

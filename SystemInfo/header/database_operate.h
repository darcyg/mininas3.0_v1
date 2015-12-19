/*
 * database_operate.h
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */

#ifndef HEADER_DATABASE_OPERATE_H_
#define HEADER_DATABASE_OPERATE_H_

#include "./common_system.h"

#define USING_MYSQL
#define MACHINE_ADDRESS			"localhost"
#define MYSQL_NAME				"root"
#define MYSQL_PASSWORD			"wenshang"
#define MYSQL_DB				"mininas"

/*查询数据库内容
**	传入参数:
**			sql:				用户传入的sql语句,用于指定数据库查询行为
**			caller_space:		用户传入的空间首地址,用于将内容填充至该空间内
**								填充格式为: 数据1	数据2	...
**
**	传出参数:
**			返回值:				100100000 成功
**								100100401 内容未找到
**								100100602 数据库连接失败
**								100100605 SQL语句格式错误
**								100100606 数据库查询失败
**								100100XXX 具体内容请查看错误描述表
*/
int get_data_from_database(const char *sql, char *const caller_space);

/*增加数据库内容
**	传入参数:
**			sql:				用户传入的sql语句,用于指定数据库查询行为
**
**	传出参数:
**			返回值:				100100000 成功
**								100100602 数据库连接失败
**								100100605 SQL语句格式错误
**								100100606 数据库查询失败
**								100100XXX 具体内容请查看错误描述表
*/
int add_data_to_database(const char *sql);

/*删除数据库内容
**	传入参数:
**			sql:				用户传入的sql语句,用于指定数据库查询行为
**
**	传出参数:
**			返回值:				100100000 成功
**								100100602 数据库连接失败
**								100100605 SQL语句格式错误
**								100100606 数据库查询失败
**								100100XXX 具体内容请查看错误描述表
*/
int delete_data_from_database(const char *sql);

#endif /* HEADER_DATABASE_OPERATE_H_ */

/*
 * database_operate.c
 *
 *  Created on: Dec 18, 2015
 *      Author: root
 */

#include"../header/database_operate.h"

//get data from database
int get_data_from_database(const char *sql, char *const caller_space)
{
	if(!sql[0])
	{
		return WRONG_SQL_FORMAT;
	}

#ifdef USING_MYSQL
	MYSQL *conn, *connect_result;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int num_rows, num_columns, tmp;
	int _tmp_result = -1;

	conn = mysql_init(NULL);

	connect_result = mysql_real_connect(conn, MACHINE_ADDRESS, MYSQL_NAME, MYSQL_PASSWORD, MYSQL_DB, 0, NULL, 0);
	if(connect_result == NULL)
	{
		printf("error contents: %s\n", mysql_error(conn));
		mysql_close(conn);
		return CONNECTING_FAILED;
	}
	_tmp_result = mysql_query(conn, sql);
	if(_tmp_result != 0)
	{
		mysql_close(conn);
		return SQL_QUERY_FAILED;
	}

	result = mysql_store_result(conn);
	num_rows = mysql_num_rows(result);
	num_columns = mysql_num_fields(result);
	if(num_rows == 0 || num_columns == 0)
	{
		mysql_free_result(result);
		mysql_close(conn);

		return NOT_FOUND;
	}

	while ((row = mysql_fetch_row(result)))
	{
		for(tmp = 0; tmp < num_columns; tmp ++)
		{
			//Format Just Like: ?????	?????	?????
			if(row[tmp] != NULL && row[tmp] && row[tmp][0])
			{
				strncat(caller_space, row[tmp], strlen(row[tmp]));
				strncat(caller_space, "	", 1);
			}
			else
			{
				strncat(caller_space, "null", 4);
				strncat(caller_space, "	", 1);
			}
		}
	}

	mysql_free_result(result);
	mysql_close(conn);

	return SUCCESS;
#else
	return NOT_USE_MYSQL;
#endif
}

//add data to dababase
int add_data_to_database(const char *sql)
{
	if(!sql[0])
	{
		return WRONG_SQL_FORMAT;
	}

#ifdef USING_MYSQL
	MYSQL *conn, *connect_result;
	int _tmp_result = -1;

	conn = mysql_init(NULL);

	connect_result = mysql_real_connect(conn, MACHINE_ADDRESS, MYSQL_NAME, MYSQL_PASSWORD, MYSQL_DB, 0, NULL, 0);
	if(connect_result == NULL)
	{
		mysql_close(conn);
		return CONNECTING_FAILED;
	}

	_tmp_result = mysql_query(conn, sql);
	if(_tmp_result != 0)
	{
		mysql_close(conn);
		return SQL_QUERY_FAILED;
	}
	else
	{
		mysql_close(conn);
		return SUCCESS;
	}
#else
	return NOT_USE_MYSQL;
#endif
}

//delete data from database
int delete_data_from_database(const char *sql)
{
	if(!sql[0])
	{
		return WRONG_SQL_FORMAT;
	}

#ifdef USING_MYSQL
	MYSQL *conn, *connect_result;
	int _tmp_result = -1;

	conn = mysql_init(NULL);

	connect_result = mysql_real_connect(conn, MACHINE_ADDRESS, MYSQL_NAME, MYSQL_PASSWORD, MYSQL_DB, 0, NULL, 0);
	if(connect_result == NULL)
	{
		mysql_close(conn);
		return CONNECTING_FAILED;
	}

	_tmp_result = mysql_query(conn, sql);
	if(_tmp_result != 0)
	{
		mysql_close(conn);
		return SQL_QUERY_FAILED;
	}
	else
	{
		mysql_close(conn);
		return SUCCESS;
	}
#else
	return NOT_USE_MYSQL;
#endif
}

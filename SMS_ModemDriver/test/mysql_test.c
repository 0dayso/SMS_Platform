/* testsql.c  
 ** An example to use MYSQL C API  
 ** Copyright 2004 Coon Xu.  
 ** Author: Coon Xu  
 ** Date: 05 Nov 2004  
 */  

#include <mysql.h>  
#include <stdio.h>  
int main(){  
	MYSQL mysql;     // need a instance to init  
	MYSQL_RES *res;  
	MYSQL_ROW row;  
	char *query;  
	int t,r;  
	// connect the database   
	mysql_init(&mysql);  
	if (!mysql_real_connect(&mysql,"localhost", "mmim", "mmim", "test",0,NULL,0))  
	{  
		printf( "Error connecting to database: %s\n",mysql_error(&mysql));  
	}  
	else printf("Connected...\n");  

	// get the result from the executing select query  
	query = "select * from t1";  

	t = mysql_real_query(&mysql,query,(unsigned int) strlen(query));  
	if (t)  
	{  
		printf("Error making query: %s\n",  
				mysql_error(&mysql));  
	}  
	else printf("[%s] made...\n", query);   
	res = mysql_store_result(&mysql);   
	while(row = mysql_fetch_row(res))  
	{  
		for(t=0;t<mysql_num_fields(res);t++)  
		{  
			printf("%s ",row[t]);  
		}  
		printf("\n");  
	}  

	printf("mysql_free_result...\n");  
	mysql_free_result(res);     //free result after you get the result  

	sleep(1);      

	// execute the insert query  
	query = "insert into t1(id, name) values(3, 'kunp')";  
	t = mysql_real_query(&mysql, query,(unsigned int) strlen(query));  
	if (t)  
	{  
		printf("Error making query: %s\n",  
				mysql_error(&mysql));  
	}  
	else printf("[%s] made...\n", query);   

	mysql_close(&mysql);  

	return 0;  
}  

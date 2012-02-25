#include "main.h"


//连接数据库
void connecting( CONF_NODE *head )
{   
	mysql_init( &my_connection );

//	if ( mysql_real_connect(&my_connection,"localhost", "root", "123321", "test",0 ,NULL ,0) ) {	
    if ( mysql_real_connect( &my_connection, 
							get_conf_node_value( conf_head, "HOST" ), 
							get_conf_node_value( conf_head, "USERNAME" ), 
							get_conf_node_value( conf_head, "PASSWORD" ), 
							get_conf_node_value( conf_head, "DATABASE" ), 
							0 ,NULL ,0 ) ) 
	{	
		printf("Mysql Connect OK!\n\n");
		mysql_query( &my_connection, "set names utf8" );
	} else {
		printf( "Error connecting to database: %s  (%d)\n", 
				mysql_error(&my_connection), mysql_errno(&my_connection));
	}


}


/*int connecting( CONF_NODE *head )
{   
	mysql_init( &my_connection );
	
	if (!mysql_real_connect(&my_connection,"localhost", "root", "123321", "shortmessage",0,NULL,0))
	{
		printf( "Error connecting to database: %s  (%d)\n",mysql_error(&my_connection), mysql_errno(&my_connection));
	} else {
		printf("Connected...\n");
		mysql_query(&my_connection, "set names utf8");
	}

	return 0;

}*/



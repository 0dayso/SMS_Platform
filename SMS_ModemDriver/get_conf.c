#include "main.h"

//读取配置文件，类似于关联数组
//包括对应名字的相应内容
//例如：HOST = localhosnode_value等等
//		行首为'#'的为注释



//去除字符串两端的空白字符
char *stripwhite ( char *string )
{
	char *start, *end;
  	for ( start = string; whitespace ( *start ); start++ )
    	;
  	if (*start == '\0')
    	return ( start );
	
  	end = start + strlen (start) - 1;
  	while (end > start && whitespace (*end))
    	end--;
  	*++end = '\0';
  	return start;
}

//读取配置文件
CONF_NODE * readcfg( char *filename )
{
	FILE *fp = NULL;
	char buf[ 256 ];
	char *cut, *node_value, *k;
	char *node_name;
	int i, n = 0;
	CONF_NODE *p, *q, *head;
	if( NULL == ( fp = fopen( filename, "r" ) ) )
	{
		printf( "file can't open \n" );
		return NULL;
	}
	
	while( !feof( fp ) )
	{
		fgets( buf, 256, fp );
		for ( node_name = buf; whitespace( *node_name ); node_name++ )
			;
		if( '#' == *node_name ) {		//跳过注释行
			continue;
		}

		p = ( CONF_NODE *)malloc( sizeof( CONF_NODE ) );

		cut = strchr( node_name, '=' );
		if( cut )
		{
			node_value = cut + 1;
			*cut = '\0';
		
			node_name = stripwhite( node_name );
			node_value = stripwhite( node_value );
			//printf( "[%s]: [%s]\n", node_name, node_value );

			strcpy( p->name, node_name );
			strcpy( p->value, node_value );
			
			if( !n )
			{
				head = q = p;
				n++;
				continue;
			}
			q->next = p;
			q = p;
		}

	}
	p->next = NULL;
	fclose( fp );
	return head;
}


//查字典，得到相应配置项的值
char *get_conf_node_value( CONF_NODE *head, char *node_name ) {
	CONF_NODE *scan = head;
	
	while( scan != NULL ) {
		if( STREQ( scan->name, node_name ) ) {
			return scan->value;
		}
		scan = scan->next;
	}
	return NULL;
}

//打印输出配置信息
void print_conf_node( CONF_NODE *head ) {
	CONF_NODE *scan = head;
	printf("\n\n--------------info of conf_node------------\n");
	while( scan != NULL ) {
		printf( "[%s]: [%s]\n", scan->name, scan->value );
		scan = scan->next;
	}
	printf("--------------info of conf_node------------\n\n");
}

//释放开辟的空间
void free_conf_node( CONF_NODE *head ) {
	CONF_NODE *scan, *next_node;
	scan = head;
	DBGPRINT("\n\n--------------start to FREE the conf_node link list------------\n");
	while( scan != NULL ) {
		next_node = scan->next;
		FREE( scan );
		scan = next_node;
	}
	DBGPRINT("--------------FREE the conf_node link list finished!------------\n\n");
}


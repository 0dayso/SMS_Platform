/*
 * A platform based on B/S mode with enough features to be as a SMS server.
 * <SMS_Platform> Copyright (C) <2012>  <Jiangzhe Ren>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Email : jiangzhe78@gmail.com
 */

#include "main.h"

//��ȡ�����ļ��������ڹ�������
//������Ӧ���ֵ���Ӧ����
//���磺HOST = localhosnode_value�ȵ�
//		����Ϊ'#'��Ϊע��


//ȥ���ַ������˵Ŀհ��ַ�
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

//��ȡ�����ļ�
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
		if( '#' == *node_name ) {		//����ע����
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


//���ֵ䣬�õ���Ӧ�������ֵ
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

//��ӡ���������Ϣ
void print_conf_node( CONF_NODE *head ) {
	CONF_NODE *scan = head;
	printf("\n\n--------------info of conf_node------------\n");
	while( scan != NULL ) {
		printf( "[%s]: [%s]\n", scan->name, scan->value );
		scan = scan->next;
	}
	printf("--------------info of conf_node------------\n\n");
}

//�ͷſ��ٵĿռ�
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


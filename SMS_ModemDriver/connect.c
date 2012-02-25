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



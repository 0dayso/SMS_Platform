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
 * Email	: jiangzhe78@gmail.com
 */
 
#include "main.h"

// MYSQL my_connection;

// main函数
// 对串口进行基本操作
// 发送短信
// 读取短信
int main(int argc, char **argv)
{
	conf_head = readcfg( CONF_FILE );		// 初始化配置信息
	char *dev  = get_conf_node_value( conf_head, "SERIAL_DEV" );	

	pthread_t id1, id2, id3, id4;
	
	fd = OpenDev(dev);						// 打开串口
	
	char *bond_rate_c = get_conf_node_value( conf_head, "BOND_RATE" );
	int bond_rate_i = atoi( bond_rate_c );
	// printf( "bond_rate is:%d\n", bond_rate_i );
	set_speed( fd, bond_rate_i );			// 设置串口的波特率
	
	if (set_Parity(fd,8,1,'N') == FALSE) {	// 设置串口的校验位
		printf("Set Parity Error\n");
		exit (0);
	}
	
	// 互斥量，用于控制发送时，进和出对列
	pthread_mutex_init ( &mutex_send, NULL );
	// 互斥量，用于控制接收时，进和出对列
	pthread_mutex_init ( &mutex_receive, NULL );
	// 互斥量，用于控制发送和接受时，对串口的读和写
	pthread_mutex_init ( &mutex, NULL );
	 
	connecting( conf_head );							// 连接数据库
	 
	pthread_create(&id1, NULL, (void*)SendThread2ScanDB, NULL);			// 创建线程一
	pthread_create(&id2, NULL, (void*)SendThread2ScanList, NULL);		// 创建线程二
	pthread_create(&id3, NULL, (void*)RecvThread2ScanPort, NULL);		// 创建线程三
	pthread_create(&id4, NULL, (void*)RecvThread2ScanList, NULL);		// 创建线程四
	
	pthread_join(id1, NULL);				// 结束线程一
	pthread_join(id2, NULL);				// 结束线程二
	pthread_join(id3, NULL);				// 结束线程三
	pthread_join(id4, NULL);				// 结束线程四
	
	close(fd); 							// 关闭串口 
	free_conf_node( conf_head );		// 释放开辟的空间
	mysql_close( &my_connection );		// 关闭数据库
	//mysql_library_end();	//
	return TRUE;
}


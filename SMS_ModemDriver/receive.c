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

struct ReceiveList//接收队列结构体
{
	char num[20];
	char message[256];
	char time[20];
	struct ReceiveList *next;
};

struct ReceiveList *ReceiveHead = NULL,*ReceiveTail = NULL;

/********************接收短信部分,处理短信从串口中读取并进入接收链队列中**************/

//把从串口中读取的数据，放入接收链队列中
void ReceiveAdd( SM_PARAM* pSrc )
{
	struct ReceiveList *p1;
	p1 = (struct ReceiveList *)malloc(sizeof(struct ReceiveList));

	strcpy( p1->num, pSrc->TPA );
	strcpy( p1->message, pSrc->TP_UD );
	strcpy( p1->time, pSrc->TP_SCTS );
	
	if(ReceiveHead == NULL)		//当接收链队列为空时
	{
		ReceiveHead = p1;
		ReceiveTail = ReceiveHead;
		ReceiveTail->next = NULL;
	} else { 					//当接收链队列不为空时
		ReceiveTail->next = p1;
		ReceiveTail = p1;
		ReceiveTail->next = NULL;
	}		
}

// 读取短消息
// 用+CMGL代替+CMGR，可一次性读出全部短消息
// pMsg: 短消息缓冲区，必须足够大
// 返回: 短消息条数
int gsmReadMessage(int fd, SM_PARAM* pMsg)
{
	
	int nLength;	
	int nMsg;			
	char* ptr;			
	char cmd[16];		
	char ans[1024];	
	nMsg = 0;
	ptr = ans;
	count++;
	printf("\n===================gsmReadMessage================  %d\n",count);
	sprintf(cmd, "AT+CMGF=0\nat+cmgr=%d\n",count);
	
	int tmp = 0;
	// !!!!!!!!!!!!!!!!!!!!下面开始串口操作，对串口加锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	write( fd, cmd, strlen(cmd) );	
	sleep(1);
	nLength = read(fd, ans, 1024);
	// !!!!!!!!!!!!!!!!串口操作完毕，对串口解锁!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );
	//printf("65 the ptr:[%s] \n", ptr );
	
	while( nLength ) 
	{
		//printf( "In while\n" );
		//if( (nLength > 0) && (strstr( ans, "+CMS ERROR" ) == NULL) )
		if( nLength > 0 )
		{
			//当串口中没有新消息时，把计数归为零，并提前结束本次循环
			//if((ptr = strstr(ptr, "+CMGR:")) == NULL)
			if( strstr(ptr, "ERROR") != NULL )				
			{
				//printf( "In while :have Error\n" );
				count = 0;
				break;
			}
			printf("the ptr:[%s] \n", ptr );
			
			while((ptr = strstr(ptr, "+CMGR:")) != NULL)
			{
				ptr += 6;		
				sscanf(ptr, "%d", &pMsg->index);	
				//printf("the SMS index:[%d] \n",pMsg->index );
				ptr = strstr(ptr, "\r\n");	
				ptr += 2;		
				if( strstr( ptr, "OK" ) != NULL ) {
					*strstr( ptr, "OK" ) = '\0';		//去掉后面的尾巴
				}
				printf("the ptr in 91:[%s] \n", ptr );
				memset( pMsg, 0, sizeof( SM_PARAM ) );
				int len = gsmDecodePdu( ptr, pMsg );
				
				printf( "IN gsmReadMessage::::::::\n" );
				print_SM_PARAM( pMsg );
				
				
				if(pMsg->TPA != NULL)
				{
					//进接收队列时，进行加锁处理
					pthread_mutex_lock ( &mutex_receive );
					//从串口中读取数据，并进链队列
					ReceiveAdd(pMsg);
					//解锁
					pthread_mutex_unlock ( &mutex_receive );
				}
				
				pMsg++;		
				nMsg++;	
			}
		}
		if( ptr == NULL )
		{
			break;
		}
		// !!!!!!!!!!!!!!!!!!!!下面开始串口操作，对串口加锁!!!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_lock ( &mutex );
		nLength = read( fd, ans, 1024 );
		// !!!!!!!!!!!!!!!!串口操作完毕，对串口解锁!!!!!!!!!!!!!!!!!!!
		pthread_mutex_unlock ( &mutex );
	}
	if(count != 0)
	{
		gsmDeleteMessage( count );//每次读取一条，存入数据库，便删除
	}
	return nMsg;
}

// 删除短消息
// index: 短消息序号，从1开始
int gsmDeleteMessage( int index)
{
	int nLength;		// 串口收到的数据长度
	char cmd[12];		// 命令串
	char ans[128];		// 应答串

	sprintf(cmd, "AT+CMGD=%d\r", index);	// 生成命令

	// !!!!!!!!!!!!!!!!!!!!下面开始串口操作，对串口加锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	// 输出命令串
	write( fd, cmd, strlen(cmd) );
	// 读应答数据
	nLength = read( fd, ans, 128 );
	//printf( "\n========[%s]=======>[%s]\n", cmd, ans );
	// !!!!!!!!!!!!!!!!串口操作完毕，对串口解锁!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );

	// 根据能否找到"+CMS ERROR"决定成功与否
	if( nLength > 0 && strstr(ans, "OK" ) != NULL )
	{
		printf("\n========gsmDeleteMessage=======>%d delete successful !\n\n",index);
		return TRUE;
	}

	return FALSE;
}

//线程三，用于接收
//不停地扫描串口，
//从中取出数据，并放入接收链队列中
void *RecvThread2ScanPort()
{	
	//申请动态内存空间
	SM_PARAM* pMsg=( SM_PARAM *  )malloc( sizeof( SM_PARAM ) );
	while(1)
	{
		sleep(1);
		memset( pMsg, 0, sizeof( SM_PARAM ) );
		//读取串口中的数据
		gsmReadMessage( fd, pMsg );
		//	free(pMsg);
	}
}


/*
 *接收短信部分,
 *处理短信出接收链队列进入数据库中接收表，
 *以及实现自动回复功能
*/

//自动回复
//链队列的结构体
struct AutoReplyList
{
	char num[20];
	char message[256];
	struct AutoReplyList *next;
};
struct AutoReplyList *AutoReplyHead = NULL,*AutoReplyTail = NULL;


//向自动回复队列中添加消息
void AutoReplyAdd( SM_PARAM* pSrc )
{
	struct AutoReplyList *p1;
	p1 = (struct AutoReplyList *)malloc(sizeof(struct AutoReplyList));
	
	strcpy( p1->num, pSrc->TPA );
	strcpy( p1->message, pSrc->TP_UD );
	
	//判断自动回复队列消息是否为空
	if( AutoReplyHead == NULL ) {
		AutoReplyHead = p1;
		AutoReplyTail = AutoReplyHead;
		AutoReplyTail->next = NULL;	
	} else {
		AutoReplyTail->next = p1;
		AutoReplyTail = p1;
		AutoReplyTail->next = NULL;
	}
}

//用于自动回复
//扫描自动回复数据库的信息
void AutoReplyDatabase()
{
	int row,column,i=0,j;
 	char str[20];
	MYSQL_RES *res_ptr;
    MYSQL_ROW result_row;
    MYSQL_FIELD *field;
	//申请动态内存
    SM_PARAM* pMsg=( SM_PARAM *  )malloc( sizeof( SM_PARAM ) );
	mysql_query(&my_connection, "select * from replymessage  ");
	res_ptr = mysql_store_result(&my_connection);
	if (res_ptr)
	{
		for (i = 0; field = mysql_fetch_field(res_ptr); i++)
		{
			printf("%-6s\t", field->name);
		}
		printf("\n");  
		column = mysql_num_fields(res_ptr);
	    row = mysql_num_rows(res_ptr);
	    for (i =0; i < row; i++)
		{
			result_row = mysql_fetch_row(res_ptr);
			for (j = 0; j < column; j++)
			{
				printf("%-6s\t", result_row[j]);
				if(j == 1)
				{
					//为了便于处理，进行号码的规范化
					//printf( "=====str======[%s]\n", str );
					memset( str, 0, sizeof(str) );
					if( strncmp( result_row[j], "86", 2 ) != 0 ) {
						str[0] = '8';
						str[1] = '6';
						str[2] = '\0';
					}
					strcat( str, result_row[j] );
					strcpy( pMsg->TPA, str );
				}
				else if(j == 2)
				{
					strcpy(pMsg->TP_UD, result_row[j]);
				}
			}
			//对自动回复队列，进队列时加锁			   						
			pthread_mutex_lock (&mutex_reply);
			//进自动回复链队列
			AutoReplyAdd( pMsg );
			//对自动回复队列，进队列时解锁
			pthread_mutex_unlock (&mutex_reply);
			printf("\n");
		}
	}
   else
   {
	   printf("自动回复表中无任何信息记录!\n");
   }
}


//用于接收，
//把链队列中的数据取出，
//放入数据库中,并且进行相应的自动回复
int ReceiveIntoDatabase(SM_PARAM* pSrc)
{
	struct AutoReplyList *p1, *p2;
	int i;
	int res_query;
	
	char query[512], str[16], str_time[20], str_num[16];
	
	//处理时间字符，把它转化为0000-00-00 00:00:00,
	//而一般解码以后的形式可以处理为00-00-00 00:00:00
	//由于编码的时候一般没有在年前面加上20，所以写入数据库之前，对其进行处理
	strcat(str, pSrc->TP_SCTS);
	//printf("\n===>%s\n", str);
	//把时间转化为具体的年月日，时分秒格式，即0000-00-00 00:00:00
	sprintf(str_time, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\0", '2', '0', 
			str[0], str[1], str[2], str[3], str[4], str[5], str[6], 
			str[7], str[8], str[9], str[10], str[11] );
	//printf("OK in ReceiveIntoDatabase!\n");
	//print_SM_PARAM( pSrc );
	
	sprintf( query, "insert into `receivemessage` values('%s','%s','%s','%s','%d');", 0, 
				pSrc->TPA, pSrc->TP_UD, str_time, 1 );//把短消息插入数据库
	//printf("[%s] will be made...\n", query); 
	
	res_query = mysql_real_query( &my_connection, query,(unsigned int) strlen(query) );  
	if ( res_query ) {  
		printf("Error making query: %s\n",  
				mysql_error(&my_connection));  
	} else {
		printf("[%s] made...\n", query);  
	}
	
	if(pSrc->TPA != NULL)
	{
		//通过扫描自动回复数据库
		//向自动回复队列中添加信息
		//RJZ:每接受一条短信就要查询一次自动回复表，太浪费资源了吧？！有待斟酌。
		AutoReplyDatabase();
		//处理自动回复链队列  
		//此处为什么要这样处理?
		//提高了程序的效率
		p1 = AutoReplyHead;
		while( p1 != NULL )
		{	 
			printf( "start to autoreply\n\n" );
			//printf("=====p1->num===>[%s]\n", p1->num);
			//printf("=====pSrc->TPA===>[%s]\n", pSrc->TPA);
			
			if( subnstr(pSrc->TPA, p1->num, 0, 11) == 0 )
			{
				sprintf(query, 
						"insert into `sendmessage` values('%s','%s','%s','%s','%s');",
						0, str_time, p1->num, p1->message, 0);
				
				res_query = mysql_real_query( &my_connection, query,
								(unsigned int) strlen(query) );  
				if ( res_query ) {  
					printf("Error making query: %s\n",  
					mysql_error(&my_connection));  
				} else {
					printf("[%s] made...\n", query);  
				}
			} 
			p2 = p1->next;
			free(p1);			//释放本次自动回复消息队列中的信息
			p1 = p2;
		}
	}
	
	return 0;
}


//线程四
//用于接收，不停扫描接收链表
//从接收链队列中取出数据，插入数据库
void *RecvThread2ScanList()
{
	int i;
	struct ReceiveList *p1,*p2;
	//申请动态存储空间
	//用于存储接收到的信息
	SM_PARAM * pSrc = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	//	p1 = p2 = (struct ReceiveList *)malloc(sizeof(struct ReceiveList));
	while(1)
	{
		sleep(1);
		
		struct ReceiveList *tmp_head, *tmp_tail;
	
		// !!!!!!!!!!!!!!!!出链队列加锁!!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_lock ( &mutex_receive );
		tmp_head = ReceiveHead;
		tmp_tail = ReceiveTail;
		// !!!!!!!!!!!!!!!!!!!出链队列解锁!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_unlock ( &mutex_receive );
		
		//判断接收链队列是否为空
		if(tmp_head != NULL)
		{
			//判断接收链队列中是否只有一个元素
			if(tmp_head != tmp_tail)
			{
				// !!!!!!!!!!!!!!!!出链队列加锁!!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_lock ( &mutex_receive );
				p1 = ReceiveHead;
				strcpy(pSrc->TPA,p1->num);
				strcpy(pSrc->TP_UD,p1->message);
				strcpy(pSrc->TP_SCTS,p1->time);
				ReceiveHead = ReceiveHead->next;
				free(p1);
				// !!!!!!!!!!!!!!!!!!!出链队列解锁!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_unlock ( &mutex_receive );
				
				i = ReceiveIntoDatabase(pSrc);
				//	free(pSrc);
				if(i == 0)
				{
					printf("\n1==>Good,Receive Successful!\n");
				}
			} else {
				// !!!!!!!!!!!!!!!!出链队列加锁!!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_lock ( &mutex_receive );
				p2 = ReceiveHead;
				strcpy(pSrc->TPA,p2->num);
				strcpy(pSrc->TP_UD,p2->message);
				strcpy(pSrc->TP_SCTS,p2->time);
				//	ReceiveHead = ReceiveHead->next;
				ReceiveTail = ReceiveHead = NULL;
				free(p2);
				// !!!!!!!!!!!!!!!!!!!出链队列解锁!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_unlock ( &mutex_receive );
				
				i = ReceiveIntoDatabase(pSrc);
				//	free(pSrc);
				if(i == 0)
				{
					printf("\n2==>Good,Receive Successful!\n");
				}
			
			}
		}
	}

}



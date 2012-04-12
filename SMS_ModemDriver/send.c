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
#define CENTER_NUMBER "8613800716500"

// 发送链队列的结构体
struct SendList {
	char num[20];
	char message[256];
	struct SendList *next;
};

struct SendList *SendHead = NULL,*SendTail = NULL;

// 打印SM_PARAM信息
void print_SM_PARAM( SM_PARAM *pSrc ) {
	printf( "\n========================\n" );
	printf( "Number of SMSC:[%s]\n", pSrc->SCA );
	printf( "Number of Dst:[%s]\n", pSrc->TPA );
	printf( "TP_PID:[%d]\n", pSrc->TP_PID );
	printf( "TP_DCS:[%d]\n", pSrc->TP_DCS );
	printf( "TimeStamp:[%s]\n", pSrc->TP_SCTS );
	printf( "Content of SMS:[%s]\n", pSrc->TP_UD );
	printf( "index:[%d]\n", pSrc->index );
	printf( "========================\n" );
}

/****************************发送短信部分,处理短信从数据库SendMessage表中取出并进入发送链队列中****************************/
// 用于发送
// 把从数据库中没有发送的短信进发送队列
void SendAdd(SM_PARAM* pSrc) {
		struct SendList *p1;
		p1 = (struct SendList *)malloc(sizeof(struct SendList));
		
		strcpy( p1->num, pSrc->TPA );
		strcpy( p1->message, pSrc->TP_UD );
		
		// 判断是否为发送队列中的第一个元素
		if(SendHead == NULL) {
			SendHead = p1;
			SendTail = SendHead;
			SendTail->next=NULL;	
		} else {
			SendTail->next = p1;
			SendTail = p1;
			SendTail->next = NULL;
		}
}

// 线程一
// 用于发送短信
// 不停地扫描数据库，从中取出没有进发送队列的短信
void *SendThread2ScanDB( void ) {
	// 申请动态内存空间
	SM_PARAM* pMsg=( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	int row,count,column,i=0,j;
	char query[512];
	int res_query;
	char str[20], mod[100];
	MYSQL_RES *res_ptr;
	MYSQL_ROW result_row;
	MYSQL_FIELD *field; 
	
	mysql_query(&my_connection, "select * from sendmessage where flagsend=0  ");
	res_ptr = mysql_store_result(&my_connection);
	for (i = 0; field = mysql_fetch_field(res_ptr); i++) {
		printf("%-6s\t", field->name);
	}
	printf("\n");
	
	while(1) {
		// printf("\n\n===============IN the while of SendThread2ScanDB=============\n\n" );
		
		// 减缓循环速度，能使服务器更有效
		sleep(1);
		// flagsend为0的表示未发送
		strcpy( query, "select * from `sendmessage` where `flagsend`=0 " );
		res_query = mysql_real_query( &my_connection, 
					query,(unsigned int) strlen(query) );  
		if ( res_query ) {  
			printf("Error making query: %s\n",  
					mysql_error(&my_connection) );  
		} else {
			//printf("[%s] made...\n", query);  
		}
		
		res_ptr = mysql_store_result( &my_connection );
		column = mysql_num_fields(res_ptr);
		row = mysql_num_rows(res_ptr);
		
		if ( row ) {
			for (i =0; i < row; i++) {
				result_row = mysql_fetch_row(res_ptr);
				
				for (j = 0; j < column; j++) {
					printf("%-6s\t", result_row[j]);
				}
				printf("\n"); 
				
				// 目的号码
				memset( str, 0, sizeof(str) );
				if( strncmp( result_row[2], "86", 2 ) != 0 ) {
					str[0] = '8';
					str[1] = '6';
					str[2] = '\0';
				}
				strcat( str, result_row[2] );
				strcpy( pMsg->TPA, str );
				// 短信内容
				strcpy( pMsg->TP_UD, result_row[3] );
				
				// 更新数据库
				sprintf( query, "update `sendmessage` set `flagsend`=1 where `flagsend`=0 ");
				
				res_query = mysql_real_query( &my_connection, 
								query,(unsigned int) strlen(query) );  
				if ( res_query ) {  
					printf("Error making query: %s\n",  
							mysql_error(&my_connection) );  
				} else {
					printf("[%s] made...\n", query);  
				}
				
				// 短信中心号码
				strcpy(pMsg->SCA, CENTER_NUMBER);
				//
				pMsg->TP_PID = 0;
				// 编码为pdu模式
				pMsg->TP_DCS = 8;
				// 对发送队列，进队列时上锁
				pthread_mutex_lock ( &mutex_send );
				// 进发送链队列
				SendAdd(pMsg);
				pthread_mutex_unlock ( &mutex_send );
				// free(pMsg);
				// 对发送队列，进队列时解锁
				printf("\n");
			}
		} else {
			//printf("There are no SMS to send!\n");
		}
	}
	// 关闭与数据库的连接
	// mysql_close(&my_connection); 
}



/****************************发送短信部分,处理短信从发送链队列取出并通过串口发送****************************/
// 发送短消息
// pSrc: 源PDU参数指针
int gsmSendMessage( int fd, SM_PARAM* pSrc ) {
	int nPduLength;		// PDU串长度
	int result = FALSE;
	unsigned char nSmscLength;	// SMSC串长度
	int nLength;		// 串口收到的数据长度
	char cmd[30];		// 命令串
	char pdu[512];		// PDU串
	char ans[128];		// 应答串
	nPduLength = gsmEncodePdu( pSrc, pdu );	// 根据PDU参数，编码PDU串
	
	printf( "PDU is:[%s]\n", pdu );
	
	strcat(pdu, "\x01a");		// 以Ctrl-Z结束
	gsmString2Bytes(pdu, &nSmscLength, 2);	// 取PDU串中的SMSC信息长度
	nSmscLength++;		// 加上长度字节本身
	// 命令中的长度，不包括SMSC信息长度，以数据字节计
	sprintf(cmd, "AT+CMGS=%d\r", nPduLength/2-nSmscLength);	// 生成命令
	//printf("==647==>%s\n",cmd);
	
	// !!!!!!!!!!!!!!!!!!!!!!!下面开始串口操作，对串口加锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	write( fd, cmd, strlen(cmd) );
	nLength = read( fd, ans, 128 );	// 读应答数据
	printf("==ans==>[%s]\n",ans);
	// 根据能否找到"\r\n> "决定成功与否
	if( strstr(ans, "ERROR") == NULL ) {
		printf( "start to send...\n" );
		write( fd, pdu, strlen(pdu) );		// 得到肯定回答，继续输出PDU串
		nLength = read( fd, ans, 128 );		// 读应答数据
		printf("==ans==>[%s]\n",ans);
		// 根据能否找到"+CMS ERROR"决定成功与否
		if(nLength > 0 && strstr(ans, "OK") != NULL) {
			printf( "SEND OK!\n\n" );
			result = TRUE;
		} else {
			printf( "SEND Failure!\n\n" );
			result = FALSE;
		}
	}
	
	// !!!!!!!!!!!!!!!!!!!!!!!串口操作完毕，对串口解锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );
	return result;
}

// 从发送队列中取出没有发送的短信，并发送
void SendShortmessage() {
	int i;
	struct SendList *p1,*p2;
	
	// printf("\n==================IN SendShortmessage==============\n" );
	
	SM_PARAM * pSrc = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );		// 申请动态内存空间
	//	p1=p2=(struct SendList *)malloc(sizeof(struct SendList));
	
	struct SendList *tmp_head, *tmp_tail;
	
	// !!!!!!!!!!!!!!!!!!!!!!!发送，进队列加锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock (&mutex_send);	
	tmp_head = SendHead;
	tmp_tail = SendTail;
	// !!!!!!!!!!!!!!!!!!!!!!!对发送链队列解锁!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex_send );
	
	if(tmp_head != NULL) {
		// sleep(3);			// 减小线程负担
		if(tmp_head != tmp_tail) {
			
			// !!!!!!!!!!!!!!!!!!!!!!!发送，进队列加锁!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_lock (&mutex_send);
			
			p1 = SendHead;
			strcpy( pSrc->TPA, p1->num );
			printf("p1->num=[%s]\n", p1->num );
			strcpy( pSrc->TP_UD, p1->message );
			strcpy( pSrc->SCA, CENTER_NUMBER );
			pSrc->TP_PID = 0;
			pSrc->TP_DCS = 8;
			SendHead = SendHead->next;
			free(p1);
			// !!!!!!!!!!!!!!!!!!!!!!!对发送链队列解锁!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_unlock ( &mutex_send );
			
			// 由于写串口时，需要一定的缓冲时间
			// 经过长时间的调试，时间设为沉睡3比较的合理
			// 发送
			print_SM_PARAM( pSrc );
			printf("==================Before gsmSendMessage===1===========\n" );
			i = gsmSendMessage( fd, pSrc );
			printf("==================After gsmSendMessage===1===========\n" );
			free(pSrc);
			if(i == 0) {
				printf("\n1==>Good,Send  Successfull!\n");
			}
		} else {
			// !!!!!!!!!!!!!!!!!!!!!!!发送，进队列加锁!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_lock ( &mutex_send );
			p2 = SendHead;
			strcpy( pSrc->TPA, p2->num );
			printf("p2->num=[%s]\n", p2->num );
			strcpy( pSrc->TP_UD, p2->message );
			strcpy( pSrc->SCA, CENTER_NUMBER );
			pSrc->TP_PID = 0;
			pSrc->TP_DCS = 8;
			//SendHead = SendHead->next;
			SendTail = SendHead = NULL;
			free(p2);
			
			// !!!!!!!!!!!!!!!!!!!!!!!对发送链队列解锁!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_unlock ( &mutex_send );
			
			// 由于写串口时，需要一定的缓冲时间
			// 经过长时间的调试，时间设为沉睡3比较的合理
			// 发送
			print_SM_PARAM( pSrc );
			printf("==================Before gsmSendMessage==2============\n" );
			i = gsmSendMessage( fd, pSrc );
			printf("==================After gsmSendMessage==2============\n" );
			free(pSrc);
			if(i == 0) {
				printf("\n2==>Good,Send  Successfull!\n");
			}
		}
	}
}

// 线程二
// 用于不停扫描发送链队列
void *SendThread2ScanList() {	
	while(1) {	
		// 减缓循环速度
		// 能使服务器更有效
		sleep(1);
		// 调用发送短消息函数
		SendShortmessage();
		// printf( "==========258==========\n" );
	}
}


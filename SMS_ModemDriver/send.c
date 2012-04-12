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

// ���������еĽṹ��
struct SendList {
	char num[20];
	char message[256];
	struct SendList *next;
};

struct SendList *SendHead = NULL,*SendTail = NULL;

// ��ӡSM_PARAM��Ϣ
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

/****************************���Ͷ��Ų���,������Ŵ����ݿ�SendMessage����ȡ�������뷢����������****************************/
// ���ڷ���
// �Ѵ����ݿ���û�з��͵Ķ��Ž����Ͷ���
void SendAdd(SM_PARAM* pSrc) {
		struct SendList *p1;
		p1 = (struct SendList *)malloc(sizeof(struct SendList));
		
		strcpy( p1->num, pSrc->TPA );
		strcpy( p1->message, pSrc->TP_UD );
		
		// �ж��Ƿ�Ϊ���Ͷ����еĵ�һ��Ԫ��
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

// �߳�һ
// ���ڷ��Ͷ���
// ��ͣ��ɨ�����ݿ⣬����ȡ��û�н����Ͷ��еĶ���
void *SendThread2ScanDB( void ) {
	// ���붯̬�ڴ�ռ�
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
		
		// ����ѭ���ٶȣ���ʹ����������Ч
		sleep(1);
		// flagsendΪ0�ı�ʾδ����
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
				
				// Ŀ�ĺ���
				memset( str, 0, sizeof(str) );
				if( strncmp( result_row[2], "86", 2 ) != 0 ) {
					str[0] = '8';
					str[1] = '6';
					str[2] = '\0';
				}
				strcat( str, result_row[2] );
				strcpy( pMsg->TPA, str );
				// ��������
				strcpy( pMsg->TP_UD, result_row[3] );
				
				// �������ݿ�
				sprintf( query, "update `sendmessage` set `flagsend`=1 where `flagsend`=0 ");
				
				res_query = mysql_real_query( &my_connection, 
								query,(unsigned int) strlen(query) );  
				if ( res_query ) {  
					printf("Error making query: %s\n",  
							mysql_error(&my_connection) );  
				} else {
					printf("[%s] made...\n", query);  
				}
				
				// �������ĺ���
				strcpy(pMsg->SCA, CENTER_NUMBER);
				//
				pMsg->TP_PID = 0;
				// ����Ϊpduģʽ
				pMsg->TP_DCS = 8;
				// �Է��Ͷ��У�������ʱ����
				pthread_mutex_lock ( &mutex_send );
				// ������������
				SendAdd(pMsg);
				pthread_mutex_unlock ( &mutex_send );
				// free(pMsg);
				// �Է��Ͷ��У�������ʱ����
				printf("\n");
			}
		} else {
			//printf("There are no SMS to send!\n");
		}
	}
	// �ر������ݿ������
	// mysql_close(&my_connection); 
}



/****************************���Ͷ��Ų���,������Ŵӷ���������ȡ����ͨ�����ڷ���****************************/
// ���Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
int gsmSendMessage( int fd, SM_PARAM* pSrc ) {
	int nPduLength;		// PDU������
	int result = FALSE;
	unsigned char nSmscLength;	// SMSC������
	int nLength;		// �����յ������ݳ���
	char cmd[30];		// ���
	char pdu[512];		// PDU��
	char ans[128];		// Ӧ��
	nPduLength = gsmEncodePdu( pSrc, pdu );	// ����PDU����������PDU��
	
	printf( "PDU is:[%s]\n", pdu );
	
	strcat(pdu, "\x01a");		// ��Ctrl-Z����
	gsmString2Bytes(pdu, &nSmscLength, 2);	// ȡPDU���е�SMSC��Ϣ����
	nSmscLength++;		// ���ϳ����ֽڱ���
	// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
	sprintf(cmd, "AT+CMGS=%d\r", nPduLength/2-nSmscLength);	// ��������
	//printf("==647==>%s\n",cmd);
	
	// !!!!!!!!!!!!!!!!!!!!!!!���濪ʼ���ڲ������Դ��ڼ���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	write( fd, cmd, strlen(cmd) );
	nLength = read( fd, ans, 128 );	// ��Ӧ������
	printf("==ans==>[%s]\n",ans);
	// �����ܷ��ҵ�"\r\n> "�����ɹ����
	if( strstr(ans, "ERROR") == NULL ) {
		printf( "start to send...\n" );
		write( fd, pdu, strlen(pdu) );		// �õ��϶��ش𣬼������PDU��
		nLength = read( fd, ans, 128 );		// ��Ӧ������
		printf("==ans==>[%s]\n",ans);
		// �����ܷ��ҵ�"+CMS ERROR"�����ɹ����
		if(nLength > 0 && strstr(ans, "OK") != NULL) {
			printf( "SEND OK!\n\n" );
			result = TRUE;
		} else {
			printf( "SEND Failure!\n\n" );
			result = FALSE;
		}
	}
	
	// !!!!!!!!!!!!!!!!!!!!!!!���ڲ�����ϣ��Դ��ڽ���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );
	return result;
}

// �ӷ��Ͷ�����ȡ��û�з��͵Ķ��ţ�������
void SendShortmessage() {
	int i;
	struct SendList *p1,*p2;
	
	// printf("\n==================IN SendShortmessage==============\n" );
	
	SM_PARAM * pSrc = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );		// ���붯̬�ڴ�ռ�
	//	p1=p2=(struct SendList *)malloc(sizeof(struct SendList));
	
	struct SendList *tmp_head, *tmp_tail;
	
	// !!!!!!!!!!!!!!!!!!!!!!!���ͣ������м���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock (&mutex_send);	
	tmp_head = SendHead;
	tmp_tail = SendTail;
	// !!!!!!!!!!!!!!!!!!!!!!!�Է��������н���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex_send );
	
	if(tmp_head != NULL) {
		// sleep(3);			// ��С�̸߳���
		if(tmp_head != tmp_tail) {
			
			// !!!!!!!!!!!!!!!!!!!!!!!���ͣ������м���!!!!!!!!!!!!!!!!!!!!!!!
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
			// !!!!!!!!!!!!!!!!!!!!!!!�Է��������н���!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_unlock ( &mutex_send );
			
			// ����д����ʱ����Ҫһ���Ļ���ʱ��
			// ������ʱ��ĵ��ԣ�ʱ����Ϊ��˯3�Ƚϵĺ���
			// ����
			print_SM_PARAM( pSrc );
			printf("==================Before gsmSendMessage===1===========\n" );
			i = gsmSendMessage( fd, pSrc );
			printf("==================After gsmSendMessage===1===========\n" );
			free(pSrc);
			if(i == 0) {
				printf("\n1==>Good,Send  Successfull!\n");
			}
		} else {
			// !!!!!!!!!!!!!!!!!!!!!!!���ͣ������м���!!!!!!!!!!!!!!!!!!!!!!!
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
			
			// !!!!!!!!!!!!!!!!!!!!!!!�Է��������н���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			pthread_mutex_unlock ( &mutex_send );
			
			// ����д����ʱ����Ҫһ���Ļ���ʱ��
			// ������ʱ��ĵ��ԣ�ʱ����Ϊ��˯3�Ƚϵĺ���
			// ����
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

// �̶߳�
// ���ڲ�ͣɨ�跢��������
void *SendThread2ScanList() {	
	while(1) {	
		// ����ѭ���ٶ�
		// ��ʹ����������Ч
		sleep(1);
		// ���÷��Ͷ���Ϣ����
		SendShortmessage();
		// printf( "==========258==========\n" );
	}
}


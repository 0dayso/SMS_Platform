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

struct ReceiveList//���ն��нṹ��
{
	char num[20];
	char message[256];
	char time[20];
	struct ReceiveList *next;
};

struct ReceiveList *ReceiveHead = NULL,*ReceiveTail = NULL;

/********************���ն��Ų���,������ŴӴ����ж�ȡ�����������������**************/

//�ѴӴ����ж�ȡ�����ݣ����������������
void ReceiveAdd( SM_PARAM* pSrc )
{
	struct ReceiveList *p1;
	p1 = (struct ReceiveList *)malloc(sizeof(struct ReceiveList));

	strcpy( p1->num, pSrc->TPA );
	strcpy( p1->message, pSrc->TP_UD );
	strcpy( p1->time, pSrc->TP_SCTS );
	
	if(ReceiveHead == NULL)		//������������Ϊ��ʱ
	{
		ReceiveHead = p1;
		ReceiveTail = ReceiveHead;
		ReceiveTail->next = NULL;
	} else { 					//�����������в�Ϊ��ʱ
		ReceiveTail->next = p1;
		ReceiveTail = p1;
		ReceiveTail->next = NULL;
	}		
}

// ��ȡ����Ϣ
// ��+CMGL����+CMGR����һ���Զ���ȫ������Ϣ
// pMsg: ����Ϣ�������������㹻��
// ����: ����Ϣ����
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
	// !!!!!!!!!!!!!!!!!!!!���濪ʼ���ڲ������Դ��ڼ���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	write( fd, cmd, strlen(cmd) );	
	sleep(1);
	nLength = read(fd, ans, 1024);
	// !!!!!!!!!!!!!!!!���ڲ�����ϣ��Դ��ڽ���!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );
	//printf("65 the ptr:[%s] \n", ptr );
	
	while( nLength ) 
	{
		//printf( "In while\n" );
		//if( (nLength > 0) && (strstr( ans, "+CMS ERROR" ) == NULL) )
		if( nLength > 0 )
		{
			//��������û������Ϣʱ���Ѽ�����Ϊ�㣬����ǰ��������ѭ��
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
					*strstr( ptr, "OK" ) = '\0';		//ȥ�������β��
				}
				printf("the ptr in 91:[%s] \n", ptr );
				memset( pMsg, 0, sizeof( SM_PARAM ) );
				int len = gsmDecodePdu( ptr, pMsg );
				
				printf( "IN gsmReadMessage::::::::\n" );
				print_SM_PARAM( pMsg );
				
				
				if(pMsg->TPA != NULL)
				{
					//�����ն���ʱ�����м�������
					pthread_mutex_lock ( &mutex_receive );
					//�Ӵ����ж�ȡ���ݣ�����������
					ReceiveAdd(pMsg);
					//����
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
		// !!!!!!!!!!!!!!!!!!!!���濪ʼ���ڲ������Դ��ڼ���!!!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_lock ( &mutex );
		nLength = read( fd, ans, 1024 );
		// !!!!!!!!!!!!!!!!���ڲ�����ϣ��Դ��ڽ���!!!!!!!!!!!!!!!!!!!
		pthread_mutex_unlock ( &mutex );
	}
	if(count != 0)
	{
		gsmDeleteMessage( count );//ÿ�ζ�ȡһ�����������ݿ⣬��ɾ��
	}
	return nMsg;
}

// ɾ������Ϣ
// index: ����Ϣ��ţ���1��ʼ
int gsmDeleteMessage( int index)
{
	int nLength;		// �����յ������ݳ���
	char cmd[12];		// ���
	char ans[128];		// Ӧ��

	sprintf(cmd, "AT+CMGD=%d\r", index);	// ��������

	// !!!!!!!!!!!!!!!!!!!!���濪ʼ���ڲ������Դ��ڼ���!!!!!!!!!!!!!!!!!!!!!!!
	pthread_mutex_lock ( &mutex );
	// ������
	write( fd, cmd, strlen(cmd) );
	// ��Ӧ������
	nLength = read( fd, ans, 128 );
	//printf( "\n========[%s]=======>[%s]\n", cmd, ans );
	// !!!!!!!!!!!!!!!!���ڲ�����ϣ��Դ��ڽ���!!!!!!!!!!!!!!!!!!!
	pthread_mutex_unlock ( &mutex );

	// �����ܷ��ҵ�"+CMS ERROR"�����ɹ����
	if( nLength > 0 && strstr(ans, "OK" ) != NULL )
	{
		printf("\n========gsmDeleteMessage=======>%d delete successful !\n\n",index);
		return TRUE;
	}

	return FALSE;
}

//�߳��������ڽ���
//��ͣ��ɨ�贮�ڣ�
//����ȡ�����ݣ������������������
void *RecvThread2ScanPort()
{	
	//���붯̬�ڴ�ռ�
	SM_PARAM* pMsg=( SM_PARAM *  )malloc( sizeof( SM_PARAM ) );
	while(1)
	{
		sleep(1);
		memset( pMsg, 0, sizeof( SM_PARAM ) );
		//��ȡ�����е�����
		gsmReadMessage( fd, pMsg );
		//	free(pMsg);
	}
}


/*
 *���ն��Ų���,
 *������ų����������н������ݿ��н��ձ�
 *�Լ�ʵ���Զ��ظ�����
*/

//�Զ��ظ�
//�����еĽṹ��
struct AutoReplyList
{
	char num[20];
	char message[256];
	struct AutoReplyList *next;
};
struct AutoReplyList *AutoReplyHead = NULL,*AutoReplyTail = NULL;


//���Զ��ظ������������Ϣ
void AutoReplyAdd( SM_PARAM* pSrc )
{
	struct AutoReplyList *p1;
	p1 = (struct AutoReplyList *)malloc(sizeof(struct AutoReplyList));
	
	strcpy( p1->num, pSrc->TPA );
	strcpy( p1->message, pSrc->TP_UD );
	
	//�ж��Զ��ظ�������Ϣ�Ƿ�Ϊ��
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

//�����Զ��ظ�
//ɨ���Զ��ظ����ݿ����Ϣ
void AutoReplyDatabase()
{
	int row,column,i=0,j;
 	char str[20];
	MYSQL_RES *res_ptr;
    MYSQL_ROW result_row;
    MYSQL_FIELD *field;
	//���붯̬�ڴ�
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
					//Ϊ�˱��ڴ������к���Ĺ淶��
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
			//���Զ��ظ����У�������ʱ����			   						
			pthread_mutex_lock (&mutex_reply);
			//���Զ��ظ�������
			AutoReplyAdd( pMsg );
			//���Զ��ظ����У�������ʱ����
			pthread_mutex_unlock (&mutex_reply);
			printf("\n");
		}
	}
   else
   {
	   printf("�Զ��ظ��������κ���Ϣ��¼!\n");
   }
}


//���ڽ��գ�
//���������е�����ȡ����
//�������ݿ���,���ҽ�����Ӧ���Զ��ظ�
int ReceiveIntoDatabase(SM_PARAM* pSrc)
{
	struct AutoReplyList *p1, *p2;
	int i;
	int res_query;
	
	char query[512], str[16], str_time[20], str_num[16];
	
	//����ʱ���ַ�������ת��Ϊ0000-00-00 00:00:00,
	//��һ������Ժ����ʽ���Դ���Ϊ00-00-00 00:00:00
	//���ڱ����ʱ��һ��û������ǰ�����20������д�����ݿ�֮ǰ��������д���
	strcat(str, pSrc->TP_SCTS);
	//printf("\n===>%s\n", str);
	//��ʱ��ת��Ϊ����������գ�ʱ�����ʽ����0000-00-00 00:00:00
	sprintf(str_time, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\0", '2', '0', 
			str[0], str[1], str[2], str[3], str[4], str[5], str[6], 
			str[7], str[8], str[9], str[10], str[11] );
	//printf("OK in ReceiveIntoDatabase!\n");
	//print_SM_PARAM( pSrc );
	
	sprintf( query, "insert into `receivemessage` values('%s','%s','%s','%s','%d');", 0, 
				pSrc->TPA, pSrc->TP_UD, str_time, 1 );//�Ѷ���Ϣ�������ݿ�
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
		//ͨ��ɨ���Զ��ظ����ݿ�
		//���Զ��ظ������������Ϣ
		//RJZ:ÿ����һ�����ž�Ҫ��ѯһ���Զ��ظ���̫�˷���Դ�˰ɣ����д����á�
		AutoReplyDatabase();
		//�����Զ��ظ�������  
		//�˴�ΪʲôҪ��������?
		//����˳����Ч��
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
			free(p1);			//�ͷű����Զ��ظ���Ϣ�����е���Ϣ
			p1 = p2;
		}
	}
	
	return 0;
}


//�߳���
//���ڽ��գ���ͣɨ���������
//�ӽ�����������ȡ�����ݣ��������ݿ�
void *RecvThread2ScanList()
{
	int i;
	struct ReceiveList *p1,*p2;
	//���붯̬�洢�ռ�
	//���ڴ洢���յ�����Ϣ
	SM_PARAM * pSrc = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	//	p1 = p2 = (struct ReceiveList *)malloc(sizeof(struct ReceiveList));
	while(1)
	{
		sleep(1);
		
		struct ReceiveList *tmp_head, *tmp_tail;
	
		// !!!!!!!!!!!!!!!!�������м���!!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_lock ( &mutex_receive );
		tmp_head = ReceiveHead;
		tmp_tail = ReceiveTail;
		// !!!!!!!!!!!!!!!!!!!�������н���!!!!!!!!!!!!!!!!!!!!!
		pthread_mutex_unlock ( &mutex_receive );
		
		//�жϽ����������Ƿ�Ϊ��
		if(tmp_head != NULL)
		{
			//�жϽ������������Ƿ�ֻ��һ��Ԫ��
			if(tmp_head != tmp_tail)
			{
				// !!!!!!!!!!!!!!!!�������м���!!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_lock ( &mutex_receive );
				p1 = ReceiveHead;
				strcpy(pSrc->TPA,p1->num);
				strcpy(pSrc->TP_UD,p1->message);
				strcpy(pSrc->TP_SCTS,p1->time);
				ReceiveHead = ReceiveHead->next;
				free(p1);
				// !!!!!!!!!!!!!!!!!!!�������н���!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_unlock ( &mutex_receive );
				
				i = ReceiveIntoDatabase(pSrc);
				//	free(pSrc);
				if(i == 0)
				{
					printf("\n1==>Good,Receive Successful!\n");
				}
			} else {
				// !!!!!!!!!!!!!!!!�������м���!!!!!!!!!!!!!!!!!!!!!!
				pthread_mutex_lock ( &mutex_receive );
				p2 = ReceiveHead;
				strcpy(pSrc->TPA,p2->num);
				strcpy(pSrc->TP_UD,p2->message);
				strcpy(pSrc->TP_SCTS,p2->time);
				//	ReceiveHead = ReceiveHead->next;
				ReceiveTail = ReceiveHead = NULL;
				free(p2);
				// !!!!!!!!!!!!!!!!!!!�������н���!!!!!!!!!!!!!!!!!!!!!
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



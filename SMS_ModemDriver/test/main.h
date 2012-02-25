#ifndef 	MAIN_H__
#define  	MAIN_H__

#include<stdio.h>
#include<stdlib.h>  
#include<unistd.h>  
#include<string.h> 
#include<sys/types.h>  
#include<sys/stat.h>   
#include<fcntl.h>     
#include<termios.h>   
#include<errno.h>
#include <locale.h> 
#include <pthread.h>
#include <mysql/mysql.h>
//#include <mysql/mysql_version.h>
//#include <mysql/errmsg.h>

#define FALSE -1
#define TRUE 0

#include <sys/types.h>
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#include <sys/stat.h>

#ifndef DBG
#define DBGPRINT(fmt, args...)   			\
{                                           \
    printf(fmt, ## args);    	\
}
#else
#define DBGPRINT(fmt, args...)
#endif


//start:  ժ��`ReadLine`��һЩ����
//��ȡ�����ļ������������ݿ�
#define whitespace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n'))

#define STREQ(a, b)	(((a)[0] == (b)[0]) && (strcmp ((a), (b)) == 0))
#define STREQN(a, b, n) (((n) == 0) ? (1) \
				    : ((a)[0] == (b)[0]) && (strncmp ((a), (b), (n)) == 0))

#define savestring(x) strcpy (xmalloc (1 + strlen (x)), (x))

#define FREE(x)	if (x) free (x)


//��ȡ�����ļ��Ľṹ�壬�����ڹ�������
//������Ӧ���ֵ���Ӧ����

typedef struct conf_node	
{
        char name[ 32 ];			//�ֶ���
        char value[ 32];			//�ֶ�ֵ
        struct conf_node *next;
} CONF_NODE;

CONF_NODE *conf_head;

char *stripwhite (char*);
CONF_NODE  * readcfg( char *filename );
char *get_conf_node_value( CONF_NODE *head, char *node_name );
void print_conf_node( CONF_NODE *head );
void free_conf_node( CONF_NODE *head );


MYSQL my_connection;
void connnecting( CONF_NODE *head );
//void connecting();
#define CONF_FILE "env.conf"
#define SERIAL_DEV "/dev/ttyS0"

int fd;					//��ȡ���ڵ�fd

pthread_mutex_t mutex_send, mutex_receive, mutex_reply, mutex;	//������
static int count=0;			//���ڶ�ȡ����ʱ������
//end



// �û���Ϣ���뷽ʽ
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

// ����Ϣ�����ṹ������/���빲��
// ���У��ַ����v��β
typedef struct {
	char SCA[16];			// ����Ϣ�������ĺ���SMSC��ַ)
	char TPA[18];			// Ŀ������ظ�����TP-DA��TP-RA)
	char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�Ã�
	char TP_UD[256];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	char index;				// ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

//�������ĸ��߳�
void *SendThread2ScanDB();
void *SendThread2ScanList();
void *RecvThread2ScanPort();
void *RecvThread2ScanList();

//����������
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);

int gsmSendMessage(int fd, SM_PARAM* pSrc);
int gsmReadMessage(int fd, SM_PARAM* pMsg);
int gsmDeleteMessage(int index);
#endif
                                     

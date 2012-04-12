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
#define DBGPRINT(fmt, args...) {   			\
    printf(fmt, ## args);    	\
}
#else
#define DBGPRINT(fmt, args...)
#endif

// 获取配置文件进而连接数据库
#define whitespace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n'))

#define STREQ(a, b)	(((a)[0] == (b)[0]) && (strcmp ((a), (b)) == 0))
#define STREQN(a, b, n) (((n) == 0) ? (1) \
				    : ((a)[0] == (b)[0]) && (strncmp ((a), (b), (n)) == 0))

#define savestring(x) strcpy (xmalloc (1 + strlen (x)), (x))

#define FREE(x)	if (x) free (x)


// 短消息参数结构，编码/解码共用
// 其中，字符串乿结尾
typedef struct {
	char SCA[16];			// 短消息服务中心号瞿SMSC地址)
	char TPA[20];			// 目标号码或回复号瞿TP-DA或TP-RA)
	char TP_PID;			// 用户信息协议标识(TP-PID),00为普通GSM类型，点到点方式
	char TP_DCS;			// 用户信息编码方式(TP-DCS)
	char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用冿
	char TP_UD[256];		// 原始用户信息(编码前或解码后的TP-UD)
	char index;				// 短消息序号，在读取时用到
} SM_PARAM;


// 读取配置文件的结构体，类似于关联数组
// 包括对应名字的相应内容

typedef struct conf_node {
        char name[ 32 ];			// 字段名
        char value[ 32];			// 字段值
        struct conf_node *next;
} CONF_NODE;

CONF_NODE *conf_head;

char *stripwhite (char*);
CONF_NODE  * readcfg( char *filename );
char *get_conf_node_value( CONF_NODE *head, char *node_name );
void print_conf_node( CONF_NODE *head );
void free_conf_node( CONF_NODE *head );

void print_SM_PARAM( SM_PARAM * pSrc );

MYSQL my_connection;
void connnecting( CONF_NODE *head );

#define CONF_FILE "env.conf"
#define SERIAL_DEV "/dev/ttyS0"

int fd;						// 获取串口的fd

pthread_mutex_t mutex_send, mutex_receive, mutex_reply, mutex;	// 互斥量
static int count=0;			// 用于读取串口时，计数

// 比较字符串函数，如果front为0，则从后开始
int subnstr( char *src1, char *src2, int front, int len );
// 截断utf8字符串
void cut_utf8( char *Src, int where );

// 用户信息编码方式
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

// 创建的四个线程
void *SendThread2ScanDB();
void *SendThread2ScanList();
void *RecvThread2ScanPort();
void *RecvThread2ScanList();

// 函数的申明
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

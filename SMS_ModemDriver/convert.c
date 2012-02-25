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

//比较字符串函数，如果front为0，则从后开始
int subnstr( char *src1, char *src2, int front, int len ) {
	char *p1, *p2;
	int len1, len2;
	
	p1 = src1;
	p2 = src2;
	len1 = strlen(src1);
	len2 = strlen(src2);
	
	if( !front && (len<=len1) && (len<=len2) ) {
		p1 = src1 + ( len1 - len );
		p2 = src2 + ( len2 - len );
	}
	printf( "p1=[%s]\n", p1 );
	printf( "p2=[%s]\n", p2 );
	return strncmp( p1, p2, len );
}


//说明一点：XXXXX 2 YYYYY表示从XXXXX to YYYYY
// 可打印字符串转换为字节数据
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: 源字符串指针
// pDst: 目标数据指针
// nSrcLength: 源字符串长度
// 返回: 目标数据长度
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int i = 0;
	for( i = 0; i<nSrcLength; i+=2)      //目的是确保能够循环，并且控制循环次数
	{
		// 输出高4位
		if(*pSrc>='0' && *pSrc<='9') {
			*pDst = (*pSrc - '0') << 4;
		} else {
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// 输出低4位
		if(*pSrc>='0' && *pSrc<='9') {
			*pDst |= *pSrc - '0';
		} else {
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		pDst++;
	}

	// 返回目标数据长度
	return nSrcLength / 2;
}

// 字节数据转换为可打印字符串
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// pSrc: 源数据指针
// pDst: 目标字符串指针
// nSrcLength: 源数据长度
// 返回: 目标字符串长度
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
	int i = 0;
	for( i=0; i<nSrcLength; i++) {
		*pDst++ = tab[*pSrc >> 4];		// 输出高4位
		*pDst++ = tab[*pSrc & 0x0f];	// 输出低4位
		pSrc++;
	}

	// 输出字符串加个结束符
	*pDst = '\0';
	// 返回目标字符串长度
	return nSrcLength * 2;
}

// 7bit编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回: 目标编码串长度

int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nSrc;		// 源字符串的计数值
	int nDst;		// 目标编码串的计数值
	int nChar;		// 当前正在处理的组内字符字节的序号，范围是0-7
	unsigned char nLeft;	// 上一字节残余的数据

	// 计数值初始化
	nSrc = 0;
	nDst = 0;

	// 将源串每8个字节分为一组，压缩成7个字节
	// 循环该处理过程，直至源串被处理完
	// 如果分组不到8字节，也能正确处理
	while(nSrc<nSrcLength)
	{
		// 取源字符串的计数值的最低3位
		nChar = nSrc & 7;     //有何作用？已解决。

		// 处理源串的每个字节
		if(nChar == 0)
		{
			// 组内第一个字节，只是保存起来，待处理下一个字节时使用
			nLeft = *pSrc;
		} else {
			// 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			// 将该字节剩下的左边部分，作为残余数据保存起来
			nLeft = *pSrc >> nChar;

			// 修改目标串的指针和计数值
			pDst++;
			nDst++;
		}

		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}

	// 返回目标串长度
	return nDst;
}

// 7bit解码
// pSrc: 源编码串指针
// pDst: 目标字符串指针
// nSrcLength: 源编码串长度
// 返回: 目标字符串长度
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		// 源字符串的计数值
	int nDst;		// 目标解码串的计数值
	int nByte;		// 当前正在处理的组内字节的序号，范围是0-6
	unsigned char nLeft;	// 上一字节残余的数据

	// 计数值初始化
	nSrc = 0;
	nDst = 0;
	
	// 组内字节序号和残余数据初始化
	nByte = 0;
	nLeft = 0;

	// 将源数据每7个字节分为一组，解压缩成8个字节
	// 循环该处理过程，直至源数据被处理完
	// 如果分组不到7字节，也能正确处理
	while(nSrc<nSrcLength)
	{
		// 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		// 将该字节剩下的左边部分，作为残余数据保存起来
		nLeft = *pSrc >> (7-nByte);

		// 修改目标串的指针和计数值
		pDst++;
		nDst++;

		// 修改字节计数值
		nByte++;

		// 到了一组的最后一个字节
		if(nByte == 7)
		{
			// 额外得到一个目标解码字节
			*pDst = nLeft;

			// 修改目标串的指针和计数值
			pDst++;
			nDst++;

			// 组内字节序号和残余数据初始化
			nByte = 0;
			nLeft = 0;
		}

		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标串长度
	return nDst;
}


// 8bit编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回: 目标编码串长度
int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	// 简单复制
	memcpy(pDst, pSrc, nSrcLength);

	return nSrcLength;
}

// 8bit解码
// pSrc: 源编码串指针
// pDst: 目标字符串指针
// nSrcLength: 源编码串长度
// 返回: 目标字符串长度
int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	// 简单复制
	memcpy(pDst, pSrc, nSrcLength);

	// 输出字符串加个结束符
	*pDst = '\0';

	return nSrcLength;
}

// UCS2编码
// pSrc: 源字符串指针
// pDst: 目标编码串指针
// nSrcLength: 源字符串长度
// 返回: 目标编码串长度
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE宽字符数目
	wchar_t wchar[128];	// UNICODE串缓冲区
	int i = 0;
	// 字符串-->UNICODE串
	setlocale(LC_ALL, "zh_CN.utf8");  //此处非常重要，否则，会出现致命的错误
	nDstLength = mbstowcs( wchar, pSrc,nSrcLength);//如果转换不成功，会返回-1

	// 高低字节分离，输出
	for(i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		// 先输出高位字节
		*pDst++ = wchar[i] & 0xff;		// 后输出低位字节
	}
	// 返回目标编码串长度
	return nDstLength * 2;
}


// UCS2解码
// pSrc: 源编码串指针
// pDst: 目标字符串指针
// nSrcLength: 源编码串长度
// 返回: 目标字符串长度
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE宽字符数目
	wchar_t wchar[128];	// UNICODE串缓冲区
	int i = 0;
	// 高低字节分离，拼成UNICODE
	wchar_t *tempchar;
	//printf( "In gsmDecodeUcs2, nSrcLength:[%d]\n\n", nSrcLength );
	tempchar = wchar;  
	for( i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	// 先高位字节
		wchar[i] |= *pSrc++;		// 后低位字节
		//printf( "wchar[%d] = [%d]\n\n", i, wchar[i] );
	}
	// UNICODE串-->字符串
	setlocale(LC_ALL,"zh_CN.utf8");
    //setlocale(LC_ALL,"en_US.iso88591");
	nDstLength = wcstombs( pDst, wchar, nSrcLength*2 );
	//nDstLength = mbstowcs( pDst, wchar, nSrcLength*2 );
	setlocale(LC_ALL,"C");
	//printf( "In gsmDecodeUcs2, nDstLength:[%d]\n\n", nDstLength );
	// 输出字符串加个结束符
	pDst[nDstLength] = '\0';
	// 返回目标字符串长度
	return nDstLength;
}



// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
// 如："8613851872468" --> "683158812764F8"
// pSrc: 源字符串指针
// pDst: 目标字符串指针
// nSrcLength: 源字符串长度
// 返回: 目标字符串长度
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// 目标字符串长度
	char ch;			// 用于保存一个字符

	// 复制串长度
	nDstLength = nSrcLength;
	int i = 0;
// 每两两字符进行颠倒对调变换
	for( i=0; i<nSrcLength;i+=2)      //i的目的是保证循环的次数
	{
		ch = *pSrc++;		// 保存先出现的字符   ++++++++++++++++++++++++
		*pDst++ = *pSrc++;	// 复制后出现的字符   +++  注意“++”运算  +++
		*pDst++ = ch;		// 复制先出现的字符   ++++++++++++++++++++++++
	}

	// 源串长度是奇数吗？
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';	// 补'F'
		nDstLength++;		// 目标串长度加1
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标字符串长度
	return nDstLength;
}

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"
// pSrc: 源字符串指针
// pDst: 目标字符串指针
// nSrcLength: 源字符串长度
// 返回: 目标字符串长度
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// 目标字符串长度
	char ch;			// 用于保存一个字符
	char *temp;
	temp = pDst;
// 复制串长度
	nDstLength = nSrcLength;
	int i = 0;
	// 两两颠倒,使之恢复原来的正常顺序
	for( i=0; i<nSrcLength;i+=2)
	{
		
		ch = *pSrc++;		// 保存先出现的字符
		*pDst++ = *pSrc++;	// 复制后出现的字符
		*pDst++ = ch;		// 复制先出现的字符
		
	}
// 最后的字符是'F'吗？
	if(*(pDst-1) == 'F')
	{
		pDst--;             //pDst此时所在的位置是整个源字符串长度加1所在的位置
		nDstLength--;		// 目标字符串长度减1
	}

	// 输出字符串加个结束符
	*pDst = '\0';
	// 返回目标字符串长度
	return nDstLength;
}

// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
	int nLength;			// 内部用的串长度
	int nDstLength;			// 目标PDU串长度
	unsigned char buf[1024];	// 内部用的缓冲区
	// SMSC地址信息段
	// SMSC地址字符串的长度	
	nLength = strlen(pSrc->SCA);	
	// SMSC地址信息长度,加1是因为要算上91为一位
	buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;
	// 固定: 用国际格式号码	
	buf[1] = 0x91;			
	// 转换2个字节到目标PDU串，得到的是字符串
	nDstLength = gsmBytes2String(buf, pDst, 2);	
	// 转换SMSC号码到目标PDU串
	// TPDU段基本参数、目标地址等
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);	
	nLength = strlen(pSrc->TPA);	// TP-DA地址字符串的长度
	buf[0] = 0x11;					// 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
	buf[1] = 0;						// TP-MR=0
	buf[2] = (char)nLength;			// 目标地址数字个数(TP-DA地址字符串真实长度)
	buf[3] = 0x91;					// 固定: 用国际格式号码
	// 转换4个字节到目标PDU串
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);		
	// 转换TP-DA到目标PDU串
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);	
	// TPDU段协议标识、编码方式、用户信息等
	nLength = strlen(pSrc->TP_UD);	// 用户信息字符串的长度
	buf[0] = pSrc->TP_PID;			// 协议标识(TP-PID)
	buf[1] = pSrc->TP_DCS;			// 用户信息编码方式(TP-DCS)
	buf[2] = 0;						// 有效期(TP-VP)为5分钟
	if(pSrc->TP_DCS == GSM_7BIT)	
	{
		// 7-bit编码方式
		buf[3] = nLength;			// 编码前长度
		// 转换TP-DA到目标PDU串
		nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;	
	}
	else if(pSrc->TP_DCS == GSM_UCS2)
	{
		// UCS2编码方式
		
		//把短信长度限制在70字以内
		cut_utf8( pSrc->TP_UD, 70 );
		
		// 转换TP-DA到目标PDU串
		buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);		
	//nLength等于该段数据长度，为什么要加4？因为nLength,buf[0],buf[1],buf[2]各占一个字节
		nLength = buf[3] + 4;		
		
	}
	else
	{
		// 8-bit编码方式
		buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);	// 转换TP-DA到目标PDU串
		nLength = buf[3] + 4;		// nLength等于该段数据长度
	}
	// 转换该段数据到目标PDU串
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);		
     /*只要理解了为什么不用pDst[nDstLength-1]就知道为什么了*/
	//  字符串长度
	return nDstLength;//实际上nDstLength是表示整个pdu串的长度，包括以上所有串
}



//截断utf8字符串
//*Src 为待截取字符串
//where 为截断位置

//UNICODE              	UTF-8 
//00000000 - 0000007F 	0xxxxxxx 
//00000080 - 000007FF 	110xxxxx 10xxxxxx 
//00000800 - 0000FFFF 	1110xxxx 10xxxxxx 10xxxxxx 
//00010000 - 001FFFFF 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
//00200000 - 03FFFFFF 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
//04000000 - 7FFFFFFF 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 

void cut_utf8( char *Src, int where ) {
	int i = 0;
	char raw_int;
	char count = 0;
	char tmp = 0;
	
	while( i < where ) {
		raw_int = *Src;
		count = 0;
		if( !(raw_int & 0x80) ) {
			count = 1;
		}
		
		while( tmp = (raw_int & 0x80) ) {
			count++;
			raw_int <<= 1;
			//tmp = (raw_int & 0x80);
		}
		//printf( "count = %d\n", count );
		Src += count;
		i++;
	}
	*Src = '\0';
}




// PDU解码，用于接收、阅读短消息
// pSrc: 源PDU串指针
// pDst: 目标PDU参数指针
// 返回: 用户信息串长度
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength;			// 目标PDU串长度
	unsigned char tmp;		// 内部用的临时字节变量
	unsigned char buf[256];	// 内部用的缓冲区
	unsigned char str[256];	// 内部用的缓冲区

	// SMSC地址信息段
	gsmString2Bytes(pSrc, &tmp, 2);	// 取长度，2为字符串中的前两位
	tmp = (tmp - 1) * 2;			// SMSC号码串长度，把91所占字节数1减掉
	pSrc += 4;						// 指针后移，忽略了SMSC地址格式
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp);	// 转换SMSC号码到目标PDU串
	pSrc += tmp;								// 指针后移

	// TPDU段基本参数、回复地址等
	gsmString2Bytes(pSrc, &tmp, 2);				// 取基本参数TP_PID
	pSrc += 2;						// 指针后移
	//if(tmp & 0x80)
	{
		// 包含回复地址，取回复地址信息
		gsmString2Bytes(pSrc, &tmp, 2);			// 取长度
		if(tmp & 1) tmp += 1;		// 调整奇偶性
		pSrc += 4;				// 指针后移，忽略了回复地址(TP-RA)格式，忽略掉91和86
		gsmSerializeNumbers(pSrc, pDst->TPA, tmp);	// 取TP-RA号码
		pSrc += tmp;				// 指针后移
	}
// TPDU段协议标识、编码方式、用户信息等
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);	// 取协议标识(TP-PID)
	pSrc += 2;		// 指针后移
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);	// 取编码方式(TP-DCS)
	pSrc += 2;		// 指针后移
	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);		// 服务时间戳字符串(TP_SCTS) 
	pSrc += 14;		// 指针后移
	gsmString2Bytes(pSrc, &tmp, 2);	// 用户信息长度(TP-UDL)
	pSrc += 2;		// 指针后移
	//printf( "Content:[%s]\n\n", pSrc );
	if(pDst->TP_DCS == GSM_7BIT)	
	{
		// 7-bit解码
		//printf( "Methoed of decoder:[GSM_7BIT]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// 格式转换
		gsmDecode7bit(buf, pDst->TP_UD, nDstLength);	// 转换到TP-DU
		nDstLength = tmp;
	}
	else if(pDst->TP_DCS == GSM_UCS2)
	{
		//长短信处理
		if( strncmp(pSrc, "050003", 6) == 0 ) {
			//printf( "this is a Multiple SMS\n\n" );
			tmp -= 6;					//减去协议头长度
			pSrc += 11;			
			pDst->TP_UD[0] = '(';	
			pDst->TP_UD[1] = pSrc[0];	//第(?)条
			pDst->TP_UD[2] = ')';
			pDst->TP_UD[3] = '\0';
			pSrc += 1;
		}
		// UCS2解码
		//printf( "Methoed of decoder:[GSM_UCS2]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, str, tmp * 2);			// 格式转换
		//printf( "tmp|nDstLength in GSM_UCS2:[%d|%d]\n\n", tmp, nDstLength );
		nDstLength = gsmDecodeUcs2(str, buf, nDstLength);	// 转换到TP-DU
		//printf( "buf after gsmDecodeUcs2:[%s]\n\n", buf );
		//printf( "strlen of buf after gsmDecodeUcs2:[%d]\n\n", strlen(buf) );
		//printf( "tmp after gsmDecodeUcs2:[%d]\n\n", tmp );
		//strncpy( str, buf, tmp );
		
		//wcstowcs bug 解决办法
		//根据数据长度截取utf8字符串
		cut_utf8( buf, (int)(tmp/2) );		
		
		strcat( pDst->TP_UD, buf );
	}
	else
	{
		// 8-bit解码
		//printf( "Methoed of decoder:[GSM_8BIT]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);			// 格式转换
		nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength);	// 转换到TP-DU
	}

	// 返回目标字符串长度
	return nDstLength;
}

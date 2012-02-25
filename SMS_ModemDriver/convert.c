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

//�Ƚ��ַ������������frontΪ0����Ӻ�ʼ
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


//˵��һ�㣺XXXXX 2 YYYYY��ʾ��XXXXX to YYYYY
// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ������ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ�����ݳ���
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int i = 0;
	for( i = 0; i<nSrcLength; i+=2)      //Ŀ����ȷ���ܹ�ѭ�������ҿ���ѭ������
	{
		// �����4λ
		if(*pSrc>='0' && *pSrc<='9') {
			*pDst = (*pSrc - '0') << 4;
		} else {
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// �����4λ
		if(*pSrc>='0' && *pSrc<='9') {
			*pDst |= *pSrc - '0';
		} else {
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		pDst++;
	}

	// ����Ŀ�����ݳ���
	return nSrcLength / 2;
}

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// pSrc: Դ����ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���ݳ���
// ����: Ŀ���ַ�������
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�
	int i = 0;
	for( i=0; i<nSrcLength; i++) {
		*pDst++ = tab[*pSrc >> 4];		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';
	// ����Ŀ���ַ�������
	return nSrcLength * 2;
}

// 7bit����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����

int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nChar;		// ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	unsigned char nLeft;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	// ѭ���ô�����̣�ֱ��Դ����������
	// ������鲻��8�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength)
	{
		// ȡԴ�ַ����ļ���ֵ�����3λ
		nChar = nSrc & 7;     //�к����ã��ѽ����

		// ����Դ����ÿ���ֽ�
		if(nChar == 0)
		{
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			nLeft = *pSrc;
		} else {
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			nLeft = *pSrc >> nChar;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����Ŀ�괮����
	return nDst;
}

// 7bit����
// pSrc: Դ���봮ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���봮����
// ����: Ŀ���ַ�������
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nByte;		// ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	unsigned char nLeft;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	
	// �����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;

	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength)
	{
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);

		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;

		// �޸��ֽڼ���ֵ
		nByte++;

		// ����һ������һ���ֽ�
		if(nByte == 7)
		{
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;

			// �����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ�괮����
	return nDst;
}


// 8bit����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����
int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	// �򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	return nSrcLength;
}

// 8bit����
// pSrc: Դ���봮ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���봮����
// ����: Ŀ���ַ�������
int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	// �򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	// ����ַ����Ӹ�������
	*pDst = '\0';

	return nSrcLength;
}

// UCS2����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE���ַ���Ŀ
	wchar_t wchar[128];	// UNICODE��������
	int i = 0;
	// �ַ���-->UNICODE��
	setlocale(LC_ALL, "zh_CN.utf8");  //�˴��ǳ���Ҫ�����򣬻���������Ĵ���
	nDstLength = mbstowcs( wchar, pSrc,nSrcLength);//���ת�����ɹ����᷵��-1

	// �ߵ��ֽڷ��룬���
	for(i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		// �������λ�ֽ�
		*pDst++ = wchar[i] & 0xff;		// �������λ�ֽ�
	}
	// ����Ŀ����봮����
	return nDstLength * 2;
}


// UCS2����
// pSrc: Դ���봮ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���봮����
// ����: Ŀ���ַ�������
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE���ַ���Ŀ
	wchar_t wchar[128];	// UNICODE��������
	int i = 0;
	// �ߵ��ֽڷ��룬ƴ��UNICODE
	wchar_t *tempchar;
	//printf( "In gsmDecodeUcs2, nSrcLength:[%d]\n\n", nSrcLength );
	tempchar = wchar;  
	for( i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	// �ȸ�λ�ֽ�
		wchar[i] |= *pSrc++;		// ���λ�ֽ�
		//printf( "wchar[%d] = [%d]\n\n", i, wchar[i] );
	}
	// UNICODE��-->�ַ���
	setlocale(LC_ALL,"zh_CN.utf8");
    //setlocale(LC_ALL,"en_US.iso88591");
	nDstLength = wcstombs( pDst, wchar, nSrcLength*2 );
	//nDstLength = mbstowcs( pDst, wchar, nSrcLength*2 );
	setlocale(LC_ALL,"C");
	//printf( "In gsmDecodeUcs2, nDstLength:[%d]\n\n", nDstLength );
	// ����ַ����Ӹ�������
	pDst[nDstLength] = '\0';
	// ����Ŀ���ַ�������
	return nDstLength;
}



// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�

	// ���ƴ�����
	nDstLength = nSrcLength;
	int i = 0;
// ÿ�����ַ����еߵ��Ե��任
	for( i=0; i<nSrcLength;i+=2)      //i��Ŀ���Ǳ�֤ѭ���Ĵ���
	{
		ch = *pSrc++;		// �����ȳ��ֵ��ַ�   ++++++++++++++++++++++++
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�   +++  ע�⡰++������  +++
		*pDst++ = ch;		// �����ȳ��ֵ��ַ�   ++++++++++++++++++++++++
	}

	// Դ��������������
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';	// ��'F'
		nDstLength++;		// Ŀ�괮���ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ���ַ�������
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�
	char *temp;
	temp = pDst;
// ���ƴ�����
	nDstLength = nSrcLength;
	int i = 0;
	// �����ߵ�,ʹ֮�ָ�ԭ��������˳��
	for( i=0; i<nSrcLength;i+=2)
	{
		
		ch = *pSrc++;		// �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�
		*pDst++ = ch;		// �����ȳ��ֵ��ַ�
		
	}
// �����ַ���'F'��
	if(*(pDst-1) == 'F')
	{
		pDst--;             //pDst��ʱ���ڵ�λ��������Դ�ַ������ȼ�1���ڵ�λ��
		nDstLength--;		// Ŀ���ַ������ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';
	// ����Ŀ���ַ�������
	return nDstLength;
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
	int nLength;			// �ڲ��õĴ�����
	int nDstLength;			// Ŀ��PDU������
	unsigned char buf[1024];	// �ڲ��õĻ�����
	// SMSC��ַ��Ϣ��
	// SMSC��ַ�ַ����ĳ���	
	nLength = strlen(pSrc->SCA);	
	// SMSC��ַ��Ϣ����,��1����ΪҪ����91Ϊһλ
	buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;
	// �̶�: �ù��ʸ�ʽ����	
	buf[1] = 0x91;			
	// ת��2���ֽڵ�Ŀ��PDU�����õ������ַ���
	nDstLength = gsmBytes2String(buf, pDst, 2);	
	// ת��SMSC���뵽Ŀ��PDU��
	// TPDU�λ���������Ŀ���ַ��
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);	
	nLength = strlen(pSrc->TPA);	// TP-DA��ַ�ַ����ĳ���
	buf[0] = 0x11;					// �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	buf[1] = 0;						// TP-MR=0
	buf[2] = (char)nLength;			// Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	buf[3] = 0x91;					// �̶�: �ù��ʸ�ʽ����
	// ת��4���ֽڵ�Ŀ��PDU��
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);		
	// ת��TP-DA��Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);	
	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	nLength = strlen(pSrc->TP_UD);	// �û���Ϣ�ַ����ĳ���
	buf[0] = pSrc->TP_PID;			// Э���ʶ(TP-PID)
	buf[1] = pSrc->TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2] = 0;						// ��Ч��(TP-VP)Ϊ5����
	if(pSrc->TP_DCS == GSM_7BIT)	
	{
		// 7-bit���뷽ʽ
		buf[3] = nLength;			// ����ǰ����
		// ת��TP-DA��Ŀ��PDU��
		nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;	
	}
	else if(pSrc->TP_DCS == GSM_UCS2)
	{
		// UCS2���뷽ʽ
		
		//�Ѷ��ų���������70������
		cut_utf8( pSrc->TP_UD, 70 );
		
		// ת��TP-DA��Ŀ��PDU��
		buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);		
	//nLength���ڸö����ݳ��ȣ�ΪʲôҪ��4����ΪnLength,buf[0],buf[1],buf[2]��ռһ���ֽ�
		nLength = buf[3] + 4;		
		
	}
	else
	{
		// 8-bit���뷽ʽ
		buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);	// ת��TP-DA��Ŀ��PDU��
		nLength = buf[3] + 4;		// nLength���ڸö����ݳ���
	}
	// ת���ö����ݵ�Ŀ��PDU��
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);		
     /*ֻҪ�����Ϊʲô����pDst[nDstLength-1]��֪��Ϊʲô��*/
	//  �ַ�������
	return nDstLength;//ʵ����nDstLength�Ǳ�ʾ����pdu���ĳ��ȣ������������д�
}



//�ض�utf8�ַ���
//*Src Ϊ����ȡ�ַ���
//where Ϊ�ض�λ��

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




// PDU���룬���ڽ��ա��Ķ�����Ϣ
// pSrc: ԴPDU��ָ��
// pDst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength;			// Ŀ��PDU������
	unsigned char tmp;		// �ڲ��õ���ʱ�ֽڱ���
	unsigned char buf[256];	// �ڲ��õĻ�����
	unsigned char str[256];	// �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	gsmString2Bytes(pSrc, &tmp, 2);	// ȡ���ȣ�2Ϊ�ַ����е�ǰ��λ
	tmp = (tmp - 1) * 2;			// SMSC���봮���ȣ���91��ռ�ֽ���1����
	pSrc += 4;						// ָ����ƣ�������SMSC��ַ��ʽ
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp);	// ת��SMSC���뵽Ŀ��PDU��
	pSrc += tmp;								// ָ�����

	// TPDU�λ����������ظ���ַ��
	gsmString2Bytes(pSrc, &tmp, 2);				// ȡ��������TP_PID
	pSrc += 2;						// ָ�����
	//if(tmp & 0x80)
	{
		// �����ظ���ַ��ȡ�ظ���ַ��Ϣ
		gsmString2Bytes(pSrc, &tmp, 2);			// ȡ����
		if(tmp & 1) tmp += 1;		// ������ż��
		pSrc += 4;				// ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ�����Ե�91��86
		gsmSerializeNumbers(pSrc, pDst->TPA, tmp);	// ȡTP-RA����
		pSrc += tmp;				// ָ�����
	}
// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);	// ȡЭ���ʶ(TP-PID)
	pSrc += 2;		// ָ�����
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);	// ȡ���뷽ʽ(TP-DCS)
	pSrc += 2;		// ָ�����
	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);		// ����ʱ����ַ���(TP_SCTS) 
	pSrc += 14;		// ָ�����
	gsmString2Bytes(pSrc, &tmp, 2);	// �û���Ϣ����(TP-UDL)
	pSrc += 2;		// ָ�����
	//printf( "Content:[%s]\n\n", pSrc );
	if(pDst->TP_DCS == GSM_7BIT)	
	{
		// 7-bit����
		//printf( "Methoed of decoder:[GSM_7BIT]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// ��ʽת��
		gsmDecode7bit(buf, pDst->TP_UD, nDstLength);	// ת����TP-DU
		nDstLength = tmp;
	}
	else if(pDst->TP_DCS == GSM_UCS2)
	{
		//�����Ŵ���
		if( strncmp(pSrc, "050003", 6) == 0 ) {
			//printf( "this is a Multiple SMS\n\n" );
			tmp -= 6;					//��ȥЭ��ͷ����
			pSrc += 11;			
			pDst->TP_UD[0] = '(';	
			pDst->TP_UD[1] = pSrc[0];	//��(?)��
			pDst->TP_UD[2] = ')';
			pDst->TP_UD[3] = '\0';
			pSrc += 1;
		}
		// UCS2����
		//printf( "Methoed of decoder:[GSM_UCS2]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, str, tmp * 2);			// ��ʽת��
		//printf( "tmp|nDstLength in GSM_UCS2:[%d|%d]\n\n", tmp, nDstLength );
		nDstLength = gsmDecodeUcs2(str, buf, nDstLength);	// ת����TP-DU
		//printf( "buf after gsmDecodeUcs2:[%s]\n\n", buf );
		//printf( "strlen of buf after gsmDecodeUcs2:[%d]\n\n", strlen(buf) );
		//printf( "tmp after gsmDecodeUcs2:[%d]\n\n", tmp );
		//strncpy( str, buf, tmp );
		
		//wcstowcs bug ����취
		//�������ݳ��Ƚ�ȡutf8�ַ���
		cut_utf8( buf, (int)(tmp/2) );		
		
		strcat( pDst->TP_UD, buf );
	}
	else
	{
		// 8-bit����
		//printf( "Methoed of decoder:[GSM_8BIT]\n\n" );
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);			// ��ʽת��
		nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength);	// ת����TP-DU
	}

	// ����Ŀ���ַ�������
	return nDstLength;
}

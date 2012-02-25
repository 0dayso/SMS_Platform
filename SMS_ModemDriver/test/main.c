/*
**  Function: 
**  Author: RJZ     Date: 2009-00-00
**  EMAIL:  jiangzhe78@126.com
*/

#include "main.h"



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



//打开串口
//返回串口的fd
int OpenDev(char *Dev)
{
		int	fd = open( Dev, O_RDWR );         
		if (-1 == fd)	
		{			
				perror("Can't Open Serial Port");
				exit(0);		
		}
		else
		{
				return fd;
		}
}


//设置串口的波特率
void set_speed(int fd, int speed)
{
	int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200, 38400,  19200,  9600,  4800,  2400,  1200,  300};
	int   i; 
	int   status; 
	struct termios   Opt;
	tcgetattr(fd, &Opt); 
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) { 
		if  (speed == name_arr[i]) {     
			tcflush(fd, TCIOFLUSH);     
			cfsetispeed(&Opt, speed_arr[i]);  
			cfsetospeed(&Opt, speed_arr[i]);   
			status = tcsetattr(fd, TCSANOW, &Opt);  
			if  (status != 0) {        
				perror("tcsetattr fd failed!\n");  
				exit(0);	  
			}    
			tcflush(fd,TCIOFLUSH);   
		}  
	}
}


//设置串口的校验位
int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
		struct termios options; 
		if  ( tcgetattr( fd,&options)  !=  0) { 
				perror("SetupSerial 1\n");     
				return(FALSE);  
		}
		options.c_cflag &= ~CSIZE; 
		switch (databits) 
		{   
				case 7: options.c_cflag |= CS7; break;
				case 8:	options.c_cflag |= CS8;	break;   
				default: fprintf(stderr,"Unsupported data size\n"); return (FALSE);  
		}							
		switch (parity) 
		{   
				case 'n':
				case 'N': options.c_cflag &= ~PARENB;   
						  options.c_iflag &= ~INPCK; 	break;  
				case 'o':   
				case 'O': options.c_cflag |= (PARODD | PARENB);  
						  options.c_iflag |= INPCK; break;  
				case 'e':  
				case 'E': options.c_cflag |= PARENB;      
						  options.c_cflag &= ~PARODD;    
						  options.c_iflag |= INPCK;  break;
				case 'S': 
				case 's': options.c_cflag &= ~PARENB;
						  options.c_cflag &= ~CSTOPB;break;  
				default: fprintf(stderr,"Unsupported parity\n");    
						 return (FALSE);  
		}  
		switch (stopbits)
		{   
				case 1: options.c_cflag &= ~CSTOPB;	break;  
				case 2:	options.c_cflag |= CSTOPB;  break;
				default: fprintf(stderr,"Unsupported stop bits\n");  
						 return (FALSE); 
		} 
		if (parity != 'n')   
				options.c_iflag |= INPCK; 
		tcflush(fd,TCIFLUSH);
		options.c_cc[VTIME] = 40; 
		options.c_cc[VMIN] = 196;
		if (tcsetattr(fd,TCSANOW,&options) != 0)   
		{ 
				perror("SetupSerial 3\n");   
				return (FALSE);  
		} 
		return (TRUE);  
}





int main()
{	





	SM_PARAM * pSrc = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	char ans[1024];	

#if 1	

	SM_PARAM * pSrc1 = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	char ans1[1024];	
	SM_PARAM * pSrc2 = ( SM_PARAM *  )malloc ( sizeof( SM_PARAM ) );
	char ans2[1024];	
	strcpy( ans1, "0891683108200705F06410A121251025271532680008015032903404238C0500035F020182736167003A8BFB4E0A676576846570636E4FDD5B5857287F1351B2533A00620075006600204E2D002C540C65F665874EF676845F53524D8BFB51994F4D7F6E5411540E79FB30026CE8610F8FD94E2A8BFB51994F4D7F6E548C4F7F75280043680751C60049002F004F5E9365F676848BFB51994F4D7F6E670953EF80FD4E0D540C002C8FD9" );
	strcpy( ans2, "0891683108200705F02410A121251025271532680008015022327392230882736167003A5965" );

	
	gsmDecodePdu( ans1, pSrc1 );
	print_SM_PARAM( pSrc1 );
	gsmDecodePdu( ans2, pSrc2 );
	print_SM_PARAM( pSrc2);
#endif	




#if 0	
	strcpy( pSrc->SCA, "8613800716500" );
	strcpy( pSrc->TPA, "8615926510635" );
	pSrc->TP_PID = 0;
	pSrc->TP_DCS = 8;
	//strcpy( pSrc->TP_SCTS, "10052016531132" );
	strcpy( pSrc->TP_UD, "艳慧:需要注意的是，PDU串的用户信息长度(TP-UDL)，在各种编码方式下意义有所不同。" );
	//pSrc->index = 0;
	gsmEncodePdu( pSrc, ans );
	
	printf( "\nans:[%s]\n\n", ans );
#endif




#if 0
	char str1[30];
	char str2[30];
	char *p1, *p2;
	strcpy( str1, "86125015926510635" );
	strcpy( str2, "8615926510635" );
	printf( "subnstr res:[%d]\n", subnstr(str1, str2, 1, 3) );
	

#endif	




#if 0
	int nLength;	
	int nMsg;			
	char* ptr;			
	char cmd[16];		
	char ans[1024];	
	nMsg = 0;
	ptr = ans;
	
	fd = OpenDev( "/dev/ttyS0" );				//打开串口
	
	set_speed( fd, 9600 );			//设置串口的波特率
	
	if (set_Parity(fd,8,1,'N') == FALSE)	//设置串口的校验位
	{
		printf("Set Parity Error\n");
		exit (0);
	}
	
	
	sprintf( cmd, "AT+CMGF=0\nat+cmgr=1\n" );
	
	int tmp = 0;
	printf("start to write! \n", ptr );
	write( fd, cmd, strlen(cmd) );	
	printf("start to tick! \n", ptr );
	while( tmp<100000005 ) {
		tmp++;
	}
	printf("finish the tick! \n", ptr );
	nLength = read(fd, ans, 1024);
	printf("finish read! \n", ptr );
	
	printf("65 the ptr:[%s] \n", ptr );
	
	while( nLength ) 
	{
		printf( "In while nLength=[%d]\n", nLength );
		//if( (nLength > 0) && (strstr( ans, "+CMS ERROR" ) == NULL) )
		if( nLength > 0 )
		{
			
			printf("the ptr:[%s] \n", ptr );
			
			while((ptr = strstr(ptr, "+CMGR:")) != NULL)
			{
				ptr += 6;		
				ptr = strstr(ptr, "\r\n");	
				ptr += 2;		
				if( strstr( ptr, "OK" ) != NULL ) {
					//*strstr( ptr, "OK" ) = '\0';		//去掉后面的尾巴
				}
		
			}
		}
		if( ptr == NULL )
		{
			printf( "ptr is NULL\n" );
			//break;
		}
		nLength = read( fd, ans, 1024 );
		
		ptr = ans;
	}


#endif













    return  EXIT_SUCCESS;
}






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


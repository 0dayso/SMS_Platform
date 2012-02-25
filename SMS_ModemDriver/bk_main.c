#include "main.h"

//main函数
//对串口进行基本操作
//发送短信
//读取短信
int main(int argc, char **argv)
{
		CONF_NODE *conf_head;
		conf_head = readcfg( CONF_FILE );
		
		char *dev  = SERIAL_DEV;			//初始化串口

	 	pthread_t id1,id2,id3,id4;
		
 		fd = OpenDev(dev);				//打开串口
 		
		set_speed( fd,115200 );			//设置串口的波特率
		
		if (set_Parity(fd,8,1,'N') == FALSE)	//设置串口的校验位
		{
				printf("Set Parity Error\n");
				exit (0);
		}
		
		//互斥量，用于控制发送时，进和出对列
  		pthread_mutex_init (&mutex_send,NULL);
    
		//互斥量，用于控制接收时，进和出对列
  		pthread_mutex_init (&mutex_receive,NULL);
		
		//互斥量，用于控制发送和接受时，对串口的读和写
		pthread_mutex_init (&mutex,NULL);
 		 
		 connect();							//连接数据库
 		 
/*         pthread_create(&id1, NULL, (void*)myThread1, NULL);		//创建线程一*/
	 
		pthread_create(&id2, NULL, (void*)myThread2, NULL);		//创建线程二
	
		 pthread_create(&id3, NULL, (void*)myThread3, NULL);		//创建线程三
		
		pthread_create(&id4, NULL, (void*)myThread4, NULL);		//创建线程四
		
/*        pthread_join(id1, NULL);				//结束线程一*/
	
		pthread_join(id2, NULL);				//结束线程二
	  
		pthread_join(id3, NULL);				//结束线程三
	   
		pthread_join(id4, NULL);				//结束线程四
	  	
		close(fd); 							// 关闭串口 
			
		//mysql_close( &my_connection );		//关闭数据库
		//mysql_library_end();	//
		return TRUE;
}


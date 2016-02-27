#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include "config.h"
#include <iostream>
#include <pthread.h>
#include <string>
#include "in-out.h"

ThreadMutex cout_mutex; // 互斥信号量， 一次只有一个线程调用标准输出 
int cfd; 
char buffer[1024]={0};   

void *Recv(void* para)
{
	int ret_val;
	while(1)
	{
		ret_val = read(cfd,buffer,1024);
		if(ret_val == 0)
		{
			PString("server closed");
			exit(0);
		}
		else if(ret_val == -1)  
		{  
			std::cout << "read data fail !" << std::endl;
			return NULL;
		}
		buffer[ret_val]='\0';   
		//cout_mutex.lock();
		std::cout << "recv data is:" << buffer << std::endl;

	}


	return NULL;  
}

int main()  
{  

	int recbytes;  
	int sin_size;  

	struct sockaddr_in s_add,c_add; 
	std::cout << "Hello,welcome to client !" << std::endl;

	cfd = socket(AF_INET, SOCK_STREAM, 0);  
	if(cfd == -1)  
	{  
		std::cout << "socket fail ! !" << std::endl;
		return -1;  
	}  


	bzero(&s_add,sizeof(struct sockaddr_in));  
	s_add.sin_family=AF_INET;  
	s_add.sin_addr.s_addr= inet_addr(SERVER_ADDR); 
	//s_add.sin_addr.s_addr= inet_addr("192.168.1.116"); 
	s_add.sin_port=htons(SERVER_PORT); 
	std::cout << "server address is:" << inet_ntoa(s_add.sin_addr) << " port:"<< ntohs(s_add.sin_port) << std::endl;
	if(connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)) == -1)  
	{  
		std::cout << "connect fail  !" << std::endl;
		return -1;  
	}  

	else std::cout << "connect to server" << std::endl;

	pthread_t thr_id;
	int ret = pthread_create(&thr_id, NULL,Recv,NULL);
	//pthread_join(thr_id, NULL);

	if(ret != 0)
	{
		printf("thread WaitCommand creation failed \n");
		exit(1);
	}  
	std::string str;
	int ret_val;
	while(1) 
	{
		if(_kbhit())
		{
			std::cin >> str ;
			if(str.compare("exit")==0)
			{
				// goto end;
				exit(0);
			}
			else
			{
				int len = str.size();
				char buff[1024]={0}; 
				memcpy(buff,str.c_str(),len);
				buff[len] = 0;
				PString(buff);

				if(write(cfd,str.c_str(),str.size() + 1) == -1)  
				{  
					PString("write fail!");
					close(cfd);
					exit(-1);
				}  
				

			}

		}	
	}
end:
	close(cfd);
	exit(0);
	// return 0;  



}  

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
#include "in-out.h"


#define BUFFER_SIZE 1024
bool opened = true;
ThreadMutex cout_mutex; // 互斥信号量， 一次只有一个线程调用标准输出 
int sfp; 

void *communicate(void* para)
{
	int cur_socket = *(int*)para,ret_val = 0;
	char buffer[1024]={0};  
	int recbytes; 
	cout_mutex.lock();
	std::cout << "thread_id for socket " << cur_socket << "is:"
		<< pthread_self() << std::endl;
	cout_mutex.unlock();
	// if(write(cur_socket,"hello,welcome to my server \r\n",32) == -1)  
	// {  
	// 	PString("first time write fail!");
	// 	ret_val = -1;

	// } 
	while(1)
	{
		ret_val = recbytes = read(cur_socket,buffer,1024);
		if(ret_val == 0)
		{
			PString("client closed");
			exit(0);
		}
		else if(ret_val == -1)  
		{  
			std::cout << "read data fail !" << std::endl;

		}
		else
		{
			buffer[recbytes]='\0';  
			if(strcmp(buffer,"exit") == 0)
			{
				PString("receice a exit");
				close(cur_socket); 
				return NULL;
			}

			else{
				PString(buffer);
				if(write(cur_socket,buffer,recbytes + 1) == -1)  
				{  
					PString("echo write fail!");
					ret_val = -1;

				} 
			}

		}
		
	}

	return NULL;
}

void *WaitConnect(void* para)
{

	int nfp;
	struct sockaddr_in c_add; 
	socklen_t clilen = sizeof(struct sockaddr_in);  
	while(opened)  
	{  
		nfp = accept(sfp, (struct sockaddr *)(&c_add), &clilen); //第三个参数一定要赋值初始化，否则出现connect成功而accept失败的情况。  
		if(nfp == -1)  
		{ 
			PString("accept fail  !"); 
			return NULL;  
		}  

		cout_mutex.lock();
		std::cout << "get connect from " << inet_ntoa(c_add.sin_addr) << " port:"<< ntohs(c_add.sin_port) << std::endl;
		cout_mutex.unlock();

		pthread_t thr_id;
		int ret = pthread_create(&thr_id, NULL, communicate, (void *)(&nfp));
		if(ret != 0)
		{
			PString("communicate thread creation failed "); 
			exit(1);
		}  
	}
	return NULL;  
}




int main()  
{  


	struct sockaddr_in s_add;  


	std::cout << "Hello,welcome to server !" << std::endl;
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if(sfp == -1){
		std::cout << "socket fail !" << std::endl;
	}

	bzero(&s_add,sizeof(struct sockaddr_in));  
	s_add.sin_family=AF_INET;  
	s_add.sin_addr.s_addr=htonl(INADDR_ANY); /* 这里地址使用全0，即所有 */  
	s_add.sin_port=htons(SERVER_PORT);  


	if(bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)) == -1 )  
	{  
		std::cout << "bind fail  !" << std::endl;
		return -1;  
	}  


	if(listen(sfp,5) == -1)  
	{  
		std::cout << "listen fail  !" << std::endl;
		return -1;  
	}  


	pthread_t thr_id;
	int ret = pthread_create(&thr_id, NULL,WaitConnect,NULL);
	//pthread_join(thr_id, NULL);

	if(ret != 0)
	{
		printf("thread WaitCommand creation failed \n");
		exit(1);
	}  

	while(1) 
	{
		if(_kbhit())
		{
			char c  = getchar();
			if(c == 'q')
			{
				exit(0);
			}

		}	
	}

	close(sfp);
	return 0;  
}  

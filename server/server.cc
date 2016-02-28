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
#include <map>
#include "in-out.h"
#include "packet.h"


#define BUFFER_SIZE 1024
bool opened = true;
ThreadMutex cout_mutex; // 互斥信号量， 一次只有一个线程调用标准输出 
std::map<std::string,std::string> UserList;
int sfp; 

void print_userList()
{
	std::map<std::string, std::string>::iterator it = UserList.begin();
	cout_mutex.lock();
	std::cout << "user list is:" << std::endl;
	for(;it != UserList.end();++ it)
	{
		std::cout << it->first << ":" <<it->second;
	}
	cout_mutex.unlock();
}

RegisterResultType Register(const std::string& name,const std::string& password)
{
	PString(name);
	PString(password);
	std::map<std::string, std::string>::iterator it = UserList.find(name);
	if (it != UserList.end()) {
		return USERNAME_CONFLICT;
	}
	UserList[name] = password;
	return SUCCEED;
}

RegisterResultType Register(const UserInfo& info)
{
	return Register(info.m_name,info.m_password);
}

void *communicate(void* para)
{
	int cur_socket = *(int*)para,ret_val = 0;
	char buffer[1024]={0};  
	int recbytes; 
	cout_mutex.lock();
	std::cout << "thread_id for socket " << cur_socket << "is:"
		<< pthread_self() << std::endl;
	cout_mutex.unlock();
	while(1)
	{
		memset(buffer,0,1024);
		ret_val = recbytes = read(cur_socket,buffer,1024);
		if(ret_val == 0)
		{
			PString("client closed");
			close(cur_socket);
			return NULL;
		}
		else if(ret_val == -1)  
		{  
			std::cout << "read data fail !" << std::endl;

		}
		else
		{
			Packet* pkt_ptr = (Packet*)buffer;
			int type = pkt_ptr->m_type;
			switch(type)
			{
				case REGISTER_PKT:
				{
					PString("receice a REGISTER_PKT");
					UserInfo* user_info = (UserInfo*) (buffer + HEADER_LEN);
					RegisterResultType ret = Register(*user_info);
					RegisterResult resu = {ret};
					Packet pkt(REGISTER__RESULT_PKT,&resu);
					RegisterResult* new_resu = (RegisterResult*)(pkt.m_buff);
					if(SendPkt(cur_socket,pkt) == -1)  
					{  
						exit(-1);
					}  
					break;
				}
				case CLIENT_EXIT_PKT:
				{
					PString("receice a exit pkt");
					close(cur_socket); 
					return NULL;
				}
				default:
					{
						PString(buffer);
						if(write(cur_socket,buffer,recbytes + 1) == -1)  
						{  
							PString("echo write fail!");
							ret_val = -1;

						} 
						break;
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
			return NULL;
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
			std::string command;
			std::cin >> command;
			if(command.compare("exit")==0)
			{
				// goto end;
				exit(0);
			}
			else if(command.compare("pr")==0)
			{
				print_userList();
			}
		}	
	}

	close(sfp);
	return 0;  
}  

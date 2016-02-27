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

int main()  
{  
int cfd; 
int recbytes;  
int sin_size;  
char buffer[1024]={0};   
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
if((recbytes = read(cfd,buffer,1024)) == -1)  
{  
	std::cout << "read data fail !" << std::endl;
    return -1;  
}
 
buffer[recbytes]='\0';   
std::cout << "recv data is:" << buffer << std::endl;
  
getchar();   
close(cfd);
return 0;  
  
  
  
}  

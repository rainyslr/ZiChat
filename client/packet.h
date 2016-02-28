#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <iostream>

#define MAX_NAME_LEN 20
#define MAX_PASSWORD_LEN 20
#define MAX_PKT_LEN 510
#define HEADER_LEN 8

enum PacketType{  
	REGISTER_PKT = 0,
	REGISTER__RESULT_PKT,
	CLIENT_EXIT_PKT,
	ERROR_PKT
};

struct Packet
{
	PacketType m_type;
	unsigned int m_totalLen;
	char m_buff[MAX_PKT_LEN];
	Packet(PacketType type,void* payload);
	Packet(PacketType type);
};


struct UserInfo
{
	char m_name[MAX_NAME_LEN];
	char m_password[MAX_PASSWORD_LEN]; 
	UserInfo(const std::string& name,const std::string& password);
	UserInfo(const char* name_ptr,const char* password_ptr);
};

enum RegisterResultType
{
	SUCCEED,
	USERNAME_CONFLICT
};

struct RegisterResult
{
	RegisterResultType m_resultType;
};

int SendPkt(int soc,const Packet& pkt);
int SendPkt(int soc,const Packet* pkt_ptr);

class Cmd
{
public:
	std::string m_cmd[5];
	Cmd(const std::string& str);
	void Print();
};
#endif
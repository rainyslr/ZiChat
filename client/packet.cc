#include "packet.h"
#include "string.h"
#include "in-out.h"

Packet::Packet(PacketType type,void* payload)
:m_type(type)
{
	switch(type)
	{
		case REGISTER_PKT:
			{
				m_totalLen = HEADER_LEN + sizeof(UserInfo);
				memcpy(m_buff,payload,sizeof(UserInfo));
				break;
			}
		default:
			{
				m_totalLen = HEADER_LEN + 0;
				break;
			}

	}

}

Packet::Packet(PacketType type)
:m_type(type),m_totalLen(HEADER_LEN){}


UserInfo::UserInfo(const std::string& name,const std::string& password)
{
	int name_len,password_len;
	name_len = name.size() < (MAX_NAME_LEN - 1)?(name.size() + 1):MAX_NAME_LEN;
	memcpy(m_name,name.c_str(),name_len);
	password_len = password.size() < (MAX_PASSWORD_LEN - 1)?(password.size() + 1):MAX_PASSWORD_LEN;
	memcpy(m_password,password.c_str(),name.size() + 1);
}

UserInfo::UserInfo(const char* name_ptr,const char* password_ptr)
{
	memcpy(m_name,name_ptr,MAX_NAME_LEN);
	memcpy(m_password,password_ptr,MAX_PASSWORD_LEN);
}

int SendPkt(int soc,const Packet& pkt)
{
	int ret = write(soc,&pkt,pkt.m_totalLen);
	if(ret == -1)
	{
		PString("write fail!");
		close(soc);
	}
	return ret;
}

int SendPkt(int soc,const Packet* pkt_ptr)
{
	int ret = write(soc,pkt_ptr,pkt_ptr->m_totalLen);
	if(ret == -1)
	{
		PString("write fail!");
		close(soc);
	}
	return ret;
}

Cmd::Cmd(const std::string& str)
{
	char data[100],temp[20];
	int len = str.size() + 1;
	memcpy(data,str.c_str(),len);
	int pos = 0,previous = 0;
	int data_index = 0;
	for(int i = 0;i < len && i < 100; ++ i)
	{
		if(data[i] == ':')
		{
			memset(temp,0,20);
			int cmd_len = i - previous;
			memcpy(temp,data+ previous,cmd_len);
			temp[cmd_len] = 0;
			m_cmd[data_index] = std::string(temp);
			++ data_index;
			previous = i + 1;
		}
	}
}

void Cmd::Print()
{
	for(int i = 0;i < 5;++ i)
	{
		if(m_cmd[i].size() == 0)
			break;
		PString(m_cmd[i]);
	}
}

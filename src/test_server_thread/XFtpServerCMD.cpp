#include "XFtpServerCMD.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <thread>
using namespace std;

using namespace std;
bool XFtpServerCMD::Init()
{
	//创建每个socket的bufferevent
	bev = bufferevent_socket_new(base, sock, BEV_OPT_THREADSAFE| BEV_OPT_CLOSE_ON_FREE);
	if (!bev)return false;
	SetCallback(bev);

	//发送数据 写入到输出缓冲
	Send("220  Welcome to XFtpServer (libevent)\r\n");
	return true;
}
void XFtpServerCMD::Read(struct bufferevent *bev)
{
	char data[1024] = { 0 };
	for (;;)
	{
		//读取输入缓冲数据
		int len = bufferevent_read(bev, data, sizeof(data) - 1);
		if (len <= 0) return;
		cout << "RECV:" << data << endl;
		string msg = data;
		string sendmsg = "200 OK\r\n";

		//USER root
		//PWD\r\n
		string type = ""; 
		int i = 0;
		for ( ;i < msg.size(); i++)
		{
			if (msg[i] == ' '|| msg[i] == '\r')
				break;
		}
		if (i == 0)
			return;
		type = msg.substr(0, i);

	
		cout << "type = " << type << endl;

		if (calls.find(type) != calls.end())
		{
			XFtpTask *t = calls[type];
			t->base = base;
			t->port = port;
			t->ip = ip;
			t->cmdTask = this;
			t->Parse(type,msg);
			if (type == "PORT")
			{
				ip = t->ip;
				port = t->port;
			}
		}
		else
		{
			cout << "SEND:" << sendmsg << endl;
			Send(sendmsg);
			//bufferevent_write(bev, sendmsg.c_str(), sendmsg.size());
		}
	}

}
void XFtpServerCMD::Event(struct bufferevent *be, short what)
{
	if ((what & BEV_EVENT_ERROR) || (what & BEV_EVENT_EOF))
	{
		cout << "BEV_EVENT_ERROR OR BEV_EVENT_EOF " << endl;
		//Close();
		delete this;
	}
} 

void XFtpServerCMD::Reg(std::string cmd, XFtpTask *call)
{
	if (!call)
	{
		cout << "Reg XFtpCMDCall call is null" << endl;
		return;
	}
	if (calls.find(cmd) != calls.end())
	{
		cout << cmd << " is alreay register" << endl;
		return;
	}

	calls[cmd] = call;
	//防止重复
	callstmp[call] = 0;
	
}


XFtpServerCMD::XFtpServerCMD()
{
}


XFtpServerCMD::~XFtpServerCMD()
{
	cout << "~XFtpServerCMD" << endl;
	Close();
	for (auto ptr = callstmp.begin(); ptr != callstmp.end();ptr++)
	{
		delete ptr->first;
	}
	calls.clear();
}

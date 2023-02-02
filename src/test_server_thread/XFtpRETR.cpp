#include "XFtpRETR.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
using namespace std;


void XFtpRETR::Write(struct bufferevent *be)
{
	if (!cmdTask)return;
	if (fp)
	{
		
		if (feof(fp) != 0)
		{
			//有可能进不来，客户端先收全之后关闭了连接
			fclose(fp);
			fp = 0;
			//string msg = "226 Transfer complete\r\n";
			//bufferevent_write(cmdbev, sendmsg.c_str(), sendmsg.size());
			cmdTask->Send("226 Transfer complete\r\n");
			Close();
			return;
		}
		int len = fread(buf, 1, sizeof(buf), fp);
		if (len <= 0)
			return;
		cout << "[" << len<<"]" << flush;
		//bufferevent_write(portbev, buf, len);
		Send(buf, len);
		return;
	}
}


void XFtpRETR::Event(struct bufferevent *bev, short what)
{
	if ((what & BEV_EVENT_ERROR) || (what & BEV_EVENT_EOF))
	{
		cout << "XFtpLIST BEV_EVENT_ERROR OR BEV_EVENT_EOF " << endl;
		if (fp)
		{
			cmdTask->Send("226 Transfer complete\r\n");
			fclose(fp);
			fp = 0;
		}
		cout << "XFtpRETR Event call" << endl;
		Close();
	}
}

void XFtpRETR::Parse(std::string type, std::string msg)
{/*
	string sendmsg = "150　Start send file.\r\n";
	bufferevent_write(cmdbev, sendmsg.c_str(), sendmsg.size());*/

	if (type == "RETR")
	{
		string path = cmdTask->rootDir;
		path += cmdTask->curDir;

		int pos = msg.rfind(" ") + 1;
		path += msg.substr(pos, msg.size() - pos - 2);
		fp = fopen(path.c_str(), "rb");
		if (fp)
		{
			Connect();
			cmdTask->Send("150 file OK.\r\n");
			bufferevent_trigger(bev, EV_WRITE, 0);
		}
		else
		{
			string msg = "450　file open failed.";
			msg += path;
			msg += "\r\n";
			cmdTask->Send(msg);
		}
	}
	
		
}
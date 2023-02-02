#include "XFtpSTOR.h"

#include "XFtpRETR.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
using namespace std;


void XFtpSTOR::Read(struct bufferevent *be)
{
	cout << "XFtpSTOR::Read" << endl;
	if (fp)
	{
		char data[1024] = { 0 };
		for (;;)
		{
			int len = bufferevent_read(be, data, sizeof(data));
			if (len <= 0)return;

			int size = fwrite(data, 1, len, fp);

			//cout << "RECV SIZE " << size << endl;
			cout << "<" << len << ">" << flush;
		}
		//bufferevent_write(portbev, buf, len);
		//portTask->Send(buf, len);
		return;
	}
}
void XFtpSTOR::Event(struct bufferevent *bev, short what)
{
	if ((what & BEV_EVENT_ERROR) || (what & BEV_EVENT_EOF))
	{
		cout << "XFtpLIST BEV_EVENT_ERROR OR BEV_EVENT_EOF " << endl;
		if (fp)
		{
			fclose(fp);
			fp = 0;
		}
		cmdTask->Send("226 Transfer complete\r\n");
		cout << "XFtpRETR Event call" << endl;
		Close();
	}
}

void XFtpSTOR::Parse(std::string type, std::string msg)
{
	//if (!portTask->bev)return;
	type = type;
	string path = cmdTask->rootDir;
	path += cmdTask->curDir;

	int pos = msg.rfind(" ") + 1;
	path += msg.substr(pos, msg.size() - pos - 2);
	fp = fopen(path.c_str(), "wb");
	if (fp)
	{
		////设置回调函数
		//设置接收、状态改变 回调函数
		cmdTask->Send("125 file OK.\r\n");

		Connect();
		
		bufferevent_trigger(bev, EV_READ, 0);
	}
	else
	{
		string msg = "450　file open failed.";
		msg += path;
		msg += "\r\n";
		cmdTask->Send(msg);
	}

}


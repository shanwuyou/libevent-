#include "XFtpPORTTask.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

void XFtpPORTTask::Parse(std::string type, std::string msg)
{
	if (type == "PORT")
	{
		//PORT 127,0,0,1,70,96
		vector<string> vals;
		string tmp = "";
		for (int i = 4; i < (int)msg.size(); i++)
		{
			if (msg[i] == ',' || i == msg.size() - 1)
			{
				vals.push_back(tmp.c_str());
				tmp = "";
				continue;
			}
			if (msg[i] == ' ')
				continue;
			tmp += msg[i];
		}
		//port = n5*256 + n6
		port = atoi(vals[4].c_str()) * 256 + atoi(vals[5].c_str());
		cout << " port is " << port;
		ip = vals[0];
		ip += ".";
		ip += vals[1];
		ip += ".";
		ip += vals[2];
		ip += ".";
		ip += vals[3];
		cout << " ip is " << ip << endl;
		cout << "XFTPPort Init call" << endl;
		string sendmsg = "200 PORT command successful.\r\n";
		cout << "Send PORT:" << sendmsg << endl;
		cmdTask->Send(sendmsg);
		//Init();
	}
}

XFtpPORTTask::~XFtpPORTTask()
{
	//cout << "~XFTPPort  call" << endl;
	//cout << "delete ~XFtpPORTTask "<<this->thread_id << endl;
}

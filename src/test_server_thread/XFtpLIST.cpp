#include "XFtpLIST.h"
#include <iostream>
#include <event2/bufferevent.h>
#ifdef _WIN32
#include <io.h>
#endif

using namespace std;


void XFtpLIST::Write(struct bufferevent *be)
{
	if (!cmdTask)return;
	//cout << "XFTPPort::Write" << endl;
	string msg = "226 Transfer complete\r\n";
	//bufferevent_write(cmdbev, sendmsg.c_str(), sendmsg.size());
	cmdTask->Send(msg);

	cout << "XFtpLIST Write call" << endl;
	Close();
	//bufferevent_free(portbev);
	//bufferevent_free(portbev);
}

void XFtpLIST::Event(struct bufferevent *bev, short what)
{
	if ((what & BEV_EVENT_ERROR) || (what & BEV_EVENT_EOF))
	{
		Close();
		if (!cmdTask)return;
		cout << "XFtpLIST BEV_EVENT_ERROR OR BEV_EVENT_EOF " << endl;
		cout << "XFtpLIST Event call" << endl;
		
	}
	if ((what & BEV_EVENT_CONNECTED))
	{
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
	}
}

void XFtpLIST::Parse(std::string type, std::string msg)
{
	if (!cmdTask)return;

	string resmsg = "";
	
	if (type == "LIST") 
	{
		Connect();
		cmdTask->Send("150　Here comes the directory listing.\r\n");
		string path = cmdTask->rootDir + cmdTask->curDir;
		string listdata = GetListData(path);
		Send(listdata);
		return;
	}
	else if (type == "CDUP")
	{
		string path = cmdTask->curDir;

		if (path[path.size() - 1] == '/')
		{
			path = path.substr(0, path.size() - 1);
		}
		int pos = path.rfind("/");
		path = path.substr(0, pos);
		if (path.size() == 0)
			path = "/";
		cmdTask->curDir = path;
		resmsg = "250 Directory success changed.";
	}
	else if (type == "CWD")
	{
		//230 Login successful
		resmsg = "250 Directory success changed.";
		string path = "";
		int pos = msg.rfind(" ")+1;
		int size = msg.size() - pos - 2;
		path = msg.substr(pos, msg.size()- pos-2);
		if(path[path.size()-1] != '/')
			path += "/";
		if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
			cmdTask->curDir += "/";
		cmdTask->curDir += path;
	}
	else if (type == "PWD")
	{
		resmsg = "257 \"";
		if (cmdTask->curDir.empty())
		{
			cmdTask->curDir = "/";
		}
		if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
			cmdTask->curDir += "/";

		if (cmdTask->curDir[0] == '.')
		{
			resmsg += cmdTask->curDir.substr(1, cmdTask->curDir.size()-1);
		}
		else
		{
			resmsg += cmdTask->curDir;
		}


		//resmsg += "/";
		resmsg += "\" is current directory.";
	}

	resmsg += "\r\n";
	//bufferevent_write(cmdbev, resmsg.c_str(), resmsg.size());
	cmdTask->Send(resmsg);
	//CWD libevent_lesson2
}

std::string XFtpLIST::GetListData(string path)
{
	string data = "";
	
	//return "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	#ifdef _WIN32
	_finddata_t file;
	intptr_t dir;
	path += "/*.*";
	dir = _findfirst(path.c_str(), &file);
	if (dir < 0)
		return data;
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	do {
		//cout << file.name << " " << endl;
		char buf[1024] = { 0 };
		//判断是否有子目录  
		if (file.attrib & _A_SUBDIR)
		{
			//判断是否为"."当前目录，".."上一层目录
			if ((strcmp(file.name, ".") == 0) && (strcmp(file.name, "..") == 0))
				continue;
			data += "drwxrwxrwx";
		}
		else
		{
			data += "-rwxrwxrwx";
		}
		data += " 1 root group ";
		sprintf(buf, "%u ", file.size);
		data += buf;
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%b %m %H:%M ", localtime(&file.time_write));
		data += tmp;
		data += file.name;
		data += "\r\n";
	} while (_findnext(dir, &file) == 0);
	_findclose(dir);
	
	
	#else
		string cmd = "ls -l ";
		cmd += path;
		cout <<"popen:"<< cmd << endl;
		FILE *fp = popen(cmd.c_str(),"r");
		if (!fp)
		{
			return data;
		}
		char buf[1024] = {0};
		for(;;)
		{
			int len = fread(buf,1,sizeof(buf)-1,fp);
			if(len <=0)break;
			buf[len] = '\0';
			data += buf;
		}
		pclose(fp);
	#endif 
	//string data = "-rwxrw-r-- 1 user group             0 Nov 14:57 cwinvnc337.ESn\r\n";
	//data = "drwxr-xr-x 1 root group 4096 Mar 14 09:53 libevent_lesson2\r\n";
	//for (int i = 0; i < 1000; i++)
	//{
	//	char buf[1024] = { 0 };
	//	sprintf(buf, "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 10%d.jpg\r\n", i);
	//	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 ";
	//	//data += "101.jpg\r\n";
	//	data += buf;
	//}
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 102.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 103.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 104.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 105.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 106.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 107.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 108.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 109.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 110.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	//data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	return data;
}
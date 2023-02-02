//#include <event2/event.h>
//#include <event2/listener.h>
#include <string.h>
#include "XServerMain.h"
#include <event2/thread.h>
#include <event2/bufferevent.h>
#ifndef _WIN32
#include <signal.h>
#else
#include <windows.h>
#endif
#include <iostream>
#include <string.h>
#include <string>
#include <time.h>
using namespace std;
//#define SPORT 5001
//void listen_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg)
//{
//	cout << "listen_cb" << endl;
//}


int main()
{
	//int a = BEV_EVENT_WRITING &(BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT | BEV_EVENT_EOF);
	//char s[10];
	//_itoa(a, s, 2);
	////if(a)
	//	cout << s << endl;
	//getchar();
	//string data = "";
	//_finddata_t file;
	//intptr_t dir;
	//dir = _findfirst("./*.*", &file);
	////data += "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
	//do {
	//	cout << file.name << " " << endl;
	//	char buf[1024] = { 0 };
	//	//判断是否有子目录  
	//	if (file.attrib & _A_SUBDIR)
	//	{
	//		//判断是否为"."当前目录，".."上一层目录
	//		if ((strcmp(file.name, ".") == 0) && (strcmp(file.name, "..") == 0))
	//			continue;
	//		data += "drwxrwxrwx";
	//	}
	//	else
	//	{
	//		data += "-rwxrwxrwx";
	//	}
	//	data += " 1 root group ";
	//	sprintf(buf, "%u ", file.size);
	//	data += buf;
	//	char tmp[64];
	//	strftime(tmp, sizeof(tmp), "%b %m %H:%M ", localtime(&file.time_write));
	//	data += tmp;
	//	data += file.name;
	//	data += "\r\n";
	//} while (_findnext(dir, &file) == 0);
	//_findclose(dir);
	//cout << data << endl;
	//getchar();


#ifdef _WIN32 
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
#else
	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif
#ifdef WIN32

	//多线程中调用必须处理化，不然会失败
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif

	XServerMain::Get()->Main();


 //   std::cout << "test server!\n"; 
	////创建libevent的上下文
	//event_base * base = event_base_new();
	//if (base)
	//{
	//	cout << "event_base_new success!" << endl;
	//}
	////监听端口
	////socket ，bind，listen 绑定事件
	//sockaddr_in sin;
	//memset(&sin, 0, sizeof(sin));
	//sin.sin_family = AF_INET;
	//sin.sin_port = htons(SPORT);

	//evconnlistener *ev = evconnlistener_new_bind(base,	// libevent的上下文
	//	listen_cb,					//接收到连接的回调函数
	//	base,						//回调函数获取的参数 arg
	//	LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,//地址重用，evconnlistener关闭同时关闭socket
	//	10,							//连接队列大小，对应listen函数
	//	(sockaddr*)&sin,							//绑定的地址和端口
	//	sizeof(sin)
	//	);
	////事件分发处理
	//if(base)
	//	event_base_dispatch(base);
	//if(ev)
	//	evconnlistener_free(ev);
	//if(base)
	//	event_base_free(base);
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

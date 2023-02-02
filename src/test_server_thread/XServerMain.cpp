#include "XServerMain.h"
#include "XThread.h"
#include<thread>
#include <iostream>
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "XThreadPool.h"
#include "XFtpFactory.h"
#include "XTask.h"
#define THREADCOUNT 10
#define SPORT 21
//����Ҫ��ֱ����main����������

using namespace std;
static void server_main_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg)
{
	XServerMain *t = (XServerMain *)arg;
	t->ListenCB(s, a, socklen);
}


void XServerMain::ListenCB(int sock, struct sockaddr *a, int sockle)
{
	cout << "ListenCB" << endl;
	XTask *task = XFtpFactory::Get()->CreateTask();
	task->sock = sock;
	XThreadPool::Get()->Dispatch(task);

}
void XServerMain::Main()
{
	XThreadPool::Get()->Init(THREADCOUNT);
	//InitThreads(THREADCOUNT);
	struct event_config *ev_config;
	ev_config = event_config_new();
	event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
	event_base * base = event_base_new_with_config(ev_config);
	event_config_free(ev_config);
	if (!base)
	{
		cerr << "XServerMain Init failed" << endl;
		return ;
	}
	//socket ��bind��listen ���¼�
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);

	evconnlistener *ev = evconnlistener_new_bind(base,	// libevent��������
		server_main_cb,					//���յ����ӵĻص�����
		this,						//�ص�������ȡ�Ĳ��� arg
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,//��ַ���ã�evconnlistener�ر�ͬʱ�ر�socket
		10,							//���Ӷ��д�С����Ӧlisten����
		(sockaddr*)&sin,							//�󶨵ĵ�ַ�Ͷ˿�
		sizeof(sin)
	);
	if (!ev)
	{
		cerr << "evconnlistener_new_bind port " << SPORT << " failed��" << endl;
		return;
	}
	
	//�¼��ַ�����
	cout << "XServerMain::Main() loop begin listen port "<<SPORT<< endl;
	event_base_dispatch(base);
	event_base_free(base);
	cout << "XServerMain::Main() loop end" << endl;
}
XServerMain::XServerMain()
{
}


XServerMain::~XServerMain()
{
}

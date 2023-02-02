#include "XFtpTask.h"

#include "XFtpTask.h"
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <iostream>
using namespace std;

void XFtpTask::Connect(XFtpTask *t)
{
	if (!t) t = this;
	Close();
	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	evutil_inet_pton(AF_INET, ip.c_str(), &sin.sin_addr.s_addr);
	//设置接收、状态改变 回调函数
	SetCallback(bev);
	int re = bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
	if (re == 0)
	{
		cout << "connected" << endl;
	}
	return;
}
void XFtpTask::Close()
{
	if (bev)
	{
		bufferevent_free(bev);
		bev = 0;
	}
}
int XFtpTask::Send(const char *data, int size)
{
	if (!bev)return -1;
	return bufferevent_write(bev, data, size);
}
int XFtpTask::Send(std::string str)
{
	return Send(str.c_str(), str.size());
}

void XFtpTask::SetCallback(struct bufferevent * bev, XFtpTask *t)
{
	if (!bev)return;
	if (!t)
		t = this;
	//设置接收、状态改变 回调函数
	bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, t);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}
void XFtpTask::ReadCB(struct bufferevent *bev, void *arg)
{
	XFtpTask *task = (XFtpTask *)arg;
	task->Read(bev);
}
void XFtpTask::WriteCB(struct bufferevent *bev, void *arg)
{
	XFtpTask *task = (XFtpTask *)arg;
	task->Write(bev);
}
void XFtpTask::EventCB(struct bufferevent *bev, short what, void *arg)
{
	XFtpTask *task = (XFtpTask *)arg;
	task->Event(bev, what);
}



#include "XThread.h"
#include <iostream>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "XTask.h"
#include <string.h>
#include <thread>
#ifdef _WIN32
#else
#include <unistd.h>
#endif
using namespace std;
static void NotifyCB(evutil_socket_t fd, short which, void *arg)
{
	XThread *t = (XThread *)arg;
	t->Notify(fd);
}

void XThread::Activate()
{
	//激活读线程的读事件
#ifdef _WIN32
	int len = send(notfiy_send_fd, "c", 1, 0);
#else
	int len = write(notfiy_send_fd, "c", 1);
#endif
}

void XThread::Notify(evutil_socket_t fd)
{
	//首先将socketpair的1个字节通知信号读出（这是必须的，在水平触发模式下如果不处理该事件，则会循环通知，直到事件被处理）
	char  buf[2] = {0};
	
#ifdef _WIN32
	recv(fd, buf, 1, 0);//从sockpair的另一端读数据
#else
	read(fd, buf, 1);//从sockpair的另一端读数据
#endif
	cout <<id<<" thread "<< buf << endl;
	//取出一个任务
	XTask *task = GetTask();
	if (!task)
	{
		cout << "GetTask is empty" << endl;
		return;
	}
	task->Init();

}

bool XThread::Setup()
{
#ifdef _WIN32
	//创建一个socketpair即可与互相通信的两个socket，保存在fds里面
	evutil_socket_t fds[2];
	if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0)
	{
		std::cout << "创建socketpair失败\n";
		return false;
	}
	//设置成无阻赛的socket
	evutil_make_socket_nonblocking(fds[0]);
	evutil_make_socket_nonblocking(fds[1]);
#else
	int fds[2];
	if (pipe(fds))
	{
		perror("Can't create notify pipe");
		exit(1);
	}
#endif
	notfiy_send_fd = fds[1];

	// 创建线程的event_base
	//创建libevent的上下文
	struct event_config *ev_config;
	ev_config = event_config_new();
	event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
	base = event_base_new_with_config(ev_config);
	event_config_free(ev_config);
	if (!base)
	{
		cout << "thread " << id << " event_base_new_with_config failed!";
		return false;
	}

   //给每个libevent线程设置连接通知回调函数。
	event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
	//边缘触发
	//event *ev = event_new(base,client,EV_READ|EV_PERSIST|EV_ET,client_cb,event_self_cbarg());
	//timeval t = { 10,0 };
	event_add(ev,0);
	return true;
}

XTask *XThread::GetTask()
{
	XTask*t = 0;
	task_mutex.lock();
	if (tasks.empty())
	{
		task_mutex.unlock();
		return 0;
	}
	t = tasks.front();
	tasks.pop_front();
	task_mutex.unlock();
	return t;
}
void  XThread::AddTask(XTask *t)
{
	if (!t)return;
	task_mutex.lock();
	t->base = base;
	tasks.push_back(t);
	task_mutex.unlock();
}
void XThread::Main()
{
	cout << id<<"XThread::Main() begin" << endl;

	event_base_dispatch(base);
		//event_base_loop(base, EVLOOP_NONBLOCK);
	event_base_free(base);
	cout << id << "XThread::Main() end" << endl;
}

void XThread::Start()
{
	//this->id = id;
	thread th(&XThread::Main,this);
	th.detach();
}

XThread::XThread()
{
}


XThread::~XThread()
{
}

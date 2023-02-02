#ifndef XTHREAD_H
#define XTHREAD_H
#include <list>
#include <mutex>
#include <event2/util.h>
#include <event2/event.h>
using namespace std;
struct event_base;
class XTask;
class XThread
{
public:
	int id = 0;
	//确保对象都是在堆中创建
	static XThread* Create(int id)
	{
		XThread*x =  new XThread();
		x->id = id;
		return x;
	}
	virtual ~XThread();
	
	//安装线程，初始化后libevent的事件
	bool Setup();

	//线程函数，libevent的事件循环
	void Main();
	
	//开始线程的运行
	void Start();

	//发出激活消息
	void Activate();
	
	//收到线程池发出的激活消息
	void Notify(evutil_socket_t fd);

	//添加处理任务，一个线程可以同时处理多个任务，他们共用一个event_base
	void AddTask(XTask *t);

	//获取任务，如果没有，返回NULL
	XTask *GetTask();
private:
	XThread();
	event_base * base = 0;
	event *notify_event = 0;
	evutil_socket_t notfiy_send_fd;
	list< XTask*> tasks;
	mutex task_mutex;
};

#endif
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
	//ȷ���������ڶ��д���
	static XThread* Create(int id)
	{
		XThread*x =  new XThread();
		x->id = id;
		return x;
	}
	virtual ~XThread();
	
	//��װ�̣߳���ʼ����libevent���¼�
	bool Setup();

	//�̺߳�����libevent���¼�ѭ��
	void Main();
	
	//��ʼ�̵߳�����
	void Start();

	//����������Ϣ
	void Activate();
	
	//�յ��̳߳ط����ļ�����Ϣ
	void Notify(evutil_socket_t fd);

	//��Ӵ�������һ���߳̿���ͬʱ�������������ǹ���һ��event_base
	void AddTask(XTask *t);

	//��ȡ�������û�У�����NULL
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
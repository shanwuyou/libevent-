#include "XThreadPool.h"
#include "XThread.h"
#include <event2/event.h>
#include <iostream>
#include <thread>
#include "XTask.h"
using namespace std;

//分发线程
bool XThreadPool::Dispatch(XTask *task)
{
	if (!task)return false;
	int tid = (lastThread + 1) % threadCount;
	lastThread = tid;
	XThread *t = threads[lastThread];
	t->AddTask(task);
	t->Activate();
	task->thread_id = t->id;
	return true;

}

bool XThreadPool::Init(int threadCount)
{
	this->threadCount = threadCount;
	this->lastThread = -1;
	for (int i = 0; i < threadCount; i++)
	{
		XThread *t = XThread::Create(i);
		t->Setup();
		t->Start();
		threads.push_back(t);
		this_thread::sleep_for(10ms);
	}
	return true;
}

XThreadPool::XThreadPool()
{
}


XThreadPool::~XThreadPool()
{
}

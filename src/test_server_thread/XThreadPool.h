#pragma once
#include <vector>
class XThread;
class XTask;
class XThreadPool
{
public:
	static XThreadPool*Get()
	{
		static XThreadPool poll;
		return &poll;
	}
	//只能调用一次
	bool Init(int threadCount = 10);

	//分发线程
	bool Dispatch(XTask *task);

	~XThreadPool();
protected:
	int threadCount = 0;
	int lastThread = -1;
	std::vector <XThread*> threads;
	XThreadPool();
};


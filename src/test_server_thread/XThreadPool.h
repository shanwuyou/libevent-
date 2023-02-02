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
	//ֻ�ܵ���һ��
	bool Init(int threadCount = 10);

	//�ַ��߳�
	bool Dispatch(XTask *task);

	~XThreadPool();
protected:
	int threadCount = 0;
	int lastThread = -1;
	std::vector <XThread*> threads;
	XThreadPool();
};


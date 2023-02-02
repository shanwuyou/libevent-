#pragma once
#include <vector>
using namespace std;
class XThread;
struct event_base;
struct sockaddr;
class XServerMain
{
public:
	static XServerMain*Get()
	{
		static XServerMain x;
		return &x;
	}
	XServerMain();
	virtual ~XServerMain();
	void ListenCB(int sock, struct sockaddr *a, int sockle);
	void Main();

};


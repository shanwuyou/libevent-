#pragma once
#include "XTask.h"
class XFtpTask:public XTask
{
public:
	std::string rootDir = ".";
	std::string curDir = "/";
	struct bufferevent *bev = 0;
	XFtpTask *cmdTask = 0;
	std::string ip = "";
	int port = 0;
	virtual int Send(const char *data, int size);
	virtual int Send(std::string str);
	virtual void Parse(std::string type, std::string msg) {};
	virtual void Read(struct bufferevent *bev) {};
	virtual void Write(struct bufferevent *bev) {};
	virtual void Event(struct bufferevent *bev, short what) {};
	virtual void SetCallback(struct bufferevent * bev, XFtpTask *t = 0);
	virtual void Close();
	virtual bool Init() { return true; };
	virtual void Connect(XFtpTask *t = 0);
protected:
	static void ReadCB(struct bufferevent *bev, void *arg);
	static void WriteCB(struct bufferevent *bev, void *arg);
	static void EventCB(struct bufferevent *bev, short what, void *arg);
};


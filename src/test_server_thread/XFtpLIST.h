#pragma once
#include <string>
#include "XFtpTask.h"
class XFtpLIST:public XFtpTask
{
public:
	virtual void Parse(std::string type, std::string msg);
	std::string GetListData(std::string path);
	virtual void Write(struct bufferevent *bev);
	virtual void Event(struct bufferevent *bev, short what);
};


#pragma once
#include "XFtpTask.h"
#include <string>
struct bufferevent;
class XFtpPORTTask :public XFtpTask
{
public:
	~XFtpPORTTask();
	void Parse(std::string type, std::string msg);
protected:
};


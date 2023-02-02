#pragma once
#include "XFtpTask.h"
#include <string>
class XFtpUSER :public XFtpTask
{
public:
	virtual void Parse(std::string type, std::string msg);
};


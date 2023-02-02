#pragma once
#include "XFtpTask.h"
#include <string>
#include <map>

class XFtpServerCMD : public XFtpTask
{
public:
	XFtpServerCMD();
	virtual ~XFtpServerCMD();
	void Reg(std::string cmd, XFtpTask *call);
	virtual bool Init();
	virtual void Read(struct bufferevent *bev);
	virtual void Event(struct bufferevent *bev, short what);
private:
	std::map< std::string, XFtpTask*> calls;
	std::map< XFtpTask*,int> callstmp;
};


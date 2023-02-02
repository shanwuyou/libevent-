#include "XFtpUSER.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
using namespace std;


void XFtpUSER::Parse(std::string type, std::string msg)
{
	if (!cmdTask)return;
	string resmsg = "";
	if (type == "USER")
	{
		//230 Login successful
		resmsg = "230 Login successful";
	}
	
	resmsg += "\r\n";
	//bufferevent_write(cmdbev, resmsg.c_str(), resmsg.size());
	cmdTask->Send(resmsg);
}

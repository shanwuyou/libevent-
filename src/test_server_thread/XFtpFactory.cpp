#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpPORTTask.h"
#include "XFtpLIST.h"
#include "XFtpRETR.h"
#include "XFtpSTOR.h"
XTask *XFtpFactory::CreateTask()
{
	XFtpServerCMD *t = new XFtpServerCMD();
	XFtpUSER *user = new XFtpUSER();
	t->Reg("USER", user);
	
	
	t->Reg("PORT", new XFtpPORTTask());
	XFtpLIST *dir = new XFtpLIST();
	t->Reg("LIST", dir);
	t->Reg("PWD", dir);
	t->Reg("CWD", dir);
	t->Reg("CDUP", dir);
	XFtpRETR *retr = new XFtpRETR();
	t->Reg("RETR", retr);
	t->Reg("STOR", new XFtpSTOR());
	
	return t;
}

XFtpFactory::XFtpFactory()
{
}


XFtpFactory::~XFtpFactory()
{
}

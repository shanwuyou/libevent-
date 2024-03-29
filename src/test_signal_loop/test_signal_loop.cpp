#include <iostream>
#include <event2/event.h>
#include <signal.h>
using namespace std;
bool isexit = false;
//sock 文件描述符，which 事件类型 arg传递的参数
static void Ctrl_C(int sock,short which,void *arg)
{
    cout<<"Ctrl_C"<<endl;
    event_base *base = (event_base *)arg;
    //执行完当前处理的事件函数就退出
    //event_base_loopbreak(base);

    //运行所有的活动事件再退出
    //事件循环没有运行时也要等运行一次再退出
    timeval t = {3,0}; //至少运行三秒后退出
    event_base_loopexit(base,&t);
}

static void Kill(int sock,short which,void *arg)
{
    cout<<"Kill"<<endl;
	event *ev = (event *)arg;
	
	//如果处于非待决
	if(!evsignal_pending(ev,NULL))
	{
		event_del(ev);
		event_add(ev,NULL);
	}
}
int main(int argc,char *argv[])
{
        event_base *base = event_base_new();
        //添加ctrl +C 信号事件，处于no pending
		//evsignal_new 隐藏的状态 EV_SIGNAL|EV_PERSIST
        event *csig = evsignal_new(base,SIGINT,Ctrl_C,base);
		if(!csig)
		{
			cerr<<"SIGINT evsignal_new failed!"<<endl;
			return -1;
		}
		
		//添加事件到pending
		if(event_add(csig,0) != 0)
		{
			cerr<<"SIGINT event_add failed!"<<endl;
			return -1;
		}
		
		//添加kill信号
		//非持久事件，只进入一次 event_self_cbarg()传递当前的event
		event *ksig = event_new(base,SIGTERM,EV_SIGNAL,Kill,
		event_self_cbarg());
		if(!ksig)
		{
			cerr<<"SIGTERM evsignal_new failed!"<<endl;
			return -1;
		}
		
		//添加事件到pending
		if(event_add(ksig,0) != 0)
		{
			cerr<<"SIGTERM event_add failed!"<<endl;
			return -1;
		}
		

		
		
		//进入事件主循环
		//event_base_dispatch(base);
		//EVLOOP_ONCE 等待一个事件运行，直到没有活动事件就退出返回0
		//EVLOOP_NONBLOCK  有活动事件就处理，没有就返回0
		/*
		while(!isexit)
		{
			event_base_loop(base,EVLOOP_NONBLOCK);
		}
		*/
		//EVLOOP_NO_EXIT_ON_EMPTY 没有注册事件也不返回 用于事件后期多线程添加
		//event_base_loop(base,EVLOOP_NO_EXIT_ON_EMPTY);
		event_base_loop(base,EVLOOP_NO_EXIT_ON_EMPTY);
		//event_free(csig);
		event_base_free(base);
		
        return 0;
}
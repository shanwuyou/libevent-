#include <iostream>
#include <event2/event.h>
#ifndef _WIN32
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#else
#endif
#include <thread>
using namespace std;

void read_file(evutil_socket_t fd,short event, void *arg)
{
	char buf[1024] = {0};
	int len = read(fd,buf,sizeof(buf)-1);
	if(len>0)
	{
		cout<<buf<<endl;
	}
	else
	{
		cout<<"."<<flush;
		this_thread::sleep_for(500ms);
	}
}

int main(int argc,char *argv[])
{
#ifdef _WIN32 
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
#else
	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif
	event_config *conf = event_config_new();
	//设置支持文件描述符
	event_config_require_features(conf,EV_FEATURE_FDS);
	event_base *base = event_base_new_with_config(conf);
	event_config_free(conf);
	if(!base)
	{
		cerr<<"event_base_new_with_config failed!"<<endl;
		return -1;
	}
	//打开文件只读，非阻塞
	int sock = open("/var/log/auth.log",O_RDONLY|O_NONBLOCK,0);
	if(sock<=0)
	{
		cerr<<"open /var/log/auth.log failed!"<<endl;
		return -2;
	}
	
	//文件指针移到结尾处
	lseek(sock,0,SEEK_END);
	
	//监听文件数据
	event *fev = event_new(base,sock,EV_READ|EV_PERSIST,read_file,0);
	event_add(fev,NULL);
	
	//进入事件主循环
	event_base_dispatch(base);
	event_base_free(base);
		
    return 0;
}
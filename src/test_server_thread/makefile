SRC=test_server_thread.cpp XFtpPORTTask.cpp XServerMain.cpp XFtpRETR.cpp XFtpUSER.cpp \
	XFtpLIST.cpp XFtpServerCMD.cpp XFtpServerCMD.cpp XFtpFactory.cpp XTask.cpp \
	XThread.cpp XThreadPool.cpp XFtpSTOR.cpp
test_server_thread:$(SRC)
	g++ $^ -o $@ -levent -lpthread -levent_pthreads -g
	#gdb $@
clean:
	rm -rf test_server_thread
	rm -rf *.o


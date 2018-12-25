# network_programming
A homework of network_programming

This semester we have a course called Network Programming, on which we are demanded to write a simple program using the socket. So this is my project, a simple chat server.  

#文件列表如下：
./common.h
./config.sh
./database.c
./database.h
./deal_with_message.h
./main_assistant.c
./main.c
./Makefile
./Readme
./wrap_function.c

#文件说明：

###config.sh
是数据库配置文件，因为服务器在存储用户信息时用到了数据库，所以在编译服务器之前请先执行该文件进行数据库的配置（注意：请确保已经正确安装mysql8.0 :-) ）使用方法：
	  bash config.sh create		进行相关数据库的配置
	  bash config.sh remove		删除相关配置信息

###Makefile
在进行完数据库的配置后就可以编译服务器了，Makefile提供了两个功能
  	  make				编译服务器
  	  make clean			将编译过程中产生的目标文件全部删除

###main.c
包含主函数的文件

###main_assistant.c
包含的函数主要为每一个线程对客户端不同请求所做相应处理

###database.c
与数据库相关的函数

###wrap_function.c
一些系统函数的简单封装，当函数出错时自动报错

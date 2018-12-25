#include "database.h"

MYSQL *conn;//连接mysql的全局变量
pthread_mutex_t lock[256];//每一个用户都有一个写入的缓冲区，在同一时间只能有一个线程对其写入，所以为每一个用户分配一个锁
pthread_mutex_t mysql_lock=PTHREAD_MUTEX_INITIALIZER;//对mysql的查询也需要一个锁来进行限制，不然一个查询的结果可能被立刻执行的下一个查询覆盖掉

int main()
{
    int sockfd_l;//监听套接字
    int sockfd_c;//连接时产生的套接字
    struct sockaddr_in server;

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    sockfd_l=Socket(AF_INET,SOCK_STREAM,0);
    Bind(sockfd_l,(SA *)&server,sizeof(server));
    Listen(sockfd_l,BACKLOG);//以上大写字母开头的函数均为原函数的简单包装，如果出错自动打印错误，函数定义在wrap_function.c中
    Init_database();//初始化数据库
    Pthread_mutex_init(&lock[0]);//将第一个锁初始化

    while(1)
    {
	if((sockfd_c=accept(sockfd_l,NULL,NULL))>0)
	    New_thread(sockfd_c);//开启新的线程来完成有关注册、登录、聊天等功能
    }

    mysql_close(conn);
    close(sockfd_l);
    return 0;
}

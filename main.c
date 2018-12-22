#include "database.h"

MYSQL *conn;
pthread_mutex_t lock[256];
pthread_mutex_t mysql_lock=PTHREAD_MUTEX_INITIALIZER;

int main()
{
    int sockfd_l;
    int sockfd_c;
    struct sockaddr_in server;

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    sockfd_l=Socket(AF_INET,SOCK_STREAM,0);
    Bind(sockfd_l,(SA *)&server,sizeof(server));
    Listen(sockfd_l,BACKLOG);//以上大写字母开头的函数均为原函数的简单包装，如果出错自动打印错误，函数定义在wrap_function.c中
    Init_database();
    Pthread_mutex_init(&lock[0]);

    while(1)
    {
	if((sockfd_c=accept(sockfd_l,NULL,NULL))>0)
	    New_thread(sockfd_c);//开启新的线程来完成有关注册、登录、聊天等功能
    }

    mysql_close(conn);
    close(sockfd_l);
    return 0;
}

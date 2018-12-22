#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <errno.h>

#define PORT 6666
#define MAX 1000
#define BACKLOG 256

typedef struct sockaddr SA;


int Socket(int domain,int type,int protocol);
void Bind(int sockfd,const SA * addr,socklen_t addrlen);
void Listen(int sockfd,int backlog);
void Connect(int sockfd,const SA * addr,socklen_t addrlen);
void Read(int sockfd,char *buf,size_t len);
void Write(int sockfd,const char *buf,size_t len);


void Log_in_fun(int sockfd,char *id);
void Sign_up_fun(int sockfd,char *id);

int Socket(int domain,int type,int protocol)
{
    int sockfd;
    sockfd=socket(domain,type,protocol);
    if(sockfd==-1)
    {
	perror("socket error");
	exit(1);
    }
    return sockfd;
}

void Bind(int sockfd,const SA * addr,socklen_t addrlen)
{
    int i;
    i=bind(sockfd,addr,addrlen);
    if(i==-1)
    {
	perror("bind error");
	exit(1);
    }
}

void Listen(int sockfd,int backlog)
{
    int i;
    i=listen(sockfd,backlog);
    if(i==-1)
    {
	perror("listen error");
	exit(1);
    }
}

void Connect(int sockfd,const SA * addr,socklen_t addrlen)
{
    int i;
    i=connect(sockfd,addr,addrlen);
    if(i==-1)
    {
	perror("connect error");
	exit(1);
    }
}

void Read(int sockfd,char *buf,size_t len)
{
    ssize_t ret;
    int sum=0;
    while(len!=0 && (ret=read(sockfd,buf,len))!=0)
    {
	if(ret==-1)
	{
	    if(errno==EINTR)
		continue;
	    perror("read error");
	    exit(1);
	}
	len-=ret;
	buf+=ret;
	sum+=ret;
    }
    if(sum==0)
    {
	puts("链接已经中断");
	exit(1);
    }
}


}

void Write(int sockfd,const char *buf,size_t len)
{
    ssize_t ret;
    while(len !=0 &&(ret=write(sockfd,buf,len))!=0)
    {
	if(ret==-1)
	{
	    if(errno==EINTR)
		continue;
	    perror("write error");
	    exit(1);
	}
	buf+=ret;
	len-=ret;
    }
}

void Pthread_mutex_lock(pthread_mutex_t * mutex)
{
    if(pthread_mutex_lock(mutex))
    {
	perror("pthread_mutex_lock error");
	exit(1);
    }
}

void Pthread_mutex_unlock(pthread_mutex_t * mutex)
{
    if(pthread_mutex_unlock(mutex))
    {
	perror("pthread_mutex_lock error");
	exit(1);
    }
}

void Pthread_mutex_init(pthread_mutex_t * mutex)
{
    if(pthread_mutex_init(mutex,NULL))
    {
	perror("pthread_mutex_init error");
	exit(1);
    }
}

void Pthread_mutex_destroy(pthread_mutex_t * mutex)
{
    if(pthread_mutex_destroy(mutex))
    {
	perror("pthread_mutex_destroy error");
	exit(1);
    }
}

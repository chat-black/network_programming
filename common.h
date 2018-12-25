#ifndef COMMON_
#define COMMON_

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
#include <pthread.h>
#include <mysql/mysql.h>

#define PORT 6666
#define BACKLOG 256

typedef struct sockaddr SA;

extern pthread_mutex_t lock[256];
extern MYSQL *conn;
extern pthread_mutex_t mysql_lock;

int Socket(int domain,int type,int protocol);
void Bind(int sockfd,const SA * addr,socklen_t addrlen);
void Listen(int sockfd,int backlog);
void Connect(int sockfd,const SA * addr,socklen_t addrlen);
void Read(int sockfd,char *buf,size_t len,char *id);
void Write(int sockfd,const char *buf,size_t len);
void Pthread_mutex_lock(pthread_mutex_t * mutex);
void Pthread_mutex_unlock(pthread_mutex_t * mutex);
void Pthread_mutex_init(pthread_mutex_t * mutex);
void Pthread_mutex_destroy(pthread_mutex_t * mutex);
void New_thread(int sockfd);
void * thread_fun(void *arg);


void Log_in_fun(int sockfd,char *id);
void Sign_up_fun(int sockfd,char *id);
unsigned short int Get_id();
void Send_message_fun(int sockfd,const char *message);
#endif

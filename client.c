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


#define DEAL_WITH_MESSAG_

//  消息与控制信息均分组定长发送，以下为设定的长度
#define CONTROL_DATA_LEN 20
#define MESSAGE_LEN 100


//在消息与控制信息前均有一个字节用来表示类型，以下为其定义
#define NETWORK_SUCCESS 0
#define NETWORK_FAIL 3
#define NETWORK_LOG_IN 1
#define NETWORK_SIGN_UP 2
#define NETWORK_LIST 4
#define NETWORK_MESS 5

//下面的宏定义是操作数据库的格式化字符串
#define	INSERT_NAME_PASSWD "insert into user_message values('%s','%s',%d,1,%d)"
#define SEARCH_PASSWORD_BY_ID "select password from user_message where id=%s"
#define USER_ON_LINE "update user_message set log_in=1,sockfd=%d where id=%s"


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


int mylog=0;

void Print_help(void);
void Log_in(int sockfd);
void Sign_fun(int sockfd);
void List_fun(int sockfd);

int main()
{
    int sd;
    int len=0;
    char buffer[MAX]="wang";
    struct sockaddr_in server;

    bzero(&server,sizeof(server));
    server.sin_port=htons(PORT);
    server.sin_family=AF_INET;
    inet_aton("127.0.0.1",&(server.sin_addr));
    //inet_aton("107.191.52.170",&(server.sin_addr));
    sd=Socket(AF_INET,SOCK_STREAM,0);
    Connect(sd,(struct sockaddr *)&server,sizeof(server));

    puts("请选择要实现的功能(\\help 打印帮助信息):");
    while(1)
    {
	printf("您的输入： ");
	scanf("%s",buffer);
	if(strcmp(buffer,"\\help")==0)
	    Print_help();
	if(strcmp(buffer,"\\login")==0)
	    Log_in(sd);
	if(strcmp(buffer,"\\sign")==0)
	    Sign_fun(sd);
	if(strcmp(buffer,"\\list")==0)
	    List_fun(sd);

//	fgets(buffer,MAX,stdin);
//	len=sendto(sd,buffer,6,0,NULL,0);
//	if(len==strlen(buffer))
//	    printf("succeed\n");
	if(buffer[0]=='q')
	    break;
    }
    close(sd);
}

void Print_help()
{
    puts("");
    puts("	\\help		打印帮助信息");
    puts("	\\login		注册新用户");
    puts("	\\sign		登录");
    puts("	\\list		列出在线用户");
    puts("");
}

void Log_in(int sockfd)
{
    char buf[MAX];
    char name[CONTROL_DATA_LEN];
    char password[CONTROL_DATA_LEN];
    char id[CONTROL_DATA_LEN];

    buf[0]=NETWORK_LOG_IN;
    sendto(sockfd,buf,CONTROL_DATA_LEN,0,NULL,0);
    printf("your name: ");
    scanf("%s",name);
    printf("your password:");
    scanf("%s",password);
    write(sockfd,name,CONTROL_DATA_LEN);
    write(sockfd,password,CONTROL_DATA_LEN);
    Read(sockfd,id,CONTROL_DATA_LEN);
    Read(sockfd,buf,CONTROL_DATA_LEN);
    if(buf[0]==NETWORK_SUCCESS)
    {
	puts("log in success");
	printf("your id is %s",id);
	mylog=1;
    }
    else if(buf[0]==NETWORK_FAIL)
	puts("log in fail");
}

void Sign_fun(int sockfd)
{
    char buf[MAX];
    char id[CONTROL_DATA_LEN];
    char password[CONTROL_DATA_LEN];
    buf[0]=NETWORK_SIGN_UP;
    Write(sockfd,buf,CONTROL_DATA_LEN);
    printf("your id: ");
    scanf("%s",id);
    printf("your password:");
    scanf("%s",password);
    Write(sockfd,id,CONTROL_DATA_LEN);//将id返回给客户端
    Write(sockfd,password,CONTROL_DATA_LEN);//将
    Read(sockfd,buf,CONTROL_DATA_LEN);
    if(buf[0]==NETWORK_SUCCESS)
    {
	puts("sign success");
	mylog=1;
    }
    else if(buf[0]==NETWORK_FAIL)
	puts("sign fail");
}
void List_fun(int sockfd)
{
    if(mylog==0)
	puts("请先登录或注册");
    else if(mylog==1)
    {
	char buf[MAX];
	char message[CONTROL_DATA_LEN];
	int num;
	message[0]=NETWORK_LIST;
	Write(sockfd,message,CONTROL_DATA_LEN);
	Read(sockfd,buf,MESSAGE_LEN);
	while(buf[0]!=NETWORK_FAIL)
	{
	    printf("%s\n",buf+1);
	    Read(sockfd,buf,MESSAGE_LEN);
	}
    }
}

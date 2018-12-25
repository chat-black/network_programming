#include "common.h"
#include "deal_with_message.h"
#include "database.h"

struct user
{
    unsigned int user_id;
    int user_sockfd;
} ;

void New_thread(int sockfd)
{
    pthread_t t;
    if(pthread_create(&t,NULL,thread_fun,&sockfd)==0)//创建新线程
	printf("thread success\n");
}
void * thread_fun(void *arg)
{
    char buf[DATA_LEN];
    int sockfd=*((int *)arg);
    char id[DATA_LEN];
    id[0]=NETWORK_FAIL;//该变量为线程所连接的用户的id，记录该值便于在用户退出时，更新数据库

    while(1)
    {
	Read(sockfd,buf,DATA_LEN,id);

	switch(buf[0])
	{
	    case NETWORK_LOG_IN://注册
		puts("log in");
		Log_in_fun(sockfd,id);
		break;
	    case NETWORK_SIGN_UP ://登录
		puts("sign up");
		Sign_up_fun(sockfd,id);
		break;
	    case NETWORK_LIST ://打印用户
		puts("list users");
		List_user_fun(sockfd);
		break;
	    case MSG:
		puts("send message");
		Send_message_fun(sockfd,buf);
	    default:
		break;
	}
    }
    close(sockfd);
    pthread_exit(NULL);
}

void Log_in_fun(int sockfd,char *id)
{
    char name[DATA_LEN];
    char password[DATA_LEN];
    unsigned short int id_num;
    char  temp_id[DATA_LEN];
    int num;

    Read(sockfd,name,DATA_LEN,id);//得到用户名
    Read(sockfd,password,DATA_LEN,id);//得到密码

    printf("get name :%s\n",name+3);
    printf("get password :%s\n",password+3);
    Pthread_mutex_lock(&lock[0]);
    id_num=Get_id();//产生id
    sprintf(id,"%hu",id_num);
    sprintf(temp_id+3,"%c",id_num);
    printf("send id:%d",temp_id[3]);
    Write(sockfd,temp_id,DATA_LEN);//将id返回给客户端
    Insert_name_passwd(name+3,password+3,id_num,sockfd);//将新用户信息注册到数据库中
    Pthread_mutex_unlock(&lock[0]);
    Pthread_mutex_init(&lock[id_num]);
}

void Sign_up_fun(int sockfd,char *id)
{
    char password[DATA_LEN];
    auto char message[DATA_LEN];
    auto char temp_id[DATA_LEN];

    Read(sockfd,temp_id,DATA_LEN,id);
    printf("get id: %hhu\n",temp_id[3]);//
    Read(sockfd,password,DATA_LEN,id);
    printf("get password: %s\n",password+3);//


    if(Passwd_right_fun(temp_id,password+3))
    {
	message[0]=NETWORK_SUCCESS;
	Write(sockfd,message,DATA_LEN);
	User_online_fun(temp_id,sockfd);
	sprintf(id,"%hhu",temp_id[3]);
	printf("success\n");
    }
    else
    {
	message[0]=NETWORK_FAIL;
	Write(sockfd,message,DATA_LEN);
	printf("fail\n");
    }
}

unsigned short int Get_id()//获得id给客户端
{
    unsigned short int i;
    i=Get_max_id();
    i++;
    return i;
}

void Send_message_fun(int sockfd,const char *message)
{
    static struct user last_user={0,0};//存储上一个通信对象的id以及sockfd，这次的通信如果没有改变，直接使用该id进行通信，如果对象已经改变，则根据id，重新查询sockfd

    char error_message[DATA_LEN];
    if(last_user.user_id==message[2])
	Write(last_user.user_sockfd,message,DATA_LEN);
    else
    {
	unsigned int j;
	j=Get_user_sockfd(message[2]);
	if(j==0)
	{
	    puts("id wrong");
	    error_message[0]=NETWORK_FAIL;
	    Write(sockfd,error_message,DATA_LEN);
	}
	else
	{
	    last_user.user_sockfd=j;
	    last_user.user_id=message[2];//更新last_user的信息
	    Write(last_user.user_sockfd,message,DATA_LEN);
	}
    }
}

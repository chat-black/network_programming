#include "common.h"
#include "deal_with_message.h"
#include "database.h"

void New_thread(int sockfd)
{
    pthread_t t;
    if(pthread_create(&t,NULL,thread_fun,&sockfd)==0)
	printf("thread success\n");
}
void * thread_fun(void *arg)
{
    char buf[MAX];
    int sockfd=*((int *)arg);
    int num;
    char id[CONTROL_DATA_LEN];
    id[0]=NETWORK_FAIL;

    while(1)
    {
	Read(sockfd,buf,CONTROL_DATA_LEN,id);

	switch(buf[0])
	{
	    case NETWORK_LOG_IN:
		//puts("log in");
		Log_in_fun(sockfd,id);
		break;
	    case NETWORK_SIGN_UP :
		puts("sign up");
		Sign_up_fun(sockfd,id);
		break;
	    case NETWORK_LIST :
		puts("list users");
		List_user_fun(sockfd);
		break;

	if(buf[0]=='q')
	    break;
	}
    }
    close(sockfd);
    pthread_exit(NULL);
}

void Log_in_fun(int sockfd,char *id)
{
    char name[CONTROL_DATA_LEN];
    char password[CONTROL_DATA_LEN];
    unsigned short int id_num;
    auto char message[CONTROL_DATA_LEN];
    int num;

    Read(sockfd,name,CONTROL_DATA_LEN,id);//得到用户名
    Read(sockfd,password,CONTROL_DATA_LEN,id);//得到密码

    printf("get name :%s\n",name);
    printf("get id :%s\n",id);
    Pthread_mutex_lock(&lock[0]);
    id_num=Get_id();//产生id
    sprintf(id,"%d",id_num);
    Write(sockfd,id,CONTROL_DATA_LEN);//将id返回给客户端
    message[0]=NETWORK_SUCCESS;
    Write(sockfd,message,CONTROL_DATA_LEN);//将id返回给客户端
    Insert_name_passwd(name,password,id_num,sockfd);//将新用户信息注册到数据库中
    Pthread_mutex_unlock(&lock[0]);
    Pthread_mutex_init(&lock[id_num]);
}

void Sign_up_fun(int sockfd,char *id)
{
    char password[CONTROL_DATA_LEN];
    auto char message[CONTROL_DATA_LEN];

    Read(sockfd,id,CONTROL_DATA_LEN,id);
    printf("get id: %s\n",id);//
    Read(sockfd,password,CONTROL_DATA_LEN,id);
    printf("get password: %s\n",password);//


    if(Passwd_right_fun(id,password))
    {
	message[0]=NETWORK_SUCCESS;
	Write(sockfd,message,CONTROL_DATA_LEN);
	User_online_fun(id,sockfd);
    }
    else
    {
	message[0]=NETWORK_FAIL;
	Write(sockfd,message,CONTROL_DATA_LEN);
    }
}

unsigned short int Get_id()//获得id给客户端
{
    unsigned short int i;
    i=Get_max_id();
    i++;
    return i;
}

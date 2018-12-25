#include "database.h"

void Init_database()
{
    char name[20];
    char password[20];
    printf("正在初始化服务器....\n");
    sleep(1);

    conn=mysql_init(NULL);
    if (conn == NULL) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    while(1)
    {
	printf("服务器需要连接数据库，请输入数据库用户名: ");
	scanf("%s",name);
	printf("请输入数据库用户的密码:");
	scanf("%s",password);
	//read(0,password,20); 
	//printf("%s\n",password);
	if(mysql_real_connect(conn,"localhost",name,password,"Network",0,NULL,0)==NULL) 
	{
	    printf("连接数据库失败,请重新输入\n");
	    printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	    continue;
	}
	break;
    }
    printf("服务器启动成功\n");
}

void Insert_name_passwd(const char * name,const char *password,int id,int sockfd)
{
    char temp[DATA_LEN];
    sprintf(temp,INSERT_NAME_PASSWD,name,password,id,sockfd);
    if(mysql_query(conn,temp))
    {
	printf("%s\n",temp);
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
}

unsigned short int Get_max_id()
{
    MYSQL_RES * result;
    my_ulonglong num;
    MYSQL_ROW row;
    unsigned short int max;

    Pthread_mutex_lock(&mysql_lock);
    if (mysql_query(conn, "select max(id) from user_message")) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    result=mysql_store_result(conn);
    Pthread_mutex_unlock(&mysql_lock);
    if((num=mysql_num_rows(result))==0)
	return 0;
    else
    {
	row=mysql_fetch_row(result);
	sscanf(row[0],"%hu",&max);
	return max;
    }
}

int Passwd_right_fun(const char *id,const char *password)
{
    char temp[DATA_LEN];
    my_ulonglong num;
    MYSQL_RES *result;
    MYSQL_ROW row;

    sprintf(temp,SEARCH_PASSWORD_BY_ID,id[3]);
    Pthread_mutex_lock(&mysql_lock);
    if(mysql_query(conn,temp))
    {
	printf("%s\n",temp);
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	Pthread_mutex_unlock(&mysql_lock);
	return 0;

    }
    result=mysql_store_result(conn);
    Pthread_mutex_unlock(&mysql_lock);
    if((num=mysql_num_rows(result))==0)
	return 0;
    else
    {
	row=mysql_fetch_row(result);
	auto int len1=strlen(row[0]);
	auto int len2=strlen(password);
	if(len1!=len2)
	    return 0;
	else
	{
	    if(strcmp(row[0],password)!=0)
		return 0;
	    else return 1;
	}
    }
}
void User_online_fun(const char *id,int sockfd)
{
    char temp[DATA_LEN];
    unsigned short int id_num;
    sprintf(temp,USER_ON_LINE,sockfd,id[3]);
    id_num=id[3];
    if(mysql_query(conn,temp))
    {
	printf("%s\n",temp);
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    Pthread_mutex_init(&lock[id_num]);
}
    
void List_user_fun(int sockfd)
{
    MYSQL_RES * result;
    my_ulonglong num;
    MYSQL_ROW row;
    Pthread_mutex_lock(&mysql_lock);
    char message[DATA_LEN];

    if (mysql_query(conn, "select name,id from user_message where log_in=1")) 
    {
	printf("select name,id from user_message where log_in=1\n");
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    result=mysql_store_result(conn);
    Pthread_mutex_unlock(&mysql_lock);
    if((num=mysql_num_rows(result))!=0)
    {
	message[0]=1;
	for(int i=0;i<num;i++)
	{
	    row=mysql_fetch_row(result);
	    int j;
	    sscanf(row[1],"%d",&j);
	    message[3]=j;
	    sprintf(message+4,"%s",row[0]);
	    printf("%d\n",message[3]);
	    printf("send :%s\n",message+4);
	    Write(sockfd,message,DATA_LEN);
	}
    }
    message[0]=NETWORK_LIST_END;
    Write(sockfd,message,DATA_LEN);
}

void User_offline_fun(const char * id)
{

    if(id[0]!=NETWORK_FAIL)
    {
	char temp[DATA_LEN];
	sprintf(temp,"update user_message set log_in=0 where id=%s",id);
	if (mysql_query(conn, temp)) 
	{
	    printf("%s\n",temp);
	    printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	    exit(1);
	}
    }
}

unsigned int Get_user_sockfd(int id)
{
    MYSQL_RES * result;
    my_ulonglong num;
    MYSQL_ROW row;
    Pthread_mutex_lock(&mysql_lock);
    char temp[DATA_LEN];

    sprintf(temp,"select sockfd from user_message where id=%d",id);
    printf("%s",temp);

    if (mysql_query(conn, temp)) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    result=mysql_store_result(conn);
    Pthread_mutex_unlock(&mysql_lock);
    if((num=mysql_num_rows(result))!=0)
    {
	row=mysql_fetch_row(result);
	unsigned int j;
	sscanf(row[0],"%u",&j);
	return j;
    }
    else
    {
	printf("%lld\n",num);
	return 0;
    }
}

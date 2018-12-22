#include "database.h"

void Init_database()
{
    conn=mysql_init(NULL);
    if (conn == NULL) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    if(mysql_real_connect(conn,"localhost","root","19980309","network",0,NULL,0)==NULL) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
}

void Insert_name_passwd(const char * name,const char *password,int id,int sockfd)
{
    auto char temp[MAX];
    sprintf(temp,INSERT_NAME_PASSWD,name,password,id,sockfd);
    if(mysql_query(conn,temp))
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
}

unsigned short int Get_max_id()
{
    MYSQL_RES * result;
    my_ulonglong num;
    MYSQL_ROW row;
    auto unsigned short int max;

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
    auto char temp[MAX];
    auto my_ulonglong num;
    auto MYSQL_RES *result;
    auto MYSQL_ROW row;

    sprintf(temp,SEARCH_PASSWORD_BY_ID,id);
    Pthread_mutex_lock(&mysql_lock);
    if(mysql_query(conn,temp))
    {
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
    auto char temp[MAX];
    auto unsigned short int id_num;
    sprintf(temp,USER_ON_LINE,sockfd,id);
    sscanf(id,"%hu",&id_num);
    if(mysql_query(conn,temp))
    {
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
    auto char message[MESSAGE_LEN];

    if (mysql_query(conn, "select name,id from user_message where log_in=1")) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
    result=mysql_store_result(conn);
    Pthread_mutex_unlock(&mysql_lock);
    if((num=mysql_num_rows(result))!=0)
    {
	message[0]=NETWORK_MESS;
	for(int i=0;i<num;i++)
	{
	    row=mysql_fetch_row(result);
	    sprintf(message+1,"%s		%s",row[0],row[1]);
	    printf("send :%s\n",message+1);
	    Write(sockfd,message,MESSAGE_LEN);
	}
    }
    message[0]=NETWORK_FAIL;
    Write(sockfd,message,MESSAGE_LEN);
}

void User_offline_fun(const char * id)
{

    auto char temp[MAX];
    sprintf(temp,"update user_message set log_in=0 where id=%s",id);
    if (mysql_query(conn, temp)) 
    {
	printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
	exit(1);
    }
}

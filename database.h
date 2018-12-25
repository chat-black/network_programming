#ifndef DATABASE_
#define DATABASE_

#include "common.h"
#include "deal_with_message.h"

void Init_database();
void Insert_name_passwd(const char * name,const char *password,int id,int sockfd);
unsigned short int Get_max_id();
int Passwd_right_fun(const char *id,const char *password);
void User_online_fun(const char *id,int sockfd);
void User_offline_fun(const char *id);
void List_user_fun(int sockfd);
unsigned int Get_user_sockfd(int id);

#endif

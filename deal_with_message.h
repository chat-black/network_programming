#ifndef DEAL_WITH_MESSAG_

#define DEAL_WITH_MESSAG_

//  消息与控制信息均分组定长发送，以下为设定的长度
#define DATA_LEN 128



//在消息与控制信息前均有一个字节用来表示类型，以下为其定义
#define NETWORK_SUCCESS 1
#define NETWORK_FAIL 0
#define NETWORK_LOG_IN 2
#define NETWORK_SIGN_UP 3
#define NETWORK_LIST 4
#define NETWORK_LIST_END 5
#define MSG 6

//下面的宏定义是操作数据库的格式化字符串
#define	INSERT_NAME_PASSWD "insert into user_message values('%s','%s',%d,1,%d)"
#define SEARCH_PASSWORD_BY_ID "select password from user_message where id=%d"
#define USER_ON_LINE "update user_message set log_in=1,sockfd=%d where id=%d"

#endif

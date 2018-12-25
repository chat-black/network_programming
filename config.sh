create="create"
remove="remove"
if [[ $1 = $create ]]
then
echo "正在配置数据库..."
sleep 1
temp=`tail -n 4 config.sh`
echo "请输入登录数据库所用的用户姓名"
read -p "Enter name: " name
echo "请输入数据库密码"
echo -n $temp |mysql -u $name -p
if [[ $? == "0" ]]
then
echo "配置成功"
else
echo "配置失败"
fi

elif [[ $1 = $remove ]]
then
echo "正在移除数据库..."
sleep 1
temp=`tail -n 5 config.sh|head -n 1`
echo "请输入登录数据库所用的用户姓名"
read -p "Enter name: " name
echo "请输入数据库密码"
echo -n $temp |mysql -u $name -p
if [[ $? == "0" ]]
then
echo "移除成功"
else
echo "配置失败"
fi
else
echo "命令错误，用法请参考Readme文件"
fi
exit
drop database Network;
create database Network;
use Network;
create table user_message( name char(20), password char(20), id int primary key, log_in int, sockfd int);
insert into user_message values("root","root",0,0,0);

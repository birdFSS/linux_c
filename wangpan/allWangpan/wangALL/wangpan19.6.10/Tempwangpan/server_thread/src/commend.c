#include "../include/factory.h"
#define STR_LEN 11

void sendErrorMsg(int fd, linkMsg_t* plmsg)
{
    plmsg->flag = FAIL_FLAG;
    plmsg->size = 16 + strlen(plmsg->buf);
    send(fd, plmsg, plmsg->size, 0);
}

static void GenerateStr(char *str)
{
    int i=0, flag;
    str[0] = '$';
    str[1] = '6';
    str[2] = '$';
    srand(time(NULL));
    for(i=3;i<STR_LEN;i++)
    {
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break;
        case 2:
            str[i] = rand()%10 + '0';
            break;
        }
    }
    //printf("%s\n", str);    
}

static int apartUserInformation(char* buf, User_t* pUser)
{
    char* p=buf;
    int i=0;
    while(*p != '$')
    {
        pUser->name[i++] = *p;
        p++;
    }
    p++;
    i=0;
    while(*p != '$' && *p != '\0')
    {
        pUser->passwd[i++] = *p;
        p++;
    }
    return 0;
}

int userEnroll(int fd, MYSQL* conn,linkMsg_t* plmsg)
{
    User_t user;
    int ret;
    bzero(&user, sizeof(User_t));
    apartUserInformation(plmsg->buf, &user);
    //清空信息，保存返回信息
    bzero(plmsg->buf,sizeof(plmsg->buf));
    
    GenerateStr(user.salt);
    //将密码暂存至一个字符数组，生成密文，防止明文密码长于密文导致出错
    char TempBuf[100] = {0};
    strcpy(TempBuf, user.passwd);
    memset(user.passwd, 0, sizeof(user.passwd));
    strcpy(user.passwd, crypt(TempBuf, user.salt));
    //send(fd,user.salt, strlen(user.salt),0);
    char insertUser[300] = {0};
    char insertHead[50]  = "INSERT INTO user(name, salt, password) values(";
    sprintf(insertUser,"%s'%s','%s','%s')",insertHead,user.name, user.salt, user.passwd);
    ret = insertMySQL(conn,insertUser,plmsg->buf);
    if(-1 == ret)
    {
        sendErrorMsg(fd,plmsg);
        return -1;
    }

    //将目录插入文件表,目录没有md5，和真实文件。
    char insertFile[300] = "INSERT INTO file(prevID,userName,type,clientName) values(";
    sprintf(insertFile, "%s%s,'%s','%s','%s')",insertFile, "-1", user.name, "d","/");
    ret = insertMySQL(conn, insertFile, plmsg->buf);
    if(-1 == ret)
    {
        sendErrorMsg(fd,plmsg);
        return -1;
    }
    //获取新增目录的id
    long dirID = mysql_insert_id(conn); 
    //更新用户主目录id
    char updateUser[300] = "UPDATE user set dirID=";
    sprintf(updateUser, "%s%ld %s%s'",updateUser, dirID, "where name='",user.name);
    ret = updateMySQL(conn, updateUser, plmsg->buf);
    if(-1 == ret)
    {
        sendErrorMsg(fd,plmsg);
        return -1;
    }

    /*
    printf("insertUser=%s\n", insertUser);
    printf("salt = %s\n", user.salt);
    printf("crypt = %s\n", crypt(user.passwd, user.salt));
    printf("dirID=%ld\n", dirID);
    printf("insertFile=%s\n", insertFile);
    printf("updateUser = %s\n", updateUser);
    */

    printf("insert success\n");

    //注册成功，发送信息给客户端
    plmsg->flag = SUCCESS;
    plmsg->size = 16 + strlen(plmsg->buf);
    send(fd, plmsg, plmsg->size,0); 
    return 0;
}

int userLogin(int fd, MYSQL* conn, linkMsg_t* plmsg)
{
    User_t user;
    int ret;
    ret = 0;
    bzero(&user, sizeof(user));
    strcpy(user.name, plmsg->buf);

    //根据用户名，检索user表，得到salt值和密文
    char queryInfo[300]="select * from user where name = '";
    sprintf(queryInfo, "%s%s'", queryInfo, user.name);
    //printf("user.name=%s\n", user.name);
    ret = queryMySQLForUser(conn, queryInfo, &user);
    if(-1 == ret)
    {
        strcpy(plmsg->buf, "没有该用户！");
        plmsg->size = strlen(plmsg->buf)+16;
        plmsg->flag = FAIL_FLAG;
        sendErrorMsg(fd, plmsg);
        return -1;
    }
#ifdef DEBUG_SERVER
    printf("\n\n-----------------");
    printf("user.name = %s\n", user.name);
    printf("user.salt = %s\n", user.salt);
    printf("user.id = %s\n", user.id);
    printf("user.passwd = %s\n", user.passwd);
    printf("user.dirID = %s\n", user.dirID);
#endif

    //发送salt值给客户端
    bzero(plmsg->buf, strlen(plmsg->buf));
    strcpy(plmsg->buf, user.salt);
    plmsg->size = strlen(plmsg->buf) + 16;
    plmsg->flag = SUCCESS;
    send(fd,plmsg,plmsg->size,0);

    //接收客户端发送来的密文
    recvCycle(fd, plmsg, 16);
    recvCycle(fd, plmsg->buf, plmsg->size-16);

    //将两个密文进行匹配，将结果发送给客户端
    if(!strcmp(plmsg->buf, user.passwd))
    {
        //密码正确
        plmsg->flag = SUCCESS;
        strcpy(plmsg->buf, "登录成功");
        plmsg->size = strlen(plmsg->buf)+16;
        send(fd, plmsg, plmsg->size,0);
    }else{
        plmsg->flag = FAIL_FLAG;
        strcpy(plmsg->buf,"密码错误");
        plmsg->size = strlen(plmsg->buf)+16;
        send(fd, plmsg, plmsg->size, 0);
        return -1;
    }
    printf("success\n");
    return 0;
}

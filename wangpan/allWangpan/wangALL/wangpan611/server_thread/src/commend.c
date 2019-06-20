#include "../include/factory.h"
#define STR_LEN 11

void sendErrorMsg(int fd, linkMsg_t* plmsg)
{
    plmsg->flag = FAIL_FLAG;
    plmsg->size = MSGHEADSIZE + strlen(plmsg->buf);
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
    plmsg->size = MSGHEADSIZE + strlen(plmsg->buf);
    send(fd, plmsg, plmsg->size,0); 
    return 0;
}

int userLogin(int fd, MYSQL* conn, linkMsg_t* plmsg, UserState_t* pUState)
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
        plmsg->size = strlen(plmsg->buf)+MSGHEADSIZE;
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
    plmsg->size = strlen(plmsg->buf) + MSGHEADSIZE;
    plmsg->flag = SUCCESS;
    send(fd,plmsg,plmsg->size,0);

    //接收客户端发送来的密文
    recvCycle(fd, plmsg, MSGHEADSIZE);
    recvCycle(fd, plmsg->buf, plmsg->size-MSGHEADSIZE);

    //将两个密文进行匹配，将结果发送给客户端
    if(!strcmp(plmsg->buf, user.passwd))
    {
        //密码正确,将token发送给客户端
        //登记用户最后操作时间
        time_t t;
        t = time(NULL);
        updateOPTime(conn, user.id, t);
        //生成token,存入数据库，并发送给客户端
        char dest[100] = {0};
        sprintf(dest, "%s%ld", user.name, t);
        char md5_string[40];
        Compute_string_md5((unsigned char*)dest, strlen(dest), md5_string);
        plmsg->flag = SUCCESS;
        strcpy(plmsg->buf, md5_string);
        plmsg->size = strlen(plmsg->buf)+ MSGHEADSIZE;
        send(fd, plmsg, plmsg->size,0);
        char InsertMd5Str[200]="update user set token=";
        sprintf(InsertMd5Str, "%s'%s' %s'%s'", InsertMd5Str, md5_string, "where id=", user.id);
        updateMySQL(conn, InsertMd5Str, NULL);
        printf("success\n");
    }else{
        plmsg->flag = FAIL_FLAG;
        strcpy(plmsg->buf,"密码错误");
        plmsg->size = strlen(plmsg->buf)+MSGHEADSIZE;
        send(fd, plmsg, plmsg->size, 0);
        return -1;
    }
    //将用户状态保存在另一个结构体中
    strcpy(pUState->name,user.name);
    strcpy(pUState->currentDirId,user.dirID);

    return 0;
}

int CdCommand(int fd, MYSQL* conn, linkMsg_t* plmsg, UserState_t* pUState)
{
    
    return 0;
}

int PwdCommand(int fd, MYSQL* conn, linkMsg_t* plmsg, UserState_t* pUState)
{

    return 0;
}

int RmCommand(int fd, MYSQL* conn, linkMsg_t* plmsg)
{

    return 0;
}

int LsCommand(int fd, MYSQL* conn, UserState_t* pUState)
{
    char queryInfo[100] = "select type, fileSize,clientName from file where prevID=";
    sprintf(queryInfo,"%s%s", queryInfo,pUState->currentDirId);
    linkMsg_t lmsg;
    bzero(&lmsg, sizeof(lmsg));
    lmsg.flag = SUCCESS;
    //将目录消息放入lmsg.buf
    /*这里目录长度可能会超过lmsg.buf最大值*/
    queryMySQL(conn, queryInfo, lmsg.buf);
    lmsg.size = strlen(lmsg.buf) + MSGHEADSIZE;
    send(fd, &lmsg, lmsg.size,0);
    return 0;
}

int findFatherDirId(MYSQL* conn, char* childDirID, pDirState_t fatherDir)
{
    bzero(fatherDir->dirID, DIRIDSIZE);
    bzero(fatherDir->dirName, DIRNAMESIZE);
    //获得父目录id
    char queryInfo[100] = "select prevID from file where dirID=";
    sprintf(queryInfo, "%s%s", queryInfo,childDirID);
    queryMySQL(conn, queryInfo,fatherDir->dirID);

    //获得父目录的名字
    char queryInfoForName[100] = "select clientName from file where dirID=";
    sprintf(queryInfo, "%s%s", queryInfoForName,childDirID);
    queryMySQL(conn, queryInfo,fatherDir->dirName);

    //当走到最上层目录时，返回-1
    if(!strcmp(fatherDir->dirID, "-1"))
    {
        return -1;
    }
    return 0;
}

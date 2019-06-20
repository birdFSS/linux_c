#ifndef __FACTORY_H__
#define __FACTORY_H__
#include"thread_all.h"
#include"work_que.h"
#include <aio.h>
#include <mysql/mysql.h>

#define DEBUG_SERVER
//表示使用mmap的临界值
#define BIGFILESIZE 100 * 1024 * 1024  
//一般缓冲区大小
#define BUFSIZE 2048

typedef struct{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    int threadNum;
    int startFlag;
}Factory_t, *pFactory_t;

typedef struct linkMsg{
    int size;
    int flag;//标志位 读写浏览
    size_t fileSize;
    char buf[512];//文件名，文件大小，接收目录
}linkMsg_t;

typedef struct user{
    char name[50];
    char passwd[50];
    char salt[20];
    char Ciphertext[100];
}User_t;

/*客户端发给服务器的操作码*/
enum commend_num
{
    USERENROLL = 0,
    USERLOGIN,
    CDCOMMEND,
    LSCOMMEND,
    PUTSCOMMEND,
    GETSCOMMEND,
    RMCOMMEND,
    PWDCOMMEND
};

/*服务器发给客户端的确认码*/
enum flag_return
{
    FAIL_FLAG = 0,
    SUCCESS
};

int factoryInit(pFactory_t pf, int threadNum, int capacity);
int factoryStart(pFactory_t pf);
void* threadFun(void *p);
int tcpInit(char* ip, char* port);//返回socket描述符
int transmiss(int outFd, linkMsg_t *plmsg);
int recvFile(int sfd, linkMsg_t* plmsg);
int epollAdd(int epfd, int fd);
void factoryDestroy(pFactory_t pf);

/*----------------------操作指令函数-----------------------------------*/
int userEnroll(int fd, MYSQL* conn, linkMsg_t* plmsg);
int funcCD(char* dirName, char* pInfo);
int funcLS(char* dirName, char* pInfo);
int funcRM(char* fileName, char* pInfo);
int funcPWD(char* pInfo, int infoLength);

/*----------------------数据库操作函数-----------------------------------*/

int connectMYSQL(MYSQL** pConn);
int queryMySQL(MYSQL* conn, char* queryInfo, char* resultInfo);
int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo);
int deleteMySQL(MYSQL *conn, char* deleteInfo, char* resultInfo);
int updateMySQL(MYSQL* conn, char* updateInfo, char* resultInfo);



int recvCycle(int sfd, void* buf, long fileSize);

#endif

#ifndef __FACTORY_H__
#define __FACTORY_H__
#include"thread_all.h"
#include"work_que.h"
#include<aio.h>

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

int factoryInit(pFactory_t pf, int threadNum, int capacity);
int factoryStart(pFactory_t pf);
void* threadFun(void *p);
int tcpInit(char* ip, char* port);//返回socket描述符
int transmiss(int outFd, linkMsg_t *plmsg);
int recvFile(int sfd, linkMsg_t* plmsg);
int epollAdd(int epfd, int fd);
void factoryDestroy(pFactory_t pf);


int recvCycle(int sfd, void* buf, long fileSize);

int aioWr(int toFd, char* buf, size_t length);
int aioRead(int toFd, char* buf, size_t length);
#endif

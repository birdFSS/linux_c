#ifndef __FACTORY_H__
#define __FACTORY_H__
#include"thread_all.h"
#include"work_que.h"
typedef struct{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    int threadNum;
    int startFlag;
}Factory_t, *pFactory_t;

int factoryInit(pFactory_t pf, int threadNum, int capacity);
int factoryStart(pFactory_t pf);
void* threadFun(void *p);
int tcpInit(char* ip, char* port);//返回socket描述符
int transport(int outFd);
int epollAdd(int epfd, int fd);
void factoryDestroy(pFactory_t pf);
#endif

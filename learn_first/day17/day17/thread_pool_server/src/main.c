#include "factory.h"

int main(int argc,char *argv[])
{
    if(argc!=5)
    {
        printf("./server ip port threadNum capacity\n");
        return -1;
    }
    Factory_t threadMainData;
    int threadNum=atoi(argv[3]);
    int capacity=atoi(argv[4]);
    factoryInit(&threadMainData,threadNum,capacity);
    factoryStart(&threadMainData);
    int socketFd,newFd;
    tcpInit(&socketFd,argv[1],argv[2]);
    pQue_t pq=&threadMainData.que;
    pNode_t pNew;
    while(1)
    {
        newFd=accept(socketFd,NULL,NULL);
        pNew=(pNode_t)calloc(1,sizeof(Node_t));
        pNew->newFd=newFd;
        //放入队列
        pthread_mutex_lock(&pq->mutex);
        queInsert(pq,pNew);
        pthread_mutex_unlock(&pq->mutex);
        //通知子线程有任务
        pthread_cond_signal(&threadMainData.cond);
    }
    return 0;
}


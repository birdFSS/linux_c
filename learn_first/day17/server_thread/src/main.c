#include "factory.h"
int exitFlag = 0;
void exitHandler(int signum){
    printf("%d is coming\n", signum);
    exitFlag = 1;
}

int main(int argc, char* argv[])
{
    if(5 != argc){
        printf("./ip port threadNum Listcapacity\n");
        return -1;
    }
    //设置信号处理机制保证有序退出
    signal(SIGUSR1, exitHandler);
    //创建线程工厂
    Factory_t pf;//存在于栈空间，不需要释放
    int capacity = atoi(argv[4]);
    int threadNum = atoi(argv[3]);
    factoryInit(&pf, threadNum, capacity);
    factoryStart(&pf);
    //创建tcp连接，监听对应端口
    int socketFd = tcpInit(argv[1], argv[2]);
    ERROR_CHECK(socketFd, -1, "tcpInit");
    int newFd, i;
    pQue_t pq = &pf.que;
    pNode_t pNew;
    while(1){
        newFd=accept(socketFd, NULL, NULL);
        if(1 == exitFlag){
            printf("quiting now! wait a moment!\n");
            goto end;
        }
        pNew=(pNode_t)calloc(1,sizeof(Node_t));
        pNew->newFd = newFd;
        //放入队列
        pthread_mutex_lock(&pq->mutex);
        queInsert(pq, pNew);
        pthread_mutex_unlock(&pq->mutex);
        pthread_cond_signal(&pf.cond);
    }
end:
    for(i=0;i<pf.threadNum;i++){
        pthread_cancel(*(pf.pthid + i));
    }
    for(i=0;i<pf.threadNum;i++){
        pthread_join(*(pf.pthid+i), NULL);
    }
    close(socketFd);
    factoryDestroy(&pf);
    return 0;
}


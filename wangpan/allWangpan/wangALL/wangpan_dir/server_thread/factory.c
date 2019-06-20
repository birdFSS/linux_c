#include "factory.h"

int factoryInit(pFactory_t pf,int threadNum,int capacity){
    queInit(&pf->que, capacity);
    pthread_cond_init(&pf->cond, NULL);
    pf->pthid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    pf->threadNum = threadNum;
    pf->startFlag = 0;
    return 0;
}

void threadExitFunc(void *p){
    pQue_t pq = (pQue_t)p;
    pthread_mutex_unlock(&pq->mutex);
    //printf("pthread is out\n");
    //pthread_exit(NULL);//清理函数中不要有pthread_exit
}

//这里不熟
void* threadFun(void *p){
    pFactory_t pf = (pFactory_t)p;
    pQue_t pq = &pf->que;
    pNode_t pCur;
    int getSuccess;
    linkMsg_t lmsg;//size_t 8字节
    while(1){
       pthread_mutex_lock(&pq->mutex);
       pthread_cleanup_push(threadExitFunc, pf);//防止死锁
       if(!pq->size){
           pthread_cond_wait(&pf->cond, &pq->mutex);
       }
       getSuccess = queGet(pq, &pCur);
       pthread_cleanup_pop(1);
       if(!getSuccess){
           //接收客户端的请求
           memset(&lmsg, 0, sizeof(lmsg));
           recvCycle(pCur->newFd, &lmsg, 16);//接收大小，标志，文件大小
           recvCycle(pCur->newFd, lmsg.buf, lmsg.size - 16);

#ifdef DEBUG_SERVER
    printf("/n/n-------factory.c---threadFun------------\n");
    printf("接收到客户端请求，接着进行处理\n");
    printf("line = %d\n", __LINE__);
    printf("size = %d, flag = %d fileSize = %ld\n", lmsg.size, lmsg.flag,lmsg.fileSize);
    printf("filename = %s\n", lmsg.buf);
#endif

           switch(lmsg.flag){
           case GETSCOMMEND://客户端请求下载文件
               lmsg.flag = SUCCESS;
               transmiss(pCur->newFd, &lmsg);
               close(pCur->newFd);
               free(pCur);
               break;
           case PUTSCOMMEND:
               //客户端请求上传文件
               lmsg.flag = SUCCESS;
               recvFile(pCur->newFd, &lmsg);
               break;
           case CDCOMMEND:
               //浏览目录，删除文件，添加文件
               break;
           case RMCOMMEND:
               break;
           case LSCOMMEND:
               break;
           case USERENROLL:
               break;
           case PWDCOMMEND:
               break;
           default:
               break;
           }
       }
    }
}


int factoryStart(pFactory_t pf){
    int i;
    if(!pf->startFlag){
        for(i=0;i<pf->threadNum;i++){
            pthread_create(pf->pthid+i, NULL, threadFun, pf);
        }
        pf->startFlag = 1;
    }
    return 0;
}

void factoryDestroy(pFactory_t pf){
    queDestroy(&pf->que);
    pthread_cond_destroy(&pf->cond);
    free(pf->pthid);
    printf("all is clear\n");
}

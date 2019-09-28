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
    printf("pthread is out\n");
    //pthread_exit(NULL);//清理函数中不要有pthread_exit
}

//这里不熟
void* threadFun(void *p){
    pFactory_t pf = (pFactory_t)p;
    pQue_t pq = &pf->que;
    pNode_t pCur;
    int getSuccess;
    while(1){
       pthread_cleanup_push(threadExitFunc, pf);//防止死锁
       pthread_mutex_lock(&pq->mutex);
       if(!pq->size){
        pthread_cond_wait(&pf->cond, &pq->mutex);
       }
       getSuccess = queGet(pq, &pCur);
       pthread_cleanup_pop(0);
       pthread_mutex_unlock(&pq->mutex);
       if(!getSuccess){
           transport(pCur->newFd);
           free(pCur);
       }
       pCur = NULL;
       pthread_testcancel();
    }
    pthread_exit(NULL);
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
}

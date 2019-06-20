#include "../include/factory.h"



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
    int ret;
    linkMsg_t lmsg;//size_t 8字节
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        pthread_cleanup_push(threadExitFunc, pf);//防止死锁
        if(!pq->size){
            pthread_cond_wait(&pf->cond, &pq->mutex);
        }
        getSuccess = queGet(pq, &pCur);
        pthread_cleanup_pop(1);
        //连接数据库
        //这里不能传一级指针，因为需要改变一级指针的值
        if(!getSuccess){
            MYSQL *conn = NULL;
            connectMYSQL(&conn);
            UserState_t uState;
            bzero(&uState, sizeof(uState));
            while(1){
                //接收客户端的请求
                memset(&lmsg, 0, sizeof(lmsg));
                ret = recvCycle(pCur->newFd, &lmsg, MSGHEADSIZE);//接收大小，标志，文件大小
                if(-1 == ret)
                {
                    goto ERROR_DISCONNECT;
                }
                /*检查token值，如果标志位不为ENROLL,也不为LOGIN，检查数据库中是否有该token
                 * 如果为ENROLL或LOGIN，则继续进行。
                 * 
                 */
                if(USERLOGIN != lmsg.flag && USERENROLL != lmsg.flag)
                {
                    //查询数据库中该用户是否有该token值
                    if(checkToken(conn,lmsg.token,30) == -1){
                        goto ERROR_DISCONNECT;
                    }else{
                        //将用户名和当前目录id赋给子进程
                    }
                }

                ret = recvCycle(pCur->newFd, lmsg.buf, lmsg.size - MSGHEADSIZE);
                if(-1 == ret)
                {
                    goto ERROR_DISCONNECT;
                }


#ifdef DEBUG_SERVER
                printf("\n\n-------factory.c---threadFun------------\n");
                printf("接收到客户端请求，接着进行处理\n");
                printf("line = %d\n", __LINE__);
                printf("size = %d, flag = %d fileSize = %ld\n", lmsg.size, lmsg.flag,lmsg.fileSize);
                printf("lmsg.buf = %s\n\n", lmsg.buf);
#endif

                switch(lmsg.flag)
                {
                case USERENROLL:
                    userEnroll(pCur->newFd,conn, &lmsg);    //用户注册
                    break;
                case USERLOGIN:
                    userLogin(pCur->newFd, conn, &lmsg, &uState);    //用户登录
                    break;
                case GETSCOMMEND://客户端请求下载文件
                    lmsg.flag = SUCCESS;
                    transmiss(pCur->newFd, &lmsg);
                    close(pCur->newFd);
                    free(pCur);
                    break;
                case PUTSCOMMEND:
                    //客户端请求上传文件
                    recvFile(pCur->newFd, conn, &lmsg, &uState);
                    goto ERROR_DISCONNECT;
                    break;
                case CDCOMMEND:
                    //浏览目录，删除文件，添加文件
                    CdCommand(pCur->newFd, conn, &lmsg, &uState);
                    //printf("current dir = %s\n", uState.currentDirId);
                    break;
                case RMCOMMEND:
                    break;
                case LSCOMMEND:
                    LsCommand(pCur->newFd, conn,&uState);
                    break;
                case PWDCOMMEND:
                    PwdCommand(pCur->newFd, conn, &uState);
                    break;
                default:
                    break;
                }//switch(lmsg.flag)

            }//while(1)处理客户端指令
ERROR_DISCONNECT:
            close(pCur->newFd);
            mysql_close(conn);
            printf("user disconnect\n");
        }//if(!getSuccess)处理用户的命令
    }//while(1)线程循环等待
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


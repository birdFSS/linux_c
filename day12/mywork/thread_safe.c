#include <func.h>
typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}threadInfo;

void cleanUpUnlock(void *p){
    threadInfo *pClean = (threadInfo*)p;
    pthread_mutex_unlock(&pClean->mutex);
    printf("unlock! clean\n");
}


void* handler(void* p){
    threadInfo* p1 = (threadInfo*)p;
    time_t tim = time(NULL);
    char *ptime = ctime(&tim);
    printf("I am child the time is %s\n", ptime);
    sleep(1);
    printf("I am child the time is %s\n", ptime);
    pthread_cleanup_push(cleanUpUnlock, p1);
    pthread_mutex_lock(&p1->mutex);
    pthread_cond_wait(&p1->cond, &p1->mutex);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t pthid;
    threadInfo tInfo;
    pthread_mutex_init(&tInfo.mutex,NULL);
    pthread_cond_init(&tInfo.cond, NULL);
    pthread_create(&pthid, NULL, handler, &tInfo);
    //    usleep(1000);//防止没有等待，就发信号了。
    sleep(1);
    time_t tim = time(NULL);
    char *ptime = ctime(&tim);
    printf("I am main the time is %s", ptime);
    //pthread_cancel(pthid);
    sleep(2);
    pthread_cond_signal(&tInfo.cond);
    pthread_join(pthid, NULL);
    pthread_mutex_destroy(&tInfo.mutex);
    return 0;
}


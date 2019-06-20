#include <func.h>

#define N 10000000
static int global = 0;

void* thread_fun(void *p){
    pthread_mutex_t *pmtx = (pthread_mutex_t*)p;
    for(int j=0;j<N;j++){
        pthread_mutex_lock(pmtx);
        global += 1;
        pthread_mutex_unlock(pmtx);
    }
    pthread_exit(NULL);

}

int main(int argc, char* argv[])
{
    pthread_t p1, p2;
    pthread_mutex_t mtx;
    pthread_mutexattr_t mtxAttr;
    pthread_mutexattr_init(&mtxAttr);
    pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK);//设置属性
    pthread_mutex_init(&mtx, &mtxAttr);

    pthread_create(&p1, NULL, thread_fun, &mtx);
    pthread_create(&p2, NULL, thread_fun, &mtx);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    
    printf("global = %d\n", global);
    pthread_mutex_destroy(&mtx);
    pthread_mutexattr_destroy(&mtxAttr);
    return 0;
}


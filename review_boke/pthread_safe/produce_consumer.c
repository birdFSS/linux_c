#include <func.h>

static int avail = 0;

//生产者
void* thread_fun(void *p){
    pthread_mutex_t* pmtx = (pthread_mutex_t*)p;
    int n=3;//次数
    while(n-->0){
        pthread_mutex_lock(pmtx);
        avail+=10;
        printf("生产中ing, 新产品数为%d\n", avail);
        pthread_mutex_unlock(pmtx);
        usleep(400);
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    pthread_t produce;
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    pthread_create(&produce, NULL, thread_fun, &mtx);
    //消费者
    while(1){
        pthread_mutex_lock(&mtx);
        while(avail > 0){
            avail--;
            printf("销耗中，当前剩余产品：%d\n", avail);
            usleep(200);
        }
        pthread_mutex_unlock(&mtx);
    }
    pthread_join(produce, NULL);
    pthread_mutex_destroy(&mtx);
    return 0;
}


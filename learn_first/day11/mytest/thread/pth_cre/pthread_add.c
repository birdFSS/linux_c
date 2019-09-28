#include <func.h>
#define N 40000000
int global = 0;

void* threadHandler(void *arg){
    pthread_mutex_t mutex = *(pthread_mutex_t*)arg;
    while(1){
        pthread_mutex_lock(&mutex);
        if(global < N){
            global += 1;
            pthread_mutex_unlock(&mutex);
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t pthid;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&pthid, NULL, threadHandler, &mutex);
    while(1){
        pthread_mutex_lock(&mutex);
        if(global<N){
            global+=1;
            pthread_mutex_unlock(&mutex);
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    pthread_join(pthid,NULL);
    printf("all = %d\n", global);
    return 0;
}


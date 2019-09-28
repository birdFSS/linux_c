#include <func.h>

void clean_thread(void* args){
    free(args);
    printf("clean over\n");
}


void* threadFunc(void* args)
{
    char* p = (char*)malloc(20);
    pthread_cleanup_push(clean_thread,p);
    ssize_t len = read(STDIN_FILENO, p, 19);
    if(len == 0){
        perror("read");
        pthread_exit((void**)-1);
    }
    p[len] = '\0';
    printf("I am child thread, p = %s\n", p);
    pthread_exit((void**)2);
    pthread_cleanup_pop(0);
}


int main(int argc, char* argv[])
{
    pthread_t pthid;
    int ret;
    ret = pthread_create(&pthid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    printf("I am main thread, mythread id = %ld\n", pthread_self());
    long result;
    sleep(1);
    pthread_cancel(pthid);
    pthread_join(pthid, (void**)&result);
    printf("i am parent , ret = %ld\n", result);
    return 0;
}


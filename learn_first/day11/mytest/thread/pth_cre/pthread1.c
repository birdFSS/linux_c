#include <func.h>




void* threadFunc(void* p)
{
    printf("I am child thread, p = %ld\n", (long)p);
    pthread_exit((void**)2);
}


int main(int argc, char* argv[])
{
    pthread_t pthid;
    int ret;
    ret = pthread_create(&pthid, NULL, threadFunc, (void*)1);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    printf("I am main thread, mythread id = %ld\n", pthread_self());
    long result;
    pthread_join(pthid, (void**)&result);
    printf("i am parent , ret = %ld\n", result);
    return 0;
}


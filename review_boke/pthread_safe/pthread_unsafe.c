#include <func.h>

static int global = 0;

void* thread_fun(void *p){
    int loops = *(int*)p;
    int loc, j;
    for(j=0;j<loops;j++){
        loc = global;
        loc++;
        global = loc;
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);//检查是否传入参数
    int loops = atoi(argv[1]);//由传参决定循环次数
    pthread_t pthid1, pthid2;
    pthread_create(&pthid1, NULL, thread_fun, &loops);
    pthread_create(&pthid2, NULL, thread_fun, &loops);
    pthread_join(pthid1, NULL);
    pthread_join(pthid2, NULL);
    printf("global = %d\n", global);
    exit(EXIT_SUCCESS);
    return 0;
}


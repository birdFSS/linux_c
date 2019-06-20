#include <func.h>

int main(int argc, char* argv[])
{
    struct sigaction sg, sTest;
    sigemptyset(&sg.sa_mask);
    sigaddset(&sg.sa_mask, SIGINT);
    sigprocmask(SIG_BLOCK, &sg.sa_mask, NULL);
    printf("begin to pend signal 2\n");
    sleep(5);
    sigpending(&sTest.sa_mask);
    if(sigismember(&sTest.sa_mask, SIGINT)){
        printf("signal 2 is pending\n");
    }else{
        printf("signal 2 is not pending\n");
    }
    sigprocmask(SIG_UNBLOCK, &sg.sa_mask, NULL);

    return 0;
}


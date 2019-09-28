#include <func.h>
void sigHandler(int iSigNum, siginfo_t *pSigInfo, void *pReserve)
{
    struct sigaction sg;
    printf("signal number is %d\n", iSigNum);
    sleep(5);
    sigpending(&sg.sa_mask);
    if(sigismember(&sg.sa_mask, SIGQUIT)){
        printf("signal 3 exict\n");
    }else{
        printf("signal 3 doesn't exict\n");
    }
    printf("Leave from signal %d\n", iSigNum);
}

int main(int argc, char* argv[])
{
    struct sigaction sig1;
    sig1.sa_flags = SA_NODEFER | SA_SIGINFO;
    sig1.sa_sigaction = sigHandler;
    sigemptyset(&sig1.sa_mask);
    sigaddset(&sig1.sa_mask, SIGQUIT);
    sigaction(SIGINT, &sig1, NULL);
    while(1)
        sleep(1);
    return 0;
}


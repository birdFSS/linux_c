#include <func.h>
void sigHandler(int iSigNum, siginfo_t *pSigInfo, void *pReserve)
{
    printf("signal number is %d\n", iSigNum);
    sleep(1);
    printf("Leave from signal %d\n", iSigNum);
}

int main(int argc, char* argv[])
{
    struct sigaction sig1;
    sig1.sa_flags = SA_NODEFER | SA_SIGINFO;
    sig1.sa_sigaction = sigHandler;
    sigaction(SIGINT, &sig1, NULL);
    while(1)
        sleep(1);
    return 0;
}


#include <func.h>
int g_iSeq = 0;

void SignHandler(int iSignNo, siginfo_t *pInfo, void *pReserve){
    int iSeq = g_iSeq++;
    printf("%d Enter SignHandler, sigNo = %d\n", iSeq, iSignNo);
    sleep(3);
    printf("%d Leave SignHandler, sigNo = %d\n", iSeq, iSignNo);

}
int main(int argc, char* argv[])
{
    char szBuf[128];
    int iRet;
    struct sigaction act;
    act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = SignHandler;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    do{
        iRet = read(STDIN_FILENO, szBuf, sizeof(szBuf) - 1);
        if(iRet < 0){
            perror("read fail.");
            break;
        }
        szBuf[iRet] = '\0';
        printf("GET : %s\n", szBuf);
    }while(strcmp(szBuf, "quit\n") != 0);
    return 0;
}


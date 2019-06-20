#include <func.h>
void sigHandler(int iSigNo){
    printf("Capture signal number is :%d\n", iSigNo);
    sleep(2);
    printf("Leave sigHandler %d\n", iSigNo);
}
int main(int argc, char* argv[])
{
    signal(SIGINT, sigHandler);
    signal(SIGQUIT, sigHandler);
    while(1)
        sleep(1);
    return 0;
}


#include <func.h>
void SignHandler(int iSignNo){
    printf("Capture sign no:%d\n", iSignNo);
}
int main(int argc, char* argv[])
{
    signal(SIGINT, SignHandler);
    while(1)
        sleep(1);
    return 0;
}


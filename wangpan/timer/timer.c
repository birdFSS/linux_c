#define _POSIX_C_SOURCE 199309
#include <func.h>
/*目标完成定时器，在信号函数中每秒打印一个数字
 */ 

#define TIMER_SIG SIGRTMAX
int main(int argc, char* argv[])
{
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent sev;
    timer_t *tidlist;
    int j;

    tidlist = (timer_t*)calloc(argc-1,sizeof(timer_t));
    ERROR_CHECK(tidlist, NULL, "calloc");

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    int ret = sigaction(TIMER_SIG, &sa, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMER_SIG;
    for(j=0; j<argc-1;j++)
    {
        itimerspecFromStr(argv[j+1],&ts);
        sev.sigev_value.sival_ptr = &tidlist[j];

        if(timer_create(CLOCK_REALTIME,&sev,&tidlist[j]) == -1)
        {
            perror("timer_create");
            return -1;
        }
        
    }
    return 0;
}


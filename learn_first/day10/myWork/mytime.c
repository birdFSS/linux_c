#include <func.h>
#define N 1000000000
long t_real = 0, t_vt = 0, t_pr = 0;
void sigFunc(int sigNum){
    if(sigNum == SIGALRM){
        t_real += 1;
    }else if(sigNum == SIGVTALRM){
        t_vt += 1;
    }else if(sigNum == SIGPROF){
        t_pr += 1;
    }
}

int main(int argc, char* argv[])
{
    signal(SIGALRM, sigFunc);
    signal(SIGVTALRM, sigFunc);
    signal(SIGPROF, sigFunc);
    struct itimerval tm;
    tm.it_interval.tv_sec = 0;
    tm.it_interval.tv_usec = 1000;
    
    tm.it_value.tv_sec = 0;
    tm.it_value.tv_usec = 1000;
    
    int ret = setitimer(ITIMER_REAL, &tm, NULL);
    ERROR_CHECK(ret, -1, "setitimer");
    ret = setitimer(ITIMER_VIRTUAL, &tm, NULL);
    ERROR_CHECK(ret, -1, "setitimer");
    ret = setitimer(ITIMER_PROF, &tm, NULL);
    ERROR_CHECK(ret, -1, "setitimer");
    sleep(10);
    int i=0;
    while(1){
        if(i>N){
            break;
        }
        i++;
    }
    printf("real time = %ld ms\n, virtual time = %ld ms\n, prof time = %ld ms\n",t_real, t_vt, t_pr);
    return 0;
}


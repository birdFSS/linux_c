/* welcome to world
 * file : .c
 * brief : 
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

/*************************** INCLUDE *********************************************/
#include "curr_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*************************** DEFINE **********************************************/
#define errMsg(fmt, args...) do{ \
    fprintf(stderr, "[%s:%s:%d]" fmt, __FILE__, __FUNCTION__, __LINE__, ##args); \
}while(0)

/*************************** LOCAL STRUCT ****************************************/
static volatile int numLiveChildren = 0;

/*************************** TYPEDEF *********************************************/

/*************************** LOCAL FUNCTION **************************************/
static void printWaitStatus(void* p, int status)
{
    printf("status=%d\n", status);
}

static void sigchildHandler(int sig)
{
    int status, savedErrno;
    pid_t childPid;

    savedErrno = errno;
    printf("%s handler: Caught SIGCHLD\n", currTime("%T"));

    while((childPid = waitpid(-1, &status, WNOHANG)) > 0 )
    {
        printf("%s handler: Reaped child %ld - ", currTime("%T"), (long)childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
    }

    if(-1 == childPid && ECHILD != errno)
    {
        errMsg("waitpid");
    }
    sleep(5);
    printf("%s handler: returning\n", currTime("%T"));

    errno = savedErrno;
}
/*************************** GLOBAL FUNCTION *************************************/

int main(int argc, char** argv)
{
    int j, sigCnt, ret;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;
    char buf[1024] = {0};
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        errMsg("%s child-sleep-time...\n", argv[0]);
        return 1;
    }


    setbuf(stdout, NULL);

    sigCnt = 0;
    numLiveChildren = argc - 1;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchildHandler;
    if(-1 == sigaction(SIGCHLD, &sa, NULL))
    {
        errMsg("sigaction");
        return 1;
    }

#if 0
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if(sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
    {
        errMsg("sigprocmask");
        return 1;
    }
#endif

    for(j = 1; j < argc; j++)
    {
        switch(fork())
        {
            case -1:
                errMsg("fork");
                exit(1);
            case 0: // Child
                sleep(5);
                printf("%s Child %d (PID=%ld) exiting\n", currTime("%T"), j, (long)getpid());
                _exit(EXIT_SUCCESS);
                break;
            default:
                printf("%s Parent start to read something\n", currTime("%T"));
                ret = fread(buf, 1, 1024, stdin);
                printf("%s Parent end of read, ret = %d, errno=%d\n", currTime("%T"), ret, errno);
                break;
        }
    }

    sigemptyset(&emptyMask);
    while(numLiveChildren > 0)
    {
        if(sigsuspend(&emptyMask) == -1 && EINTR != errno)
        {
            errMsg("sigsuspend");
            return 1;
        }
        sigCnt++;
    }
    printf("%s ALL %d Children have terminated; SIGCHLD was caught "
           "%d times\n", currTime("%T"), argc - 1, sigCnt);

    exit(EXIT_SUCCESS);
    return 0;
}


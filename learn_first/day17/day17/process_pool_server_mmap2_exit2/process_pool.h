#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <strings.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#define ARGS_CHECK(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {printf("errno=%d\n",errno);perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return -1;}}

typedef struct{
    pid_t pid;
    int fds;//全双工管道
    short busy;//是否忙碌，0就是非忙碌，1就是忙碌
}ProcessData_t,*pProcessData_t;
typedef struct{
    int dataLen;//代表后面存储的数据长度
    char buf[1000];
}train_t;
#define FILENAME "file"
int makeChild(pProcessData_t,int);
int childHandle(int);
int tcpInit(int *,char*,char*);
int sendFd(int,int,int);
int recvFd(int,int*,int*);
int tranFile(int);
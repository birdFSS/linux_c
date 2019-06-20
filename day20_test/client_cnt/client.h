#define _GNU_SOURCE
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
#include <fcntl.h>
#include <aio.h>
#define ARGS_CHECK(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {printf("errno=%d\n",errno);perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return -1;}}

typedef struct linkMsg{
    int size;//总长度
    int flag;//标志位 读写浏览
    size_t fileSize;
    char* buf;//文件名，文件大小，接收目录
}linkMsg_t;


int aioWr(int toFd, char* buf, size_t length);
int tcp_client(char* ip, char* port);
int download(int socketFd);
int recvCycle(int sfd, void* buf, int fileSize);

#include "process_pool.h"

int childFunc(int pipeFd){
    int newFd;
    while(1){
        recvFd(pipeFd, &newFd);
        printf("I am child, get task %d\n", newFd);
        send(newFd, "file", 4, 0);
        close(newFd);
        write(pipeFd, &newFd, 1);
    }
}

int makeChild(pProcessData_t p, int childNum){
    int i,ret;
    int fds[2];
    pid_t pid;
    for(i=0;i<childNum;i++){
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
        pid = fork();
        if(0 == pid){
            close(fds[0]);
            childFunc(fds[1]);
        }
        close(fds[1]);
        p[i].pid = pid;
        p[i].fds = fds[0];
        p[i].busy= 0;
    }
    return 0;
}



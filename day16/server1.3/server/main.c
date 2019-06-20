#include "process_pool.h"

int epollAdd(int epfd, int fd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd= fd;
    int ret;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 4);
    int ret;
    int processNum = atoi(argv[3]);
    pProcessData_t pData;
    pData = (pProcessData_t)calloc(processNum, sizeof(ProcessData_t));
    ret = makeChild(pData, processNum);
    ERROR_CHECK(ret, -1, "makeChild");
    int socketFd;
    ret = tcpInit(&socketFd, argv[1], argv[2]);
    ERROR_CHECK(ret , -1, "tcpInit");
    int epfd = epoll_create(1);
    struct epoll_event *evs;
    evs = (struct epoll_event*)calloc(processNum+1, sizeof(struct epoll_event));
    epollAdd(epfd, socketFd);
    int i;
    for(i=0;i<processNum;i++){
        epollAdd(epfd, pData[i].fds);
    }
    char flag;
    int readyFdCnt,j, newFd;
    while(1){
        readyFdCnt = epoll_wait(epfd, evs, processNum+1, -1);
        for(i=0;i<readyFdCnt;i++){
            if(evs[i].data.fd == socketFd){
                newFd = accept(socketFd, NULL, NULL);
                for(j=0;j<processNum;j++){
                    if(!pData[j].busy){
                        sendFd(pData[j].fds, newFd);
                        pData[j].busy = 1;
                        printf("pid %d is busy\n", pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }

            for(j=0;j<processNum;j++){
                if(evs[i].data.fd == pData[j].fds){
                    read(pData[j].fds, &flag, 1);
                    pData[j].busy = 0;
                    printf("%d : I am free now \n", pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}


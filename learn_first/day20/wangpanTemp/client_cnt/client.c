#include "client.h"

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int ret,socketFd;
    socketFd = tcp_client(argv[1], argv[2]);
    ERROR_CHECK(socketFd, -1, "tcp_client");
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    struct epoll_event evs[2];
    epollAdd(epfd,socketFd);
    epollAdd(epfd, STDIN_FILENO);
    int readyNumOfFd;
    int i, commend;
    char bufTemp[STDIN_MAX];
    while(1){
        readyNumOfFd = epoll_wait(epfd, evs, 2, -1);
        for(i=0;i<readyNumOfFd;i++){
            if(evs[i].data.fd == socketFd){
                //服务器发信息，更新，活动
            }
            if(evs[i].data.fd == STDIN_FILENO){
                //这里思考一下
                commend = getCommendFromStdin(bufTemp);
                switch(commend){
                case 1:
                    //cd
                    break;
                case 2:
                    //ls
                    break;
                case 3:
                    //puts
                    break;
                case 4:
                    //gets  需要传送文件名，结构体
                    ret = download(socketFd);
                    if(-1 == ret){
                        goto END;
                    }
                    break;
                case 5:
                    //rm
                    break;
                case 6:
                    //pwd
                    break;
                default:
                    break;
                }

            }
        }
    }
END:
    close(socketFd);
    return 0;
}

int epollAdd(int epfd, int fd){                                                          
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}


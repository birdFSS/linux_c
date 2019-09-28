#include <func.h>
int server_init(char *ip, char *port){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int ret;
    ret = bind(socketFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketFd, 10);
    ERROR_CHECK(ret, -1, "listen");
    return socketFd;
}

void setnonblocking(int fd){
    int status = fcntl(fd, F_GETFL);
    status = status| O_NONBLOCK;
    fcntl(fd, F_SETFL, status);
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int ret, new_fd, readyNum;
    int socketFd = server_init(argv[1], argv[2]);
    if(-1 == socketFd){
        goto end1;
    }
    socklen_t addlen;
    struct sockaddr_in clientaddr;
    char buf[512] = {0};
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    struct epoll_event event, evs[3];
    event.events = EPOLLIN|EPOLLET;
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    event.data.fd = socketFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, socketFd, &event);
    //int size = 5;
    //setsockopt(new_fd, SOL_SOCKET, SO_RCVLOWAT, &size, sizeof(int));
    int i;
    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
                printf("test%d\n", __LINE__);
        for(i=0;i<readyNum;i++){
            if(socketFd == evs[i].data.fd){
                printf("test%d\n", __LINE__);
                memset(&clientaddr, 0, sizeof(clientaddr));
                addlen = sizeof(clientaddr);
                new_fd = accept(socketFd, (struct sockaddr*)&clientaddr, &addlen);
                ERROR_CHECK(new_fd, -1, "accept");
                //setnonblocking(new_fd);
                printf("client ip = %s, client port = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                printf("test%d\n", __LINE__);
                event.data.fd=new_fd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &event);
            }
            if(new_fd == evs[i].data.fd){
                memset(buf, 0, sizeof(buf));
                printf("test%d\n", __LINE__);
                //ret = recv(new_fd, buf, sizeof(buf), 0);
                ret = recv(new_fd, buf, 5, 0);
                if(0 == ret){
                    printf("goodbye!\n");
                    event.events = EPOLLIN;
                    event.data.fd = new_fd;
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, new_fd, &event);
                    ERROR_CHECK(ret, -1, "epoll_ctl");
                    close(new_fd);
                    break;
                }
                printf("%s\n", buf);
            }
            if(STDIN_FILENO == evs[i].data.fd){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf)-1);
                if(0==ret){
                    printf("byebye\n");
                    goto end1;
                }
                send(new_fd, buf, strlen(buf)-1, 0);
            }
        }
    }    
end1:
    close(new_fd);
    close(socketFd);
    return 0;
}


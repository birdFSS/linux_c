#include <func.h>
int server_init(char *ip, char *port){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    int ret;
    ret = bind(socketFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketFd, 10);
    ERROR_CHECK(ret, -1, "listen");
    return socketFd;
}


int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGINT);
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
    fd_set rdset, needMonitorSet;
    int ret, new_fd, readyNum;
    socklen_t addlen;
    struct sockaddr_in clientaddr;
    char buf[512] = {0};
    int socketFd = server_init(argv[1], argv[2]);
    if(-1 == socketFd){
        goto end1;
    }
    FD_ZERO(&needMonitorSet);//这里缺少了select工作不正常
    FD_SET(STDIN_FILENO, &needMonitorSet);
    FD_SET(socketFd, &needMonitorSet);
    while(1){
        FD_ZERO(&rdset);
        memcpy(&rdset, &needMonitorSet, sizeof(fd_set));
        readyNum = select(15, &rdset, NULL, NULL, NULL);
        // printf("ret = %d", ret);
        //printf("readyNum = %d\n", readyNum);
        if(readyNum>0){
            if(FD_ISSET(socketFd, &rdset)){
                memset(&clientaddr, 0, sizeof(clientaddr));
                addlen = sizeof(clientaddr);
                new_fd = accept(socketFd, (struct sockaddr*)&clientaddr, &addlen);
                ERROR_CHECK(new_fd, -1, "accept");
                int recvLowVal = 20;
                setsockopt(new_fd, SOL_SOCKET, SO_RCVLOWAT , &recvLowVal, sizeof(int));
                printf("client ip = %s, client port = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                FD_SET(new_fd, &needMonitorSet);
            }
            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf)-1);
                if(0 == ret){
                    break;
                }
                if(!strcmp(buf, "ENDSER\n")){
                    printf("close the server, goodbye!\n");
                    break;
                }
                send(new_fd, buf, strlen(buf)-1, 0);
            }

            if(FD_ISSET(new_fd, &rdset)){
                memset(buf, 0, sizeof(buf));
                ret = recv(new_fd, buf, sizeof(buf), 0);
                if(-1 == ret){
                    perror("recv");
                    close(new_fd);
                    close(socketFd);
                }else if(0 == ret){
                    close(new_fd);
                    FD_CLR(new_fd, &needMonitorSet);
                }else{
                    printf("%s\n", buf);
                }
            }
        }
    }    
end1:
    close(socketFd);
    return 0;
}


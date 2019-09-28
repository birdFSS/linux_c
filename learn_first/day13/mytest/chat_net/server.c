#include <func.h>
int glo_error = 0;



int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGINT);
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
    int socketFd;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret;
    ret = bind(socketFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketFd, 5);
    ERROR_CHECK(ret, -1, "listen");
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    socklen_t addlen = sizeof(clientaddr);
    int new_fd = accept(socketFd, (struct sockaddr*)&clientaddr, &addlen);
    ERROR_CHECK(new_fd, -1, "accept");
    printf("client ip = %s, client port = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    fd_set rdset;
    struct timeval tv;
    char buf[512] = {0};
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(new_fd, &rdset);
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        ret = select(new_fd+1, &rdset, NULL, NULL, &tv);
        // printf("ret = %d", ret);
        if(ret == -1){
            perror("select");
            return -1;
        }
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf)-1);
            if(0 == ret){
                break;
            }
            if(!strcmp(buf, "ENDSERVER\n")){
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
                break;
            }
            printf("%s\n", buf);
        }
    }
    close(new_fd);
    close(socketFd);
    return 0;
}


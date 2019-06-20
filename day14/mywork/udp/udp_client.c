#include <func.h>
int client_init(char *ip, char* port, struct sockaddr_in *addr){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    int ret;
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    ret = connect(socketFd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");
    *addr = serAddr;
    return socketFd;
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    struct sockaddr_in serAddr;
    int servFd = client_init(argv[1], argv[2], &serAddr);
    if(-1 == servFd){
        goto end;
    }
    fd_set rdset;
    char buf[128] = {0};
    //bzero(&serAddr, sizeof(serAddr));
    int readyNum; 
    sendto(servFd, "hello", 5, 0, (struct sockaddr*)&serAddr, sizeof(serAddr));
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(servFd, &rdset);
        readyNum = select(servFd+1, &rdset, NULL, NULL, NULL);
        if(readyNum>0){
            if(FD_ISSET(servFd, &rdset)){
                memset(buf, 0, sizeof(buf));
                recvfrom(servFd, buf, sizeof(buf), 0, NULL,NULL);
                printf("%s\n", buf);
            }

            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                sendto(servFd, buf, strlen(buf)-1, 0, (struct sockaddr*)&serAddr, sizeof(serAddr));
            }

        }
    }
end:
    close(servFd);
    return 0;
}


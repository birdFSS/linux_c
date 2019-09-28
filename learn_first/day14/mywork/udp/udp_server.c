#include <func.h>

int clientver_init(char *ip, char* port){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    int ret;
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(atoi(port));
    clientAddr.sin_addr.s_addr = inet_addr(ip);
    ret = bind(socketFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    ERROR_CHECK(ret, -1, "bind");
    return socketFd;
}



int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockFd = clientver_init(argv[1], argv[2]);
    if(sockFd == -1){
        goto end;
    }
    fd_set rdset;
    int readyNum;
    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    socklen_t len = sizeof(clientAddr);
    char buf[128];
    recvfrom(sockFd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, &len);
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockFd, &rdset);
        readyNum = select(15, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(sockFd, &rdset)){
            memset(buf, 0, sizeof(buf));
            recvfrom(sockFd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, &len);
            printf("%s\n", buf);
        }
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf)-1);
            sendto(sockFd, buf, strlen(buf)-1, 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        }
    }
    close(sockFd);
end:
    return 0;
}


#include "process_pool.h"

int tcpInit(int *sFd, char *ip, char* port){
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");
    int ret, reuse = 1;
    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    ERROR_CHECK(ret , -1, "setsockopt");
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    ret = bind(socketFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketFd, 10);
    ERROR_CHECK(ret, -1, "listen");
    *sFd = socketFd;
    return 0;
}

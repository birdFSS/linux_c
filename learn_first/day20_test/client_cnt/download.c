#include "client.h"


int download(int socketFd){
    //接收文件名
    char filename[128]={0};
    int dataLen = 0, ret;
    recvCycle(socketFd, &dataLen, 4);
    recvCycle(socketFd, filename, dataLen);
    int newFileFd = open(filename, O_CREAT| O_RDWR, 0666);
    ERROR_CHECK(newFileFd, -1, "open");
    //接收文件大小
    off_t fileSize;
    recvCycle(socketFd, &dataLen, 4);
    recvCycle(socketFd, &fileSize, dataLen);
    ftruncate(newFileFd, fileSize);
    char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE| PROT_READ, MAP_SHARED, newFileFd, 0);
    ERROR_CHECK(pMap, (char*)-1, "mmap");
    recvCycle(socketFd, pMap, fileSize);
    ret = munmap(pMap, fileSize);
    ERROR_CHECK(ret, -1, "munmap");
    //printf("100%%\n");
    close(newFileFd);
    return 0;
}

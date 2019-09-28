#include "factory.h"

int recvCycle(int fd, char* buf, long fileSize){
    long total;
    int ret;
    total = 0;
    while(total < fileSize){
        ret = recv(fd, buf+total, fileSize-total, 0);
        if(0 == ret){
            printf("connect close!\n");
            return -1;
        }
    }
    return 0;
}

int transport(int tranFd){
    int dataLen;
    char fileName[128] = "file";
    int ret;
    int fd = open(fileName, O_RDONLY);//只读就可以了，不需要修改
    ERROR_CHECK(fd, -1, "open");
    //send fileName
    dataLen = strlen(fileName);
    send(tranFd, &dataLen, 4, 0);
    send(tranFd, fileName, dataLen, 0);
    //send fileSize
    struct stat fileStat;
    ret = fstat(fd, &fileStat);
    ERROR_CHECK(ret, -1, "fstat");
    dataLen = sizeof(fileStat.st_size);
    send(tranFd, &dataLen, 4, 0);
    send(tranFd, &fileStat.st_size, dataLen, 0);
    //mmap文件映射
    char *pMap = (char*)mmap(NULL, fileStat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(pMap, (char*)-1, "mmap");
    //计时
    struct timeval start, end;
    gettimeofday(&start, NULL);
    //发送文件
    send(tranFd, pMap, fileStat.st_size, 0);
    ret = munmap(pMap, fileStat.st_size);
    ERROR_CHECK(ret, -1, "munmap");
    gettimeofday(&end, NULL);
    printf("use time is %ld\n", end.tv_usec-start.tv_usec+(end.tv_sec-start.tv_sec)*1000000);
    close(fd);
    close(tranFd);
    return 0;
}

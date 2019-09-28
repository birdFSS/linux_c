#include "process_pool.h"
int tranFun(int newFd){
    train_t data;
    //发送文件名
    char fileName[100] = "file";
    memset(&data, 0, sizeof(train_t));
    strcpy(data.buf, fileName);//文件名
    data.dataLen = strlen(data.buf);
    send(newFd, &data, data.dataLen + 4,0);
    int fd = open(fileName, O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    //发送文件大小
    struct stat fileStat;
    memset(&fileStat, 0, sizeof(struct stat));
    fstat(fd, &fileStat);
    data.dataLen = sizeof(fileStat.st_size);
    memcpy(data.buf, &fileStat.st_size, data.dataLen);
    send(newFd, &data, 4+data.dataLen, 0);
    //打开并发送文件
    char *pMap = (char *)mmap(NULL, fileStat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(pMap, (char*)-1, "mmap");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    send(newFd, pMap, fileStat.st_size, 0);//发送文件
    //send(newFd, &data, 4+data.dataLen, 0);//告诉客户端文件发送结束
    gettimeofday(&end, NULL);
    printf("the cost of time is %ld\n", end.tv_usec - start.tv_usec + (end.tv_sec - start.tv_sec)*1000000);
    close(fd);
    close(newFd);
    return 0;
}

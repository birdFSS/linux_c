#include <func.h>

int recvCycle(int , void*, int);

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd;
    socketFd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    printf("socketFd=%d\n",socketFd);
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret;
    ret=connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");
    //接收文件名
    char filename[128]={0};
    int dataLen = 0;
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
    close(socketFd);
    return 0;
}

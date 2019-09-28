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
    printf("fileSize=%ld\n", fileSize);
    ftruncate(newFileFd, fileSize);
    lseek(newFileFd, 0, SEEK_SET);
    int fds[2];
    pipe(fds);
    while(1){
        ret = splice(socketFd, NULL, fds[1], NULL, 8192, SPLICE_F_MOVE| SPLICE_F_MORE);
        if(0 == ret){
            break;
        }
        splice(fds[0], NULL, newFileFd, NULL, ret, SPLICE_F_MOVE | SPLICE_F_MORE);
    }
    printf("100.00%%\n");
    close(newFileFd);
    close(socketFd);
    return 0;
}

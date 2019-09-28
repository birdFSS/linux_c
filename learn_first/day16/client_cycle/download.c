#include <func.h>

int recvCycle(int ,void* , int );

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

    char filename[128]={0};
    int dataLen = 0;
    recvCycle(socketFd, &dataLen, 4);
    recvCycle(socketFd, filename, dataLen);
    int newFileFd = open(filename, O_CREAT| O_WRONLY | O_TRUNC, 0666);
    ERROR_CHECK(newFileFd, -1, "open");

    //recv the file size
    off_t fileSize = 0;
    recvCycle(socketFd, &dataLen, 4);
    recvCycle(socketFd, &fileSize, dataLen);
    printf("the size of file is %ld\n", fileSize);

    char buf[1024] = {0};
    while(1)
    {
        recvCycle(socketFd, &dataLen, 4);
        //printf("recvCycle datasize = %d\n", dataLen);
        //sleep(1);
        if(dataLen > 0){
            recvCycle(socketFd, &buf, dataLen);
            write(newFileFd, buf, dataLen);
        }else{
            sleep(1);
            printf("download success\n");
            break;
        }
    }
    close(newFileFd);
    close(socketFd);
    return 0;
}

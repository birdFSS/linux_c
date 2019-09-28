#include "client.h"

int download(int socketFd){
    //接收文件名
    char filename[128]="file";
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    strcpy(lmsg.buf,filename);
    lmsg.flag = 0;
    lmsg.size = 16 + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);
    /*传送不再一个地址，所以后面跟着就不对了。
     *lmsg.buf 指向别的地方，不能直接这样传 
     *将lmsg.buf改为字符数组，创建的时候占用一定空间
     *lmsg.buf不用取地址，它本身就是地址 
     */
#ifdef TEST_DOWNLOAD
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("filename = %s\n", lmsg.buf);
#endif
    int ret;
    //接收文件大小
    recvCycle(socketFd, &lmsg, 16);
#ifdef TEST_DOWNLOAD
    printf("----------------download.c 1-----------------\n");
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("%s\n", lmsg.buf);
#endif
    if(1 == lmsg.flag){
        int newFileFd = open(filename, O_CREAT| O_RDWR, 0666);
        ERROR_CHECK(newFileFd, -1, "open");
        recvCycle(socketFd, &lmsg.buf, lmsg.size - 16);
        ftruncate(newFileFd, lmsg.fileSize);
        char *pMap = (char*)mmap(NULL, lmsg.fileSize, PROT_WRITE| PROT_READ, MAP_SHARED, newFileFd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        recvCycle(socketFd, pMap, lmsg.fileSize);
        ret = munmap(pMap, lmsg.fileSize);
        ERROR_CHECK(ret, -1, "munmap");
        close(newFileFd);
    }else{
        printf("download fail\n");
    }
    //printf("100%%\n");
    return 0;
}

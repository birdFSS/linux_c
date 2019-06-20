#include "client.h"

int download(int socketFd, char* filename){
    /*传送文件名*/
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    strcpy(lmsg.buf,filename);
    lmsg.flag = GETSCOMMEND;
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);

    /*传送不再一个地址，所以后面跟着就不对了。
     *lmsg.buf 指向别的地方，不能直接这样传 
     *将lmsg.buf改为字符数组，创建的时候占用一定空间
     *lmsg.buf不用取地址，它本身就是地址 
     */

#ifdef TEST_DOWNLOAD
    printf("\n\n------------------------download.c 1------------\n");
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("filename = %s\n", lmsg.buf);
#endif

    int ret;
    //接收文件大小
    recvCycle(socketFd, &lmsg, MSGHEADSIZE);

#ifdef TEST_DOWNLOAD
    printf("\n\n----------------download.c 1-----------------\n");
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("%s\n", lmsg.buf);
#endif

    if(SUCCESS == lmsg.flag){
        int newFileFd = open(filename, O_CREAT| O_RDWR, 0666);
        ERROR_CHECK(newFileFd, -1, "open");
        recvCycle(socketFd, &lmsg.buf, lmsg.size - MSGHEADSIZE);
        ftruncate(newFileFd, lmsg.fileSize);
        char *pMap = (char*)mmap(NULL, lmsg.fileSize, PROT_WRITE| PROT_READ, MAP_SHARED, newFileFd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        recvCycle(socketFd, pMap, lmsg.fileSize);
        ret = munmap(pMap, lmsg.fileSize);
        ERROR_CHECK(ret, -1, "munmap");
        close(newFileFd);
    }else{
        printf("download refuse\n");
        printf("%s\n", lmsg.buf);       //服务器拒绝，并返回拒绝信息   
    }
    //printf("100%%\n");
    return 0;
}

int upload(int socketFd, char* filename){
    int ret;
    int sendFd = open(filename, O_RDONLY);
    ERROR_CHECK(sendFd, -1, "open");
    struct stat fileStat;
    ret = fstat(sendFd, &fileStat);
    ERROR_CHECK(ret, -1, "fstat");

    /*传送文件名,文件大小，文件属性
     * 文件属性后面再做,不确定需要吗
     * 
     * 
     */
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    strcpy(lmsg.buf,filename);
    lmsg.flag = PUTSCOMMEND;
    lmsg.fileSize = fileStat.st_size;
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);

#ifdef TEST_DOWNLOAD
    printf("\n\n---------------upload.c line = %d------------\n", __LINE__);
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("filename = %s\n", lmsg.buf);
#endif

    //接收服务器返回消息
    ret = recvCycle(socketFd, &lmsg, MSGHEADSIZE);
    if(SUCCESS == lmsg.flag){
        char* pMap = (char*)mmap(NULL, lmsg.fileSize, PROT_READ, MAP_SHARED, sendFd, 0);
        send(socketFd, pMap, lmsg.fileSize, 0);
        ret = munmap(pMap, lmsg.fileSize);
        ERROR_CHECK(ret, -1, "munmap");
    }else{
        recvCycle(socketFd, lmsg.buf, lmsg.size - MSGHEADSIZE);  //接收错误信息
        printf("upload refuse\n");
        printf("%s\n", lmsg.buf);
    }
    close(sendFd);
    return 0;
}

int simpleCommend(int socketFd, char* dirName, int command, void (*print)(char* Info))
{
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    lmsg.flag = command;
    strcpy(lmsg.buf, dirName);
    strcpy(lmsg.token, TOKEN);
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);
#ifdef ERROR_DEBUG
    printf("\n\nsend information\n");
    printf("lmsg.buf = %s##\n", lmsg.buf);
    printf("send lsmg.size = %d\n", lmsg.size);
#endif
    //接收返回信息
    int ret;
    bzero(lmsg.buf, strlen(lmsg.buf));
    ret = recvCycle(socketFd, &lmsg, MSGHEADSIZE);
    if(-1 == ret)
    {
        return -1;
    }
    ret = recvCycle(socketFd, lmsg.buf, lmsg.size - MSGHEADSIZE);
    if(-1 == ret)
    {
        return -1;
    }
    if(SUCCESS == lmsg.flag)
    {
        print(lmsg.buf);
    }else{
        printf("error:%s\n", lmsg.buf);
    }
    return 0;
}

int simpleCommendNoParameter(int socketFd, int command, void (*print)(char* Info))
{
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    lmsg.flag = command;
    strcpy(lmsg.token, TOKEN);
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);

    //接收返回信息
    int ret;
    bzero(lmsg.buf, strlen(lmsg.buf));
    ret = recvCycle(socketFd, &lmsg, MSGHEADSIZE);
    if(-1 == ret)
    {
        return -1;
    }
    ret =recvCycle(socketFd, lmsg.buf, lmsg.size - MSGHEADSIZE);
    if(-1 == ret)
    {
        return -1;
    }
    if(SUCCESS == lmsg.flag)
    {
        print(lmsg.buf);
    }else{
        printf("error:%s\n", lmsg.buf);
    }
    return 0;
}

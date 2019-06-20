#include "../include/client.h"

int download(int socketFd, char* filename){
    /*传送文件名*/
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    strcpy(lmsg.buf,filename);
    strcpy(lmsg.token, TOKEN);
    lmsg.flag = GETSCOMMEND;
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);

    /*传送一个地址，而不是一个数组。所以后面跟着就不对了。
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
    //接收文件大小,和服务器确认信息
    recvCycle(socketFd, &lmsg, MSGHEADSIZE);

#ifdef TEST_DOWNLOAD
    printf("\n\n----------------download.c 1-----------------\n");
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("%s\n", lmsg.buf);
#endif

    if(SUCCESS == lmsg.flag){
        //接收文件类型，Md5, 文件大小,offset
        char fileType[4] = {0};
        char Md5Str[MD5SIZE] = {0};
        size_t offset = 0;
        size_t fileSize = 0;
        recvCycle(socketFd, &lmsg.buf, lmsg.size - MSGHEADSIZE);
        sscanf(lmsg.buf,"%s %s %ld %ld", fileType, Md5Str, &fileSize, &offset);
        
        //打开文件
        int newFileFd = open(filename, O_CREAT| O_RDWR, 0666);
        ERROR_CHECK(newFileFd, -1, "open");
        
        //创建文件空洞，mmap映射
        ftruncate(newFileFd, fileSize);
        char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE| PROT_READ, MAP_SHARED, newFileFd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        recvCycle(socketFd, pMap+offset, fileSize-offset);
        ret = munmap(pMap, fileSize);
        ERROR_CHECK(ret, -1, "munmap");
        printf("download success\n");
        close(newFileFd);
        return -1;
    }else{
        printf("download refuse\n");
        printf("%s\n", lmsg.buf);       //服务器拒绝，并返回拒绝信息   
        return 0;
    }
}

int upload(int socketFd, char* filename){
    int ret;
    int sendFd = open(filename, O_RDONLY);
    ERROR_CHECK(sendFd, -1, "open");

    //获取文件属性
    struct stat fileStat;
    ret = fstat(sendFd, &fileStat);
    ERROR_CHECK(ret, -1, "fstat");

    char fileType[4] = {0};
    //获取文件的类型
    if(S_ISDIR(fileStat.st_mode))
    {
        fileType[0] = 'd';
    }else{
        fileType[0] = '-';
    }
    
    //获取文件md5值
    char md5Str[MD5SIZE] = {0};
    Compute_file_md5(filename, md5Str);

    /*传送文件名,文件大小，文件属性
     * 文件属性后面再做,不确定需要吗
     */
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    sprintf(lmsg.buf,"%s %s %s",fileType, filename, md5Str);
    lmsg.flag = PUTSCOMMEND;
    lmsg.fileSize = fileStat.st_size;
    strcpy(lmsg.token, TOKEN);
    lmsg.size = MSGHEADSIZE + strlen(lmsg.buf);
    send(socketFd, &lmsg, lmsg.size, 0);

#ifdef TEST_DOWNLOAD
    printf("\n\n---------------upload.c line = %d------------\n", __LINE__);
    printf("size = %d, flag = %d, fileSize = %ld\n", lmsg.size, lmsg.flag, lmsg.fileSize);
    printf("token = %s\n", lmsg.token);
    printf("filename = %s\n", lmsg.buf);
#endif

    //接收服务器返回消息
    ret = recvCycle(socketFd, &lmsg, MSGHEADSIZE);
    if(SUCCESS == lmsg.flag){
        char* pMap = (char*)mmap(NULL, lmsg.fileSize, PROT_READ, MAP_SHARED, sendFd, 0);
        send(socketFd, pMap, lmsg.fileSize, 0);
        ret = munmap(pMap, lmsg.fileSize);
        ERROR_CHECK(ret, -1, "munmap");
        close(sendFd);
        return 0;
    }else if(EXIST_FALG == lmsg.flag){
        printf("upload success!\n");
        close(sendFd);
        return 0;
    }else{
        recvCycle(socketFd, lmsg.buf, lmsg.size - MSGHEADSIZE);  //接收错误信息
        printf("upload refuse\n");
        printf("%s\n", lmsg.buf);
        close(sendFd);
        return lmsg.flag;
    }
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

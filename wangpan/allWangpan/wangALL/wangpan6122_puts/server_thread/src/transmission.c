#include "../include/factory.h"

int InsertFileInfo(MYSQL* conn, UserState_t *pUState, char* fileType, char* fileName, char* md5Str, size_t fileSize);
int checkMd5InMySQL(MYSQL *conn, const char* md5Str);

int transmiss(int tranFd, linkMsg_t *plmsg){
    int ret;

#ifdef DEBUG_SERVER
    printf("-----------transport 1 ------------\n");
    printf("size = %d, flag = %d fileSize = %ld\n", plmsg->size, plmsg->flag,plmsg->fileSize);
    printf("filename = %s\n", plmsg->buf);
#endif

    int fd = open(plmsg->buf, O_RDONLY);//只读就可以了，不需要修改
    ERROR_CHECK(fd, -1, "open");
    //send fileSize
    struct stat fileStat;
    ret = fstat(fd, &fileStat);
    ERROR_CHECK(ret, -1, "fstat");
    plmsg->fileSize = fileStat.st_size;
    ret = send(tranFd, plmsg, plmsg->size, 0);
    ERROR_CHECK(ret, -1, "send");

#ifdef DEBUG_SERVER
    printf("size = %d, flag = %d fileSize = %ld\n", plmsg->size, plmsg->flag,plmsg->fileSize);
    printf("filename = %s\n", plmsg->buf);
#endif

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
    //tranFd 是上一层的，交给上层去关
    return 0;
}

/*查找数据库user的file表中md5值，有返回0，没有返回-1*/
int checkMd5InMySQL(MYSQL *conn, const char* md5Str)
{
    char queryMd5[200]="select * from file where md5=";
    sprintf(queryMd5, "%s'%s'", queryMd5, md5Str);
    char bufTemp[300] = {0};
    queryMySQL(conn, queryMd5,bufTemp);
    if(0 == strlen(bufTemp))        //缓冲区里没东西说明没有找到
    {
        return -1;
    }else{
        return 0;
    }
}

int InsertFileInfo(MYSQL* conn, UserState_t *pUState, char* fileType, char* fileName, char* md5Str, size_t fileSize)
{
    char insertFile[300] ="insert into file(previd,creator, type,md5, clientName, fileSize) values";
    sprintf(insertFile, "%s(%s,'%s','%s','%s','%s',%ld)", insertFile, 
            pUState->currentDirId, pUState->name, fileType,md5Str,fileName,fileSize);
    printf("insert file = %s###\n", insertFile);
    char bufTemp[100];
    int ret = insertMySQL(conn, insertFile, bufTemp);
    return ret;
}

/*对应客户端上传文件功能，将文件信息写入数据库并接收文件*/
int recvFile(int sfd, MYSQL* conn,linkMsg_t* plmsg, UserState_t* pUState){
    printf("userid = %s\n userDir = %s\n", pUState->currentDirId, pUState->name);
    //接收文件名和文件md5码
    int ret;
    char fileName[255] = {0};
    char md5Str[40] = {0};
    char fileType[4] = {0};
    sscanf(plmsg->buf, "%s %s %s", fileType, fileName, md5Str);
    /*通过md5码检查数据库是否有相同文件，
     * 无论有无都要写入记录，区别在于有就不传只写
     */
    if(!checkMd5InMySQL(conn, md5Str))    //返回0，表示存在
    {
        InsertFileInfo(conn, pUState, fileType, fileName, md5Str,plmsg->fileSize);
        printf("已经存在，不必上传\n");
        plmsg->flag = EXIST_FALG;
        plmsg->size = MSGHEADSIZE;
        send(sfd, plmsg, plmsg->size, 0);
    }else{
        //发送确认传递消息，后面可以对plmsg->buf内容进行确认，不合法的
        //暂时先写一点, 断点续传的实现等等
        plmsg->flag = SUCCESS;
        plmsg->size = MSGHEADSIZE;
        send(sfd, plmsg, plmsg->size, 0);

        /*创建文件，并接收*/
        int fd = open(md5Str, O_CREAT | O_RDWR , 0666);
        ERROR_CHECK(fd, -1, "open");
        printf("创建文件成功！\n");
        if(plmsg->fileSize > BIGFILESIZE){
            ftruncate(fd, plmsg->fileSize);
            char* pMap = (char*)mmap(NULL, plmsg->fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            ERROR_CHECK(pMap, (char*)-1, "mmap");
            recvCycleCnt(sfd, pMap, plmsg->fileSize);
            close(fd);
        }else{
            int fds[2];
            pipe(fds);
            while(1)
            {
                ret = splice(sfd, NULL, fds[1], NULL, BUFSIZE, SPLICE_F_MORE | SPLICE_F_MOVE);
                printf("ret = %d\n", ret);
                if(0 == ret)
                {  
                    printf("传输完毕\n");
                    break;
                }
                splice(fds[0], NULL, fd, NULL, ret, SPLICE_F_MORE | SPLICE_F_MOVE);
            }
            ret = close(fds[1]);
            ret = close(fds[0]);
            close(fd);
        }
        /*插入文件消息*/
        printf("userid = %s\n userDir = %s\n", pUState->currentDirId, pUState->name);
        InsertFileInfo(conn, pUState, fileType, fileName, md5Str,plmsg->fileSize);
    }//if(checkMd5InMySQL);
    return 0;
}


int recvCycle(int sfd, void* buf, long fileSize){
    long total = 0;
    int ret;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        //当服务器端中断时，要即时退出
#ifdef DEBUG_SERVER
        printf("\nrecv %d B\n", ret);
        printf("recv content = %s\n", p);
#endif
        if(0 == ret){
            return -1;
        }
        total += ret;
    }
    return 0;
}

int recvCycleCnt(int sfd, void* buf, long fileSize){
    long total = 0, prev = 0;
    int ret;
    long cnt = fileSize/10000;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        //当服务器端中断时，要即时退出
#ifdef DEBUG_SERVER
        printf("\nrecv %d B\n", ret);
        printf("recv content = %s\n", p);
#endif
        if(0 == ret){
            printf("\n");
            return -1;
        }
        total += ret;
        if(total - prev > cnt){
            printf("%5.2lf%%\r", (double)total/fileSize * 100);
            fflush(stdout);
            prev = total;
        }
    }
    return 0;
}

#include "factory.h"


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

int recvFile(int sfd, linkMsg_t* plmsg){
    int ret;
    char fileName[255] = {0};
    strcpy(fileName, plmsg->buf);
    //发送确认传递消息，后面可以对plmsg->buf内容进行确认，不合法的
    //暂时先写一点
    plmsg->flag = SUCCESS;
    plmsg->size = 16;
    send(sfd, plmsg, plmsg->size, 0);

    /*创建文件，并接收*/
    int fd = open(fileName, O_CREAT | O_RDWR , 0666);
    ERROR_CHECK(fd, -1, "open");
    if(plmsg->fileSize > BIGFILESIZE){
        ftruncate(fd, plmsg->fileSize);
        char* pMap = (char*)mmap(NULL, plmsg->fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        recvCycle(sfd, pMap, plmsg->fileSize);
        close(fd);
    }else{
        int fds[2];
        pipe(fds);
        while(1){
            ret = splice(sfd, NULL, fds[1], NULL, BUFSIZE, SPLICE_F_MORE | SPLICE_F_MOVE);
            if(0 == ret){
                break;
            }
            splice(fds[0], NULL, fd, NULL, ret, SPLICE_F_MORE | SPLICE_F_MOVE);
        }
        close(fds[0]);
        close(fds[1]);
        close(fd);
    }

    return 0;
}


int recvCycle(int sfd, void* buf, long fileSize){
    long total = 0, prev = 0;
    int ret;
    long cnt = fileSize/10000;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        //当服务器端中断时，要即时退出
#ifdef DEBUG_SERVER
        printf("recv %d B\n", ret);
        printf("%s\n", p);
#endif
        if(0 == ret){
            printf("\n");
            exit(0);
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

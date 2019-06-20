#include "factory.h"


int transport(int tranFd, linkMsg_t *plmsg){
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
#ifdef DEBUG_SERVER
    printf("size = %d, flag = %d fileSize = %ld\n", plmsg->size, plmsg->flag,plmsg->fileSize);
    printf("filename = %s\n", plmsg->buf);
#endif
    ERROR_CHECK(ret, -1, "send");
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

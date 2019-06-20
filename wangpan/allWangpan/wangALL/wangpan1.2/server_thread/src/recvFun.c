#include "factory.h"

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

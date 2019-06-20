#include <func.h>

int recvCycle(int sfd, void* buf, int fileSize){
    int total = 0, ret, prev = 0;
    int cnt = fileSize/10000;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        total += ret;
        if(total - prev > cnt){
            printf("%5.2f%%\r", (double)total/fileSize * 100);
            fflush(stdout);
            prev = total;
        }
    }
    return 0;
}

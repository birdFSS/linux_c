#include <func.h>

int recvCycle(int sfd, void* buf, int fileSize){
    int total = 0, ret;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        if(0 == ret){
            return -1;
        }
        total += ret;
    }
    return 0;
}

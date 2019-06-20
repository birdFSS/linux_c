#include "client.h"

int upload(int socketFd, char* fileName){
    int fd;
    fd = open(fileName, O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    struct stat fileStat;
    int ret;
    ret = fstat(fd, &fileStat);
    ERROR_CHECK(ret, -1, "fstat");
    linkMsg_t lmsg;
    memset(&lmsg, 0, sizeof(lmsg));
    lmsg.flag = 1;
    lmsg.fileSize = fileStat.st_size;
    lmsg.buf = fileName;
    lmsg.size = strlen(lmsg.buf) + 12;
    aioWr(socketFd, (char*)&lmsg, lmsg.size);
    return 0;
}

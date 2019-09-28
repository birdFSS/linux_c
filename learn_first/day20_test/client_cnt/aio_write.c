#include "client.h"

int aioWr(int toFd, char* buf, size_t length){
    struct aiocb wr;
    bzero(&wr, sizeof(wr));

    int ret;
    //为wr.aio_buf分配空间
    wr.aio_buf = buf;
    //填充wr结构体
    wr.aio_fildes = toFd;
    wr.aio_nbytes = length;
    wr.aio_offset = 0;
    //进行异步读操作
    ret = aio_write(&wr);
    ERROR_CHECK(ret, -1, "aio_write");
    return 0;
}










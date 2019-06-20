#include "factory.h"

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

int aioRead(int fromFd, char* buf, size_t length){
    struct aiocb rd;
    bzero(&rd, sizeof(rd));
    int ret;
    //为rd.aio_buf分配空间
    rd.aio_buf = buf;
    //填充rd结构体
    rd.aio_fildes = fromFd;
    rd.aio_nbytes = length;
    rd.aio_offset = 0;
    //进行异步读操作
    ret = aio_read(&rd);
    ERROR_CHECK(ret, -1, "aio_read");
    return 0;
}
    









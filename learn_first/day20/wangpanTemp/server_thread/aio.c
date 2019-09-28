#include "factory.h"

void aio_completion_handler(sigval_t sigval){
    struct aiocb *prd;
    int ret;
    prd = (struct aiocb*)sigval.sival_ptr;
    printf("hello\n");
    ret = aio_return(prd);
    close(prd->aio_fildes);
}

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

int aioRead(int fromFd, char* fileName, size_t length){
    struct aiocb rd;
    bzero(&rd, sizeof(rd));
    int ret;
    int fd = open(fileName, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    char *buf = (char*)calloc(length, 1);
    //为rd.aio_buf分配空间
    rd.aio_buf = buf;
    //填充rd结构体
    rd.aio_fildes = fromFd;
    rd.aio_nbytes = length;
    rd.aio_offset = 0;
    rd.aio_sigevent.sigev_notify = SIGEV_THREAD;
    rd.aio_sigevent.sigev_notify_function = aio_completion_handler;
    rd.aio_sigevent.sigev_notify_attributes = NULL;
    rd.aio_sigevent.sigev_value.sival_ptr = &rd;
    //进行异步读操作
    ret = aio_read(&rd);
    ERROR_CHECK(ret, -1, "aio_read");
    return 0;
}
    









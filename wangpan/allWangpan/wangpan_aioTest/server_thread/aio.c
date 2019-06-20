#include "aio.h"

void aio_completion_handler(sigval_t sigval){
    aio_block* pAio = (aio_block*)sigval.sival_ptr;
    printf("\naio_buf= %s\n", (char*)pAio->paio->aio_buf);
    write(pAio->fd,(char*)pAio->paio->aio_buf,pAio->paio->aio_nbytes);
    close(pAio->fd);
    free((void*)pAio->paio->aio_buf);
    free(pAio->paio);
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
    while(aio_error(&wr) == EINPROGRESS){

    }
    ret = aio_return(&wr);
    printf("aio write ret = %d\n", ret);
    return 0;
}

int aioRead(int fromFd, char* fileName, size_t length){
    aio_block* aioBlock;
    aioBlock = (aio_block*)calloc(1,sizeof(aio_block));
    aioBlock->paio = (struct aiocb*)calloc(1, sizeof(struct aiocb));
    int ret;
    int fd = open(fileName, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    aioBlock->fd = fd;

    //为aioBlock->paio->aio_buf分配空间
    aioBlock->paio->aio_buf = calloc(length, 1);
    //填充aioBlock->paio->构体
    aioBlock->paio->aio_fildes = fromFd;
    aioBlock->paio->aio_nbytes = length;
    aioBlock->paio->aio_offset = 0;
    aioBlock->paio->aio_sigevent.sigev_notify = SIGEV_THREAD;
    aioBlock->paio->aio_sigevent.sigev_notify_function = aio_completion_handler;
    aioBlock->paio->aio_sigevent.sigev_notify_attributes = NULL;
    aioBlock->paio->aio_sigevent.sigev_value.sival_ptr = aioBlock;
    //进行异步读操作
    ret = aio_read(aioBlock->paio);
    ERROR_CHECK(ret, -1, "aio_read");
    return 0;
}
    









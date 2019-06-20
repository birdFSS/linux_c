#pragma once
#include "factory.h"
typedef struct aio_block{
    struct aiocb *paio;
    int fd;             //保存要写入文件的文件描述符
}aio_block;

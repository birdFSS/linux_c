#include <func.h>

int main(int argc, char* argv[])
{
    int fd;
    printf("\n");//刷新缓冲区！！！！！
    close(STDOUT_FILENO);
    fd = open("file", O_RDWR| O_TRUNC);
    ERROR_CHECK(fd, -1, "open");
    printf("fd=%d\n", fd);
    printf("stdout can't be used!!!\n");
    close(fd);
    return 0;
}


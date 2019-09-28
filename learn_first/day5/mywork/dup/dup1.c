#include <func.h>

int main(int argc, char* argv[])
{
    int fd;
    fd = open("file", O_RDWR| O_CREAT);
    int fd1;
    fd1 = dup(fd);
    printf("fd = %d\n", fd);
    close(fd);
    printf("fd1 = %d\n", fd1);
    char buf[128] = {0};
    int ret = read(fd1, buf, sizeof(buf)-1);
    ERROR_CHECK(ret, -1 , "read"); 
    printf("%s\n", buf);
    return 0;
}


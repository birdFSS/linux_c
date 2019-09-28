#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc , 2);
    int fd;
    fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd , -1 , "open");
   
    int ret = ftruncate(fd, 100);
    ERROR_CHECK(ret, -1, "ftruncate");
    char buf[100] = {0};
    strcpy(buf, "whosyourdaddy");
    ret = write(fd,buf, strlen(buf));
    ERROR_CHECK(ret, -1, "write");
    ret = read(fd, buf, sizeof(buf)-1);
    ERROR_CHECK(ret, -1, "read");
    printf("%s\n", buf);
    ret = close(fd);
    ERROR_CHECK(ret, -1, "close");
    return 0;
}


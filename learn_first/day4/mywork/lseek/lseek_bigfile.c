#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd;
    fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    printf("fd=%d\n", fd);
    int ret = lseek(fd,1024 * 1024, SEEK_SET);

    printf("lseek ret = %d\n", ret);
    char buf[128] = "hello";
    write(fd, buf, strlen(buf));
    return 0;
}

#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd;
    fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    lseek(fd, 5, SEEK_SET);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    int ret = read(fd, buf, sizeof(buf)-1);
    ERROR_CHECK(ret, -1, "read");
    printf("%s\n", buf);
    return 0;
}


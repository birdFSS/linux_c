#include <func.h>

int main(int argc, char* argv[])
{
    int fd, fd1;
    fd = open("file", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    fd1 = dup(fd);
    lseek(fd, 5, SEEK_SET);
    char buf[128] = {0};
    int ret = read(fd1, buf, sizeof(buf)-1);
    ERROR_CHECK(ret, -1, "read");
    printf("%s\n", buf);
    return 0;
}


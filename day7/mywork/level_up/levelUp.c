#include <func.h>

int main(int argc, char* argv[])
{
    int fd = open("file", O_RDWR| O_APPEND);
    ERROR_CHECK(fd, -1, "open");
    char buf[128] = {0};
    strcpy(buf, "hello");
    write(fd, buf, strlen(buf));
    return 0;
}


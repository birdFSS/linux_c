#include <func.h>

int main(int argc, char* argv[])
{
    int fd = open("file", O_RDONLY| O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    char* p=(char*)mmap(NULL, 1024 * 1024 * 1024, PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, (char*)-1, "mmap");
    int ret = munmap(p, 4096);
    ERROR_CHECK(ret, -1, "mummap");
    return 0;
}


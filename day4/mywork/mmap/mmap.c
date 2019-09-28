#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd;
    fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    printf("fd=%d\n", fd);
    char *p;
    ftruncate(fd, 1024);
    //lseek(fd, 1024, SEEK_SET);
    p = (char*)mmap(NULL,2091,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p,(char*)-1, "mmap");
    strcpy(p,"world is null");
    int ret = munmap(p,5);
    ERROR_CHECK(ret,-1,"munmap");
    return 0;
}


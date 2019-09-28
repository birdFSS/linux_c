#include <func.h>

int main(int argc, char* argv[])
{
    char *addr;
    int fd;
    struct stat sb;
    ARGS_CHECK(argc, 2);
    fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    if(fstat(fd, &sb) == -1){
        perror("fstat");
    }
    addr = (char*)mmap(NULL,sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(addr== MAP_FAILED){
        perror("mmap");
    }
    if(write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
        printf("partial/failed write");
    printf("\n");  
    char *p = addr + 4096;
    int ret = munmap(addr, 4096);
    ERROR_CHECK(ret, -1, "munmap");
    printf("%c\n", *p);
    munmap(p, 5);
    ERROR_CHECK(ret, -1, "munmap");
    return 0;
}


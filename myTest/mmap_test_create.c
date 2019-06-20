#include <func.h>

int main(int argc, char* argv[])
{
    int fd = open("file", O_RDWR | O_CREAT, 0666);
    if(-1 == fd){
        perror("open");
        return -1;
    }
    int length =4;
    char *addr =(char*)malloc(sizeof(char));
    printf("addr = %p\n", addr);
    char* pMap = (char*)mmap(addr+1, length, PROT_READ| PROT_WRITE, MAP_PRIVATE, fd, 1);
    if(MAP_FAILED == pMap){
        perror("mmap");
        return -1;
    }
    
    if(munmap(pMap, length) == -1){
        perror("munmap");
        return -1;
    }
    return 0;
}


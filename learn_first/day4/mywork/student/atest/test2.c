#include <func.h>
#define N 10
int main()
{
    int fd = open("test.txt", O_RDWR| O_CREAT);
    ERROR_CHECK(fd, -1, "open");
    char buf[1024]= "123456789DEEEFFFGGGIII";
    int num =write(fd, buf, N);
    printf("num = %d, buf = %s\n",num, buf);
    num = read(fd, buf, N);
    printf("num = %d buf = %s\n",num,  buf);
    printf("Hello world\n");
    return 0;
}


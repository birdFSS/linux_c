#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    char buf[128]={0};
    while(1){
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf)-1);
        write(fdw,buf,strlen(buf)-1);

        memset(buf,0,sizeof(buf));
        read(fdr, buf, sizeof(buf));
        printf("%s\n", buf);
    }
    return 0;
}


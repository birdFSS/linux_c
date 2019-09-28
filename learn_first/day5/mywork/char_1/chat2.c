#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    char buf[128];
    while(1){
        memset(buf,0,sizeof(buf));
        read(fdr, buf, sizeof(buf));
        printf("%s\n", buf);

        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf)-1);
        write(fdw,buf,strlen(buf)-1);
    }
    return 0;
}


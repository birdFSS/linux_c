#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    char buf[128]={0};
    printf("I am chat1, fdr=%d , fdw=%d", fdr, fdw);
    fd_set rdset;
    int fdReadyNum;
    int ret;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);
        fdReadyNum = select(fdr+1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf)-1);
            if(0 == ret){
                printf("close\n");
                break;
            }
            write(fdw,buf,strlen(buf)-1);
        }
        if(FD_ISSET(fdr, &rdset)){
            memset(buf,0,sizeof(buf));
            ret = read(fdr, buf, sizeof(buf));
            if(0==ret){
                printf("byebye\n");
                break;
            }
            printf("%s\n", buf);
        }
    }
    return 0;
}


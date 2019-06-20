#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    int res;
    char buf[128];
    fd_set rdset;
    int fdReadyNum;
    struct timeval tv;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(fdr, &rdset);
        FD_SET(STDIN_FILENO, &rdset);
        tv.tv_sec = 3;
        tv.tv_usec = 3;
        fdReadyNum = select(fdr+1,&rdset,NULL,NULL,&tv);
        if(fdReadyNum > 0){
            if(FD_ISSET(fdr, &rdset)){
                memset(buf,0,sizeof(buf));
                res = read(fdr, buf, sizeof(buf));
                if(0==res){
                    printf("byebye\n");
                    break;
                }
                printf("%s\n", buf);

            }
            if(FD_ISSET(STDIN_FILENO, &rdset)){
                memset(buf, 0, sizeof(buf));
                res =read(STDIN_FILENO, buf, sizeof(buf)-1);
                if(0==res){
                    printf("close link\n");
                    break;
                }
                write(fdw,buf,strlen(buf)-1);
            }
        }else{
            printf("timeout\n");
        }
    }
    return 0;
}


#include <func.h>
void deamon1(){
    const int MAXFD = 64;
    pid_t pid = fork();
    if(0 == pid){
        setsid();
        chdir("/");
        umask(0);
        int i = 0;
        for(;i<MAXFD;i++){
            close(i);
        }
    }else{
        exit(0);
    }
}


int main(int argc, char* argv[])
{
    deamon1();
    char buf[128] = "wohenshuai";
    int fd = open("/tmp/log", O_RDWR| O_CREAT | O_TRUNC);
    ERROR_CHECK(fd, -1, "open");
    while(1){
        write(fd,buf,strlen(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}


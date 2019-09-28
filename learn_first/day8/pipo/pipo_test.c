#include <func.h>

int main(int argc, char* argv[])
{

    int fds[2]= {0};
    pipe(fds);
    char szBuf[32] = {0};
    if(fork() == 0){
        close(fds[1]);
        //sleep(2);
        if(read(fds[0], szBuf, sizeof(szBuf)) > 0){
            puts(szBuf);
            close(fds[0]);
            exit(0);
        }
    }else{
        sleep(1);
        close(fds[0]);
        write(fds[1], "hello", 6);                                                     
        waitpid(-1, NULL, 0);
        close(fds[1]);
        exit(0);

    }
    return 0;
}


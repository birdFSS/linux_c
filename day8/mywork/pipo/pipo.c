#include <func.h>

int main(int argc, char* argv[])
{
    int fds[2] = {0};
    pipe(fds);
    if(!fork()){//子进程
        printf("I am child!\n");
        close(fds[0]);
        write(fds[1], "hello", 5);
        close(fds[1]);
        exit(0);
    }else{
        printf("I am parent\n");
        close(fds[1]);
        sleep(1);
        char buf[128] = {0};
        read(fds[0], buf, sizeof(buf));
        printf("the message from child : %s\n", buf);
        close(fds[0]);
        return 0;
    }
}


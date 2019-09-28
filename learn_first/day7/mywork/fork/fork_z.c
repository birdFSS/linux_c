#include <func.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("create error!\n");
    }else if(pid == 0){
        printf("child id = %d\n", getpid());
    }else{
        printf("parent id = %d\n", getpid());
        while(1);
    }
    return 0;
}


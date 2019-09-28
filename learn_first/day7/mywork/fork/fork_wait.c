#include <func.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("create error!\n");
    }else if(pid == 0){
        printf("child id = %d\n", getpid());
        exit(10);
    }else{
        printf("parent id = %d\n", getpid());
        int status; 
        pid = wait(&status);
        if(WIFEXITED(status)){
            printf("exit value from child is %d\n", WEXITSTATUS(status));
        }else{
            printf("child crash\n");
        }
        printf("return value from child is %d\n", pid);
    }
    return 0;
}


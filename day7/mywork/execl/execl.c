#include <func.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    pid = fork();
    if(0 == pid){
        execl("./add", "add","3", "4", NULL);
        return 0;
    }else{
        printf("wait for 1\n");
        sleep(1);
    }
    return 0;
}

